package com.cf.parse;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import com.parse.ParsePushBroadcastReceiver;

public class CFAndroidParseBroadcastReceiver extends ParsePushBroadcastReceiver {
    private static final String TAG = "CFAndroidParseBroadcastReceiver";

    @Override
    protected void onPushReceive(Context context, Intent intent) {
        if (CFAndroidParseController.instance() != null) {
            String data = intent.getExtras().getString("com.parse.Data");
            Log.d(TAG, "Push received with data: " + data);
            CFAndroidParseController.instance().onPushReceived(data);
        } else {
            Log.d(TAG, "Push received.");
        }

        super.onPushReceive(context, intent);
    }

    @Override
    public void onPushOpen(Context context, Intent intent) {
        if (CFAndroidParseController.instance() != null) {
            String data = intent.getExtras().getString("com.parse.Data");
            Log.d(TAG, "Push open: " + data);
            CFAndroidParseController.instance().onPushOpen(data);
        } else {
            Log.d(TAG, "Push open & starting main activity.");
            Intent i = context.getPackageManager().getLaunchIntentForPackage(context.getPackageName());
            i.putExtras(intent.getExtras());
            i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(i);
        }

        super.onPushOpen(context, intent);
    }
}
