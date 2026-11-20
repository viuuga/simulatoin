#include "controls/CameraController.h"

#include "graphics/camera/Camera.h"

CameraController::CameraController(Camera *camera)
    : m_camera(camera)
{
}

void CameraController::setCamera(Camera *camera)
{
    m_camera = camera;
}

void CameraController::setActionActive(const QString &action, bool active)
{
    if (action == QLatin1String("move_forward"))
        m_moveForward = active;
    else if (action == QLatin1String("move_back"))
        m_moveBack = active;
    else if (action == QLatin1String("move_left"))
        m_moveLeft = active;
    else if (action == QLatin1String("move_right"))
        m_moveRight = active;
    else if (action == QLatin1String("move_up"))
        m_moveUp = active;
    else if (action == QLatin1String("move_down"))
        m_moveDown = active;
}

bool CameraController::isActionActive(const QString &action) const
{
    if (action == QLatin1String("move_forward"))
        return m_moveForward;
    if (action == QLatin1String("move_back"))
        return m_moveBack;
    if (action == QLatin1String("move_left"))
        return m_moveLeft;
    if (action == QLatin1String("move_right"))
        return m_moveRight;
    if (action == QLatin1String("move_up"))
        return m_moveUp;
    if (action == QLatin1String("move_down"))
        return m_moveDown;
    return false;
}

void CameraController::applyLookDelta(float yawDelta, float pitchDelta)
{
    if (!m_camera)
        return;
    m_camera->rotate(yawDelta, pitchDelta);
}

void CameraController::update(float dt)
{
    if (!m_camera)
        return;
    m_camera->updateMovement(m_moveForward, m_moveBack, m_moveLeft, m_moveRight, m_moveUp, m_moveDown, dt, m_moveSpeed);
}

void CameraController::resetMovement()
{
    m_moveForward = m_moveBack = m_moveLeft = m_moveRight = false;
    m_moveUp = m_moveDown = false;
}

