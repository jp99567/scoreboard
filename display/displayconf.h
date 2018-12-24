#pragma once

#include <memory>
#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
#include "protocol.h"
#include "LineParser.h"

class QLabel;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QSpinBox;
class QRadioButton;
//class QTextToSpeech;

class DisplayConf : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayConf(QWidget *parent = nullptr);
    ~DisplayConf();

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
    QPushButton *plusA, *plusB;
    QSpinBox *scoreL, *scoreR, *setL, *setR;
    QRadioButton *servisNon, *servisL, *servisR;
    QPushButton* apply;
    scoreboard::Protocol::DisplayFeatures features;

    QTcpSocket socket;
    QTimer reconnect;
    LineParser in;
//    std::unique_ptr<QTextToSpeech> speech;

    void enableSpeech(bool on);
    void sendLine(QString line);
    void sendRole();
    void sendFeatures();
};
