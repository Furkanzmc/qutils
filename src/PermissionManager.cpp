#include "qutils/PermissionManager.h"
// qutils
#include "qutils/Macros.h"

namespace zmc
{

PermissionManager::PermissionManager(QObject *parent)
    : QObject(parent)
#if defined(Q_OS_IOS)
    , m_Private()
#endif // Q_OS_IOS
{

}

PermissionManager::Result PermissionManager::checkPermission(Permissions permission) const
{
    Result result = Result::Denied;
    // Permission checks on Android is done with strings.
#if defined(Q_OS_ANDROID)
    const QString permissionStr = permissionString(permission);
    if (!permissionStr.isEmpty()) {
        QtAndroid::PermissionResult qresult = QtAndroid::checkPermission(permissionStr);
        if (qresult == QtAndroid::PermissionResult::Granted) {
            result = Result::Granted;
        }
    }
    else {
        LOG_ERROR("Given permission is not an Android permission.");
    }
#elif defined(Q_OS_IOS)
    if (permission == Permissions::Photos) {
        PermissionManagerPrivate::AuthorizationStatus authStatus = m_Private.getPhotosAuthorizationStatus();
        result = getResultFromAuthStatus(authStatus);
    }
    else if (permission == Permissions::LocationServices) {
        PermissionManagerPrivate::AuthorizationStatus authStatus = m_Private.getLocationAuthorizationStatus();
        result = getResultFromAuthStatus(authStatus);
    }
    else if (permission == Permissions::Camera) {
        PermissionManagerPrivate::AuthorizationStatus authStatus = m_Private.getCameraAccessAuthStatus();
        result = getResultFromAuthStatus(authStatus);
    }
    else {
        LOG_ERROR("This permission type is not yet supported on iOS.");
    }
#else
    Q_UNUSED(permission);
#endif // Platform Check

    return result;
}

void PermissionManager::requestPermission(int permission)
{
#if defined(Q_OS_ANDROID)
    requestPermissions(QList<int> {permission});
#elif defined(Q_OS_IOS)
    if (permission == Permissions::Photos) {
        if (!m_Private.onPhotosAccessResult) {
            m_Private.onPhotosAccessResult = std::bind(&PermissionManager::permissionResultCallback, this, Permissions::Photos, std::placeholders::_1);
        }

        m_Private.requestPhotosPermisson();
    }
    else if (permission == Permissions::LocationServices) {
        if (!m_Private.onLocationServicesResult) {
            m_Private.onLocationServicesResult = std::bind(&PermissionManager::permissionResultCallback, this, Permissions::LocationServices, std::placeholders::_1);
        }

        m_Private.requestLocationPermission();
    }
    else if (permission == Permissions::Camera) {
        if (!m_Private.onCameraPermissionResult) {
            m_Private.onCameraPermissionResult = std::bind(&PermissionManager::permissionResultCallback, this, Permissions::Camera, std::placeholders::_1);
        }

        m_Private.requestCameraAccess();
    }
    else {
        LOG_ERROR("This permission type is not yet supported on iOS.");
    }
#else
    Q_UNUSED(permission);
#endif // Platform Check
}

void PermissionManager::requestPermissions(QList<int> permissions)
{
#if defined(Q_OS_ANDROID)
    QStringList names;
    for (int value : permissions) {
        const QString str = permissionString(static_cast<Permissions>(value));
        if (!str.isEmpty()) {
            names.append(str);
        }
    }

    if (names.isEmpty()) {
        LOG_ERROR("Given permissions is not valid for Android. Given value: " << permissions);
        return;
    }

    QtAndroid::requestPermissions(names, std::bind(&PermissionManager::permissionResultCallback, this, std::placeholders::_1));
#elif defined(Q_OS_IOS)
    Q_UNUSED(permissions);
    LOG_ERROR("This method is not available on iOS.");
#else
    Q_UNUSED(permissions);
#endif // Platform Check
}

QString PermissionManager::permissionString(Permissions permission) const
{
    QString typeStr = "";
    if (permission == Permissions::AcceptHandover) {
        typeStr = AndroidPermNames::ACCEPT_HANDOVER;
    }
    else if (permission == Permissions::AccessCheckInProperties) {
        typeStr = AndroidPermNames::ACCESS_CHECKIN_PROPERTIES;
    }
    else if (permission == Permissions::AccessCoarseLocation) {
        typeStr = AndroidPermNames::ACCESS_COARSE_LOCATION;
    }
    else if (permission == Permissions::AccessFineLocation) {
        typeStr = AndroidPermNames::ACCESS_FINE_LOCATION;
    }
    else if (permission == Permissions::AccessLocationExtraCommands) {
        typeStr = AndroidPermNames::ACCESS_LOCATION_EXTRA_COMMANDS;
    }
    else if (permission == Permissions::AccessNetworkState) {
        typeStr = AndroidPermNames::ACCESS_NETWORK_STATE;
    }
    else if (permission == Permissions::AccessNotificationPolicy) {
        typeStr = AndroidPermNames::ACCESS_NOTIFICATION_POLICY;
    }
    else if (permission == Permissions::AccessWiFiState) {
        typeStr = AndroidPermNames::ACCESS_WIFI_STATE;
    }
    else if (permission == Permissions::AccountManager) {
        typeStr = AndroidPermNames::ACCOUNT_MANAGER;
    }
    else if (permission == Permissions::AddVoiceMail) {
        typeStr = AndroidPermNames::ADD_VOICEMAIL;
    }
    else if (permission == Permissions::AnswerPhoneCalls) {
        typeStr = AndroidPermNames::ANSWER_PHONE_CALLS;
    }
    else if (permission == Permissions::BatteryStats) {
        typeStr = AndroidPermNames::BATTERY_STATS;
    }
    else if (permission == Permissions::BindAccessibilityService) {
        typeStr = AndroidPermNames::BIND_ACCESSIBILITY_SERVICE;
    }
    else if (permission == Permissions::BindAppWidget) {
        typeStr = AndroidPermNames::BIND_APPWIDGET;
    }
    else if (permission == Permissions::BindAutofillService) {
        typeStr = AndroidPermNames::BIND_AUTOFILL_SERVICE;
    }
    else if (permission == Permissions::BindCarrierMessagingService) {
        typeStr = AndroidPermNames::BIND_CARRIER_MESSAGING_SERVICE;
    }
    else if (permission == Permissions::BindCarrierServices) {
        typeStr = AndroidPermNames::BIND_CARRIER_SERVICES;
    }
    else if (permission == Permissions::BindChooserTargetService) {
        typeStr = AndroidPermNames::BIND_CHOOSER_TARGET_SERVICE;
    }
    else if (permission == Permissions::BindConditionProviderService) {
        typeStr = AndroidPermNames::BIND_CONDITION_PROVIDER_SERVICE;
    }
    else if (permission == Permissions::BindDeviceAdmin) {
        typeStr = AndroidPermNames::BIND_DEVICE_ADMIN;
    }
    else if (permission == Permissions::BindDreamService) {
        typeStr = AndroidPermNames::BIND_DREAM_SERVICE;
    }
    else if (permission == Permissions::BindInCallService) {
        typeStr = AndroidPermNames::BIND_INCALL_SERVICE;
    }
    else if (permission == Permissions::BindInputMethod) {
        typeStr = AndroidPermNames::BIND_INPUT_METHOD;
    }
    else if (permission == Permissions::BindMidiDeviceService) {
        typeStr = AndroidPermNames::BIND_MIDI_DEVICE_SERVICE;
    }
    else if (permission == Permissions::BindNFCService) {
        typeStr = AndroidPermNames::BIND_NFC_SERVICE;
    }
    else if (permission == Permissions::BindNotificationListenerService) {
        typeStr = AndroidPermNames::BIND_NOTIFICATION_LISTENER_SERVICE;
    }
    else if (permission == Permissions::BindPrintService) {
        typeStr = AndroidPermNames::BIND_PRINT_SERVICE;
    }
    else if (permission == Permissions::BindQuickSettingsTile) {
        typeStr = AndroidPermNames::BIND_QUICK_SETTINGS_TILE;
    }
    else if (permission == Permissions::BindRemoteViews) {
        typeStr = AndroidPermNames::BIND_REMOTEVIEWS;
    }
    else if (permission == Permissions::BindScreeningService) {
        typeStr = AndroidPermNames::BIND_SCREENING_SERVICE;
    }
    else if (permission == Permissions::BindTelecomConnectionService) {
        typeStr = AndroidPermNames::BIND_TELECOM_CONNECTION_SERVICE;
    }
    else if (permission == Permissions::BindTextService) {
        typeStr = AndroidPermNames::BIND_TEXT_SERVICE;
    }
    else if (permission == Permissions::BindTVInput) {
        typeStr = AndroidPermNames::BIND_TV_INPUT;
    }
    else if (permission == Permissions::BindVisualVoiceMailService) {
        typeStr = AndroidPermNames::BIND_VISUAL_VOICEMAIL_SERVICE;
    }
    else if (permission == Permissions::BindVoiceInteraction) {
        typeStr = AndroidPermNames::BIND_VOICE_INTERACTION;
    }
    else if (permission == Permissions::BindVPNService) {
        typeStr = AndroidPermNames::BIND_VPN_SERVICE;
    }
    else if (permission == Permissions::BindVRListenerService) {
        typeStr = AndroidPermNames::BIND_VR_LISTENER_SERVICE;
    }
    else if (permission == Permissions::BindWallpaper) {
        typeStr = AndroidPermNames::BIND_WALLPAPER;
    }
    else if (permission == Permissions::Bluetooth) {
        typeStr = AndroidPermNames::BLUETOOTH;
    }
    else if (permission == Permissions::BluetoothAdmin) {
        typeStr = AndroidPermNames::BLUETOOTH_ADMIN;
    }
    else if (permission == Permissions::BluetoothPrivileged) {
        typeStr = AndroidPermNames::BLUETOOTH_PRIVILEGED;
    }
    else if (permission == Permissions::BodySensors) {
        typeStr = AndroidPermNames::BODY_SENSORS;
    }
    else if (permission == Permissions::BroadcastPackageRemoved) {
        typeStr = AndroidPermNames::BROADCAST_PACKAGE_REMOVED;
    }
    else if (permission == Permissions::BroadcastSMS) {
        typeStr = AndroidPermNames::BROADCAST_SMS;
    }
    else if (permission == Permissions::BroadcastSticky) {
        typeStr = AndroidPermNames::BROADCAST_STICKY;
    }
    else if (permission == Permissions::BroadcastWAPPush) {
        typeStr = AndroidPermNames::BROADCAST_WAP_PUSH;
    }
    else if (permission == Permissions::CallPhone) {
        typeStr = AndroidPermNames::CALL_PHONE;
    }
    else if (permission == Permissions::CallPrivileged) {
        typeStr = AndroidPermNames::CALL_PRIVILEGED;
    }
    else if (permission == Permissions::Camera) {
        typeStr = AndroidPermNames::CAMERA;
    }
    else if (permission == Permissions::CaptureAudioOutput) {
        typeStr = AndroidPermNames::CAPTURE_AUDIO_OUTPUT;
    }
    else if (permission == Permissions::CaptureSecureVideoOutput) {
        typeStr = AndroidPermNames::CAPTURE_SECURE_VIDEO_OUTPUT;
    }
    else if (permission == Permissions::CaptureVideoOutput) {
        typeStr = AndroidPermNames::CAPTURE_VIDEO_OUTPUT;
    }
    else if (permission == Permissions::ChangeComponentEnabledState) {
        typeStr = AndroidPermNames::CHANGE_COMPONENT_ENABLED_STATE;
    }
    else if (permission == Permissions::ChangeConfiguration) {
        typeStr = AndroidPermNames::CHANGE_CONFIGURATION;
    }
    else if (permission == Permissions::ChangeNetworkState) {
        typeStr = AndroidPermNames::CHANGE_NETWORK_STATE;
    }
    else if (permission == Permissions::ChangeWiFiMulticastState) {
        typeStr = AndroidPermNames::CHANGE_WIFI_MULTICAST_STATE;
    }
    else if (permission == Permissions::ChangeWiFiState) {
        typeStr = AndroidPermNames::CHANGE_WIFI_STATE;
    }
    else if (permission == Permissions::ClearAppCache) {
        typeStr = AndroidPermNames::CLEAR_APP_CACHE;
    }
    else if (permission == Permissions::ControlLocationUpdates) {
        typeStr = AndroidPermNames::CONTROL_LOCATION_UPDATES;
    }
    else if (permission == Permissions::DeleteCacheFiles) {
        typeStr = AndroidPermNames::DELETE_CACHE_FILES;
    }
    else if (permission == Permissions::DeletePackages) {
        typeStr = AndroidPermNames::DELETE_PACKAGES;
    }
    else if (permission == Permissions::Diagnostic) {
        typeStr = AndroidPermNames::DIAGNOSTIC;
    }
    else if (permission == Permissions::DisableKeyguard) {
        typeStr = AndroidPermNames::DISABLE_KEYGUARD;
    }
    else if (permission == Permissions::Dump) {
        typeStr = AndroidPermNames::DUMP;
    }
    else if (permission == Permissions::ExpandStatusBar) {
        typeStr = AndroidPermNames::EXPAND_STATUS_BAR;
    }
    else if (permission == Permissions::FactoryTest) {
        typeStr = AndroidPermNames::FACTORY_TEST;
    }
    else if (permission == Permissions::ForegroundService) {
        typeStr = AndroidPermNames::FOREGROUND_SERVICE;
    }
    else if (permission == Permissions::GetAccounts) {
        typeStr = AndroidPermNames::GET_ACCOUNTS;
    }
    else if (permission == Permissions::GetAccountsPrivileged) {
        typeStr = AndroidPermNames::GET_ACCOUNTS_PRIVILEGED;
    }
    else if (permission == Permissions::GetPackageSize) {
        typeStr = AndroidPermNames::GET_PACKAGE_SIZE;
    }
    else if (permission == Permissions::GetTasks) {
        typeStr = AndroidPermNames::GET_TASKS;
    }
    else if (permission == Permissions::GlobalSearch) {
        typeStr = AndroidPermNames::GLOBAL_SEARCH;
    }
    else if (permission == Permissions::InstallLocationProvider) {
        typeStr = AndroidPermNames::INSTALL_LOCATION_PROVIDER;
    }
    else if (permission == Permissions::InstallPackages) {
        typeStr = AndroidPermNames::INSTALL_PACKAGES;
    }
    else if (permission == Permissions::InstallShortcut) {
        typeStr = AndroidPermNames::INSTALL_SHORTCUT;
    }
    else if (permission == Permissions::InstantAppForegroundService) {
        typeStr = AndroidPermNames::INSTANT_APP_FOREGROUND_SERVICE;
    }
    else if (permission == Permissions::Internet) {
        typeStr = AndroidPermNames::INTERNET;
    }
    else if (permission == Permissions::KillBackgroundProcesses) {
        typeStr = AndroidPermNames::KILL_BACKGROUND_PROCESSES;
    }
    else if (permission == Permissions::LocationHardware) {
        typeStr = AndroidPermNames::LOCATION_HARDWARE;
    }
    else if (permission == Permissions::ManageDocuments) {
        typeStr = AndroidPermNames::MANAGE_DOCUMENTS;
    }
    else if (permission == Permissions::ManageOwnCalls) {
        typeStr = AndroidPermNames::MANAGE_OWN_CALLS;
    }
    else if (permission == Permissions::MediaContentControl) {
        typeStr = AndroidPermNames::MEDIA_CONTENT_CONTROL;
    }
    else if (permission == Permissions::ModifyAudioSettings) {
        typeStr = AndroidPermNames::MODIFY_AUDIO_SETTINGS;
    }
    else if (permission == Permissions::ModifyPhoneState) {
        typeStr = AndroidPermNames::MODIFY_PHONE_STATE;
    }
    else if (permission == Permissions::MountFormatFileSystems) {
        typeStr = AndroidPermNames::MOUNT_FORMAT_FILESYSTEMS;
    }
    else if (permission == Permissions::MountUnmountFileSystems) {
        typeStr = AndroidPermNames::MOUNT_UNMOUNT_FILESYSTEMS;
    }
    else if (permission == Permissions::NFC) {
        typeStr = AndroidPermNames::NFC_PERM;
    }
    else if (permission == Permissions::NFCTransactionEvent) {
        typeStr = AndroidPermNames::NFC_TRANSACTION_EVENT;
    }
    else if (permission == Permissions::PackageUsageStats) {
        typeStr = AndroidPermNames::PACKAGE_USAGE_STATS;
    }
    else if (permission == Permissions::ProcessOutgoingCalls) {
        typeStr = AndroidPermNames::PROCESS_OUTGOING_CALLS;
    }
    else if (permission == Permissions::Calendar) {
        typeStr = AndroidPermNames::READ_CALENDAR;
    }
    else if (permission == Permissions::ReadCallLog) {
        typeStr = AndroidPermNames::READ_CALL_LOG;
    }
    else if (permission == Permissions::Contacts) {
        typeStr = AndroidPermNames::READ_CONTACTS;
    }
    else if (permission == Permissions::ReadExternalStorage) {
        typeStr = AndroidPermNames::READ_EXTERNAL_STORAGE;
    }
    else if (permission == Permissions::ReadFrameBuffer) {
        typeStr = AndroidPermNames::READ_FRAME_BUFFER;
    }
    else if (permission == Permissions::ReadLogs) {
        typeStr = AndroidPermNames::READ_LOGS;
    }
    else if (permission == Permissions::ReadPhoneNumbers) {
        typeStr = AndroidPermNames::READ_PHONE_NUMBERS;
    }
    else if (permission == Permissions::ReadPhoneState) {
        typeStr = AndroidPermNames::READ_PHONE_STATE;
    }
    else if (permission == Permissions::ReadSMS) {
        typeStr = AndroidPermNames::READ_SMS;
    }
    else if (permission == Permissions::ReadSyncSettings) {
        typeStr = AndroidPermNames::READ_SYNC_SETTINGS;
    }
    else if (permission == Permissions::ReadSyncStats) {
        typeStr = AndroidPermNames::READ_SYNC_STATS;
    }
    else if (permission == Permissions::ReadVoicemail) {
        typeStr = AndroidPermNames::READ_VOICEMAIL;
    }
    else if (permission == Permissions::Reboot) {
        typeStr = AndroidPermNames::REBOOT;
    }
    else if (permission == Permissions::ReceiveBootCompleted) {
        typeStr = AndroidPermNames::RECEIVE_BOOT_COMPLETED;
    }
    else if (permission == Permissions::ReceiveMMS) {
        typeStr = AndroidPermNames::RECEIVE_MMS;
    }
    else if (permission == Permissions::ReceiveSMS) {
        typeStr = AndroidPermNames::RECEIVE_SMS;
    }
    else if (permission == Permissions::ReceiveWAPPush) {
        typeStr = AndroidPermNames::RECEIVE_WAP_PUSH;
    }
    else if (permission == Permissions::Microphone) {
        typeStr = AndroidPermNames::RECORD_AUDIO;
    }
    else if (permission == Permissions::ReorderTasks) {
        typeStr = AndroidPermNames::REORDER_TASKS;
    }
    else if (permission == Permissions::RequestCompanionRunInBackground) {
        typeStr = AndroidPermNames::REQUEST_COMPANION_RUN_IN_BACKGROUND;
    }
    else if (permission == Permissions::RequestCompanionUseDataInBackground) {
        typeStr = AndroidPermNames::REQUEST_COMPANION_USE_DATA_IN_BACKGROUND;
    }
    else if (permission == Permissions::RequestDeletePackages) {
        typeStr = AndroidPermNames::REQUEST_DELETE_PACKAGES;
    }
    else if (permission == Permissions::RequestIgnoreBatteryOptimization) {
        typeStr = AndroidPermNames::REQUEST_IGNORE_BATTERY_OPTIMIZATION;
    }
    else if (permission == Permissions::RequestInstallPackages) {
        typeStr = AndroidPermNames::REQUEST_INSTALL_PACKAGES;
    }
    else if (permission == Permissions::SendRespondViaMessage) {
        typeStr = AndroidPermNames::SEND_RESPOND_VIA_MESSAGE;
    }
    else if (permission == Permissions::SendSMS) {
        typeStr = AndroidPermNames::SEND_SMS;
    }
    else if (permission == Permissions::SetAlarm) {
        typeStr = AndroidPermNames::SET_ALARM;
    }
    else if (permission == Permissions::SetAlwaysFinish) {
        typeStr = AndroidPermNames::SET_ALWAYS_FINISH;
    }
    else if (permission == Permissions::SetAnimationScale) {
        typeStr = AndroidPermNames::SET_ANIMATION_SCALE;
    }
    else if (permission == Permissions::SetDebugApp) {
        typeStr = AndroidPermNames::SET_DEBUG_APP;
    }
    else if (permission == Permissions::SetProcessLimit) {
        typeStr = AndroidPermNames::SET_PROCESS_LIMIT;
    }
    else if (permission == Permissions::SetTime) {
        typeStr = AndroidPermNames::SET_TIME;
    }
    else if (permission == Permissions::SetTimeZone) {
        typeStr = AndroidPermNames::SET_TIME_ZONE;
    }
    else if (permission == Permissions::SetWallpaper) {
        typeStr = AndroidPermNames::SET_WALLPAPER;
    }
    else if (permission == Permissions::SetWallpaperHints) {
        typeStr = AndroidPermNames::SET_WALLPAPER_HINTS;
    }
    else if (permission == Permissions::SignalPersistentProcesses) {
        typeStr = AndroidPermNames::SIGNAL_PERSISTENT_PROCESSES;
    }
    else if (permission == Permissions::StatusBar) {
        typeStr = AndroidPermNames::STATUS_BAR;
    }
    else if (permission == Permissions::SystemAlertWindow) {
        typeStr = AndroidPermNames::SYSTEM_ALERT_WINDOW;
    }
    else if (permission == Permissions::TransmitIR) {
        typeStr = AndroidPermNames::TRANSMIT_IR;
    }
    else if (permission == Permissions::UninstallShortcut) {
        typeStr = AndroidPermNames::UNINSTALL_SHORTCUT;
    }
    else if (permission == Permissions::UpdateDeviceStats) {
        typeStr = AndroidPermNames::UPDATE_DEVICE_STATS;
    }
    else if (permission == Permissions::UseBiometric) {
        typeStr = AndroidPermNames::USE_BIOMETRIC;
    }
    else if (permission == Permissions::UseFingerprint) {
        typeStr = AndroidPermNames::USE_FINGERPRINT;
    }
    else if (permission == Permissions::UseSIP) {
        typeStr = AndroidPermNames::USE_SIP;
    }
    else if (permission == Permissions::Vibrate) {
        typeStr = AndroidPermNames::VIBRATE;
    }
    else if (permission == Permissions::WakeLock) {
        typeStr = AndroidPermNames::WAKE_LOCK;
    }
    else if (permission == Permissions::WriteAPNSettings) {
        typeStr = AndroidPermNames::WRITE_APN_SETTINGS;
    }
    else if (permission == Permissions::WriteCalendar) {
        typeStr = AndroidPermNames::WRITE_CALENDAR;
    }
    else if (permission == Permissions::WriteCallLog) {
        typeStr = AndroidPermNames::WRITE_CALLLOG;
    }
    else if (permission == Permissions::WriteContacts) {
        typeStr = AndroidPermNames::WRITE_CONTACTS;
    }
    else if (permission == Permissions::WriteExternalStorage) {
        typeStr = AndroidPermNames::WRITE_EXTERNAL_STORAGE;
    }
    else if (permission == Permissions::WriteGServices) {
        typeStr = AndroidPermNames::WRITE_GSERVICES;
    }
    else if (permission == Permissions::WriteSecureSettings) {
        typeStr = AndroidPermNames::WRITE_SECURE_SETTINGS;
    }
    else if (permission == Permissions::WriteSettings) {
        typeStr = AndroidPermNames::WRITE_SETTINGS;
    }
    else if (permission == Permissions::WriteSyncSettings) {
        typeStr = AndroidPermNames::WRITE_SYNC_SETTINGS;
    }
    else if (permission == Permissions::WriteVoiceMail) {
        typeStr = AndroidPermNames::WRITE_VOICEMAIL;
    }

    return typeStr;
}

PermissionManager::Permissions PermissionManager::permissionType(const QString &permissionStr) const
{
    Permissions type = Permissions::None;
    if (permissionStr == AndroidPermNames::ACCEPT_HANDOVER) {
        type = Permissions::AcceptHandover;
    }
    else if (permissionStr == AndroidPermNames::ACCESS_CHECKIN_PROPERTIES) {
        type = Permissions::AccessCheckInProperties;
    }
    else if (permissionStr == AndroidPermNames::ACCESS_COARSE_LOCATION) {
        type = Permissions::AccessCoarseLocation;
    }
    else if (permissionStr == AndroidPermNames::ACCESS_FINE_LOCATION) {
        type = Permissions::AccessFineLocation;
    }
    else if (permissionStr == AndroidPermNames::ACCESS_LOCATION_EXTRA_COMMANDS) {
        type = Permissions::AccessLocationExtraCommands;
    }
    else if (permissionStr == AndroidPermNames::ACCESS_NETWORK_STATE) {
        type = Permissions::AccessNetworkState;
    }
    else if (permissionStr == AndroidPermNames::ACCESS_NOTIFICATION_POLICY) {
        type = Permissions::AccessNotificationPolicy;
    }
    else if (permissionStr == AndroidPermNames::ACCESS_WIFI_STATE) {
        type = Permissions::AccessWiFiState;
    }
    else if (permissionStr == AndroidPermNames::ACCOUNT_MANAGER) {
        type = Permissions::AccountManager;
    }
    else if (permissionStr == AndroidPermNames::ADD_VOICEMAIL) {
        type = Permissions::AddVoiceMail;
    }
    else if (permissionStr == AndroidPermNames::ANSWER_PHONE_CALLS) {
        type = Permissions::AnswerPhoneCalls;
    }
    else if (permissionStr == AndroidPermNames::BATTERY_STATS) {
        type = Permissions::BatteryStats;
    }
    else if (permissionStr == AndroidPermNames::BIND_ACCESSIBILITY_SERVICE) {
        type = Permissions::BindAccessibilityService;
    }
    else if (permissionStr == AndroidPermNames::BIND_APPWIDGET) {
        type = Permissions::BindAppWidget;
    }
    else if (permissionStr == AndroidPermNames::BIND_AUTOFILL_SERVICE) {
        type = Permissions::BindAutofillService;
    }
    else if (permissionStr == AndroidPermNames::BIND_CARRIER_MESSAGING_SERVICE) {
        type = Permissions::BindCarrierMessagingService;
    }
    else if (permissionStr == AndroidPermNames::BIND_CARRIER_SERVICES) {
        type = Permissions::BindCarrierServices;
    }
    else if (permissionStr == AndroidPermNames::BIND_CHOOSER_TARGET_SERVICE) {
        type = Permissions::BindChooserTargetService;
    }
    else if (permissionStr == AndroidPermNames::BIND_CONDITION_PROVIDER_SERVICE) {
        type = Permissions::BindConditionProviderService;
    }
    else if (permissionStr == AndroidPermNames::BIND_DEVICE_ADMIN) {
        type = Permissions::BindDeviceAdmin;
    }
    else if (permissionStr == AndroidPermNames::BIND_DREAM_SERVICE) {
        type = Permissions::BindDreamService;
    }
    else if (permissionStr == AndroidPermNames::BIND_INCALL_SERVICE) {
        type = Permissions::BindInCallService;
    }
    else if (permissionStr == AndroidPermNames::BIND_INPUT_METHOD) {
        type = Permissions::BindInputMethod;
    }
    else if (permissionStr == AndroidPermNames::BIND_MIDI_DEVICE_SERVICE) {
        type = Permissions::BindMidiDeviceService;
    }
    else if (permissionStr == AndroidPermNames::BIND_NFC_SERVICE) {
        type = Permissions::BindNFCService;
    }
    else if (permissionStr == AndroidPermNames::BIND_NOTIFICATION_LISTENER_SERVICE) {
        type = Permissions::BindNotificationListenerService;
    }
    else if (permissionStr == AndroidPermNames::BIND_PRINT_SERVICE) {
        type = Permissions::BindPrintService;
    }
    else if (permissionStr == AndroidPermNames::BIND_QUICK_SETTINGS_TILE) {
        type = Permissions::BindQuickSettingsTile;
    }
    else if (permissionStr == AndroidPermNames::BIND_REMOTEVIEWS) {
        type = Permissions::BindRemoteViews;
    }
    else if (permissionStr == AndroidPermNames::BIND_SCREENING_SERVICE) {
        type = Permissions::BindScreeningService;
    }
    else if (permissionStr == AndroidPermNames::BIND_TELECOM_CONNECTION_SERVICE) {
        type = Permissions::BindTelecomConnectionService;
    }
    else if (permissionStr == AndroidPermNames::BIND_TEXT_SERVICE) {
        type = Permissions::BindTextService;
    }
    else if (permissionStr == AndroidPermNames::BIND_TV_INPUT) {
        type = Permissions::BindTVInput;
    }
    else if (permissionStr == AndroidPermNames::BIND_VISUAL_VOICEMAIL_SERVICE) {
        type = Permissions::BindVisualVoiceMailService;
    }
    else if (permissionStr == AndroidPermNames::BIND_VOICE_INTERACTION) {
        type = Permissions::BindVoiceInteraction;
    }
    else if (permissionStr == AndroidPermNames::BIND_VPN_SERVICE) {
        type = Permissions::BindVPNService;
    }
    else if (permissionStr == AndroidPermNames::BIND_VR_LISTENER_SERVICE) {
        type = Permissions::BindVRListenerService;
    }
    else if (permissionStr == AndroidPermNames::BIND_WALLPAPER) {
        type = Permissions::BindWallpaper;
    }
    else if (permissionStr == AndroidPermNames::BLUETOOTH) {
        type = Permissions::Bluetooth;
    }
    else if (permissionStr == AndroidPermNames::BLUETOOTH_ADMIN) {
        type = Permissions::BluetoothAdmin;
    }
    else if (permissionStr == AndroidPermNames::BLUETOOTH_PRIVILEGED) {
        type = Permissions::BluetoothPrivileged;
    }
    else if (permissionStr == AndroidPermNames::BODY_SENSORS) {
        type = Permissions::BodySensors;
    }
    else if (permissionStr == AndroidPermNames::BROADCAST_PACKAGE_REMOVED) {
        type = Permissions::BroadcastPackageRemoved;
    }
    else if (permissionStr == AndroidPermNames::BROADCAST_SMS) {
        type = Permissions::BroadcastSMS;
    }
    else if (permissionStr == AndroidPermNames::BROADCAST_STICKY) {
        type = Permissions::BroadcastSticky;
    }
    else if (permissionStr == AndroidPermNames::BROADCAST_WAP_PUSH) {
        type = Permissions::BroadcastWAPPush;
    }
    else if (permissionStr == AndroidPermNames::CALL_PHONE) {
        type = Permissions::CallPhone;
    }
    else if (permissionStr == AndroidPermNames::CALL_PRIVILEGED) {
        type = Permissions::CallPrivileged;
    }
    else if (permissionStr == AndroidPermNames::CAMERA) {
        type = Permissions::Camera;
    }
    else if (permissionStr == AndroidPermNames::CAPTURE_AUDIO_OUTPUT) {
        type = Permissions::CaptureAudioOutput;
    }
    else if (permissionStr == AndroidPermNames::CAPTURE_SECURE_VIDEO_OUTPUT) {
        type = Permissions::CaptureSecureVideoOutput;
    }
    else if (permissionStr == AndroidPermNames::CAPTURE_VIDEO_OUTPUT) {
        type = Permissions::CaptureVideoOutput;
    }
    else if (permissionStr == AndroidPermNames::CHANGE_COMPONENT_ENABLED_STATE) {
        type = Permissions::ChangeComponentEnabledState;
    }
    else if (permissionStr == AndroidPermNames::CHANGE_CONFIGURATION) {
        type = Permissions::ChangeConfiguration;
    }
    else if (permissionStr == AndroidPermNames::CHANGE_NETWORK_STATE) {
        type = Permissions::ChangeNetworkState;
    }
    else if (permissionStr == AndroidPermNames::CHANGE_WIFI_MULTICAST_STATE) {
        type = Permissions::ChangeWiFiMulticastState;
    }
    else if (permissionStr == AndroidPermNames::CHANGE_WIFI_STATE) {
        type = Permissions::ChangeWiFiState;
    }
    else if (permissionStr == AndroidPermNames::CLEAR_APP_CACHE) {
        type = Permissions::ClearAppCache;
    }
    else if (permissionStr == AndroidPermNames::CONTROL_LOCATION_UPDATES) {
        type = Permissions::ControlLocationUpdates;
    }
    else if (permissionStr == AndroidPermNames::DELETE_CACHE_FILES) {
        type = Permissions::DeleteCacheFiles;
    }
    else if (permissionStr == AndroidPermNames::DELETE_PACKAGES) {
        type = Permissions::DeletePackages;
    }
    else if (permissionStr == AndroidPermNames::DIAGNOSTIC) {
        type = Permissions::Diagnostic;
    }
    else if (permissionStr == AndroidPermNames::DISABLE_KEYGUARD) {
        type = Permissions::DisableKeyguard;
    }
    else if (permissionStr == AndroidPermNames::DUMP) {
        type = Permissions::Dump;
    }
    else if (permissionStr == AndroidPermNames::EXPAND_STATUS_BAR) {
        type = Permissions::ExpandStatusBar;
    }
    else if (permissionStr == AndroidPermNames::FACTORY_TEST) {
        type = Permissions::FactoryTest;
    }
    else if (permissionStr == AndroidPermNames::FOREGROUND_SERVICE) {
        type = Permissions::ForegroundService;
    }
    else if (permissionStr == AndroidPermNames::GET_ACCOUNTS) {
        type = Permissions::GetAccounts;
    }
    else if (permissionStr == AndroidPermNames::GET_ACCOUNTS_PRIVILEGED) {
        type = Permissions::GetAccountsPrivileged;
    }
    else if (permissionStr == AndroidPermNames::GET_PACKAGE_SIZE) {
        type = Permissions::GetPackageSize;
    }
    else if (permissionStr == AndroidPermNames::GET_TASKS) {
        type = Permissions::GetTasks;
    }
    else if (permissionStr == AndroidPermNames::GLOBAL_SEARCH) {
        type = Permissions::GlobalSearch;
    }
    else if (permissionStr == AndroidPermNames::INSTALL_LOCATION_PROVIDER) {
        type = Permissions::InstallLocationProvider;
    }
    else if (permissionStr == AndroidPermNames::INSTALL_PACKAGES) {
        type = Permissions::InstallPackages;
    }
    else if (permissionStr == AndroidPermNames::INSTALL_SHORTCUT) {
        type = Permissions::InstallShortcut;
    }
    else if (permissionStr == AndroidPermNames::INSTANT_APP_FOREGROUND_SERVICE) {
        type = Permissions::InstantAppForegroundService;
    }
    else if (permissionStr == AndroidPermNames::INTERNET) {
        type = Permissions::Internet;
    }
    else if (permissionStr == AndroidPermNames::KILL_BACKGROUND_PROCESSES) {
        type = Permissions::KillBackgroundProcesses;
    }
    else if (permissionStr == AndroidPermNames::LOCATION_HARDWARE) {
        type = Permissions::LocationHardware;
    }
    else if (permissionStr == AndroidPermNames::MANAGE_DOCUMENTS) {
        type = Permissions::ManageDocuments;
    }
    else if (permissionStr == AndroidPermNames::MANAGE_OWN_CALLS) {
        type = Permissions::ManageOwnCalls;
    }
    else if (permissionStr == AndroidPermNames::MEDIA_CONTENT_CONTROL) {
        type = Permissions::MediaContentControl;
    }
    else if (permissionStr == AndroidPermNames::MODIFY_AUDIO_SETTINGS) {
        type = Permissions::ModifyAudioSettings;
    }
    else if (permissionStr == AndroidPermNames::MODIFY_PHONE_STATE) {
        type = Permissions::ModifyPhoneState;
    }
    else if (permissionStr == AndroidPermNames::MOUNT_FORMAT_FILESYSTEMS) {
        type = Permissions::MountFormatFileSystems;
    }
    else if (permissionStr == AndroidPermNames::MOUNT_UNMOUNT_FILESYSTEMS) {
        type = Permissions::MountUnmountFileSystems;
    }
    else if (permissionStr == AndroidPermNames::NFC_PERM) {
        type = Permissions::NFC;
    }
    else if (permissionStr == AndroidPermNames::NFC_TRANSACTION_EVENT) {
        type = Permissions::NFCTransactionEvent;
    }
    else if (permissionStr == AndroidPermNames::PACKAGE_USAGE_STATS) {
        type = Permissions::PackageUsageStats;
    }
    else if (permissionStr == AndroidPermNames::PROCESS_OUTGOING_CALLS) {
        type = Permissions::ProcessOutgoingCalls;
    }
    else if (permissionStr == AndroidPermNames::READ_CALENDAR) {
        type = Permissions::Calendar;
    }
    else if (permissionStr == AndroidPermNames::READ_CALL_LOG) {
        type = Permissions::ReadCallLog;
    }
    else if (permissionStr == AndroidPermNames::READ_CONTACTS) {
        type = Permissions::Contacts;
    }
    else if (permissionStr == AndroidPermNames::READ_EXTERNAL_STORAGE) {
        type = Permissions::ReadExternalStorage;
    }
    else if (permissionStr == AndroidPermNames::READ_FRAME_BUFFER) {
        type = Permissions::ReadFrameBuffer;
    }
    else if (permissionStr == AndroidPermNames::READ_LOGS) {
        type = Permissions::ReadLogs;
    }
    else if (permissionStr == AndroidPermNames::READ_PHONE_NUMBERS) {
        type = Permissions::ReadPhoneNumbers;
    }
    else if (permissionStr == AndroidPermNames::READ_PHONE_STATE) {
        type = Permissions::ReadPhoneState;
    }
    else if (permissionStr == AndroidPermNames::READ_SMS) {
        type = Permissions::ReadSMS;
    }
    else if (permissionStr == AndroidPermNames::READ_SYNC_SETTINGS) {
        type = Permissions::ReadSyncSettings;
    }
    else if (permissionStr == AndroidPermNames::READ_SYNC_STATS) {
        type = Permissions::ReadSyncStats;
    }
    else if (permissionStr == AndroidPermNames::READ_VOICEMAIL) {
        type = Permissions::ReadVoicemail;
    }
    else if (permissionStr == AndroidPermNames::REBOOT) {
        type = Permissions::Reboot;
    }
    else if (permissionStr == AndroidPermNames::RECEIVE_BOOT_COMPLETED) {
        type = Permissions::ReceiveBootCompleted;
    }
    else if (permissionStr == AndroidPermNames::RECEIVE_MMS) {
        type = Permissions::ReceiveMMS;
    }
    else if (permissionStr == AndroidPermNames::RECEIVE_SMS) {
        type = Permissions::ReceiveSMS;
    }
    else if (permissionStr == AndroidPermNames::RECEIVE_WAP_PUSH) {
        type = Permissions::ReceiveWAPPush;
    }
    else if (permissionStr == AndroidPermNames::RECORD_AUDIO) {
        type = Permissions::Microphone;
    }
    else if (permissionStr == AndroidPermNames::REORDER_TASKS) {
        type = Permissions::ReorderTasks;
    }
    else if (permissionStr == AndroidPermNames::REQUEST_COMPANION_RUN_IN_BACKGROUND) {
        type = Permissions::RequestCompanionRunInBackground;
    }
    else if (permissionStr == AndroidPermNames::REQUEST_COMPANION_USE_DATA_IN_BACKGROUND) {
        type = Permissions::RequestCompanionUseDataInBackground;
    }
    else if (permissionStr == AndroidPermNames::REQUEST_DELETE_PACKAGES) {
        type = Permissions::RequestDeletePackages;
    }
    else if (permissionStr == AndroidPermNames::REQUEST_IGNORE_BATTERY_OPTIMIZATION) {
        type = Permissions::RequestIgnoreBatteryOptimization;
    }
    else if (permissionStr == AndroidPermNames::REQUEST_INSTALL_PACKAGES) {
        type = Permissions::RequestInstallPackages;
    }
    else if (permissionStr == AndroidPermNames::SEND_RESPOND_VIA_MESSAGE) {
        type = Permissions::SendRespondViaMessage;
    }
    else if (permissionStr == AndroidPermNames::SEND_SMS) {
        type = Permissions::SendSMS;
    }
    else if (permissionStr == AndroidPermNames::SET_ALARM) {
        type = Permissions::SetAlarm;
    }
    else if (permissionStr == AndroidPermNames::SET_ALWAYS_FINISH) {
        type = Permissions::SetAlwaysFinish;
    }
    else if (permissionStr == AndroidPermNames::SET_ANIMATION_SCALE) {
        type = Permissions::SetAnimationScale;
    }
    else if (permissionStr == AndroidPermNames::SET_DEBUG_APP) {
        type = Permissions::SetDebugApp;
    }
    else if (permissionStr == AndroidPermNames::SET_PROCESS_LIMIT) {
        type = Permissions::SetProcessLimit;
    }
    else if (permissionStr == AndroidPermNames::SET_TIME) {
        type = Permissions::SetTime;
    }
    else if (permissionStr == AndroidPermNames::SET_TIME_ZONE) {
        type = Permissions::SetTimeZone;
    }
    else if (permissionStr == AndroidPermNames::SET_WALLPAPER) {
        type = Permissions::SetWallpaper;
    }
    else if (permissionStr == AndroidPermNames::SET_WALLPAPER_HINTS) {
        type = Permissions::SetWallpaperHints;
    }
    else if (permissionStr == AndroidPermNames::SIGNAL_PERSISTENT_PROCESSES) {
        type = Permissions::SignalPersistentProcesses;
    }
    else if (permissionStr == AndroidPermNames::STATUS_BAR) {
        type = Permissions::StatusBar;
    }
    else if (permissionStr == AndroidPermNames::SYSTEM_ALERT_WINDOW) {
        type = Permissions::SystemAlertWindow;
    }
    else if (permissionStr == AndroidPermNames::TRANSMIT_IR) {
        type = Permissions::TransmitIR;
    }
    else if (permissionStr == AndroidPermNames::UNINSTALL_SHORTCUT) {
        type = Permissions::UninstallShortcut;
    }
    else if (permissionStr == AndroidPermNames::UPDATE_DEVICE_STATS) {
        type = Permissions::UpdateDeviceStats;
    }
    else if (permissionStr == AndroidPermNames::USE_BIOMETRIC) {
        type = Permissions::UseBiometric;
    }
    else if (permissionStr == AndroidPermNames::USE_FINGERPRINT) {
        type = Permissions::UseFingerprint;
    }
    else if (permissionStr == AndroidPermNames::USE_SIP) {
        type = Permissions::UseSIP;
    }
    else if (permissionStr == AndroidPermNames::VIBRATE) {
        type = Permissions::Vibrate;
    }
    else if (permissionStr == AndroidPermNames::WAKE_LOCK) {
        type = Permissions::WakeLock;
    }
    else if (permissionStr == AndroidPermNames::WRITE_APN_SETTINGS) {
        type = Permissions::WriteAPNSettings;
    }
    else if (permissionStr == AndroidPermNames::WRITE_CALENDAR) {
        type = Permissions::WriteCalendar;
    }
    else if (permissionStr == AndroidPermNames::WRITE_CALLLOG) {
        type = Permissions::WriteCallLog;
    }
    else if (permissionStr == AndroidPermNames::WRITE_CONTACTS) {
        type = Permissions::WriteContacts;
    }
    else if (permissionStr == AndroidPermNames::WRITE_EXTERNAL_STORAGE) {
        type = Permissions::WriteExternalStorage;
    }
    else if (permissionStr == AndroidPermNames::WRITE_GSERVICES) {
        type = Permissions::WriteGServices;
    }
    else if (permissionStr == AndroidPermNames::WRITE_SECURE_SETTINGS) {
        type = Permissions::WriteSecureSettings;
    }
    else if (permissionStr == AndroidPermNames::WRITE_SETTINGS) {
        type = Permissions::WriteSettings;
    }
    else if (permissionStr == AndroidPermNames::WRITE_SYNC_SETTINGS) {
        type = Permissions::WriteSyncSettings;
    }
    else if (permissionStr == AndroidPermNames::WRITE_VOICEMAIL) {
        type = Permissions::WriteVoiceMail;
    }

    return type;
}

#if defined(Q_OS_ANDROID)
void PermissionManager::permissionResultCallback(const QHash<QString, QtAndroid::PermissionResult> &results)
{
    if (results.size() > 1) {
        QList<QObject *> data;
        for (auto it = results.constBegin(); it != results.constEnd(); it++) {
            PermissionRequestResult *reqResult = new PermissionRequestResult(this);
            reqResult->setPermission(static_cast<int>(permissionType(it.key())));
            Result result = it.value() == QtAndroid::PermissionResult::Granted ? Result::Granted : Result::Denied;
            reqResult->setResult(static_cast<int>(result));
            data.append(reqResult);
        }

        emit permissionResultsReceived(data);
    }
    else if (results.size() == 1) {
        const QStringList keys = results.keys();
        const int result = static_cast<int>(results[keys.at(0)] == QtAndroid::PermissionResult::Granted ? Result::Granted : Result::Denied);
        emit permissionResultReceived(result, static_cast<int>(permissionType(keys.at(0))));
    }
    else {
        LOG_ERROR("The request for permission resulted in 0 results.");
    }
}
#endif // Q_OS_ANDROID

#if defined(Q_OS_IOS)
void PermissionManager::permissionResultCallback(Permissions permission, PermissionManagerPrivate::AuthorizationStatus status)
{
    Result result = getResultFromAuthStatus(status);
    emit permissionResultReceived(result, permission);
}

PermissionManager::Result PermissionManager::getResultFromAuthStatus(PermissionManagerPrivate::AuthorizationStatus status) const
{
    Result result = Result::Denied;
    if (status == PermissionManagerPrivate::AuthorizationStatus::Authorized) {
        result = Result::Granted;
    }
    else if (status == PermissionManagerPrivate::AuthorizationStatus::AuthorizedWhenInUse) {
        result = Result::AuthorizedWhenInUse;
    }
    else if (status == PermissionManagerPrivate::AuthorizationStatus::Denied) {
        result = Result::Denied;
    }
    else if (status == PermissionManagerPrivate::AuthorizationStatus::NotDetermined) {
        result = Result::NotDetermined;
    }
    else if (status == PermissionManagerPrivate::AuthorizationStatus::Restricted) {
        result = Result::Restricted;
    }

    return result;
}
#endif // Q_OS_IOS

}
