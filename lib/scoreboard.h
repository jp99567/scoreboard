#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <vector>
#include <memory>

class Client : public QObject
{
    Q_OBJECT

    QTcpSocket& socket;
    QString in;

public:
    Client(QTcpSocket& socket);
    ~Client();

signals:
    void disconnected(Client* thisConnection);

private slots:
    void onReadyRead();

private:
    void process(QString line);
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

