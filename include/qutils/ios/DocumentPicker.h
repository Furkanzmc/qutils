//
//  DocumentPicker.h
//
//  Created by Furkan Üzümcü on 12-04-2018.
//  Copyright © 2018 Cuz Translation. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "qutils/ios/FileUtils_Private.h"

@interface DocumentPicker : NSObject<UIDocumentPickerDelegate> {}

- (void)showDocumentPicker:(const QStringList &)types allowMultiple: (bool)allowMultiple;

@end
