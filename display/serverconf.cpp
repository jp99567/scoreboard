#include "serverconf.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QTextEdit>
#include "scoreboard.h"

ServerConf::ServerConf()
{
    auto enable = new QCheckBox;
    auto vlayout = new QVBoxLayout;
    auto flayout = new QFormLayout;
    flayout->addRow("Enable", enable);
    vlayout->addLayout(flayout);
    logW = new QTextEdit;
    logW->setReadOnly(true);
    vlayout->addWidget(logW);
    setLayout(vlayout);

    connect(enable, &QCheckBox::clicked, [this](bool checked){
        if(checked)
        {
            log.clear();
            server = std::make_unique<Scoreboard>();
            appendMsg(server->interfaces());
        }
        else{
            if(server)
            {
            }
            server = nullptr;
        }
    });
}

void ServerConf::appendMsg(QString msg)
{
    while(log.count() > 20){
        log.pop_front();
    }
    log.append(msg);
    QString text;
    foreach(QString i, log)
    {
        text.append(i + '\n');
    }

    logW->setText(text);
}
