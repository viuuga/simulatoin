#pragma once

#include <QVulkanWindow>
#include <QKeyEvent>

class VulkanWindow : public QVulkanWindow
{
    Q_OBJECT
public:
    explicit VulkanWindow(QWindow *parent = nullptr);
    ~VulkanWindow() override = default;

signals:
    void escapePressed();

protected:
    QVulkanWindowRenderer *createRenderer() override;
    void keyPressEvent(QKeyEvent *event) override;
};
