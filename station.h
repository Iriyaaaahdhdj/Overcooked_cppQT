#pragma once

#include <QGraphicsObject>

#include "gameconstants.h"

class Station : public QGraphicsObject
{
public:
    Station(StationType type, const QRectF &rect, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    StationType stationType() const;

    HeldItem counterItem() const;
    void setCounterItem(HeldItem item);
    HeldItem takeCounterItem();

    HeldItem boardItem() const;
    bool placeOnBoard(HeldItem item);
    bool chopOnce();
    HeldItem takeBoardItem();
    int chopProgress() const;
    int maxChopProgress() const;

    bool addPotIngredient(HeldItem item);
    void advancePot(qreal deltaSeconds);
    bool canPlateSoup() const;
    bool plateSoup();
    int potIngredientCount() const;
    qreal potCookProgress() const;
    bool isPotReady() const;

private:
    QColor fillColor() const;
    void drawCounterDecoration(QPainter *painter) const;
    void drawBoardDecoration(QPainter *painter) const;
    void drawPotDecoration(QPainter *painter) const;
    void drawPlateDecoration(QPainter *painter) const;
    void drawDeliveryDecoration(QPainter *painter) const;

    StationType m_type;
    QRectF m_rect;
    HeldItem m_counterItem;
    HeldItem m_boardItem;
    int m_chopProgress;
    int m_potIngredients;
    qreal m_potProgress;
    bool m_potCooking;
    bool m_potReady;
};
