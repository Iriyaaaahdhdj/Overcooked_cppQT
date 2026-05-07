#pragma once

#include <QGraphicsItem>

class TutorialOverlayItem : public QGraphicsItem
{
public:
    TutorialOverlayItem(qreal width, qreal height, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setLanguageChinese(bool chinese);
    void setDifficultyMode(int difficultyMode);

private:
    void drawBoard(QPainter *painter) const;
    void drawRecipeFlow(QPainter *painter) const;
    void drawFooter(QPainter *painter) const;

    QRectF m_rect;
    bool m_chinese;
    int m_difficultyMode;
};
