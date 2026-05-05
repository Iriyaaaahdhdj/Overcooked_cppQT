#pragma once

#include <QWidget>

class GameScene;
class GameView;

class GameWindow : public QWidget
{
public:
    GameWindow();

private:
    GameScene *m_scene;
    GameView *m_view;
};
