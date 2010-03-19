/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the DialUtils factory interface.
*
*/



// INCLUDE FILES
#include    <dialutils.h> 
#include    "DialUtilsFactImpl.h" 
#include    "DialUtilsImpl.h" 



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CDialUtilsFactImpl::CDialUtilsApiLD
// 
// 
// -----------------------------------------------------------------------------
//
CDialUtilsApi* CDialUtilsFactImpl::CDialUtilsApiLD()
    {
    CleanupStack::PushL( this );

    CDialUtilsImpl* dialutils = CDialUtilsImpl::NewL();
    CleanupStack::PopAndDestroy( this );
    return dialutils;
    }


// -----------------------------------------------------------------------------
// CDialUtilsFactImpl::~CDialUtilsFactImpl
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CDialUtilsFactImpl::~CDialUtilsFactImpl()
    {
    }



// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CreateDialUtilsFactoryL
// 
// Creates DialUtilsFactory. This is only exported function.
// Returns: CDialUtilsFactory*: DialUtils Factory instance.
// -----------------------------------------------------------------------------
//
EXPORT_C CDialUtilsFactory* CreateDialUtilsFactoryL()
    {
    return new ( ELeave ) CDialUtilsFactImpl;
    }


//  End of File
