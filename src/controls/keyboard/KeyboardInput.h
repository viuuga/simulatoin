#pragma once

#include <QHash>
#include <QSet>
#include <QString>

class QKeyEvent;

class KeyboardInput
{
public:
    KeyboardInput();

    bool loadBindings(const QString &filePath);
    void loadDefaultBindings();

    QString actionForEvent(const QKeyEvent *event) const;
    bool handleKeyPress(QKeyEvent *event);
    bool handleKeyRelease(QKeyEvent *event);
    bool isActionActive(const QString &action) const;
    void reset();

private:
    struct Binding {
        QSet<int> qtKeys;
        QSet<int> scanCodes;
    };

    bool matchesBinding(const QKeyEvent *event, const Binding &binding) const;
    static int qtKeyFromString(const QString &name);

    QHash<QString, Binding> m_bindings;
    QSet<QString> m_activeActions;
};
