#pragma once
// Qt
#include <QMap>
#include <QObject>
#if defined(Q_OS_ANDROID)
    #include <QtAndroid>
#endif // Q_OS_ANDROID
#if defined(Q_OS_IOS)
    #include "qutils/ios/PermissionManagerPrivate.h"
#endif // Q_OS_IOS

namespace zmc
{

namespace AndroidPermNames
{
static const char *ACCEPT_HANDOVER = "android.permission.ACCEPT_HANDOVER";
static const char *ACCESS_CHECKIN_PROPERTIES = "android.permission.ACCESS_CHECKIN_PROPERTIES";
static const char *ACCESS_COARSE_LOCATION = "android.permission.ACCESS_COARSE_LOCATION";
static const char *ACCESS_FINE_LOCATION = "android.permission.ACCESS_FINE_LOCATION";
static const char *ACCESS_LOCATION_EXTRA_COMMANDS = "android.permission.ACCESS_LOCATION_EXTRA_COMMANDS";
static const char *ACCESS_NETWORK_STATE = "android.permission.ACCESS_NETWORK_STATE";
static const char *ACCESS_NOTIFICATION_POLICY = "android.permission.ACCESS_NOTIFICATION_POLICY";
static const char *ACCESS_WIFI_STATE = "android.permission.ACCESS_WIFI_STATE";
static const char *ACCOUNT_MANAGER = "android.permission.ACCOUNT_MANAGER";
static const char *ADD_VOICEMAIL = "android.permission.ADD_VOICEMAIL";
static const char *ANSWER_PHONE_CALLS = "android.permission.ANSWER_PHONE_CALLS";
static const char *BATTERY_STATS = "android.permission.BATTERY_STATS";
static const char *BIND_ACCESSIBILITY_SERVICE = "android.permission.BIND_ACCESSIBILITY_SERVICE";
static const char *BIND_APPWIDGET = "android.permission.BIND_APPWIDGET";
static const char *BIND_AUTOFILL_SERVICE = "android.permission.BIND_AUTOFILL_SERVICE";
static const char *BIND_CARRIER_MESSAGING_SERVICE = "android.permission.BIND_CARRIER_MESSAGING_SERVICE";
static const char *BIND_CARRIER_SERVICES = "android.permission.BIND_CARRIER_SERVICES";
static const char *BIND_CHOOSER_TARGET_SERVICE = "android.permission.BIND_CHOOSER_TARGET_SERVICE";
static const char *BIND_CONDITION_PROVIDER_SERVICE = "android.permission.BIND_CONDITION_PROVIDER_SERVICE";
static const char *BIND_DEVICE_ADMIN = "android.permission.BIND_DEVICE_ADMIN";
static const char *BIND_DREAM_SERVICE = "android.permission.BIND_DREAM_SERVICE";
static const char *BIND_INCALL_SERVICE = "android.permission.BIND_INCALL_SERVICE";
static const char *BIND_INPUT_METHOD = "android.permission.BIND_INPUT_METHOD";
static const char *BIND_MIDI_DEVICE_SERVICE = "android.permission.BIND_MIDI_DEVICE_SERVICE";
static const char *BIND_NFC_SERVICE = "android.permission.BIND_NFC_SERVICE";
static const char *BIND_NOTIFICATION_LISTENER_SERVICE = "android.permission.BIND_NOTIFICATION_LISTENER_SERVICE";
static const char *BIND_PRINT_SERVICE = "android.permission.BIND_PRINT_SERVICE";
static const char *BIND_QUICK_SETTINGS_TILE = "android.permission.BIND_QUICK_SETTINGS_TILE";
static const char *BIND_REMOTEVIEWS = "android.permission.BIND_REMOTEVIEWS";
static const char *BIND_SCREENING_SERVICE = "android.permission.BIND_SCREENING_SERVICE";
static const char *BIND_TELECOM_CONNECTION_SERVICE = "android.permission.BIND_TELECOM_CONNECTION_SERVICE";
static const char *BIND_TEXT_SERVICE = "android.permission.BIND_TEXT_SERVICE";
static const char *BIND_TV_INPUT = "android.permission.BIND_TV_INPUT";
static const char *BIND_VISUAL_VOICEMAIL_SERVICE = "android.permission.BIND_VISUAL_VOICEMAIL_SERVICE";
static const char *BIND_VOICE_INTERACTION = "android.permission.BIND_VOICE_INTERACTION";
static const char *BIND_VPN_SERVICE = "android.permission.BIND_VPN_SERVICE";
static const char *BIND_VR_LISTENER_SERVICE = "android.permission.BIND_VR_LISTENER_SERVICE";
static const char *BIND_WALLPAPER = "android.permission.BIND_WALLPAPER";
static const char *BLUETOOTH = "android.permission.BLUETOOTH";
static const char *BLUETOOTH_ADMIN = "android.permission.BLUETOOTH_ADMIN";
static const char *BLUETOOTH_PRIVILEGED = "android.permission.BLUETOOTH_PRIVILEGED";
static const char *BODY_SENSORS = "android.permission.BODY_SENSORS";
static const char *BROADCAST_PACKAGE_REMOVED = "android.permission.BROADCAST_PACKAGE_REMOVED";
static const char *BROADCAST_SMS = "android.permission.BROADCAST_SMS";
static const char *BROADCAST_STICKY = "android.permission.BROADCAST_STICKY";
static const char *BROADCAST_WAP_PUSH = "android.permission.BROADCAST_WAP_PUSH";
static const char *CALL_PHONE = "android.permission.CALL_PHONE";
static const char *CALL_PRIVILEGED = "android.permission.CALL_PRIVILEGED";
static const char *CAMERA = "android.permission.CAMERA";
static const char *CAPTURE_AUDIO_OUTPUT = "android.permission.CAPTURE_AUDIO_OUTPUT";
static const char *CAPTURE_SECURE_VIDEO_OUTPUT = "android.permission.CAPTURE_SECURE_VIDEO_OUTPUT";
static const char *CAPTURE_VIDEO_OUTPUT = "android.permission.CAPTURE_VIDEO_OUTPUT";
static const char *CHANGE_COMPONENT_ENABLED_STATE = "android.permission.CHANGE_COMPONENT_ENABLED_STATE";
static const char *CHANGE_CONFIGURATION = "android.permission.CHANGE_CONFIGURATION";
static const char *CHANGE_NETWORK_STATE = "android.permission.CHANGE_NETWORK_STATE";
static const char *CHANGE_WIFI_MULTICAST_STATE = "android.permission.CHANGE_WIFI_MULTICAST_STATE";
static const char *CHANGE_WIFI_STATE = "android.permission.CHANGE_WIFI_STATE";
static const char *CLEAR_APP_CACHE = "android.permission.CLEAR_APP_CACHE";
static const char *CONTROL_LOCATION_UPDATES = "android.permission.CONTROL_LOCATION_UPDATES";
static const char *DELETE_CACHE_FILES = "android.permission.DELETE_CACHE_FILES";
static const char *DELETE_PACKAGES = "android.permission.DELETE_PACKAGES";
static const char *DIAGNOSTIC = "android.permission.DIAGNOSTIC";
static const char *DISABLE_KEYGUARD = "android.permission.DISABLE_KEYGUARD";
static const char *DUMP = "android.permission.DUMP";
static const char *EXPAND_STATUS_BAR = "android.permission.EXPAND_STATUS_BAR";
static const char *FACTORY_TEST = "android.permission.FACTORY_TEST";
static const char *FOREGROUND_SERVICE = "android.permission.FOREGROUND_SERVICE";
static const char *GET_ACCOUNTS = "android.permission.GET_ACCOUNTS";
static const char *GET_ACCOUNTS_PRIVILEGED = "android.permission.GET_ACCOUNTS_PRIVILEGED";
static const char *GET_PACKAGE_SIZE = "android.permission.GET_PACKAGE_SIZE";
static const char *GET_TASKS = "android.permission.GET_TASKS";
static const char *GLOBAL_SEARCH = "android.permission.GLOBAL_SEARCH";
static const char *INSTALL_LOCATION_PROVIDER = "android.permission.INSTALL_LOCATION_PROVIDER";
static const char *INSTALL_PACKAGES = "android.permission.INSTALL_PACKAGES";
static const char *INSTALL_SHORTCUT = "android.permission.INSTALL_SHORTCUT";
static const char *INSTANT_APP_FOREGROUND_SERVICE = "android.permission.INSTANT_APP_FOREGROUND_SERVICE";
static const char *INTERNET = "android.permission.INTERNET";
static const char *KILL_BACKGROUND_PROCESSES = "android.permission.KILL_BACKGROUND_PROCESSES";
static const char *LOCATION_HARDWARE = "android.permission.LOCATION_HARDWARE";
static const char *MANAGE_DOCUMENTS = "android.permission.MANAGE_DOCUMENTS";
static const char *MANAGE_OWN_CALLS = "android.permission.MANAGE_OWN_CALLS";
static const char *MEDIA_CONTENT_CONTROL = "android.permission.MEDIA_CONTENT_CONTROL";
static const char *MODIFY_AUDIO_SETTINGS = "android.permission.MODIFY_AUDIO_SETTINGS";
static const char *MODIFY_PHONE_STATE = "android.permission.MODIFY_PHONE_STATE";
static const char *MOUNT_FORMAT_FILESYSTEMS = "android.permission.MOUNT_FORMAT_FILESYSTEMS";
static const char *MOUNT_UNMOUNT_FILESYSTEMS = "android.permission.MOUNT_UNMOUNT_FILESYSTEMS";
static const char *NFC_PERM = "android.permission.NFC";
static const char *NFC_TRANSACTION_EVENT = "android.permission.NFC_TRANSACTION_EVENT";
static const char *PACKAGE_USAGE_STATS = "android.permission.PACKAGE_USAGE_STATS";
static const char *PROCESS_OUTGOING_CALLS = "android.permission.PROCESS_OUTGOING_CALLS";
static const char *READ_CALENDAR = "android.permission.READ_CALENDAR";
static const char *READ_CALL_LOG = "android.permission.READ_CALL_LOG";
static const char *READ_CONTACTS = "android.permission.READ_CONTACTS";
static const char *READ_EXTERNAL_STORAGE = "android.permission.READ_EXTERNAL_STORAGE";
static const char *READ_FRAME_BUFFER = "android.permission.READ_FRAME_BUFFER";
static const char *READ_LOGS = "android.permission.READ_LOGS";
static const char *READ_PHONE_NUMBERS = "android.permission.READ_PHONE_NUMBERS";
static const char *READ_PHONE_STATE = "android.permission.READ_PHONE_STATE";
static const char *READ_SMS = "android.permission.READ_SMS";
static const char *READ_SYNC_SETTINGS = "android.permission.READ_SYNC_SETTINGS";
static const char *READ_SYNC_STATS = "android.permission.READ_SYNC_STATS";
static const char *READ_VOICEMAIL = "android.permission.READ_VOICEMAIL";
static const char *REBOOT = "android.permission.REBOOT";
static const char *RECEIVE_BOOT_COMPLETED = "android.permission.RECEIVE_BOOT_COMPLETED";
static const char *RECEIVE_MMS = "android.permission.RECEIVE_MMS";
static const char *RECEIVE_SMS = "android.permission.RECEIVE_SMS";
static const char *RECEIVE_WAP_PUSH = "android.permission.RECEIVE_WAP_PUSH";
static const char *RECORD_AUDIO = "android.permission.RECORD_AUDIO";
static const char *REORDER_TASKS = "android.permission.REORDER_TASKS";
static const char *REQUEST_COMPANION_RUN_IN_BACKGROUND = "android.permission.REQUEST_COMPANION_RUN_IN_BACKGROUND";
static const char *REQUEST_COMPANION_USE_DATA_IN_BACKGROUND = "android.permission.REQUEST_COMPANION_USE_DATA_IN_BACKGROUND";
static const char *REQUEST_DELETE_PACKAGES = "android.permission.REQUEST_DELETE_PACKAGES";
static const char *REQUEST_IGNORE_BATTERY_OPTIMIZATION = "android.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATION";
static const char *REQUEST_INSTALL_PACKAGES = "android.permission.REQUEST_INSTALL_PACKAGES";
static const char *SEND_RESPOND_VIA_MESSAGE = "android.permission.SEND_RESPOND_VIA_MESSAGE";
static const char *SEND_SMS = "android.permission.SEND_SMS";
static const char *SET_ALARM = "android.permission.SET_ALARM";
static const char *SET_ALWAYS_FINISH = "android.permission.SET_ALWAYS_FINISH";
static const char *SET_ANIMATION_SCALE = "android.permission.SET_ANIMATION_SCALE";
static const char *SET_DEBUG_APP = "android.permission.SET_DEBUG_APP";
static const char *SET_PROCESS_LIMIT = "android.permission.SET_PROCESS_LIMIT";
static const char *SET_TIME = "android.permission.SET_TIME";
static const char *SET_TIME_ZONE = "android.permission.SET_TIME_ZONE";
static const char *SET_WALLPAPER = "android.permission.SET_WALLPAPER";
static const char *SET_WALLPAPER_HINTS = "android.permission.SET_WALLPAPER_HINTS";
static const char *SIGNAL_PERSISTENT_PROCESSES = "android.permission.SIGNAL_PERSISTENT_PROCESSES";
static const char *STATUS_BAR = "android.permission.STATUS_BAR";
static const char *SYSTEM_ALERT_WINDOW = "android.permission.SYSTEM_ALERT_WINDOW";
static const char *TRANSMIT_IR = "android.permission.TRANSMIT_IR";
static const char *UNINSTALL_SHORTCUT = "android.permission.UNINSTALL_SHORTCUT";
static const char *UPDATE_DEVICE_STATS = "android.permission.UPDATE_DEVICE_STATS";
static const char *USE_BIOMETRIC = "android.permission.USE_BIOMETRIC";
static const char *USE_FINGERPRINT = "android.permission.USE_FINGERPRINT";
static const char *USE_SIP = "android.permission.USE_SIP";
static const char *VIBRATE = "android.permission.VIBRATE";
static const char *WAKE_LOCK = "android.permission.WAKE_LOCK";
static const char *WRITE_APN_SETTINGS = "android.permission.WRITE_APN_SETTINGS";
static const char *WRITE_CALENDAR = "android.permission.WRITE_CALENDAR";
static const char *WRITE_CALLLOG = "android.permission.WRITE_CALLLOG";
static const char *WRITE_CONTACTS = "android.permission.WRITE_CONTACTS";
static const char *WRITE_EXTERNAL_STORAGE = "android.permission.WRITE_EXTERNAL_STORAGE";
static const char *WRITE_GSERVICES = "android.permission.WRITE_GSERVICES";
static const char *WRITE_SECURE_SETTINGS = "android.permission.WRITE_SECURE_SETTINGS";
static const char *WRITE_SETTINGS = "android.permission.WRITE_SETTINGS";
static const char *WRITE_SYNC_SETTINGS = "android.permission.WRITE_SYNC_SETTINGS";
static const char *WRITE_VOICEMAIL = "android.permission.WRITE_VOICEMAIL";
}

class PermissionRequestResult : public QObject
{
    Q_OBJECT Q_PROPERTY(int permission READ permission CONSTANT)
    Q_PROPERTY(int result READ result CONSTANT)

public:
    PermissionRequestResult(QObject *parent = nullptr)
        : QObject(parent)
    {}


    int permission() const
    {
        return m_Permission;
    }

    void setPermission(const int &permission)
    {
        m_Permission = permission;
    }

    int result() const
    {
        return m_Result;
    }

    void setResult(const int &result)
    {
        m_Result = result;
    }

private:
    int m_Permission;
    int m_Result;
};

/*!
 * \class PermissionManager
 * \brief The PermissionManager class is a cross-platform implementation of checking for and requesting permissions.
 */
class PermissionManager : public QObject
{
    Q_OBJECT

public:

    /*!
     * \enum Result
     * \brief The Result enum provides information about the permission result check.
     *
     * \value Denied
     *        The permission is denied. This value is common on all platforms.
     * \value Granted
     *        The permission is granted. This value is common on all platforms.
     * \value Restricted
     *        This value is present on iOS. Your app is not authorized to access the photo library, and the user
     *        cannot grant such permission.
     * \value NotDetermined
     *        This permission has not been asked to the user explicitly. This is only available on iOS. On Android, no
     *        such information is given.
     * \value AuthorizedWhenInUse
     *        This is used only on iOS for location access.
     */
    enum Result {
        Denied = 0,
        Granted = 1,
        Restricted = 2,
        NotDetermined = 3,
        AuthorizedWhenInUse = 4
    };
    Q_ENUM(Result);

    /*!
     * \enum Permissions
     * \brief The Permissions enum contains the permissions for all platforms.
     *
     * For a complete overview of Android permissions, see \l {https://developer.android.com/reference/android/Manifest.permission}
     * {here}.
     *
     * \value None
     *        Represents an invalid permission. \b {Android only.}
     * \value AcceptHandover
     *        Allows a calling app to continue a call which was started in another app. \b {Android only.}
     * \value AccessCheckInProperties
     *        Allows read/write access to the "properties" table in the checkin database, to change values that get \b {Android only.}
     *        uploaded.
     * \value AccessCoarseLocation
     *        Allows an app to access approximate location. \b {Android only.}
     * \value AccessFineLocation
     *        Allows an app to access precise location. \b {Android only.}
     * \value AccessLocationExtraCommands
     *        Allows an application to access extra location provider commands. \b {Android only.}
     * \value AccessNetworkState
     *        Allows applications to access information about networks. \b {Android only.}
     * \value AccessNotificationPolicy
     *        Marker permission for applications that wish to access notification policy. \b {Android only.}
     * \value AccessWiFiState
     *        Allows applications to access information about Wi-Fi networks. \b {Android only.}
     * \value AccountManager
     *        Allows applications to call into AccountAuthenticators. \b {Android only.}
     * \value AddVoiceMail
     *        Allows an application to add voicemails into the system. \b {Android only.}
     * \value AnswerPhoneCalls
     *        Allows the app to answer an incoming phone call. \b {Android only.}
     * \value BatteryStats
     *        Allows an application to collect battery statistics. \b {Android only.}
     * \value BindAccessibilityService
     *        Must be required by an AccessibilityService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindAppWidget
     *        Allows an application to tell the AppWidget service which application can access AppWidget's data. \b {Android only.}
     * \value BindAutofillService
     *        Must be required by a AutofillService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindCarrierMessagingService
     *        This constant was deprecated in API level 23. \b {Use BindCarrierServices instead}. \b {Android only.}
     * \value BindCarrierServices
     *        The system process that is allowed to bind to services in carrier apps will have this permission. \b {Android only.}
     * \value BindChooserTargetService
     *        Must be required by a ChooserTargetService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindConditionProviderService
     *        Must be required by a ConditionProviderService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindDeviceAdmin
     *        Must be required by device administration receiver, to ensure that only the system can interact with it. \b {Android only.}
     * \value BindDreamService
     *        Must be required by an DreamService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindInCallService
     *        Must be required by a InCallService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindInputMethod
     *        Must be required by an InputMethodService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindMidiDeviceService
     *        Must be required by an MidiDeviceService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindNFCService
     *        Must be required by a HostApduService or OffHostApduService to ensure that only the system can bind to it. \b {Android only.}
     * \value BindNotificationListenerService
     *        Must be required by an NotificationListenerService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindPrintService
     *        Must be required by a PrintService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindQuickSettingsTile
     *        Allows an application to bind to third party quick settings tiles. \b {Android only.}
     * \value BindRemoteViews
     *        Must be required by a RemoteViewsService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindScreeningService
     *        Must be required by a CallScreeningService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindTelecomConnectionService
     *        Must be required by a ConnectionService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindTextService
     *        Must be required by a TextService (e.g. SpellCheckerService) to ensure that only the system can bind to it. \b {Android only.}
     * \value BindTVInput
     *        Must be required by a TvInputService to ensure that only the system can bind to it. \b {Android only.}
     * \value BindVisualVoiceMailService
     *        Must be required by a link VisualVoicemailService to ensure that only the system can bind to it. \b {Android only.}
     * \value BindVoiceInteraction
     *        Must be required by a VoiceInteractionService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindVPNService
     *        Must be required by a VpnService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindVRListenerService
     *        Must be required by an VrListenerService, to ensure that only the system can bind to it. \b {Android only.}
     * \value BindWallpaper
     *        Must be required by a WallpaperService, to ensure that only the system can bind to it. \b {Android only.}
     * \value Bluetooth
     *        Allows applications to connect to paired bluetooth devices. \b {Android only.}
     * \value BluetoothAdmin
     *        Allows applications to discover and pair bluetooth devices. \b {Android only.}
     * \value BluetoothPrivileged
     *        Allows applications to pair bluetooth devices without user interaction, and to allow or disallow phonebook
     *        access or message access. \b {Android only.}
     * \value BodySensors
     *        Allows an application to access data from sensors that the user uses to measure what is happening inside
     *        his/her body, such as heart rate. \b {Android only.}
     * \value BroadcastPackageRemoved
     *        Allows an application to broadcast a notification that an application package has been removed. \b {Android only.}
     * \value BroadcastSMS
     *        Allows an application to broadcast an SMS receipt notification. \b {Android only.}
     * \value BroadcastSticky
     *        Allows an application to broadcast sticky intents. \b {Android only.}
     * \value BroadcastWAPPush
     *        Allows an application to broadcast a WAP PUSH receipt notification. \b {Android only.}
     * \value CallPhone
     *        Allows an application to initiate a phone call without going through the Dialer user interface for the
     *        user to confirm the call. \b {Android only.}
     * \value CallPrivileged
     *        Allows an application to call any phone number, including emergency numbers, without going through the
     *        Dialer user interface for the user to confirm the call being placed. \b {Android only.}
     * \value Camera
     *        Required to be able to access the camera device. \b {Android and iOS.}
     * \value CaptureAudioOutput
     *        Allows an application to capture audio output. \b {Android only.}
     * \value CaptureSecureVideoOutput
     *        Allows an application to capture secure video output. \b {Android only.}
     * \value CaptureVideoOutput
     *        Allows an application to capture video output. \b {Android only.}
     * \value ChangeComponentEnabledState
     *        Allows an application to change whether an application component (other than its own) is enabled or not. \b {Android only.}
     * \value ChangeConfiguration
     *        Allows an application to modify the current configuration, such as locale. \b {Android only.}
     * \value ChangeNetworkState
     *        Allows applications to change network connectivity state. \b {Android only.}
     * \value ChangeWiFiMulticastState
     *        Allows applications to enter Wi-Fi Multicast mode. \b {Android only.}
     * \value ChangeWiFiState
     *        Allows applications to change Wi-Fi connectivity state. \b {Android only.}
     * \value ClearAppCache
     *        Allows an application to clear the caches of all installed applications on the device. \b {Android only.}
     * \value ControlLocationUpdates
     *        Allows enabling/disabling location update notifications from the radio. \b {Android only.}
     * \value DeleteCacheFiles
     *        Old permission for deleting an app's cache files, no longer used, but signals for us to quietly ignore
     *        calls instead of throwing an exception. \b {Android only.}
     * \value DeletePackages
     *        Allows an application to delete packages. \b {Android only.}
     * \value Diagnostic
     *        Allows applications to RW to diagnostic resources. \b {Android only.}
     * \value DisableKeyguard
     *        Allows applications to disable the keyguard if it is not secure. \b {Android only.}
     * \value Dump
     *        Allows an application to retrieve state dump information from system services. \b {Android only.}
     * \value ExpandStatusBar
     *        Allows an application to expand or collapse the status bar. \b {Android only.}
     * \value FactoryTest
     *        Run as a manufacturer test application, running as the root user. \b {Android only.}
     * \value ForegroundService
     *        Allows a regular application to use Service.startForeground. \b {Android only.}
     * \value GetAccounts
     *        Allows access to the list of accounts in the Accounts Service. \b {Android only.}
     * \value GetAccountsPrivileged
     *        Allows access to the list of accounts in the Accounts Service. \b {Android only.}
     * \value GetPackageSize
     *        Allows an application to find out the space used by any package. \b {Android only.}
     * \value GetTasks
     *        This constant was deprecated in API level 21. No longer enforced. \b {Android only.}
     * \value GlobalSearch
     *        This permission can be used on content providers to allow the global search system to access their data. \b {Android only.}
     * \value InstallLocationProvider
     *        Allows an application to install a location provider into the Location Manager. \b {Android only.}
     * \value InstallPackages
     *        Allows an application to install packages. \b {Android only.}
     * \value InstallShortcut
     *        Allows an application to install a shortcut in Launcher. \b {Android only.}
     * \value InstantAppForegroundService
     *        Allows an instant app to create foreground services. \b {Android only.}
     * \value Internet
     *        Allows applications to open network sockets. \b {Android only.}
     * \value KillBackgroundProcesses
     *        Allows an application to call ActivityManager.killBackgroundProcesses(String). \b {Android only.}
     * \value LocationHardware
     *        Allows an application to use location features in hardware, such as the geofencing api. \b {Android only.}
     * \value ManageDocuments
     *        Allows an application to manage access to documents, usually as part of a document picker. \b {Android only.}
     * \value ManageOwnCalls
     *        Allows a calling application which manages it own calls through the self-managed ConnectionService APIs. \b {Android only.}
     * \value MediaContentControl
     *        Allows an application to know what content is playing and control its playback. \b {Android only.}
     * \value ModifyAudioSettings
     *        Allows an application to modify global audio settings. \b {Android only.}
     * \value ModifyPhoneState
     *        Allows modification of the telephony state - power on, mmi, etc. \b {Android only.}
     * \value MountFormatFileSystems
     *        Allows formatting file systems for removable storage.
     * \value MountUnmountFileSystems
     *        Allows mounting and unmounting file systems for removable storage. \b {Android only.}
     * \value NFC
     *        Allows applications to perform I/O operations over NFC. \b {Android only.}
     * \value NFCTransactionEvent
     *        Allows applications to receive NFC transaction events.
     * \value PackageUsageStats
     *        Allows an application to collect component usage statistics. Declaring the permission implies intention to
     *        use the API and the user of the device can grant permission through the Settings application. \b {Android only.}
     * \value ProcessOutgoingCalls
     *        Allows an application to see the number being dialed during an outgoing call with the option to redirect
     *        the call to a different number or abort the call altogether. \b {Android only.}
     * \value Calendar
     *        Allows an application to read the user's calendar data. \b {Android and iOS.}
     * \value ReadCallLog
     *        Allows an application to read the user's call log. \b {Android only.}
     * \value Contacts
     *        Allows an application to read the user's contacts data. \b {Android and iOS.}
     * \value ReadExternalStorage
     *        Allows an application to read from external storage. \b {Android only.}
     * \value ReadFrameBuffer
     *        Allows an application to take screen shots and more generally get access to the frame buffer data. \b {Android only.}
     * \value ReadLogs
     *        Allows an application to read the low-level system log files. \b {Android only.}
     * \value ReadPhoneNumbers
     *        Allows read access to the device's phone number(s). \b {Android only.}
     * \value ReadPhoneState
     *        Allows read only access to phone state, including the phone number of the device, current cellular network
     *        information, the status of any ongoing calls, and a list of any PhoneAccounts registered on the device. \b {Android only.}
     * \value ReadSMS
     *        Allows an application to read SMS messages. \b {Android only.}
     * \value ReadSyncSettings
     *        Allows applications to read the sync settings. \b {Android only.}
     * \value ReadSyncStats
     *        Allows applications to read the sync stats. \b {Android only.}
     * \value ReadVoicemail
     *        Allows an application to read voicemails in the system. \b {Android only.}
     * \value Reboot
     *        Required to be able to reboot the device. \b {Android only.}
     * \value ReceiveBootCompleted
     *        Allows an application to receive the Intent.ACTION_BOOT_COMPLETED that is broadcast after the system
     *        finishes booting. \b {Android only.}
     * \value ReceiveMMS
     *        Allows an application to monitor incoming MMS messages. \b {Android only.}
     * \value ReceiveSMS
     *        Allows an application to receive SMS messages. \b {Android only.}
     * \value ReceiveWAPPush
     *        Allows an application to receive WAP push messages. \b {Android only.}
     * \value Microphone
     *        Allows an application to record audio. \b {Android and iOS.}
     * \value ReorderTasks
     *        Allows an application to change the Z-order of tasks. \b {Android only.}
     * \value RequestCompanionRunInBackground
     *        Allows a companion app to run in the background. \b {Android only.}
     * \value RequestCompanionUseDataInBackground
     *        Allows a companion app to use data in the background. \b {Android only.}
     * \value RequestDeletePackages
     *        Allows an application to request deleting packages. \b {Android only.}
     * \value RequestIgnoreBatteryOptimization
     *        Permission an application must hold in order to use Settings.ACTION_REQUEST_IGNORE_BATTERY_OPTIMIZATIONS. \b {Android only.}
     * \value RequestInstallPackages
     *        Allows an application to request installing packages. \b {Android only.}
     * \value SendRespondViaMessage
     *        Allows an application (Phone) to send a request to other applications to handle the respond-via-message
     *        action during incoming calls. \b {Android only.}
     * \value SendSMS
     *        Allows an application to send SMS messages. \b {Android only.}
     * \value SetAlarm
     *        Allows an application to broadcast an Intent to set an alarm for the user. \b {Android only.}
     * \value SetAlwaysFinish
     *        Allows an application to control whether activities are immediately finished when put in the background. \b {Android only.}
     * \value SetAnimationScale
     *        Modify the global animation scaling factor. \b {Android only.}
     * \value SetDebugApp
     *        Configure an application for debugging. \b {Android only.}
     * \value SetProcessLimit
     *        Allows an application to set the maximum number of (not needed) application processes that can be running. \b {Android only.}
     * \value SetTime
     *        Allows applications to set the system time. \b {Android only.}
     * \value SetTimeZone
     *        Allows applications to set the system time zone. \b {Android only.}
     * \value SetWallpaper
     *        Allows applications to set the wallpaper. \b {Android only.}
     * \value SetWallpaperHints
     *        Allows applications to set the wallpaper hints. \b {Android only.}
     * \value SignalPersistentProcesses
     *        Allow an application to request that a signal be sent to all persistent processes. \b {Android only.}
     * \value StatusBar
     *        Allows an application to open, close, or disable the status bar and its icons. \b {Android only.}
     * \value SystemAlertWindow
     *        Allows an app to create windows using the type WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY, shown
     *        on top of all other apps. \b {Android only.}
     * \value TransmitIR
     *        Allows using the device's IR transmitter, if available. \b {Android only.}
     * \value UninstallShortcut
     *        This permission is no longer supported. \b {Android only.}
     * \value UpdateDeviceStats
     *        Allows an application to update device statistics. \b {Android only.}
     * \value UseBiometric
     *        Allows an app to use device supported biometric modalities. \b {Android only.}
     * \value UseFingerprint
     *        This constant was deprecated in API level 28. Applications should request USE_BIOMETRIC instead. \b {Android only.}
     * \value UseSIP
     *        Allows an application to use SIP service. \b {Android only.}
     * \value Vibrate
     *        Allows access to the vibrator. \b {Android only.}
     * \value WakeLock
     *        Allows using PowerManager WakeLocks to keep processor from sleeping or screen from dimming. \b {Android only.}
     * \value WriteAPNSettings
     *        Allows applications to write the apn settings. \b {Android only.}
     * \value WriteCalendar
     *        Allows an application to write the user's calendar data. \b {Android only.}
     * \value WriteCallLog
     *        Allows an application to write (but not read) the user's call log data. \b {Android only.}
     * \value WriteContacts
     *        Allows an application to write the user's contacts data. \b {Android only.}
     * \value WriteExternalStorage
     *        Allows an application to write to external storage. \b {Android only.}
     * \value WriteGServices
     *        Allows an application to modify the Google service map. \b {Android only.}
     * \value WriteSecureSettings
     *        Allows an application to read or write the secure system settings. \b {Android only.}
     * \value WriteSettings
     *        Allows an application to read or write the system settings. \b {Android only.}
     * \value WriteSyncSettings
     *        Allows applications to write the sync settings. \b {Android only.}
     * \value WriteVoiceMail
     *        Allows an application to modify and remove existing voicemails in the system. \b {Android only.}
     * \value Reminders
     *        Allows access to reminders. \b {iOS only.}
     * \value Photos
     *        Allows access to photo gallery. \b {iOS only.}
     * \value MotionActivityAndFitness
     *        Allows access to user's motion activity and fitness data. \b {iOS only.}
     * \value LocationServices
     *        Allows access to user's location. User can choose to allow location only when the app is open or also when
     *        in background. \b {iOS only.}
     * \value AppleMusic
     *        Allows you to add an Apple Music track to the music library and play it. \b {iOS only.}
     * \value MusicVideoActivity
     *        Allows you to access music and video activity of the user. \b {iOS only.}
     * \value Twitter
     *        Allows access to user's Twitter account on the device. \b {iOS only.}
     * \value Facebook
     *        Allows access to user's Facebook account on the device. \b {iOS only.}
     * \value HomeKit
     *        Allows access to HomeKit. \b {iOS only.}
     * \value Health
     *        Allows access to health data. \b {iOS only.}
     * \value SpeechRecognition
     *        Allows access to speech recognition. \b {iOS only.}
     * \value BluetoothSharing
     *        Allows access to bluetooth sharing. \b {iOS only.}
     * \value MediaLibrary
     *        Allows access to media library. \b {iOS only.}
     */
    enum Permissions {
        None = -1,
        AcceptHandover = 0,
        AccessCheckInProperties = 1,
        AccessCoarseLocation = 2,
        AccessFineLocation = 3,
        AccessLocationExtraCommands = 4,
        AccessNetworkState = 5,
        AccessNotificationPolicy = 6,
        AccessWiFiState = 7,
        AccountManager = 8,
        AddVoiceMail = 9,
        AnswerPhoneCalls = 10,
        BatteryStats = 11,
        BindAccessibilityService = 12,
        BindAppWidget = 13,
        BindAutofillService = 14,
        BindCarrierMessagingService = 15,
        BindCarrierServices = 16,
        BindChooserTargetService = 17,
        BindConditionProviderService = 18,
        BindDeviceAdmin = 19,
        BindDreamService = 20,
        BindInCallService = 21,
        BindInputMethod = 22,
        BindMidiDeviceService = 23,
        BindNFCService = 24,
        BindNotificationListenerService = 25,
        BindPrintService = 26,
        BindQuickSettingsTile = 27,
        BindRemoteViews = 28,
        BindScreeningService = 29,
        BindTelecomConnectionService = 30,
        BindTextService = 31,
        BindTVInput = 32,
        BindVisualVoiceMailService = 33,
        BindVoiceInteraction = 34,
        BindVPNService = 35,
        BindVRListenerService = 36,
        BindWallpaper = 37,
        Bluetooth = 38,
        BluetoothAdmin = 39,
        BluetoothPrivileged = 40,
        BodySensors = 41,
        BroadcastPackageRemoved = 42,
        BroadcastSMS = 43,
        BroadcastSticky = 44,
        BroadcastWAPPush = 45,
        CallPhone = 46,
        CallPrivileged = 47,
        Camera = 48,
        CaptureAudioOutput = 49,
        CaptureSecureVideoOutput = 50,
        CaptureVideoOutput = 51,
        ChangeComponentEnabledState = 52,
        ChangeConfiguration = 53,
        ChangeNetworkState = 54,
        ChangeWiFiMulticastState = 55,
        ChangeWiFiState = 56,
        ClearAppCache = 57,
        ControlLocationUpdates = 58,
        DeleteCacheFiles = 59,
        DeletePackages = 60,
        Diagnostic = 61,
        DisableKeyguard = 62,
        Dump = 63,
        ExpandStatusBar = 64,
        FactoryTest = 65,
        ForegroundService = 66,
        GetAccounts = 67,
        GetAccountsPrivileged = 68,
        GetPackageSize = 69,
        GetTasks = 70,
        GlobalSearch = 71,
        InstallLocationProvider = 72,
        InstallPackages = 73,
        InstallShortcut = 74,
        InstantAppForegroundService = 75,
        Internet = 76,
        KillBackgroundProcesses = 77,
        LocationHardware = 78,
        ManageDocuments = 79,
        ManageOwnCalls = 80,
        MediaContentControl = 81,
        ModifyAudioSettings = 82,
        ModifyPhoneState = 83,
        MountFormatFileSystems = 84,
        MountUnmountFileSystems = 85,
        NFC = 86,
        NFCTransactionEvent = 87,
        PackageUsageStats = 88,
        ProcessOutgoingCalls = 89,
        Calendar = 90,
        ReadCallLog = 91,
        Contacts = 92,
        ReadExternalStorage = 93,
        ReadFrameBuffer = 94,
        ReadLogs = 95,
        ReadPhoneNumbers = 96,
        ReadPhoneState = 97,
        ReadSMS = 98,
        ReadSyncSettings = 99,
        ReadSyncStats = 100,
        ReadVoicemail = 101,
        Reboot = 102,
        ReceiveBootCompleted = 103,
        ReceiveMMS = 104,
        ReceiveSMS = 105,
        ReceiveWAPPush = 106,
        Microphone = 107,
        ReorderTasks = 108,
        RequestCompanionRunInBackground = 109,
        RequestCompanionUseDataInBackground = 110,
        RequestDeletePackages = 111,
        RequestIgnoreBatteryOptimization = 112,
        RequestInstallPackages = 113,
        SendRespondViaMessage = 114,
        SendSMS = 115,
        SetAlarm = 116,
        SetAlwaysFinish = 117,
        SetAnimationScale = 118,
        SetDebugApp = 119,
        SetProcessLimit = 120,
        SetTime = 121,
        SetTimeZone = 122,
        SetWallpaper = 123,
        SetWallpaperHints = 124,
        SignalPersistentProcesses = 125,
        StatusBar = 126,
        SystemAlertWindow = 127,
        TransmitIR = 128,
        UninstallShortcut = 129,
        UpdateDeviceStats = 130,
        UseBiometric = 131,
        UseFingerprint = 132,
        UseSIP = 133,
        Vibrate = 134,
        WakeLock = 135,
        WriteAPNSettings = 136,
        WriteCalendar = 137,
        WriteCallLog = 138,
        WriteContacts = 139,
        WriteExternalStorage = 140,
        WriteGServices = 141,
        WriteSecureSettings = 142,
        WriteSettings = 143,
        WriteSyncSettings = 144,
        WriteVoiceMail = 145,
        Reminders = 146,
        Photos = 147,
        MotionActivityAndFitness = 148,
        LocationServices = 149,
        AppleMusic = 150,
        MusicVideoActivity = 151,
        Twitter = 152,
        Facebook = 153,
        HomeKit = 154,
        Health = 155,
        SpeechRecognition = 156,
        BluetoothSharing = 157,
        MediaLibrary = 158
    };
    Q_ENUM(Permissions);

public:
    explicit PermissionManager(QObject *parent = nullptr);

    /*!
     * \brief Returns the permission result for the given permission.
     * \param permission
     *
     * Note that If you try to check for a permission that the OS does not have, you'll always get
     * Result::Denied.
     *
     * This method is invokable from QML.
     *
     * \return Result
     */
    Q_INVOKABLE Result checkPermission(Permissions permission) const;

    /*!
     * \brief Requests the given permission asynchronously.
     * \param permission
     *
     * When the user responds to the reuest, permissionResultReceived() signal will be emitted
     * .
     * This method is invokable from QML.
     */
    Q_INVOKABLE void requestPermission(int permission);

    /*!
     * \brief Requests the given permissions asynchronously.
     * \param permissions
     *
     * When the user responds to the reuest, permissionResultsReceived() signal will be emitted
     * .
     * This method is invokable from QML.
     */
    Q_INVOKABLE void requestPermissions(QList<int> permissions);

    /*!
     * \brief Returns the string represnetation for the given permission.
     * \param permission
     *
     * If a string version cannot be found, returns an empty string.
     *
     * \note This method is only for Android permissions.
     * \return QString
     */
    QString permissionString(Permissions permission) const;

    /*!
     * \brief Returns a Permissions enum for the given type.
     * \param typeStr
     * \return Permissions
     *
     * \note This method is only for Android permissions.
     */
    Permissions permissionType(const QString &permissionStr) const;

signals:

    /*!
     * \brief Emitted when multiple permissions were requested.
     * \param results
     *
     * The key of the map is the permission type and the value is the permission result.
     * \a results is a QList<PermissionRequestResult *>
     *
     * After the signal is emitted, all of the result objects will be destroyed.
     *
     * \note This signal is not emitted o iOS.
     */
    void permissionResultsReceived(QList<QObject *> results);

    /*!
     * \brief Emitted when a single permission was requested.
     * \param result
     * \param permission
     */
    void permissionResultReceived(int result, int permission);

private:
#if defined(Q_OS_IOS)
    PermissionManagerPrivate m_Private;
#endif // Q_OS_IOS

private:
#if defined(Q_OS_ANDROID)
    /*!
     * \brief Callback for QtAndroid::requestPermissions().
     * \param results
     */
    void permissionResultCallback(const QHash<QString, QtAndroid::PermissionResult> &results);
#endif // Q_OS_ANDROID

#if defined(Q_OS_IOS)
    /*!
     * \brief Converts PermissionManagerPrivate::AuthorizationStatus to PermissionManager::Result.
     * \param status
     * \return PermissionManager::Result
     */
    Result getResultFromAuthStatus(PermissionManagerPrivate::AuthorizationStatus status) const;

    /*!
     * \brief Callback for PermissionManagerPrivate.
     * \param permission
     * \param status
     */
    void permissionResultCallback(Permissions permission, PermissionManagerPrivate::AuthorizationStatus status);
#endif // Q_OS_IOS
};

}