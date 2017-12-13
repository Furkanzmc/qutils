#import "qutils/ios/QutilsViewDelegate.h"
#import "qutils/ios/iOSNativeUtils.h"
#import "qutils/FileUtils.h"
// iOS
#import <Photos/Photos.h>

@interface QutilsViewDelegate ()

@end

@implementation QutilsViewDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    [picker dismissViewControllerAnimated:YES completion:nil];

    QVariantMap data;
    NSURL *fileURL = nil;
    NSURL *imageURL = [info valueForKey:UIImagePickerControllerReferenceURL];

    if (imageURL) {
        PHAsset *asset = [[PHAsset fetchAssetsWithALAssetURLs:@[imageURL] options:nil] lastObject];
        fileURL = [self getAssetPath:asset];
    }

    data["mediaType"] = QString::fromNSString(info[UIImagePickerControllerMediaType]);
    if (fileURL != nil) {
        data["isSourceCamera"] = false;
        data["referenceUrl"] = QString::fromNSString([fileURL absoluteString]);
    }
    else {
        data["referenceUrl"] = "";
        data["isSourceCamera"] = true;
    }

    UIImage *image = info[UIImagePickerControllerEditedImage];
    if (!image) {
        image = info[UIImagePickerControllerOriginalImage];
    }

    if (image) {
        image = [self fixOrientation:info[UIImagePickerControllerOriginalImage]];

        const QString tempPath = zmc::FileUtils::getTemporaryFile("XXXXXX.jpg");
        data["tempUrl"] = tempPath;
        NSString *savePath = tempPath.toNSString();
        NSData *binaryImageData = UIImageJPEGRepresentation(image, 1.0);
        [[NSFileManager defaultManager] createFileAtPath:savePath contents:binaryImageData attributes:nil];
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

- (UIImage *)fixOrientation:(UIImage *)image {
    // Code is taken from here: https://stackoverflow.com/questions/5427656/ios-uiimagepickercontroller-result-image-orientation-after-upload
    // No-op if the orientation is already correct
    if (image.imageOrientation == UIImageOrientationUp) {
        return image;
    }

    // We need to calculate the proper transformation to make the image upright.
    // We do it in 2 steps: Rotate if Left/Right/Down, and then flip if Mirrored.
    CGAffineTransform transform = CGAffineTransformIdentity;

    switch (image.imageOrientation) {
        case UIImageOrientationDown:
        case UIImageOrientationDownMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, image.size.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, 0);
            transform = CGAffineTransformRotate(transform, M_PI_2);
            break;
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformTranslate(transform, 0, image.size.height);
            transform = CGAffineTransformRotate(transform, -M_PI_2);
            break;
        case UIImageOrientationUp:
        case UIImageOrientationUpMirrored:
            break;
    }

    switch (image.imageOrientation) {
        case UIImageOrientationUpMirrored:
        case UIImageOrientationDownMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, 0);
            transform = CGAffineTransformScale(transform, -1, 1);
            break;
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.height, 0);
            transform = CGAffineTransformScale(transform, -1, 1);
            break;
        case UIImageOrientationUp:
        case UIImageOrientationDown:
        case UIImageOrientationLeft:
        case UIImageOrientationRight:
            break;
    }

    // Now we draw the underlying CGImage into a new context, applying the transform
    // calculated above.
    CGContextRef ctx = CGBitmapContextCreate(NULL,
                                             image.size.width,
                                             image.size.height,
                                             CGImageGetBitsPerComponent(image.CGImage),
                                             0,
                                             CGImageGetColorSpace(image.CGImage),
                                             CGImageGetBitmapInfo(image.CGImage));
    CGContextConcatCTM(ctx, transform);

    switch (image.imageOrientation) {
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            // Grr...
            CGContextDrawImage(ctx, CGRectMake(0, 0, image.size.height, image.size.width), image.CGImage);
            break;
        default:
            CGContextDrawImage(ctx, CGRectMake(0, 0, image.size.width, image.size.height), image.CGImage);
            break;
    }

    // And now we just create a new UIImage from the drawing context
    CGImageRef cgimg = CGBitmapContextCreateImage(ctx);
    UIImage *img = [UIImage imageWithCGImage:cgimg];
    CGContextRelease(ctx);
    CGImageRelease(cgimg);
    return img;
}

@end
