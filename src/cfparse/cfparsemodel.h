#ifndef CFPARSEMODEL_H
#define CFPARSEMODEL_H

#include <CFMvc/cfmodel.h>
#include <QLoggingCategory>
#include "cfparse_global.h"

Q_DECLARE_LOGGING_CATEGORY(CFParseModelTag)

class CFParseModelPrivate;

class Q_CFPARSE_EXPORT CFParseModel : public CFModel
{
    Q_OBJECT
    CFMVC_MODEL_OBJECT(CFParseModel)

public:
    explicit CFParseModel(CFMvc *mvc, const QVariantMap &config = QVariantMap());
    ~CFParseModel();

    Q_INVOKABLE void init();
    Q_INVOKABLE void subscribe(const QString &channel);
    Q_INVOKABLE void unsubscribe(const QString &channel);

Q_SIGNALS:
    void subscribed(const QString &name);
    void subscribeFailed(const QString &name, const QString &error);

    void unsubscribed(const QString &name);
    void unsubscribeFailed(const QString &name, const QString &error);

    void pushReceived(const QVariantMap &data);
    void pushOpen(const QVariantMap &data);

private:
    Q_DECLARE_PRIVATE(CFParseModel)
    CFParseModelPrivate *d_ptr;
};

#endif // CFPARSEMODEL_H
