#ifndef TYPEWRITERLABEL_H
#define TYPEWRITERLABEL_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class TypewriterLabel : public QLabel {
    Q_OBJECT

public:
    explicit TypewriterLabel(QWidget *parent = nullptr);
    void setTextToShow(const QString &text);
    void startTypewriterEffect();

private slots:
    void showNextCharacter();

private:
    QString fullText;
    int currentIndex;
    QTimer *timer;
};

#endif // TYPEWRITERLABEL_H
