#include "ResultOverlayItem.h"

#include <QPainter>
#include <QPainterPath>
#include <QtMath>

#include "Theme.h"

namespace {
QPainterPath starPath(const QPointF &center, qreal radius)
{
    QPainterPath path;
    for (int i = 0; i < 10; ++i) {
        const qreal angle = qDegreesToRadians(-90.0 + i * 36.0);
        const qreal currentRadius = (i % 2 == 0) ? radius : radius * 0.45;
        const QPointF point(center.x() + std::cos(angle) * currentRadius,
                            center.y() + std::sin(angle) * currentRadius);
        if (i == 0) {
            path.moveTo(point);
        } else {
            path.lineTo(point);
        }
    }
    path.closeSubpath();
    return path;
}
}

ResultOverlayItem::ResultOverlayItem(qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_rect(0.0, 0.0, width, height)
    , m_chinese(true)
    , m_success(false)
    , m_levelIndex(1)
    , m_starCount(0)
    , m_deliveriesCompleted(0)
    , m_targetDeliveries(0)
    , m_score(0)
    , m_nextLevelEnabled(false)
    , m_hasNextLevel(true)
    , m_selectedButton(RestartButton)
{
    setZValue(210.0);
    setAcceptedMouseButtons(Qt::NoButton);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QRectF ResultOverlayItem::boundingRect() const
{
    return m_rect;
}

void ResultOverlayItem::setLanguageChinese(bool chinese)
{
    if (m_chinese == chinese) {
        return;
    }

    m_chinese = chinese;
    update();
}

void ResultOverlayItem::setResultData(bool success,
                                      int levelIndex,
                                      int starCount,
                                      int deliveriesCompleted,
                                      int targetDeliveries,
                                      int score,
                                      bool nextLevelEnabled,
                                      bool hasNextLevel)
{
    m_success = success;
    m_levelIndex = levelIndex;
    m_starCount = starCount;
    m_deliveriesCompleted = deliveriesCompleted;
    m_targetDeliveries = targetDeliveries;
    m_score = score;
    m_nextLevelEnabled = nextLevelEnabled;
    m_hasNextLevel = hasNextLevel;
    m_selectedButton = RestartButton;
    update();
}

void ResultOverlayItem::setSelectedButton(int button)
{
    const int clamped = qBound(0, button, 1);
    if (m_selectedButton == clamped) {
        return;
    }

    m_selectedButton = clamped;
    update();
}

int ResultOverlayItem::selectedButton() const
{
    return m_selectedButton;
}

void ResultOverlayItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->fillRect(m_rect, QColor(10, 17, 24, 158));

    const QRectF card(286.0, 102.0, 708.0, 500.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(22, 34, 47, 210));
    painter->drawRoundedRect(card.adjusted(10.0, 14.0, 10.0, 18.0), 28.0, 28.0);
    painter->setBrush(QColor(255, 248, 234));
    painter->drawRoundedRect(card, 28.0, 28.0);

    painter->setBrush(m_success ? QColor(115, 208, 142) : QColor(240, 121, 111));
    painter->drawRoundedRect(QRectF(card.left(), card.top(), card.width(), 82.0), 28.0, 28.0);
    painter->fillRect(QRectF(card.left(), card.top() + 41.0, card.width(), 41.0),
                      m_success ? QColor(115, 208, 142) : QColor(240, 121, 111));

    const QString title = m_success
                              ? (m_chinese ? QStringLiteral("关卡完成") : QStringLiteral("Level Clear"))
                              : (m_chinese ? QStringLiteral("关卡失败") : QStringLiteral("Level Failed"));
    const QString subtitle = m_chinese
                                 ? QStringLiteral("第 %1 关结算").arg(m_levelIndex)
                                 : QStringLiteral("Level %1 Results").arg(m_levelIndex);
    Theme::text(painter, QRectF(card.left() + 28.0, card.top() + 16.0, card.width() - 56.0, 28.0), title, 24, true, Qt::white);
    Theme::text(painter, QRectF(card.left() + 28.0, card.top() + 46.0, card.width() - 56.0, 22.0), subtitle, 12, true, QColor(248, 252, 255));

    Theme::text(painter,
                QRectF(card.left() + 44.0, card.top() + 112.0, card.width() - 88.0, 24.0),
                m_chinese ? QStringLiteral("星级评价") : QStringLiteral("STAR RATING"),
                12,
                true,
                QColor(116, 88, 54));
    drawStars(painter, QRectF(card.left() + 178.0, card.top() + 148.0, 352.0, 86.0));

    const QRectF statsPanel(card.left() + 72.0, card.top() + 258.0, card.width() - 144.0, 116.0);
    painter->setBrush(QColor(246, 236, 214));
    painter->drawRoundedRect(statsPanel, 24.0, 24.0);
    Theme::text(painter,
                QRectF(statsPanel.left() + 24.0, statsPanel.top() + 16.0, 160.0, 18.0),
                m_chinese ? QStringLiteral("出餐进度") : QStringLiteral("DELIVERY"),
                11,
                true,
                QColor(120, 90, 54),
                Qt::AlignLeft | Qt::AlignVCenter);
    Theme::text(painter,
                QRectF(statsPanel.left() + 24.0, statsPanel.top() + 40.0, 220.0, 32.0),
                QStringLiteral("%1 / %2").arg(m_deliveriesCompleted).arg(m_targetDeliveries),
                24,
                true,
                QColor(57, 42, 28),
                Qt::AlignLeft | Qt::AlignVCenter);
    Theme::text(painter,
                QRectF(statsPanel.right() - 220.0, statsPanel.top() + 16.0, 160.0, 18.0),
                m_chinese ? QStringLiteral("最终分数") : QStringLiteral("SCORE"),
                11,
                true,
                QColor(120, 90, 54),
                Qt::AlignRight | Qt::AlignVCenter);
    Theme::text(painter,
                QRectF(statsPanel.right() - 220.0, statsPanel.top() + 40.0, 160.0, 32.0),
                QString::number(m_score),
                24,
                true,
                QColor(57, 42, 28),
                Qt::AlignRight | Qt::AlignVCenter);

    const QString hint = m_success
                             ? (m_nextLevelEnabled
                                    ? (m_hasNextLevel
                                           ? (m_chinese ? QStringLiteral("达到一星以上，已解锁下一关。")
                                                         : QStringLiteral("1 star or more earned. Next level unlocked."))
                                           : (m_chinese ? QStringLiteral("达到一星以上，已完成当前所有关卡。")
                                                         : QStringLiteral("1 star or more earned. All current levels cleared.")))
                                    : (m_chinese ? QStringLiteral("未达到一星，下一关仍保持锁定。")
                                                  : QStringLiteral("Less than 1 star. Next level stays locked.")))
                             : (m_chinese ? QStringLiteral("先完成目标出餐数量，才能获得星级并解锁下一关。")
                                           : QStringLiteral("Meet the delivery goal first to earn stars and unlock the next level."));
    Theme::text(painter,
                QRectF(card.left() + 64.0, card.top() + 394.0, card.width() - 128.0, 24.0),
                hint,
                11,
                true,
                QColor(91, 75, 57));

    const QRectF restartButton(card.left() + 86.0, card.bottom() - 96.0, 236.0, 56.0);
    const QRectF nextButton(card.right() - 322.0, card.bottom() - 96.0, 236.0, 56.0);
    const QString restartLabel = m_chinese ? QStringLiteral("重新开始") : QStringLiteral("Restart");
    const QString nextLabel = m_hasNextLevel
                                  ? (m_chinese ? QStringLiteral("下一关卡") : QStringLiteral("Next Level"))
                                  : (m_chinese ? QStringLiteral("返回菜单") : QStringLiteral("Back to Menu"));
    drawButton(painter, restartButton, restartLabel, m_selectedButton == RestartButton, true);
    drawButton(painter, nextButton, nextLabel, m_selectedButton == NextLevelButton, m_nextLevelEnabled);

    const QString footer = m_chinese
                               ? QStringLiteral("A/D 或方向键切换，Enter / F 确认")
                               : QStringLiteral("Use A/D or arrows to switch, Enter / F to confirm");
    Theme::text(painter,
                QRectF(card.left() + 64.0, card.bottom() - 26.0, card.width() - 128.0, 18.0),
                footer,
                10,
                true,
                QColor(136, 115, 86));
}

void ResultOverlayItem::drawStars(QPainter *painter, const QRectF &rect) const
{
    const qreal spacing = rect.width() / 3.0;
    for (int i = 0; i < 3; ++i) {
        const QPointF center(rect.left() + spacing * (i + 0.5), rect.center().y());
        const bool filled = i < m_starCount;
        painter->setPen(QPen(filled ? QColor(196, 130, 26) : QColor(179, 169, 151), 2.0));
        painter->setBrush(filled ? QColor(255, 208, 70) : QColor(231, 224, 210));
        painter->drawPath(starPath(center, 34.0));
    }
}

void ResultOverlayItem::drawButton(QPainter *painter,
                                   const QRectF &rect,
                                   const QString &text,
                                   bool selected,
                                   bool enabled) const
{
    QColor fill = enabled ? QColor(255, 255, 255) : QColor(225, 221, 214);
    QColor border = enabled ? QColor(83, 141, 196) : QColor(172, 166, 157);
    QColor textColor = enabled ? QColor(42, 60, 84) : QColor(148, 138, 125);

    if (selected && enabled) {
        fill = QColor(112, 186, 250);
        border = QColor(38, 106, 165);
        textColor = Qt::white;
    }

    painter->setPen(QPen(border, selected ? 3.0 : 2.0));
    painter->setBrush(fill);
    painter->drawRoundedRect(rect, 20.0, 20.0);
    Theme::text(painter, rect, text, 16, true, textColor);
}
