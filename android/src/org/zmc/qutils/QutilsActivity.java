package org.zmc.qutils;

import android.util.Log;
import android.os.Bundle;
import android.content.Intent;

import org.zmc.qutils.notification.NotificationClient;
import org.zmc.qutils.notification.NotificationReceiver;
import org.zmc.qutils.notification.CppCallbacks;
// Qt
import org.qtproject.qt5.android.bindings.QtActivity;

/**
 * This is the main acitivity used throughout the QtUtils. Either extend this in your main activity of your app, or initialize it.
 */
public class QutilsActivity extends QtActivity
{
    private static QutilsActivity m_Instance;
    protected static NotificationClient m_NotificationClient;

    public QutilsActivity()
    {
        m_Instance = this;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        m_NotificationClient = new NotificationClient(this);
    }

    @Override
    public void onResume() {
        super.onResume();

        Intent in = getIntent();
        String tag = in.getStringExtra(NotificationReceiver.KEY_NOTIFICATION_TAG);
        String notificationManagerName = in.getStringExtra(NotificationReceiver.KEY_NOTIFICATION_MANAGER);
        int id = in.getIntExtra(NotificationReceiver.KEY_NOTIFICATION_ID, -1);
        if (id > -1) {
            CppCallbacks.notificationReceived(tag, 0, notificationManagerName);
        }
    }
}
