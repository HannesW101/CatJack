#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // custom cursor for window
    QPixmap pixmap_cursor_normal = QPixmap(":/images/resources/images/Cat_Cursor_normal.png");
    QCursor customCursor = QCursor(pixmap_cursor_normal, 0, 0);
    w.setCursor(customCursor);

    // set window icon
    w.setWindowIcon(QIcon(":/images/resources/images/window_icon.png"));

    // setup custom font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Casino.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont customFont(fontFamily, 12);
    a.setFont(customFont);

    w.show();
    return a.exec();
}
