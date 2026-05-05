#include "gamewindow.h"

#include <QHBoxLayout>

#include "gameconstants.h"
#include "gamescene.h"
#include "gameview.h"

GameWindow::GameWindow()
    : m_scene(new GameScene(this))
    , m_view(new GameView(this))
{
    setWindowTitle(QStringLiteral("Overcooked Qt6"));
    setFixedSize(kSceneWidth + 24, kSceneHeight + 24);
    setStyleSheet(QStringLiteral("background:#fff3d4;"));

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->addWidget(m_view);

    m_view->setScene(m_scene);
    m_view->setFixedSize(kSceneWidth, kSceneHeight);
    m_view->setFocus();
}
