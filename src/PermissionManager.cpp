#include "qutils/PermissionManager.h"
// qutils
#include "qutils/Macros.h"

namespace zmc
{

PermissionManager::PermissionManager(QObject *parent)
    : QObject(parent)
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
#endif // Q_OS_ANDROID

    return result;
}

void PermissionManager::requestPermission(int permission)
{
#if defined(Q_OS_ANDROID)
    requestPermissions(QList<int> {permission});
#endif // Q_OS_ANDROID
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
#endif // Q_OS_ANDROID
}

QString PermissionManager::permissionString(Permissions permission) const
{
    QString typeStr = "";
    if (permission == Permissions::AcceptHandover) {
        typeStr = ACCEPT_HANDOVER;
    }
    else if (permission == Permissions::AccessCheckInProperties) {
        typeStr = ACCESS_CHECKIN_PROPERTIES;
    }
    else if (permission == Permissions::AccessCoarseLocation) {
        typeStr = ACCESS_COARSE_LOCATION;
    }
    else if (permission == Permissions::AccessFineLocation) {
        typeStr = ACCESS_FINE_LOCATION;
    }
    else if (permission == Permissions::AccessLocationExtraCommands) {
        typeStr = ACCESS_LOCATION_EXTRA_COMMANDS;
    }
    else if (permission == Permissions::AccessNetworkState) {
        typeStr = ACCESS_NETWORK_STATE;
    }
    else if (permission == Permissions::AccessNotificationPolicy) {
        typeStr = ACCESS_NOTIFICATION_POLICY;
    }
    else if (permission == Permissions::AccessWiFiState) {
        typeStr = ACCESS_WIFI_STATE;
    }
    else if (permission == Permissions::AccountManager) {
        typeStr = ACCOUNT_MANAGER;
    }
    else if (permission == Permissions::AddVoiceMail) {
        typeStr = ADD_VOICEMAIL;
    }
    else if (permission == Permissions::AnswerPhoneCalls) {
        typeStr = ANSWER_PHONE_CALLS;
    }
    else if (permission == Permissions::BatteryStats) {
        typeStr = BATTERY_STATS;
    }
    else if (permission == Permissions::BindAccessibilityService) {
        typeStr = BIND_ACCESSIBILITY_SERVICE;
    }
    else if (permission == Permissions::BindAppWidget) {
        typeStr = BIND_APPWIDGET;
    }
    else if (permission == Permissions::BindAutofillService) {
        typeStr = BIND_AUTOFILL_SERVICE;
    }
    else if (permission == Permissions::BindCarrierMessagingService) {
        typeStr = BIND_CARRIER_MESSAGING_SERVICE;
    }
    else if (permission == Permissions::BindCarrierServices) {
        typeStr = BIND_CARRIER_SERVICES;
    }
    else if (permission == Permissions::BindChooserTargetService) {
        typeStr = BIND_CHOOSER_TARGET_SERVICE;
    }
    else if (permission == Permissions::BindConditionProviderService) {
        typeStr = BIND_CONDITION_PROVIDER_SERVICE;
    }
    else if (permission == Permissions::BindDeviceAdmin) {
        typeStr = BIND_DEVICE_ADMIN;
    }
    else if (permission == Permissions::BindDreamService) {
        typeStr = BIND_DREAM_SERVICE;
    }
    else if (permission == Permissions::BindInCallService) {
        typeStr = BIND_INCALL_SERVICE;
    }
    else if (permission == Permissions::BindInputMethod) {
        typeStr = BIND_INPUT_METHOD;
    }
    else if (permission == Permissions::BindMidiDeviceService) {
        typeStr = BIND_MIDI_DEVICE_SERVICE;
    }
    else if (permission == Permissions::BindNFCService) {
        typeStr = BIND_NFC_SERVICE;
    }
    else if (permission == Permissions::BindNotificationListenerService) {
        typeStr = BIND_NOTIFICATION_LISTENER_SERVICE;
    }
    else if (permission == Permissions::BindPrintService) {
        typeStr = BIND_PRINT_SERVICE;
    }
    else if (permission == Permissions::BindQuickSettingsTile) {
        typeStr = BIND_QUICK_SETTINGS_TILE;
    }
    else if (permission == Permissions::BindRemoteViews) {
        typeStr = BIND_REMOTEVIEWS;
    }
    else if (permission == Permissions::BindScreeningService) {
        typeStr = BIND_SCREENING_SERVICE;
    }
    else if (permission == Permissions::BindTelecomConnectionService) {
        typeStr = BIND_TELECOM_CONNECTION_SERVICE;
    }
    else if (permission == Permissions::BindTextService) {
        typeStr = BIND_TEXT_SERVICE;
    }
    else if (permission == Permissions::BindTVInput) {
        typeStr = BIND_TV_INPUT;
    }
    else if (permission == Permissions::BindVisualVoiceMailService) {
        typeStr = BIND_VISUAL_VOICEMAIL_SERVICE;
    }
    else if (permission == Permissions::BindVoiceInteraction) {
        typeStr = BIND_VOICE_INTERACTION;
    }
    else if (permission == Permissions::BindVPNService) {
        typeStr = BIND_VPN_SERVICE;
    }
    else if (permission == Permissions::BindVRListenerService) {
        typeStr = BIND_VR_LISTENER_SERVICE;
    }
    else if (permission == Permissions::BindWallpaper) {
        typeStr = BIND_WALLPAPER;
    }
    else if (permission == Permissions::Bluetooth) {
        typeStr = BLUETOOTH;
    }
    else if (permission == Permissions::BluetoothAdmin) {
        typeStr = BLUETOOTH_ADMIN;
    }
    else if (permission == Permissions::BluetoothPrivileged) {
        typeStr = BLUETOOTH_PRIVILEGED;
    }
    else if (permission == Permissions::BodySensors) {
        typeStr = BODY_SENSORS;
    }
    else if (permission == Permissions::BroadcastPackageRemoved) {
        typeStr = BROADCAST_PACKAGE_REMOVED;
    }
    else if (permission == Permissions::BroadcastSMS) {
        typeStr = BROADCAST_SMS;
    }
    else if (permission == Permissions::BroadcastSticky) {
        typeStr = BROADCAST_STICKY;
    }
    else if (permission == Permissions::BroadcastWAPPush) {
        typeStr = BROADCAST_WAP_PUSH;
    }
    else if (permission == Permissions::CallPhone) {
        typeStr = CALL_PHONE;
    }
    else if (permission == Permissions::CallPrivileged) {
        typeStr = CALL_PRIVILEGED;
    }
    else if (permission == Permissions::Camera) {
        typeStr = CAMERA;
    }
    else if (permission == Permissions::CaptureAudioOutput) {
        typeStr = CAPTURE_AUDIO_OUTPUT;
    }
    else if (permission == Permissions::CaptureSecureVideoOutput) {
        typeStr = CAPTURE_SECURE_VIDEO_OUTPUT;
    }
    else if (permission == Permissions::CaptureVideoOutput) {
        typeStr = CAPTURE_VIDEO_OUTPUT;
    }
    else if (permission == Permissions::ChangeComponentEnabledState) {
        typeStr = CHANGE_COMPONENT_ENABLED_STATE;
    }
    else if (permission == Permissions::ChangeConfiguration) {
        typeStr = CHANGE_CONFIGURATION;
    }
    else if (permission == Permissions::ChangeNetworkState) {
        typeStr = CHANGE_NETWORK_STATE;
    }
    else if (permission == Permissions::ChangeWiFiMulticastState) {
        typeStr = CHANGE_WIFI_MULTICAST_STATE;
    }
    else if (permission == Permissions::ChangeWiFiState) {
        typeStr = CHANGE_WIFI_STATE;
    }
    else if (permission == Permissions::ClearAppCache) {
        typeStr = CLEAR_APP_CACHE;
    }
    else if (permission == Permissions::ControlLocationUpdates) {
        typeStr = CONTROL_LOCATION_UPDATES;
    }
    else if (permission == Permissions::DeleteCacheFiles) {
        typeStr = DELETE_CACHE_FILES;
    }
    else if (permission == Permissions::DeletePackages) {
        typeStr = DELETE_PACKAGES;
    }
    else if (permission == Permissions::Diagnostic) {
        typeStr = DIAGNOSTIC;
    }
    else if (permission == Permissions::DisableKeyguard) {
        typeStr = DISABLE_KEYGUARD;
    }
    else if (permission == Permissions::Dump) {
        typeStr = DUMP;
    }
    else if (permission == Permissions::ExpandStatusBar) {
        typeStr = EXPAND_STATUS_BAR;
    }
    else if (permission == Permissions::FactoryTest) {
        typeStr = FACTORY_TEST;
    }
    else if (permission == Permissions::ForegroundService) {
        typeStr = FOREGROUND_SERVICE;
    }
    else if (permission == Permissions::GetAccounts) {
        typeStr = GET_ACCOUNTS;
    }
    else if (permission == Permissions::GetAccountsPrivileged) {
        typeStr = GET_ACCOUNTS_PRIVILEGED;
    }
    else if (permission == Permissions::GetPackageSize) {
        typeStr = GET_PACKAGE_SIZE;
    }
    else if (permission == Permissions::GetTasks) {
        typeStr = GET_TASKS;
    }
    else if (permission == Permissions::GlobalSearch) {
        typeStr = GLOBAL_SEARCH;
    }
    else if (permission == Permissions::InstallLocationProvider) {
        typeStr = INSTALL_LOCATION_PROVIDER;
    }
    else if (permission == Permissions::InstallPackages) {
        typeStr = INSTALL_PACKAGES;
    }
    else if (permission == Permissions::InstallShortcut) {
        typeStr = INSTALL_SHORTCUT;
    }
    else if (permission == Permissions::InstantAppForegroundService) {
        typeStr = INSTANT_APP_FOREGROUND_SERVICE;
    }
    else if (permission == Permissions::Internet) {
        typeStr = INTERNET;
    }
    else if (permission == Permissions::KillBackgroundProcesses) {
        typeStr = KILL_BACKGROUND_PROCESSES;
    }
    else if (permission == Permissions::LocationHardware) {
        typeStr = LOCATION_HARDWARE;
    }
    else if (permission == Permissions::ManageDocuments) {
        typeStr = MANAGE_DOCUMENTS;
    }
    else if (permission == Permissions::ManageOwnCalls) {
        typeStr = MANAGE_OWN_CALLS;
    }
    else if (permission == Permissions::MediaContentControl) {
        typeStr = MEDIA_CONTENT_CONTROL;
    }
    else if (permission == Permissions::ModifyAudioSettings) {
        typeStr = MODIFY_AUDIO_SETTINGS;
    }
    else if (permission == Permissions::ModifyPhoneState) {
        typeStr = MODIFY_PHONE_STATE;
    }
    else if (permission == Permissions::MountFormatFileSystems) {
        typeStr = MOUNT_FORMAT_FILESYSTEMS;
    }
    else if (permission == Permissions::MountUnmountFileSystems) {
        typeStr = MOUNT_UNMOUNT_FILESYSTEMS;
    }
    else if (permission == Permissions::NFC) {
        typeStr = NFC_PERM;
    }
    else if (permission == Permissions::NFCTransactionEvent) {
        typeStr = NFC_TRANSACTION_EVENT;
    }
    else if (permission == Permissions::PackageUsageStats) {
        typeStr = PACKAGE_USAGE_STATS;
    }
    else if (permission == Permissions::ProcessOutgoingCalls) {
        typeStr = PROCESS_OUTGOING_CALLS;
    }
    else if (permission == Permissions::Calendar) {
        typeStr = READ_CALENDAR;
    }
    else if (permission == Permissions::ReadCallLog) {
        typeStr = READ_CALL_LOG;
    }
    else if (permission == Permissions::Contacts) {
        typeStr = READ_CONTACTS;
    }
    else if (permission == Permissions::ReadExternalStorage) {
        typeStr = READ_EXTERNAL_STORAGE;
    }
    else if (permission == Permissions::ReadFrameBuffer) {
        typeStr = READ_FRAME_BUFFER;
    }
    else if (permission == Permissions::ReadLogs) {
        typeStr = READ_LOGS;
    }
    else if (permission == Permissions::ReadPhoneNumbers) {
        typeStr = READ_PHONE_NUMBERS;
    }
    else if (permission == Permissions::ReadPhoneState) {
        typeStr = READ_PHONE_STATE;
    }
    else if (permission == Permissions::ReadSMS) {
        typeStr = READ_SMS;
    }
    else if (permission == Permissions::ReadSyncSettings) {
        typeStr = READ_SYNC_SETTINGS;
    }
    else if (permission == Permissions::ReadSyncStats) {
        typeStr = READ_SYNC_STATS;
    }
    else if (permission == Permissions::ReadVoicemail) {
        typeStr = READ_VOICEMAIL;
    }
    else if (permission == Permissions::Reboot) {
        typeStr = REBOOT;
    }
    else if (permission == Permissions::ReceiveBootCompleted) {
        typeStr = RECEIVE_BOOT_COMPLETED;
    }
    else if (permission == Permissions::ReceiveMMS) {
        typeStr = RECEIVE_MMS;
    }
    else if (permission == Permissions::ReceiveSMS) {
        typeStr = RECEIVE_SMS;
    }
    else if (permission == Permissions::ReceiveWAPPush) {
        typeStr = RECEIVE_WAP_PUSH;
    }
    else if (permission == Permissions::Microphone) {
        typeStr = RECORD_AUDIO;
    }
    else if (permission == Permissions::ReorderTasks) {
        typeStr = REORDER_TASKS;
    }
    else if (permission == Permissions::RequestCompanionRunInBackground) {
        typeStr = REQUEST_COMPANION_RUN_IN_BACKGROUND;
    }
    else if (permission == Permissions::RequestCompanionUseDataInBackground) {
        typeStr = REQUEST_COMPANION_USE_DATA_IN_BACKGROUND;
    }
    else if (permission == Permissions::RequestDeletePackages) {
        typeStr = REQUEST_DELETE_PACKAGES;
    }
    else if (permission == Permissions::RequestIgnoreBatteryOptimization) {
        typeStr = REQUEST_IGNORE_BATTERY_OPTIMIZATION;
    }
    else if (permission == Permissions::RequestInstallPackages) {
        typeStr = REQUEST_INSTALL_PACKAGES;
    }
    else if (permission == Permissions::SendRespondViaMessage) {
        typeStr = SEND_RESPOND_VIA_MESSAGE;
    }
    else if (permission == Permissions::SendSMS) {
        typeStr = SEND_SMS;
    }
    else if (permission == Permissions::SetAlarm) {
        typeStr = SET_ALARM;
    }
    else if (permission == Permissions::SetAlwaysFinish) {
        typeStr = SET_ALWAYS_FINISH;
    }
    else if (permission == Permissions::SetAnimationScale) {
        typeStr = SET_ANIMATION_SCALE;
    }
    else if (permission == Permissions::SetDebugApp) {
        typeStr = SET_DEBUG_APP;
    }
    else if (permission == Permissions::SetProcessLimit) {
        typeStr = SET_PROCESS_LIMIT;
    }
    else if (permission == Permissions::SetTime) {
        typeStr = SET_TIME;
    }
    else if (permission == Permissions::SetTimeZone) {
        typeStr = SET_TIME_ZONE;
    }
    else if (permission == Permissions::SetWallpaper) {
        typeStr = SET_WALLPAPER;
    }
    else if (permission == Permissions::SetWallpaperHints) {
        typeStr = SET_WALLPAPER_HINTS;
    }
    else if (permission == Permissions::SignalPersistentProcesses) {
        typeStr = SIGNAL_PERSISTENT_PROCESSES;
    }
    else if (permission == Permissions::StatusBar) {
        typeStr = STATUS_BAR;
    }
    else if (permission == Permissions::SystemAlertWindow) {
        typeStr = SYSTEM_ALERT_WINDOW;
    }
    else if (permission == Permissions::TransmitIR) {
        typeStr = TRANSMIT_IR;
    }
    else if (permission == Permissions::UninstallShortcut) {
        typeStr = UNINSTALL_SHORTCUT;
    }
    else if (permission == Permissions::UpdateDeviceStats) {
        typeStr = UPDATE_DEVICE_STATS;
    }
    else if (permission == Permissions::UseBiometric) {
        typeStr = USE_BIOMETRIC;
    }
    else if (permission == Permissions::UseFingerprint) {
        typeStr = USE_FINGERPRINT;
    }
    else if (permission == Permissions::UseSIP) {
        typeStr = USE_SIP;
    }
    else if (permission == Permissions::Vibrate) {
        typeStr = VIBRATE;
    }
    else if (permission == Permissions::WakeLock) {
        typeStr = WAKE_LOCK;
    }
    else if (permission == Permissions::WriteAPNSettings) {
        typeStr = WRITE_APN_SETTINGS;
    }
    else if (permission == Permissions::WriteCalendar) {
        typeStr = WRITE_CALENDAR;
    }
    else if (permission == Permissions::WriteCallLog) {
        typeStr = WRITE_CALLLOG;
    }
    else if (permission == Permissions::WriteContacts) {
        typeStr = WRITE_CONTACTS;
    }
    else if (permission == Permissions::WriteExternalStorage) {
        typeStr = WRITE_EXTERNAL_STORAGE;
    }
    else if (permission == Permissions::WriteGServices) {
        typeStr = WRITE_GSERVICES;
    }
    else if (permission == Permissions::WriteSecureSettings) {
        typeStr = WRITE_SECURE_SETTINGS;
    }
    else if (permission == Permissions::WriteSettings) {
        typeStr = WRITE_SETTINGS;
    }
    else if (permission == Permissions::WriteSyncSettings) {
        typeStr = WRITE_SYNC_SETTINGS;
    }
    else if (permission == Permissions::WriteVoiceMail) {
        typeStr = WRITE_VOICEMAIL;
    }

    return typeStr;
}

PermissionManager::Permissions PermissionManager::permissionType(const QString &permissionStr) const
{
    Permissions type = Permissions::None;
    if (permissionStr == ACCEPT_HANDOVER) {
        type = Permissions::AcceptHandover;
    }
    else if (permissionStr == ACCESS_CHECKIN_PROPERTIES) {
        type = Permissions::AccessCheckInProperties;
    }
    else if (permissionStr == ACCESS_COARSE_LOCATION) {
        type = Permissions::AccessCoarseLocation;
    }
    else if (permissionStr == ACCESS_FINE_LOCATION) {
        type = Permissions::AccessFineLocation;
    }
    else if (permissionStr == ACCESS_LOCATION_EXTRA_COMMANDS) {
        type = Permissions::AccessLocationExtraCommands;
    }
    else if (permissionStr == ACCESS_NETWORK_STATE) {
        type = Permissions::AccessNetworkState;
    }
    else if (permissionStr == ACCESS_NOTIFICATION_POLICY) {
        type = Permissions::AccessNotificationPolicy;
    }
    else if (permissionStr == ACCESS_WIFI_STATE) {
        type = Permissions::AccessWiFiState;
    }
    else if (permissionStr == ACCOUNT_MANAGER) {
        type = Permissions::AccountManager;
    }
    else if (permissionStr == ADD_VOICEMAIL) {
        type = Permissions::AddVoiceMail;
    }
    else if (permissionStr == ANSWER_PHONE_CALLS) {
        type = Permissions::AnswerPhoneCalls;
    }
    else if (permissionStr == BATTERY_STATS) {
        type = Permissions::BatteryStats;
    }
    else if (permissionStr == BIND_ACCESSIBILITY_SERVICE) {
        type = Permissions::BindAccessibilityService;
    }
    else if (permissionStr == BIND_APPWIDGET) {
        type = Permissions::BindAppWidget;
    }
    else if (permissionStr == BIND_AUTOFILL_SERVICE) {
        type = Permissions::BindAutofillService;
    }
    else if (permissionStr == BIND_CARRIER_MESSAGING_SERVICE) {
        type = Permissions::BindCarrierMessagingService;
    }
    else if (permissionStr == BIND_CARRIER_SERVICES) {
        type = Permissions::BindCarrierServices;
    }
    else if (permissionStr == BIND_CHOOSER_TARGET_SERVICE) {
        type = Permissions::BindChooserTargetService;
    }
    else if (permissionStr == BIND_CONDITION_PROVIDER_SERVICE) {
        type = Permissions::BindConditionProviderService;
    }
    else if (permissionStr == BIND_DEVICE_ADMIN) {
        type = Permissions::BindDeviceAdmin;
    }
    else if (permissionStr == BIND_DREAM_SERVICE) {
        type = Permissions::BindDreamService;
    }
    else if (permissionStr == BIND_INCALL_SERVICE) {
        type = Permissions::BindInCallService;
    }
    else if (permissionStr == BIND_INPUT_METHOD) {
        type = Permissions::BindInputMethod;
    }
    else if (permissionStr == BIND_MIDI_DEVICE_SERVICE) {
        type = Permissions::BindMidiDeviceService;
    }
    else if (permissionStr == BIND_NFC_SERVICE) {
        type = Permissions::BindNFCService;
    }
    else if (permissionStr == BIND_NOTIFICATION_LISTENER_SERVICE) {
        type = Permissions::BindNotificationListenerService;
    }
    else if (permissionStr == BIND_PRINT_SERVICE) {
        type = Permissions::BindPrintService;
    }
    else if (permissionStr == BIND_QUICK_SETTINGS_TILE) {
        type = Permissions::BindQuickSettingsTile;
    }
    else if (permissionStr == BIND_REMOTEVIEWS) {
        type = Permissions::BindRemoteViews;
    }
    else if (permissionStr == BIND_SCREENING_SERVICE) {
        type = Permissions::BindScreeningService;
    }
    else if (permissionStr == BIND_TELECOM_CONNECTION_SERVICE) {
        type = Permissions::BindTelecomConnectionService;
    }
    else if (permissionStr == BIND_TEXT_SERVICE) {
        type = Permissions::BindTextService;
    }
    else if (permissionStr == BIND_TV_INPUT) {
        type = Permissions::BindTVInput;
    }
    else if (permissionStr == BIND_VISUAL_VOICEMAIL_SERVICE) {
        type = Permissions::BindVisualVoiceMailService;
    }
    else if (permissionStr == BIND_VOICE_INTERACTION) {
        type = Permissions::BindVoiceInteraction;
    }
    else if (permissionStr == BIND_VPN_SERVICE) {
        type = Permissions::BindVPNService;
    }
    else if (permissionStr == BIND_VR_LISTENER_SERVICE) {
        type = Permissions::BindVRListenerService;
    }
    else if (permissionStr == BIND_WALLPAPER) {
        type = Permissions::BindWallpaper;
    }
    else if (permissionStr == BLUETOOTH) {
        type = Permissions::Bluetooth;
    }
    else if (permissionStr == BLUETOOTH_ADMIN) {
        type = Permissions::BluetoothAdmin;
    }
    else if (permissionStr == BLUETOOTH_PRIVILEGED) {
        type = Permissions::BluetoothPrivileged;
    }
    else if (permissionStr == BODY_SENSORS) {
        type = Permissions::BodySensors;
    }
    else if (permissionStr == BROADCAST_PACKAGE_REMOVED) {
        type = Permissions::BroadcastPackageRemoved;
    }
    else if (permissionStr == BROADCAST_SMS) {
        type = Permissions::BroadcastSMS;
    }
    else if (permissionStr == BROADCAST_STICKY) {
        type = Permissions::BroadcastSticky;
    }
    else if (permissionStr == BROADCAST_WAP_PUSH) {
        type = Permissions::BroadcastWAPPush;
    }
    else if (permissionStr == CALL_PHONE) {
        type = Permissions::CallPhone;
    }
    else if (permissionStr == CALL_PRIVILEGED) {
        type = Permissions::CallPrivileged;
    }
    else if (permissionStr == CAMERA) {
        type = Permissions::Camera;
    }
    else if (permissionStr == CAPTURE_AUDIO_OUTPUT) {
        type = Permissions::CaptureAudioOutput;
    }
    else if (permissionStr == CAPTURE_SECURE_VIDEO_OUTPUT) {
        type = Permissions::CaptureSecureVideoOutput;
    }
    else if (permissionStr == CAPTURE_VIDEO_OUTPUT) {
        type = Permissions::CaptureVideoOutput;
    }
    else if (permissionStr == CHANGE_COMPONENT_ENABLED_STATE) {
        type = Permissions::ChangeComponentEnabledState;
    }
    else if (permissionStr == CHANGE_CONFIGURATION) {
        type = Permissions::ChangeConfiguration;
    }
    else if (permissionStr == CHANGE_NETWORK_STATE) {
        type = Permissions::ChangeNetworkState;
    }
    else if (permissionStr == CHANGE_WIFI_MULTICAST_STATE) {
        type = Permissions::ChangeWiFiMulticastState;
    }
    else if (permissionStr == CHANGE_WIFI_STATE) {
        type = Permissions::ChangeWiFiState;
    }
    else if (permissionStr == CLEAR_APP_CACHE) {
        type = Permissions::ClearAppCache;
    }
    else if (permissionStr == CONTROL_LOCATION_UPDATES) {
        type = Permissions::ControlLocationUpdates;
    }
    else if (permissionStr == DELETE_CACHE_FILES) {
        type = Permissions::DeleteCacheFiles;
    }
    else if (permissionStr == DELETE_PACKAGES) {
        type = Permissions::DeletePackages;
    }
    else if (permissionStr == DIAGNOSTIC) {
        type = Permissions::Diagnostic;
    }
    else if (permissionStr == DISABLE_KEYGUARD) {
        type = Permissions::DisableKeyguard;
    }
    else if (permissionStr == DUMP) {
        type = Permissions::Dump;
    }
    else if (permissionStr == EXPAND_STATUS_BAR) {
        type = Permissions::ExpandStatusBar;
    }
    else if (permissionStr == FACTORY_TEST) {
        type = Permissions::FactoryTest;
    }
    else if (permissionStr == FOREGROUND_SERVICE) {
        type = Permissions::ForegroundService;
    }
    else if (permissionStr == GET_ACCOUNTS) {
        type = Permissions::GetAccounts;
    }
    else if (permissionStr == GET_ACCOUNTS_PRIVILEGED) {
        type = Permissions::GetAccountsPrivileged;
    }
    else if (permissionStr == GET_PACKAGE_SIZE) {
        type = Permissions::GetPackageSize;
    }
    else if (permissionStr == GET_TASKS) {
        type = Permissions::GetTasks;
    }
    else if (permissionStr == GLOBAL_SEARCH) {
        type = Permissions::GlobalSearch;
    }
    else if (permissionStr == INSTALL_LOCATION_PROVIDER) {
        type = Permissions::InstallLocationProvider;
    }
    else if (permissionStr == INSTALL_PACKAGES) {
        type = Permissions::InstallPackages;
    }
    else if (permissionStr == INSTALL_SHORTCUT) {
        type = Permissions::InstallShortcut;
    }
    else if (permissionStr == INSTANT_APP_FOREGROUND_SERVICE) {
        type = Permissions::InstantAppForegroundService;
    }
    else if (permissionStr == INTERNET) {
        type = Permissions::Internet;
    }
    else if (permissionStr == KILL_BACKGROUND_PROCESSES) {
        type = Permissions::KillBackgroundProcesses;
    }
    else if (permissionStr == LOCATION_HARDWARE) {
        type = Permissions::LocationHardware;
    }
    else if (permissionStr == MANAGE_DOCUMENTS) {
        type = Permissions::ManageDocuments;
    }
    else if (permissionStr == MANAGE_OWN_CALLS) {
        type = Permissions::ManageOwnCalls;
    }
    else if (permissionStr == MEDIA_CONTENT_CONTROL) {
        type = Permissions::MediaContentControl;
    }
    else if (permissionStr == MODIFY_AUDIO_SETTINGS) {
        type = Permissions::ModifyAudioSettings;
    }
    else if (permissionStr == MODIFY_PHONE_STATE) {
        type = Permissions::ModifyPhoneState;
    }
    else if (permissionStr == MOUNT_FORMAT_FILESYSTEMS) {
        type = Permissions::MountFormatFileSystems;
    }
    else if (permissionStr == MOUNT_UNMOUNT_FILESYSTEMS) {
        type = Permissions::MountUnmountFileSystems;
    }
    else if (permissionStr == NFC_PERM) {
        type = Permissions::NFC;
    }
    else if (permissionStr == NFC_TRANSACTION_EVENT) {
        type = Permissions::NFCTransactionEvent;
    }
    else if (permissionStr == PACKAGE_USAGE_STATS) {
        type = Permissions::PackageUsageStats;
    }
    else if (permissionStr == PROCESS_OUTGOING_CALLS) {
        type = Permissions::ProcessOutgoingCalls;
    }
    else if (permissionStr == READ_CALENDAR) {
        type = Permissions::Calendar;
    }
    else if (permissionStr == READ_CALL_LOG) {
        type = Permissions::ReadCallLog;
    }
    else if (permissionStr == READ_CONTACTS) {
        type = Permissions::Contacts;
    }
    else if (permissionStr == READ_EXTERNAL_STORAGE) {
        type = Permissions::ReadExternalStorage;
    }
    else if (permissionStr == READ_FRAME_BUFFER) {
        type = Permissions::ReadFrameBuffer;
    }
    else if (permissionStr == READ_LOGS) {
        type = Permissions::ReadLogs;
    }
    else if (permissionStr == READ_PHONE_NUMBERS) {
        type = Permissions::ReadPhoneNumbers;
    }
    else if (permissionStr == READ_PHONE_STATE) {
        type = Permissions::ReadPhoneState;
    }
    else if (permissionStr == READ_SMS) {
        type = Permissions::ReadSMS;
    }
    else if (permissionStr == READ_SYNC_SETTINGS) {
        type = Permissions::ReadSyncSettings;
    }
    else if (permissionStr == READ_SYNC_STATS) {
        type = Permissions::ReadSyncStats;
    }
    else if (permissionStr == READ_VOICEMAIL) {
        type = Permissions::ReadVoicemail;
    }
    else if (permissionStr == REBOOT) {
        type = Permissions::Reboot;
    }
    else if (permissionStr == RECEIVE_BOOT_COMPLETED) {
        type = Permissions::ReceiveBootCompleted;
    }
    else if (permissionStr == RECEIVE_MMS) {
        type = Permissions::ReceiveMMS;
    }
    else if (permissionStr == RECEIVE_SMS) {
        type = Permissions::ReceiveSMS;
    }
    else if (permissionStr == RECEIVE_WAP_PUSH) {
        type = Permissions::ReceiveWAPPush;
    }
    else if (permissionStr == RECORD_AUDIO) {
        type = Permissions::Microphone;
    }
    else if (permissionStr == REORDER_TASKS) {
        type = Permissions::ReorderTasks;
    }
    else if (permissionStr == REQUEST_COMPANION_RUN_IN_BACKGROUND) {
        type = Permissions::RequestCompanionRunInBackground;
    }
    else if (permissionStr == REQUEST_COMPANION_USE_DATA_IN_BACKGROUND) {
        type = Permissions::RequestCompanionUseDataInBackground;
    }
    else if (permissionStr == REQUEST_DELETE_PACKAGES) {
        type = Permissions::RequestDeletePackages;
    }
    else if (permissionStr == REQUEST_IGNORE_BATTERY_OPTIMIZATION) {
        type = Permissions::RequestIgnoreBatteryOptimization;
    }
    else if (permissionStr == REQUEST_INSTALL_PACKAGES) {
        type = Permissions::RequestInstallPackages;
    }
    else if (permissionStr == SEND_RESPOND_VIA_MESSAGE) {
        type = Permissions::SendRespondViaMessage;
    }
    else if (permissionStr == SEND_SMS) {
        type = Permissions::SendSMS;
    }
    else if (permissionStr == SET_ALARM) {
        type = Permissions::SetAlarm;
    }
    else if (permissionStr == SET_ALWAYS_FINISH) {
        type = Permissions::SetAlwaysFinish;
    }
    else if (permissionStr == SET_ANIMATION_SCALE) {
        type = Permissions::SetAnimationScale;
    }
    else if (permissionStr == SET_DEBUG_APP) {
        type = Permissions::SetDebugApp;
    }
    else if (permissionStr == SET_PROCESS_LIMIT) {
        type = Permissions::SetProcessLimit;
    }
    else if (permissionStr == SET_TIME) {
        type = Permissions::SetTime;
    }
    else if (permissionStr == SET_TIME_ZONE) {
        type = Permissions::SetTimeZone;
    }
    else if (permissionStr == SET_WALLPAPER) {
        type = Permissions::SetWallpaper;
    }
    else if (permissionStr == SET_WALLPAPER_HINTS) {
        type = Permissions::SetWallpaperHints;
    }
    else if (permissionStr == SIGNAL_PERSISTENT_PROCESSES) {
        type = Permissions::SignalPersistentProcesses;
    }
    else if (permissionStr == STATUS_BAR) {
        type = Permissions::StatusBar;
    }
    else if (permissionStr == SYSTEM_ALERT_WINDOW) {
        type = Permissions::SystemAlertWindow;
    }
    else if (permissionStr == TRANSMIT_IR) {
        type = Permissions::TransmitIR;
    }
    else if (permissionStr == UNINSTALL_SHORTCUT) {
        type = Permissions::UninstallShortcut;
    }
    else if (permissionStr == UPDATE_DEVICE_STATS) {
        type = Permissions::UpdateDeviceStats;
    }
    else if (permissionStr == USE_BIOMETRIC) {
        type = Permissions::UseBiometric;
    }
    else if (permissionStr == USE_FINGERPRINT) {
        type = Permissions::UseFingerprint;
    }
    else if (permissionStr == USE_SIP) {
        type = Permissions::UseSIP;
    }
    else if (permissionStr == VIBRATE) {
        type = Permissions::Vibrate;
    }
    else if (permissionStr == WAKE_LOCK) {
        type = Permissions::WakeLock;
    }
    else if (permissionStr == WRITE_APN_SETTINGS) {
        type = Permissions::WriteAPNSettings;
    }
    else if (permissionStr == WRITE_CALENDAR) {
        type = Permissions::WriteCalendar;
    }
    else if (permissionStr == WRITE_CALLLOG) {
        type = Permissions::WriteCallLog;
    }
    else if (permissionStr == WRITE_CONTACTS) {
        type = Permissions::WriteContacts;
    }
    else if (permissionStr == WRITE_EXTERNAL_STORAGE) {
        type = Permissions::WriteExternalStorage;
    }
    else if (permissionStr == WRITE_GSERVICES) {
        type = Permissions::WriteGServices;
    }
    else if (permissionStr == WRITE_SECURE_SETTINGS) {
        type = Permissions::WriteSecureSettings;
    }
    else if (permissionStr == WRITE_SETTINGS) {
        type = Permissions::WriteSettings;
    }
    else if (permissionStr == WRITE_SYNC_SETTINGS) {
        type = Permissions::WriteSyncSettings;
    }
    else if (permissionStr == WRITE_VOICEMAIL) {
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
        emit permissionResultReceived(static_cast<int>(permissionType(keys.at(0))),
            results[keys.at(0)] == QtAndroid::PermissionResult::Granted ? Result::Granted : Result::Denied);
    }
    else {
        LOG_ERROR("The request for permission resulted in 0 results.");
    }
}
#endif // Q_OS_ANDROID

}
