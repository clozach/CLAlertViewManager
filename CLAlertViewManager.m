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

#import "CLAlertViewManager.h"

@interface CLAlertViewManager ()
@property (nonatomic, retain) NSMutableSet *alertIds;
@property (nonatomic, retain) NSMutableDictionary *completionBlocks;
@end 

@implementation CLAlertViewManager

#ifdef USE_SYNTHESIZE_SINGLETON
SYNTHESIZE_SINGLETON_FOR_CLASS(CLAlertViewManager)
#endif

@synthesize alertIds;
@synthesize completionBlocks;

- (void)showWithTitle:(NSString *)aTitle
                   message:(NSString *)aMessage
             alwaysDisplay:(BOOL)ignorePreviousAlerts
         cancelButtonTitle:(NSString *)cancelTitle {
    
    [self showWithTitle:aTitle
                message:aMessage
             completion:nil
          alwaysDisplay:ignorePreviousAlerts
      cancelButtonTitle:cancelTitle
      otherButtonTitles:nil];
}

- (void)showWithTitle:(NSString *)aTitle
              message:(NSString *)aMessage
           completion:(CLAlertCompletionBlock)completionBlock
        alwaysDisplay:(BOOL)ignorePreviousAlerts
    cancelButtonTitle:(NSString *)cancelButtonTitle
    otherButtonTitles:(NSString *)otherButtonTitles, ... {

    if ( !alertIds ) {
        self.alertIds = [NSMutableSet set];
    }
    
    BOOL titleMatch = [alertIds containsObject:aTitle];
    BOOL messageMatch = [alertIds containsObject:aMessage];

    if ( ignorePreviousAlerts || (!titleMatch && !messageMatch) ) {

        NSString *alertId = nil;
        if ( ![NSString stringIsEmpty:aTitle] ) {
            alertId = aTitle;
        }
        else if ( ![NSString stringIsEmpty:aMessage] ) {
            alertId = aMessage;
        }
        [self.alertIds addObject:alertId];
        
        UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:aTitle
                                                         message:aMessage
                                                        delegate:self
                                               cancelButtonTitle:cancelButtonTitle
                                               otherButtonTitles:otherButtonTitles, nil] autorelease];
        
        // Add a button for each string after the first in the otherButtonTitles nil terminated list
        if ( otherButtonTitles ) {
            va_list args;
            va_start(args, otherButtonTitles);
            {
                NSString *buttonTitle = nil;
                while ((buttonTitle = va_arg(args, NSString*))) {
                    [alert addButtonWithTitle:buttonTitle];
                }        
            }
            va_end(args);
        }

        [alert show];
        
        if ( completionBlock ) {
            
            if ( !self.completionBlocks ) {
                self.completionBlocks = [NSMutableDictionary dictionary];
            }
            
            [self.completionBlocks setObject:completionBlock forKey:alertId];
        }
    }
}

#pragma mark -
#pragma mark UIAlertViewDelegate

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    
    NSString *alertId = nil;
    NSString *tappedButtonTitle = [alertView buttonTitleAtIndex:buttonIndex];
    
	// Remove the alert's identifier that would otherwise
	// cause us to ignore future alerts with the same id
	if ( ![NSString stringIsEmpty:alertView.title] ) {
        alertId = alertView.title;
	}
	else if ( ![NSString stringIsEmpty:alertView.message] ) {
        alertId = alertView.message;
	}
    [self.alertIds removeObject:alertId];
    
    // Run the alert's block if there is one
    CLAlertCompletionBlock completion = [self.completionBlocks objectForKey:alertId];

    if ( completion ) {
        completion(tappedButtonTitle);
    }
}


@end
