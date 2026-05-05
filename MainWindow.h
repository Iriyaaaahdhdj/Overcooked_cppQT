#pragma once

#include <QMainWindow>

class QGraphicsView;
class GameScene;

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private:
    GameScene *m_scene;
    QGraphicsView *m_view;
};
