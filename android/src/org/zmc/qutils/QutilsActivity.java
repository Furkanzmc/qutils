package org.zmc.qutils;

import android.util.Log;
import android.os.Bundle;
import android.content.Intent;

import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;

import android.view.View;
import android.view.KeyEvent;

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
    protected static AndroidUtils m_AndroidUtils;

    protected static boolean m_IsImmersiveModeEnabled = false;
    protected static boolean m_IsStatusBarVisible = true;

    public QutilsActivity()
    {
        m_Instance = this;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        m_NotificationClient = new NotificationClient(this);
        m_AndroidUtils = new AndroidUtils(this);
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

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            if (m_IsImmersiveModeEnabled) {
                m_AndroidUtils.setImmersiveMode(true);
            }

            if (m_IsStatusBarVisible == false) {
                m_AndroidUtils.setStatusBarVisible(false);
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == 1 && resultCode == RESULT_OK) {
            Bundle extras = data.getExtras();
            System.out.println(extras);
        }

        System.out.println(requestCode);
        System.out.println(RESULT_OK);
        System.out.println(resultCode);
    }

    public static void setImmersiveModeEnabled(boolean enabled)
    {
        m_IsImmersiveModeEnabled = enabled;
    }

    public static void setStatusBarVisible(boolean visible)
    {
        m_IsStatusBarVisible = visible;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            CppCallbacks.backButtonPressed();
            return true;
        }
        else if (keyCode == KeyEvent.KEYCODE_MENU) {
            CppCallbacks.menuButtonPressed();
            return true;
        }

        return super.onKeyUp(keyCode, event);
    }
}
