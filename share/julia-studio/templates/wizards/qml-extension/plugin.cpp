#include "%ProjectName:l%_plugin.%CppHeaderSuffix%"
#include "%ObjectName:l%.%CppHeaderSuffix%"

#include <qdeclarative.h>

void %ProjectName:s%Plugin::registerTypes(const char *uri)
{
    // @uri %Uri%
    qmlRegisterType<%ObjectName%>(uri, 1, 0, "%ObjectName%");
}

Q_EXPORT_PLUGIN2(%ProjectName:s%, %ProjectName:s%Plugin)
