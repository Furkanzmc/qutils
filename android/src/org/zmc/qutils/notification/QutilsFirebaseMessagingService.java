package org.zmc.qutils.notification;
// Java
import java.lang.StringBuilder;
import java.util.Map;
// Android
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;

import android.content.Intent;
import android.media.RingtoneManager;
import android.net.Uri;

import android.support.v4.app.NotificationCompat;
import android.util.Log;
// Firebase
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

// qutils
import org.zmc.qutils.CppCallbacks;

public class QutilsFirebaseMessagingService extends FirebaseMessagingService {

    private static final String TAG = "org.zmc.qutils.notification";

    /**
     * Called when message is received.
     *
     * @param remoteMessage Object representing the message received from Firebase Cloud Messaging.
     */
    // [START receive_message]
    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        // [START_EXCLUDE]
        // There are two types of messages data messages and notification messages. Data messages are handled
        // here in onMessageReceived whether the app is in the foreground or background. Data messages are the type
        // traditionally used with GCM. Notification messages are only received here in onMessageReceived when the app
        // is in the foreground. When the app is in the background an automatically generated notification is displayed.
        // When the user taps on the notification they are returned to the app. Messages containing both notification
        // and data payloads are treated as notification messages. The Firebase console always sends notification
        // messages. For more see: https://firebase.google.com/docs/cloud-messaging/concept-options
        // [END_EXCLUDE]

        // TODO(developer): Handle FCM messages here.
        // Not getting messages here? See why this may be: https://goo.gl/39bRNJ

        sendNotification(remoteMessage);
        // Also if you intend on generating your own notifications as a result of a received FCM
        // message, here is where that should be initiated. See sendNotification method below.
    }
    // [END receive_message]

    private String convertRemoteMessageToJsonString(RemoteMessage remoteMessage) {
        StringBuilder jsonStr = new StringBuilder();
        jsonStr.append("{");
        // Append collapse key
        jsonStr.append("\"collapse_key\":");
        String value = remoteMessage.getCollapseKey();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append from
        jsonStr.append("\"from\":");
        value = remoteMessage.getFrom();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append to
        jsonStr.append("\"to\":");
        value = remoteMessage.getTo();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append message ID
        jsonStr.append("\"message_id\":");
        value = remoteMessage.getMessageId();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append message type
        jsonStr.append("\"message_type\":");
        value = remoteMessage.getMessageType();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append sent time
        jsonStr.append("\"sent_time\":");
        jsonStr.append(remoteMessage.getSentTime());
        jsonStr.append(",");
        // Append ttl
        jsonStr.append("\"ttl\":");
        jsonStr.append(remoteMessage.getTtl());
        jsonStr.append(",");

        // Append data
        Map<String, String> data = remoteMessage.getData();
        if (data.size() > 0) {
            jsonStr.append("\"data\": {");
            int dataIndex = 0;
            for (Map.Entry<String, String> entry : data.entrySet()) {
                jsonStr.append("\"" + entry.getKey() + "\": ");
                value = entry.getValue();
                jsonStr.append(value == null ? "null" : "\"" + value + "\"");
                if (dataIndex < data.size() - 1) {
                    jsonStr.append(",");
                }
                dataIndex++;
            }
            jsonStr.append("}");
        }

        if (remoteMessage.getNotification() != null) {
            jsonStr.append(",");
            jsonStr.append(getRemoteMessageNotificationStr(remoteMessage.getNotification()));
        }

        jsonStr.append("}");

        return jsonStr.toString();
    }

    private String getRemoteMessageNotificationStr(RemoteMessage.Notification notification) {
        StringBuilder jsonStr = new StringBuilder();
        jsonStr.append("\"notification\": {");
        // Append body
        jsonStr.append("\"body\":");
        String value = notification.getBody();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append click action
        jsonStr.append("\"click_action\":");
        value = notification.getClickAction();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append color
        jsonStr.append("\"color\":");
        value = notification.getColor();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append icon
        jsonStr.append("\"icon\":");
        value = notification.getIcon();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append sound
        jsonStr.append("\"sound\":");
        value = notification.getSound();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append link
        jsonStr.append("\"link\":");
        value = notification.getLink() != null ? notification.getLink().toString() : null;
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append tag
        jsonStr.append("\"tag\":");
        value = notification.getTag();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");
        jsonStr.append(",");
        // Append title
        jsonStr.append("\"title\":");
        value = notification.getTitle();
        jsonStr.append(value == null ? "null" : "\"" + value + "\"");

        jsonStr.append("}");

        return jsonStr.toString();
    }

    /**
     * Create and show a simple notification containing the received FCM message.
     *
     * @param messageBody FCM message body received.
     */
    private void sendNotification(RemoteMessage remoteMessage) {
        NotificationClient.resetProperties();
        NotificationClient.setFlags(Notification.FLAG_AUTO_CANCEL);
        RemoteMessage.Notification notification = remoteMessage.getNotification();
        String title = "Notification", message = "Message";
        if (notification != null) {
            title = notification.getTitle();
            message = notification.getBody();
        }

        NotificationClient.notify(message, title, convertRemoteMessageToJsonString(remoteMessage), 0);
    }
}
