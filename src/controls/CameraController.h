#pragma once

#include <QString>

class Camera;

class CameraController
{
public:
    explicit CameraController(Camera *camera = nullptr);

    void setCamera(Camera *camera);
    Camera *camera() const { return m_camera; }

    void setActionActive(const QString &action, bool active);
    bool isActionActive(const QString &action) const;

    void applyLookDelta(float yawDelta, float pitchDelta);
    void update(float dt);
    void resetMovement();

    void setMoveSpeed(float speed) { m_moveSpeed = speed; }

private:
    Camera *m_camera = nullptr;
    bool m_moveForward = false;
    bool m_moveBack = false;
    bool m_moveLeft = false;
    bool m_moveRight = false;
    bool m_moveUp = false;
    bool m_moveDown = false;
    float m_moveSpeed = 2.5f;
};

