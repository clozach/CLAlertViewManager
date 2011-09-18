//
//  CLAlertViewManager.m
//
//  Created by Lozac'h, Chris on 9/16/11.
//  Copyright 2011 Chris Lozac'h. All rights reserved.
//
//  This software is provided 'as-is', without any express or implied
//  warranty. In no event will the authors be held liable for any damages
//  arising from the use of this software. Permission is granted to anyone to
//  use this software for any purpose, including commercial applications, and to
//  alter it and redistribute it freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source
//     distribution.
//

#import <Foundation/Foundation.h>

#define USE_SYNTHESIZE_SINGLETON
#ifdef USE_SYNTHESIZE_SINGLETON
#import "SynthesizeSingleton.h"
#endif

typedef void (^CLAlertCompletionBlock)(NSString *);

/**
 ~ ~ ~ ~ ~
 INSTALLATION NOTE: CLAlertViewManager makes use of Matt Gallagher's
 SynthesizeSingleton macro, which you can read about and download here:
 
 http://cocoawithlove.com/2008/11/singletons-appdelegates-and-top-level.html
 
 To prevent use as a singleton, comment out the #define above.
 ~ ~ ~ ~ ~

 
 CLAlertViewManager provides two benefits over using UIAlertView
 directly.
 
 First, it prevents duplicate alerts from firing. Every
 time you call an alert, the title (or message, if no title is provided)
 is used as an alert identifier that is compared to a set of existing
 identifiers. Your alert will only be shown if the same identifier
 isn't already in the set. This can be particularly useful in cases
 where you display an alert as the result of a network error. Even
 Apple's own apps are prone to displaying duplicate alerts when the
 network's down.
 
 _Note:_ You can override this uniquing behavior by passing YES as the "alwaysDisplay" parameter.
 
 The second benefit of CLAlertViewManager is block support. Rather
 than having to implement UIAlertViewDelegate, your class can simply
 tell CLAlertViewManager to run a given block in the case where a
 user taps a button other than the cancel button. As you might expect,
 The block method requires iOS 4 and above.

 Example 1. Display 5 alerts, each with only a cancel button and no action upon dismissal.

    for ( int i = 0; i < 5; i++ ) {
    
        CLAlertViewManager *alertManager = [CLAlertViewManager sharedCLAlertViewManager];
        [alertManager showWithTitle:@"Hellow World" 
                            message:@"góðan dag, 你好, hola, नमस्ते, ellohay, etc."
                      alwaysDisplay:YES
                  cancelButtonTitle:@"OK"];
    }
 
 Example 2. Display only 1 alert, outputting the title of the button tapped to the console.

    for ( int i = 0; i < 5; i++ ) {
    
        CLAlertCompletionBlock alertCompletion = ^(NSString *buttonTitle) {
            NSLog(@"Button Title: %@", buttonTitle);
        };
    
        CLAlertViewManager *alertManager = [CLAlertViewManager sharedCLAlertViewManager];
        [alertManager showWithTitle:@"Hello World"
                           message:@"Bonjour, السّلام عليكم, Grüßgott, おはよう, shwmae, etc.";
                        completion:alertCompletion
                     alwaysDisplay:NO
                 cancelButtonTitle:@"Cancel" 
                 otherButtonTitles:@"OK1", @"OK2", nil];
    }
 */
@interface CLAlertViewManager : NSObject <UIAlertViewDelegate> {
    
}

#ifdef USE_SYNTHESIZE_SINGLETON
+ (id)sharedCLAlertViewManager;
#endif

/// Presents an information-only alert.
- (void)showWithTitle:(NSString *)aTitle
              message:(NSString *)aMessage
        alwaysDisplay:(BOOL)ignorePreviousAlerts
    cancelButtonTitle:(NSString *)cancelTitle;

/** Intended for cases where the user has one or more actions
 to choose from, this method will run the completion block
 if any non-cancel button is tapped by the user. The block type,
 CLAlertCompletion, provides the title of the button tapped to
 your block so that you can provide multiple action options
 within a single block.
 */
- (void)showWithTitle:(NSString *)title
              message:(NSString *)message
           completion:(CLAlertCompletionBlock)completionBlock
        alwaysDisplay:(BOOL)ignorePreviousAlerts
    cancelButtonTitle:(NSString *)cancelButtonTitle
    otherButtonTitles:(NSString *)otherButtonTitles, ...;

@end
