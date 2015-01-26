package com.cf.parse;

import android.app.Activity;
import java.lang.String;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import com.parse.Parse;
import com.parse.ParseException;
import com.parse.ParsePush;
import com.parse.SaveCallback;

public class CFAndroidParseController
{
    private static final String TAG = "CFAndroidParseController";
    private static final String META_APPLICATION_ID = "com.cf.parse.applicationId";
    private static final String META_CLIENT_KEY = "com.cf.parse.clientKey";

    private final Activity m_activity;
    private final long m_id;

    private static CFAndroidParseController m_instance;

    public CFAndroidParseController(final Activity activity, final long id) {
        m_activity = activity;
        m_id = id;

        if (m_instance == null)
            m_instance = this;
    }

    public static void init(Context context) {
        String applicationId = null;
        String clientKey = null;

        try {
            Log.d(TAG, "Package name: " + context.getPackageName());

            ApplicationInfo applicationInfo = context.getPackageManager().getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA);
            Bundle meta = applicationInfo.metaData;
            applicationId = meta.getString(META_APPLICATION_ID);
            clientKey = meta.getString(META_CLIENT_KEY);

        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        } catch (NullPointerException e) {
            e.printStackTrace();
        }

        init(context, applicationId, clientKey);
    }

    public static void init(Context context, String applicationId, String clientKey) {
        if (applicationId != null && clientKey != null)
            Parse.initialize(context, applicationId, clientKey);
        else
            Log.w(TAG, "Application Id or Client Key could not retrieved from AndroidManifest.xml");
    }

    public static CFAndroidParseController instance() {
        return m_instance;
    }

    public void subscribe(final String channel) {
        ParsePush.subscribeInBackground(channel, new SaveCallback() {
            @Override
            public void done(ParseException e) {
                if (e == null) {
                    Log.d(TAG, "Subscribed for channel: " + channel);
                    subscribed(m_id, channel);
                } else {
                    Log.e(TAG, "Failed to subscribe for channel: " + channel, e);
                    subscribeFailed(m_id, channel, e.getLocalizedMessage());
                }
            }
        });
    }

    public void unscribe(final String channel) {
        ParsePush.unsubscribeInBackground(channel, new SaveCallback() {
            @Override
            public void done(ParseException e) {
                if (e == null) {
                    Log.d(TAG, "Unsubscribed for channel: " + channel);
                    unsubscribed(m_id, channel);
                } else {
                    Log.e(TAG, "Failed to unsubscribe for channel: " + channel, e);
                    unsubscribeFailed(m_id, channel, e.getLocalizedMessage());
                }
            }
        });
    }

    public void onPushReceived(String data) {
        pushReceived(m_id, data);
    }

    public void onPushOpen(String data) {
        pushOpened(m_id, data);
    }

    public native void pushReceived(long id, String data);
    public native void pushOpened(long id, String data);
    public native void subscribed(long id, String channel);
    public native void subscribeFailed(long id, String channel, String error);
    public native void unsubscribed(long id, String channel);
    public native void unsubscribeFailed(long id, String channel, String error);
}
