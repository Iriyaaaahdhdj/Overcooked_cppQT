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
    painter->setPen(QPen(QColor(245, 245, 245), 5.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(from, to);

    const QPointF direction = to - from;
    const qreal length = std::hypot(direction.x(), direction.y());
    if (length < 1.0) {
        return;
    }

    const QPointF unit(direction.x() / length, direction.y() / length);
    const QPointF normal(-unit.y(), unit.x());
    const QPointF tip = to;
    const QPointF left = tip - unit * 18.0 + normal * 10.0;
    const QPointF right = tip - unit * 18.0 - normal * 10.0;

    painter->setBrush(QColor(245, 245, 245));
    QPolygonF head;
    head << tip << left << right;
    painter->drawPolygon(head);
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

    painter->fillRect(m_rect, QColor(0, 0, 0, 158));
    drawBoard(painter);
    drawRecipeFlow(painter);
    drawFooter(painter);
}

void TutorialOverlayItem::drawBoard(QPainter *painter) const
{
    const QRectF frame(90.0, 58.0, m_rect.width() - 180.0, m_rect.height() - 116.0);
    painter->setPen(QPen(QColor(82, 52, 24), 10.0));
    painter->setBrush(QColor(30, 31, 33));
    painter->drawRoundedRect(frame, 10.0, 10.0);

    Theme::text(painter,
                QRectF(frame.left() + 60.0, frame.top() + 28.0, frame.width() - 120.0, 54.0),
                m_chinese ? QStringLiteral("番茄鸡蛋汤教学") : QStringLiteral("Tomato Egg Soup Tutorial"),
                24,
                true,
                QColor(250, 248, 243));

    painter->setPen(QPen(QColor(238, 238, 238, 180), 4.0, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(QPointF(frame.left() + 140.0, frame.top() + 102.0),
                      QPointF(frame.right() - 140.0, frame.top() + 102.0));
}

void TutorialOverlayItem::drawRecipeFlow(QPainter *painter) const
{
    const QRectF icon1(170.0, 232.0, 120.0, 120.0);
    const QRectF icon2(378.0, 232.0, 120.0, 120.0);
    const QRectF icon3(586.0, 232.0, 120.0, 120.0);
    const QRectF icon4(794.0, 232.0, 120.0, 120.0);
    const QRectF icon5(1002.0, 232.0, 120.0, 120.0);

    painter->setPen(QPen(QColor(238, 238, 238), 3.0));
    painter->setBrush(QColor(74, 54, 45));
    painter->drawRoundedRect(icon1, 18.0, 18.0);
    painter->setBrush(QColor(255, 255, 255));
    painter->drawRoundedRect(icon1.adjusted(14.0, 18.0, -14.0, -18.0), 12.0, 12.0);
    painter->setBrush(QColor(217, 81, 65));
    painter->drawEllipse(icon1.adjusted(26.0, 30.0, -56.0, -56.0));
    painter->drawEllipse(icon1.adjusted(54.0, 30.0, -28.0, -56.0));
    painter->drawEllipse(icon1.adjusted(40.0, 56.0, -42.0, -30.0));
    painter->setBrush(Theme::leafGreen());
    painter->drawEllipse(QRectF(icon1.left() + 46.0, icon1.top() + 30.0, 12.0, 12.0));
    painter->drawEllipse(QRectF(icon1.left() + 74.0, icon1.top() + 30.0, 12.0, 12.0));

    painter->setBrush(QColor(255, 232, 196));
    painter->drawRoundedRect(icon2, 18.0, 18.0);
    painter->setBrush(QColor(254, 248, 233));
    painter->drawRoundedRect(icon2.adjusted(16.0, 18.0, -16.0, -32.0), 12.0, 12.0);
    painter->setBrush(QColor(136, 110, 82));
    painter->drawRoundedRect(QRectF(icon2.left() + 24.0, icon2.top() + 84.0, 72.0, 18.0), 8.0, 8.0);
    painter->setBrush(QColor(217, 81, 65));
    painter->drawRoundedRect(QRectF(icon2.left() + 36.0, icon2.top() + 46.0, 18.0, 18.0), 4.0, 4.0);
    painter->drawRoundedRect(QRectF(icon2.left() + 58.0, icon2.top() + 42.0, 18.0, 18.0), 4.0, 4.0);
    painter->drawRoundedRect(QRectF(icon2.left() + 48.0, icon2.top() + 64.0, 18.0, 18.0), 4.0, 4.0);

    painter->setBrush(QColor(176, 126, 84));
    painter->drawRoundedRect(icon3, 18.0, 18.0);
    painter->setBrush(QColor(255, 248, 231));
    painter->drawEllipse(QRectF(icon3.left() + 22.0, icon3.top() + 26.0, 30.0, 40.0));
    painter->drawEllipse(QRectF(icon3.left() + 50.0, icon3.top() + 26.0, 30.0, 40.0));
    painter->setBrush(QColor(247, 197, 76));
    painter->drawEllipse(QRectF(icon3.left() + 33.0, icon3.top() + 41.0, 8.0, 8.0));
    painter->drawEllipse(QRectF(icon3.left() + 61.0, icon3.top() + 41.0, 8.0, 8.0));
    Theme::text(painter,
                QRectF(icon3.left() + 10.0, icon3.top() + 82.0, 100.0, 18.0),
                QStringLiteral("1x"),
                18,
                true,
                QColor(255, 244, 204));

    painter->setBrush(QColor(100, 105, 115));
    painter->drawRoundedRect(icon4, 18.0, 18.0);
    painter->setBrush(QColor(212, 222, 232));
    painter->drawRoundedRect(icon4.adjusted(18.0, 22.0, -18.0, -36.0), 16.0, 16.0);
    painter->setBrush(QColor(72, 74, 80));
    painter->drawEllipse(QRectF(icon4.left() + 24.0, icon4.top() + 24.0, 72.0, 56.0));
    painter->setBrush(QColor(217, 81, 65));
    painter->drawRoundedRect(QRectF(icon4.left() + 30.0, icon4.top() + 42.0, 14.0, 14.0), 3.0, 3.0);
    painter->drawRoundedRect(QRectF(icon4.left() + 48.0, icon4.top() + 34.0, 14.0, 14.0), 3.0, 3.0);
    painter->setBrush(QColor(255, 248, 231));
    painter->drawEllipse(QRectF(icon4.left() + 64.0, icon4.top() + 40.0, 16.0, 20.0));
    painter->setBrush(QColor(247, 197, 76));
    painter->drawEllipse(QRectF(icon4.left() + 69.0, icon4.top() + 47.0, 6.0, 6.0));

    painter->setBrush(QColor(255, 238, 160));
    painter->drawRoundedRect(icon5, 18.0, 18.0);
    painter->setBrush(QColor(255, 255, 255));
    painter->drawRoundedRect(icon5.adjusted(22.0, 24.0, -22.0, -42.0), 12.0, 12.0);
    painter->setBrush(QColor(255, 194, 101));
    painter->drawEllipse(QRectF(icon5.left() + 40.0, icon5.top() + 38.0, 40.0, 40.0));
    Theme::text(painter,
                QRectF(icon5.left() + 14.0, icon5.top() + 86.0, 92.0, 18.0),
                QStringLiteral("+20"),
                16,
                true,
                QColor(98, 62, 24));

    drawArrow(painter, QPointF(icon1.right() + 16.0, icon1.center().y()), QPointF(icon2.left() - 16.0, icon2.center().y()));
    drawArrow(painter, QPointF(icon2.right() + 16.0, icon2.center().y()), QPointF(icon3.left() - 16.0, icon3.center().y()));
    drawArrow(painter, QPointF(icon3.right() + 16.0, icon3.center().y()), QPointF(icon4.left() - 16.0, icon4.center().y()));
    drawArrow(painter, QPointF(icon4.right() + 16.0, icon4.center().y()), QPointF(icon5.left() - 16.0, icon5.center().y()));

    Theme::text(painter, QRectF(icon1.left() - 14.0, 372.0, 148.0, 24.0), m_chinese ? QStringLiteral("1. 取番茄") : QStringLiteral("1. Pick Tomato"), 13, true, QColor(248, 248, 248));
    Theme::text(painter, QRectF(icon2.left() - 14.0, 372.0, 148.0, 24.0), m_chinese ? QStringLiteral("2. 切番茄") : QStringLiteral("2. Chop Tomato"), 13, true, QColor(248, 248, 248));
    Theme::text(painter, QRectF(icon3.left() - 14.0, 372.0, 148.0, 24.0), m_chinese ? QStringLiteral("3. 取鸡蛋") : QStringLiteral("3. Pick Egg"), 13, true, QColor(248, 248, 248));
    Theme::text(painter, QRectF(icon4.left() - 14.0, 372.0, 148.0, 24.0), m_chinese ? QStringLiteral("4. 锅里放 2 番茄 1 鸡蛋") : QStringLiteral("4. Pot: 2 Tomato + 1 Egg"), 11, true, QColor(248, 248, 248));
    Theme::text(painter, QRectF(icon5.left() - 14.0, 372.0, 148.0, 24.0), m_chinese ? QStringLiteral("5. 出餐") : QStringLiteral("5. Deliver"), 13, true, QColor(248, 248, 248));

    Theme::text(painter,
                QRectF(174.0, 446.0, 932.0, 28.0),
                m_chinese ? QStringLiteral("目标：完成本局要求的番茄鸡蛋汤出餐数量。")
                          : QStringLiteral("Goal: Deliver the required number of tomato egg soups."),
                17,
                true,
                QColor(255, 247, 214));

    QString difficultyTip;
    if (m_chinese) {
        if (m_difficultyMode == 0) {
            difficultyTip = QStringLiteral("难度 1：无限时，专注协作分工。");
        } else if (m_difficultyMode == 1) {
            difficultyTip = QStringLiteral("难度 2：有限时，需要更快配合。");
        } else {
            difficultyTip = QStringLiteral("难度 3：熟汤放太久会糊锅，记得及时装盘。");
        }
    } else {
        if (m_difficultyMode == 0) {
            difficultyTip = QStringLiteral("Difficulty 1: untimed co-op with a delivery goal.");
        } else if (m_difficultyMode == 1) {
            difficultyTip = QStringLiteral("Difficulty 2: timed round with a delivery goal.");
        } else {
            difficultyTip = QStringLiteral("Difficulty 3: ready soup can burn if ignored.");
        }
    }

    Theme::text(painter,
                QRectF(174.0, 486.0, 932.0, 24.0),
                difficultyTip,
                12,
                true,
                QColor(224, 224, 224));
}

void TutorialOverlayItem::drawFooter(QPainter *painter) const
{
    Theme::roundedBox(painter, QRectF(424.0, 592.0, 432.0, 52.0), QColor(56, 125, 182));
    Theme::text(painter,
                QRectF(442.0, 606.0, 396.0, 24.0),
                m_chinese ? QStringLiteral("按 Enter / Space / F / / 开始")
                          : QStringLiteral("Press Enter / Space / F / / to begin"),
                16,
                true,
                QColor(255, 255, 255));
}
