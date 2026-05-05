#pragma once

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QString>

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

enum class HeldItem {
    None,
    TomatoRaw,
    TomatoChopped,
    Plate,
    SoupPlated
};

enum class StationType {
    Counter,
    IngredientCrate,
    CuttingBoard,
    Pot,
    PlateCrate,
    DeliveryZone
};

struct PlayerControls {
    int up;
    int down;
    int left;
    int right;
    int interact;
};

constexpr int kTileSize = 64;
constexpr int kKitchenColumns = 14;
constexpr int kKitchenRows = 9;
constexpr int kHudHeight = 92;
constexpr int kSceneWidth = kKitchenColumns * kTileSize;
constexpr int kSceneHeight = kHudHeight + kKitchenRows * kTileSize;
constexpr int kDataRoleBlocking = 0;
constexpr qreal kPlayerSpeed = 190.0;
constexpr qreal kPotCookSeconds = 6.0;

inline QColor colorBackground()
{
    return QColor(255, 243, 212);
}

inline QColor colorFloorA()
{
    return QColor(255, 230, 179);
}

inline QColor colorFloorB()
{
    return QColor(255, 221, 153);
}

inline QColor colorCounter()
{
    return QColor(201, 223, 255);
}

inline QColor colorBoard()
{
    return QColor(255, 214, 153);
}

inline QColor colorPotStation()
{
    return QColor(185, 246, 202);
}

inline QColor colorIngredient()
{
    return QColor(255, 179, 186);
}

inline QColor colorPlateStation()
{
    return QColor(202, 231, 255);
}

inline QColor colorDelivery()
{
    return QColor(255, 241, 118);
}

inline QColor colorHudCard()
{
    return QColor(255, 255, 255);
}

inline QColor colorSoup()
{
    return QColor(255, 183, 77);
}

inline QColor colorTomato()
{
    return QColor(255, 112, 99);
}

inline QColor colorLeaf()
{
    return QColor(119, 221, 119);
}

inline QColor colorChefHat()
{
    return QColor(255, 255, 255);
}

inline QColor colorSkin()
{
    return QColor(255, 224, 189);
}

inline QPen outlinePen()
{
    QPen pen(Qt::black, 1.0);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    return pen;
}

inline QPointF directionVector(Direction direction)
{
    switch (direction) {
    case Direction::Up:
        return QPointF(0.0, -1.0);
    case Direction::Down:
        return QPointF(0.0, 1.0);
    case Direction::Left:
        return QPointF(-1.0, 0.0);
    case Direction::Right:
    default:
        return QPointF(1.0, 0.0);
    }
}

inline QString heldItemLabel(HeldItem item)
{
    switch (item) {
    case HeldItem::TomatoRaw:
        return QStringLiteral("Tomato");
    case HeldItem::TomatoChopped:
        return QStringLiteral("Chopped");
    case HeldItem::Plate:
        return QStringLiteral("Plate");
    case HeldItem::SoupPlated:
        return QStringLiteral("Soup");
    case HeldItem::None:
    default:
        return QStringLiteral("Empty");
    }
}

inline void drawRoundedCard(QPainter *painter, const QRectF &rect, const QColor &fill)
{
    painter->setPen(outlinePen());
    painter->setBrush(fill);
    painter->drawRoundedRect(rect, 8.0, 8.0);
}

inline void drawFoodIcon(QPainter *painter, HeldItem item, const QRectF &rect)
{
    painter->setPen(outlinePen());

    if (item == HeldItem::None) {
        return;
    }

    if (item == HeldItem::TomatoRaw) {
        painter->setBrush(colorTomato());
        painter->drawEllipse(rect.adjusted(4.0, 6.0, -4.0, -2.0));
        painter->setBrush(colorLeaf());
        painter->drawEllipse(QRectF(rect.center().x() - 10.0, rect.top() + 2.0, 8.0, 8.0));
        painter->drawEllipse(QRectF(rect.center().x() + 2.0, rect.top() + 2.0, 8.0, 8.0));
        painter->drawEllipse(QRectF(rect.center().x() - 4.0, rect.top(), 8.0, 8.0));
        return;
    }

    if (item == HeldItem::TomatoChopped) {
        painter->setBrush(colorTomato());
        painter->drawRoundedRect(QRectF(rect.left() + 2.0, rect.center().y() - 6.0, 12.0, 12.0), 4.0, 4.0);
        painter->drawRoundedRect(QRectF(rect.center().x() - 6.0, rect.top() + 4.0, 12.0, 12.0), 4.0, 4.0);
        painter->drawRoundedRect(QRectF(rect.right() - 14.0, rect.center().y() - 4.0, 12.0, 12.0), 4.0, 4.0);
        return;
    }

    if (item == HeldItem::Plate) {
        painter->setBrush(QColor(255, 255, 255));
        painter->drawEllipse(rect.adjusted(2.0, 5.0, -2.0, -3.0));
        painter->setBrush(QColor(225, 240, 255));
        painter->drawEllipse(rect.adjusted(8.0, 11.0, -8.0, -9.0));
        return;
    }

    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(rect.adjusted(1.0, 4.0, -1.0, -2.0));
    painter->setBrush(colorSoup());
    painter->drawEllipse(rect.adjusted(8.0, 11.0, -8.0, -9.0));
    painter->setBrush(colorTomato());
    painter->drawEllipse(QRectF(rect.center().x() - 10.0, rect.center().y() - 3.0, 7.0, 7.0));
    painter->drawEllipse(QRectF(rect.center().x() + 2.0, rect.center().y() - 1.0, 7.0, 7.0));
}
