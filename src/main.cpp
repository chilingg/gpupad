#include "MainWindow.h"
#include "SingleApplication/singleapplication.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
#if defined(__linux)
    // try to increase device/driver support
    // does not seem to cause any problems
    setenv("MESA_GL_VERSION_OVERRIDE", "4.5", 0);
    setenv("MESA_GLSL_VERSION_OVERRIDE", "450", 0);
#endif

    SingleApplication app(argc, argv, true);
    auto arguments = app.arguments();
    arguments.removeFirst();

    if(app.isSecondary() && !arguments.empty()) {
        for (const auto &argument : qAsConst(arguments))
            app.sendMessage(argument.toUtf8());
        return 0;
    }

    app.setApplicationVersion(
#include "_version.h"
    );
    app.setOrganizationName("gpupad");
    app.setApplicationName("GPUpad");

    app.setStyle(QStyleFactory::create("Fusion"));

    MainWindow window;
    window.show();

    QObject::connect(&app, &SingleApplication::receivedMessage,
        [&](quint32 instanceId, QByteArray argument) {
            Q_UNUSED(instanceId);
            window.openFile(QString::fromUtf8(argument));
            window.setWindowState((window.windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            window.raise();
            window.activateWindow();
        });

    foreach (QString argument, arguments)
        window.openFile(argument);

    if (!window.hasEditor())
        window.newFile();

    return app.exec();
}
