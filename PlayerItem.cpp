#include "PlayerItem.h"

#include <QCoreApplication>
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QtMath>

#include "GameTypes.h"
#include "KitchenItem.h"
#include "PixelArt.h"

namespace {
bool isHelloKittyCharacter(const QColor &color)
{
    return color.red() >= color.blue();
}

QImage loadNearestSprite(const QString &fileName)
{
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        QDir(appDir).absoluteFilePath(QStringLiteral("../pixel_matrices/") + fileName),
        QDir(appDir).absoluteFilePath(QStringLiteral("../../pixel_matrices/") + fileName),
        QDir(appDir).absoluteFilePath(fileName),
        QDir::current().absoluteFilePath(QStringLiteral("pixel_matrices/") + fileName)
    };

    for (const QString &path : candidates) {
        QImage image(path);
        if (!image.isNull()) {
            return image;
        }
    }

    return {};
}
}

PlayerItem::PlayerItem(const QColor &coatColor,
                       const QColor &accentColor,
                       const Controls &controls,
                       QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_visualRect(-60.0, -130.0, 120.0, 180.0)
    , m_collisionRect(-14.0, -2.0, 28.0, 30.0)
    , m_coatColor(coatColor)
    , m_accentColor(accentColor)
    , m_controls(controls)
    , m_facing(FaceDown)
    , m_carriedItem(NoItem)
{
    setZValue(4.0);
    setCacheMode(QGraphicsItem::NoCache);
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

void PlayerItem::setFacing(Facing facing)
{
    if (m_facing == facing) {
        return;
    }

    m_facing = facing;
    update();
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

    const qreal speed = 190.0;
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
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    const bool isHelloKitty = isHelloKittyCharacter(m_coatColor);

    painter->fillRect(QRectF(-14.0, 25.0, 28.0, 4.0), QColor(0, 0, 0, 46));

    if (!isHelloKitty) {
        static const QImage cinnamorollSprite = loadNearestSprite(
            QStringLiteral("cinnamoroll_candidate_65x72_transparent.png"));
        if (!cinnamorollSprite.isNull()) {
            const qreal scale = 1.28;
            const QSizeF spriteSize(cinnamorollSprite.width() * scale,
                                    cinnamorollSprite.height() * scale);
            const QRectF targetRect(-spriteSize.width() / 2.0,
                                    -spriteSize.height() + 28.0,
                                    spriteSize.width(),
                                    spriteSize.height());
            painter->drawImage(targetRect, cinnamorollSprite);
        } else {
            const QStringList sprite = PixelArt::cinnamorollChefSprite();
            const PixelArt::Palette palette = PixelArt::cinnamorollPalette();
            int maxColumns = 1;
            for (const QString &row : sprite) {
                if (row.size() > maxColumns) {
                    maxColumns = row.size();
                }
            }
            const qreal pixelSize = 0.82;
            const qreal spriteWidth = maxColumns * pixelSize;
            const qreal spriteHeight = sprite.size() * pixelSize;
            PixelArt::drawSprite(painter,
                                 sprite,
                                 palette,
                                 QPointF(-spriteWidth / 2.0, -spriteHeight + 26.0),
                                 pixelSize);
        }
    } else {
        static const QImage helloKittySprite = loadNearestSprite(
            QStringLiteral("kitty_from_new_png_sprite_masked_v2.png"));
        if (!helloKittySprite.isNull()) {
            const qreal scale = 0.78;
            const QSizeF spriteSize(helloKittySprite.width() * scale,
                                    helloKittySprite.height() * scale);
            const QRectF targetRect(-spriteSize.width() / 2.0,
                                    -spriteSize.height() + 28.0,
                                    spriteSize.width(),
                                    spriteSize.height());
            painter->drawImage(targetRect, helloKittySprite);
        } else {
            const QStringList sprite = PixelArt::helloKittyChefSprite();
            const PixelArt::Palette palette = PixelArt::helloKittyPalette();
            int maxColumns = 1;
            for (const QString &row : sprite) {
                if (row.size() > maxColumns) {
                    maxColumns = row.size();
                }
            }
            const qreal pixelSize = 1.35;
            const qreal spriteWidth = maxColumns * pixelSize;
            const qreal spriteHeight = sprite.size() * pixelSize;
            PixelArt::drawSprite(painter,
                                 sprite,
                                 palette,
                                 QPointF(-spriteWidth / 2.0, -spriteHeight + 26.0),
                                 pixelSize);
        }
    }

    if (m_carriedItem != NoItem) {
        const QRectF plateRect(-24.0, -22.0, 48.0, 24.0);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 0, 0, 42));
        painter->drawEllipse(plateRect.translated(0.0, 4.0));

        painter->setBrush(isHelloKitty ? QColor(255, 246, 248) : QColor(250, 252, 255));
        painter->drawEllipse(QRectF(-31.0, -21.0, 13.0, 17.0));
        painter->drawEllipse(QRectF(18.0, -21.0, 13.0, 17.0));

        if (isPlateCarrierItem(m_carriedItem)) {
            drawCarryItemIcon(painter, m_carriedItem, QRectF(-25.0, -30.0, 50.0, 42.0));
        } else {
            painter->setBrush(QColor(255, 252, 244));
            painter->drawEllipse(plateRect);
            painter->setBrush(QColor(226, 236, 246));
            painter->drawEllipse(plateRect.adjusted(8.0, 5.0, -8.0, -5.0));
            drawCarryItemIcon(painter, m_carriedItem, QRectF(-17.0, -28.0, 34.0, 34.0));
        }
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(54, 42, 74));
    QPointF indicator(0.0, 0.0);
    if (m_facing == FaceUp) {
        indicator = QPointF(0.0, -22.0);
    } else if (m_facing == FaceDown) {
        indicator = QPointF(0.0, 16.0);
    } else if (m_facing == FaceLeft) {
        indicator = QPointF(-18.0, -4.0);
    } else {
        indicator = QPointF(18.0, -4.0);
    }
    painter->drawRect(QRectF(indicator.x() - 2.0, indicator.y() - 2.0, 4.0, 4.0));
}
