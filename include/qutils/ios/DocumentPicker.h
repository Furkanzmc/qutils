#pragma once
#import <UIKit/UIKit.h>
#import "qutils/ios/FileUtils_Private.h"

@interface DocumentPicker : NSObject<UIDocumentPickerDelegate> {}

- (void)showDocumentPicker:(const QStringList &)types allowMultiple: (bool)allowMultiple;

@end
