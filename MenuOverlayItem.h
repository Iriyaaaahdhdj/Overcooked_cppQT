#pragma once

#include <QGraphicsItem>

class MenuOverlayItem : public QGraphicsItem
{
public:
    enum LanguageOption {
        ChineseLanguage = 0,
        EnglishLanguage = 1
    };

    enum DifficultyOption {
        DifficultyOne = 0,
        DifficultyTwo = 1,
        DifficultyThree = 2
    };

    static constexpr int MenuCount = 4;

    MenuOverlayItem(qreal width, qreal height, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setSelectedIndex(int index);
    int selectedIndex() const;
    void setLanguageOption(int languageOption);
    int languageOption() const;
    void setDifficultyOption(int difficultyOption);
    int difficultyOption() const;

private:
    void drawSky(QPainter *painter) const;
    void drawTruck(QPainter *painter) const;
    void drawKitchenWindow(QPainter *painter) const;
    void drawStoryPanel(QPainter *painter) const;
    void drawBottomBar(QPainter *painter) const;
    void drawBunting(QPainter *painter) const;
    void drawDecorFood(QPainter *painter) const;
    void drawMascot(QPainter *painter) const;

    QRectF m_rect;
    int m_selectedIndex;
    int m_languageOption;
    int m_difficultyOption;
};
