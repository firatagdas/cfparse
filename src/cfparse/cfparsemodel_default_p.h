#ifndef CFPARSEMODEL_DEFAULT_P_H
#define CFPARSEMODEL_DEFAULT_P_H

#include "cfparsemodel_p.h"

class CFParseModelDefaultPrivate : public CFParseModelPrivate
{
    Q_OBJECT
public:
    explicit CFParseModelDefaultPrivate(CFParseModel *q);
    ~CFParseModelDefaultPrivate();

    void init();
    void init(const QString &applicationId, const QString &clientKey);

    void subscribe(const QString &channel);
    void unsubscribe(const QString &channel);
};

#endif // CFPARSEMODEL_DEFAULT_P_H
