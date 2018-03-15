package org.zmc.qutils;

// Java

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Set;
import java.util.StringTokenizer;

// Android
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.content.Intent;
import android.view.Display;
import android.view.Window;

import android.view.View;
import android.view.KeyEvent;
import android.provider.MediaStore;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;

import android.os.Build;
import android.provider.DocumentsContract;
import android.os.Environment;

import android.content.ContentUris;
import android.graphics.Rect;
import android.view.ViewTreeObserver;

import android.util.Log;

// qutils
import org.zmc.qutils.notification.NotificationClient;
import org.zmc.qutils.notification.NotificationReceiver;

// Qt
import org.qtproject.qt5.android.bindings.QtActivity;

/**
 * This is the main activity used throughout the QtUtils. Either extend this in your main activity of your app, or initialize it.
 */
public class QutilsActivity extends QtActivity {
    private static QutilsActivity m_Instance;
    protected static NotificationClient m_NotificationClient;
    protected static AndroidUtils m_AndroidUtils;

    protected static boolean m_IsImmersiveModeEnabled = false;
    protected static boolean m_IsStatusBarVisible = true;
    public static HashMap m_CustomData;

    private static int m_PreviousKeyboardHeight = -1;
    private static int m_LastHandledNotificationID = -1;
    private static String m_LastHandledFCMMessageID = "";

    public QutilsActivity() {
        m_Instance = this;
        m_CustomData = new HashMap();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        m_NotificationClient = new NotificationClient(this);
        m_AndroidUtils = new AndroidUtils(this);

        final Window rootWindow = m_Instance.getWindow();
        final View rootView = rootWindow.getDecorView().findViewById(android.R.id.content);
        rootView.getViewTreeObserver().addOnGlobalLayoutListener(
            new ViewTreeObserver.OnGlobalLayoutListener() {
                public void onGlobalLayout() {
                    Display display = getWindowManager().getDefaultDisplay();
                    android.graphics.Point outSize = new android.graphics.Point();
                    display.getSize(outSize);

                    Rect rect = new Rect();
                    View view = rootWindow.getDecorView();
                    view.getWindowVisibleDisplayFrame(rect);

                    int screenHeight = outSize.y;
                    int keyboardHeight = screenHeight - (rect.bottom);
                    if (keyboardHeight < 0) {
                        keyboardHeight = 0;
                    }

                    if (m_PreviousKeyboardHeight == -1) {
                        m_PreviousKeyboardHeight = keyboardHeight;
                        CppCallbacks.keyboardHeightChanged(keyboardHeight);
                    }
                    else if (m_PreviousKeyboardHeight != keyboardHeight) {
                        m_PreviousKeyboardHeight = keyboardHeight;
                        CppCallbacks.keyboardHeightChanged(keyboardHeight);
                    }
                }
            }
        );
    }

    @Override
    public void onResume() {
        super.onResume();

        Intent intent = getIntent();
        checkForNotification(intent);
        handleAppLink(intent);
    }

    @Override
    public void onNewIntent(Intent intent) {
        super.onNewIntent(intent);

        setIntent(intent);
        checkForNotification(intent);
        handleAppLink(intent);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            if (m_IsImmersiveModeEnabled) {
                AndroidUtils.setImmersiveMode(true);
            }

            if (!m_IsStatusBarVisible) {
                AndroidUtils.setStatusBarVisible(false);
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) {
            if (requestCode == Constants.CAMERA_CAPTURE_REQUEST_CODE) {
                final String capturedFilePath = (String) getCustomData("capture_save_path");
                Bitmap bitmap = BitmapFactory.decodeFile(capturedFilePath);
                File file = new File(capturedFilePath);
                try {
                    bitmap = Utils.rotateImageIfRequired(bitmap, this, Uri.fromFile(file));
                    FileOutputStream fOut = new FileOutputStream(file);
                    bitmap.compress(Bitmap.CompressFormat.JPEG, 85, fOut);
                    fOut.flush();
                    fOut.close();
                }
                catch (IOException ex) {
                    Log.d("org.zmc.quitls", "Error rotating file: " + ex.getMessage());
                }

                CppCallbacks.cameraCaptured(capturedFilePath);
                removeCustomData("capture_save_path");
            }
            else if (requestCode == Constants.OPEN_GALLERY_REQUEST_CODE) {
                String filePath = Utils.getRealPathFromURI(getApplicationContext(), data.getData());
                CppCallbacks.fileSelected(filePath);
            }
        }
        else {
            if (requestCode == Constants.CAMERA_CAPTURE_REQUEST_CODE) {
                CppCallbacks.cameraCaptureCancelled();
            }
            else if (requestCode == Constants.OPEN_GALLERY_REQUEST_CODE) {
                CppCallbacks.fileSelectionCancelled();
            }
        }
    }

    static public void checkForNotification(Intent intent) {
        if (intent.getExtras() != null) {
            int id = intent.getIntExtra(NotificationReceiver.KEY_NOTIFICATION_ID, -1);
            String messageID = intent.getStringExtra("google.message_id");
            boolean isFCMNotification = messageID != null && messageID.length() > 0;

            if (id > -1 || isFCMNotification) {
                if (!isFCMNotification && m_LastHandledNotificationID == id) {
                    return;
                }

                if (isFCMNotification && m_LastHandledFCMMessageID == messageID) {
                    return;
                }

                String tag = intent.getStringExtra(NotificationReceiver.KEY_NOTIFICATION_TAG);
                String notificationManagerName = intent.getStringExtra(NotificationReceiver.KEY_NOTIFICATION_MANAGER);
                String payload;

                if (!isFCMNotification) {
                    payload = intent.getStringExtra(NotificationReceiver.KEY_PAYLOAD);
                    m_LastHandledNotificationID = id;
                }
                else {
                    m_LastHandledFCMMessageID = messageID;
                    Bundle extras = intent.getExtras();
                    Set<String> keySet = extras.keySet();
                    /*
                     * Here's the payload data that comes from the created notification by the Firebase SDK.
                     * {
                     *     "google.sent_time": 1501533936476,
                     *     "from": "353277717468",
                     *     "google.message_id": "0:1501533936482206%8fe8bb7f8fe8bb7f",
                     *     "collapse_key": "org.edmoware.arifname"
                     * }
                     */
                    StringBuilder jsonBuilder = new StringBuilder();
                    jsonBuilder.append("{");
                    int extrasSize = extras.size();
                    for (String key : keySet) {
                        extrasSize--;

                        if (key.equals("google.sent_time")) {
                            jsonBuilder.append("\"sent_time\":");
                        }
                        else if (key.equals("google.message_id")) {
                            jsonBuilder.append("\"message_id\":");
                        }
                        else {
                            jsonBuilder.append("\"" + key + "\":");
                        }

                        Object value = extras.get(key);
                        if (value.getClass().equals(String.class)) {
                            jsonBuilder.append("\"" + value + "\"");
                        }
                        else {
                            jsonBuilder.append(value);
                        }

                        if (extrasSize > 0) {
                            jsonBuilder.append(",");
                        }
                    }
                    jsonBuilder.append("}");
                    payload = jsonBuilder.toString();
                }

                CppCallbacks.notificationTapped(tag, id, notificationManagerName, payload);
            }
        }
    }

    public static void setImmersiveModeEnabled(boolean enabled) {
        m_IsImmersiveModeEnabled = enabled;
    }

    public static void setStatusBarVisible(boolean visible) {
        m_IsStatusBarVisible = visible;
    }

    public static boolean isStatusBarVisible() {
        return m_IsStatusBarVisible;
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

    public static void setCustomData(String key, Object value) {
        m_CustomData.put(key, value);
    }

    public static Object getCustomData(String key) {
        return m_CustomData.get(key);
    }

    public static void removeCustomData(String key) {
        m_CustomData.remove(key);
    }

    private void handleAppLink(Intent intent) {
        if (intent.getAction() != null) {
            String uri = intent.getDataString();
            if (uri != null) {
                CppCallbacks.openedWithURL(uri);
            }
        }
    }
}
