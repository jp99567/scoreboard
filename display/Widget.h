#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QLabel;
class QResizeEvent;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QLabel* main;
    QLabel* aux;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // WIDGET_H
