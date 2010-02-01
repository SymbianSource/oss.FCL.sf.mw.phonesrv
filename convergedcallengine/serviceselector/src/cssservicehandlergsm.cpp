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
*              : CSsServiceHandlerGsm class
*
*/


//  INCLUDE FILES
#include "cssservicehandlergsm.h"
#include "cssserviceutilities.h"
#include "csslogger.h"

// CONSTANTS
// CS service id
const TInt KGSMServiceId = 1;


// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CSsServiceHandlerGsm::CSsServiceHandlerGsm
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSsServiceHandlerGsm::CSsServiceHandlerGsm
        ( 
        // None.
        )
    {
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerGsm::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSsServiceHandlerGsm* CSsServiceHandlerGsm::NewL
        ( 
        // None.
        )
    {
    CSsServiceHandlerGsm* self = new(ELeave) CSsServiceHandlerGsm();
    return self;
    }

// Destructor
CSsServiceHandlerGsm::~CSsServiceHandlerGsm
        (
        // None.
        )
    {

    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerGsm::ExecuteServiceSelectionL
// Gets the calling service by current call type and service id.
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerGsm::ExecuteServiceSelectionL
        ( 
        CConvergedServiceSelector::TSsResult& aResult, 
        CConvergedServiceSelector::TSsCallType aCallType,
        TUint /*aServiceId*/,
        TBool /*aSendKey*/,
        const TDesC& /*aString*/ 
        )
    {
    CSSLOGSTRING("CSSelector::ExecuteGsm");
    
    // Set correct service id and call type ( CS voice or video call ).
    SsServiceUtilities::SetServiceResult( aResult,
                                          aCallType,
                                          KGSMServiceId,
                                          ETrue );
            
    }

// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
