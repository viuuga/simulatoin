#include <QGuiApplication>
#include <QQuickView>
#include <QQuickWindow>
#include <QQmlContext>
#include <QSGRendererInterface>
#include <QUrl>
#include <QtQml/qqml.h>

#include "graphics/vulkan/VulkanItem.h"

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::VulkanRhi);
    qputenv("QSG_RENDER_LOOP", "basic");
    QQuickWindow::setDefaultAlphaBuffer(true);

    QGuiApplication app(argc, argv);

    qmlRegisterType<VulkanItem>("App.Vulkan", 1, 0, "VulkanItem");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setFlags(Qt::FramelessWindowHint);
    view.setColor(Qt::transparent);
    view.setMinimumSize(QSize(640, 480));
    view.setSource(QUrl("qrc:/Main.qml"));
    view.show();

    return app.exec();
}
