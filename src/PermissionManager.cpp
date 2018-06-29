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
    , m_AndroidPermissionMap(getPermissionsMap())
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

    try {
        typeStr = m_AndroidPermissionMap.at(permission);
    }
    catch (std::out_of_range ex) {
        Q_UNUSED(ex);
        LOG_WARNING("String for the given permission type cannot be found.");
    }

    return typeStr;
}

PermissionManager::Permissions PermissionManager::permissionType(const QString &permissionStr) const
{
    Permissions type = Permissions::None;
    auto foundIt = std::find_if(m_AndroidPermissionMap.cbegin(), m_AndroidPermissionMap.cend(), [&permissionStr](const std::pair<Permissions, const char *> &pair) {
        return pair.second == permissionStr;
    });

    if (foundIt != m_AndroidPermissionMap.cend()) {
        type = (*foundIt).first;
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

PermissionManager::AndroidPermissionMap PermissionManager::getPermissionsMap() const
{
    return {
        {Permissions::AcceptHandover, AndroidPermNames::ACCEPT_HANDOVER},
        {Permissions::AccessCheckInProperties, AndroidPermNames::ACCESS_CHECKIN_PROPERTIES},
        {Permissions::AccessCoarseLocation, AndroidPermNames::ACCESS_COARSE_LOCATION},
        {Permissions::AccessFineLocation, AndroidPermNames::ACCESS_FINE_LOCATION},
        {Permissions::AccessLocationExtraCommands, AndroidPermNames::ACCESS_LOCATION_EXTRA_COMMANDS},
        {Permissions::AccessNetworkState, AndroidPermNames::ACCESS_NETWORK_STATE},
        {Permissions::AccessNotificationPolicy, AndroidPermNames::ACCESS_NOTIFICATION_POLICY},
        {Permissions::AccessWiFiState, AndroidPermNames::ACCESS_WIFI_STATE},
        {Permissions::AccountManager, AndroidPermNames::ACCOUNT_MANAGER},
        {Permissions::AddVoiceMail, AndroidPermNames::ADD_VOICEMAIL},
        {Permissions::AnswerPhoneCalls, AndroidPermNames::ANSWER_PHONE_CALLS},
        {Permissions::BatteryStats, AndroidPermNames::BATTERY_STATS},
        {Permissions::BindAccessibilityService, AndroidPermNames::BIND_ACCESSIBILITY_SERVICE},
        {Permissions::BindAppWidget, AndroidPermNames::BIND_APPWIDGET},
        {Permissions::BindAutofillService, AndroidPermNames::BIND_AUTOFILL_SERVICE},
        {Permissions::BindCarrierMessagingService, AndroidPermNames::BIND_CARRIER_MESSAGING_SERVICE},
        {Permissions::BindCarrierServices, AndroidPermNames::BIND_CARRIER_SERVICES},
        {Permissions::BindChooserTargetService, AndroidPermNames::BIND_CHOOSER_TARGET_SERVICE},
        {Permissions::BindConditionProviderService, AndroidPermNames::BIND_CONDITION_PROVIDER_SERVICE},
        {Permissions::BindDeviceAdmin, AndroidPermNames::BIND_DEVICE_ADMIN},
        {Permissions::BindDreamService, AndroidPermNames::BIND_DREAM_SERVICE},
        {Permissions::BindInCallService, AndroidPermNames::BIND_INCALL_SERVICE},
        {Permissions::BindInputMethod, AndroidPermNames::BIND_INPUT_METHOD},
        {Permissions::BindMidiDeviceService, AndroidPermNames::BIND_MIDI_DEVICE_SERVICE},
        {Permissions::BindNFCService, AndroidPermNames::BIND_NFC_SERVICE},
        {Permissions::BindNotificationListenerService, AndroidPermNames::BIND_NOTIFICATION_LISTENER_SERVICE},
        {Permissions::BindPrintService, AndroidPermNames::BIND_PRINT_SERVICE},
        {Permissions::BindQuickSettingsTile, AndroidPermNames::BIND_QUICK_SETTINGS_TILE},
        {Permissions::BindRemoteViews, AndroidPermNames::BIND_REMOTEVIEWS},
        {Permissions::BindScreeningService, AndroidPermNames::BIND_SCREENING_SERVICE},
        {Permissions::BindTelecomConnectionService, AndroidPermNames::BIND_TELECOM_CONNECTION_SERVICE},
        {Permissions::BindTextService, AndroidPermNames::BIND_TEXT_SERVICE},
        {Permissions::BindTVInput, AndroidPermNames::BIND_TV_INPUT},
        {Permissions::BindVisualVoiceMailService, AndroidPermNames::BIND_VISUAL_VOICEMAIL_SERVICE},
        {Permissions::BindVoiceInteraction, AndroidPermNames::BIND_VOICE_INTERACTION},
        {Permissions::BindVPNService, AndroidPermNames::BIND_VPN_SERVICE},
        {Permissions::BindVRListenerService, AndroidPermNames::BIND_VR_LISTENER_SERVICE},
        {Permissions::BindWallpaper, AndroidPermNames::BIND_WALLPAPER},
        {Permissions::Bluetooth, AndroidPermNames::BLUETOOTH},
        {Permissions::BluetoothAdmin, AndroidPermNames::BLUETOOTH_ADMIN},
        {Permissions::BluetoothPrivileged, AndroidPermNames::BLUETOOTH_PRIVILEGED},
        {Permissions::BodySensors, AndroidPermNames::BODY_SENSORS},
        {Permissions::BroadcastPackageRemoved, AndroidPermNames::BROADCAST_PACKAGE_REMOVED},
        {Permissions::BroadcastSMS, AndroidPermNames::BROADCAST_SMS},
        {Permissions::BroadcastSticky, AndroidPermNames::BROADCAST_STICKY},
        {Permissions::BroadcastWAPPush, AndroidPermNames::BROADCAST_WAP_PUSH},
        {Permissions::CallPhone, AndroidPermNames::CALL_PHONE},
        {Permissions::CallPrivileged, AndroidPermNames::CALL_PRIVILEGED},
        {Permissions::Camera, AndroidPermNames::CAMERA},
        {Permissions::CaptureAudioOutput, AndroidPermNames::CAPTURE_AUDIO_OUTPUT},
        {Permissions::CaptureSecureVideoOutput, AndroidPermNames::CAPTURE_SECURE_VIDEO_OUTPUT},
        {Permissions::CaptureVideoOutput, AndroidPermNames::CAPTURE_VIDEO_OUTPUT},
        {Permissions::ChangeComponentEnabledState, AndroidPermNames::CHANGE_COMPONENT_ENABLED_STATE},
        {Permissions::ChangeConfiguration, AndroidPermNames::CHANGE_CONFIGURATION},
        {Permissions::ChangeNetworkState, AndroidPermNames::CHANGE_NETWORK_STATE},
        {Permissions::ChangeWiFiMulticastState, AndroidPermNames::CHANGE_WIFI_MULTICAST_STATE},
        {Permissions::ChangeWiFiState, AndroidPermNames::CHANGE_WIFI_STATE},
        {Permissions::ClearAppCache, AndroidPermNames::CLEAR_APP_CACHE},
        {Permissions::ControlLocationUpdates, AndroidPermNames::CONTROL_LOCATION_UPDATES},
        {Permissions::DeleteCacheFiles, AndroidPermNames::DELETE_CACHE_FILES},
        {Permissions::DeletePackages, AndroidPermNames::DELETE_PACKAGES},
        {Permissions::Diagnostic, AndroidPermNames::DIAGNOSTIC},
        {Permissions::DisableKeyguard, AndroidPermNames::DISABLE_KEYGUARD},
        {Permissions::Dump, AndroidPermNames::DUMP},
        {Permissions::ExpandStatusBar, AndroidPermNames::EXPAND_STATUS_BAR},
        {Permissions::FactoryTest, AndroidPermNames::FACTORY_TEST},
        {Permissions::ForegroundService, AndroidPermNames::FOREGROUND_SERVICE},
        {Permissions::GetAccounts, AndroidPermNames::GET_ACCOUNTS},
        {Permissions::GetAccountsPrivileged, AndroidPermNames::GET_ACCOUNTS_PRIVILEGED},
        {Permissions::GetPackageSize, AndroidPermNames::GET_PACKAGE_SIZE},
        {Permissions::GetTasks, AndroidPermNames::GET_TASKS},
        {Permissions::GlobalSearch, AndroidPermNames::GLOBAL_SEARCH},
        {Permissions::InstallLocationProvider, AndroidPermNames::INSTALL_LOCATION_PROVIDER},
        {Permissions::InstallPackages, AndroidPermNames::INSTALL_PACKAGES},
        {Permissions::InstallShortcut, AndroidPermNames::INSTALL_SHORTCUT},
        {Permissions::InstantAppForegroundService, AndroidPermNames::INSTANT_APP_FOREGROUND_SERVICE},
        {Permissions::Internet, AndroidPermNames::INTERNET},
        {Permissions::KillBackgroundProcesses, AndroidPermNames::KILL_BACKGROUND_PROCESSES},
        {Permissions::LocationHardware, AndroidPermNames::LOCATION_HARDWARE},
        {Permissions::ManageDocuments, AndroidPermNames::MANAGE_DOCUMENTS},
        {Permissions::ManageOwnCalls, AndroidPermNames::MANAGE_OWN_CALLS},
        {Permissions::MediaContentControl, AndroidPermNames::MEDIA_CONTENT_CONTROL},
        {Permissions::ModifyAudioSettings, AndroidPermNames::MODIFY_AUDIO_SETTINGS},
        {Permissions::ModifyPhoneState, AndroidPermNames::MODIFY_PHONE_STATE},
        {Permissions::MountFormatFileSystems, AndroidPermNames::MOUNT_FORMAT_FILESYSTEMS},
        {Permissions::MountUnmountFileSystems, AndroidPermNames::MOUNT_UNMOUNT_FILESYSTEMS},
        {Permissions::NFC, AndroidPermNames::NFC_PERM},
        {Permissions::NFCTransactionEvent, AndroidPermNames::NFC_TRANSACTION_EVENT},
        {Permissions::PackageUsageStats, AndroidPermNames::PACKAGE_USAGE_STATS},
        {Permissions::ProcessOutgoingCalls, AndroidPermNames::PROCESS_OUTGOING_CALLS},
        {Permissions::Calendar, AndroidPermNames::READ_CALENDAR},
        {Permissions::ReadCallLog, AndroidPermNames::READ_CALL_LOG},
        {Permissions::Contacts, AndroidPermNames::READ_CONTACTS},
        {Permissions::ReadExternalStorage, AndroidPermNames::READ_EXTERNAL_STORAGE},
        {Permissions::ReadFrameBuffer, AndroidPermNames::READ_FRAME_BUFFER},
        {Permissions::ReadLogs, AndroidPermNames::READ_LOGS},
        {Permissions::ReadPhoneNumbers, AndroidPermNames::READ_PHONE_NUMBERS},
        {Permissions::ReadPhoneState, AndroidPermNames::READ_PHONE_STATE},
        {Permissions::ReadSMS, AndroidPermNames::READ_SMS},
        {Permissions::ReadSyncSettings, AndroidPermNames::READ_SYNC_SETTINGS},
        {Permissions::ReadSyncStats, AndroidPermNames::READ_SYNC_STATS},
        {Permissions::ReadVoicemail, AndroidPermNames::READ_VOICEMAIL},
        {Permissions::Reboot, AndroidPermNames::REBOOT},
        {Permissions::ReceiveBootCompleted, AndroidPermNames::RECEIVE_BOOT_COMPLETED},
        {Permissions::ReceiveMMS, AndroidPermNames::RECEIVE_MMS},
        {Permissions::ReceiveSMS, AndroidPermNames::RECEIVE_SMS},
        {Permissions::ReceiveWAPPush, AndroidPermNames::RECEIVE_WAP_PUSH},
        {Permissions::Microphone, AndroidPermNames::RECORD_AUDIO},
        {Permissions::ReorderTasks, AndroidPermNames::REORDER_TASKS},
        {Permissions::RequestCompanionRunInBackground, AndroidPermNames::REQUEST_COMPANION_RUN_IN_BACKGROUND},
        {Permissions::RequestCompanionUseDataInBackground, AndroidPermNames::REQUEST_COMPANION_USE_DATA_IN_BACKGROUND},
        {Permissions::RequestDeletePackages, AndroidPermNames::REQUEST_DELETE_PACKAGES},
        {Permissions::RequestIgnoreBatteryOptimization, AndroidPermNames::REQUEST_IGNORE_BATTERY_OPTIMIZATION},
        {Permissions::RequestInstallPackages, AndroidPermNames::REQUEST_INSTALL_PACKAGES},
        {Permissions::SendRespondViaMessage, AndroidPermNames::SEND_RESPOND_VIA_MESSAGE},
        {Permissions::SendSMS, AndroidPermNames::SEND_SMS},
        {Permissions::SetAlarm, AndroidPermNames::SET_ALARM},
        {Permissions::SetAlwaysFinish, AndroidPermNames::SET_ALWAYS_FINISH},
        {Permissions::SetAnimationScale, AndroidPermNames::SET_ANIMATION_SCALE},
        {Permissions::SetDebugApp, AndroidPermNames::SET_DEBUG_APP},
        {Permissions::SetProcessLimit, AndroidPermNames::SET_PROCESS_LIMIT},
        {Permissions::SetTime, AndroidPermNames::SET_TIME},
        {Permissions::SetTimeZone, AndroidPermNames::SET_TIME_ZONE},
        {Permissions::SetWallpaper, AndroidPermNames::SET_WALLPAPER},
        {Permissions::SetWallpaperHints, AndroidPermNames::SET_WALLPAPER_HINTS},
        {Permissions::SignalPersistentProcesses, AndroidPermNames::SIGNAL_PERSISTENT_PROCESSES},
        {Permissions::StatusBar, AndroidPermNames::STATUS_BAR},
        {Permissions::SystemAlertWindow, AndroidPermNames::SYSTEM_ALERT_WINDOW},
        {Permissions::TransmitIR, AndroidPermNames::TRANSMIT_IR},
        {Permissions::UninstallShortcut, AndroidPermNames::UNINSTALL_SHORTCUT},
        {Permissions::UpdateDeviceStats, AndroidPermNames::UPDATE_DEVICE_STATS},
        {Permissions::UseBiometric, AndroidPermNames::USE_BIOMETRIC},
        {Permissions::UseFingerprint, AndroidPermNames::USE_FINGERPRINT},
        {Permissions::UseSIP, AndroidPermNames::USE_SIP},
        {Permissions::Vibrate, AndroidPermNames::VIBRATE},
        {Permissions::WakeLock, AndroidPermNames::WAKE_LOCK},
        {Permissions::WriteAPNSettings, AndroidPermNames::WRITE_APN_SETTINGS},
        {Permissions::WriteCalendar, AndroidPermNames::WRITE_CALENDAR},
        {Permissions::WriteCallLog, AndroidPermNames::WRITE_CALLLOG},
        {Permissions::WriteContacts, AndroidPermNames::WRITE_CONTACTS},
        {Permissions::WriteExternalStorage, AndroidPermNames::WRITE_EXTERNAL_STORAGE},
        {Permissions::WriteGServices, AndroidPermNames::WRITE_GSERVICES},
        {Permissions::WriteSecureSettings, AndroidPermNames::WRITE_SECURE_SETTINGS},
        {Permissions::WriteSettings, AndroidPermNames::WRITE_SETTINGS},
        {Permissions::WriteSyncSettings, AndroidPermNames::WRITE_SYNC_SETTINGS},
        {Permissions::WriteVoiceMail, AndroidPermNames::WRITE_VOICEMAIL}
    };
}

}
