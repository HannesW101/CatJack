#include "imagewidget.h"
#include <QPixmap>
#include <QHBoxLayout>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent), currentIndex(0) {
    imageButton = new QPushButton(this);
    imageButton->setIconSize(QSize(80, 80));
    imageButton->setFixedSize(QSize(100, 100));

    // set custom cursor for hover ( custom cursor for window is set in main.cpp
    QPixmap pixmap_cursor_hover = QPixmap(":/images/resources/images/Cat_Cursor_hover.png");
    QCursor customCursor_hover = QCursor(pixmap_cursor_hover, 0, 0);

    nextButton = new QPushButton(">", this);
    nextButton->setCursor(customCursor_hover);

    prevButton = new QPushButton("<", this);
    prevButton->setCursor(customCursor_hover);

    // Main layout for the widget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Center the imageButton horizontally
    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addStretch();
    imageLayout->addWidget(imageButton);
    imageLayout->addStretch();
    mainLayout->addLayout(imageLayout);

    // Add some space between the image button and navigation buttons
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(verticalSpacer);

    // Layout for the navigation buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(prevButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(nextButton, &QPushButton::clicked, this, &ImageWidget::showNextImage);
    connect(prevButton, &QPushButton::clicked, this, &ImageWidget::showPrevImage);

    // Apply custom stylesheet
    nextButton->setStyleSheet(
        "QPushButton {"
        "border-radius: 15px;"
        "font-size: 18px;"
        "background-color: #E4A700;"
        "border: 2px solid white;"
        "}"
    );

    prevButton->setStyleSheet(
        "QPushButton {"
        "border-radius: 15px;"
        "font-size: 18px;"
        "background-color: #E4A700;"
        "border: 2px solid white;"
        "}"
    );

    imageButton->setStyleSheet(
        "QPushButton {"
        "border-radius: 15px;"
        "font-size: 18px;"
        "background-color: #E4A700;"
        "border: 2px solid white;"
        "}"
    );
}

void ImageWidget::setImageList(const QStringList &imageList) {
    images = imageList;
    if (!images.isEmpty()) {
        imageButton->setIcon(QIcon(images.at(0)));
        currentIndex = 0;
    }
}

QPushButton *ImageWidget::getNextButton() const
{
    return nextButton;
}

QPushButton *ImageWidget::getPrevButton() const
{
    return prevButton;
}

int ImageWidget::get_currentIndex() const
{
    return currentIndex;
}

void ImageWidget::showNextImage() {
    if (images.isEmpty()) return;
    currentIndex = (currentIndex + 1) % images.size();
    imageButton->setIcon(QIcon(images.at(currentIndex)));
}

void ImageWidget::showPrevImage() {
    if (images.isEmpty()) return;
    currentIndex = (currentIndex - 1 + images.size()) % images.size();
    imageButton->setIcon(QIcon(images.at(currentIndex)));
}
