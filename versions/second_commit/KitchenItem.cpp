#include "KitchenItem.h"

#include <QColor>
#include <QPainter>
#include <QPen>

#include "GameTypes.h"
#include "Theme.h"

namespace {
QPen outlinePen()
{
    QPen pen(Qt::black, 1.0);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    return pen;
}

void drawRoundedRect(QPainter *painter, const QRectF &rect, const QColor &fillColor)
{
    painter->setPen(outlinePen());
    painter->setBrush(fillColor);
    painter->drawRoundedRect(rect, KitchenItem::CornerRadius, KitchenItem::CornerRadius);
}
}

KitchenItem::KitchenItem(Kind kind, int gridX, int gridY, int gridWidth, int gridHeight, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_kind(kind)
    , m_rect(0.0, 0.0, gridWidth * GridSize, gridHeight * GridSize)
    , m_storedItem(NoItem)
    , m_storedCount(0)
    , m_tomatoCount(0)
    , m_eggCount(0)
    , m_stationProgress(0.0)
    , m_burnProgress(0.0)
    , m_soupReady(false)
    , m_isBurnt(false)
{
    setPos(gridX * GridSize, gridY * GridSize);
    setZValue(kind == FloorTileKind ? 0.0 : 10.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF KitchenItem::boundingRect() const
{
    return m_rect;
}

QPainterPath KitchenItem::shape() const
{
    QPainterPath path;
    path.addRoundedRect(m_rect, CornerRadius, CornerRadius);
    return path;
}

KitchenItem::Kind KitchenItem::itemKind() const
{
    return m_kind;
}

bool KitchenItem::blocksMovement() const
{
    return m_kind != FloorTileKind;
}

CarryItemType KitchenItem::storedItem() const
{
    return m_storedItem;
}

void KitchenItem::setStoredItem(CarryItemType item)
{
    if (m_storedItem == item) {
        return;
    }

    m_storedItem = item;
    update();
}

int KitchenItem::storedCount() const
{
    return m_storedCount;
}

void KitchenItem::setStoredCount(int count)
{
    if (m_storedCount == count) {
        return;
    }

    m_storedCount = count;
    update();
}

qreal KitchenItem::stationProgress() const
{
    return m_stationProgress;
}

void KitchenItem::setStationProgress(qreal progress)
{
    const qreal bounded = qBound(0.0, progress, 1.0);
    if (qFuzzyCompare(m_stationProgress, bounded)) {
        return;
    }

    m_stationProgress = bounded;
    update();
}

bool KitchenItem::soupReady() const
{
    return m_soupReady;
}

void KitchenItem::setSoupReady(bool ready)
{
    if (m_soupReady == ready) {
        return;
    }

    m_soupReady = ready;
    update();
}

int KitchenItem::tomatoCount() const
{
    return m_tomatoCount;
}

void KitchenItem::setTomatoCount(int count)
{
    if (m_tomatoCount == count) {
        return;
    }

    m_tomatoCount = count;
    update();
}

int KitchenItem::eggCount() const
{
    return m_eggCount;
}

void KitchenItem::setEggCount(int count)
{
    if (m_eggCount == count) {
        return;
    }

    m_eggCount = count;
    update();
}

qreal KitchenItem::burnProgress() const
{
    return m_burnProgress;
}

bool KitchenItem::isBurnt() const
{
    return m_isBurnt;
}

void KitchenItem::tick(qreal deltaSeconds, bool allowBurning)
{
    if (m_kind != StoveKind || m_isBurnt) {
        return;
    }

    if (m_soupReady) {
        if (!allowBurning) {
            return;
        }

        m_burnProgress = qBound(0.0, m_burnProgress + deltaSeconds / 6.0, 1.0);
        if (m_burnProgress >= 1.0) {
            m_isBurnt = true;
        }
        update();
        return;
    }

    if (m_tomatoCount < 2 || m_eggCount < 1) {
        return;
    }

    setStationProgress(m_stationProgress + deltaSeconds / 5.0);
    if (m_stationProgress >= 1.0) {
        m_soupReady = true;
        m_burnProgress = 0.0;
        m_storedItem = SoupPlateItem;
        update();
    }
}

void KitchenItem::resetState()
{
    m_storedItem = NoItem;
    m_storedCount = 0;
    m_tomatoCount = 0;
    m_eggCount = 0;
    m_stationProgress = 0.0;
    m_burnProgress = 0.0;
    m_soupReady = false;
    m_isBurnt = false;
    update();
}

void KitchenItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    switch (m_kind) {
    case FloorTileKind:
        drawFloor(painter);
        break;
    case BoundaryWallKind:
        drawWall(painter);
        break;
    case StoveKind:
        drawStove(painter);
        break;
    case ChoppingStationKind:
        drawChoppingStation(painter);
        break;
    case DeliveryWindowKind:
        drawDeliveryWindow(painter);
        break;
    case TomatoStationKind:
        drawTomatoStation(painter);
        break;
    case EggStationKind:
        drawEggStation(painter);
        break;
    case PlateStationKind:
        drawPlateStation(painter);
        break;
    case CounterKind:
        drawCounter(painter);
        break;
    }
}

QColor KitchenItem::fillColor() const
{
    switch (m_kind) {
    case FloorTileKind:
        return QColor(247, 231, 198);
    case BoundaryWallKind:
        return QColor(88, 88, 88);
    case StoveKind:
        return QColor(173, 216, 255);
    case ChoppingStationKind:
        return QColor(255, 214, 153);
    case DeliveryWindowKind:
        return QColor(255, 241, 118);
    case TomatoStationKind:
        return QColor(255, 179, 186);
    case EggStationKind:
        return QColor(255, 240, 198);
    case PlateStationKind:
        return QColor(232, 239, 255);
    case CounterKind:
        return QColor(196, 231, 255);
    }

    return QColor(255, 255, 255);
}

void KitchenItem::drawBase(QPainter *painter, const QColor &fillColor) const
{
    drawRoundedRect(painter, m_rect.adjusted(0.5, 0.5, -0.5, -0.5), fillColor);
}

void KitchenItem::drawFloor(QPainter *painter) const
{
    painter->setPen(outlinePen());
    painter->setBrush(fillColor());
    painter->drawRect(m_rect.adjusted(0.0, 0.0, -1.0, -1.0));
}

void KitchenItem::drawWall(QPainter *painter) const
{
    drawBase(painter, fillColor());

    painter->setPen(outlinePen());
    painter->setBrush(QColor(130, 130, 130));
    painter->drawRoundedRect(m_rect.adjusted(5.0, 5.0, -5.0, -m_rect.height() * 0.5), CornerRadius, CornerRadius);
}

void KitchenItem::drawStove(QPainter *painter) const
{
    drawBase(painter, fillColor());

    painter->setPen(outlinePen());
    painter->setBrush(QColor(92, 94, 101));
    painter->drawRoundedRect(m_rect.adjusted(8.0, 9.0, -8.0, -17.0), CornerRadius, CornerRadius);

    painter->setBrush(QColor(214, 72, 52));
    painter->drawRoundedRect(QRectF(8.0, 48.0, 48.0, 10.0), 4.0, 4.0);

    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(QRectF(13.0, 14.0, 14.0, 14.0));
    painter->drawEllipse(QRectF(37.0, 14.0, 14.0, 14.0));
    painter->drawEllipse(QRectF(13.0, 34.0, 14.0, 14.0));
    painter->drawEllipse(QRectF(37.0, 34.0, 14.0, 14.0));

    painter->setBrush(QColor(255, 183, 77));
    painter->drawEllipse(QRectF(17.0, 18.0, 6.0, 6.0));
    painter->drawEllipse(QRectF(41.0, 18.0, 6.0, 6.0));
    painter->drawEllipse(QRectF(17.0, 38.0, 6.0, 6.0));
    painter->drawEllipse(QRectF(41.0, 38.0, 6.0, 6.0));

    painter->setBrush(QColor(255, 255, 255));
    painter->drawRoundedRect(QRectF(12.0, 51.0, 40.0, 7.0), 3.0, 3.0);

    if (m_isBurnt) {
        painter->setBrush(QColor(44, 44, 44));
        painter->drawEllipse(QRectF(18.0, 18.0, 28.0, 28.0));
        painter->setPen(QPen(QColor(255, 120, 92), 2.0, Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(QPointF(20.0, 20.0), QPointF(44.0, 44.0));
        painter->drawLine(QPointF(44.0, 20.0), QPointF(20.0, 44.0));
        Theme::text(painter, QRectF(8.0, 4.0, 50.0, 16.0), QStringLiteral("BURNT"), 8, true, QColor(255, 225, 200));
    } else if (m_soupReady) {
        drawCarryItemIcon(painter, SoupPlateItem, QRectF(18.0, 18.0, 28.0, 28.0));
        painter->setPen(QPen(QColor(255, 255, 255, 210), 2.0, Qt::SolidLine, Qt::RoundCap));
        painter->drawArc(QRectF(18.0, 7.0, 10.0, 18.0), 0 * 16, 180 * 16);
        painter->drawArc(QRectF(28.0, 2.0, 10.0, 20.0), 0 * 16, 180 * 16);
        painter->drawArc(QRectF(38.0, 7.0, 10.0, 18.0), 0 * 16, 180 * 16);
    } else if (m_tomatoCount > 0 || m_eggCount > 0) {
        if (m_tomatoCount > 0) {
            drawCarryItemIcon(painter, ChoppedTomatoItem, QRectF(12.0, 17.0, 22.0, 22.0));
        }
        if (m_eggCount > 0) {
            drawCarryItemIcon(painter, EggItem, QRectF(30.0, 19.0, 20.0, 20.0));
        }
        Theme::text(painter,
                    QRectF(8.0, 4.0, 50.0, 16.0),
                    QStringLiteral("T%1 E%2").arg(m_tomatoCount).arg(m_eggCount),
                    8,
                    true);
    }

    if (m_tomatoCount >= 2 && m_eggCount >= 1 && !m_soupReady) {
        drawProgressBar(painter, QRectF(11.0, 52.0, 42.0, 6.0), QColor(255, 159, 67));
    } else if (m_soupReady) {
        drawProgressBar(painter, QRectF(11.0, 52.0, 42.0, 6.0), QColor(255, 104, 88));
    }
}

void KitchenItem::drawChoppingStation(QPainter *painter) const
{
    drawBase(painter, fillColor());

    painter->setPen(outlinePen());
    painter->setBrush(QColor(255, 244, 224));
    painter->drawRoundedRect(QRectF(8.0, 10.0, 48.0, 40.0), CornerRadius, CornerRadius);
    painter->setBrush(QColor(180, 104, 62));
    painter->drawRoundedRect(QRectF(12.0, 14.0, 40.0, 28.0), 8.0, 8.0);
    painter->setBrush(QColor(228, 228, 228));
    painter->drawRoundedRect(QRectF(34.0, 17.0, 14.0, 6.0), 3.0, 3.0);
    painter->setBrush(QColor(92, 54, 36));
    painter->drawRoundedRect(QRectF(44.0, 18.0, 4.0, 16.0), 2.0, 2.0);

    painter->setBrush(QColor(255, 255, 255));
    painter->drawRoundedRect(QRectF(12.0, 52.0, 24.0, 6.0), 3.0, 3.0);

    painter->setBrush(QColor(173, 216, 230));
    painter->drawRoundedRect(QRectF(38.0, 52.0, 14.0, 6.0), 3.0, 3.0);

    drawStationItem(painter, QRectF(19.0, 16.0, 26.0, 26.0));
    if (m_storedItem == TomatoItem && m_stationProgress > 0.0 && m_stationProgress < 1.0) {
        drawProgressBar(painter, QRectF(13.0, 52.0, 38.0, 5.0), QColor(120, 202, 63));
    }
}

void KitchenItem::drawDeliveryWindow(QPainter *painter) const
{
    drawBase(painter, fillColor());

    painter->setPen(outlinePen());
    painter->setBrush(QColor(255, 255, 255));
    painter->drawRoundedRect(QRectF(8.0, 12.0, 48.0, 18.0), CornerRadius, CornerRadius);
    painter->drawRoundedRect(QRectF(16.0, 34.0, 32.0, 18.0), CornerRadius, CornerRadius);

    painter->setBrush(QColor(255, 204, 128));
    painter->drawEllipse(QRectF(26.0, 18.0, 12.0, 12.0));
    painter->setBrush(QColor(214, 72, 52));
    painter->drawRoundedRect(QRectF(21.0, 38.0, 22.0, 10.0), 4.0, 4.0);
}

void KitchenItem::drawTomatoStation(QPainter *painter) const
{
    drawBase(painter, fillColor());

    painter->setPen(outlinePen());
    painter->setBrush(QColor(148, 90, 66));
    painter->drawRoundedRect(QRectF(10.0, 12.0, 44.0, 40.0), CornerRadius, CornerRadius);
    painter->setBrush(QColor(171, 109, 82));
    painter->drawRoundedRect(QRectF(14.0, 16.0, 36.0, 32.0), 8.0, 8.0);

    painter->setBrush(QColor(255, 112, 99));
    painter->drawEllipse(QRectF(14.0, 18.0, 16.0, 16.0));
    painter->drawEllipse(QRectF(34.0, 18.0, 16.0, 16.0));
    painter->drawEllipse(QRectF(24.0, 32.0, 16.0, 16.0));

    painter->setBrush(QColor(119, 221, 119));
    painter->drawEllipse(QRectF(20.0, 15.0, 6.0, 6.0));
    painter->drawEllipse(QRectF(40.0, 15.0, 6.0, 6.0));
    painter->drawEllipse(QRectF(30.0, 29.0, 6.0, 6.0));
}

void KitchenItem::drawEggStation(QPainter *painter) const
{
    drawBase(painter, fillColor());

    painter->setPen(outlinePen());
    painter->setBrush(QColor(164, 112, 73));
    painter->drawRoundedRect(QRectF(10.0, 12.0, 44.0, 40.0), CornerRadius, CornerRadius);
    painter->setBrush(QColor(194, 143, 92));
    painter->drawRoundedRect(QRectF(14.0, 16.0, 36.0, 32.0), 8.0, 8.0);

    painter->setBrush(QColor(255, 249, 235));
    painter->drawEllipse(QRectF(16.0, 18.0, 14.0, 18.0));
    painter->drawEllipse(QRectF(30.0, 18.0, 14.0, 18.0));
    painter->drawEllipse(QRectF(23.0, 30.0, 14.0, 18.0));
    painter->setBrush(QColor(247, 197, 76));
    painter->drawEllipse(QRectF(20.0, 24.0, 6.0, 6.0));
    painter->drawEllipse(QRectF(34.0, 24.0, 6.0, 6.0));
    painter->drawEllipse(QRectF(27.0, 36.0, 6.0, 6.0));
}

void KitchenItem::drawPlateStation(QPainter *painter) const
{
    drawBase(painter, fillColor());

    painter->setPen(outlinePen());
    painter->setBrush(QColor(244, 247, 252));
    painter->drawRoundedRect(QRectF(10.0, 12.0, 44.0, 40.0), CornerRadius, CornerRadius);

    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(QRectF(16.0, 24.0, 18.0, 18.0));
    painter->drawEllipse(QRectF(22.0, 18.0, 18.0, 18.0));
    painter->drawEllipse(QRectF(30.0, 24.0, 18.0, 18.0));
    painter->setBrush(QColor(222, 234, 248));
    painter->drawEllipse(QRectF(20.0, 28.0, 10.0, 10.0));
    painter->drawEllipse(QRectF(26.0, 22.0, 10.0, 10.0));
    painter->drawEllipse(QRectF(34.0, 28.0, 10.0, 10.0));
}

void KitchenItem::drawCounter(QPainter *painter) const
{
    drawBase(painter, fillColor());

    painter->setPen(outlinePen());
    painter->setBrush(QColor(247, 222, 173));
    painter->drawRoundedRect(QRectF(8.0, 12.0, 48.0, 16.0), CornerRadius, CornerRadius);
    painter->setBrush(QColor(199, 162, 118));
    painter->drawRoundedRect(QRectF(8.0, 30.0, 48.0, 24.0), CornerRadius, CornerRadius);
    painter->setBrush(QColor(177, 140, 101));
    painter->drawRoundedRect(QRectF(12.0, 34.0, 16.0, 16.0), 4.0, 4.0);
    painter->drawRoundedRect(QRectF(36.0, 34.0, 16.0, 16.0), 4.0, 4.0);

    drawStationItem(painter, QRectF(18.0, 14.0, 28.0, 28.0));
}

void KitchenItem::drawStationItem(QPainter *painter, const QRectF &rect) const
{
    drawCarryItemIcon(painter, m_storedItem, rect);
}

void KitchenItem::drawProgressBar(QPainter *painter, const QRectF &rect, const QColor &fillColor) const
{
    painter->setPen(outlinePen());
    painter->setBrush(QColor(255, 255, 255, 220));
    painter->drawRoundedRect(rect, 3.0, 3.0);
    painter->setBrush(fillColor);
    const qreal progressValue = m_soupReady ? m_burnProgress : m_stationProgress;
    painter->drawRoundedRect(QRectF(rect.left(),
                                    rect.top(),
                                    rect.width() * progressValue,
                                    rect.height()),
                             3.0,
                             3.0);
}
