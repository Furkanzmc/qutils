package org.zmc.qutils;

// Java
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;

// Android
import android.net.Uri;
import android.util.Log;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.Display;
import android.graphics.Rect;
import android.view.KeyEvent;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.ViewTreeObserver;

// Firebase
import com.google.firebase.messaging.MessageForwardingService;

// Qt
import org.qtproject.qt5.android.bindings.QtActivity;

/**
 * This is the main activity used throughout the QtUtils. Either extend this in your main activity of your app, or initialize it.
 */
public class QutilsActivity extends QtActivity {
    private static QutilsActivity m_Instance;
    protected static AndroidUtils m_AndroidUtils;
    protected static boolean m_IsImmersiveModeEnabled = false;

    protected static boolean m_IsStatusBarVisible = true;
    public static HashMap m_CustomData;
    private static int m_PreviousKeyboardHeight = -1;

    public QutilsActivity() {
        m_Instance = this;
        m_CustomData = new HashMap();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
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
        handleAppLink(intent);
    }

    @Override
    public void onNewIntent(Intent intent) {
        super.onNewIntent(intent);

        checkNotification(intent);

        setIntent(intent);
        handleAppLink(intent);
    }

    private void checkNotification(Intent intent) {
        Log.d("org.zmc.qutils", "A message was sent to this app while it was in the background.");
        Intent message = new Intent(this, MessageForwardingService.class);
        message.setAction(MessageForwardingService.ACTION_REMOTE_INTENT);
        message.putExtras(intent);
        message.setData(intent.getData());
        startService(message);
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
        else if (resultCode == RESULT_CANCELED) {
            if (requestCode == Constants.CAMERA_CAPTURE_REQUEST_CODE) {
                CppCallbacks.cameraCaptureCancelled();
            }
            else if (requestCode == Constants.OPEN_GALLERY_REQUEST_CODE) {
                CppCallbacks.fileSelectionCancelled();
            }
        }
        else {
            Log.d("org.zmc.quitls", "onActivityResult failed with result code " + String.valueOf(resultCode) + " for request " + String.valueOf(requestCode));
        }
    }

    public static void setImmersiveModeEnabled(boolean enabled) {
        m_IsImmersiveModeEnabled = enabled;
    }

    public static boolean isImmersiveModeEnabled() {
        return m_IsImmersiveModeEnabled;
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
