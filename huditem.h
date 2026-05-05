#pragma once

#include <QGraphicsObject>

class HudItem : public QGraphicsObject
{
public:
    explicit HudItem(qreal width, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setScore(int score);
    void setOrders(int orders);
    void setTimeLeft(qreal seconds);
    void setGameOver(bool gameOver);

private:
    QRectF m_rect;
    int m_score;
    int m_orders;
    qreal m_timeLeft;
    bool m_gameOver;
};
