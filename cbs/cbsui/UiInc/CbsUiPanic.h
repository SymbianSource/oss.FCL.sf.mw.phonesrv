/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*   Contains the panic function for CbsUiApp.
*
*
*/


#ifndef __CBS_UI_PANIC_H
#define __CBS_UI_PANIC_H

//  INCLUDES
#include <e32def.h>


// DATA TYPES
enum TCbsUiPanic
    {
    ECbsUiError                         = 0,  // not used at the moment
    EStatusPaneError                    = 1,  // if status pane is null
    EStatusOrTitlePaneError             = 2,  // if status or title pane is null
    ENaviPaneError                      = 3,  // if navi pane is null
    ENaviDecoratorError                 = 4,  // navi decorator null error
    EListBoxInDialogError               = 5,  // list box creation error
    ETopicHandleError                   = 6,  // handles creation error
    EServerCreationFailedError          = 7,  // server creation failed
    EServerNullError                    = 8,  // server null error
    EListBoxNullError                   = 9,  // list box null error
    EMenuPaneNullError                  = 10, // menu pane null error
    EFindItemNullError                  = 11, // find item null error
    ETopicCountError                    = 12, // error when counting new topics
    EContainerNullError                 = 13, // container null error
    EMessageListNullError               = 14, // not used at the moment
    EMessageContentsError               = 15, // message content is not valid
    EListBoxAlreadyExistsError          = 16, // list box already exists error
    EEditorNullError                    = 17, // editor null error
    ETopicNumberError                   = 18, // topic number is invalid
    EEditorCreationError                = 19, // not used at the moment
    EWrongViewIdError                   = 20, // not used at the moment
    EContextMenuAlreadeyExists          = 21, // not used at the moment
    EErrEditTopicDlgOkToExit            = 22, // null pointer error in edit 
                                              // dialog
    EErrMsgContents                     = 23, // not used at the moment
    EErrNotAMsgHandle                   = 24, // invalid message handle
    EErrNotANumber                      = 25, // converting a descriptor 
                                              // to number error
    EErrScrollIndicatorAllreadyExists   = 26, // scroll indicator allready 
                                              // exists
    EErrAlgorithm                       = 27, // not used at them moment
    EErrIndexOutOfBounds                = 28, // index out of bounds error
    EErrMsgNotFound                     = 29, // message not found error
    EErrMsgHandleNotFound               = 30, // message handle not found error
    EErrNoSuchMessageHandle             = 31, // not used at the moment
    EErrTopicsCountError                = 32, // topics count error
    EErrBadIndex                        = 33, // bad index error
    EErrWrongNotify                     = 34, // not used
    EErrTopicNotFound                   = 35, // topic not found error
    EErrBadHandle                       = 36, // not used
    EErrMsgReadError                    = 37, // message read error
    EErrNullPointer                     = 38, // null pointer
    EErrBadSelection                    = 39, // selection error
    EErrAllocation                      = 40, // not used
    EErrInvalidValue                    = 41, // invalid value error
    EErrNotATopicNumber                 = 42, // invalid topic number
    EErrServerDisconnected              = 43, // server not connected error
    EErrAllreadyExists                  = 44, // trying to creat item, which is
                                              // allready created
    EErrInvalidEvent                    = 45, // invalid event
    EErrBadLength                       = 46, // descriptor too big
    EErrEmptyDesc                       = 47, // empty descriptor error
    EErrMsgImbalance                    = 48, // message postion is invalid
    EErrMsgNotExists                    = 49, // not used
    EErrTopicNumberNotFound             = 50, // topic number not found
    EErrNotAValidObserver               = 51, // not used
    EErrServerError                     = 52  // not used
    };

// FUNCTION PROTOTYPES
GLDEF_C void CbsUiPanic( TCbsUiPanic aPanic );

#endif      // __CBS_UI_PANIC_H
            
// End of File
