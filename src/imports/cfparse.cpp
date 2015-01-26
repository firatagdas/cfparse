#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>
#include <CFMvc/cfmodel.h>
#include <CFParse/cfparsemodel.h>

CFMVC_DEFINE_MODEL(CFParseModel)

QT_BEGIN_NAMESPACE

class CFParseModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("CFParse"));

        // @uri QtWebView
        qmlRegisterSingletonType<CFParseModel>(uri, 1, 0, "CFParseModel", CFMVC_MODEL(CFParseModel));
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};

QT_END_NAMESPACE

#include "cfparse.moc"



