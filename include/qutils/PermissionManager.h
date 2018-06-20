#pragma once
// Qt
#include <QObject>

namespace zmc
{

/*!
 * \class PermissionManager
 * \brief The PermissionManager class is a cross-platform implementation of checking for and requesting permissions.
 */
class PermissionManager : public QObject
{
    Q_OBJECT

public:
    /*!
     * \enum PermissionResult
     * \brief The PermissionResult enum provides information about the permission result check.
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
     */
    enum PermissionResult {
        Denied = 0,
        Granted = 1,
        Restricted = 2,
        NotDetermined = 3
    };
    Q_ENUM(PermissionResult);

    /*!
     * \enum Permissions
     * \brief The Permissions enum contains the permissions for all platforms.
     *
     * Unless otherwise specified, all the permissions are cross-platform.
     *
     * For a complete overview of Android permissions, see \l {https://developer.android.com/reference/android/Manifest.permission}
     * {here}.
     *
     * \value AcceptHandover
     *        Allows a calling app to continue a call which was started in another app.
     * \value AccessCheckInProperties
     *        Allows read/write access to the "properties" table in the checkin database, to change values that get
     *        uploaded.
     * \value AccessCoarseocation
     *        Allows an app to access approximate location.
     * \value AccessFineLocation
     *        Allows an app to access precise location.
     * \value AccessLocationExtraCommands
     *        Allows an application to access extra location provider commands.
     * \value AccessNetworkState
     *        Allows applications to access information about networks.
     * \value AccessNotificationPolicy
     *        Marker permission for applications that wish to access notification policy.
     * \value AccessWiFiState
     *        Allows applications to access information about Wi-Fi networks.
     * \value AccountManager
     *        Allows applications to call into AccountAuthenticators. \b {Android only.}
     * \value AddVoiceMail
     *        Allows an application to add voicemails into the system.
     * \value AnswerPhoneCalls
     *        Allows the app to answer an incoming phone call.
     * \value BatteryStats
     *        Allows an application to collect battery statistics.
     * \value BindAccessibilityService
     *        Must be required by an AccessibilityService, to ensure that only the system can bind to it.
     * \value BindAppWidget
     *        Allows an application to tell the AppWidget service which application can access AppWidget's data.
     * \value BindAutofillService
     *        Must be required by a AutofillService, to ensure that only the system can bind to it.
     * \value BindCarrierMessagingService
     *        This constant was deprecated in API level 23. \b {Use BindCarrierServices instead}.
     * \value BindCarrierServices
     *        The system process that is allowed to bind to services in carrier apps will have this permission.
     * \value BindChooserTargetService
     *        Must be required by a ChooserTargetService, to ensure that only the system can bind to it.
     * \value BindConditionProviderService
     *        Must be required by a ConditionProviderService, to ensure that only the system can bind to it.
     * \value BindDeviceAdmin
     *        Must be required by device administration receiver, to ensure that only the system can interact with it.
     * \value BindDreamService
     *        Must be required by an DreamService, to ensure that only the system can bind to it.
     * \value BindInCallService
     *        Must be required by a InCallService, to ensure that only the system can bind to it.
     * \value BindInputMethod
     *        Must be required by an InputMethodService, to ensure that only the system can bind to it.
     * \value BindMidiDeviceService
     *        Must be required by an MidiDeviceService, to ensure that only the system can bind to it.
     * \value BindNFCService
     *        Must be required by a HostApduService or OffHostApduService to ensure that only the system can bind to it.
     * \value BindNotificationListenerService
     *        Must be required by an NotificationListenerService, to ensure that only the system can bind to it.
     * \value BindPrintService
     *        Must be required by a PrintService, to ensure that only the system can bind to it.
     * \value BindQuickSettingsTile
     *        Allows an application to bind to third party quick settings tiles.
     * \value BindRemoteViews
     *        Must be required by a RemoteViewsService, to ensure that only the system can bind to it.
     * \value BindScreeningService
     *        Must be required by a CallScreeningService, to ensure that only the system can bind to it.
     * \value BindTelecomConnectionService
     *        Must be required by a ConnectionService, to ensure that only the system can bind to it.
     * \value BindTextServices
     *        Must be required by a TextService (e.g. SpellCheckerService) to ensure that only the system can bind to it.
     * \value BindTVInput
     *        Must be required by a TvInputService to ensure that only the system can bind to it.
     * \value BindVisualVoiceMailService
     *        Must be required by a link VisualVoicemailService to ensure that only the system can bind to it.
     * \value BindVoiceInteraction
     *        Must be required by a VoiceInteractionService, to ensure that only the system can bind to it.
     * \value BindVPNService
     *        Must be required by a VpnService, to ensure that only the system can bind to it.
     * \value BindVRListenerService
     *        Must be required by an VrListenerService, to ensure that only the system can bind to it.
     * \value BindWallpaper
     *        Must be required by a WallpaperService, to ensure that only the system can bind to it.
     * \value Bluetooth
     *        Allows applications to connect to paired bluetooth devices.
     * \value BluetoothAdmin
     *        Allows applications to discover and pair bluetooth devices.
     * \value BluetoothPriviliged
     *        Allows applications to pair bluetooth devices without user interaction, and to allow or disallow phonebook
     *        access or message access.
     * \value BodySensors
     *        Allows an application to access data from sensors that the user uses to measure what is happening inside
     *        his/her body, such as heart rate.
     * \value BroadcastPackageRemoved
     *        Allows an application to broadcast a notification that an application package has been removed.
     * \value BroadcastSMS
     *        Allows an application to broadcast an SMS receipt notification.
     * \value BroadcastSticky
     *        Allows an application to broadcast sticky intents.
     * \value BroadcastWAPPush
     *        Allows an application to broadcast a WAP PUSH receipt notification.
     * \value CallPhone
     *        Allows an application to initiate a phone call without going through the Dialer user interface for the user to confirm the call.
     * \value CallPriviliged
     *        Allows an application to call any phone number, including emergency numbers, without going through the
     *        Dialer user interface for the user to confirm the call being placed.
     * \value Camera
     *        Required to be able to access the camera device.
     * \value CaptureAudioOutput
     *        Allows an application to capture audio output.
     * \value CaptureSecureVideoOutput
     *        Allows an application to capture secure video output.
     * \value CaptureVideoOutput
     *        Allows an application to capture video output.
     * \value ChangeComponentEnabledState
     *        Allows an application to change whether an application component (other than its own) is enabled or not.
     * \value ChangeConfiguration
     *        Allows an application to modify the current configuration, such as locale.
     * \value ChangeNetworkState
     *        Allows applications to change network connectivity state.
     * \value ChangeWiFiMulticastState
     *        Allows applications to enter Wi-Fi Multicast mode.
     * \value ChangeWiFiState
     *        Allows applications to change Wi-Fi connectivity state.
     * \value ClearAppCache
     *        Allows an application to clear the caches of all installed applications on the device.
     * \value ControlLocationUpdates
     *        Allows enabling/disabling location update notifications from the radio.
     * \value DeleteCacheFiles
     *        Old permission for deleting an app's cache files, no longer used, but signals for us to quietly ignore calls instead of throwing an exception.
     * \value DeletePackages
     *        Allows an application to delete packages.
     * \value Diagnostic
     *        Allows applications to RW to diagnostic resources.
     * \value DisableKeyguard
     *        Allows applications to disable the keyguard if it is not secure.
     * \value Dump
     *        Allows an application to retrieve state dump information from system services.
     * \value ExpandStatusBar
     *        Allows an application to expand or collapse the status bar.
     * \value FactoryReset
     *        Run as a manufacturer test application, running as the root user.
     * \value ForegroundService
     *        Allows a regular application to use Service.startForeground.
     * \value GetAccounts
     *        Allows access to the list of accounts in the Accounts Service.
     * \value GetAccountsPriviliged
     *        Allows access to the list of accounts in the Accounts Service.
     * \value GetPackageSize
     *        Allows an application to find out the space used by any package.
     * \value GetTasks
     *        This constant was deprecated in API level 21. No longer enforced.
     * \value GlobalSearch
     *        This permission can be used on content providers to allow the global search system to access their data.
     * \value InstallLocationProvider
     *        Allows an application to install a location provider into the Location Manager.
     * \value InstallPackages
     *        Allows an application to install packages.
     * \value InstallShortcut
     *        Allows an application to install a shortcut in Launcher.
     * \value InstantAppForegroundService
     *        Allows an instant app to create foreground services.
     * \value Internet
     *        Allows applications to open network sockets.
     * \value KillBackgroundProcesses
     *        Allows an application to call ActivityManager.killBackgroundProcesses(String).
     * \value LocationHardware
     *        Allows an application to use location features in hardware, such as the geofencing api.
     * \value ManageDocuments
     *        Allows an application to manage access to documents, usually as part of a document picker.
     * \value ManageOwnCalls
     *        Allows a calling application which manages it own calls through the self-managed ConnectionService APIs.
     * \value MediaContentControl
     *        Allows an application to know what content is playing and control its playback.
     * \value ModifyAudioSettings
     *        Allows an application to modify global audio settings.
     * \value ModifyPhoneState
     *        Allows modification of the telephony state - power on, mmi, etc.
     * \value MountFormatFileSystems
     *        Allows formatting file systems for removable storage.
     * \value MountUnmountFileSystems
     *        Allows mounting and unmounting file systems for removable storage.
     * \value NFC
     *        Allows applications to perform I/O operations over NFC.
     * \value NFCTransactionEvent
     *        Allows applications to receive NFC transaction events.
     * \value PackageUsageStats
     *        Allows an application to collect component usage statistics. Declaring the permission implies intention to
     *        use the API and the user of the device can grant permission through the Settings application.
     * \value ProcessOutgoingEvents
     *        Allows an application to see the number being dialed during an outgoing call with the option to redirect
     *        the call to a different number or abort the call altogether.
     * \value ReadCalendar
     *        Allows an application to read the user's calendar data.
     * \value ReadCallLog
     *        Allows an application to read the user's call log.
     * \value ReadContacts
     *        Allows an application to read the user's contacts data.
     * \value ReadExternalStorage
     *        Allows an application to read from external storage.
     * \value ReadFrameBuffer
     *        Allows an application to take screen shots and more generally get access to the frame buffer data.
     * \value ReadLogs
     *        Allows an application to read the low-level system log files.
     * \value ReadPhoneNumbers
     *        Allows read access to the device's phone number(s).
     * \value ReadPhoneState
     *        Allows read only access to phone state, including the phone number of the device, current cellular network
     *        information, the status of any ongoing calls, and a list of any PhoneAccounts registered on the device.
     * \value ReadSMS
     *        Allows an application to read SMS messages.
     * \value ReadSyncSettings
     *        Allows applications to read the sync settings.
     * \value ReadSyncStats
     *        Allows applications to read the sync stats.
     * \value ReadVoicemail
     *        Allows an application to read voicemails in the system.
     * \value Reboot
     *        Required to be able to reboot the device.
     * \value ReceiveBootCompleted
     *        Allows an application to receive the Intent.ACTION_BOOT_COMPLETED that is broadcast after the system
     *        finishes booting.
     * \value ReceiveMMS
     *        Allows an application to monitor incoming MMS messages.
     * \value ReceiveSMS
     *        Allows an application to receive SMS messages.
     * \value ReceiveWAPPush
     *        Allows an application to receive WAP push messages.
     * \value RecordAudio
     *        Allows an application to record audio.
     * \value ReorderTasks
     *        Allows an application to change the Z-order of tasks.
     * \value RequestCompanionRunInBackground
     *        Allows a companion app to run in the background.
     * \value RequestCompanionUseDataInBackground
     *        Allows a companion app to use data in the background.
     * \value RequestDeletePackages
     *        Allows an application to request deleting packages.
     * \value RequestIgnoreBatteryOptimization
     *        Permission an application must hold in order to use Settings.ACTION_REQUEST_IGNORE_BATTERY_OPTIMIZATIONS.
     * \value RequestInstallPackages
     *        Allows an application to request installing packages.
     * \value SendRespondViaMessage
     *        Allows an application (Phone) to send a request to other applications to handle the respond-via-message
     *        action during incoming calls.
     * \value SendSMS
     *        Allows an application to send SMS messages.
     * \value SetAlarm
     *        Allows an application to broadcast an Intent to set an alarm for the user.
     * \value SetAlwaysFinish
     *        Allows an application to control whether activities are immediately finished when put in the background.
     * \value SetAnimationScale
     *        Modify the global animation scaling factor.
     * \value SetDebugApp
     *        Configure an application for debugging.
     * \value SetProcessLimit
     *        Allows an application to set the maximum number of (not needed) application processes that can be running.
     * \value SetTime
     *        Allows applications to set the system time.
     * \value SetTimeZone
     *        Allows applications to set the system time zone.
     * \value SetWallpaper
     *        Allows applications to set the wallpaper.
     * \value SetWallpaperHints
     *        Allows applications to set the wallpaper hints.
     * \value SignalPersistentProcesses
     *        Allow an application to request that a signal be sent to all persistent processes.
     * \value StatusBar
     *        Allows an application to open, close, or disable the status bar and its icons.
     * \value SystemAlertWindow
     *        Allows an app to create windows using the type WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY, shown
     *        on top of all other apps.
     * \value TransmitIR
     *        Allows using the device's IR transmitter, if available.
     * \value UninstallShortcut
     *        This permission is no longer supported.
     * \value UpdateDeviceStats
     *        Allows an application to update device statistics.
     * \value UseBiometric
     *        Allows an app to use device supported biometric modalities.
     * \value UseFingerprint
     *        This constant was deprecated in API level 28. Applications should request USE_BIOMETRIC instead
     * \value UseSIP
     *        Allows an application to use SIP service.
     * \value Vibrate
     *        Allows access to the vibrator.
     * \value WakeLock
     *        Allows using PowerManager WakeLocks to keep processor from sleeping or screen from dimming.
     * \value WriteAPNSettings
     *        Allows applications to write the apn settings.
     * \value WriteCalendar
     *        Allows an application to write the user's calendar data.
     * \value WriteCallLog
     *        Allows an application to write (but not read) the user's call log data.
     * \value WriteContacts
     *        Allows an application to write the user's contacts data.
     * \value WriteExternalStorage
     *        Allows an application to write to external storage.
     * \value WriteGServices
     *        Allows an application to modify the Google service map.
     * \value WriteSecureSettings
     *        Allows an application to read or write the secure system settings.
     * \value WriteSettings
     *        Allows an application to read or write the system settings.
     * \value WriteSyncSettings
     *        Allows applications to write the sync settings.
     * \value WriteVoiceMail
     *        Allows an application to modify and remove existing voicemails in the system.
     */
    enum Permissions {
        AcceptHandover = 0,
        AccessCheckInProperties = 1,
        AccessCoarseocation = 2,
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
        BindTextServices = 31,
        BindTVInput = 32,
        BindVisualVoiceMailService = 33,
        BindVoiceInteraction = 34,
        BindVPNService = 35,
        BindVRListenerService = 36,
        BindWallpaper = 37,
        Bluetooth = 38,
        BluetoothAdmin = 39,
        BluetoothPriviliged = 40,
        BodySensors = 41,
        BroadcastPackageRemoved = 42,
        BroadcastSMS = 43,
        BroadcastSticky = 44,
        BroadcastWAPPush = 45,
        CallPhone = 46,
        CallPriviliged = 47,
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
        FactoryReset = 65,
        ForegroundService = 66,
        GetAccounts = 67,
        GetAccountsPriviliged = 68,
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
        ProcessOutgoingEvents = 89,
        ReadCalendar = 90,
        ReadCallLog = 91,
        ReadContacts = 92,
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
        RecordAudio = 107,
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
        WriteVoiceMail = 145
    };
    Q_ENUM(Permissions);

public:
    explicit PermissionManager(QObject *parent = nullptr);

signals:

public slots:
};

}
