#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    ~ClickableLabel();

signals:
    void clicked();

protected:

};

#endif // CLICKABLELABEL_H
