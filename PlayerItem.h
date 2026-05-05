#pragma once

#include <QGraphicsItem>
#include <QPointF>
#include <QSet>

#include "GameTypes.h"

class QColor;

class PlayerItem : public QGraphicsItem
{
public:
    struct Controls {
        int up;
        int down;
        int left;
        int right;
        int interact;
    };

    enum Facing {
        FaceUp,
        FaceDown,
        FaceLeft,
        FaceRight
    };

    PlayerItem(const QColor &coatColor,
               const QColor &accentColor,
               const Controls &controls,
               QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void tick(const QSet<int> &pressedKeys, qreal deltaSeconds, const QRectF &playArea);
    const Controls &controls() const;
    void resetPosition(const QPointF &position);
    QPointF facingVector() const;
    CarryItemType carriedItem() const;
    void setCarriedItem(CarryItemType item);
    bool isHandsEmpty() const;

private:
    void moveAxis(qreal dx, qreal dy, const QRectF &playArea);
    bool collidesWithBlockingItem() const;

    QRectF m_visualRect;
    QRectF m_collisionRect;
    QColor m_coatColor;
    QColor m_accentColor;
    Controls m_controls;
    Facing m_facing;
    CarryItemType m_carriedItem;
};
