#pragma once

#include <QGraphicsObject>

class TileItem : public QGraphicsObject
{
public:
    TileItem(const QRectF &rect, const QColor &fill, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QRectF m_rect;
    QColor m_fill;
};
