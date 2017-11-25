#import "qutils/ios/QutilsViewDelegate.h"
#import "qutils/ios/iOSNativeUtils.h"
// iOS
#import <Photos/Photos.h>
// Qt
#include <QImage>
#include <QSize>
#include <QVariant>

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

    UIImage *image = info[UIImagePickerControllerEditedImage];
    if (!image) {
        image = info[UIImagePickerControllerOriginalImage];
        data["image"] = QVariant::fromValue<QImage>([self convertUIImageToQImage:image]);
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

-(QImage)convertUIImageToQImage:(UIImage *)image {
    if (!image) {
        return QImage();
    }

    QImage::Format format = QImage::Format_RGB32;

    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
    CGFloat width = image.size.width;
    CGFloat height = image.size.height;

    int orientation = [image imageOrientation];
    int degree = 0;

    switch (orientation) {
        case UIImageOrientationLeft:
            degree = -90;
            break;
        case UIImageOrientationDown: // Down
            degree = 180;
            break;
        case UIImageOrientationRight:
            degree = 90;
            break;
    }

    if (degree == 90 || degree == -90)  {
        CGFloat tmp = width;
        width = height;
        height = tmp;
    }

    QSize size(width, height);
    QImage result = QImage(size, format);

    CGContextRef contextRef = CGBitmapContextCreate(result.bits(),
                                                    width,
                                                    height,
                                                    8,
                                                    result.bytesPerLine(),
                                                    colorSpace,
                                                    kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Little);

    CGContextDrawImage(contextRef, CGRectMake(0, 0, width, height), image.CGImage);
    CGContextRelease(contextRef);

    if (degree != 0) {
        QTransform myTransform;
        myTransform.rotate(degree);
        result = result.transformed(myTransform,Qt::SmoothTransformation);
    }

    return result;
}

@end
