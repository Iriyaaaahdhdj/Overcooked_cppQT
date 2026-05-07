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
    setZValue(10.0);
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
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter->setPen(Qt::NoPen);

    const QRectF scorePanel(18.0, 624.0, 250.0, 70.0);
    painter->setBrush(QColor(25, 34, 47, 90));
    painter->drawRect(scorePanel.translated(5.0, 5.0));
    painter->setBrush(QColor(55, 123, 169, 236));
    painter->drawRect(scorePanel);
    painter->setBrush(QColor(77, 156, 204, 236));
    painter->drawRect(QRectF(scorePanel.left(), scorePanel.top(), scorePanel.width(), 18.0));
    painter->setBrush(QColor(30, 81, 126, 236));
    painter->drawRect(QRectF(scorePanel.left(), scorePanel.bottom() - 16.0, scorePanel.width(), 16.0));

    painter->setBrush(QColor(245, 196, 58));
    painter->drawEllipse(QRectF(34.0, 640.0, 38.0, 38.0));
    painter->setBrush(QColor(255, 228, 118));
    painter->drawEllipse(QRectF(41.0, 647.0, 24.0, 24.0));

    Theme::text(painter,
                QRectF(82.0, 636.0, 160.0, 14.0),
                m_chinese ? QStringLiteral("分数") : QStringLiteral("SCORE"),
                8,
                true,
                QColor(220, 238, 255),
                Qt::AlignLeft | Qt::AlignVCenter);
    Theme::text(painter,
                QRectF(80.0, 650.0, 170.0, 24.0),
                QString::number(m_score),
                20,
                true,
                QColor(255, 255, 255),
                Qt::AlignLeft | Qt::AlignVCenter);
    Theme::text(painter,
                QRectF(82.0, 674.0, 170.0, 12.0),
                m_chinese ? QStringLiteral("寿司 %1 / %2").arg(m_completed).arg(m_target)
                          : QStringLiteral("SUSHI %1 / %2").arg(m_completed).arg(m_target),
                7,
                true,
                QColor(224, 239, 255),
                Qt::AlignLeft | Qt::AlignVCenter);

    const QRectF timerPanel(m_rect.width() - 240.0, 624.0, 220.0, 70.0);
    painter->setBrush(QColor(25, 34, 47, 90));
    painter->drawRect(timerPanel.translated(5.0, 5.0));
    painter->setBrush(QColor(255, 244, 222, 236));
    painter->drawRect(timerPanel);
    painter->setBrush(QColor(245, 211, 154, 236));
    painter->drawRect(QRectF(timerPanel.left(), timerPanel.top(), timerPanel.width(), 18.0));
    painter->setBrush(QColor(224, 180, 116, 236));
    painter->drawRect(QRectF(timerPanel.left(), timerPanel.bottom() - 16.0, timerPanel.width(), 16.0));

    const int minutes = m_timeSeconds >= 0 ? m_timeSeconds / 60 : 0;
    const int seconds = m_timeSeconds >= 0 ? m_timeSeconds % 60 : 0;
    const QString timeText = m_timeSeconds < 0
                                 ? QStringLiteral("∞")
                                 : QStringLiteral("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));

    Theme::text(painter,
                QRectF(timerPanel.left() + 16.0, 636.0, 160.0, 14.0),
                m_chinese ? QStringLiteral("时间") : QStringLiteral("TIME"),
                8,
                true,
                QColor(126, 95, 56),
                Qt::AlignLeft | Qt::AlignVCenter);
    Theme::text(painter,
                QRectF(timerPanel.left() + 16.0, 650.0, 170.0, 24.0),
                timeText,
                18,
                true,
                QColor(88, 58, 30),
                Qt::AlignLeft | Qt::AlignVCenter);
    Theme::text(painter,
                QRectF(timerPanel.left() + 16.0, 674.0, 178.0, 12.0),
                m_chinese ? m_recipeName + QStringLiteral("冲分") : m_recipeName + QStringLiteral(" run"),
                7,
                true,
                QColor(143, 111, 76),
                Qt::AlignLeft | Qt::AlignVCenter);
}

void HudOverlayItem::drawPanel(QPainter *, const QRectF &, const QString &, const QString &) const
{
}
