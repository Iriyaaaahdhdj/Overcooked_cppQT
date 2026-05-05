#include "HudOverlayItem.h"

#include <QPainter>

#include "Theme.h"

HudOverlayItem::HudOverlayItem(qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_rect(0.0, 0.0, width, height)
    , m_score(0)
    , m_timeSeconds(0)
    , m_completed(0)
    , m_target(0)
    , m_chinese(true)
{
    setZValue(190.0);
    setAcceptedMouseButtons(Qt::NoButton);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QRectF HudOverlayItem::boundingRect() const
{
    return m_rect;
}

void HudOverlayItem::setStats(int score, int timeSeconds, int completed, int target, const QString &recipeName, bool chinese)
{
    m_score = score;
    m_timeSeconds = timeSeconds;
    m_completed = completed;
    m_target = target;
    m_recipeName = recipeName;
    m_chinese = chinese;
    update();
}

void HudOverlayItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    const QRectF band(190.0, 18.0, 900.0, 74.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(16, 28, 38, 92));
    painter->drawRoundedRect(band, 24.0, 24.0);

    drawPanel(painter, QRectF(214.0, 28.0, 200.0, 54.0), m_chinese ? QStringLiteral("分数") : QStringLiteral("SCORE"), QString::number(m_score));

    const int minutes = m_timeSeconds / 60;
    const int seconds = m_timeSeconds % 60;
    drawPanel(painter,
              QRectF(438.0, 28.0, 200.0, 54.0),
              m_chinese ? QStringLiteral("时间") : QStringLiteral("TIME"),
              m_timeSeconds < 0 ? (m_chinese ? QStringLiteral("无限时") : QStringLiteral("No Limit"))
                                : QStringLiteral("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')));

    drawPanel(painter,
              QRectF(662.0, 28.0, 200.0, 54.0),
              m_chinese ? QStringLiteral("出餐") : QStringLiteral("DELIVERED"),
              QStringLiteral("%1 / %2").arg(m_completed).arg(m_target));

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 244, 224, 232));
    painter->drawRoundedRect(QRectF(886.0, 28.0, 180.0, 54.0), 18.0, 18.0);
    Theme::text(painter,
                QRectF(896.0, 35.0, 160.0, 16.0),
                m_chinese ? QStringLiteral("菜单") : QStringLiteral("RECIPE"),
                9,
                true,
                QColor(116, 88, 54),
                Qt::AlignCenter);
    Theme::text(painter,
                QRectF(896.0, 50.0, 160.0, 20.0),
                m_recipeName,
                12,
                true,
                QColor(69, 48, 22),
                Qt::AlignCenter);
}

void HudOverlayItem::drawPanel(QPainter *painter, const QRectF &rect, const QString &label, const QString &value) const
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(246, 250, 255, 236));
    painter->drawRoundedRect(rect, 18.0, 18.0);

    Theme::text(painter,
                QRectF(rect.left(), rect.top() + 7.0, rect.width(), 12.0),
                label,
                8,
                true,
                QColor(93, 124, 152),
                Qt::AlignCenter);
    Theme::text(painter,
                QRectF(rect.left(), rect.top() + 20.0, rect.width(), 24.0),
                value,
                16,
                true,
                QColor(29, 42, 57),
                Qt::AlignCenter);
}
