#include "cfparsemodel.h"
#include "cfparsemodel_p.h"

Q_LOGGING_CATEGORY(CFParseModelTag, "com.cf.parse.CFParseModel")

CFMVC_MODEL_BEGIN(CFParseModel)

CFParseModel::CFParseModel(CFMvc *mvc, const QVariantMap &config)
    : CFModel(mvc, config)
    , d_ptr(CFParseModelPrivate::create(this))
{
}

CFParseModel::~CFParseModel()
{
    delete d_ptr;
}

void CFParseModel::init()
{
    Q_D(CFParseModel);
    d->init();
}

void CFParseModel::subscribe(const QString &channel)
{
    Q_D(CFParseModel);
    d->subscribe(channel);
}

void CFParseModel::unsubscribe(const QString &channel)
{
    Q_D(CFParseModel);
    d->unsubscribe(channel);
}

CFMVC_MODEL_END(CFParseModel)
