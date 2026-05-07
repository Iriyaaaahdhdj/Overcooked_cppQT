#pragma once

#include <QElapsedTimer>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QSet>
#include <QTimer>

class DialogOverlayItem;
class HudOverlayItem;
class KitchenItem;
class MenuOverlayItem;
class PlayerItem;
class TutorialOverlayItem;

class GameScene : public QGraphicsScene
{
public:
    static constexpr int WindowWidth = 1280;
    static constexpr int WindowHeight = 720;
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
    void createFloor();
    void createBoundaryWalls();
    void createKitchenFixtures();
    void addKitchenItem(int gridX, int gridY, int gridWidth, int gridHeight, int kind);
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
    QRectF playArea() const;
    QString trText(const QString &zh, const QString &en) const;
    bool isTimedMode() const;
    bool isBurnMode() const;

    QTimer m_tickTimer;
    QElapsedTimer m_elapsedTimer;
    QSet<int> m_pressedKeys;
    QList<KitchenItem *> m_kitchenItems;
    QList<PlayerItem *> m_players;
    MenuOverlayItem *m_menuOverlay;
    DialogOverlayItem *m_dialogOverlay;
    TutorialOverlayItem *m_tutorialOverlay;
    HudOverlayItem *m_hudOverlay;
    QGraphicsSimpleTextItem *m_statusText;
    QGraphicsSimpleTextItem *m_bannerText;
    SceneState m_state;
    SceneState m_previousState;
    int m_score;
    int m_deliveriesCompleted;
    int m_targetDeliveries;
    int m_starCount;
    qreal m_timeRemainingSeconds;
    qreal m_roundDurationSeconds;
    UiLanguage m_language;
    DifficultyMode m_difficulty;
};
