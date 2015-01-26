#ifndef CFPARSEMODEL_ANDROID_P_H
#define CFPARSEMODEL_ANDROID_P_H

#include "cfparsemodel_p.h"
#include <QtCore/private/qjnihelpers_p.h>
#include <QtCore/private/qjni_p.h>

class CFParseModelAndroidPrivate : public CFParseModelPrivate
{
    Q_OBJECT
public:
    explicit CFParseModelAndroidPrivate(CFParseModel *q);
    ~CFParseModelAndroidPrivate();

    void init();
    void init(const QString &applicationId, const QString &clientKey);

    void subscribe(const QString &channel);
    void unsubscribe(const QString &channel);

private:
    quintptr m_id;
    QJNIObjectPrivate m_parseController;
};

#endif // CFPARSEMODEL_ANDROID_P_H
