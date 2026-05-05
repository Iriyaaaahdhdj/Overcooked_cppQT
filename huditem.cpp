#include "huditem.h"

#include <QPainter>

#include "gameconstants.h"

HudItem::HudItem(qreal width, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_rect(0.0, 0.0, width, kHudHeight)
    , m_score(0)
    , m_orders(0)
    , m_timeLeft(180.0)
    , m_gameOver(false)
{
    setZValue(50.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF HudItem::boundingRect() const
{
    return m_rect;
}

void HudItem::setScore(int score)
{
    if (m_score == score) {
        return;
    }

    m_score = score;
    update();
}

void HudItem::setOrders(int orders)
{
    if (m_orders == orders) {
        return;
    }

    m_orders = orders;
    update();
}

void HudItem::setTimeLeft(qreal seconds)
{
    m_timeLeft = seconds;
    update();
}

void HudItem::setGameOver(bool gameOver)
{
    if (m_gameOver == gameOver) {
        return;
    }

    m_gameOver = gameOver;
    update();
}

void HudItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    drawRoundedCard(painter, QRectF(8.0, 8.0, m_rect.width() - 16.0, m_rect.height() - 16.0), QColor(255, 248, 235));
    drawRoundedCard(painter, QRectF(20.0, 22.0, 132.0, 48.0), colorHudCard());
    drawRoundedCard(painter, QRectF(166.0, 22.0, 132.0, 48.0), colorHudCard());
    drawRoundedCard(painter, QRectF(312.0, 22.0, 178.0, 48.0), colorHudCard());
    drawRoundedCard(painter, QRectF(504.0, 22.0, 372.0, 48.0), colorHudCard());

    painter->setPen(Qt::black);
    QFont titleFont = painter->font();
    titleFont.setBold(true);
    titleFont.setPointSize(11);
    painter->setFont(titleFont);
    painter->drawText(QRectF(32.0, 32.0, 108.0, 20.0), QStringLiteral("Score"));
    painter->drawText(QRectF(178.0, 32.0, 108.0, 20.0), QStringLiteral("Orders"));
    painter->drawText(QRectF(324.0, 32.0, 166.0, 20.0), QStringLiteral("Timer"));
    painter->drawText(QRectF(518.0, 28.0, 350.0, 16.0), QStringLiteral("P1 WASD + F   |   P2 Arrows + /   |   Make Tomato Soup"));

    QFont valueFont = painter->font();
    valueFont.setBold(true);
    valueFont.setPointSize(13);
    painter->setFont(valueFont);
    painter->drawText(QRectF(32.0, 46.0, 108.0, 18.0), QString::number(m_score));
    painter->drawText(QRectF(178.0, 46.0, 108.0, 18.0), QString::number(m_orders));
    painter->drawText(QRectF(324.0, 46.0, 166.0, 18.0), QString::number(qMax(0, int(m_timeLeft))) + QStringLiteral("s"));

    QFont noteFont = painter->font();
    noteFont.setBold(false);
    noteFont.setPointSize(9);
    painter->setFont(noteFont);
    painter->drawText(QRectF(518.0, 48.0, 350.0, 16.0),
                      QStringLiteral("Order: Tomato -> Chop -> Pot x3 -> Plate -> Serve"));

    if (m_gameOver) {
        drawRoundedCard(painter, QRectF(m_rect.width() / 2.0 - 150.0, 20.0, 300.0, 52.0), QColor(255, 255, 255));
        painter->setFont(valueFont);
        painter->drawText(QRectF(m_rect.width() / 2.0 - 130.0, 36.0, 260.0, 20.0),
                          Qt::AlignCenter,
                          QStringLiteral("Time Up! Final Score %1").arg(m_score));
    }
}
