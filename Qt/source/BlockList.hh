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

#ifndef blocklist_hh_
#define blocklist_hh_

#include <QTabWidget>
#include <QMainWindow>
#include <QResizeEvent>
#include <QTableWidget>

class BlockList : public QTableWidget
{
    Q_OBJECT

public:
    // Constructor and Destructors
    BlockList(QWidget* parent, QString);
    // Virtual destructors are required by abstract classes
    // so add it by default, just in case
    virtual ~BlockList();

public:
    // Public functions
    void read();

protected:
    // Protected functions

protected:
    // Protected variables
    QString m_file_name;

};

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//

#endif
