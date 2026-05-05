#include "gameview.h"

#include <QFrame>

#include "gameconstants.h"

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent)
{
    setFrameStyle(QFrame::NoFrame);
    setSceneRect(0, 0, kSceneWidth, kSceneHeight);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setRenderHint(QPainter::Antialiasing, true);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | QGraphicsView::DontSavePainterState);
    setCacheMode(QGraphicsView::CacheBackground);
    setBackgroundBrush(colorBackground());
    setFocusPolicy(Qt::StrongFocus);
}
