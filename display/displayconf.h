#pragma once

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
#include "protocol.h"
#include "LineParser.h"

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
    QCheckBox* toSpeech;
    QCheckBox* soundPlayer;
    scoreboard::Protocol::DisplayFeatures features;

    QTcpSocket socket;
    QTimer reconnect;
    LineParser in;

    void sendLine(QString line);
    void sendRole();
    void sendFeatures();
};