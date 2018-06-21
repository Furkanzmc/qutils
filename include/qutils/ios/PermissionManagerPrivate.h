#pragma once
// std
#include <functional>

namespace zmc
{

/*!
 * \class
 * \brief The PermissionManagerPrivate is a platform specific class to handle iOS permissions.
 *
 * You don't need to use this class directly. It is used by PermissionManager to request permissions on iOS.
 */
class PermissionManagerPrivate
{
public:

    /*!
     * \enum AuthorizationStatus
     * \brief The AuthorizationStatus represents the permission status for iOS.
     *
     * \value None
     *        Service is not in use.
     * \value NotDetermined
     *        The user has not yet made a choice.
     * \value Restricted
     *        This app is not authorized to use the service.
     * \value Authorized
     *        The user has authorized access to the service. For location services, this means that the user chose to
     *        allow always use access.
     * \value AuthorizedWhenInUse
     *        Only for location services. This app is authorized to start most location services while running in the
     *        foreground.
     */
    enum class AuthorizationStatus {
        None,
        NotDetermined,
        Restricted,
        Denied,
        Authorized,
        AuthorizedWhenInUse
    };

    /*!
     * \brief This is called when the user responds to the photo access permission request.
     */
    std::function<void(AuthorizationStatus /*status*/)> onPhotosAccessResult;

public:
    PermissionManagerPrivate();

    /*!
     * \brief Requests access to photos on iOS.
     *
     * When the request is responded to onPhotosAccessResult will be called.
     */
    void requestPhotosPermisson();

    /*!
     * \brief Returns the current photos access authorization status from the device. If photos is not enabled for
     * qutils, AuthorizationStatus::None is returned.
     * \return AuthorizationStatus
     */
    AuthorizationStatus getPhotosAuthorizationStatus() const;

    /*!
     * \brief Requests the location services permission.
     * \return void
     */
    void requestLocationPermission();

    /*!
     * \brief Returns the current location authorization status from the device. If location is not enabled for qutils,
     * AuthorizationStatus::None is returned.
     * \return AuthorizationStatus
     */
    AuthorizationStatus getLocationAuthorizationStatus() const;
};

}
