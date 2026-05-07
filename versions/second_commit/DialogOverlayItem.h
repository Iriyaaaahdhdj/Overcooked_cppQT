#pragma once

#include <QGraphicsItem>

class DialogOverlayItem : public QGraphicsItem
{
public:
    DialogOverlayItem(qreal width, qreal height, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setSelectedButton(int index);
    int selectedButton() const;

private:
    QRectF m_rect;
    int m_selectedButton;
};
