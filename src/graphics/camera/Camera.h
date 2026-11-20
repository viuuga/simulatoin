#pragma once

#include <QMatrix4x4>
#include <QVector3D>

class Camera
{
public:
    Camera();

    const QVector3D &position() const { return m_position; }
    float yaw() const { return m_yaw; }
    float pitch() const { return m_pitch; }

    void setPosition(const QVector3D &position);
    void setYawPitch(float yaw, float pitch);
    void rotate(float yawDelta, float pitchDelta);

    QVector3D forwardVector() const;
    QVector3D rightVector() const;
    QVector3D upVector() const;
    QVector3D flatForwardVector() const;

    void updateMovement(bool forward, bool back, bool left, bool right, bool up, bool down, float dt, float speed);

    QMatrix4x4 viewMatrix() const;

private:
    QVector3D m_position;
    float m_yaw = 0.0f;   // radians, 0 looks toward -Z
    float m_pitch = 0.0f; // radians
};
