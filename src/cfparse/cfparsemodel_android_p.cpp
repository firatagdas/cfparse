#include "cfparsemodel_android_p.h"
#include <QtAndroidExtras>
#include <QJsonDocument>
#include <CFMvc/cfandroidutil.h>

CFMVC_DEFINE_PRIVATE_GLOBAL_INSTANCE(CFParseModelAndroidPrivate)

static const char androidParseControllerClass[] = "com/cf/parse/CFAndroidParseController";

CFParseModelPrivate *CFParseModelPrivate::create(CFParseModel *q)
{
    return new CFParseModelAndroidPrivate(q);
}

CFParseModelAndroidPrivate::CFParseModelAndroidPrivate(CFParseModel *q)
    : CFParseModelPrivate(q)
    , m_id(reinterpret_cast<quintptr>(this))
{
    m_parseController = QJNIObjectPrivate(androidParseControllerClass,
                                          "(Landroid/app/Activity;J)V",
                                          QtAndroidPrivate::activity());
}

CFParseModelAndroidPrivate::~CFParseModelAndroidPrivate()
{
}

void CFParseModelAndroidPrivate::subscribe(const QString &channel)
{
    m_parseController.callMethod<void>("subscribe", "(Ljava/lang/String;)V", Q2J_STRING(channel));
}

void CFParseModelAndroidPrivate::unsubscribe(const QString &channel)
{
    m_parseController.callMethod<void>("unsubscribe", "(Ljava/lang/String;)V", Q2J_STRING(channel));
}

void CFParseModelAndroidPrivate::init()
{
    qCDebug(CFParseModelTag) << "it should be inited in QtApplication.onCreate method.";
}

void CFParseModelAndroidPrivate::init(const QString &applicationId, const QString &clientKey)
{
    Q_UNUSED(applicationId)
    Q_UNUSED(clientKey)
}


static inline void onSubscribed(JNIEnv *env, jclass, jlong id, jstring channel)
{
    CFMVC_PRIVATE_GLOBAL_D(CFParseModelAndroidPrivate, id);
    emit d->q()->subscribed(J2Q_STRING(env, channel));
}

static inline void onSubscribeFailed(JNIEnv *env, jclass, jlong id, jstring channel, jstring error)
{
    CFMVC_PRIVATE_GLOBAL_D(CFParseModelAndroidPrivate, id);
    emit d->q()->subscribeFailed(J2Q_STRING(env, channel), J2Q_STRING(env, error));
}

static inline void onUnubscribed(JNIEnv *env, jclass, jlong id, jstring channel)
{
    CFMVC_PRIVATE_GLOBAL_D(CFParseModelAndroidPrivate, id);
    emit d->q()->unsubscribed(J2Q_STRING(env, channel));
}

static inline void onUnsubscribeFailed(JNIEnv *env, jclass, jlong id, jstring channel, jstring error)
{
    CFMVC_PRIVATE_GLOBAL_D(CFParseModelAndroidPrivate, id);
    emit d->q()->unsubscribeFailed(J2Q_STRING(env, channel), J2Q_STRING(env, error));
}

static inline void onPushReceived(JNIEnv *env, jclass, jlong id, jstring data)
{
    CFMVC_PRIVATE_GLOBAL_D(CFParseModelAndroidPrivate, id);

    QString json = J2Q_STRING(env, data);
    QJsonParseError error;
    QVariantMap obj;

    QVariant jsonVar = QJsonDocument::fromJson(json.toUtf8(), &error).toVariant();
    if (error.error != QJsonParseError::NoError)
        qCWarning(CFParseModelTag) << "JSON Parse error:" << error.errorString();
    else if (!jsonVar.isNull() && jsonVar.type() == QVariant::Map)
        obj = jsonVar.toMap();

    emit d->q()->pushReceived(obj);
}

static inline void onPushOpened(JNIEnv *env, jclass, jlong id, jstring data)
{
    CFMVC_PRIVATE_GLOBAL_D(CFParseModelAndroidPrivate, id);

    QString json = J2Q_STRING(env, data);
    QJsonParseError error;
    QVariantMap obj;

    QVariant jsonVar = QJsonDocument::fromJson(json.toUtf8(), &error).toVariant();
    if (error.error != QJsonParseError::NoError)
        qCWarning(CFParseModelTag) << "JSON Parse error:" << error.errorString();
    else if (!jsonVar.isNull() && jsonVar.type() == QVariant::Map)
        obj = jsonVar.toMap();

    emit d->q()->pushOpen(obj);
}

JNINativeMethod jniMethods[] = {
    { "subscribed", "(JLjava/lang/String;)V", (void *) onSubscribed },
    { "subscribeFailed", "(JLjava/lang/String;Ljava/lang/String;)V", (void *) onSubscribeFailed },
    { "unsubscribed", "(JLjava/lang/String;)V", (void *) onUnubscribed },
    { "unsubscribeFailed", "(JLjava/lang/String;Ljava/lang/String;)V", (void *) onUnsubscribeFailed },
    { "pushReceived", "(JLjava/lang/String;)V", (void *) onPushReceived },
    { "pushOpened", "(JLjava/lang/String;)V", (void *) onPushOpened }
};

int JNICALL JNI_OnLoad(JavaVM *vm, void *)
{
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK)
        return JNI_FALSE;

    jclass parseControllerClass = env->FindClass(androidParseControllerClass);
    if (!parseControllerClass) {
        qDebug() << "AndroidParseController class not found.";
        return JNI_ERR;
    }

    if (env->RegisterNatives(parseControllerClass, jniMethods, sizeof(jniMethods) / sizeof(jniMethods[0]))) {
        qDebug() << "AndroidParseController could not register native methods.";
        return JNI_ERR;
    }

    return JNI_VERSION_1_4;
}

