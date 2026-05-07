#pragma once

#include <QGraphicsItem>

class ResultOverlayItem : public QGraphicsItem
{
public:
    enum ActionButton {
        RestartButton = 0,
        NextLevelButton = 1
    };

    ResultOverlayItem(qreal width, qreal height, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setLanguageChinese(bool chinese);
    void setResultData(bool success,
                       int levelIndex,
                       int starCount,
                       int deliveriesCompleted,
                       int targetDeliveries,
                       int score,
                       bool nextLevelEnabled,
                       bool hasNextLevel);
    void setSelectedButton(int button);
    int selectedButton() const;

private:
    void drawStars(QPainter *painter, const QRectF &rect) const;
    void drawButton(QPainter *painter,
                    const QRectF &rect,
                    const QString &text,
                    bool selected,
                    bool enabled) const;

    QRectF m_rect;
    bool m_chinese;
    bool m_success;
    int m_levelIndex;
    int m_starCount;
    int m_deliveriesCompleted;
    int m_targetDeliveries;
    int m_score;
    bool m_nextLevelEnabled;
    bool m_hasNextLevel;
    int m_selectedButton;
};
