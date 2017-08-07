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


#include "Interface.hh"
#include "BlockList.hh"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

//============================================================================//

#ifdef __APPLE__
QString Interface::load_default_path = QDir::homePath();
#else
QString Interface::load_default_path = QDir::currentPath();
#endif

//============================================================================//

#ifdef __APPLE__
QString Interface::save_default_path = QDir::homePath();
#else
QString Interface::save_default_path = QDir::currentPath();
#endif

//============================================================================//

Interface::Interface(QWidget* parent, int w, int h, QString dir)
: QMainWindow(parent),
  m_menu_bar(new QMenuBar(this)),
  m_tool_bar(nullptr),
  m_dock_widget(nullptr),
  m_dir(QDir(dir)),
  m_tab_widget(new QTabWidget(this))
{
    setWindowTitle("Tracking Blocker");

    int dw = QApplication::desktop()->width(); // desktop width
    int dh = QApplication::desktop()->height(); // desktop height
    int ww = std::min(w, dw); // widget width
    int wh = std::min(h, dh); // widget height

    resize(ww, wh);

    float dA = dw * dh; // desktop area
    float wA = ww * wh; // widget area

    m_tab_widget->resize(ww, wh);

    setMenuBar(m_menu_bar);

    QList<QString> file_list;
    QStringList flist = m_dir.entryList(QStringList() << "hosts.*");
    for(auto itr : flist)
    {
        QString f = QString("%1/%2").arg(dir).arg(itr);
        file_list.push_back(f);
        m_tab_widget->addTab(new BlockList(m_tab_widget, f), itr);
    }

    // show or maximize
    if(wA / dA < 0.80f)
        show();
    else
        showMaximized();

    CreateMenuAndActions();
}

//============================================================================//

Interface::~Interface()
{  }

//============================================================================//

void Interface::CreateMenuAndActions()
{

    QAction* open_file_act = new QAction(tr("&Open..."), this);
    connect(open_file_act, &QAction::triggered, this, &Interface::open);

    QMenu* file_menu = m_menu_bar->addMenu("&File");
    file_menu->addAction(open_file_act);
}

//============================================================================//

void Interface::open()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Open file...",
                                                 load_default_path,
                                                 tr("*.txt"));

    QDir _dir(fname);
    _dir.cdUp();
    load_default_path = _dir.absolutePath();

}

//============================================================================//

void Interface::updateTabNames()
{

}

//============================================================================//

void Interface::read()
{
    for(int i = 0; i < m_tab_widget->count(); ++i)
    {
        m_tab_widget->setCurrentIndex(i);
        BlockList* bl = dynamic_cast<BlockList*>(m_tab_widget->currentWidget());
        bl->read();
    }
}

//============================================================================//

void Interface::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    resize(event->size().width(), event->size().height());
    if(centralWidget())
        centralWidget()->resize(width(), height());
    m_tab_widget->resize(width(), height());
    update();
}

//============================================================================//


