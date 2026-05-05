#pragma once

#include <QGraphicsObject>
#include <QSet>

#include "gameconstants.h"

class Player : public QGraphicsObject
{
public:
    Player(const QString &name,
           const QColor &outfitColor,
           const PlayerControls &controls,
           QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void tick(const QSet<int> &pressedKeys, qreal deltaSeconds);

    QString name() const;
    const PlayerControls &controls() const;
    Direction facing() const;

    HeldItem heldItem() const;
    void setHeldItem(HeldItem item);
    bool isHandsEmpty() const;

private:
    bool isBlocked() const;
    void moveAxis(qreal dx, qreal dy);

    QRectF m_bounds;
    QString m_name;
    QColor m_outfitColor;
    PlayerControls m_controls;
    Direction m_facing;
    HeldItem m_heldItem;
};
