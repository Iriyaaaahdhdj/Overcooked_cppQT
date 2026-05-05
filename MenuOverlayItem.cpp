#include "MenuOverlayItem.h"

#include <QPainter>
#include <QStringList>

#include "Theme.h"

namespace {
QString difficultyLabel(int languageOption, int difficultyOption)
{
    if (languageOption == MenuOverlayItem::ChineseLanguage) {
        switch (difficultyOption) {
        case MenuOverlayItem::DifficultyOne:
            return QStringLiteral("1 休闲协作");
        case MenuOverlayItem::DifficultyTwo:
            return QStringLiteral("2 限时挑战");
        case MenuOverlayItem::DifficultyThree:
        default:
            return QStringLiteral("3 限时糊锅");
        }
    }

    switch (difficultyOption) {
    case MenuOverlayItem::DifficultyOne:
        return QStringLiteral("1 Casual Co-op");
    case MenuOverlayItem::DifficultyTwo:
        return QStringLiteral("2 Timed Rush");
    case MenuOverlayItem::DifficultyThree:
    default:
        return QStringLiteral("3 Burn Risk");
    }
}

QString difficultyDescription(int languageOption, int difficultyOption)
{
    if (languageOption == MenuOverlayItem::ChineseLanguage) {
        switch (difficultyOption) {
        case MenuOverlayItem::DifficultyOne:
            return QStringLiteral("无限时，完成固定出餐数量即可通关。");
        case MenuOverlayItem::DifficultyTwo:
            return QStringLiteral("有限时，完成固定出餐数量即可通关。");
        case MenuOverlayItem::DifficultyThree:
        default:
            return QStringLiteral("有限时，锅会沸腾糊掉，需要及时出锅。");
        }
    }

    switch (difficultyOption) {
    case MenuOverlayItem::DifficultyOne:
        return QStringLiteral("No timer. Finish the delivery goal to clear.");
    case MenuOverlayItem::DifficultyTwo:
        return QStringLiteral("Timed round with a fixed delivery goal.");
    case MenuOverlayItem::DifficultyThree:
    default:
        return QStringLiteral("Timed round. Ready soup can burn if ignored.");
    }
}
}

MenuOverlayItem::MenuOverlayItem(qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_rect(0.0, 0.0, width, height)
    , m_selectedIndex(0)
    , m_languageOption(ChineseLanguage)
    , m_difficultyOption(DifficultyOne)
{
    setZValue(80.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF MenuOverlayItem::boundingRect() const
{
    return m_rect;
}

void MenuOverlayItem::setSelectedIndex(int index)
{
    if (index == m_selectedIndex) {
        return;
    }

    m_selectedIndex = index;
    update();
}

int MenuOverlayItem::selectedIndex() const
{
    return m_selectedIndex;
}

void MenuOverlayItem::setLanguageOption(int languageOption)
{
    if (m_languageOption == languageOption) {
        return;
    }

    m_languageOption = languageOption;
    update();
}

int MenuOverlayItem::languageOption() const
{
    return m_languageOption;
}

void MenuOverlayItem::setDifficultyOption(int difficultyOption)
{
    if (m_difficultyOption == difficultyOption) {
        return;
    }

    m_difficultyOption = difficultyOption;
    update();
}

int MenuOverlayItem::difficultyOption() const
{
    return m_difficultyOption;
}

void MenuOverlayItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    drawSky(painter);
    drawTruck(painter);
    drawKitchenWindow(painter);
    drawStoryPanel(painter);
    drawBottomBar(painter);
    drawBunting(painter);
    drawDecorFood(painter);
    drawMascot(painter);
}

void MenuOverlayItem::drawSky(QPainter *painter) const
{
    painter->fillRect(m_rect, Theme::skyBlue());

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255, 140));
    painter->drawEllipse(QRectF(36.0, 76.0, 220.0, 74.0));
    painter->drawEllipse(QRectF(180.0, 98.0, 118.0, 46.0));
    painter->drawEllipse(QRectF(1062.0, 66.0, 194.0, 68.0));
    painter->drawEllipse(QRectF(1120.0, 110.0, 96.0, 34.0));

    painter->setBrush(QColor(77, 180, 90));
    painter->drawEllipse(QRectF(-60.0, 536.0, 190.0, 200.0));
    painter->drawEllipse(QRectF(1126.0, 542.0, 182.0, 194.0));
}

void MenuOverlayItem::drawTruck(QPainter *painter) const
{
    painter->setPen(Theme::outlinePen());
    painter->setBrush(Theme::truckOrange());
    painter->drawRoundedRect(QRectF(24.0, 18.0, 1236.0, 652.0), 28.0, 28.0);

    painter->setBrush(Theme::truckOrangeDark());
    painter->drawRoundedRect(QRectF(232.0, 46.0, 828.0, 520.0), 24.0, 24.0);

    painter->setBrush(QColor(230, 230, 230));
    painter->drawRoundedRect(QRectF(54.0, 622.0, 360.0, 56.0), 18.0, 18.0);

    painter->setBrush(Theme::truckOrangeDark());
    painter->drawRoundedRect(QRectF(1124.0, 138.0, 112.0, 284.0), 18.0, 18.0);
    painter->setBrush(QColor(180, 235, 255));
    painter->drawRoundedRect(QRectF(1152.0, 170.0, 56.0, 154.0), 18.0, 18.0);

    painter->setBrush(Theme::truckOrangeDark());
    painter->drawRoundedRect(QRectF(238.0, 578.0, 548.0, 62.0), 18.0, 18.0);
    painter->drawRoundedRect(QRectF(284.0, 596.0, 458.0, 16.0), 8.0, 8.0);

    painter->setBrush(QColor(183, 183, 183));
    painter->drawEllipse(QRectF(120.0, 626.0, 88.0, 50.0));
    painter->setBrush(QColor(235, 175, 64));
    painter->drawEllipse(QRectF(152.0, 642.0, 24.0, 18.0));
}

void MenuOverlayItem::drawKitchenWindow(QPainter *painter) const
{
    painter->setPen(Theme::outlinePen());
    painter->setBrush(QColor(52, 52, 52));
    painter->drawRoundedRect(QRectF(238.0, 84.0, 818.0, 462.0), 18.0, 18.0);

    painter->setBrush(QColor(248, 248, 248));
    painter->drawRoundedRect(QRectF(258.0, 118.0, 778.0, 352.0), 16.0, 16.0);

    painter->setBrush(Theme::warmGray());
    painter->drawRoundedRect(QRectF(268.0, 128.0, 758.0, 332.0), 16.0, 16.0);

    painter->setBrush(QColor(158, 158, 148));
    painter->drawRoundedRect(QRectF(680.0, 166.0, 110.0, 146.0), 12.0, 12.0);
    painter->drawRoundedRect(QRectF(796.0, 166.0, 110.0, 146.0), 12.0, 12.0);
    painter->setBrush(QColor(178, 245, 255));
    painter->drawRoundedRect(QRectF(918.0, 188.0, 94.0, 112.0), 12.0, 12.0);

    painter->setBrush(QColor(176, 176, 176));
    painter->drawRoundedRect(QRectF(458.0, 176.0, 178.0, 26.0), 10.0, 10.0);

    painter->setBrush(QColor(98, 98, 98));
    painter->drawRoundedRect(QRectF(352.0, 300.0, 70.0, 18.0), 8.0, 8.0);
    painter->drawRoundedRect(QRectF(450.0, 300.0, 70.0, 18.0), 8.0, 8.0);
    painter->drawRoundedRect(QRectF(548.0, 300.0, 70.0, 18.0), 8.0, 8.0);

    painter->setBrush(QColor(124, 124, 124));
    painter->drawEllipse(QRectF(456.0, 242.0, 92.0, 58.0));
    painter->drawRoundedRect(QRectF(462.0, 278.0, 80.0, 42.0), 12.0, 12.0);
    painter->setBrush(Theme::truckOrangeDark());
    painter->drawEllipse(QRectF(476.0, 256.0, 52.0, 18.0));

    painter->setBrush(Theme::tomatoRed());
    painter->drawEllipse(QRectF(306.0, 236.0, 38.0, 38.0));
    painter->drawEllipse(QRectF(346.0, 236.0, 38.0, 38.0));
    painter->setBrush(QColor(255, 249, 236));
    painter->drawEllipse(QRectF(390.0, 236.0, 34.0, 42.0));
    painter->setBrush(Theme::leafGreen());
    painter->drawEllipse(QRectF(320.0, 226.0, 8.0, 8.0));
    painter->drawEllipse(QRectF(360.0, 226.0, 8.0, 8.0));
    painter->setBrush(QColor(247, 197, 76));
    painter->drawEllipse(QRectF(403.0, 250.0, 8.0, 8.0));

    painter->setBrush(QColor(255, 255, 255));
    painter->drawRoundedRect(QRectF(430.0, 352.0, 416.0, 54.0), 16.0, 16.0);
    Theme::text(painter, QRectF(446.0, 360.0, 384.0, 36.0), QStringLiteral("OVERCOOKED_QT"), 24, true, Theme::truckOrangeDeep());
}

void MenuOverlayItem::drawStoryPanel(QPainter *painter) const
{
    const bool zh = m_languageOption == ChineseLanguage;

    Theme::roundedBox(painter, QRectF(242.0, 130.0, 282.0, 278.0), Theme::panelBlueLight());
    Theme::roundedBox(painter, QRectF(242.0, 130.0, 282.0, 42.0), QColor(255, 173, 58));
    Theme::text(painter,
                QRectF(254.0, 136.0, 258.0, 28.0),
                zh ? QStringLiteral("游戏菜单") : QStringLiteral("Game Menu"),
                17,
                true,
                QColor(255, 255, 255));

    const QStringList labels = zh
                                   ? QStringList{QStringLiteral("语言"), QStringLiteral("难度"), QStringLiteral("开始游戏"), QStringLiteral("退出")}
                                   : QStringList{QStringLiteral("Language"), QStringLiteral("Difficulty"), QStringLiteral("Start Game"), QStringLiteral("Exit")};
    const QString languageValue = zh ? QStringLiteral("中文") : QStringLiteral("English");
    const QString difficultyValue = difficultyLabel(m_languageOption, m_difficultyOption);

    for (int i = 0; i < labels.size(); ++i) {
        const QRectF rowRect(258.0, 188.0 + i * 44.0, 250.0, 34.0);
        const QColor fill = i == m_selectedIndex ? QColor(255, 255, 255) : QColor(220, 240, 255, 150);
        Theme::roundedBox(painter, rowRect, fill);
        Theme::text(painter,
                    QRectF(rowRect.left() + 12.0, rowRect.top() + 4.0, 90.0, 24.0),
                    labels.at(i),
                    13,
                    true,
                    QColor(73, 110, 145),
                    Qt::AlignLeft | Qt::AlignVCenter);

        QString value;
        if (i == 0) {
            value = languageValue;
        } else if (i == 1) {
            value = difficultyValue;
        } else if (i == 2) {
            value = zh ? QStringLiteral("确认进入") : QStringLiteral("Confirm");
        } else {
            value = zh ? QStringLiteral("返回桌面") : QStringLiteral("Quit");
        }

        Theme::text(painter,
                    QRectF(rowRect.left() + 110.0, rowRect.top() + 4.0, 128.0, 24.0),
                    value,
                    12,
                    true,
                    QColor(52, 84, 115),
                    Qt::AlignRight | Qt::AlignVCenter);
    }

    Theme::text(painter,
                QRectF(252.0, 372.0, 262.0, 24.0),
                difficultyDescription(m_languageOption, m_difficultyOption),
                10,
                true,
                QColor(88, 130, 165));
}

void MenuOverlayItem::drawBottomBar(QPainter *painter) const
{
    const bool zh = m_languageOption == ChineseLanguage;
    Theme::roundedBox(painter, QRectF(40.0, 654.0, 724.0, 40.0), Theme::panelBlueDark());
    Theme::text(painter,
                QRectF(56.0, 660.0, 214.0, 22.0),
                zh ? QStringLiteral("W/S 选择项目") : QStringLiteral("W/S Select Row"),
                12,
                true,
                QColor(255, 255, 255));
    Theme::text(painter,
                QRectF(276.0, 660.0, 228.0, 22.0),
                zh ? QStringLiteral("A/D 切换语言或难度") : QStringLiteral("A/D Change Option"),
                12,
                true,
                QColor(255, 255, 255));
    Theme::text(painter,
                QRectF(512.0, 660.0, 120.0, 22.0),
                zh ? QStringLiteral("Enter 确认") : QStringLiteral("Enter Confirm"),
                12,
                true,
                QColor(255, 255, 255));
    Theme::text(painter,
                QRectF(636.0, 660.0, 104.0, 22.0),
                zh ? QStringLiteral("Esc 退出") : QStringLiteral("Esc Quit"),
                12,
                true,
                QColor(255, 255, 255));
}

void MenuOverlayItem::drawBunting(QPainter *painter) const
{
    painter->setPen(QPen(QColor(230, 206, 154), 4.0));
    painter->drawLine(QPointF(244.0, 538.0), QPointF(1144.0, 538.0));

    const QColor colors[] = {
        Theme::panelBlue(),
        Theme::leafGreen(),
        Theme::tomatoRed(),
        QColor(250, 214, 102),
        Theme::panelBlue()
    };

    qreal x = 300.0;
    for (const QColor &color : colors) {
        painter->setPen(Theme::outlinePen());
        painter->setBrush(color);
        QPolygonF triangle;
        triangle << QPointF(x, 540.0) << QPointF(x + 28.0, 588.0) << QPointF(x + 56.0, 540.0);
        painter->drawPolygon(triangle);
        x += 148.0;
    }
}

void MenuOverlayItem::drawDecorFood(QPainter *painter) const
{
    painter->setPen(Theme::outlinePen());
    painter->setBrush(Theme::tomatoRed());
    painter->drawEllipse(QRectF(92.0, 160.0, 58.0, 58.0));
    painter->drawEllipse(QRectF(92.0, 572.0, 56.0, 56.0));

    painter->setBrush(Theme::leafGreen());
    painter->drawEllipse(QRectF(106.0, 150.0, 10.0, 10.0));
    painter->drawEllipse(QRectF(120.0, 150.0, 10.0, 10.0));
    painter->drawEllipse(QRectF(106.0, 562.0, 10.0, 10.0));
    painter->drawEllipse(QRectF(120.0, 562.0, 10.0, 10.0));

    painter->setBrush(QColor(255, 248, 232));
    painter->drawEllipse(QRectF(94.0, 250.0, 54.0, 68.0));
    painter->setBrush(QColor(247, 197, 76));
    painter->drawEllipse(QRectF(114.0, 276.0, 14.0, 14.0));
}

void MenuOverlayItem::drawMascot(QPainter *painter) const
{
    painter->setPen(Theme::outlinePen());
    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(QRectF(642.0, 304.0, 90.0, 54.0));
    painter->drawEllipse(QRectF(666.0, 280.0, 42.0, 32.0));
    painter->setBrush(QColor(191, 140, 92));
    painter->drawEllipse(QRectF(670.0, 346.0, 66.0, 56.0));
    painter->drawEllipse(QRectF(658.0, 360.0, 16.0, 18.0));
    painter->drawEllipse(QRectF(732.0, 360.0, 16.0, 18.0));
    painter->setBrush(Theme::panelBlueDark());
    painter->drawRoundedRect(QRectF(658.0, 382.0, 102.0, 38.0), 10.0, 10.0);
}
