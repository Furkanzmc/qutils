#include "qutils/ios/PermissionManagerPrivate.h"
// iOS
#if QUTILS_CAMERA_ENABLED
    #import <AVFoundation/AVCaptureDevice.h>
#endif // QUTILS_CAMERA_ENABLED
#if QUTILS_LOCATION_ENABLED
    #import <CoreLocation/CoreLocation.h>
#endif // QUTILS_LOCATION_ENABLED
#if QUTILS_PHOTOS_ENABLED
    #import <Photos/PHPhotoLibrary.h>
#endif // QUTILS_LOCATION_ENABLED
// qutils
#include "qutils/Macros.h"

#if QUTILS_LOCATION_ENABLED
@interface LocationDelegate : NSObject<CLLocationManagerDelegate> {}
- (void)locationManager: (CLLocationManager *)manager didChangeAuthorizationStatus: (CLAuthorizationStatus)status;
@end

@interface LocationDelegate()
@end

@implementation LocationDelegate

- (void)locationManager: (CLLocationManager *)manager didChangeAuthorizationStatus: (CLAuthorizationStatus)status
{
    Q_UNUSED(manager);
    zmc::PermissionManagerPrivate::locationServicesCallback(status);
}

- (void)requestPermission {
    CLLocationManager *locationManager = [[CLLocationManager alloc] init];
    locationManager.delegate = self;
    [locationManager requestWhenInUseAuthorization];
}
@end

static LocationDelegate *m_LocationDelegate = nullptr;
#endif // QUTILS_LOCATION_ENABLED

namespace zmc
{

std::map<int, PermissionManagerPrivate *> PermissionManagerPrivate::m_Instances = std::map<int, PermissionManagerPrivate *>();
unsigned int PermissionManagerPrivate::m_NextInstanceID = 0;

PermissionManagerPrivate::PermissionManagerPrivate()
    : m_InstanceIndex(m_NextInstanceID)
    , m_IsRequestedLocationPermission(false)
{
    m_NextInstanceID++;
    m_Instances[m_InstanceIndex] = this;
}

PermissionManagerPrivate::~PermissionManagerPrivate()
{
    m_Instances.erase(m_InstanceIndex);
}

void PermissionManagerPrivate::requestPhotosPermisson()
{
#if QUTILS_PHOTOS_ENABLED
    if ([PHPhotoLibrary authorizationStatus] == PHAuthorizationStatusNotDetermined) {
        [PHPhotoLibrary requestAuthorization: ^(PHAuthorizationStatus status) {
            if (onPhotosAccessResult) {
                AuthorizationStatus pstatus = AuthorizationStatus::Denied;
                if (status == PHAuthorizationStatusAuthorized) {
                    pstatus = AuthorizationStatus::Authorized;
                }
                else if (status == PHAuthorizationStatusNotDetermined) {
                    pstatus = AuthorizationStatus::NotDetermined;
                }
                else if (status == PHAuthorizationStatusRestricted) {
                    pstatus = AuthorizationStatus::Restricted;
                }

                onPhotosAccessResult(pstatus);
            }
            else {
                LOG_ERROR("onPhotosAccessResult is not set.");
            }
        }];
    }
#endif // QUTILS_PHOTOS_ENABLED
}

PermissionManagerPrivate::AuthorizationStatus PermissionManagerPrivate::getPhotosAuthorizationStatus() const
{
    AuthorizationStatus authStatus = AuthorizationStatus::None;
#if QUTILS_PHOTOS_ENABLED
    PHAuthorizationStatus status = [PHPhotoLibrary authorizationStatus];
    if (status == PHAuthorizationStatusDenied) {
        authStatus = AuthorizationStatus::Denied;
    }
    else if (status == PHAuthorizationStatusRestricted) {
        authStatus = AuthorizationStatus::Restricted;
    }
    else if (status == PHAuthorizationStatusNotDetermined) {
        authStatus = AuthorizationStatus::NotDetermined;
    }
    else if (status == PHAuthorizationStatusAuthorized) {
        authStatus = AuthorizationStatus::Authorized;
    }
#endif // QUTILS_PHOTOS_ENABLED

    return authStatus;
}

void PermissionManagerPrivate::requestLocationPermission()
{
#if QUTILS_LOCATION_ENABLED
    m_IsRequestedLocationPermission = true;
    if (m_LocationDelegate == nullptr) {
        m_LocationDelegate = [[LocationDelegate alloc] init];
    }

    [m_LocationDelegate requestPermission];
#endif // QUTILS_LOCATION_ENABLED
}

PermissionManagerPrivate::AuthorizationStatus PermissionManagerPrivate::getLocationAuthorizationStatus() const
{
    AuthorizationStatus authStatus = AuthorizationStatus::None;
#if QUTILS_LOCATION_ENABLED
    CLAuthorizationStatus status = [CLLocationManager authorizationStatus];
    if (status == kCLAuthorizationStatusDenied) {
        authStatus = AuthorizationStatus::Denied;
    }
    else if (status == kCLAuthorizationStatusRestricted) {
        authStatus = AuthorizationStatus::Restricted;
    }
    else if (status == kCLAuthorizationStatusNotDetermined) {
        authStatus = AuthorizationStatus::NotDetermined;
    }
    else if (status == kCLAuthorizationStatusAuthorizedAlways) {
        authStatus = AuthorizationStatus::Authorized;
    }
    else if (status == kCLAuthorizationStatusAuthorizedWhenInUse) {
        authStatus = AuthorizationStatus::AuthorizedWhenInUse;
    }
#endif // QUTILS_LOCATION_ENABLED

    return authStatus;
}

void PermissionManagerPrivate::requestCameraAccess()
{
#if QUTILS_CAMERA_ENABLED
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL handled) {
        if (onCameraPermissionResult) {
            if (handled == YES) {
                onCameraPermissionResult(AuthorizationStatus::Authorized);
            }
            else {
                onCameraPermissionResult(AuthorizationStatus::Denied);
            }
        }
        else {
            LOG_ERROR("onCameraPermissionResult callback is not set.");
        }
    }];
#endif // QUTILS_CAMERA_ENABLED
}

PermissionManagerPrivate::AuthorizationStatus PermissionManagerPrivate::getCameraAccessAuthStatus() const
{
    AuthorizationStatus status = AuthorizationStatus::None;
#if QUTILS_CAMERA_ENABLED
    const AVAuthorizationStatus avstatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if (avstatus == AVAuthorizationStatusDenied) {
        status = AuthorizationStatus::Denied;
    }
    else if (avstatus == AVAuthorizationStatusAuthorized) {
        status = AuthorizationStatus::Authorized;
    }
    else if (avstatus == AVAuthorizationStatusRestricted) {
        status = AuthorizationStatus::Restricted;
    }
    else if (avstatus == AVAuthorizationStatusNotDetermined) {
        status = AuthorizationStatus::NotDetermined;
    }
#endif // QUTILS_CAMERA_ENABLED
    return status;
}

void PermissionManagerPrivate::locationServicesCallback(int authStatus)
{
#if QUTILS_LOCATION_ENABLED
    AuthorizationStatus status = AuthorizationStatus::None;
    if (authStatus == kCLAuthorizationStatusDenied) {
        status = AuthorizationStatus::Denied;
    }
    else if (authStatus == kCLAuthorizationStatusRestricted) {
        status = AuthorizationStatus::Restricted;
    }
    else if (authStatus == kCLAuthorizationStatusNotDetermined) {
        status = AuthorizationStatus::NotDetermined;
    }
    else if (authStatus == kCLAuthorizationStatusAuthorizedAlways) {
        status = AuthorizationStatus::Authorized;
    }
    else if (authStatus == kCLAuthorizationStatusAuthorizedWhenInUse) {
        status = AuthorizationStatus::AuthorizedWhenInUse;
    }

    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        PermissionManagerPrivate *instance = (*it).second;
        if (instance->m_IsRequestedLocationPermission && instance->onLocationServicesResult) {
            instance->onLocationServicesResult(status);
        }
    }
#else
    Q_UNUSED(authStatus);
#endif // QUTILS_LOCATION_ENABLED
}

}
