#pragma once

#include <QSGRenderNode>

class VulkanNode : public QSGRenderNode
{
public:
    VulkanNode();

    void prepare() override;
    void render(const RenderState *state) override;
    RenderingFlags flags() const override;
    QRectF rect() const override;

    void setRect(const QRectF &rect);

private:
    QRectF m_rect;
};
