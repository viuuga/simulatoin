#include "graphics/vulkan/VulkanItem.h"

#include <rhi/qrhi.h>

#include <QCoreApplication>
#include <QCursor>
#include <QEvent>
#include <QFile>
#include <QFileInfo>
#include <QFocusEvent>
#include <QGuiApplication>
#include <QHoverEvent>
#include <QImage>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QQuickWindow>
#include <QtMath>

#include <memory>

namespace {

QShader loadShader(const QString &name)
{
    QFile f(name);
    if (f.open(QIODevice::ReadOnly))
        return QShader::fromSerialized(f.readAll());
    return QShader();
}

QString findTexturePath()
{
    const QString rel = QStringLiteral("data/textures/test_texture.png");
    const QStringList candidates = {
        rel,
        QStringLiteral("../") + rel,
        QStringLiteral("../../") + rel,
        QStringLiteral("../../../") + rel,
        QCoreApplication::applicationDirPath() + QLatin1Char('/') + rel,
        QCoreApplication::applicationDirPath() + QStringLiteral("/../") + rel,
        QCoreApplication::applicationDirPath() + QStringLiteral("/../../") + rel,
        QCoreApplication::applicationDirPath() + QStringLiteral("/../../../") + rel,
    };
    for (const QString &c : candidates) {
        if (QFileInfo::exists(c))
            return c;
    }
    return rel;
}

struct Vertex {
    float px, py, pz;
    float u, v;
};

} // namespace

class VulkanRhiRenderer : public QQuickRhiItemRenderer
{
public:
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    void ensureResources();
    void releaseResources();

    QRhi *m_rhi = nullptr;
    QRhiResourceUpdateBatch *m_pendingUpdates = nullptr;

    std::unique_ptr<QRhiBuffer> m_vbuf;
    std::unique_ptr<QRhiBuffer> m_ubuf;
    std::unique_ptr<QRhiTexture> m_texture;
    std::unique_ptr<QRhiSampler> m_sampler;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb;
    std::unique_ptr<QRhiGraphicsPipeline> m_pipeline;

    QSize m_pixelSize;
    QMatrix4x4 m_viewMatrix;
};

void VulkanRhiRenderer::initialize(QRhiCommandBuffer *cb)
{
    Q_UNUSED(cb);
    if (m_rhi != rhi()) {
        releaseResources();
        m_rhi = rhi();
    }
}

void VulkanRhiRenderer::synchronize(QQuickRhiItem *item)
{
    auto *vkItem = static_cast<VulkanItem *>(item);
    m_pixelSize = renderTarget()->pixelSize();
    m_viewMatrix = vkItem->camera().viewMatrix();
}

void VulkanRhiRenderer::ensureResources()
{
    if (m_pipeline)
        return;

    QRhi *r = rhi();
    if (!r || !renderTarget())
        return;

    // Текстура на расстоянии 2 единиц перед камерой (старт: камера в (0,0,0), взгляд на -Z)
    static const Vertex vertices[] = {
        { -0.5f, -0.5f, -2.0f,  0.0f, 1.0f },
        {  0.5f, -0.5f, -2.0f,  1.0f, 1.0f },
        {  0.5f,  0.5f, -2.0f,  1.0f, 0.0f },
        { -0.5f, -0.5f, -2.0f,  0.0f, 1.0f },
        {  0.5f,  0.5f, -2.0f,  1.0f, 0.0f },
        { -0.5f,  0.5f, -2.0f,  0.0f, 0.0f },
    };

    m_vbuf.reset(r->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer, sizeof(vertices)));
    m_vbuf->create();

    m_ubuf.reset(r->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, 64));
    m_ubuf->create();

    QImage image(findTexturePath());
    if (image.isNull())
        image = QImage(2, 2, QImage::Format_RGBA8888);
    image = image.convertToFormat(QImage::Format_RGBA8888);

    m_texture.reset(r->newTexture(QRhiTexture::RGBA8, image.size()));
    m_texture->create();

    m_sampler.reset(r->newSampler(QRhiSampler::Linear, QRhiSampler::Linear, QRhiSampler::None,
                                  QRhiSampler::ClampToEdge, QRhiSampler::ClampToEdge));
    m_sampler->create();

    m_srb.reset(r->newShaderResourceBindings());
    m_srb->setBindings({
        QRhiShaderResourceBinding::uniformBuffer(0, QRhiShaderResourceBinding::VertexStage, m_ubuf.get()),
        QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, m_texture.get(), m_sampler.get()),
    });
    m_srb->create();

    QShader vs = loadShader(QStringLiteral(":/shaders/texture.vert.qsb"));
    QShader fs = loadShader(QStringLiteral(":/shaders/texture.frag.qsb"));
    if (!vs.isValid() || !fs.isValid()) {
        vs = loadShader(QStringLiteral(":/shaders/src/graphics/vulkan/shaders/texture.vert.qsb"));
        fs = loadShader(QStringLiteral(":/shaders/src/graphics/vulkan/shaders/texture.frag.qsb"));
    }
    if (!vs.isValid() || !fs.isValid())
        return;

    m_pipeline.reset(r->newGraphicsPipeline());
    m_pipeline->setShaderStages({
        { QRhiShaderStage::Vertex, vs },
        { QRhiShaderStage::Fragment, fs },
    });

    QRhiVertexInputLayout inputLayout;
    inputLayout.setBindings({ { 5 * sizeof(float) } });
    inputLayout.setAttributes({
        { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
        { 0, 1, QRhiVertexInputAttribute::Float2, 3 * sizeof(float) },
    });
    m_pipeline->setVertexInputLayout(inputLayout);
    m_pipeline->setShaderResourceBindings(m_srb.get());
    m_pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_pipeline->setSampleCount(renderTarget()->sampleCount());
    if (!m_pipeline->create())
        m_pipeline.reset();

    QRhiResourceUpdateBatch *u = r->nextResourceUpdateBatch();
    u->uploadStaticBuffer(m_vbuf.get(), vertices);
    u->uploadTexture(m_texture.get(), image);
    m_pendingUpdates = u;
}

void VulkanRhiRenderer::releaseResources()
{
    m_pipeline.reset();
    m_srb.reset();
    m_sampler.reset();
    m_texture.reset();
    m_ubuf.reset();
    m_vbuf.reset();
    m_pendingUpdates = nullptr;
}

void VulkanRhiRenderer::render(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        releaseResources();
        m_rhi = rhi();
    }

    ensureResources();

    if (!m_pipeline) {
        const QColor clearColor = QColor::fromRgbF(0.05f, 0.05f, 0.08f, 1.0f);
        cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, m_pendingUpdates);
        m_pendingUpdates = nullptr;
        cb->endPass();
        update();
        return;
    }

    QRhiResourceUpdateBatch *u = m_pendingUpdates;
    m_pendingUpdates = nullptr;
    if (!u)
        u = rhi()->nextResourceUpdateBatch();

    const float aspect = m_pixelSize.height() > 0 ? float(m_pixelSize.width()) / float(m_pixelSize.height()) : 1.0f;
    QMatrix4x4 proj = rhi()->clipSpaceCorrMatrix();
    proj.perspective(60.0f, aspect, 0.1f, 100.0f);

    const QMatrix4x4 mvp = proj * m_viewMatrix;
    u->updateDynamicBuffer(m_ubuf.get(), 0, 64, mvp.constData());

    const QColor clearColor = QColor::fromRgbF(0.05f, 0.05f, 0.08f, 1.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, u);

    cb->setGraphicsPipeline(m_pipeline.get());
    cb->setViewport(QRhiViewport(0, 0, float(m_pixelSize.width()), float(m_pixelSize.height())));
    cb->setShaderResources();
    const QRhiCommandBuffer::VertexInput vbufBinding(m_vbuf.get(), 0);
    cb->setVertexInput(0, 1, &vbufBinding);
    cb->draw(6);

    cb->endPass();
    update();
}

VulkanItem::VulkanItem(QQuickItem *parent)
    : QQuickRhiItem(parent)
    , m_controller(&m_camera)
{
    setFlag(ItemIsFocusScope, true);
    setFocusPolicy(Qt::StrongFocus);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);

    loadControlsSettings();

    m_tickTimer.setInterval(16);
    connect(&m_tickTimer, &QTimer::timeout, this, &VulkanItem::tick);
}

QQuickRhiItemRenderer *VulkanItem::createRenderer()
{
    return new VulkanRhiRenderer();
}

void VulkanItem::loadControlsSettings()
{
    const QString controlsPath = QStringLiteral("data/options/controls/controls.json");
    m_keyboardInput.loadBindings(controlsPath);
    m_mouseInput.loadSettings(controlsPath);
}

void VulkanItem::syncMovementActions()
{
    m_controller.setActionActive(QStringLiteral("move_forward"), m_keyboardInput.isActionActive(QStringLiteral("move_forward")));
    m_controller.setActionActive(QStringLiteral("move_back"), m_keyboardInput.isActionActive(QStringLiteral("move_back")));
    m_controller.setActionActive(QStringLiteral("move_left"), m_keyboardInput.isActionActive(QStringLiteral("move_left")));
    m_controller.setActionActive(QStringLiteral("move_right"), m_keyboardInput.isActionActive(QStringLiteral("move_right")));
    m_controller.setActionActive(QStringLiteral("move_up"), m_keyboardInput.isActionActive(QStringLiteral("move_up")));
    m_controller.setActionActive(QStringLiteral("move_down"), m_keyboardInput.isActionActive(QStringLiteral("move_down")));
}

void VulkanItem::setMouseCaptured(bool captured)
{
    if (m_mouseCaptured == captured)
        return;

    m_mouseCaptured = captured;
    if (captured) {
        setKeepMouseGrab(true);
        grabMouse();
        QGuiApplication::setOverrideCursor(Qt::BlankCursor);
        if (window()) {
            window()->installEventFilter(this);
            const QPointF center(width() / 2.0, height() / 2.0);
            QCursor::setPos(mapToGlobal(center).toPoint());
        }
        setFocus(true);
        forceActiveFocus();
        m_frameTimer.restart();
        m_tickTimer.start();
    } else {
        m_tickTimer.stop();
        setKeepMouseGrab(false);
        ungrabMouse();
        if (window())
            window()->removeEventFilter(this);
        QGuiApplication::restoreOverrideCursor();
        m_keyboardInput.reset();
        m_controller.resetMovement();
    }

    emit mouseCapturedChanged();
    update();
}

void VulkanItem::keyPressEvent(QKeyEvent *event)
{
    if (m_mouseCaptured && m_keyboardInput.handleKeyPress(event)) {
        syncMovementActions();
        event->accept();
        return;
    }
    QQuickRhiItem::keyPressEvent(event);
}

void VulkanItem::keyReleaseEvent(QKeyEvent *event)
{
    if (m_mouseCaptured && m_keyboardInput.handleKeyRelease(event)) {
        syncMovementActions();
        event->accept();
        return;
    }
    QQuickRhiItem::keyReleaseEvent(event);
}

void VulkanItem::mousePressEvent(QMouseEvent *event)
{
    if (m_mouseCaptured) {
        setFocus(true);
        forceActiveFocus();
        event->accept();
        return;
    }
    QQuickRhiItem::mousePressEvent(event);
}

void VulkanItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_mouseCaptured) {
        event->accept();
        return;
    }
    QQuickRhiItem::mouseReleaseEvent(event);
}

void VulkanItem::handlePointerMove()
{
    if (!m_mouseCaptured || !window())
        return;

    const QPointF center(width() / 2.0, height() / 2.0);
    const QPointF localPos = mapFromGlobal(QCursor::pos());
    const QPointF delta = localPos - center;

    if (qAbs(delta.x()) < 0.01 && qAbs(delta.y()) < 0.01)
        return;

    float yawDelta = 0.0f;
    float pitchDelta = 0.0f;
    m_mouseInput.processDelta(float(delta.x()), float(delta.y()), yawDelta, pitchDelta);
    m_controller.applyLookDelta(yawDelta, pitchDelta);
    QCursor::setPos(mapToGlobal(center).toPoint());
    update();
}

void VulkanItem::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_mouseCaptured) {
        QQuickRhiItem::mouseMoveEvent(event);
        return;
    }

    event->accept();
}

void VulkanItem::hoverMoveEvent(QHoverEvent *event)
{
    if (!m_mouseCaptured) {
        QQuickRhiItem::hoverMoveEvent(event);
        return;
    }

    event->accept();
}

void VulkanItem::focusOutEvent(QFocusEvent *event)
{
    QQuickRhiItem::focusOutEvent(event);
}

bool VulkanItem::eventFilter(QObject *watched, QEvent *event)
{
    if (m_mouseCaptured && watched == window()) {
        switch (event->type()) {
        case QEvent::MouseMove:
        case QEvent::HoverMove:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
            handlePointerMove();
            break;
        default:
            break;
        }
    }
    return QQuickRhiItem::eventFilter(watched, event);
}

void VulkanItem::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickRhiItem::itemChange(change, value);
    if (change == ItemSceneChange) {
        if (value.window)
            value.window->installEventFilter(this);
    }
}

void VulkanItem::tick()
{
    if (!m_mouseCaptured)
        return;

    const qint64 ns = m_frameTimer.nsecsElapsed();
    m_frameTimer.restart();
    float dt = ns / 1e9f;
    if (dt < 0.0f)
        dt = 0.0f;
    if (dt > 0.1f)
        dt = 0.1f;

    m_controller.update(dt);
    update();
}

