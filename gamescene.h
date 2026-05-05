#pragma once

#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QSet>
#include <QTimer>

class HudItem;
class Player;
class Station;

class GameScene : public QGraphicsScene
{
public:
    explicit GameScene(QObject *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void buildKitchen();
    void addFloorTile(int column, int row, const QColor &color);
    void addStation(Station *station);
    void tick();
    void handlePlayerInteraction(Player *player);
    Station *findFacingStation(const Player *player) const;
    void refreshHud();

    QTimer m_tickTimer;
    QElapsedTimer m_elapsedTimer;
    QSet<int> m_pressedKeys;
    QSet<int> m_previousKeys;
    QList<Station *> m_stations;
    Player *m_playerOne;
    Player *m_playerTwo;
    HudItem *m_hud;
    int m_score;
    int m_ordersDelivered;
    qreal m_timeLeft;
    bool m_gameOver;
};
