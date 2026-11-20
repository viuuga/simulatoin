#include "graphics/camera/Camera.h"

#include <QtMath>

#include <cmath>

Camera::Camera()
    : m_position(0.0f, 0.0f, 0.0f)
{
}

void Camera::setPosition(const QVector3D &position)
{
    m_position = position;
}

void Camera::setYawPitch(float yaw, float pitch)
{
    m_yaw = yaw;
    m_pitch = pitch;
}

void Camera::rotate(float yawDelta, float pitchDelta)
{
    m_yaw += yawDelta;
    m_pitch += pitchDelta;

    const float limit = 1.55f;
    if (m_pitch > limit)
        m_pitch = limit;
    if (m_pitch < -limit)
        m_pitch = -limit;
}

QVector3D Camera::forwardVector() const
{
    const float cp = std::cos(m_pitch);
    const float sp = std::sin(m_pitch);
    const float cy = std::cos(m_yaw);
    const float sy = std::sin(m_yaw);
    return QVector3D(cp * sy, sp, -cp * cy);
}

QVector3D Camera::rightVector() const
{
    QVector3D right = QVector3D::crossProduct(forwardVector(), QVector3D(0.0f, 1.0f, 0.0f));
    if (!right.isNull())
        right.normalize();
    return right;
}

QVector3D Camera::upVector() const
{
    QVector3D up = QVector3D::crossProduct(rightVector(), forwardVector());
    if (!up.isNull())
        up.normalize();
    return up;
}

QVector3D Camera::flatForwardVector() const
{
    QVector3D flat(forwardVector().x(), 0.0f, forwardVector().z());
    if (!flat.isNull())
        flat.normalize();
    return flat;
}

void Camera::updateMovement(bool forward, bool back, bool left, bool right, bool up, bool down, float dt, float speed)
{
    const float distance = speed * dt;
    const QVector3D forwardVec = forwardVector();
    const QVector3D rightVec = rightVector();
    const QVector3D upVec = upVector();

    if (forward)
        m_position += forwardVec * distance;
    if (back)
        m_position -= forwardVec * distance;
    if (left)
        m_position -= rightVec * distance;
    if (right)
        m_position += rightVec * distance;
    if (up)
        m_position += upVec * distance;
    if (down)
        m_position -= upVec * distance;
}

QMatrix4x4 Camera::viewMatrix() const
{
    QMatrix4x4 view;
    const QVector3D pos = m_position;
    view.lookAt(pos, pos + forwardVector(), QVector3D(0.0f, 1.0f, 0.0f));
    return view;
}
