#include "VulkanNode.h"

VulkanNode::VulkanNode() = default;

void VulkanNode::prepare()
{
}

void VulkanNode::render(const RenderState *state)
{
    Q_UNUSED(state);
    // Vulkan rendering (QSGRenderNode, Vulkan RHI) will be added here.
}

QSGRenderNode::RenderingFlags VulkanNode::flags() const
{
    return BoundedRectRendering;
}

QRectF VulkanNode::rect() const
{
    return m_rect;
}

void VulkanNode::setRect(const QRectF &rect)
{
    m_rect = rect;
}
