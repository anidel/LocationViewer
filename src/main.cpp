// Navigation pane project template
#include "LocationViewer.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>
#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;

void myMessageOutput(QtMsgType type, const char* msg){
               fprintf(stdout, "%s\n", msg);
               fflush(stdout);
}

Q_DECL_EXPORT int main(int argc, char **argv)
{
    // this is where the server is started etc
    Application app(argc, argv);

#ifndef QT_NO_DEBUG
    qInstallMsgHandler(myMessageOutput);
#endif

    // localization support
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString( "LocationViewer_%1" ).arg( locale_string );
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator( &translator );
    }

    // create the application pane object to init UI etc.
    LocationViewer viewer(&app);
    viewer.loadImages();

    // we complete the transaction started in the app constructor and start the client event loop here
    return Application::exec();
    // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
