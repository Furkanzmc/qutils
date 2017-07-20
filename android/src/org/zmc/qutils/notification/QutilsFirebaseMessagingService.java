package org.zmc.qutils.notification;
// Java
import java.lang.StringBuilder;
import java.util.Map;
// Android
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;

import android.content.Intent;
import android.media.RingtoneManager;
import android.net.Uri;

import android.support.v4.app.NotificationCompat;
import android.util.Log;

// Firebase
import com.firebase.jobdispatcher.Constraint;
import com.firebase.jobdispatcher.FirebaseJobDispatcher;
import com.firebase.jobdispatcher.GooglePlayDriver;

import com.firebase.jobdispatcher.Job;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

public class QutilsFirebaseMessagingService extends FirebaseMessagingService {

    private static final String TAG = "QutilsFirebaseMessagingService";

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
        Log.d(TAG, "From: " + remoteMessage);
        convertRemoteMessageToJsonString(remoteMessage);

        // Check if message contains a data payload.
        if (remoteMessage.getData().size() > 0) {
            Log.d(TAG, "Message data payload: " + remoteMessage.getData());

            if (/* Check if data needs to be processed by long running job */ true) {
                // For long-running tasks (10 seconds or more) use Firebase Job Dispatcher.
                scheduleJob();
            } else {
                // Handle message within 10 seconds
                handleNow();
            }

        }

        // Also if you intend on generating your own notifications as a result of a received FCM
        // message, here is where that should be initiated. See sendNotification method below.
    }
    // [END receive_message]

    /**
     * Schedule a job using FirebaseJobDispatcher.
     */
    private void scheduleJob() {
        // [START dispatch_job]
        FirebaseJobDispatcher dispatcher = new FirebaseJobDispatcher(new GooglePlayDriver(this));
        Job myJob = dispatcher.newJobBuilder()
                .setService(QutilsJobService.class)
                .setTag("my-job-tag")
                .build();
        dispatcher.schedule(myJob);
        // [END dispatch_job]
    }

    /**
     * Handle time allotted to BroadcastReceivers.
     */
    private void handleNow() {
        Log.d(TAG, "Short lived task is done.");
    }

    private String convertRemoteMessageToJsonString(RemoteMessage remoteMessage) {
        StringBuilder jsonStr = new StringBuilder();
        jsonStr.append("{");
        // Append collapse key
        jsonStr.append("\"collapse_key\":\"");
        jsonStr.append(remoteMessage.getCollapseKey());
        jsonStr.append("\",");
        // Append from
        jsonStr.append("\"from\":\"");
        jsonStr.append(remoteMessage.getFrom());
        jsonStr.append("\",");
        // Append to
        jsonStr.append("\"to\":\"");
        jsonStr.append(remoteMessage.getTo() == null ? "null" : remoteMessage.getTo());
        jsonStr.append("\",");
        // Append message ID
        jsonStr.append("\"message_id\":\"");
        jsonStr.append(remoteMessage.getMessageId());
        jsonStr.append("\",");
        // Append message type
        jsonStr.append("\"message_type\":");
        jsonStr.append(remoteMessage.getMessageType() != null ? "\"" + remoteMessage.getMessageType() + "\"" : "null");
        jsonStr.append(",");
        // Append sent time
        jsonStr.append("\"sent_time\":");
        jsonStr.append(remoteMessage.getSentTime());
        jsonStr.append(",");
        // Append ttl
        jsonStr.append("\"ttl\":" + String.valueOf(remoteMessage.getTtl()));
        jsonStr.append(",");

        // Append data
        Map<String, String> data = remoteMessage.getData();
        if (data.size() > 0) {
            jsonStr.append("\"data\": {");
            int dataIndex = 0;
            for (Map.Entry<String, String> entry : data.entrySet()) {
                jsonStr.append("\"" + entry.getKey() + "\": ");
                jsonStr.append(entry.getValue() != null ? "\"" + entry.getValue() + "\"": "null");
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
        jsonStr.append("\"body\":\"");
        jsonStr.append(notification.getBody());
        jsonStr.append("\",");
        // Append click action
        jsonStr.append("\"click_action\":\"");
        jsonStr.append(notification.getClickAction());
        jsonStr.append("\",");
        // Append color
        jsonStr.append("\"color\":\"");
        jsonStr.append(notification.getColor());
        jsonStr.append("\",");
        // Append icon
        jsonStr.append("\"icon\":\"");
        jsonStr.append(notification.getIcon());
        jsonStr.append("\",");
        // Append sound
        jsonStr.append("\"sound\":\"");
        jsonStr.append(notification.getSound());
        jsonStr.append("\",");
        // Append link
        jsonStr.append("\"link\":\"");
        jsonStr.append(notification.getLink() != null ? notification.getLink().toString() : "null");
        jsonStr.append("\",");
        // Append tag
        jsonStr.append("\"tag\":\"");
        jsonStr.append(notification.getTag());
        jsonStr.append("\",");
        // Append title
        jsonStr.append("\"title\":\"");
        jsonStr.append(notification.getTitle());
        jsonStr.append("\"");

        jsonStr.append("}");

        return jsonStr.toString();
    }

    /**
     * Create and show a simple notification containing the received FCM message.
     *
     * @param messageBody FCM message body received.
     */
    private void sendNotification(String messageBody) {
        Intent intent = new Intent(this, NotificationReceiver.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0 /* Request code */, intent,
                PendingIntent.FLAG_ONE_SHOT);

        // .setSmallIcon(R.drawable.ic_stat_ic_notification)
        Uri defaultSoundUri= RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(this)
                .setContentTitle("FCM Message")
                .setContentText(messageBody)
                .setAutoCancel(true)
                .setSound(defaultSoundUri)
                .setContentIntent(pendingIntent);

        NotificationManager notificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);

        notificationManager.notify(0 /* ID of notification */, notificationBuilder.build());
    }
}
