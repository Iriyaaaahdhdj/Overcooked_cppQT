#include "gamescene.h"

#include <QKeyEvent>
#include <QLineF>
#include <QtMath>

#include "gameconstants.h"
#include "huditem.h"
#include "player.h"
#include "station.h"
#include "tileitem.h"

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_playerOne(nullptr)
    , m_playerTwo(nullptr)
    , m_hud(nullptr)
    , m_score(0)
    , m_ordersDelivered(0)
    , m_timeLeft(180.0)
    , m_gameOver(false)
{
    setSceneRect(0.0, 0.0, kSceneWidth, kSceneHeight);
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setBackgroundBrush(colorBackground());

    buildKitchen();
    refreshHud();

    connect(&m_tickTimer, &QTimer::timeout, this, [this]() { tick(); });
    m_tickTimer.start(16);
    m_elapsedTimer.start();
}

void GameScene::buildKitchen()
{
    for (int row = 0; row < kKitchenRows; ++row) {
        for (int column = 0; column < kKitchenColumns; ++column) {
            const bool alternate = (row + column) % 2 == 0;
            addFloorTile(column, row, alternate ? colorFloorA() : colorFloorB());
        }
    }

    m_hud = new HudItem(kSceneWidth);
    addItem(m_hud);

    const auto worldRect = [](int column, int row) {
        return QRectF(column * kTileSize, kHudHeight + row * kTileSize, kTileSize, kTileSize);
    };

    const QList<int> topRowStations = {0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13};
    for (int column : topRowStations) {
        StationType type = StationType::Counter;
        if (column == 1 || column == 12) {
            type = StationType::IngredientCrate;
        } else if (column == 3 || column == 10) {
            type = StationType::CuttingBoard;
        } else if (column == 5 || column == 8) {
            type = StationType::Pot;
        }
        addStation(new Station(type, worldRect(column, 0)));
    }

    const QList<int> bottomRowStations = {0, 1, 2, 3, 4, 6, 7, 9, 10, 11, 12, 13};
    for (int column : bottomRowStations) {
        StationType type = StationType::Counter;
        if (column == 1 || column == 12) {
            type = StationType::PlateCrate;
        } else if (column == 6 || column == 7) {
            type = StationType::DeliveryZone;
        }
        addStation(new Station(type, worldRect(column, 8)));
    }

    const QList<QPoint> islandCounters = {
        QPoint(4, 3), QPoint(5, 3), QPoint(8, 3), QPoint(9, 3),
        QPoint(2, 5), QPoint(3, 5), QPoint(10, 5), QPoint(11, 5)
    };
    for (const QPoint &cell : islandCounters) {
        addStation(new Station(StationType::Counter, worldRect(cell.x(), cell.y())));
    }

    m_playerOne = new Player(QStringLiteral("Chef 1"),
                             QColor(255, 138, 128),
                             PlayerControls{Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_F});
    m_playerOne->setPos(4.5 * kTileSize, kHudHeight + 4.6 * kTileSize);
    addItem(m_playerOne);

    m_playerTwo = new Player(QStringLiteral("Chef 2"),
                             QColor(128, 222, 234),
                             PlayerControls{Qt::Key_Up, Qt::Key_Down, Qt::Key_Left, Qt::Key_Right, Qt::Key_Slash});
    m_playerTwo->setPos(9.5 * kTileSize, kHudHeight + 4.6 * kTileSize);
    addItem(m_playerTwo);
}

void GameScene::addFloorTile(int column, int row, const QColor &color)
{
    auto *tile = new TileItem(QRectF(column * kTileSize, kHudHeight + row * kTileSize, kTileSize, kTileSize), color);
    addItem(tile);
}

void GameScene::addStation(Station *station)
{
    m_stations.append(station);
    addItem(station);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        m_pressedKeys.insert(event->key());
    }

    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        m_pressedKeys.remove(event->key());
    }

    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::tick()
{
    qreal deltaSeconds = m_elapsedTimer.restart() / 1000.0;
    deltaSeconds = qBound(0.0, deltaSeconds, 0.05);

    if (!m_gameOver) {
        m_timeLeft -= deltaSeconds;
        if (m_timeLeft <= 0.0) {
            m_timeLeft = 0.0;
            m_gameOver = true;
        } else {
            m_playerOne->tick(m_pressedKeys, deltaSeconds);
            m_playerTwo->tick(m_pressedKeys, deltaSeconds);
        }
    }

    for (Station *station : m_stations) {
        if (station->stationType() == StationType::Pot) {
            station->advancePot(deltaSeconds);
        }
    }

    const bool p1Interact = m_pressedKeys.contains(m_playerOne->controls().interact)
                            && !m_previousKeys.contains(m_playerOne->controls().interact);
    const bool p2Interact = m_pressedKeys.contains(m_playerTwo->controls().interact)
                            && !m_previousKeys.contains(m_playerTwo->controls().interact);

    if (!m_gameOver && p1Interact) {
        handlePlayerInteraction(m_playerOne);
    }
    if (!m_gameOver && p2Interact) {
        handlePlayerInteraction(m_playerTwo);
    }

    m_previousKeys = m_pressedKeys;
    refreshHud();
}

void GameScene::handlePlayerInteraction(Player *player)
{
    Station *station = findFacingStation(player);
    if (station == nullptr) {
        return;
    }

    switch (station->stationType()) {
    case StationType::IngredientCrate:
        if (player->isHandsEmpty()) {
            player->setHeldItem(HeldItem::TomatoRaw);
        }
        break;

    case StationType::PlateCrate:
        if (player->isHandsEmpty()) {
            player->setHeldItem(HeldItem::Plate);
        }
        break;

    case StationType::Counter:
        if (player->isHandsEmpty() && station->counterItem() != HeldItem::None) {
            player->setHeldItem(station->takeCounterItem());
        } else if (!player->isHandsEmpty() && station->counterItem() == HeldItem::None) {
            station->setCounterItem(player->heldItem());
            player->setHeldItem(HeldItem::None);
        }
        break;

    case StationType::CuttingBoard:
        if (player->heldItem() == HeldItem::TomatoRaw && station->boardItem() == HeldItem::None) {
            if (station->placeOnBoard(player->heldItem())) {
                player->setHeldItem(HeldItem::None);
            }
        } else if (player->isHandsEmpty() && station->boardItem() == HeldItem::TomatoRaw) {
            station->chopOnce();
        } else if (player->isHandsEmpty() && station->boardItem() == HeldItem::TomatoChopped) {
            player->setHeldItem(station->takeBoardItem());
        }
        break;

    case StationType::Pot:
        if (player->heldItem() == HeldItem::TomatoChopped) {
            if (station->addPotIngredient(player->heldItem())) {
                player->setHeldItem(HeldItem::None);
            }
        } else if (player->heldItem() == HeldItem::Plate && station->canPlateSoup()) {
            if (station->plateSoup()) {
                player->setHeldItem(HeldItem::SoupPlated);
            }
        }
        break;

    case StationType::DeliveryZone:
        if (player->heldItem() == HeldItem::SoupPlated) {
            player->setHeldItem(HeldItem::None);
            m_score += 20;
            ++m_ordersDelivered;
        }
        break;
    }
}

Station *GameScene::findFacingStation(const Player *player) const
{
    const QPointF samplePoint = player->scenePos() + directionVector(player->facing()) * 44.0;

    Station *bestStation = nullptr;
    qreal bestDistance = 1.0e9;

    for (Station *station : m_stations) {
        const QRectF area = station->sceneBoundingRect().adjusted(-10.0, -10.0, 10.0, 10.0);
        if (!area.contains(samplePoint)) {
            continue;
        }

        const qreal distance = QLineF(samplePoint, station->sceneBoundingRect().center()).length();
        if (distance < bestDistance) {
            bestDistance = distance;
            bestStation = station;
        }
    }

    return bestStation;
}

void GameScene::refreshHud()
{
    m_hud->setScore(m_score);
    m_hud->setOrders(m_ordersDelivered);
    m_hud->setTimeLeft(m_timeLeft);
    m_hud->setGameOver(m_gameOver);
}
