#include "GameScene.h"

#include <cmath>
#include <QColor>
#include <QCoreApplication>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include <QKeyEvent>
#include <QLineF>
#include <QtMath>

#include "DialogOverlayItem.h"
#include "GameTypes.h"
#include "HudOverlayItem.h"
#include "KitchenItem.h"
#include "MenuOverlayItem.h"
#include "PlayerItem.h"
#include "TutorialOverlayItem.h"

namespace {
bool itemCanInteractWithStation(CarryItemType carriedItem, const KitchenItem *station)
{
    if (station == nullptr) {
        return false;
    }

    switch (station->itemKind()) {
    case KitchenItem::TomatoStationKind:
    case KitchenItem::EggStationKind:
    case KitchenItem::PlateStationKind:
        return carriedItem == NoItem;
    case KitchenItem::ChoppingStationKind:
        return carriedItem == TomatoItem || carriedItem == NoItem;
    case KitchenItem::StoveKind:
        return carriedItem == ChoppedTomatoItem || carriedItem == EggItem || carriedItem == PlateItem || carriedItem == NoItem;
    case KitchenItem::DeliveryWindowKind:
        return carriedItem == SoupPlateItem;
    case KitchenItem::CounterKind:
        return true;
    case KitchenItem::FloorTileKind:
    case KitchenItem::BoundaryWallKind:
    default:
        return false;
    }
}
}

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_menuOverlay(nullptr)
    , m_dialogOverlay(nullptr)
    , m_tutorialOverlay(nullptr)
    , m_hudOverlay(nullptr)
    , m_statusText(nullptr)
    , m_bannerText(nullptr)
    , m_state(StartMenuState)
    , m_previousState(StartMenuState)
    , m_score(0)
    , m_deliveriesCompleted(0)
    , m_targetDeliveries(4)
    , m_starCount(0)
    , m_timeRemainingSeconds(120.0)
    , m_roundDurationSeconds(120.0)
    , m_language(ChineseUi)
    , m_difficulty(Difficulty1)
{
    setSceneRect(0.0, 0.0, WindowWidth, WindowHeight);
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setBackgroundBrush(QColor(247, 231, 198));

    buildScene();

    connect(&m_tickTimer, &QTimer::timeout, this, [this]() { tick(); });
    m_tickTimer.start(16);
    m_elapsedTimer.start();
}

void GameScene::buildScene()
{
    buildKitchen();
    createPlayers();
    createMenu();
}

void GameScene::buildKitchen()
{
    createFloor();
    createBoundaryWalls();
    createKitchenFixtures();
}

void GameScene::createPlayers()
{
    auto *playerOne = new PlayerItem(QColor(255, 160, 182),
                                     QColor(255, 233, 119),
                                     PlayerItem::Controls{Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_F});
    playerOne->resetPosition(QPointF(420.0, 518.0));
    playerOne->setVisible(false);
    addItem(playerOne);
    m_players.append(playerOne);

    auto *playerTwo = new PlayerItem(QColor(113, 194, 255),
                                     QColor(143, 232, 164),
                                     PlayerItem::Controls{Qt::Key_Up, Qt::Key_Down, Qt::Key_Left, Qt::Key_Right, Qt::Key_Slash});
    playerTwo->resetPosition(QPointF(840.0, 518.0));
    playerTwo->setVisible(false);
    addItem(playerTwo);
    m_players.append(playerTwo);
}

void GameScene::createMenu()
{
    m_menuOverlay = new MenuOverlayItem(WindowWidth, WindowHeight);
    addItem(m_menuOverlay);

    m_dialogOverlay = new DialogOverlayItem(WindowWidth, WindowHeight);
    m_dialogOverlay->setVisible(false);
    addItem(m_dialogOverlay);

    m_tutorialOverlay = new TutorialOverlayItem(WindowWidth, WindowHeight);
    m_tutorialOverlay->setVisible(false);
    addItem(m_tutorialOverlay);

    m_hudOverlay = new HudOverlayItem(WindowWidth, WindowHeight);
    m_hudOverlay->setVisible(false);
    addItem(m_hudOverlay);

    QFont helpFont;
    helpFont.setPointSize(10);
    helpFont.setBold(true);

    m_statusText = addSimpleText(QString(), helpFont);
    m_statusText->setBrush(QColor(70, 70, 70));
    m_statusText->setZValue(200.0);
    m_statusText->setPos(24.0, WindowHeight - 56.0);
    m_statusText->setVisible(false);

    QFont bannerFont;
    bannerFont.setPointSize(22);
    bannerFont.setBold(true);

    m_bannerText = addSimpleText(QString(), bannerFont);
    m_bannerText->setBrush(QColor(255, 255, 255));
    m_bannerText->setZValue(220.0);
    m_bannerText->setPos(260.0, 86.0);
    m_bannerText->setVisible(false);
}

void GameScene::createFloor()
{
    for (int row = 0; row < RowCount; ++row) {
        for (int column = 0; column < ColumnCount; ++column) {
            addKitchenItem(column, row, 1, 1, KitchenItem::FloorTileKind);
        }
    }
}

void GameScene::createBoundaryWalls()
{
    for (int column = 0; column < ColumnCount; ++column) {
        addKitchenItem(column, 0, 1, 1, KitchenItem::BoundaryWallKind);
        addKitchenItem(column, RowCount - 1, 1, 1, KitchenItem::BoundaryWallKind);
    }

    for (int row = 1; row < RowCount - 1; ++row) {
        addKitchenItem(0, row, 1, 1, KitchenItem::BoundaryWallKind);
        addKitchenItem(ColumnCount - 1, row, 1, 1, KitchenItem::BoundaryWallKind);
    }
}

void GameScene::createKitchenFixtures()
{
    addKitchenItem(4, 4, 2, 2, KitchenItem::TomatoStationKind);
    addKitchenItem(34, 4, 2, 2, KitchenItem::EggStationKind);
    addKitchenItem(4, 15, 2, 2, KitchenItem::PlateStationKind);
    addKitchenItem(34, 15, 2, 2, KitchenItem::PlateStationKind);

    addKitchenItem(11, 4, 2, 2, KitchenItem::ChoppingStationKind);
    addKitchenItem(27, 4, 2, 2, KitchenItem::ChoppingStationKind);

    addKitchenItem(8, 10, 2, 2, KitchenItem::CounterKind);
    addKitchenItem(30, 10, 2, 2, KitchenItem::CounterKind);
    addKitchenItem(14, 10, 2, 2, KitchenItem::StoveKind);
    addKitchenItem(17, 10, 2, 2, KitchenItem::StoveKind);
    addKitchenItem(20, 10, 2, 2, KitchenItem::StoveKind);
    addKitchenItem(23, 10, 2, 2, KitchenItem::StoveKind);
    addKitchenItem(14, 15, 2, 2, KitchenItem::CounterKind);
    addKitchenItem(17, 15, 2, 2, KitchenItem::CounterKind);
    addKitchenItem(20, 15, 2, 2, KitchenItem::CounterKind);
    addKitchenItem(23, 15, 2, 2, KitchenItem::CounterKind);

    addKitchenItem(19, 17, 2, 2, KitchenItem::DeliveryWindowKind);
}

void GameScene::addKitchenItem(int gridX, int gridY, int gridWidth, int gridHeight, int kind)
{
    auto *item = new KitchenItem(static_cast<KitchenItem::Kind>(kind), gridX, gridY, gridWidth, gridHeight);
    addItem(item);
    m_kitchenItems.append(item);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        m_pressedKeys.insert(event->key());
    }

    if (m_state == StartMenuState) {
        if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
            m_menuOverlay->setSelectedIndex((m_menuOverlay->selectedIndex() + MenuOverlayItem::MenuCount - 1)
                                            % MenuOverlayItem::MenuCount);
        } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
            m_menuOverlay->setSelectedIndex((m_menuOverlay->selectedIndex() + 1) % MenuOverlayItem::MenuCount);
        } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
            if (m_menuOverlay->selectedIndex() == 0) {
                m_menuOverlay->setLanguageOption((m_menuOverlay->languageOption() + 1) % 2);
            } else if (m_menuOverlay->selectedIndex() == 1) {
                m_menuOverlay->setDifficultyOption((m_menuOverlay->difficultyOption() + 2) % 3);
            }
        } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
            if (m_menuOverlay->selectedIndex() == 0) {
                m_menuOverlay->setLanguageOption((m_menuOverlay->languageOption() + 1) % 2);
            } else if (m_menuOverlay->selectedIndex() == 1) {
                m_menuOverlay->setDifficultyOption((m_menuOverlay->difficultyOption() + 1) % 3);
            }
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
                   || event->key() == Qt::Key_Space || event->key() == Qt::Key_F
                   || event->key() == Qt::Key_Slash) {
            activateMenuSelection();
        } else if (event->key() == Qt::Key_Escape) {
            openExitDialog();
        }
    } else if (m_state == TutorialState) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
            || event->key() == Qt::Key_Space || event->key() == Qt::Key_F
            || event->key() == Qt::Key_Slash) {
            beginRound();
        } else if (event->key() == Qt::Key_Escape) {
            showMenu();
        }
    } else if (m_state == PlayingState) {
        if (event->key() == Qt::Key_Escape) {
            openExitDialog();
        } else if (!event->isAutoRepeat()) {
            for (PlayerItem *player : m_players) {
                if (event->key() == player->controls().interact) {
                    tryInteract(player);
                    break;
                }
            }
        }
    } else if (m_state == ExitDialogState) {
        if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A
            || event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
            m_dialogOverlay->setSelectedButton(0);
        } else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D
                   || event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
            m_dialogOverlay->setSelectedButton(1);
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
                   || event->key() == Qt::Key_Space || event->key() == Qt::Key_F
                   || event->key() == Qt::Key_Slash) {
            confirmDialogSelection();
        } else if (event->key() == Qt::Key_Escape) {
            closeExitDialog();
        }
    } else if (m_state == RoundEndState) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
            || event->key() == Qt::Key_Space || event->key() == Qt::Key_F
            || event->key() == Qt::Key_Slash || event->key() == Qt::Key_Escape) {
            showMenu();
        }
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
    if (deltaSeconds > 0.05) {
        deltaSeconds = 0.05;
    }

    if (m_state != PlayingState) {
        return;
    }

    if (isTimedMode()) {
        m_timeRemainingSeconds -= deltaSeconds;
        if (m_timeRemainingSeconds <= 0.0) {
            m_timeRemainingSeconds = 0.0;
            finishRound(m_deliveriesCompleted >= m_targetDeliveries);
            return;
        }
    }

    const QRectF bounds = playArea();
    for (KitchenItem *item : m_kitchenItems) {
        item->tick(deltaSeconds, isBurnMode());
    }

    for (PlayerItem *player : m_players) {
        player->tick(m_pressedKeys, deltaSeconds, bounds);
    }

    updateHud();
}

void GameScene::startGame()
{
    m_state = TutorialState;
    m_menuOverlay->setVisible(false);
    m_dialogOverlay->setVisible(false);
    m_tutorialOverlay->setVisible(true);
    m_pressedKeys.clear();

    if (m_players.size() >= 2) {
        m_players.at(0)->resetPosition(QPointF(420.0, 518.0));
        m_players.at(1)->resetPosition(QPointF(840.0, 518.0));
    }

    m_score = 0;
    m_deliveriesCompleted = 0;
    m_starCount = 0;
    m_language = m_menuOverlay->languageOption() == MenuOverlayItem::ChineseLanguage ? ChineseUi : EnglishUi;
    m_difficulty = static_cast<DifficultyMode>(m_menuOverlay->difficultyOption());
    applyDifficultySettings();
    m_timeRemainingSeconds = isTimedMode() ? m_roundDurationSeconds : -1.0;
    resetKitchenState();
    for (PlayerItem *player : m_players) {
        player->setCarriedItem(NoItem);
        player->setVisible(false);
    }

    m_hudOverlay->setVisible(true);
    m_statusText->setVisible(true);
    m_bannerText->setVisible(false);
    m_tutorialOverlay->setLanguageChinese(m_language == ChineseUi);
    m_tutorialOverlay->setDifficultyMode(static_cast<int>(m_difficulty));
    updateHud();
}

void GameScene::beginRound()
{
    m_state = PlayingState;
    m_tutorialOverlay->setVisible(false);
    m_bannerText->setVisible(false);
    for (PlayerItem *player : m_players) {
        player->setVisible(true);
    }
    updateHud();
}

void GameScene::showMenu()
{
    m_state = StartMenuState;
    m_menuOverlay->setVisible(true);
    m_dialogOverlay->setVisible(false);
    m_tutorialOverlay->setVisible(false);
    m_pressedKeys.clear();
    for (PlayerItem *player : m_players) {
        player->setCarriedItem(NoItem);
        player->setVisible(false);
    }

    m_hudOverlay->setVisible(false);
    m_statusText->setVisible(false);
    m_bannerText->setVisible(false);
}

void GameScene::resetKitchenState()
{
    for (KitchenItem *item : m_kitchenItems) {
        item->resetState();
    }
}

void GameScene::updateHud()
{
    if (m_state != PlayingState && m_state != TutorialState && m_state != RoundEndState) {
        return;
    }

    const int wholeSeconds = isTimedMode() ? qMax(0, static_cast<int>(std::ceil(m_timeRemainingSeconds))) : -1;
    m_hudOverlay->setStats(m_score,
                           wholeSeconds,
                           m_deliveriesCompleted,
                           m_targetDeliveries,
                           trText(QStringLiteral("番茄鸡蛋汤"), QStringLiteral("Tomato Egg Soup")),
                           m_language == ChineseUi);
    if (m_players.size() < 2) {
        return;
    }

    const QString playerOne = carryItemLabel(m_players.at(0)->carriedItem());
    const QString playerTwo = carryItemLabel(m_players.at(1)->carriedItem());
    m_statusText->setText(trText(QStringLiteral("P1[F]: %1    P2[/]: %2"),
                                 QStringLiteral("P1[F]: %1    P2[/]: %2"))
                              .arg(playerOne, playerTwo));
}

void GameScene::finishRound(bool success)
{
    m_state = RoundEndState;
    for (PlayerItem *player : m_players) {
        player->setVisible(true);
    }

    m_starCount = success ? calculateStarCount() : 0;
    const QString stars = QStringLiteral("%1/3").arg(m_starCount);
    const bool unlocked = success && m_starCount >= 1;

    if (success) {
        m_bannerText->setText(trText(QStringLiteral("关卡完成！\n星级：%1\n已完成 %2 份出餐。\n%3"),
                                     QStringLiteral("Level Clear!\nStars: %1\nDelivered %2 soups.\n%3"))
                                  .arg(stars)
                                  .arg(m_deliveriesCompleted)
                                  .arg(unlocked
                                           ? trText(QStringLiteral("达到一星，已满足进入下一关条件。按确认键返回菜单。"),
                                                    QStringLiteral("At least 1 star earned. Next level unlocked. Press confirm to return."))
                                           : trText(QStringLiteral("未达到一星，暂时不能进入下一关。按确认键返回菜单。"),
                                                    QStringLiteral("Less than 1 star. Next level stays locked. Press confirm to return."))));
    } else {
        m_bannerText->setText(trText(QStringLiteral("关卡失败！\n星级：0/3\n已完成 %1 / %2 份出餐。\n按确认键返回菜单。"),
                                     QStringLiteral("Level Failed!\nStars: 0/3\nDelivered %1 / %2 soups.\nPress confirm to return."))
                                  .arg(m_deliveriesCompleted)
                                  .arg(m_targetDeliveries));
    }
    m_bannerText->setVisible(true);
    updateHud();
}

void GameScene::tryInteract(PlayerItem *player)
{
    KitchenItem *item = findInteractiveItem(player);
    if (item == nullptr) {
        return;
    }

    switch (item->itemKind()) {
    case KitchenItem::TomatoStationKind:
        if (player->isHandsEmpty()) {
            player->setCarriedItem(TomatoItem);
        }
        break;
    case KitchenItem::EggStationKind:
        if (player->isHandsEmpty()) {
            player->setCarriedItem(EggItem);
        }
        break;
    case KitchenItem::PlateStationKind:
        if (player->isHandsEmpty()) {
            player->setCarriedItem(PlateItem);
        }
        break;
    case KitchenItem::CounterKind:
        if (player->isHandsEmpty() && item->storedItem() != NoItem) {
            player->setCarriedItem(item->storedItem());
            item->resetState();
        } else if (!player->isHandsEmpty() && item->storedItem() == NoItem) {
            item->setStoredItem(player->carriedItem());
            item->setStoredCount(1);
            player->setCarriedItem(NoItem);
        }
        break;
    case KitchenItem::ChoppingStationKind:
        if (player->carriedItem() == TomatoItem && item->storedItem() == NoItem) {
            item->setStoredItem(TomatoItem);
            item->setStoredCount(1);
            item->setStationProgress(0.0);
            player->setCarriedItem(NoItem);
        } else if (player->isHandsEmpty() && item->storedItem() == TomatoItem) {
            item->setStationProgress(item->stationProgress() + 0.5);
            if (item->stationProgress() >= 1.0) {
                item->setStoredItem(ChoppedTomatoItem);
                item->setStoredCount(1);
            }
        } else if (player->isHandsEmpty() && item->storedItem() == ChoppedTomatoItem) {
            player->setCarriedItem(ChoppedTomatoItem);
            item->resetState();
        }
        break;
    case KitchenItem::StoveKind:
        if (item->isBurnt() && player->isHandsEmpty()) {
            item->resetState();
            break;
        }
        if (player->carriedItem() == ChoppedTomatoItem && !item->soupReady() && item->tomatoCount() < 2) {
            item->setStoredItem(ChoppedTomatoItem);
            item->setTomatoCount(item->tomatoCount() + 1);
            item->setStationProgress(0.0);
            player->setCarriedItem(NoItem);
        } else if (player->carriedItem() == EggItem && !item->soupReady() && item->eggCount() < 1) {
            item->setStoredItem(EggItem);
            item->setEggCount(item->eggCount() + 1);
            item->setStationProgress(0.0);
            player->setCarriedItem(NoItem);
        } else if (item->soupReady() && player->carriedItem() == PlateItem) {
            player->setCarriedItem(SoupPlateItem);
            item->resetState();
        }
        break;
    case KitchenItem::DeliveryWindowKind:
        if (player->carriedItem() == SoupPlateItem) {
            m_score += 20;
            ++m_deliveriesCompleted;
            player->setCarriedItem(NoItem);
            if (m_deliveriesCompleted >= m_targetDeliveries) {
                finishRound(true);
            }
        }
        break;
    case KitchenItem::FloorTileKind:
    case KitchenItem::BoundaryWallKind:
        break;
    }

    updateHud();
}

KitchenItem *GameScene::findInteractiveItem(const PlayerItem *player) const
{
    const QPointF origin = player->scenePos() + QPointF(0.0, 6.0);
    const QPointF facing = player->facingVector();
    const QPointF probeCenter = origin + facing * 62.0;
    const QRectF probeRect(probeCenter.x() - 28.0, probeCenter.y() - 28.0, 56.0, 56.0);
    const CarryItemType carriedItem = player->carriedItem();

    KitchenItem *bestItem = nullptr;
    qreal bestScore = 1e9;
    for (KitchenItem *item : m_kitchenItems) {
        if (item->itemKind() == KitchenItem::FloorTileKind || item->itemKind() == KitchenItem::BoundaryWallKind) {
            continue;
        }

        const QRectF sceneRect = item->mapToScene(item->boundingRect()).boundingRect();
        if (!sceneRect.intersects(probeRect)) {
            continue;
        }

        const qreal distance = QLineF(origin, sceneRect.center()).length();
        const qreal compatibilityPenalty = itemCanInteractWithStation(carriedItem, item) ? 0.0 : 1000.0;
        const qreal score = distance + compatibilityPenalty;
        if (score < bestScore) {
            bestScore = score;
            bestItem = item;
        }
    }

    return bestItem;
}

void GameScene::openExitDialog()
{
    m_previousState = m_state;
    m_state = ExitDialogState;
    m_dialogOverlay->setSelectedButton(1);
    m_dialogOverlay->setVisible(true);
    if (m_statusText != nullptr && m_state == ExitDialogState) {
        m_statusText->setText(QStringLiteral("Game paused"));
    }
}

void GameScene::closeExitDialog()
{
    m_state = m_previousState;
    m_dialogOverlay->setVisible(false);
    updateHud();
}

void GameScene::activateMenuSelection()
{
    const int selected = m_menuOverlay->selectedIndex();
    if (selected == 2) {
        startGame();
    } else if (selected == 3) {
        openExitDialog();
    }
}

void GameScene::confirmDialogSelection()
{
    if (m_dialogOverlay->selectedButton() == 0) {
        QCoreApplication::quit();
        return;
    }

    closeExitDialog();
}

QRectF GameScene::playArea() const
{
    return QRectF(32.0, 32.0, WindowWidth - 64.0, WindowHeight - 64.0);
}

void GameScene::applyDifficultySettings()
{
    if (m_difficulty == Difficulty1) {
        m_targetDeliveries = 4;
        m_roundDurationSeconds = -1.0;
    } else if (m_difficulty == Difficulty2) {
        m_targetDeliveries = 5;
        m_roundDurationSeconds = 120.0;
    } else {
        m_targetDeliveries = 6;
        m_roundDurationSeconds = 135.0;
    }
}

QString GameScene::trText(const QString &zh, const QString &en) const
{
    return m_language == ChineseUi ? zh : en;
}

bool GameScene::isTimedMode() const
{
    return m_difficulty != Difficulty1;
}

bool GameScene::isBurnMode() const
{
    return m_difficulty == Difficulty3;
}

int GameScene::calculateStarCount() const
{
    if (m_deliveriesCompleted < m_targetDeliveries) {
        return 0;
    }

    if (!isTimedMode() || m_roundDurationSeconds <= 0.0) {
        return 1;
    }

    const qreal ratio = qBound(0.0, m_timeRemainingSeconds / m_roundDurationSeconds, 1.0);
    if (ratio >= 0.45) {
        return 3;
    }
    if (ratio >= 0.2) {
        return 2;
    }
    return 1;
}
