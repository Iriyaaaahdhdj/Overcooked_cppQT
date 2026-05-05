#include "MainWindow.h"

#include <QGraphicsView>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

#include "GameScene.h"

MainWindow::MainWindow()
    : m_scene(new GameScene(this))
    , m_view(new QGraphicsView(this))
{
    setWindowTitle(QStringLiteral("Overcooked_Qt"));
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
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | QGraphicsView::DontSavePainterState);
    m_view->setCacheMode(QGraphicsView::CacheBackground);
    m_view->setStyleSheet(QStringLiteral("background:#f7e7c6; border:none;"));
    m_view->setFocus();
}
