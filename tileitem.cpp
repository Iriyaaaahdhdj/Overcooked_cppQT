#include "tileitem.h"

#include <QPainter>

#include "gameconstants.h"

TileItem::TileItem(const QRectF &rect, const QColor &fill, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_rect(0.0, 0.0, rect.width(), rect.height())
    , m_fill(fill)
{
    setPos(rect.topLeft());
    setZValue(0.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF TileItem::boundingRect() const
{
    return m_rect;
}

void TileItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    drawRoundedCard(painter, m_rect.adjusted(2.0, 2.0, -2.0, -2.0), m_fill);
}
