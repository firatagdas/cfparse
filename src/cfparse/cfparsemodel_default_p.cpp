#include "cfparsemodel_default_p.h"

CFParseModelPrivate *CFParseModelPrivate::create(CFParseModel *q)
{
    return new CFParseModelDefaultPrivate(q);
}

CFParseModelDefaultPrivate::CFParseModelDefaultPrivate(CFParseModel *q)
    : CFParseModelPrivate(q)
{
}

CFParseModelDefaultPrivate::~CFParseModelDefaultPrivate()
{
}

void CFParseModelDefaultPrivate::subscribe(const QString &channel)
{
    Q_UNUSED(channel)
}

void CFParseModelDefaultPrivate::unsubscribe(const QString &channel)
{
    Q_UNUSED(channel)
}

void CFParseModelDefaultPrivate::init()
{

}

void CFParseModelDefaultPrivate::init(const QString &applicationId, const QString &clientKey)
{
    Q_UNUSED(applicationId)
    Q_UNUSED(clientKey)
}

