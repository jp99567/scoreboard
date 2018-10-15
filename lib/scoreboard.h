#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>
#include <memory>

#include "LineParser.h"

class Client : public QObject
{
    Q_OBJECT

    QTcpSocket& socket;
    LineParser in;

public:
    Client(QTcpSocket& socket);
    ~Client();

signals:
    void disconnected(Client* thisConnection);
    void newLine(Client* thisConnection, QString line);

private slots:
    void onReadyRead();

};

class Scoreboard : public QObject
{
    Q_OBJECT

    QTcpServer listener;
    std::vector<std::unique_ptr<Client>> clients;

public:
    Scoreboard();

private slots:
    void removeClient(Client* client);
};

