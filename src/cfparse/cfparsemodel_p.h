#ifndef CFPARSEMODELPRIVATE_H
#define CFPARSEMODELPRIVATE_H

#include <QObject>
#include "cfparsemodel.h"

class CFParseModelPrivate : public QObject
{
    Q_OBJECT
public:
    explicit CFParseModelPrivate(CFParseModel *q)
        : QObject(0)
        , q_ptr(q)
    {}
    virtual ~CFParseModelPrivate() {}

    static CFParseModelPrivate *create(CFParseModel *q);

    CFParseModel *q() { return q_ptr; }

    virtual void init() = 0;
    virtual void init(const QString &applicationId, const QString &clientKey) = 0;

    virtual void subscribe(const QString &channel) = 0;
    virtual void unsubscribe(const QString &channel) = 0;

Q_SIGNALS:

public Q_SLOTS:

private:
    Q_DECLARE_PUBLIC(CFParseModel)
    CFParseModel *q_ptr;
};

#endif // CFPARSEMODELPRIVATE_H
