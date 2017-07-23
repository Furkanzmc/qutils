package org.zmc.qutils;

public class CppCallbacks
{
    public static native void notificationReceived(String tag, int id, String notificationManagerName, String notificationPayload);
    public static native void backButtonPressed();
    public static native void menuButtonPressed();

    // If an item is clicked, index will be non-zero
    public static native void alertDialogClicked(int buttonIndex);
    // If all of the dates are -1, then the datePickerCancelled signal is emitted.
    public static native void datePicked(int year, int month, int day);
    // If all of the parameters are -1, then the timePickerCancelled signal is emitted.
    public static native void timePicked(int hourOfDay, int minute);

    public static native void cameraCaptured(String capturePath);
    public static native void cameraCaptureCancelled();
    // This is called when user selects an image from the gallery.
    public static native void fileSelected(String filePath);

    public static native void fileSelectionCancelled();
    // keyboadrdHeight will be 0 If it is invisible, otherwise it will have a value greater than 0.
    public static native void keyboardHeightChanged(int keyboardHeight);
    public static native void fcmTokenReceived(String token);
}
