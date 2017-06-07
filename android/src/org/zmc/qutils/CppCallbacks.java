package org.zmc.qutils.notification;

public class CppCallbacks
{
    public static native void notificationReceived(String tag, int id, String notificationManagerName);
    public static native void backButtonPressed();
    public static native void menuButtonPressed();

    // If an item is clicked, item index will be non-zero and buttonType will be -3
    public static native void alertDialogClicked(int buttonType, int itemIndex);
    // If all of the dates are -1, then the datePickerCancelled signal is emitted.
    public static native void datePicked(int year, int month, int day);
    // If all of the parameters are -1, then the timePickerCancelled signal is emitted.
    public static native void timePicked(int hourOfDay, int minute);

    public static native void cameraCaptured(String capturePath);
}
