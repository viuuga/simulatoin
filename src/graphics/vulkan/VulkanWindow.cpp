#include <vulkan/vulkan.h>

#include "VulkanWindow.h"

#include <QVulkanInstance>

class VulkanRenderer : public QVulkanWindowRenderer
{
public:
    explicit VulkanRenderer(VulkanWindow *window)
        : m_window(window)
    {
    }

    void initResources() override {}
    void initSwapChainResources() override {}
    void releaseSwapChainResources() override {}
    void releaseResources() override {}

    void startNextFrame() override
    {
        VkCommandBuffer cmdBuf = m_window->currentCommandBuffer();
        const QSize sz = m_window->swapChainImageSize();

        VkClearValue clearValue{};
        clearValue.color = {{0.05f, 0.05f, 0.08f, 1.0f}};

        VkRenderPassBeginInfo rpBeginInfo{};
        rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBeginInfo.renderPass = m_window->defaultRenderPass();
        rpBeginInfo.framebuffer = m_window->currentFramebuffer();
        rpBeginInfo.renderArea.extent.width = static_cast<uint32_t>(sz.width());
        rpBeginInfo.renderArea.extent.height = static_cast<uint32_t>(sz.height());
        rpBeginInfo.clearValueCount = 1;
        rpBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdEndRenderPass(cmdBuf);

        m_window->frameReady();
        m_window->requestUpdate();
    }

private:
    VulkanWindow *m_window;
};

VulkanWindow::VulkanWindow(QWindow *parent)
    : QVulkanWindow(parent)
{
    setTitle("Voxel Engine - Vulkan");
}

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
    return new VulkanRenderer(this);
}

void VulkanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit escapePressed();
        event->accept();
    } else {
        QVulkanWindow::keyPressEvent(event);
    }
}
