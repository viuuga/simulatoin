#include "controls/mouse/MouseInput.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

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

MouseInput::MouseInput()
{
    loadDefaultSettings();
}

void MouseInput::loadDefaultSettings()
{
    m_sensitivity = 0.0025f;
}

bool MouseInput::loadSettings(const QString &filePath)
{
    const QString resolvedPath = resolveControlsPath(filePath);
    QFile file(resolvedPath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject())
        return false;

    const QJsonObject root = doc.object();

    if (root.contains(QStringLiteral("mouseSensitivity"))) {
        m_sensitivity = static_cast<float>(root.value(QStringLiteral("mouseSensitivity")).toDouble(m_sensitivity));
        return true;
    }

    const QJsonObject mouse = root.value(QStringLiteral("mouse")).toObject();
    if (!mouse.isEmpty() && mouse.contains(QStringLiteral("sensitivity"))) {
        m_sensitivity = static_cast<float>(mouse.value(QStringLiteral("sensitivity")).toDouble(m_sensitivity));
        return true;
    }

    return false;
}

void MouseInput::processDelta(float dx, float dy, float &yawDelta, float &pitchDelta) const
{
    yawDelta = dx * m_sensitivity;
    pitchDelta = -dy * m_sensitivity;
}
