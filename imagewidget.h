#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStringList>

#include "ui_MainWindow.h"

class ImageWidget : public QWidget {
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void setImageList(const QStringList &imageList);
    QPushButton* getNextButton() const;
    QPushButton* getPrevButton() const;
    int get_currentIndex() const;

private slots:
    void showNextImage();
    void showPrevImage();

private:
    QPushButton *imageButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QStringList images;
    int currentIndex;
};

#endif // IMAGEWIDGET_H
