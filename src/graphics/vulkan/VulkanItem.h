#pragma once

#include <QElapsedTimer>
#include <QQuickRhiItem>
#include <QTimer>

#include "controls/CameraController.h"
#include "controls/keyboard/KeyboardInput.h"
#include "controls/mouse/MouseInput.h"
#include "graphics/camera/Camera.h"

class VulkanRhiRenderer;

class VulkanItem : public QQuickRhiItem
{
    Q_OBJECT
    Q_PROPERTY(bool mouseCaptured READ mouseCaptured NOTIFY mouseCapturedChanged)

public:
    explicit VulkanItem(QQuickItem *parent = nullptr);

    bool mouseCaptured() const { return m_mouseCaptured; }
    Q_INVOKABLE void setMouseCaptured(bool captured);

    const Camera &camera() const { return m_camera; }

signals:
    void mouseCapturedChanged();

protected:
    QQuickRhiItemRenderer *createRenderer() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void itemChange(ItemChange change, const ItemChangeData &value) override;

private:
    void tick();
    void handlePointerMove();
    void syncMovementActions();
    void loadControlsSettings();

    Camera m_camera;
    CameraController m_controller;
    KeyboardInput m_keyboardInput;
    MouseInput m_mouseInput;

    bool m_mouseCaptured = false;

    QElapsedTimer m_frameTimer;
    QTimer m_tickTimer;
};

