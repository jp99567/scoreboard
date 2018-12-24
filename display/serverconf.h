#pragma once

#include <QWidget>
#include <QStringList>
#include <memory>

class Scoreboard;
class QTextEdit;

class ServerConf : public QWidget
{
public:
    ServerConf();

private:
    std::unique_ptr<Scoreboard> server;
    QStringList log;
    QTextEdit *logW;
    void appendMsg(QString msg);
};

