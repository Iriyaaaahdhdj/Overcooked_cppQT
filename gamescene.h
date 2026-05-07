#pragma once

#include <QElapsedTimer>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QList>
#include <QSet>
#include <QTimer>

class DialogOverlayItem;
class HudOverlayItem;
class KitchenItem;
class MenuOverlayItem;
class PlayerItem;
class QGraphicsItem;
class ResultOverlayItem;
class TutorialOverlayItem;

class GameScene : public QGraphicsScene
{
public:
    static constexpr int WindowWidth = 1206;
    static constexpr int WindowHeight = 748;
    static constexpr int BASE_GRID_WIDTH = 64;
    static constexpr int BASE_GRID_HEIGHT = 32;
    static constexpr int SCENE_COLS = 12;
    static constexpr int SCENE_ROWS = 9;
    static constexpr int GridSize = 32;
    static constexpr int ColumnCount = WindowWidth / GridSize;
    static constexpr int RowCount = 23;

    explicit GameScene(QObject *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    enum UiLanguage {
        ChineseUi,
        EnglishUi
    };

    enum DifficultyMode {
        Difficulty1,
        Difficulty2,
        Difficulty3
    };

    enum SceneState {
        StartMenuState,
        TutorialState,
        PlayingState,
        RoundEndState,
        ExitDialogState
    };

    void buildScene();
    void buildKitchen();
    void createPlayers();
    void createMenu();
    void drawUI();
    void createFloor();
    void drawGroundGrid();
    void drawStaticElements();
    void createBoundaryWalls();
    void createEnvironmentDecor();
    void createKitchenFixtures();
    void addKitchenItem(int gridX, int gridY, int gridWidth, int gridHeight, int kind);
    void addKitchenItemRect(const QRectF &rect, int kind);
    void refreshDishStationVisuals();
    void tick();
    void startGame();
    void beginRound();
    void showMenu();
    void resetKitchenState();
    void applyDifficultySettings();
    void updateHud();
    int calculateStarCount() const;
    void finishRound(bool success);
    void tryInteract(PlayerItem *player);
    KitchenItem *findInteractiveItem(const PlayerItem *player) const;
    void openExitDialog();
    void closeExitDialog();
    void activateMenuSelection();
    void confirmDialogSelection();
    void confirmResultSelection();
    void restartLevel();
    void goToNextLevel();
    QRectF playArea() const;
    QString trText(const QString &zh, const QString &en) const;
    bool isTimedMode() const;
    bool isBurnMode() const;
    bool hasNextLevel() const;

    QTimer m_tickTimer;
    QElapsedTimer m_elapsedTimer;
    QSet<int> m_pressedKeys;
    QList<KitchenItem *> m_kitchenItems;
    QList<PlayerItem *> m_players;
    MenuOverlayItem *m_menuOverlay;
    DialogOverlayItem *m_dialogOverlay;
    TutorialOverlayItem *m_tutorialOverlay;
    HudOverlayItem *m_hudOverlay;
    ResultOverlayItem *m_resultOverlay;
    QGraphicsSimpleTextItem *m_statusText;
    QGraphicsSimpleTextItem *m_bannerText;
    QGraphicsItem *m_dynamicEffectsItem;
    SceneState m_state;
    SceneState m_previousState;
    int m_score;
    int m_deliveriesCompleted;
    int m_targetDeliveries;
    int m_starCount;
    int m_currentLevel;
    int m_maxLevel;
    int m_totalPlateCount;
    int m_availableCleanPlates;
    int m_dirtyPlateCount;
    QList<qreal> m_pendingDirtyPlateTimers;
    qreal m_timeRemainingSeconds;
    qreal m_roundDurationSeconds;
    qreal m_effectPhase;
    UiLanguage m_language;
    DifficultyMode m_difficulty;
};
