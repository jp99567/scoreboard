#include <QString>
#include <QObject>

class QByteArray;

class LineParser : public QObject
{
    Q_OBJECT
    QString in;
public:
    LineParser();
    void clear();
    void process(QByteArray data);

signals:
    void newLine(QString line);
};

