package org.zmc.qutils.notification;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.BroadcastReceiver;

import android.content.Context;
import android.content.Intent;

// qutils
import org.zmc.qutils.CppCallbacks;
import org.zmc.qutils.QutilsActivity;

public class NotificationReceiver extends BroadcastReceiver {

    public static String KEY_NOTIFICATION_ID = "notification_id";
    public static String KEY_NOTIFICATION_TAG = "notification_tag";
    public static String KEY_NOTIFICATION_MANAGER = "notification_manager_name";

    public static String KEY_NOTIFICATION = "notification";
    public static String KEY_PAYLOAD = "payload";

    public void onReceive(Context context, Intent intent) {
        NotificationManager notificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);

        Notification notification = intent.getParcelableExtra(KEY_NOTIFICATION);
        int id = intent.getIntExtra(KEY_NOTIFICATION_ID, -1);
        String tag = intent.getStringExtra(KEY_NOTIFICATION_TAG);

        if (NotificationClient.isInitialized()) {
            QutilsActivity.checkForNotification(intent);
        }

        notificationManager.notify(tag, id, notification);
    }
}
