#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStringLiteral("Fusion"));
    app.setApplicationName(QStringLiteral("胡闹厨房 overcooked!"));

    MainWindow window;
    window.show();

    return app.exec();
}
