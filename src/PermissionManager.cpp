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
    auto foundIt = std::find_if(m_AndroidPermissionMap.cbegin(), m_AndroidPermissionMap.cend(), [&permissionStr](const std::pair<int, const char *> &pair) {
        return pair.second == permissionStr;
    });

    if (foundIt != m_AndroidPermissionMap.cend()) {
        type = static_cast<Permissions>((*foundIt).first);
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
        {static_cast<int>(Permissions::AcceptHandover), AndroidPermNames::ACCEPT_HANDOVER},
        {static_cast<int>(Permissions::AccessCheckInProperties), AndroidPermNames::ACCESS_CHECKIN_PROPERTIES},
        {static_cast<int>(Permissions::AccessCoarseLocation), AndroidPermNames::ACCESS_COARSE_LOCATION},
        {static_cast<int>(Permissions::AccessFineLocation), AndroidPermNames::ACCESS_FINE_LOCATION},
        {static_cast<int>(Permissions::AccessLocationExtraCommands), AndroidPermNames::ACCESS_LOCATION_EXTRA_COMMANDS},
        {static_cast<int>(Permissions::AccessNetworkState), AndroidPermNames::ACCESS_NETWORK_STATE},
        {static_cast<int>(Permissions::AccessNotificationPolicy), AndroidPermNames::ACCESS_NOTIFICATION_POLICY},
        {static_cast<int>(Permissions::AccessWiFiState), AndroidPermNames::ACCESS_WIFI_STATE},
        {static_cast<int>(Permissions::AccountManager), AndroidPermNames::ACCOUNT_MANAGER},
        {static_cast<int>(Permissions::AddVoiceMail), AndroidPermNames::ADD_VOICEMAIL},
        {static_cast<int>(Permissions::AnswerPhoneCalls), AndroidPermNames::ANSWER_PHONE_CALLS},
        {static_cast<int>(Permissions::BatteryStats), AndroidPermNames::BATTERY_STATS},
        {static_cast<int>(Permissions::BindAccessibilityService), AndroidPermNames::BIND_ACCESSIBILITY_SERVICE},
        {static_cast<int>(Permissions::BindAppWidget), AndroidPermNames::BIND_APPWIDGET},
        {static_cast<int>(Permissions::BindAutofillService), AndroidPermNames::BIND_AUTOFILL_SERVICE},
        {static_cast<int>(Permissions::BindCarrierMessagingService), AndroidPermNames::BIND_CARRIER_MESSAGING_SERVICE},
        {static_cast<int>(Permissions::BindCarrierServices), AndroidPermNames::BIND_CARRIER_SERVICES},
        {static_cast<int>(Permissions::BindChooserTargetService), AndroidPermNames::BIND_CHOOSER_TARGET_SERVICE},
        {static_cast<int>(Permissions::BindConditionProviderService), AndroidPermNames::BIND_CONDITION_PROVIDER_SERVICE},
        {static_cast<int>(Permissions::BindDeviceAdmin), AndroidPermNames::BIND_DEVICE_ADMIN},
        {static_cast<int>(Permissions::BindDreamService), AndroidPermNames::BIND_DREAM_SERVICE},
        {static_cast<int>(Permissions::BindInCallService), AndroidPermNames::BIND_INCALL_SERVICE},
        {static_cast<int>(Permissions::BindInputMethod), AndroidPermNames::BIND_INPUT_METHOD},
        {static_cast<int>(Permissions::BindMidiDeviceService), AndroidPermNames::BIND_MIDI_DEVICE_SERVICE},
        {static_cast<int>(Permissions::BindNFCService), AndroidPermNames::BIND_NFC_SERVICE},
        {static_cast<int>(Permissions::BindNotificationListenerService), AndroidPermNames::BIND_NOTIFICATION_LISTENER_SERVICE},
        {static_cast<int>(Permissions::BindPrintService), AndroidPermNames::BIND_PRINT_SERVICE},
        {static_cast<int>(Permissions::BindQuickSettingsTile), AndroidPermNames::BIND_QUICK_SETTINGS_TILE},
        {static_cast<int>(Permissions::BindRemoteViews), AndroidPermNames::BIND_REMOTEVIEWS},
        {static_cast<int>(Permissions::BindScreeningService), AndroidPermNames::BIND_SCREENING_SERVICE},
        {static_cast<int>(Permissions::BindTelecomConnectionService), AndroidPermNames::BIND_TELECOM_CONNECTION_SERVICE},
        {static_cast<int>(Permissions::BindTextService), AndroidPermNames::BIND_TEXT_SERVICE},
        {static_cast<int>(Permissions::BindTVInput), AndroidPermNames::BIND_TV_INPUT},
        {static_cast<int>(Permissions::BindVisualVoiceMailService), AndroidPermNames::BIND_VISUAL_VOICEMAIL_SERVICE},
        {static_cast<int>(Permissions::BindVoiceInteraction), AndroidPermNames::BIND_VOICE_INTERACTION},
        {static_cast<int>(Permissions::BindVPNService), AndroidPermNames::BIND_VPN_SERVICE},
        {static_cast<int>(Permissions::BindVRListenerService), AndroidPermNames::BIND_VR_LISTENER_SERVICE},
        {static_cast<int>(Permissions::BindWallpaper), AndroidPermNames::BIND_WALLPAPER},
        {static_cast<int>(Permissions::Bluetooth), AndroidPermNames::BLUETOOTH},
        {static_cast<int>(Permissions::BluetoothAdmin), AndroidPermNames::BLUETOOTH_ADMIN},
        {static_cast<int>(Permissions::BluetoothPrivileged), AndroidPermNames::BLUETOOTH_PRIVILEGED},
        {static_cast<int>(Permissions::BodySensors), AndroidPermNames::BODY_SENSORS},
        {static_cast<int>(Permissions::BroadcastPackageRemoved), AndroidPermNames::BROADCAST_PACKAGE_REMOVED},
        {static_cast<int>(Permissions::BroadcastSMS), AndroidPermNames::BROADCAST_SMS},
        {static_cast<int>(Permissions::BroadcastSticky), AndroidPermNames::BROADCAST_STICKY},
        {static_cast<int>(Permissions::BroadcastWAPPush), AndroidPermNames::BROADCAST_WAP_PUSH},
        {static_cast<int>(Permissions::CallPhone), AndroidPermNames::CALL_PHONE},
        {static_cast<int>(Permissions::CallPrivileged), AndroidPermNames::CALL_PRIVILEGED},
        {static_cast<int>(Permissions::Camera), AndroidPermNames::CAMERA},
        {static_cast<int>(Permissions::CaptureAudioOutput), AndroidPermNames::CAPTURE_AUDIO_OUTPUT},
        {static_cast<int>(Permissions::CaptureSecureVideoOutput), AndroidPermNames::CAPTURE_SECURE_VIDEO_OUTPUT},
        {static_cast<int>(Permissions::CaptureVideoOutput), AndroidPermNames::CAPTURE_VIDEO_OUTPUT},
        {static_cast<int>(Permissions::ChangeComponentEnabledState), AndroidPermNames::CHANGE_COMPONENT_ENABLED_STATE},
        {static_cast<int>(Permissions::ChangeConfiguration), AndroidPermNames::CHANGE_CONFIGURATION},
        {static_cast<int>(Permissions::ChangeNetworkState), AndroidPermNames::CHANGE_NETWORK_STATE},
        {static_cast<int>(Permissions::ChangeWiFiMulticastState), AndroidPermNames::CHANGE_WIFI_MULTICAST_STATE},
        {static_cast<int>(Permissions::ChangeWiFiState), AndroidPermNames::CHANGE_WIFI_STATE},
        {static_cast<int>(Permissions::ClearAppCache), AndroidPermNames::CLEAR_APP_CACHE},
        {static_cast<int>(Permissions::ControlLocationUpdates), AndroidPermNames::CONTROL_LOCATION_UPDATES},
        {static_cast<int>(Permissions::DeleteCacheFiles), AndroidPermNames::DELETE_CACHE_FILES},
        {static_cast<int>(Permissions::DeletePackages), AndroidPermNames::DELETE_PACKAGES},
        {static_cast<int>(Permissions::Diagnostic), AndroidPermNames::DIAGNOSTIC},
        {static_cast<int>(Permissions::DisableKeyguard), AndroidPermNames::DISABLE_KEYGUARD},
        {static_cast<int>(Permissions::Dump), AndroidPermNames::DUMP},
        {static_cast<int>(Permissions::ExpandStatusBar), AndroidPermNames::EXPAND_STATUS_BAR},
        {static_cast<int>(Permissions::FactoryTest), AndroidPermNames::FACTORY_TEST},
        {static_cast<int>(Permissions::ForegroundService), AndroidPermNames::FOREGROUND_SERVICE},
        {static_cast<int>(Permissions::GetAccounts), AndroidPermNames::GET_ACCOUNTS},
        {static_cast<int>(Permissions::GetAccountsPrivileged), AndroidPermNames::GET_ACCOUNTS_PRIVILEGED},
        {static_cast<int>(Permissions::GetPackageSize), AndroidPermNames::GET_PACKAGE_SIZE},
        {static_cast<int>(Permissions::GetTasks), AndroidPermNames::GET_TASKS},
        {static_cast<int>(Permissions::GlobalSearch), AndroidPermNames::GLOBAL_SEARCH},
        {static_cast<int>(Permissions::InstallLocationProvider), AndroidPermNames::INSTALL_LOCATION_PROVIDER},
        {static_cast<int>(Permissions::InstallPackages), AndroidPermNames::INSTALL_PACKAGES},
        {static_cast<int>(Permissions::InstallShortcut), AndroidPermNames::INSTALL_SHORTCUT},
        {static_cast<int>(Permissions::InstantAppForegroundService), AndroidPermNames::INSTANT_APP_FOREGROUND_SERVICE},
        {static_cast<int>(Permissions::Internet), AndroidPermNames::INTERNET},
        {static_cast<int>(Permissions::KillBackgroundProcesses), AndroidPermNames::KILL_BACKGROUND_PROCESSES},
        {static_cast<int>(Permissions::LocationHardware), AndroidPermNames::LOCATION_HARDWARE},
        {static_cast<int>(Permissions::ManageDocuments), AndroidPermNames::MANAGE_DOCUMENTS},
        {static_cast<int>(Permissions::ManageOwnCalls), AndroidPermNames::MANAGE_OWN_CALLS},
        {static_cast<int>(Permissions::MediaContentControl), AndroidPermNames::MEDIA_CONTENT_CONTROL},
        {static_cast<int>(Permissions::ModifyAudioSettings), AndroidPermNames::MODIFY_AUDIO_SETTINGS},
        {static_cast<int>(Permissions::ModifyPhoneState), AndroidPermNames::MODIFY_PHONE_STATE},
        {static_cast<int>(Permissions::MountFormatFileSystems), AndroidPermNames::MOUNT_FORMAT_FILESYSTEMS},
        {static_cast<int>(Permissions::MountUnmountFileSystems), AndroidPermNames::MOUNT_UNMOUNT_FILESYSTEMS},
        {static_cast<int>(Permissions::NFC), AndroidPermNames::NFC_PERM},
        {static_cast<int>(Permissions::NFCTransactionEvent), AndroidPermNames::NFC_TRANSACTION_EVENT},
        {static_cast<int>(Permissions::PackageUsageStats), AndroidPermNames::PACKAGE_USAGE_STATS},
        {static_cast<int>(Permissions::ProcessOutgoingCalls), AndroidPermNames::PROCESS_OUTGOING_CALLS},
        {static_cast<int>(Permissions::Calendar), AndroidPermNames::READ_CALENDAR},
        {static_cast<int>(Permissions::ReadCallLog), AndroidPermNames::READ_CALL_LOG},
        {static_cast<int>(Permissions::Contacts), AndroidPermNames::READ_CONTACTS},
        {static_cast<int>(Permissions::ReadExternalStorage), AndroidPermNames::READ_EXTERNAL_STORAGE},
        {static_cast<int>(Permissions::ReadFrameBuffer), AndroidPermNames::READ_FRAME_BUFFER},
        {static_cast<int>(Permissions::ReadLogs), AndroidPermNames::READ_LOGS},
        {static_cast<int>(Permissions::ReadPhoneNumbers), AndroidPermNames::READ_PHONE_NUMBERS},
        {static_cast<int>(Permissions::ReadPhoneState), AndroidPermNames::READ_PHONE_STATE},
        {static_cast<int>(Permissions::ReadSMS), AndroidPermNames::READ_SMS},
        {static_cast<int>(Permissions::ReadSyncSettings), AndroidPermNames::READ_SYNC_SETTINGS},
        {static_cast<int>(Permissions::ReadSyncStats), AndroidPermNames::READ_SYNC_STATS},
        {static_cast<int>(Permissions::ReadVoicemail), AndroidPermNames::READ_VOICEMAIL},
        {static_cast<int>(Permissions::Reboot), AndroidPermNames::REBOOT},
        {static_cast<int>(Permissions::ReceiveBootCompleted), AndroidPermNames::RECEIVE_BOOT_COMPLETED},
        {static_cast<int>(Permissions::ReceiveMMS), AndroidPermNames::RECEIVE_MMS},
        {static_cast<int>(Permissions::ReceiveSMS), AndroidPermNames::RECEIVE_SMS},
        {static_cast<int>(Permissions::ReceiveWAPPush), AndroidPermNames::RECEIVE_WAP_PUSH},
        {static_cast<int>(Permissions::Microphone), AndroidPermNames::RECORD_AUDIO},
        {static_cast<int>(Permissions::ReorderTasks), AndroidPermNames::REORDER_TASKS},
        {static_cast<int>(Permissions::RequestCompanionRunInBackground), AndroidPermNames::REQUEST_COMPANION_RUN_IN_BACKGROUND},
        {static_cast<int>(Permissions::RequestCompanionUseDataInBackground), AndroidPermNames::REQUEST_COMPANION_USE_DATA_IN_BACKGROUND},
        {static_cast<int>(Permissions::RequestDeletePackages), AndroidPermNames::REQUEST_DELETE_PACKAGES},
        {static_cast<int>(Permissions::RequestIgnoreBatteryOptimization), AndroidPermNames::REQUEST_IGNORE_BATTERY_OPTIMIZATION},
        {static_cast<int>(Permissions::RequestInstallPackages), AndroidPermNames::REQUEST_INSTALL_PACKAGES},
        {static_cast<int>(Permissions::SendRespondViaMessage), AndroidPermNames::SEND_RESPOND_VIA_MESSAGE},
        {static_cast<int>(Permissions::SendSMS), AndroidPermNames::SEND_SMS},
        {static_cast<int>(Permissions::SetAlarm), AndroidPermNames::SET_ALARM},
        {static_cast<int>(Permissions::SetAlwaysFinish), AndroidPermNames::SET_ALWAYS_FINISH},
        {static_cast<int>(Permissions::SetAnimationScale), AndroidPermNames::SET_ANIMATION_SCALE},
        {static_cast<int>(Permissions::SetDebugApp), AndroidPermNames::SET_DEBUG_APP},
        {static_cast<int>(Permissions::SetProcessLimit), AndroidPermNames::SET_PROCESS_LIMIT},
        {static_cast<int>(Permissions::SetTime), AndroidPermNames::SET_TIME},
        {static_cast<int>(Permissions::SetTimeZone), AndroidPermNames::SET_TIME_ZONE},
        {static_cast<int>(Permissions::SetWallpaper), AndroidPermNames::SET_WALLPAPER},
        {static_cast<int>(Permissions::SetWallpaperHints), AndroidPermNames::SET_WALLPAPER_HINTS},
        {static_cast<int>(Permissions::SignalPersistentProcesses), AndroidPermNames::SIGNAL_PERSISTENT_PROCESSES},
        {static_cast<int>(Permissions::StatusBar), AndroidPermNames::STATUS_BAR},
        {static_cast<int>(Permissions::SystemAlertWindow), AndroidPermNames::SYSTEM_ALERT_WINDOW},
        {static_cast<int>(Permissions::TransmitIR), AndroidPermNames::TRANSMIT_IR},
        {static_cast<int>(Permissions::UninstallShortcut), AndroidPermNames::UNINSTALL_SHORTCUT},
        {static_cast<int>(Permissions::UpdateDeviceStats), AndroidPermNames::UPDATE_DEVICE_STATS},
        {static_cast<int>(Permissions::UseBiometric), AndroidPermNames::USE_BIOMETRIC},
        {static_cast<int>(Permissions::UseFingerprint), AndroidPermNames::USE_FINGERPRINT},
        {static_cast<int>(Permissions::UseSIP), AndroidPermNames::USE_SIP},
        {static_cast<int>(Permissions::Vibrate), AndroidPermNames::VIBRATE},
        {static_cast<int>(Permissions::WakeLock), AndroidPermNames::WAKE_LOCK},
        {static_cast<int>(Permissions::WriteAPNSettings), AndroidPermNames::WRITE_APN_SETTINGS},
        {static_cast<int>(Permissions::WriteCalendar), AndroidPermNames::WRITE_CALENDAR},
        {static_cast<int>(Permissions::WriteCallLog), AndroidPermNames::WRITE_CALLLOG},
        {static_cast<int>(Permissions::WriteContacts), AndroidPermNames::WRITE_CONTACTS},
        {static_cast<int>(Permissions::WriteExternalStorage), AndroidPermNames::WRITE_EXTERNAL_STORAGE},
        {static_cast<int>(Permissions::WriteGServices), AndroidPermNames::WRITE_GSERVICES},
        {static_cast<int>(Permissions::WriteSecureSettings), AndroidPermNames::WRITE_SECURE_SETTINGS},
        {static_cast<int>(Permissions::WriteSettings), AndroidPermNames::WRITE_SETTINGS},
        {static_cast<int>(Permissions::WriteSyncSettings), AndroidPermNames::WRITE_SYNC_SETTINGS},
        {static_cast<int>(Permissions::WriteVoiceMail), AndroidPermNames::WRITE_VOICEMAIL}
    };
}

}
