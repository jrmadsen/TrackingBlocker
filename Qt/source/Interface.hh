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

#ifndef interface_hh_
#define interface_hh_

#include <QMainWindow>
#include <QResizeEvent>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QDir>
#include <QFile>

#include "Tools.hh"

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and Destructors
    Interface(QWidget*, int, int, QString);
    // Virtual destructors are required by abstract classes
    // so add it by default, just in case
    virtual ~Interface();

public:
    // Public functions
    void CreateMenuAndActions();
    void read();

public slots:
    // Public slot functions
    void open();
    void updateTabNames();

protected:
    // Protected functions
    void resizeEvent(QResizeEvent* event);

protected:
    // Protected variables
    QMenuBar*       m_menu_bar;
    QToolBar*       m_tool_bar;
    QDockWidget*    m_dock_widget;
    QDir            m_dir;
    QTabWidget*     m_tab_widget;

private:
    // Private static variables
    static QString load_default_path;
    static QString save_default_path;


};

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//

#endif
