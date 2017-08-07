//
//
//
//
//
// created by jmadsen on Sun Mar 20 19:36:55 2016
//
//
//
//


#include "BlockList.hh"
#include "Tools.hh"

#include <QDebug>

using namespace std;

//============================================================================//

BlockList::BlockList(QWidget* parent, QString fname)
: QTableWidget(0, 3, parent),
  m_file_name(fname)
{
    resize(parent->width(), parent->height());

    setAlternatingRowColors(true);
    setAutoFillBackground(true);
    setSortingEnabled(true);

    for(int i = 0; i < columnCount(); ++i)
        setColumnWidth(i, width()/columnCount());

    qDebug() << "Reading " << fname << "...";
    read();
}

//============================================================================//

BlockList::~BlockList()
{ }

//============================================================================//

void BlockList::read()
{
    auto _add = [&] (const int& row, const QList<QString>& line)
    {
        int col = 0;
        for(auto itr = line.begin(); itr != line.end(); ++itr)
            setItem(row, col++, new QTableWidgetItem(*itr));
    };

    ifstream in(m_file_name.toStdString().c_str());
    QList<QList<QString>> delim_list;
    if(in)
    {
        vector<string> delim;
        while(tools::GetLineAndDelimit(in, " \t", "#", delim))
        {
            QList<QString> qdelim;
            for(const auto& itr : delim)
                qdelim.push_back(QString(itr.c_str()));
            delim_list.push_back(qdelim);
        }
    }
    in.close();
    setRowCount(delim_list.size());

    qDebug() << "\tBuilding " << delim_list.size() << " rows...";
    for(int i = 0; i < delim_list.size(); ++i)
    {
        if(delim_list.size() > 50 &&
           i%(delim_list.size()/50) == 0)
        {
            qDebug() << "\tBuilding row " << i << " ("
                     << (int)(static_cast<float>(i)/(delim_list.size())*100.0)
                     << "%) ...";
            update();
        }
        else if(delim_list.size() <= 50)
            update();

        _add(i, delim_list.at(i));
    }

    for(int i = 0; i < rowCount(); ++i)
        for(int j = 0; j < columnCount(); ++j)
            if(item(i,j))
                item(i, j)->setTextAlignment(Qt::AlignCenter);

    qDebug() << "\tDone...";
}

//============================================================================//

