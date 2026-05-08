#include "TutorialOverlayItem.h"

#include <cmath>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include "GameTypes.h"
#include "Theme.h"

namespace {
void drawArrow(QPainter *painter, const QPointF &from, const QPointF &to)
{
    painter->setPen(QPen(QColor(86, 58, 39, 82), 7.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(from + QPointF(1.0, 2.0), to + QPointF(1.0, 2.0));
    painter->setPen(QPen(QColor(255, 239, 199), 5.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(from, to);

    const QPointF direction = to - from;
    const qreal length = std::hypot(direction.x(), direction.y());
    if (length < 1.0) {
        return;
    }

    const QPointF unit(direction.x() / length, direction.y() / length);
    const QPointF normal(-unit.y(), unit.x());
    const QPointF tip = to;
    QPolygonF head;
    head << tip << tip - unit * 18.0 + normal * 10.0 << tip - unit * 18.0 - normal * 10.0;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(86, 58, 39, 82));
    painter->drawPolygon(head.translated(1.0, 2.0));
    painter->setBrush(QColor(255, 239, 199));
    painter->drawPolygon(head);
}

void drawIconCard(QPainter *painter, const QRectF &rect, const QString &step, const QString &label, const QColor &accent)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(82, 55, 34, 66));
    painter->drawRoundedRect(rect.translated(0.0, 7.0), 22.0, 22.0);

    QLinearGradient card(rect.topLeft(), rect.bottomLeft());
    card.setColorAt(0.0, QColor(255, 252, 238));
    card.setColorAt(0.58, QColor(255, 243, 215));
    card.setColorAt(1.0, QColor(248, 224, 184));
    painter->setPen(QPen(QColor(111, 77, 51), 2.0));
    painter->setBrush(card);
    painter->drawRoundedRect(rect, 22.0, 22.0);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 251, 238, 205));
    painter->drawRoundedRect(QRectF(rect.left() + 10.0, rect.top() + 12.0, rect.width() - 20.0, 60.0), 14.0, 14.0);

    painter->setBrush(QColor(accent.red(), accent.green(), accent.blue(), 48));
    painter->drawRoundedRect(QRectF(rect.left() + 10.0, rect.top() + 12.0, rect.width() - 20.0, 60.0), 14.0, 14.0);

    painter->setBrush(accent);
    painter->drawRoundedRect(QRectF(rect.left() + 20.0, rect.bottom() - 17.0, rect.width() - 40.0, 7.0), 4.0, 4.0);

    Theme::text(painter, QRectF(rect.left() + 10.0, rect.top() + 76.0, rect.width() - 20.0, 16.0), step, 7, true, QColor(151, 88, 45));
    Theme::text(painter, QRectF(rect.left() + 8.0, rect.top() + 93.0, rect.width() - 16.0, 23.0), label, 10, true, QColor(67, 48, 34));
}

void drawRiceBowl(QPainter *painter, const QRectF &rect, bool cooked)
{
    const QRectF bowl(rect.left() + 25.0, rect.top() + 29.0, rect.width() - 50.0, 37.0);
    painter->setPen(QPen(QColor(58, 46, 78), 2.0));
    painter->setBrush(cooked ? QColor(255, 255, 252) : QColor(234, 226, 206));
    painter->drawEllipse(bowl.adjusted(0.0, -14.0, 0.0, 8.0));
    painter->setBrush(QColor(162, 116, 82));
    painter->drawRoundedRect(QRectF(bowl.left() + 8.0, bowl.top() + 34.0, bowl.width() - 16.0, 20.0), 8.0, 8.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255, 170));
    painter->drawEllipse(QRectF(bowl.left() + 18.0, bowl.top() - 3.0, 13.0, 9.0));
    painter->drawEllipse(QRectF(bowl.left() + 38.0, bowl.top() - 6.0, 13.0, 9.0));
    if (cooked) {
        painter->setPen(QPen(QColor(255, 255, 255, 190), 2.0));
        painter->drawArc(QRectF(rect.left() + 42.0, rect.top() + 10.0, 20.0, 30.0), 30 * 16, 120 * 16);
        painter->drawArc(QRectF(rect.left() + 70.0, rect.top() + 8.0, 20.0, 32.0), 30 * 16, 120 * 16);
    }
}

void drawPot(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(QColor(58, 46, 78), 2.0));
    painter->setBrush(QColor(112, 122, 130));
    painter->drawEllipse(QRectF(rect.left() + 30.0, rect.top() + 26.0, rect.width() - 60.0, 20.0));
    painter->drawRoundedRect(QRectF(rect.left() + 34.0, rect.top() + 36.0, rect.width() - 68.0, 28.0), 7.0, 7.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 111, 68));
    painter->drawRect(QRectF(rect.left() + 42.0, rect.top() + 63.0, 11.0, 15.0));
    painter->setBrush(QColor(255, 218, 82));
    painter->drawRect(QRectF(rect.left() + 63.0, rect.top() + 60.0, 13.0, 18.0));
}

void drawCuttingBoard(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(QColor(98, 70, 52), 2.0));
    painter->setBrush(QColor(255, 224, 188));
    painter->drawRoundedRect(rect.adjusted(21.0, 19.0, -21.0, -60.0), 10.0, 10.0);
    drawCarryItemIcon(painter, CucumberItem, QRectF(rect.left() + 31.0, rect.top() + 32.0, 27.0, 27.0));
    drawCarryItemIcon(painter, ShrimpItem, QRectF(rect.left() + 59.0, rect.top() + 32.0, 27.0, 27.0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(238, 238, 232));
    painter->drawRect(QRectF(rect.left() + 67.0, rect.top() + 22.0, 24.0, 7.0));
    painter->setBrush(QColor(90, 62, 44));
    painter->drawRect(QRectF(rect.left() + 88.0, rect.top() + 22.0, 6.0, 23.0));
}

void drawSushiPlate(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(QColor(58, 46, 78), 2.0));
    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(QRectF(rect.left() + 23.0, rect.top() + 30.0, rect.width() - 46.0, 39.0));
    drawCarryItemIcon(painter, CucumberRollItem, QRectF(rect.left() + 32.0, rect.top() + 38.0, 24.0, 24.0));
    drawCarryItemIcon(painter, ShrimpRollItem, QRectF(rect.left() + 64.0, rect.top() + 38.0, 24.0, 24.0));
}
}

TutorialOverlayItem::TutorialOverlayItem(qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_rect(0.0, 0.0, width, height)
    , m_chinese(true)
    , m_difficultyMode(0)
{
    setZValue(160.0);
    setAcceptedMouseButtons(Qt::NoButton);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QRectF TutorialOverlayItem::boundingRect() const
{
    return m_rect;
}

void TutorialOverlayItem::setLanguageChinese(bool chinese)
{
    if (m_chinese == chinese) {
        return;
    }
    m_chinese = chinese;
    update();
}

void TutorialOverlayItem::setDifficultyMode(int difficultyMode)
{
    if (m_difficultyMode == difficultyMode) {
        return;
    }
    m_difficultyMode = difficultyMode;
    update();
}

void TutorialOverlayItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QLinearGradient overlay(0.0, 0.0, 0.0, m_rect.height());
    overlay.setColorAt(0.0, QColor(56, 38, 26, 98));
    overlay.setColorAt(0.50, QColor(76, 48, 28, 82));
    overlay.setColorAt(1.0, QColor(48, 32, 24, 104));
    painter->fillRect(m_rect, overlay);
    drawBoard(painter);
    drawRecipeFlow(painter);
    drawFooter(painter);
}

void TutorialOverlayItem::drawBoard(QPainter *painter) const
{
    const QRectF frame(72.0, 44.0, m_rect.width() - 144.0, m_rect.height() - 88.0);
    QLinearGradient boardGradient(frame.topLeft(), frame.bottomLeft());
    boardGradient.setColorAt(0.0, QColor(255, 248, 226, 205));
    boardGradient.setColorAt(0.62, QColor(249, 229, 190, 190));
    boardGradient.setColorAt(1.0, QColor(235, 201, 145, 184));

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(71, 47, 30, 44));
    painter->drawRoundedRect(frame.translated(0.0, 9.0), 28.0, 28.0);

    painter->setPen(QPen(QColor(99, 66, 43, 132), 2.0));
    painter->setBrush(boardGradient);
    painter->drawRoundedRect(frame, 28.0, 28.0);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 250, 232, 84));
    painter->drawRoundedRect(frame.adjusted(18.0, 18.0, -18.0, -436.0), 22.0, 22.0);

    Theme::text(painter,
                QRectF(frame.left() + 56.0, frame.top() + 18.0, frame.width() - 112.0, 40.0),
                m_chinese ? QStringLiteral("寿司制作流程") : QStringLiteral("Sushi Making Flow"),
                24,
                true,
                QColor(255, 248, 235));

    Theme::text(painter,
                QRectF(frame.left() + 96.0, frame.top() + 62.0, frame.width() - 192.0, 24.0),
                m_chinese ? QStringLiteral("目标：分工协作，持续完成黄瓜卷或虾寿司并送到出餐口。")
                          : QStringLiteral("Goal: cooperate, make cucumber rolls or shrimp sushi, then serve them."),
                12,
                true,
                QColor(230, 220, 208));

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 247, 226, 196));
    painter->drawRoundedRect(QRectF(frame.left() + 36.0, frame.top() + 18.0, frame.width() - 72.0, 76.0), 20.0, 20.0);
    Theme::text(painter,
                QRectF(frame.left() + 58.0, frame.top() + 24.0, frame.width() - 116.0, 30.0),
                m_chinese ? QStringLiteral("寿司制作流程") : QStringLiteral("Sushi Making Flow"),
                22,
                true,
                QColor(86, 55, 34));
    Theme::text(painter,
                QRectF(frame.left() + 96.0, frame.top() + 60.0, frame.width() - 192.0, 24.0),
                m_chinese ? QStringLiteral("目标：分工协作，持续完成黄瓜卷、鱼肉寿司或组合寿司并送到出餐口。")
                          : QStringLiteral("Goal: cooperate, make rolls or sushi, then serve them."),
                11,
                true,
                QColor(125, 82, 48));
}

void TutorialOverlayItem::drawRecipeFlow(QPainter *painter) const
{
    const QRectF cards[] = {
        QRectF(116.0, 152.0, 124.0, 132.0),
        QRectF(312.0, 152.0, 124.0, 132.0),
        QRectF(508.0, 152.0, 124.0, 132.0),
        QRectF(704.0, 152.0, 124.0, 132.0),
        QRectF(900.0, 152.0, 124.0, 132.0)
    };

    const QString labelsZh[] = {
        QStringLiteral("取生米"),
        QStringLiteral("放锅煮饭"),
        QStringLiteral("取海苔"),
        QStringLiteral("切黄瓜/虾"),
        QStringLiteral("组合并出餐")
    };
    const QString labelsEn[] = {
        QStringLiteral("Pick Rice"),
        QStringLiteral("Cook Rice"),
        QStringLiteral("Pick Nori"),
        QStringLiteral("Chop Filling"),
        QStringLiteral("Assemble & Serve")
    };
    const QColor accents[] = {
        QColor(244, 198, 126),
        QColor(255, 121, 75),
        QColor(80, 146, 98),
        QColor(239, 149, 172),
        QColor(130, 203, 238)
    };
    const QString prettyLabelsZh[] = {
        QStringLiteral("取生米"),
        QStringLiteral("放锅煮饭"),
        QStringLiteral("取海苔"),
        QStringLiteral("切黄瓜/鱼肉"),
        QStringLiteral("组合并出餐")
    };

    for (int i = 0; i < 5; ++i) {
        const QString step = m_chinese
                                 ? QStringLiteral("步骤 %1").arg(i + 1)
                                 : QStringLiteral("Step %1").arg(i + 1);
        drawIconCard(painter,
                     cards[i],
                     m_chinese ? QStringLiteral("步骤 %1").arg(i + 1) : step,
                     m_chinese ? prettyLabelsZh[i] : labelsEn[i],
                     accents[i]);
    }

    drawRiceBowl(painter, cards[0], false);
    drawPot(painter, cards[1]);
    drawCarryItemIcon(painter, NoriItem, cards[2].adjusted(42.0, 34.0, -42.0, -52.0));
    drawCuttingBoard(painter, cards[3]);
    drawSushiPlate(painter, cards[4]);

    for (int i = 0; i < 4; ++i) {
        drawArrow(painter,
                  QPointF(cards[i].right() + 18.0, cards[i].center().y() - 10.0),
                  QPointF(cards[i + 1].left() - 18.0, cards[i + 1].center().y() - 10.0));
    }

    const QRectF recipeBox(142.0, 336.0, 922.0, 148.0);
    painter->setPen(QPen(QColor(255, 240, 210), 2.0));
    painter->setBrush(QColor(80, 60, 50, 198));
    painter->drawRoundedRect(recipeBox, 16.0, 16.0);

    const QString line1 = m_chinese
                              ? QStringLiteral("配方 1：海苔 + 熟米饭 = 寿司底")
                              : QStringLiteral("Recipe 1: Nori + cooked rice = sushi base");
    const QString line2 = m_chinese
                              ? QStringLiteral("配方 2：寿司底 + 切好的黄瓜 = 黄瓜卷；寿司底 + 切好的虾 = 虾寿司")
                              : QStringLiteral("Recipe 2: base + chopped cucumber = cucumber roll; base + chopped shrimp = shrimp sushi");
    Theme::text(painter, QRectF(recipeBox.left() + 32.0, recipeBox.top() + 26.0, recipeBox.width() - 64.0, 24.0), line1, 15, true, QColor(255, 246, 222));
    Theme::text(painter, QRectF(recipeBox.left() + 32.0, recipeBox.top() + 62.0, recipeBox.width() - 64.0, 24.0), line2, 14, true, QColor(255, 246, 222));

    QString difficultyTip;
    if (m_chinese) {
        if (m_difficultyMode == 0) {
            difficultyTip = QStringLiteral("难度 1：无限时，先熟悉路线和分工。");
        } else if (m_difficultyMode == 1) {
            difficultyTip = QStringLiteral("难度 2：有限时，尽量提高出餐数量和分数。");
        } else {
            difficultyTip = QStringLiteral("难度 3：有限时且锅会糊，米饭熟了要及时取走。");
        }
    } else if (m_difficultyMode == 0) {
        difficultyTip = QStringLiteral("Difficulty 1: untimed, learn the route and teamwork.");
    } else if (m_difficultyMode == 1) {
        difficultyTip = QStringLiteral("Difficulty 2: timed, serve more sushi for a higher score.");
    } else {
        difficultyTip = QStringLiteral("Difficulty 3: timed with burning pots. Grab cooked rice quickly.");
    }

    Theme::text(painter, QRectF(recipeBox.left() + 32.0, recipeBox.top() + 104.0, recipeBox.width() - 64.0, 24.0), difficultyTip, 13, true, QColor(231, 222, 212));

    QLinearGradient recipeGradient(recipeBox.topLeft(), recipeBox.bottomLeft());
    recipeGradient.setColorAt(0.0, QColor(255, 247, 226, 242));
    recipeGradient.setColorAt(1.0, QColor(247, 222, 180, 236));
    painter->setPen(QPen(QColor(111, 77, 51, 170), 2.0));
    painter->setBrush(recipeGradient);
    painter->drawRoundedRect(recipeBox, 20.0, 20.0);

    const QString prettyLine1 = m_chinese
                                    ? QStringLiteral("配方：海苔 + 熟米饭 = 寿司底")
                                    : QStringLiteral("Recipe: nori + cooked rice = sushi base");
    const QString prettyLine2 = m_chinese
                                    ? QStringLiteral("寿司底 + 黄瓜 = 黄瓜卷；寿司底 + 鱼肉 = 鱼肉寿司；三样组合可做豪华寿司。")
                                    : QStringLiteral("Base + cucumber = cucumber roll; base + fish = fish sushi; combine all for deluxe sushi.");
    QString prettyDifficulty;
    if (m_chinese) {
        if (m_difficultyMode == 0) {
            prettyDifficulty = QStringLiteral("难度 1：无限时，先熟悉路线和分工。");
        } else if (m_difficultyMode == 1) {
            prettyDifficulty = QStringLiteral("难度 2：限时挑战，尽可能提高出餐数和得分。");
        } else {
            prettyDifficulty = QStringLiteral("难度 3：限时且会糊锅，米饭熟了要及时取走。");
        }
    } else if (m_difficultyMode == 0) {
        prettyDifficulty = QStringLiteral("Difficulty 1: untimed, learn the route and teamwork.");
    } else if (m_difficultyMode == 1) {
        prettyDifficulty = QStringLiteral("Difficulty 2: timed, serve more sushi for a higher score.");
    } else {
        prettyDifficulty = QStringLiteral("Difficulty 3: timed with burning pots. Grab cooked rice quickly.");
    }

    Theme::text(painter, QRectF(recipeBox.left() + 34.0, recipeBox.top() + 24.0, recipeBox.width() - 68.0, 24.0), prettyLine1, 13, true, QColor(75, 50, 34));
    Theme::text(painter, QRectF(recipeBox.left() + 34.0, recipeBox.top() + 60.0, recipeBox.width() - 68.0, 28.0), prettyLine2, 12, true, QColor(75, 50, 34));
    Theme::text(painter, QRectF(recipeBox.left() + 34.0, recipeBox.top() + 104.0, recipeBox.width() - 68.0, 24.0), prettyDifficulty, 12, true, QColor(147, 84, 43));
}

void TutorialOverlayItem::drawFooter(QPainter *painter) const
{
    const QRectF button(386.0, 588.0, 434.0, 54.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(82, 55, 34, 66));
    painter->drawRoundedRect(button.translated(0.0, 5.0), 18.0, 18.0);
    QLinearGradient buttonGradient(button.topLeft(), button.bottomLeft());
    buttonGradient.setColorAt(0.0, QColor(255, 185, 91));
    buttonGradient.setColorAt(1.0, QColor(218, 112, 43));
    painter->setPen(QPen(QColor(120, 72, 38), 2.0));
    painter->setBrush(buttonGradient);
    painter->drawRoundedRect(button, 18.0, 18.0);
    Theme::text(painter,
                button.adjusted(24.0, 15.0, -24.0, -13.0),
                m_chinese ? QStringLiteral("按 Enter / Space / F / / 开始")
                          : QStringLiteral("Press Enter / Space / F / / to begin"),
                14,
                true,
                QColor(255, 255, 255));
    QLinearGradient textCover(button.topLeft(), button.bottomLeft());
    textCover.setColorAt(0.0, QColor(255, 185, 91));
    textCover.setColorAt(1.0, QColor(218, 112, 43));
    painter->setPen(Qt::NoPen);
    painter->setBrush(textCover);
    painter->drawRoundedRect(button.adjusted(10.0, 8.0, -10.0, -8.0), 14.0, 14.0);
    Theme::text(painter,
                button.adjusted(24.0, 15.0, -24.0, -13.0),
                m_chinese ? QStringLiteral("按 Enter / Space / F / / 开始")
                          : QStringLiteral("Press Enter / Space / F / / to begin"),
                14,
                true,
                QColor(255, 255, 255));
}
