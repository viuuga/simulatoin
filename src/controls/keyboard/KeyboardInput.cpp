#include "controls/keyboard/KeyboardInput.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QMetaEnum>

namespace {

QString resolveControlsPath(const QString &filePath)
{
    if (QFileInfo::exists(filePath))
        return filePath;

    const QString rel = QStringLiteral("data/options/controls/controls.json");
    const QStringList candidates = {
        rel,
        QStringLiteral("../") + rel,
        QStringLiteral("../../") + rel,
        QStringLiteral("../../../") + rel,
        QCoreApplication::applicationDirPath() + QLatin1Char('/') + rel,
        QCoreApplication::applicationDirPath() + QStringLiteral("/../") + rel,
        QCoreApplication::applicationDirPath() + QStringLiteral("/../../") + rel,
        QCoreApplication::applicationDirPath() + QStringLiteral("/../../../") + rel,
    };
    for (const QString &c : candidates) {
        if (QFileInfo::exists(c))
            return c;
    }
    return filePath;
}

} // namespace

KeyboardInput::KeyboardInput()
{
    loadDefaultBindings();
}

void KeyboardInput::loadDefaultBindings()
{
    m_bindings.clear();

    Binding forward;
    forward.qtKeys.insert(Qt::Key_W);
    forward.scanCodes.insert(25);
    forward.scanCodes.insert(17);

    Binding back;
    back.qtKeys.insert(Qt::Key_S);
    back.scanCodes.insert(39);
    back.scanCodes.insert(31);

    Binding left;
    left.qtKeys.insert(Qt::Key_A);
    left.scanCodes.insert(38);
    left.scanCodes.insert(30);

    Binding right;
    right.qtKeys.insert(Qt::Key_D);
    right.scanCodes.insert(40);
    right.scanCodes.insert(32);

    Binding up;
    up.qtKeys.insert(Qt::Key_Space);
    up.scanCodes.insert(65); // X11
    up.scanCodes.insert(57); // evdev

    Binding down;
    down.qtKeys.insert(Qt::Key_C);
    down.scanCodes.insert(54); // X11
    down.scanCodes.insert(46); // evdev

    m_bindings.insert(QStringLiteral("move_forward"), forward);
    m_bindings.insert(QStringLiteral("move_back"), back);
    m_bindings.insert(QStringLiteral("move_left"), left);
    m_bindings.insert(QStringLiteral("move_right"), right);
    m_bindings.insert(QStringLiteral("move_up"), up);
    m_bindings.insert(QStringLiteral("move_down"), down);
}

int KeyboardInput::qtKeyFromString(const QString &name)
{
    if (name.isEmpty())
        return 0;

    const QString trimmed = name.trimmed();
    if (trimmed.size() == 1) {
        const QChar c = trimmed.at(0).toUpper();
        if (c >= QLatin1Char('A') && c <= QLatin1Char('Z'))
            return Qt::Key_A + (c.unicode() - QLatin1Char('A').unicode());
        if (c >= QLatin1Char('0') && c <= QLatin1Char('9'))
            return Qt::Key_0 + (c.unicode() - QLatin1Char('0').unicode());
    }

    QString enumName = trimmed;
    if (!enumName.startsWith(QStringLiteral("Key_")))
        enumName = QStringLiteral("Key_") + enumName;

    const QMetaEnum metaEnum = QMetaEnum::fromType<Qt::Key>();
    bool ok = false;
    const int value = metaEnum.keyToValue(enumName.toLatin1().constData(), &ok);
    return ok ? value : 0;
}

bool KeyboardInput::loadBindings(const QString &filePath)
{
    const QString resolvedPath = resolveControlsPath(filePath);
    QFile file(resolvedPath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject())
        return false;

    const QJsonObject root = doc.object();
    QJsonObject actions = root.value(QStringLiteral("actions")).toObject();
    if (actions.isEmpty())
        actions = root.value(QStringLiteral("bindings")).toObject();

    if (actions.isEmpty())
        return false;

    QHash<QString, Binding> loaded;
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        Binding binding;
        const QJsonValue val = it.value();

        if (val.isObject()) {
            const QJsonObject obj = val.toObject();
            const QJsonArray keys = obj.value(QStringLiteral("keys")).toArray();
            for (const QJsonValue &k : keys) {
                const int key = qtKeyFromString(k.toString());
                if (key != 0)
                    binding.qtKeys.insert(key);
            }
            if (obj.contains(QStringLiteral("key"))) {
                const int key = qtKeyFromString(obj.value(QStringLiteral("key")).toString());
                if (key != 0)
                    binding.qtKeys.insert(key);
            }
            const QJsonArray scanCodes = obj.value(QStringLiteral("scanCodes")).toArray();
            for (const QJsonValue &s : scanCodes)
                binding.scanCodes.insert(s.toInt());
            if (obj.contains(QStringLiteral("scanCode")))
                binding.scanCodes.insert(obj.value(QStringLiteral("scanCode")).toInt());
        } else if (val.isString()) {
            const int key = qtKeyFromString(val.toString());
            if (key != 0)
                binding.qtKeys.insert(key);
        }

        if (!binding.qtKeys.isEmpty() || !binding.scanCodes.isEmpty())
            loaded.insert(it.key(), binding);
    }

    if (loaded.isEmpty())
        return false;

    m_bindings = loaded;
    m_activeActions.clear();
    return true;
}

bool KeyboardInput::matchesBinding(const QKeyEvent *event, const Binding &binding) const
{
    if (binding.qtKeys.contains(event->key()))
        return true;
    if (binding.scanCodes.contains(event->nativeScanCode()))
        return true;
    return false;
}

QString KeyboardInput::actionForEvent(const QKeyEvent *event) const
{
    for (auto it = m_bindings.cbegin(); it != m_bindings.cend(); ++it) {
        if (matchesBinding(event, it.value()))
            return it.key();
    }
    return QString();
}

bool KeyboardInput::handleKeyPress(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return false;

    const QString action = actionForEvent(event);
    if (action.isEmpty())
        return false;

    m_activeActions.insert(action);
    return true;
}

bool KeyboardInput::handleKeyRelease(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return false;

    const QString action = actionForEvent(event);
    if (action.isEmpty())
        return false;

    m_activeActions.remove(action);
    return true;
}

bool KeyboardInput::isActionActive(const QString &action) const
{
    return m_activeActions.contains(action);
}

void KeyboardInput::reset()
{
    m_activeActions.clear();
}
