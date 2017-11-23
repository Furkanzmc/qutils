#import "qutils/ios/QutilsViewDelegate.h"
#import "qutils/ios/iOSNativeUtils.h"
// iOS
#import <Photos/Photos.h>

@interface QutilsViewDelegate ()

@end

@implementation QutilsViewDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    [picker dismissViewControllerAnimated:YES completion:nil];

    QVariantMap data;

    NSURL *imageURL = [info valueForKey:UIImagePickerControllerReferenceURL];
    PHAsset *asset = [[PHAsset fetchAssetsWithALAssetURLs:@[imageURL] options:nil] lastObject];
    NSURL *fileURL = [self getAssetPath:asset];

    data["mediaType"] = QString::fromNSString(info[UIImagePickerControllerMediaType]);
    if (fileURL != nil) {
        data["referenceUrl"] = QString::fromNSString([fileURL absoluteString]);
    }
    else {
        data["referenceUrl"] = "";
    }

    zmc::iOSNativeUtils::emitImagePickerFinished(data);
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    [picker dismissViewControllerAnimated:YES completion:nil];

    zmc::iOSNativeUtils::emitImagePickerCancelled();
}

-(NSURL *)getAssetPath:(PHAsset *)asset {
    __block NSURL *fileURL = 0;

    if (asset) {
        // get photo info from this asset
        PHImageRequestOptions * imageRequestOptions = [[PHImageRequestOptions alloc] init];
        imageRequestOptions.synchronous = YES;
        [[PHImageManager defaultManager]
         requestImageDataForAsset:asset options:imageRequestOptions
         resultHandler:^(NSData *imageData, NSString *dataUTI, UIImageOrientation orientation, NSDictionary *info) {
             (void)imageData;
             (void)dataUTI;
             (void)orientation;
             if ([info objectForKey:@"PHImageFileURLKey"]) {
                 fileURL = [info objectForKey:@"PHImageFileURLKey"];
             }
         }];
    }

    return fileURL;
}

@end
