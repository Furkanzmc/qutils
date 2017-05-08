package org.zmc.qutils;

import android.util.Log;
import android.graphics.Color;
import android.view.Window;

import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.view.View;

import android.os.Build;
import android.content.Intent;
import android.app.AlertDialog.Builder;

import android.app.AlertDialog;
import android.content.DialogInterface;
import java.util.HashMap;

import java.util.Iterator;
import java.util.Set;
import java.util.Map;

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
        if (Build.VERSION.SDK_INT >= 21) {
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

    public static void shareText(String dialogTitle, String text)
    {
        Intent sendIntent = new Intent();
        sendIntent.setAction(Intent.ACTION_SEND);
        sendIntent.putExtra(Intent.EXTRA_TEXT, text);
        sendIntent.setType("text/plain");
        m_MainContext.startActivity(Intent.createChooser(sendIntent, dialogTitle));
    }

    public static void showAlertDialog(HashMap properties)
    {
        AlertDialog.Builder builder = new AlertDialog.Builder(m_MainContext);
        builder.setCancelable(false);
        builder.setTitle((String)properties.get("title"));
        builder.setMessage((String)properties.get("message"));

        builder.setOnKeyListener(new DialogInterface.OnKeyListener() {
            @Override
            public boolean onKey(DialogInterface dialog, int keyCode, android.view.KeyEvent event) {
                if (keyCode == android.view.KeyEvent.KEYCODE_BACK && event.getAction() == android.view.KeyEvent.ACTION_UP) {
                    dialog.cancel();
                    CppCallbacks.alertDialogClicked(-2);
                    return false;
                }

                return true;
            }
        });

        properties.remove("title");
        properties.remove("message");

        // Get a set of the entries
        Set set = properties.entrySet();
        // Get an iterator
        Iterator i = set.iterator();

        // Display elements
        while(i.hasNext()) {
            Map.Entry me = (Map.Entry)i.next();
            HashMap innerValue = (HashMap)me.getValue();
            String buttonText = (String)me.getKey();
            String buttonType = (String)innerValue.get("type");

            if (buttonType.equals("positive")) {
                builder.setPositiveButton(buttonText, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        CppCallbacks.alertDialogClicked(1);
                    }
                });
            }
            else if (buttonType.equals("neutral")) {
                builder.setNeutralButton(buttonText, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        CppCallbacks.alertDialogClicked(0);
                    }
                });
            }
            else if (buttonType.equals("negative")) {
                builder.setNegativeButton(buttonText, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        CppCallbacks.alertDialogClicked(-1);
                    }
                });
            }
        }

        final AlertDialog dialog = builder.create();
        dialog.show();
    }
}
