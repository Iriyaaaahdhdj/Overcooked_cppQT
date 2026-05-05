#include "PlayerItem.h"

#include <QPainter>
#include <QtMath>

#include "GameTypes.h"
#include "KitchenItem.h"
#include "Theme.h"

PlayerItem::PlayerItem(const QColor &coatColor,
                       const QColor &accentColor,
                       const Controls &controls,
                       QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_visualRect(-24.0, -68.0, 48.0, 102.0)
    , m_collisionRect(-14.0, -2.0, 28.0, 30.0)
    , m_coatColor(coatColor)
    , m_accentColor(accentColor)
    , m_controls(controls)
    , m_facing(FaceDown)
    , m_carriedItem(NoItem)
{
    setZValue(30.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF PlayerItem::boundingRect() const
{
    return m_visualRect;
}

QPainterPath PlayerItem::shape() const
{
    QPainterPath path;
    path.addRoundedRect(m_collisionRect, 8.0, 8.0);
    return path;
}

const PlayerItem::Controls &PlayerItem::controls() const
{
    return m_controls;
}

void PlayerItem::resetPosition(const QPointF &position)
{
    setPos(position);
}

QPointF PlayerItem::facingVector() const
{
    switch (m_facing) {
    case FaceUp:
        return QPointF(0.0, -1.0);
    case FaceDown:
        return QPointF(0.0, 1.0);
    case FaceLeft:
        return QPointF(-1.0, 0.0);
    case FaceRight:
    default:
        return QPointF(1.0, 0.0);
    }
}

CarryItemType PlayerItem::carriedItem() const
{
    return m_carriedItem;
}

void PlayerItem::setCarriedItem(CarryItemType item)
{
    if (m_carriedItem == item) {
        return;
    }

    m_carriedItem = item;
    update();
}

bool PlayerItem::isHandsEmpty() const
{
    return m_carriedItem == NoItem;
}

void PlayerItem::tick(const QSet<int> &pressedKeys, qreal deltaSeconds, const QRectF &playArea)
{
    qreal dx = 0.0;
    qreal dy = 0.0;

    if (pressedKeys.contains(m_controls.left)) {
        dx -= 1.0;
        m_facing = FaceLeft;
    }
    if (pressedKeys.contains(m_controls.right)) {
        dx += 1.0;
        m_facing = FaceRight;
    }
    if (pressedKeys.contains(m_controls.up)) {
        dy -= 1.0;
        m_facing = FaceUp;
    }
    if (pressedKeys.contains(m_controls.down)) {
        dy += 1.0;
        m_facing = FaceDown;
    }

    if (qFuzzyIsNull(dx) && qFuzzyIsNull(dy)) {
        return;
    }

    const qreal speed = 158.0;
    const qreal length = qSqrt(dx * dx + dy * dy);
    if (!qFuzzyIsNull(length)) {
        dx /= length;
        dy /= length;
    }

    moveAxis(dx * speed * deltaSeconds, 0.0, playArea);
    moveAxis(0.0, dy * speed * deltaSeconds, playArea);
}

void PlayerItem::moveAxis(qreal dx, qreal dy, const QRectF &playArea)
{
    if (qFuzzyIsNull(dx) && qFuzzyIsNull(dy)) {
        return;
    }

    moveBy(dx, dy);

    const QRectF feetRect = mapRectToScene(m_collisionRect);
    if (!playArea.contains(feetRect)) {
        moveBy(-dx, -dy);
        return;
    }

    if (collidesWithBlockingItem()) {
        moveBy(-dx, -dy);
    }
}

bool PlayerItem::collidesWithBlockingItem() const
{
    const QList<QGraphicsItem *> collisions = collidingItems(Qt::IntersectsItemShape);
    for (QGraphicsItem *item : collisions) {
        if (item == this) {
            continue;
        }

        if (dynamic_cast<PlayerItem *>(item) != nullptr) {
            return true;
        }

        if (auto *kitchenItem = dynamic_cast<KitchenItem *>(item)) {
            if (kitchenItem->blocksMovement()) {
                return true;
            }
        }
    }

    return false;
}

void PlayerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Theme::outlinePen());

    painter->setBrush(QColor(0, 0, 0, 30));
    painter->drawEllipse(QRectF(-14.0, 24.0, 28.0, 10.0));

    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(QRectF(-16.0, -34.0, 32.0, 18.0));
    painter->drawEllipse(QRectF(-10.0, -42.0, 20.0, 16.0));
    painter->setBrush(QColor(210, 236, 255));
    painter->drawRoundedRect(QRectF(-14.0, -24.0, 28.0, 7.0), 3.0, 3.0);

    painter->setBrush(QColor(255, 224, 189));
    painter->drawEllipse(QRectF(-15.0, -27.0, 30.0, 24.0));

    painter->setBrush(m_coatColor);
    painter->drawRoundedRect(QRectF(-17.0, -3.0, 34.0, 26.0), 10.0, 10.0);
    painter->setBrush(QColor(245, 245, 245));
    painter->drawRoundedRect(QRectF(-11.0, 0.0, 22.0, 18.0), 5.0, 5.0);
    painter->setBrush(m_accentColor);
    painter->drawRoundedRect(QRectF(-7.0, 4.0, 14.0, 9.0), 4.0, 4.0);
    painter->drawEllipse(QRectF(-1.5, 0.5, 3.0, 3.0));
    painter->drawEllipse(QRectF(-1.5, 6.5, 3.0, 3.0));
    painter->drawEllipse(QRectF(-1.5, 12.5, 3.0, 3.0));

    painter->setBrush(QColor(255, 224, 189));
    painter->drawEllipse(QRectF(-20.0, -6.0, 8.0, 13.0));
    painter->drawEllipse(QRectF(12.0, -6.0, 8.0, 13.0));

    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(QRectF(-9.0, -18.0, 4.0, 4.0));
    painter->drawEllipse(QRectF(5.0, -18.0, 4.0, 4.0));

    painter->setBrush(QColor(255, 181, 195));
    painter->drawEllipse(QRectF(-13.0, -12.0, 5.0, 4.0));
    painter->drawEllipse(QRectF(8.0, -12.0, 5.0, 4.0));

    painter->setBrush(m_coatColor.darker(105));
    painter->drawRoundedRect(QRectF(-12.0, 23.0, 9.0, 8.0), 4.0, 4.0);
    painter->drawRoundedRect(QRectF(3.0, 23.0, 9.0, 8.0), 4.0, 4.0);

    painter->setBrush(Qt::black);
    QPointF indicator(0.0, 0.0);
    if (m_facing == FaceUp) {
        indicator = QPointF(0.0, -22.0);
    } else if (m_facing == FaceDown) {
        indicator = QPointF(0.0, 18.0);
    } else if (m_facing == FaceLeft) {
        indicator = QPointF(-18.0, 0.0);
    } else {
        indicator = QPointF(18.0, 0.0);
    }
    painter->drawEllipse(QRectF(indicator.x() - 2.0, indicator.y() - 2.0, 4.0, 4.0));

    drawCarryItemIcon(painter, m_carriedItem, QRectF(-11.0, -67.0, 22.0, 22.0));
}
