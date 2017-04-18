package org.zmc.qutils.notification;

public class CppCallbacks
{
    public static native void notificationReceived(String tag, int id, String notificationManagerName);
    public static native void backButtonPressed();
    public static native void menuButtonPressed();
}
