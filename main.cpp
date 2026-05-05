#include <QApplication>

#include "gamewindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStringLiteral("Fusion"));

    GameWindow window;
    window.show();

    return app.exec();
}
