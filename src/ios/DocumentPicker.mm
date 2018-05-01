#import "qutils/ios/DocumentPicker.h"

@interface DocumentPicker()

@end

@implementation DocumentPicker

- (void)showDocumentPicker:(const QStringList &)types allowMultiple: (bool)allowMultiple  {
    NSArray *nsTypes = nullptr;
    if (types.size() == 0) {
        nsTypes = @[@"public.data"];
    }
    else {
        NSMutableArray *mutableArray = [[NSMutableArray alloc] initWithCapacity:types.size()];
        for (int index = 0; index < types.size(); index++) {
            [mutableArray addObject:types.at(index).toNSString()];
        }

        nsTypes = [mutableArray copy];
    }

    UIDocumentPickerViewController *picker = [[UIDocumentPickerViewController alloc] initWithDocumentTypes:nsTypes inMode:UIDocumentPickerModeImport];
    picker.delegate = self;
    if (@available(iOS 11.0, *)) {
        picker.allowsMultipleSelection = allowMultiple;
    }
    UIApplication *app = [UIApplication sharedApplication];
    UIViewController *rootView = [[app keyWindow] rootViewController];
    [rootView presentViewController:picker animated:YES completion:nil];
}

- (void)documentPickerWasCancelled:(UIDocumentPickerViewController *)controller {
    (void)controller;
    zmc::FileUtilsPrivate::invokeDocumentPickerCanceled();
}

- (void)documentPicker:(UIDocumentPickerViewController *)controller didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls {
    Q_UNUSED(controller);
    QStringList paths;
    for (NSURL *url in urls) {
        paths.append(QString::fromNSString([url.absoluteString stringByRemovingPercentEncoding]));
    }

    zmc::FileUtilsPrivate::invokeDocumentPicked(paths);
}

@end
