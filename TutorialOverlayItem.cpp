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
    painter->setPen(QPen(QColor(255, 249, 230), 5.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
    painter->setBrush(QColor(255, 249, 230));
    painter->drawPolygon(head);
}

void drawIconCard(QPainter *painter, const QRectF &rect, const QString &step, const QString &label, const QColor &accent)
{
    painter->setPen(QPen(QColor(86, 66, 90), 2.0));
    painter->setBrush(QColor(255, 248, 252));
    painter->drawRoundedRect(rect, 18.0, 18.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255, 170));
    painter->drawRoundedRect(rect.adjusted(10.0, 10.0, -10.0, -70.0), 14.0, 14.0);
    painter->setBrush(accent);
    painter->drawRoundedRect(QRectF(rect.left() + 18.0, rect.bottom() - 20.0, rect.width() - 36.0, 8.0), 4.0, 4.0);
    Theme::text(painter, QRectF(rect.left() + 8.0, rect.bottom() - 62.0, rect.width() - 16.0, 20.0), step, 10, true, QColor(131, 86, 110));
    Theme::text(painter, QRectF(rect.left() + 8.0, rect.bottom() - 42.0, rect.width() - 16.0, 20.0), label, 11, true, QColor(63, 48, 75));
}

void drawRiceBowl(QPainter *painter, const QRectF &rect, bool cooked)
{
    const QRectF bowl(rect.left() + 26.0, rect.top() + 34.0, rect.width() - 52.0, 50.0);
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
    painter->drawEllipse(QRectF(rect.left() + 32.0, rect.top() + 28.0, rect.width() - 64.0, 24.0));
    painter->drawRoundedRect(QRectF(rect.left() + 36.0, rect.top() + 40.0, rect.width() - 72.0, 40.0), 8.0, 8.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 111, 68));
    painter->drawRect(QRectF(rect.left() + 44.0, rect.top() + 74.0, 14.0, 19.0));
    painter->setBrush(QColor(255, 218, 82));
    painter->drawRect(QRectF(rect.left() + 68.0, rect.top() + 70.0, 15.0, 23.0));
}

void drawCuttingBoard(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(QColor(98, 70, 52), 2.0));
    painter->setBrush(QColor(255, 224, 188));
    painter->drawRoundedRect(rect.adjusted(24.0, 20.0, -24.0, -40.0), 12.0, 12.0);
    drawCarryItemIcon(painter, CucumberItem, QRectF(rect.left() + 38.0, rect.top() + 34.0, 34.0, 34.0));
    drawCarryItemIcon(painter, ShrimpItem, QRectF(rect.left() + 70.0, rect.top() + 34.0, 34.0, 34.0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(238, 238, 232));
    painter->drawRect(QRectF(rect.left() + 78.0, rect.top() + 22.0, 28.0, 8.0));
    painter->setBrush(QColor(90, 62, 44));
    painter->drawRect(QRectF(rect.left() + 102.0, rect.top() + 22.0, 6.0, 28.0));
}

void drawSushiPlate(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(QColor(58, 46, 78), 2.0));
    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(rect.adjusted(22.0, 36.0, -22.0, -44.0));
    drawCarryItemIcon(painter, CucumberRollItem, QRectF(rect.left() + 34.0, rect.top() + 38.0, 30.0, 30.0));
    drawCarryItemIcon(painter, ShrimpRollItem, QRectF(rect.left() + 70.0, rect.top() + 38.0, 30.0, 30.0));
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
    painter->fillRect(m_rect, QColor(0, 0, 0, 142));
    drawBoard(painter);
    drawRecipeFlow(painter);
    drawFooter(painter);
}

void TutorialOverlayItem::drawBoard(QPainter *painter) const
{
    const QRectF frame(72.0, 44.0, m_rect.width() - 144.0, m_rect.height() - 88.0);
    QLinearGradient boardGradient(frame.topLeft(), frame.bottomLeft());
    boardGradient.setColorAt(0.0, QColor(48, 43, 50));
    boardGradient.setColorAt(1.0, QColor(30, 31, 36));

    painter->setPen(QPen(QColor(126, 84, 45), 8.0));
    painter->setBrush(boardGradient);
    painter->drawRoundedRect(frame, 18.0, 18.0);

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
}

void TutorialOverlayItem::drawRecipeFlow(QPainter *painter) const
{
    const QRectF cards[] = {
        QRectF(112.0, 142.0, 138.0, 138.0),
        QRectF(302.0, 142.0, 138.0, 138.0),
        QRectF(492.0, 142.0, 138.0, 138.0),
        QRectF(682.0, 142.0, 138.0, 138.0),
        QRectF(872.0, 142.0, 138.0, 138.0)
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

    for (int i = 0; i < 5; ++i) {
        const QString step = m_chinese
                                 ? QStringLiteral("步骤 %1").arg(i + 1)
                                 : QStringLiteral("Step %1").arg(i + 1);
        drawIconCard(painter, cards[i], step, m_chinese ? labelsZh[i] : labelsEn[i], accents[i]);
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
}

void TutorialOverlayItem::drawFooter(QPainter *painter) const
{
    Theme::roundedBox(painter, QRectF(386.0, 594.0, 434.0, 50.0), QColor(65, 128, 177));
    Theme::text(painter,
                QRectF(408.0, 608.0, 390.0, 22.0),
                m_chinese ? QStringLiteral("按 Enter / Space / F / / 开始")
                          : QStringLiteral("Press Enter / Space / F / / to begin"),
                14,
                true,
                QColor(255, 255, 255));
}
