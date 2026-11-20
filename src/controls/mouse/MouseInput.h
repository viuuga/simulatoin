#pragma once

#include <QString>

class MouseInput
{
public:
    MouseInput();

    bool loadSettings(const QString &filePath);
    void loadDefaultSettings();

    float sensitivity() const { return m_sensitivity; }
    void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }

    void processDelta(float dx, float dy, float &yawDelta, float &pitchDelta) const;

private:
    float m_sensitivity = 0.0025f;
};
