#include "KitchenItem.h"

#include <cmath>
#include <QColor>
#include <QPainter>
#include <QPen>

#include "GameTypes.h"

namespace {
QPen pixelPen()
{
    QPen pen(QColor(58, 46, 78), 1.0);
    pen.setCosmetic(true);
    return pen;
}
}

KitchenItem::KitchenItem(Kind kind, int gridX, int gridY, int gridWidth, int gridHeight, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_kind(kind)
    , m_rect(0.0, 0.0, gridWidth * GridSize, gridHeight * GridSize)
    , m_gridX(gridX)
    , m_gridY(gridY)
    , m_storedItem(NoItem)
    , m_storedItemLocalCenter(m_rect.center())
    , m_hasStoredItemLocalCenter(false)
    , m_storedCount(0)
    , m_tomatoCount(0)
    , m_eggCount(0)
    , m_stationProgress(0.0)
    , m_burnProgress(0.0)
    , m_animTime(0.0)
    , m_soupReady(false)
    , m_isBurnt(false)
{
    setPos(gridX * GridSize, gridY * GridSize);
    setZValue(kind == FloorTileKind ? 0.0 : 10.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

KitchenItem::KitchenItem(Kind kind, const QRectF &sceneRect, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_kind(kind)
    , m_rect(0.0, 0.0, sceneRect.width(), sceneRect.height())
    , m_gridX(static_cast<int>(sceneRect.x()) / GridSize)
    , m_gridY(static_cast<int>(sceneRect.y()) / GridSize)
    , m_storedItem(NoItem)
    , m_storedItemLocalCenter(m_rect.center())
    , m_hasStoredItemLocalCenter(false)
    , m_storedCount(0)
    , m_tomatoCount(0)
    , m_eggCount(0)
    , m_stationProgress(0.0)
    , m_burnProgress(0.0)
    , m_animTime(0.0)
    , m_soupReady(false)
    , m_isBurnt(false)
{
    setPos(sceneRect.topLeft());
    setZValue(kind == FloorTileKind ? 0.0 : 2.0);
    setCacheMode(QGraphicsItem::NoCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF KitchenItem::boundingRect() const
{
    return m_rect;
}

QPainterPath KitchenItem::shape() const
{
    QPainterPath path;
    path.addRect(m_rect);
    return path;
}

KitchenItem::Kind KitchenItem::itemKind() const
{
    return m_kind;
}

bool KitchenItem::blocksMovement() const
{
    return m_kind == BoundaryWallKind;
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
    if (m_storedItem == NoItem) {
        clearStoredItemSceneCenter();
    }
    update();
}

QPointF KitchenItem::storedItemSceneCenter() const
{
    const QPointF localCenter = m_hasStoredItemLocalCenter ? m_storedItemLocalCenter : m_rect.center();
    return mapToScene(localCenter);
}

void KitchenItem::setStoredItemSceneCenter(const QPointF &sceneCenter)
{
    m_storedItemLocalCenter = mapFromScene(sceneCenter);
    m_hasStoredItemLocalCenter = true;
    update();
}

void KitchenItem::clearStoredItemSceneCenter()
{
    m_storedItemLocalCenter = m_rect.center();
    m_hasStoredItemLocalCenter = false;
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
    m_animTime += deltaSeconds;

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

    if (m_storedItem != RiceItem) {
        return;
    }

    setStationProgress(m_stationProgress + deltaSeconds / 14.0);
    if (m_stationProgress >= 1.0) {
        m_soupReady = true;
        m_burnProgress = 0.0;
        m_storedItem = CookedRiceItem;
        update();
    }
}

void KitchenItem::resetState()
{
    m_storedItem = NoItem;
    clearStoredItemSceneCenter();
    m_storedCount = 0;
    m_tomatoCount = 0;
    m_eggCount = 0;
    m_stationProgress = 0.0;
    m_burnProgress = 0.0;
    m_animTime = 0.0;
    m_soupReady = false;
    m_isBurnt = false;
    update();
}

int KitchenItem::gridX() const
{
    return m_gridX;
}

int KitchenItem::gridY() const
{
    return m_gridY;
}

void KitchenItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, false);

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
    case CleanPlateStationKind:
        drawPlateStation(painter);
        break;
    case SinkStationKind:
    case DirtyPlateStationKind:
    case GarbageStationKind:
        drawCounter(painter);
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
        return QColor(255, 236, 245);
    case BoundaryWallKind:
        return QColor(255, 194, 214);
    case StoveKind:
        return QColor(182, 215, 248);
    case ChoppingStationKind:
        return QColor(255, 227, 188);
    case DeliveryWindowKind:
        return QColor(186, 232, 182);
    case TomatoStationKind:
        return QColor(255, 206, 220);
    case EggStationKind:
        return QColor(255, 235, 196);
    case PlateStationKind:
    case CleanPlateStationKind:
        return QColor(222, 238, 255);
    case SinkStationKind:
        return QColor(206, 226, 232);
    case DirtyPlateStationKind:
        return QColor(232, 218, 204);
    case GarbageStationKind:
        return QColor(96, 112, 80);
    case CounterKind:
        return QColor(247, 208, 180);
    }
    return QColor(255, 255, 255);
}

void KitchenItem::drawBase(QPainter *painter, const QColor &fillColor) const
{
    painter->setPen(pixelPen());
    painter->setBrush(fillColor);
    painter->drawRect(m_rect.adjusted(0.0, 0.0, -1.0, -1.0));
}

void KitchenItem::drawFloor(QPainter *painter) const
{
    painter->setPen(QPen(QColor(228, 210, 226), 1.0));
    painter->setBrush(fillColor());
    painter->drawRect(m_rect.adjusted(0.0, 0.0, -1.0, -1.0));
}

void KitchenItem::drawWall(QPainter *painter) const
{
    drawBase(painter, fillColor());
    painter->fillRect(QRectF(4.0, 4.0, 56.0, 12.0), QColor(255, 226, 236));
    painter->fillRect(QRectF(4.0, 18.0, 56.0, 12.0), QColor(245, 172, 198));
    painter->fillRect(QRectF(4.0, 32.0, 56.0, 12.0), QColor(255, 226, 236));
    painter->fillRect(QRectF(4.0, 46.0, 56.0, 12.0), QColor(245, 172, 198));
}

void KitchenItem::drawStove(QPainter *painter) const
{
    drawBase(painter, fillColor());
    painter->fillRect(QRectF(8.0, 8.0, 48.0, 36.0), QColor(76, 80, 90));
    painter->fillRect(QRectF(12.0, 12.0, 40.0, 28.0), QColor(52, 56, 64));
    painter->fillRect(QRectF(12.0, 48.0, 40.0, 8.0), QColor(224, 88, 70));
    const bool flameActive = m_storedItem == RiceItem || m_soupReady || m_isBurnt;
    if (flameActive) {
        const qreal phase = std::fmod(m_animTime * 6.0, 2.0);
        const qreal wobble = (phase < 1.0) ? phase : (2.0 - phase);
        const QColor outer = m_isBurnt ? QColor(80, 80, 80) : QColor(255, 120, 62);
        const QColor inner = m_isBurnt ? QColor(42, 42, 42) : QColor(255, 214, 96);
        painter->fillRect(QRectF(18.0, 42.0 - wobble, 8.0, 8.0 + wobble), outer);
        painter->fillRect(QRectF(28.0, 39.0 - wobble * 1.5, 10.0, 11.0 + wobble * 1.5), inner);
        painter->fillRect(QRectF(40.0, 42.0 - wobble, 8.0, 8.0 + wobble), outer);
    }

    if (m_isBurnt) {
        painter->fillRect(QRectF(18.0, 18.0, 20.0, 12.0), QColor(24, 24, 24));
    } else if (m_soupReady) {
        drawCarryItemIcon(painter, CookedRiceItem, QRectF(18.0, 14.0, 28.0, 24.0));
    } else {
        if (m_storedItem == RiceItem) {
            drawCarryItemIcon(painter, RiceItem, QRectF(18.0, 14.0, 28.0, 24.0));
        }
    }
    if (!m_isBurnt && (m_storedItem == RiceItem || m_soupReady)) {
        drawProgressBar(painter, QRectF(12.0, 52.0, 40.0, 4.0), m_soupReady ? QColor(255, 108, 88) : QColor(255, 188, 84));
    }
}

void KitchenItem::drawChoppingStation(QPainter *painter) const
{
    drawBase(painter, fillColor());
    painter->fillRect(QRectF(8.0, 8.0, 48.0, 44.0), QColor(255, 245, 226));
    painter->fillRect(QRectF(14.0, 16.0, 36.0, 18.0), QColor(168, 120, 82));
    painter->fillRect(QRectF(32.0, 14.0, 14.0, 4.0), QColor(238, 238, 238));
    painter->fillRect(QRectF(42.0, 14.0, 4.0, 16.0), QColor(90, 62, 44));
    drawStationItem(painter, QRectF(18.0, 16.0, 20.0, 20.0));
    if ((m_storedItem == CucumberItem || m_storedItem == ShrimpItem)
        && m_stationProgress > 0.0 && m_stationProgress < 1.0) {
        drawProgressBar(painter, QRectF(12.0, 52.0, 40.0, 4.0), QColor(118, 214, 112));
    }
}

void KitchenItem::drawDeliveryWindow(QPainter *painter) const
{
    drawBase(painter, fillColor());
    painter->fillRect(QRectF(8.0, 12.0, 48.0, 12.0), QColor(252, 255, 250));
    painter->fillRect(QRectF(16.0, 32.0, 32.0, 18.0), QColor(255, 255, 255));
    painter->fillRect(QRectF(20.0, 36.0, 24.0, 8.0), QColor(242, 104, 138));
}

void KitchenItem::drawTomatoStation(QPainter *painter) const
{
    drawBase(painter, fillColor());
    painter->fillRect(QRectF(10.0, 12.0, 44.0, 40.0), QColor(182, 132, 110));
    painter->fillRect(QRectF(14.0, 16.0, 36.0, 32.0), QColor(212, 162, 136));
    painter->fillRect(QRectF(16.0, 18.0, 14.0, 14.0), QColor(244, 90, 86));
    painter->fillRect(QRectF(34.0, 18.0, 14.0, 14.0), QColor(244, 90, 86));
    painter->fillRect(QRectF(24.0, 32.0, 14.0, 14.0), QColor(244, 90, 86));
}

void KitchenItem::drawEggStation(QPainter *painter) const
{
    drawBase(painter, fillColor());
    painter->fillRect(QRectF(10.0, 12.0, 44.0, 40.0), QColor(184, 142, 98));
    painter->fillRect(QRectF(14.0, 16.0, 36.0, 32.0), QColor(214, 170, 122));
    painter->fillRect(QRectF(18.0, 20.0, 10.0, 14.0), QColor(255, 250, 236));
    painter->fillRect(QRectF(32.0, 20.0, 10.0, 14.0), QColor(255, 250, 236));
    painter->fillRect(QRectF(25.0, 34.0, 10.0, 14.0), QColor(255, 250, 236));
}

void KitchenItem::drawPlateStation(QPainter *painter) const
{
    drawBase(painter, fillColor());
    painter->fillRect(QRectF(10.0, 12.0, 44.0, 40.0), QColor(238, 244, 252));
    painter->fillRect(QRectF(18.0, 22.0, 18.0, 18.0), QColor(255, 255, 255));
    painter->fillRect(QRectF(24.0, 18.0, 18.0, 18.0), QColor(255, 255, 255));
    painter->fillRect(QRectF(30.0, 24.0, 18.0, 18.0), QColor(255, 255, 255));
}

void KitchenItem::drawCounter(QPainter *painter) const
{
    drawBase(painter, fillColor());
    painter->fillRect(QRectF(8.0, 10.0, 48.0, 16.0), QColor(255, 230, 206));
    painter->fillRect(QRectF(8.0, 28.0, 48.0, 24.0), QColor(235, 186, 152));
    painter->fillRect(QRectF(12.0, 34.0, 14.0, 14.0), QColor(214, 164, 130));
    painter->fillRect(QRectF(38.0, 34.0, 14.0, 14.0), QColor(214, 164, 130));
    drawStationItem(painter, QRectF(18.0, 14.0, 24.0, 24.0));
}

void KitchenItem::drawStationItem(QPainter *painter, const QRectF &rect) const
{
    drawCarryItemIcon(painter, m_storedItem, rect);
}

void KitchenItem::drawProgressBar(QPainter *painter, const QRectF &rect, const QColor &fillColor) const
{
    painter->setPen(pixelPen());
    painter->setBrush(QColor(255, 255, 255));
    painter->drawRect(rect.adjusted(0.0, 0.0, -1.0, -1.0));
    const qreal value = m_soupReady ? m_burnProgress : m_stationProgress;
    painter->fillRect(QRectF(rect.left() + 1.0, rect.top() + 1.0, (rect.width() - 2.0) * value, rect.height() - 2.0), fillColor);
}
