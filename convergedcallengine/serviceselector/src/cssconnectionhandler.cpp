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
*              : CSsConnectionHandler class
*
*/



//  INCLUDE FILES
#include "cssconnectionhandler.h"
#include "csslogger.h"

// ================= MEMBER FUNCTIONS =======================================
// -----------------------------------------------------------------------------
// CSsConnectionHandler::CSsConnectionHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSsConnectionHandler::CSsConnectionHandler
        ( 
        // None.
        )
    {
    }

// -----------------------------------------------------------------------------
// CSsConnectionHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSsConnectionHandler* CSsConnectionHandler::NewL()
    {
    CSsConnectionHandler* self = new ( ELeave ) CSsConnectionHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSsConnectionHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSsConnectionHandler::ConstructL()
    {
    // <-- QT PHONE START -->
    //iCch = CCch::NewL(); 
    // <-- QT PHONE END -->

    }

// Destructor
CSsConnectionHandler::~CSsConnectionHandler()
    {
    // <-- QT PHONE START -->
    //delete iCch;
    // <-- QT PHONE END-->

    }

// -----------------------------------------------------------------------------
// CSsServiceUtilities::IsVoipServiceRegistered
// Checks is the service registered
// -----------------------------------------------------------------------------
//
TBool CSsConnectionHandler::IsVoipServiceRegistered
        ( 
        TUint /*aServiceId */
        ) const
    {
   
    TBool registered( EFalse );
    // <-- QT PHONE START -->
/*
    TCchServiceStatus serviceStatus;
    CCchService* service = iCch->GetService( aServiceId );
    if ( service )
        {
        service->GetStatus( ECCHVoIPSub, serviceStatus );
        registered = ( KErrNone == serviceStatus.Error() ) && 
                     ( ECCHEnabled == serviceStatus.State() );
        }
    
    CSSLOGSTRING4("CSSelector::IsRegistered: id:%d ,enabled:%d ,error:%d",
                (TInt) aServiceId,
                (TInt) ECCHEnabled == serviceStatus.State(),
                (TInt) serviceStatus.Error());
                */
                
    // <-- QT PHONE END-->

     
    return registered;        
    }

// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
