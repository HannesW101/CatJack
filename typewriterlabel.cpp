#include "typewriterlabel.h"

TypewriterLabel::TypewriterLabel(QWidget *parent)
    : QLabel(parent), currentIndex(0) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TypewriterLabel::showNextCharacter);
    setAlignment(Qt::AlignCenter); // Center align the text
    setStyleSheet("background: transparent; border: none; font-size: 18px; padding-bottom: 10px;");
}

void TypewriterLabel::setTextToShow(const QString &text) {
    fullText = text;
    currentIndex = 0;
    setText("");
}

void TypewriterLabel::startTypewriterEffect() {
    timer->start(100); // Adjust the interval for speed
}

void TypewriterLabel::showNextCharacter() {
    if (currentIndex < fullText.length()) {
        setText(text() + fullText[currentIndex]);
        currentIndex++;
    } else {
        timer->stop();
    }
}
