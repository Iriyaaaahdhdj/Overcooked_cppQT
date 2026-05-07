#pragma once

#include <QGraphicsItem>
#include <QString>

class HudOverlayItem : public QGraphicsItem
{
public:
    HudOverlayItem(qreal width, qreal height, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setStats(int score, int timeSeconds, int completed, int target, const QString &recipeName, bool chinese);

private:
    void drawPanel(QPainter *painter, const QRectF &rect, const QString &label, const QString &value) const;

    QRectF m_rect;
    int m_score;
    int m_timeSeconds;
    int m_completed;
    int m_target;
    QString m_recipeName;
    bool m_chinese;
};
