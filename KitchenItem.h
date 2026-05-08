#pragma once

#include <QGraphicsItem>
#include <QPainterPath>

#include "GameTypes.h"

class KitchenItem : public QGraphicsItem
{
public:
    enum Kind {
        FloorTileKind = 0,
        BoundaryWallKind,
        StoveKind,
        ChoppingStationKind,
        DeliveryWindowKind,
        TomatoStationKind,
        EggStationKind,
        PlateStationKind,
        CleanPlateStationKind,
        SinkStationKind,
        DirtyPlateStationKind,
        GarbageStationKind,
        CounterKind
    };

    KitchenItem(Kind kind, int gridX, int gridY, int gridWidth, int gridHeight, QGraphicsItem *parent = nullptr);
    KitchenItem(Kind kind, const QRectF &sceneRect, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    Kind itemKind() const;
    bool blocksMovement() const;
    CarryItemType storedItem() const;
    void setStoredItem(CarryItemType item);
    QPointF storedItemSceneCenter() const;
    void setStoredItemSceneCenter(const QPointF &sceneCenter);
    void clearStoredItemSceneCenter();
    int storedCount() const;
    void setStoredCount(int count);
    qreal stationProgress() const;
    void setStationProgress(qreal progress);
    bool soupReady() const;
    void setSoupReady(bool ready);
    void tick(qreal deltaSeconds, bool allowBurning);
    void resetState();
    int gridX() const;
    int gridY() const;
    int tomatoCount() const;
    void setTomatoCount(int count);
    int eggCount() const;
    void setEggCount(int count);
    qreal burnProgress() const;
    bool isBurnt() const;

    static constexpr int GridSize = 32;
    static constexpr qreal CornerRadius = 8.0;

private:
    void drawBase(QPainter *painter, const QColor &fillColor) const;
    void drawFloor(QPainter *painter) const;
    void drawWall(QPainter *painter) const;
    void drawStove(QPainter *painter) const;
    void drawChoppingStation(QPainter *painter) const;
    void drawDeliveryWindow(QPainter *painter) const;
    void drawTomatoStation(QPainter *painter) const;
    void drawEggStation(QPainter *painter) const;
    void drawPlateStation(QPainter *painter) const;
    void drawCounter(QPainter *painter) const;
    void drawStationItem(QPainter *painter, const QRectF &rect) const;
    void drawProgressBar(QPainter *painter, const QRectF &rect, const QColor &fillColor) const;

    QColor fillColor() const;

    Kind m_kind;
    QRectF m_rect;
    int m_gridX;
    int m_gridY;
    CarryItemType m_storedItem;
    QPointF m_storedItemLocalCenter;
    bool m_hasStoredItemLocalCenter;
    int m_storedCount;
    int m_tomatoCount;
    int m_eggCount;
    qreal m_stationProgress;
    qreal m_burnProgress;
    qreal m_animTime;
    bool m_soupReady;
    bool m_isBurnt;
};
