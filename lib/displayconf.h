#pragma once

#include <QWidget>
#include <QTcpSocket>

class QLabel;
class QCheckBox;
class QLineEdit;
class QComboBox;

class DisplayConf : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayConf(QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void scheduleReconnect();
private:
    QLabel* connStatus;
    QCheckBox* enabled;
    QLineEdit* host;
    QComboBox* role;

    QTcpSocket socket;
};

