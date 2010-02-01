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
*              : CConvergedServiceSelector class.
*
*/


//  INCLUDE FILES
#include "cconvergedserviceselector.h"
#include "cservicehandler.h"
#include "cssserviceutilities.h"
#include "csslogger.h"

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CConvergedServiceSelector::CConvergedServiceSelector
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CConvergedServiceSelector::CConvergedServiceSelector
        ( 
        // None.
        )
    {
    }

// -----------------------------------------------------------------------------
// CConvergedServiceSelector::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CConvergedServiceSelector* CConvergedServiceSelector::NewL
        ( 
        // None.
        )
    {
    CConvergedServiceSelector* self = new ( ELeave ) CConvergedServiceSelector();
    return self;
    }

// Destructor
EXPORT_C CConvergedServiceSelector::~CConvergedServiceSelector
        (
        // None.
        )
    {
    CancelSelection();
    }

// -----------------------------------------------------------------------------
// CConvergedServiceSelector::GetCallingServiceByCallType
// Gets the calling service by current call type and service id.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CConvergedServiceSelector::GetCallingServiceByCallType
        (
        TSsResult& aResult, 
        TSsCallType aCallType,
        TUint aServiceId,
        TBool aSendKey,
        const TDesC& aString
        )
    { 
    CSSLOGSTRING4("CSSelector: calltype:%d ,serviceId:%d ,sendkey:%d",
                (TInt)aCallType,
                (TInt)aServiceId,
                (TInt)aSendKey);
    
    if ( iHandler )
        {
        // Selection currently ongoing, return error.
        return KErrNotReady;
        }
    
    
    TRAPD( ret, DoGetCallingServiceByCallTypeL( aResult,
                                                aCallType,
                                                aServiceId,
                                                aSendKey,
                                                aString ) );
    
    
    // Free resources
    CancelSelection();
    
    CSSLOGSTRING2("CSSelector error:%d", ret);
    CSSLOGSTRING4("CSSelector result: calltype:%d ,serviceId:%d ,enabled:%d",
                (TInt)aResult.iCallType,
                (TInt)aResult.iServiceId,
                (TInt)aResult.iServiceEnabled);
    
    return ret;
    }

// -----------------------------------------------------------------------------
// CConvergedServiceSelector::GetCallingServiceByCallType
// Cancels selection.
// -----------------------------------------------------------------------------
//
EXPORT_C void CConvergedServiceSelector::CancelSelection
        (
        // None
        )
    { 
    CSSLOGSTRING("CSSelector::CancelSelection");    
    // Free memory
    delete iHandler;
    iHandler = NULL;    
    }

// -----------------------------------------------------------------------------
// CConvergedServiceSelector::DoGetCallingServiceByCallTypeL
// Gets the calling service by current call type and service id.
// -----------------------------------------------------------------------------
//
void CConvergedServiceSelector::DoGetCallingServiceByCallTypeL
        (
        TSsResult& aResult, 
        TSsCallType aCallType,
        TUint aServiceId,
        TBool aSendKey,
        const TDesC& aString
        )

    {  
    // Get handler by calltype
    iHandler = SsServiceUtilities::ServiceHandlerByCallTypeL( 
                                    aCallType,
                                    aSendKey );
    
    
    // Execute handler.                                
    iHandler->ExecuteServiceSelectionL( aResult,
                                        aCallType,
                                        aServiceId,
                                        aSendKey,
                                        aString );

    }

// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
