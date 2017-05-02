package org.zmc.qutils;

import android.util.Log;
import android.graphics.Color;
import android.view.Window;

import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.view.View;

import android.os.Build;

// qutils
import org.zmc.qutils.notification.CppCallbacks;
import org.zmc.qutils.QutilsActivity;
// Qt
import org.qtproject.qt5.android.bindings.QtActivity;

public class AndroidUtils extends QtActivity
{
    private static QutilsActivity m_MainContext;

    public AndroidUtils(QutilsActivity mainActivity)
    {
        m_MainContext = mainActivity;
    }

    public static void setStatusBarColor(String colorHexStr)
    {
        if (android.os.Build.VERSION.SDK_INT >= 21) {
            Window window = m_MainContext.getWindow();
            // clear FLAG_TRANSLUCENT_STATUS flag:
            window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            // add FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS flag to the window
            window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
            // finally change the color
            window.setStatusBarColor(Color.parseColor(colorHexStr));
        }
    }

    public static void setStatusBarVisible(boolean visible)
    {
        if (Build.VERSION.SDK_INT < 16) {
            m_MainContext.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
        else {
            View decorView = m_MainContext.getWindow().getDecorView();
            m_MainContext.setStatusBarVisible(visible);
            // Hide the status bar.
            if (visible == false) {
                decorView.setSystemUiVisibility(
                                                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                                | View.SYSTEM_UI_FLAG_FULLSCREEN);
            }
            else {
                decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
            }
        }
    }

    public static void setImmersiveMode(boolean enabled)
    {
        if (Build.VERSION.SDK_INT >= 19) {
            View decorView = m_MainContext.getWindow().getDecorView();
            m_MainContext.setImmersiveModeEnabled(enabled);
            // Hide the status bar.
            if (enabled) {
                decorView.setSystemUiVisibility(
                                                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                                | View.SYSTEM_UI_FLAG_FULLSCREEN
                                                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
            }
            else {
                decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
            }
        }
    }
}
