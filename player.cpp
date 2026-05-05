#include "player.h"

#include <QPainter>
#include <QPainterPath>
#include <QtMath>

Player::Player(const QString &name,
               const QColor &outfitColor,
               const PlayerControls &controls,
               QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_bounds(-18.0, -29.0, 36.0, 58.0)
    , m_name(name)
    , m_outfitColor(outfitColor)
    , m_controls(controls)
    , m_facing(Direction::Down)
    , m_heldItem(HeldItem::None)
{
    setZValue(20.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF Player::boundingRect() const
{
    return m_bounds.adjusted(-12.0, -20.0, 12.0, 4.0);
}

QPainterPath Player::shape() const
{
    QPainterPath path;
    path.addRoundedRect(QRectF(-12.0, 0.0, 24.0, 18.0), 8.0, 8.0);
    return path;
}

QString Player::name() const
{
    return m_name;
}

const PlayerControls &Player::controls() const
{
    return m_controls;
}

Direction Player::facing() const
{
    return m_facing;
}

HeldItem Player::heldItem() const
{
    return m_heldItem;
}

void Player::setHeldItem(HeldItem item)
{
    m_heldItem = item;
    update();
}

bool Player::isHandsEmpty() const
{
    return m_heldItem == HeldItem::None;
}

void Player::tick(const QSet<int> &pressedKeys, qreal deltaSeconds)
{
    qreal dx = 0.0;
    qreal dy = 0.0;

    if (pressedKeys.contains(m_controls.left)) {
        dx -= 1.0;
        m_facing = Direction::Left;
    }
    if (pressedKeys.contains(m_controls.right)) {
        dx += 1.0;
        m_facing = Direction::Right;
    }
    if (pressedKeys.contains(m_controls.up)) {
        dy -= 1.0;
        m_facing = Direction::Up;
    }
    if (pressedKeys.contains(m_controls.down)) {
        dy += 1.0;
        m_facing = Direction::Down;
    }

    if (qFuzzyIsNull(dx) && qFuzzyIsNull(dy)) {
        return;
    }

    const qreal length = qSqrt(dx * dx + dy * dy);
    if (!qFuzzyIsNull(length)) {
        dx /= length;
        dy /= length;
    }

    moveAxis(dx * kPlayerSpeed * deltaSeconds, 0.0);
    moveAxis(0.0, dy * kPlayerSpeed * deltaSeconds);
}

void Player::moveAxis(qreal dx, qreal dy)
{
    if (qFuzzyIsNull(dx) && qFuzzyIsNull(dy)) {
        return;
    }

    moveBy(dx, dy);

    const QRectF limits(0.0, kHudHeight, kSceneWidth, kKitchenRows * kTileSize);
    const QRectF feetRect = mapRectToScene(QRectF(-12.0, 0.0, 24.0, 18.0));
    if (!limits.contains(feetRect)) {
        moveBy(-dx, -dy);
        return;
    }

    if (isBlocked()) {
        moveBy(-dx, -dy);
    }
}

bool Player::isBlocked() const
{
    const QList<QGraphicsItem *> collisions = collidingItems(Qt::IntersectsItemShape);
    for (QGraphicsItem *item : collisions) {
        if (item == this) {
            continue;
        }

        if (dynamic_cast<Player *>(item) != nullptr) {
            return true;
        }

        if (item->data(kDataRoleBlocking).toBool()) {
            return true;
        }
    }

    return false;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(outlinePen());

    if (m_heldItem != HeldItem::None) {
        drawRoundedCard(painter, QRectF(-18.0, -47.0, 36.0, 18.0), QColor(255, 255, 255));
        drawFoodIcon(painter, m_heldItem, QRectF(-10.0, -45.0, 20.0, 14.0));
    }

    painter->setBrush(colorChefHat());
    painter->drawEllipse(QRectF(-12.0, -28.0, 24.0, 15.0));
    painter->drawEllipse(QRectF(-6.0, -34.0, 12.0, 12.0));

    painter->setBrush(colorSkin());
    painter->drawEllipse(QRectF(-13.0, -21.0, 26.0, 22.0));

    painter->setBrush(m_outfitColor);
    painter->drawRoundedRect(QRectF(-14.0, -1.0, 28.0, 21.0), 8.0, 8.0);

    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(QRectF(-8.0, -16.0, 4.0, 4.0));
    painter->drawEllipse(QRectF(4.0, -16.0, 4.0, 4.0));

    painter->setBrush(QColor(255, 183, 197));
    painter->drawEllipse(QRectF(-12.0, -12.0, 5.0, 4.0));
    painter->drawEllipse(QRectF(7.0, -12.0, 5.0, 4.0));

    painter->setBrush(QColor(255, 205, 210));
    painter->drawRoundedRect(QRectF(-10.0, 18.0, 8.0, 8.0), 4.0, 4.0);
    painter->drawRoundedRect(QRectF(2.0, 18.0, 8.0, 8.0), 4.0, 4.0);

    const QPointF facingHint = directionVector(m_facing) * 15.0;
    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(facingHint.x() - 2.0, facingHint.y() - 2.0, 4.0, 4.0));
}
