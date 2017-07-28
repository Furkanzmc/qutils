package org.zmc.qutils.notification;
// Android
import android.util.Log;
// Firebase
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.FirebaseInstanceIdService;
// qutils
import org.zmc.qutils.CppCallbacks;


public class QutilsFirebaseInstanceIDService extends FirebaseInstanceIdService {

    private static final String TAG = "org.zmc.qutils.notification";

    /**
     * Called if InstanceID token is updated. This may occur if the security of
     * the previous token had been compromised. Note that this is called when the InstanceID token
     * is initially generated so this is where you would retrieve the token.
     */
    // [START refresh_token]
    @Override
    public void onTokenRefresh() {
        // Get updated InstanceID token.
        String refreshedToken = FirebaseInstanceId.getInstance().getToken();
        Log.d(TAG, refreshedToken);
        CppCallbacks.fcmTokenReceived(refreshedToken);
    }
}
