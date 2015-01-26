#include "cfparsemodel_ios_p.h"
#include <QJsonDocument>

#import <UIKit/UIKit.h>
#import <Parse/Parse.h>
#import <objc/runtime.h>
#import <objc/message.h>

CFMVC_DEFINE_PRIVATE_GLOBAL_INSTANCE(CFParseModelIosPrivate)

static QVariant dictionaryToVariant(NSDictionary *dictionary)
{
    NSError *error = nil;
    NSData *data = [NSJSONSerialization dataWithJSONObject:dictionary options:0 error:&error];
    NSString *json = @"{}";

    if (error != nil)
        qCDebug(CFParseModelTag) << "JSON could not serialized:" << QString::fromNSString([error localizedDescription]);
    else
        json = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];

    return QJsonDocument::fromJson(QString::fromNSString(json).toUtf8()).toVariant();
}


BOOL didFinishLaunchingWithOptions(id self, SEL _cmd, UIApplication *application, NSDictionary *launchOptions)
{
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    Q_UNUSED(application)

    NSDictionary *payload = launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey];
    QVariant data = dictionaryToVariant(payload);
    if (!data.isNull() && data.type() == QVariant::Map) {
        qCDebug(CFParseModelTag) << "Notification playload" << data;

        CFMVC_PRIVATE_GLOBAL_S(CFParseModelIosPrivate);
        QMap<quintptr, CFParseModelIosPrivate *>::const_iterator it;
        for (it = dGlobal.begin(); it != dGlobal.end(); ++it)
            if (it.value())
                it.value()->q()->pushOpen(data.toMap());
    }

    return YES;
}

void didRegisterForRemoteNotificationsWithDeviceToken(id self, SEL _cmd, UIApplication *application, NSData *deviceToken)
{
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    Q_UNUSED(application)

    qCDebug(CFParseModelTag) << "Device token is " << QString::fromNSString([[NSString alloc] initWithData:deviceToken encoding:NSUTF8StringEncoding]);

    PFInstallation *currentInstallation = [PFInstallation currentInstallation];
    [currentInstallation setDeviceTokenFromData:deviceToken];
    [currentInstallation saveInBackground];
}

void didFailToRegisterForRemoteNotificationsWithError(id self, SEL _cmd, UIApplication *application, NSError *error)
{
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    Q_UNUSED(application)

    qCDebug(CFParseModelTag) << "Device token is " << QString::fromNSString([error localizedDescription]);
}

void didReceiveRemoteNotification(id self, SEL _cmd, UIApplication *application, NSDictionary *userInfo)
{
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    Q_UNUSED(application)

    [PFPush handlePush:userInfo];
}

void didReceiveRemoteNotificationFetchCompletionHandler(id self, SEL _cmd, UIApplication *application, NSDictionary *userInfo, id completionHandler)
{
    Q_UNUSED(completionHandler)
    didReceiveRemoteNotification(self, _cmd, application, userInfo);
}


CFParseModelPrivate *CFParseModelPrivate::create(CFParseModel *q)
{
    return new CFParseModelIosPrivate(q);
}

CFParseModelIosPrivate::CFParseModelIosPrivate(CFParseModel *q)
    : CFParseModelPrivate(q)
{
}

CFParseModelIosPrivate::~CFParseModelIosPrivate()
{
}

void CFParseModelIosPrivate::init()
{
    NSDictionary *infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString *applicationId = [infoDict objectForKey:@"CFParseApplicationId"];
    NSString *clientKey = [infoDict objectForKey:@"CFParseClientKey"];

    if (applicationId == nil || clientKey == nil) {
        qCWarning(CFParseModelTag) << "Application Id or Client Key is not placed in Info.plist";
        return;
    }

    init(QString::fromNSString(applicationId), QString::fromNSString(clientKey));
}

void CFParseModelIosPrivate::init(const QString &applicationId, const QString &clientKey)
{
    if (applicationId.isEmpty() || clientKey.isEmpty()) {
        qCWarning(CFParseModelTag) << "Application Id or Client Key could not be empty.";
        return;
    }

    UIApplication *application = [UIApplication sharedApplication];
    id delegate = [application delegate];
    Class delegateClass = object_getClass(delegate);

    NSString *overrideClassName = [NSString stringWithFormat:@"QIOSPlatformDelegateOverride%@", NSStringFromClass(delegateClass)];
    Class overrideDelegate = NSClassFromString(overrideClassName);

    if (overrideDelegate == nil) {
        overrideDelegate = objc_allocateClassPair(delegateClass, [overrideClassName UTF8String], 0);

        SEL didFinishLaunchingWithOptionsOverride = @selector(application:didFinishLaunchingWithOptions:);
        SEL didRegisterForRemoteNotificationsWithDeviceTokenOverride = @selector(application:didRegisterForRemoteNotificationsWithDeviceToken:);
        SEL didFailToRegisterForRemoteNotificationsWithErrorOverride = @selector(application:didFailToRegisterForRemoteNotificationsWithError:);
        SEL didReceiveRemoteNotificationOverride = @selector(application:didReceiveRemoteNotification:);
        SEL didReceiveRemoteNotificationFetchCompletionHandlerOverride = @selector(application:didReceiveRemoteNotification:fetchCompletionHandler:);

        Method didFinishLaunchingWithOptionsOverrideMethod = class_getInstanceMethod(delegateClass, didFinishLaunchingWithOptionsOverride);
        Method didRegisterForRemoteNotificationsWithDeviceTokenOverrideMethod = class_getInstanceMethod(delegateClass, didRegisterForRemoteNotificationsWithDeviceTokenOverride);
        Method didFailToRegisterForRemoteNotificationsWithErrorOverrideMethod = class_getInstanceMethod(delegateClass, didFailToRegisterForRemoteNotificationsWithErrorOverride);
        Method didReceiveRemoteNotificationOverrideMethod = class_getInstanceMethod(delegateClass, didReceiveRemoteNotificationOverride);
        Method didReceiveRemoteNotificationFetchCompletionHandlerOverrideMethod = class_getInstanceMethod(delegateClass, didReceiveRemoteNotificationFetchCompletionHandlerOverride);

        class_addMethod(
            overrideDelegate,
            didFinishLaunchingWithOptionsOverride,
            (IMP) didFinishLaunchingWithOptions,
            method_getTypeEncoding(didFinishLaunchingWithOptionsOverrideMethod));

        class_addMethod(
            overrideDelegate,
            didRegisterForRemoteNotificationsWithDeviceTokenOverride,
            (IMP) didRegisterForRemoteNotificationsWithDeviceToken,
            method_getTypeEncoding(didRegisterForRemoteNotificationsWithDeviceTokenOverrideMethod));

        class_addMethod(
            overrideDelegate,
            didFailToRegisterForRemoteNotificationsWithErrorOverride,
            (IMP) didFailToRegisterForRemoteNotificationsWithError,
            method_getTypeEncoding(didFailToRegisterForRemoteNotificationsWithErrorOverrideMethod));

        class_addMethod(
            overrideDelegate,
            didReceiveRemoteNotificationOverride,
            (IMP) didReceiveRemoteNotification,
            method_getTypeEncoding(didReceiveRemoteNotificationOverrideMethod));

        class_addMethod(
            overrideDelegate,
            didReceiveRemoteNotificationFetchCompletionHandlerOverride,
            (IMP) didReceiveRemoteNotificationFetchCompletionHandler,
            method_getTypeEncoding(didReceiveRemoteNotificationFetchCompletionHandlerOverrideMethod));

        objc_registerClassPair(overrideDelegate);
    }

    object_setClass(delegate, overrideDelegate);

    [Parse setApplicationId:[[NSString alloc] initWithFormat:@"%s", applicationId.toUtf8().data()] clientKey:[[NSString alloc] initWithFormat:@"%s", clientKey.toUtf8().data()]];

    UIUserNotificationType userNotificationTypes = (UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound);
    if ([application respondsToSelector:@selector(isRegisteredForRemoteNotifications)]) {
        UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:userNotificationTypes categories:nil];
        [application registerUserNotificationSettings:settings];
        [application registerForRemoteNotifications];
    } else {
        [application registerForRemoteNotificationTypes:userNotificationTypes];
    }
}

void CFParseModelIosPrivate::subscribe(const QString &channel)
{
    PFInstallation *currentInstallation = [PFInstallation currentInstallation];
    [currentInstallation addUniqueObject:[[NSString alloc] initWithFormat:@"%s", channel.toUtf8().data()] forKey:@"channels"];
    [currentInstallation saveInBackground];
}

void CFParseModelIosPrivate::unsubscribe(const QString &channel)
{
    PFInstallation *currentInstallation = [PFInstallation currentInstallation];
    [currentInstallation removeObject:[[NSString alloc] initWithFormat:@"%s", channel.toUtf8().data()] forKey:@"channels"];
    [currentInstallation saveInBackground];
}


