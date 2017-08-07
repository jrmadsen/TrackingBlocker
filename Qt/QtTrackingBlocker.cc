//
//
// created by jmadsen on Sun Mar 20 19:36:01 2016
//
//
//
//

#include <QApplication>
#include <QLabel>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QTabWidget>

#include "Interface.hh"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    app.setApplicationName("Tracking Blocker");
    app.setApplicationVersion("1.0.0");
    app.setQuitOnLastWindowClosed(true);

    QCommandLineParser parser;
    parser.setApplicationDescription(argv[0]);
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption widthOption(QStringList() << "W" << "width",
                                   "Set window width");
    QCommandLineOption heightOption(QStringList() << "H" << "height",
                                    "Set window height");
    QCommandLineOption dirOption(QStringList() << "d" << "directory",
                                 "Host file directory");

    widthOption.setDefaultValue("1600");
    heightOption.setDefaultValue("1000");
    dirOption.setDefaultValue("/usr/local/share/TrackingBlocker");

    parser.addOption(widthOption);
    parser.addOption(heightOption);
    parser.addOption(dirOption);

    // Process the actual command line arguments given by the user
    parser.process(app);
    int width = parser.value(widthOption).toInt();
    int height = parser.value(heightOption).toInt();

    Interface interface(nullptr, width, height, parser.value(dirOption));
    interface.resize(width, height);

    int ret = 0;
    try
    {
        ret = app.exec();
    }
    catch(std::exception& e)
    {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown exception" << std::endl;
    }

    //------------------------------------------------------------------------//
    //  CLEANUP

    return ret;
}
