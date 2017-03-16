package org.zmc.qutils.notification;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;

import android.content.Context;
import android.content.Intent;

public class NotificationReceiver extends BroadcastReceiver {

    public static String KEY_NOTIFICATION_ID = "notification-id";
    public static String KEY_NOTIFICATION_TAG = "notification-tag";
    public static String KEY_NOTIFICATION_MANAGER = "notification-manager-name";
    public static String KEY_NOTIFICATION = "notification";

    public void onReceive(Context context, Intent intent) {
        NotificationManager notificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);

        Notification notification = intent.getParcelableExtra(KEY_NOTIFICATION);
        int id = intent.getIntExtra(KEY_NOTIFICATION_ID, 0);
        String tag = intent.getStringExtra(KEY_NOTIFICATION_TAG);
        String notificationManagerName = intent.getStringExtra(KEY_NOTIFICATION_MANAGER);

        if (NotificationClient.isInitialized()) {
            CppCallbacks.notificationReceived(tag, id, notificationManagerName);
        }

        notificationManager.notify(tag, id, notification);
    }
}
