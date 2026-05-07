#include "MainWindow.h"

#include <QGraphicsView>
#include <QWheelEvent>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

#include "GameScene.h"

namespace {
class PixelSceneView final : public QGraphicsView
{
public:
    explicit PixelSceneView(QWidget *parent = nullptr)
        : QGraphicsView(parent)
        , m_zoom(1.0)
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setResizeAnchor(QGraphicsView::AnchorViewCenter);
    }

protected:
    void wheelEvent(QWheelEvent *event) override
    {
        const qreal step = event->angleDelta().y() > 0 ? 1.125 : (1.0 / 1.125);
        const qreal nextZoom = qBound(0.7, m_zoom * step, 2.8);
        if (qFuzzyCompare(nextZoom, m_zoom)) {
            event->accept();
            return;
        }

        const qreal factor = nextZoom / m_zoom;
        m_zoom = nextZoom;
        scale(factor, factor);
        event->accept();
    }

private:
    qreal m_zoom;
};
}

MainWindow::MainWindow()
    : m_scene(new GameScene(this))
    , m_view(new PixelSceneView(this))
{
    setWindowTitle(QStringLiteral("胡闹厨房 overcooked!"));
    setFixedSize(GameScene::WindowWidth, GameScene::WindowHeight);
    setMinimumSize(GameScene::WindowWidth, GameScene::WindowHeight);
    setMaximumSize(GameScene::WindowWidth, GameScene::WindowHeight);

    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_view);
    setCentralWidget(central);

    m_view->setScene(m_scene);
    m_view->setSceneRect(m_scene->sceneRect());
    m_view->setFrameShape(QGraphicsView::NoFrame);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_view->setRenderHint(QPainter::Antialiasing, false);
    m_view->setRenderHint(QPainter::TextAntialiasing, true);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform, false);
    m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    m_view->setCacheMode(QGraphicsView::CacheBackground);
    m_view->setStyleSheet(QStringLiteral("background:#ffe8f2; border:none;"));
    m_view->setFocus();
}
