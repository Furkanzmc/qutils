#include "qutils/ios/PermissionManagerPrivate.h"
// iOS
#if QUTILS_LOCATION_ENABLED
    #import <CoreLocation/CoreLocation.h>
#endif // QUTILS_LOCATION_ENABLED
#if QUTILS_PHOTOS_ENABLED
    #import <Photos/PHPhotoLibrary.h>
#endif // QUTILS_LOCATION_ENABLED
// qutils
#include "qutils/Macros.h"

namespace zmc
{

PermissionManagerPrivate::PermissionManagerPrivate()
{

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
    CLLocationManager *locationManager = [[CLLocationManager alloc] init];
    [locationManager requestWhenInUseAuthorization];
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

}
