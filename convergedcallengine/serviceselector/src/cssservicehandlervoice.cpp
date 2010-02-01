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
*              : CSsServiceHandlerVoice class
*
*/


//  INCLUDE FILES
#include "cssservicehandlervoice.h"
#include "cssconnectionhandler.h"
#include "cssserviceutilities.h"
#include "csslogger.h"
#include <spsettingsvoiputils.h>

// CONSTANTS
// CS service id.
const TInt KGSMServiceId = 1;

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoice::CSsServiceHandlerVoice
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSsServiceHandlerVoice::CSsServiceHandlerVoice
        ( 
        // None.
        )
    {
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoice::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoice::ConstructL()
    {
    iVoipUtils = CSPSettingsVoIPUtils::NewL();
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoice::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSsServiceHandlerVoice* CSsServiceHandlerVoice::NewL
        ( 
        // None.
        )
    {
    CSsServiceHandlerVoice* self = new ( ELeave ) CSsServiceHandlerVoice();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CSsServiceHandlerVoice::~CSsServiceHandlerVoice
        (
        // None.
        )
    {
    delete iVoipUtils;
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoice::ExecuteServiceSelectionL
// Gets the calling service by current call type and service id.
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoice::ExecuteServiceSelectionL
        ( 
        CConvergedServiceSelector::TSsResult& aResult, 
        CConvergedServiceSelector::TSsCallType aCallType,
        TUint /*aServiceId*/,
        TBool /*aSendKey*/,
        const TDesC& /*aString*/ 
        )
    {
    CSSLOGSTRING("CSSelector::ExecuteVoice");
    
    TUint prefService(0);
    
    if ( iVoipUtils->IsPreferredTelephonyVoIP() &&
         KErrNone == iVoipUtils->GetPreferredService( prefService ) &&
         IsServiceRegistered( prefService ) )
        {
        CSSLOGSTRING("CSSelector::VoipAsPreferred");
        
        // Preferred telephony value as voip and preferred (voip) service
        // registered -> Change call type as voip and set correct service id.
        SsServiceUtilities::SetServiceResult( aResult,
                                              CConvergedServiceSelector::ESsVoipCall,
                                              prefService,
                                              ETrue );      
        }
    else
        {
        // Set (CS)voice call parameters
        // CallType, Service id and GSM service is always "enabled"
        SsServiceUtilities::SetServiceResult( aResult,
                                              aCallType,
                                              KGSMServiceId,
                                              ETrue );    
        }

    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoice::IsServiceRegistered
// Checks is the service registered
// -----------------------------------------------------------------------------
//
TBool CSsServiceHandlerVoice::IsServiceRegistered
        ( 
        TUint aServiceId 
        ) const
    {
    TBool ret( EFalse );
    CSsConnectionHandler* connection = NULL;
    
    // Nothing to do if CCH client construction not succeed.
    // So ignore the leave and cs call will be made.
    TRAP_IGNORE( connection = CSsConnectionHandler::NewL() );
    
    if ( connection )
        {
        ret = connection->IsVoipServiceRegistered( aServiceId );
        delete connection;
        }
    
    return ret;
    }

// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
