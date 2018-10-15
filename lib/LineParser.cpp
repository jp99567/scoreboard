#include "LineParser.h"

#include <QTextStream>

LineParser::LineParser()
{

}

void LineParser::clear()
{
    in.clear();
}

void LineParser::process(QByteArray data)
{
    in.append(std::move(data));
    QTextStream ts(&in);
    while(1)
    {
        auto line =  ts.readLine();

        if(line.isNull())
        {
            in = ts.readAll();
            break;
        }
        else
        {
            if(ts.atEnd())
            {
                in = line;
                break;
            }
            emit newLine(std::move(line));
        }
    }
}
