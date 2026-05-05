#include "station.h"

#include <QPainter>
#include <QPainterPath>

Station::Station(StationType type, const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_type(type)
    , m_rect(0.0, 0.0, rect.width(), rect.height())
    , m_counterItem(HeldItem::None)
    , m_boardItem(HeldItem::None)
    , m_chopProgress(0)
    , m_potIngredients(0)
    , m_potProgress(0.0)
    , m_potCooking(false)
    , m_potReady(false)
{
    setPos(rect.topLeft());
    setZValue(10.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
    setData(kDataRoleBlocking, true);
}

QRectF Station::boundingRect() const
{
    return m_rect;
}

QPainterPath Station::shape() const
{
    QPainterPath path;
    path.addRoundedRect(m_rect.adjusted(3.0, 3.0, -3.0, -3.0), 8.0, 8.0);
    return path;
}

StationType Station::stationType() const
{
    return m_type;
}

HeldItem Station::counterItem() const
{
    return m_counterItem;
}

void Station::setCounterItem(HeldItem item)
{
    m_counterItem = item;
    update();
}

HeldItem Station::takeCounterItem()
{
    const HeldItem item = m_counterItem;
    m_counterItem = HeldItem::None;
    update();
    return item;
}

HeldItem Station::boardItem() const
{
    return m_boardItem;
}

bool Station::placeOnBoard(HeldItem item)
{
    if (m_boardItem != HeldItem::None || item != HeldItem::TomatoRaw) {
        return false;
    }

    m_boardItem = item;
    m_chopProgress = 0;
    update();
    return true;
}

bool Station::chopOnce()
{
    if (m_boardItem != HeldItem::TomatoRaw) {
        return false;
    }

    ++m_chopProgress;
    if (m_chopProgress >= maxChopProgress()) {
        m_boardItem = HeldItem::TomatoChopped;
    }

    update();
    return true;
}

HeldItem Station::takeBoardItem()
{
    const HeldItem item = m_boardItem;
    if (item == HeldItem::None) {
        return HeldItem::None;
    }

    m_boardItem = HeldItem::None;
    m_chopProgress = 0;
    update();
    return item;
}

int Station::chopProgress() const
{
    return m_chopProgress;
}

int Station::maxChopProgress() const
{
    return 3;
}

bool Station::addPotIngredient(HeldItem item)
{
    if (item != HeldItem::TomatoChopped || m_potCooking || m_potReady || m_potIngredients >= 3) {
        return false;
    }

    ++m_potIngredients;
    if (m_potIngredients >= 3) {
        m_potCooking = true;
        m_potProgress = 0.0;
    }

    update();
    return true;
}

void Station::advancePot(qreal deltaSeconds)
{
    if (!m_potCooking || m_potReady) {
        return;
    }

    m_potProgress += deltaSeconds / kPotCookSeconds;
    if (m_potProgress >= 1.0) {
        m_potProgress = 1.0;
        m_potCooking = false;
        m_potReady = true;
    }

    update();
}

bool Station::canPlateSoup() const
{
    return m_potReady;
}

bool Station::plateSoup()
{
    if (!m_potReady) {
        return false;
    }

    m_potIngredients = 0;
    m_potProgress = 0.0;
    m_potCooking = false;
    m_potReady = false;
    update();
    return true;
}

int Station::potIngredientCount() const
{
    return m_potIngredients;
}

qreal Station::potCookProgress() const
{
    return m_potProgress;
}

bool Station::isPotReady() const
{
    return m_potReady;
}

QColor Station::fillColor() const
{
    switch (m_type) {
    case StationType::IngredientCrate:
        return colorIngredient();
    case StationType::CuttingBoard:
        return colorBoard();
    case StationType::Pot:
        return colorPotStation();
    case StationType::PlateCrate:
        return colorPlateStation();
    case StationType::DeliveryZone:
        return colorDelivery();
    case StationType::Counter:
    default:
        return colorCounter();
    }
}

void Station::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    drawRoundedCard(painter, m_rect.adjusted(2.0, 2.0, -2.0, -2.0), fillColor());

    switch (m_type) {
    case StationType::IngredientCrate:
        drawCounterDecoration(painter);
        drawFoodIcon(painter, HeldItem::TomatoRaw, QRectF(16.0, 12.0, 32.0, 32.0));
        break;
    case StationType::CuttingBoard:
        drawBoardDecoration(painter);
        if (m_boardItem != HeldItem::None) {
            drawFoodIcon(painter, m_boardItem, QRectF(16.0, 16.0, 32.0, 32.0));
        }
        if (m_boardItem == HeldItem::TomatoRaw) {
            painter->setBrush(QColor(255, 255, 255));
            painter->drawRoundedRect(QRectF(10.0, 52.0, 44.0, 6.0), 3.0, 3.0);
            painter->setBrush(colorLeaf());
            const qreal width = (44.0 / maxChopProgress()) * m_chopProgress;
            painter->drawRoundedRect(QRectF(10.0, 52.0, width, 6.0), 3.0, 3.0);
        }
        break;
    case StationType::Pot:
        drawPotDecoration(painter);
        break;
    case StationType::PlateCrate:
        drawPlateDecoration(painter);
        break;
    case StationType::DeliveryZone:
        drawDeliveryDecoration(painter);
        break;
    case StationType::Counter:
    default:
        drawCounterDecoration(painter);
        if (m_counterItem != HeldItem::None) {
            drawFoodIcon(painter, m_counterItem, QRectF(16.0, 14.0, 32.0, 32.0));
        }
        break;
    }
}

void Station::drawCounterDecoration(QPainter *painter) const
{
    painter->setPen(outlinePen());
    painter->setBrush(QColor(255, 255, 255, 120));
    painter->drawRoundedRect(QRectF(8.0, 40.0, 48.0, 12.0), 6.0, 6.0);
}

void Station::drawBoardDecoration(QPainter *painter) const
{
    painter->setPen(outlinePen());
    painter->setBrush(QColor(255, 244, 214));
    painter->drawRoundedRect(QRectF(8.0, 10.0, 48.0, 44.0), 8.0, 8.0);
    painter->setBrush(QColor(255, 214, 153));
    painter->drawEllipse(QRectF(45.0, 16.0, 7.0, 7.0));
}

void Station::drawPotDecoration(QPainter *painter) const
{
    painter->setPen(outlinePen());
    painter->setBrush(QColor(120, 144, 156));
    painter->drawRoundedRect(QRectF(12.0, 15.0, 40.0, 28.0), 8.0, 8.0);
    painter->drawRoundedRect(QRectF(8.0, 20.0, 7.0, 14.0), 4.0, 4.0);
    painter->drawRoundedRect(QRectF(49.0, 20.0, 7.0, 14.0), 4.0, 4.0);

    if (m_potIngredients > 0 || m_potReady) {
        painter->setBrush(m_potReady ? colorSoup() : colorTomato());
        painter->drawEllipse(QRectF(18.0, 19.0, 28.0, 18.0));
    }

    painter->setBrush(QColor(255, 255, 255));
    painter->drawRoundedRect(QRectF(10.0, 50.0, 44.0, 6.0), 3.0, 3.0);

    QColor progressColor = colorLeaf();
    if (m_potReady) {
        progressColor = colorSoup();
    }

    painter->setBrush(progressColor);
    const qreal progressWidth = m_potReady ? 44.0 : 44.0 * m_potProgress;
    painter->drawRoundedRect(QRectF(10.0, 50.0, progressWidth, 6.0), 3.0, 3.0);

    for (int i = 0; i < m_potIngredients; ++i) {
        painter->setBrush(colorTomato());
        painter->drawEllipse(QRectF(10.0 + i * 14.0, 5.0, 10.0, 10.0));
    }
}

void Station::drawPlateDecoration(QPainter *painter) const
{
    painter->setPen(outlinePen());
    painter->setBrush(QColor(255, 255, 255));
    painter->drawEllipse(QRectF(12.0, 18.0, 28.0, 20.0));
    painter->drawEllipse(QRectF(20.0, 11.0, 28.0, 20.0));
    painter->drawEllipse(QRectF(18.0, 24.0, 28.0, 20.0));
}

void Station::drawDeliveryDecoration(QPainter *painter) const
{
    painter->setPen(outlinePen());
    painter->setBrush(QColor(255, 255, 255));
    painter->drawRoundedRect(QRectF(12.0, 12.0, 40.0, 28.0), 8.0, 8.0);
    painter->drawEllipse(QRectF(25.0, 18.0, 14.0, 14.0));
    painter->setBrush(QColor(255, 255, 255, 120));
    painter->drawRoundedRect(QRectF(18.0, 46.0, 28.0, 8.0), 4.0, 4.0);
}
