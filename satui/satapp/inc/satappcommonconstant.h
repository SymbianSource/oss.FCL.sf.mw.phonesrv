/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#ifndef SATAPPCOMMONCONSTANT_H
#define SATAPPCOMMONCONSTANT_H

//  CONSTANTS

const int KDisplayTxtUserClearTimeout( 60000 );
// As the QT for S60 is slow, we use 6 sec, in symbian sat ui we use 3 sec.
// Rollback to 3s
const int KDisplayTxtDefaultduration( 3000 );

const unsigned int KGetInputStringMaxSize = 0xEF;

const int KExtendTimeoutMseconds(3000);
// 1 minute
const int KDefaultSelectionTimeoutMseconds(60000);
const int KSymbianTimeConvertQtTime(100);
const int KMoSmControlTimeOut(3000);
const int KPlayToneSymbianConvertQtTime(1000);

//enum for SendSms
/**Sat send sms user response*/
enum TSatAppConfirmType
    {
    ESatUiConfirmSendSms = 1,
    ESatUiConfirmSendSs,
    ESatUiConfirmSendUssd
    };
   
enum TSatAppUserResponse
{
    /**
     *  default, user no response
     */
    EUserNoResponse = 0,
    /**
     *  OK, user response
     */
    EUserPrimaryResponse,
    /**
     *  Back, user response
     */
    EUserSecondaryResponse,
    /**
     *  requested to clear by the server
     */
    EUserClearResponse,
    /**
     *  requested to close application by the server
     */
    EUserCloseResponse
};
    
enum TSatAppDialogActionType
    {
    ESatDialogDisplayText = 0, // Ok Back
    ESatDialogGetInput, // Cancel ->Ok Cancel
    ESatDialogGetInkey, // Cancel ->Ok Cancel
    ESatDialogGetYesNo, // Yes No
    ESatDialogGetDigitalRsp, // Cancel
    ESatDialogSetUpCall, // Call Cancel
    ESatDialogConfirmSend, // Yes No
    ESatDialogConfirmBip // Ok Cancel
    };

#endif 
