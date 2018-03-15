package org.zmc.qutils;

import android.content.Context;
import android.support.v4.content.FileProvider;
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
import android.app.DialogFragment;

import android.provider.MediaStore;
import android.os.Environment;
import android.net.Uri;

import android.view.inputmethod.InputMethodManager;
import android.widget.Toast;

// Java
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.io.File;
import java.io.IOException;

import java.util.ArrayList;


// qutils
import org.zmc.qutils.CppCallbacks;
import org.zmc.qutils.QutilsActivity;
import org.zmc.qutils.DatePickerFragment;

import org.zmc.qutils.TimePickerFragment;
import org.zmc.qutils.Constants;

// Qt
import org.qtproject.qt5.android.bindings.QtActivity;

public class AndroidUtils extends QtActivity {
    private static QutilsActivity m_MainContext;
    private static String m_LastStatusBarColor;

    public AndroidUtils(QutilsActivity mainActivity) {
        m_MainContext = mainActivity;
    }

    public static void dispatchTakePictureIntent(String photoName) {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(m_MainContext.getPackageManager()) != null) {
            m_MainContext.setResult(m_MainContext.RESULT_OK, takePictureIntent);
            try {
                File file = createImageFile();
                Uri photoURI = null;
                if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
                    photoURI = Uri.fromFile(file);
                }
                else {
                    try {
                        photoURI = FileProvider.getUriForFile(m_MainContext, m_MainContext.getPackageName() + ".provider", file);
                    }
                    catch (IllegalArgumentException ex) {
                        Log.d("org.zmc.qutils", ex.getMessage());
                    }
                }

                if (photoURI != null) {
                    m_MainContext.setCustomData("capture_save_path", file.getAbsolutePath());
                    takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI);
                    m_MainContext.startActivityForResult(takePictureIntent, Constants.CAMERA_CAPTURE_REQUEST_CODE);
                }
            }
            catch (IOException ex) {
                Log.d("org.zmc.qutils", ex.getMessage());
            }
        }
    }

    static private File createImageFile() throws IOException {
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = "JPEG_" + timeStamp + "_";

        File storageDir = m_MainContext.getExternalCacheDir();
        File image = File.createTempFile(
                imageFileName, // Prefix
                ".jpg",        // Suffix
                storageDir     // Directory
        );

        return image;
    }

    public static void dismissKeyboard() {
        View view = m_MainContext.getCurrentFocus();
        if (view != null) {
            InputMethodManager inputManager = (InputMethodManager)m_MainContext.getSystemService(Context.INPUT_METHOD_SERVICE);
            inputManager.hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
        }
    }

    public static void setStatusBarColor(String colorHexStr) {
        if (Build.VERSION.SDK_INT >= 21) {
            if (m_MainContext != null) {
                Window window = m_MainContext.getWindow();
                if (window != null) {
                    m_LastStatusBarColor = colorHexStr;
                    // clear FLAG_TRANSLUCENT_STATUS flag:
                    window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
                    // add FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS flag to the window
                    window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                    // finally change the color
                    window.setStatusBarColor(Color.parseColor(colorHexStr));
                }
            }
        }
    }

    public static void setStatusBarVisible(boolean visible) {
        if (Build.VERSION.SDK_INT < 16) {
            m_MainContext.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        } else {
            View decorView = m_MainContext.getWindow().getDecorView();
            m_MainContext.setStatusBarVisible(visible);
            // Hide the status bar.
            if (visible == false) {
                decorView.setSystemUiVisibility(
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                | View.SYSTEM_UI_FLAG_FULLSCREEN);
            } else {
                decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
            }
        }
    }

    public static boolean isStatusBarVisible() {
        return m_MainContext.isStatusBarVisible();
    }

    public static void setImmersiveMode(boolean enabled) {
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
            } else {
                decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
            }
        }
    }

    public static void shareText(String dialogTitle, String text) {
        Intent sendIntent = new Intent();
        sendIntent.setAction(Intent.ACTION_SEND);
        sendIntent.putExtra(Intent.EXTRA_TEXT, text);
        sendIntent.setType("text/plain");
        m_MainContext.startActivity(Intent.createChooser(sendIntent, dialogTitle));
    }

    public static void showAlertDialog(HashMap properties) {
        AlertDialog.Builder builder = new AlertDialog.Builder(m_MainContext);
        builder.setCancelable(false);
        builder.setTitle((String) properties.get("title"));
        if (properties.containsKey("message")) {
            builder.setMessage((String) properties.get("message"));
        }

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

        String buttonText = "";

        if (properties.containsKey("positive")) {
            buttonText = (String) properties.get("positive");
            builder.setPositiveButton(buttonText, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int id) {
                    CppCallbacks.alertDialogClicked(1);
                }
            });
        }
        if (properties.containsKey("neutral")) {
            buttonText = (String) properties.get("neutral");
            builder.setNeutralButton(buttonText, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int id) {
                    CppCallbacks.alertDialogClicked(0);
                }
            });
        }
        if (properties.containsKey("negative")) {
            buttonText = (String) properties.get("negative");
            builder.setNegativeButton(buttonText, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int id) {
                    CppCallbacks.alertDialogClicked(-1);
                }
            });
        }

        if (properties.containsKey("items")) {
            ArrayList list = (ArrayList) properties.get("items");
            CharSequence[] items = new CharSequence[list.size()];
            for (int i = 0; i < items.length; i++) {
                items[i] = list.get(i).toString();
            }

            builder.setItems(items, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    CppCallbacks.alertDialogClicked(which);
                }
            });
        }

        final AlertDialog dialog = builder.create();
        dialog.show();
    }

    public static void showDatePicker() {
        DialogFragment newFragment = new DatePickerFragment(m_MainContext);
        newFragment.show(m_MainContext.getFragmentManager(), "datePicker");
    }

    public static void showTimePicker() {
        DialogFragment newFragment = new TimePickerFragment(m_MainContext);
        newFragment.show(m_MainContext.getFragmentManager(), "timePicker");
    }

    public static void showTaost(String text, boolean isLongDuration) {
        int duration = Toast.LENGTH_SHORT;
        if (isLongDuration == true) {
            duration = Toast.LENGTH_LONG;
        }

        Toast toast = Toast.makeText(m_MainContext, text, duration);
        toast.show();
    }

    public static void openGallery(String fileType) {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        String[] mimetypes = fileType.split("\\|");
        intent.putExtra(Intent.EXTRA_MIME_TYPES, mimetypes);
        m_MainContext.startActivityForResult(intent, Constants.OPEN_GALLERY_REQUEST_CODE);
    }

    public static String getStatusBarColor() {
        return m_LastStatusBarColor;
    }

    public static String getDeviceModel() {
        return Build.MODEL;
    }
}
