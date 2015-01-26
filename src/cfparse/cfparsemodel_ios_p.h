#ifndef CFPARSEMODEL_IOS_P_H
#define CFPARSEMODEL_IOS_P_H

#include "cfparsemodel_p.h"

class CFParseModelIosPrivate : public CFParseModelPrivate
{
    Q_OBJECT
public:
    explicit CFParseModelIosPrivate(CFParseModel *q);
    ~CFParseModelIosPrivate();

    void init();
    void init(const QString &applicationId, const QString &clientKey);

    void subscribe(const QString &channel);
    void unsubscribe(const QString &channel);
};

#endif // CFPARSEMODEL_IOS_P_H
