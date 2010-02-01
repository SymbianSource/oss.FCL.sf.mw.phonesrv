/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of 
*              : CSsServiceUtilities class
*
*/


//  INCLUDE FILES
#include "cssserviceutilities.h"
#include "cssservicehandlergsm.h"
#include "cssservicehandlervoice.h"
#include "cssservicehandlervoip.h"

// CONSTANTS
_LIT( KSsValidCsNumberChars, "0123456789*#+pwPW" );


// ================= MEMBER FUNCTIONS =======================================


// -----------------------------------------------------------------------------
// CSsServiceUtilities::ServiceHandlerByCallTypeL
// Returns handler by call type
// -----------------------------------------------------------------------------
//
CServiceHandler* SsServiceUtilities::ServiceHandlerByCallTypeL
        ( 
        CConvergedServiceSelector::TSsCallType aCallType,
        TBool aSendKey 
        )
    { 
    CServiceHandler* handler = NULL;
        
    // Get handler by calltype
    switch( aCallType )
        {
        case CConvergedServiceSelector::ESsVoiceCall:
            {
            /* 
             * CodeScanner warning "neglected to put variable on
             * cleanup stack". Method cannot leave when handler is
             * properly initialized so warning ignored.
             */
            if ( aSendKey )
                {
                // If CS call is made by SendKey use
                // Voice service handler. We may have to
                // change the call type as voip (voice) call
                handler = CSsServiceHandlerVoice::NewL();
                }
            else
                {
                // Otherwise use gsm handler
                handler = CSsServiceHandlerGsm::NewL();
                }
            break; 
            }
        case CConvergedServiceSelector::ESsVideoCall:
            {
            // Use gsm handler for video call
            handler = CSsServiceHandlerGsm::NewL();
            break;
            }
        case CConvergedServiceSelector::ESsVoipCall:
            {
            // VoIP handler for VoIP call
            handler = CSsServiceHandlerVoip::NewL();
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
    
    return handler;
    }

// -----------------------------------------------------------------------------
// CSsServiceUtilities::SetServiceResult
// Sets the results to TSsResult
// -----------------------------------------------------------------------------
//
void SsServiceUtilities::SetServiceResult
        ( 
        CConvergedServiceSelector::TSsResult& aResult, 
        CConvergedServiceSelector::TSsCallType aCallType,
        TUint aServiceId,
        TBool aEnabled
        )
    {
    aResult.iCallType = aCallType;
    aResult.iServiceId = aServiceId;
    aResult.iServiceEnabled = aEnabled;
    }

// -----------------------------------------------------------------------------
// CSsServiceUtilities::ServiceHandlerByCallTypeL
// Sets the results to TSsResult
// -----------------------------------------------------------------------------
//
TBool SsServiceUtilities::IsValidCsNumber
        ( 
        const TDesC& aString 
        )    
    {
    if ( KErrNone == aString.Compare( KNullDesC ) )
        {
        // Empty string
        return EFalse;
        }
    
    TLex input( aString );
    TPtrC valid( KSsValidCsNumberChars );

    while ( valid.Locate( input.Peek() ) != KErrNotFound )
        {
        input.Inc();
        }
    
    return !input.Remainder().Length();
    }
    
// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
