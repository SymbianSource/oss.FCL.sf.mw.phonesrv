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
* Description:  Implementation of the CaUiEngine factory interface.
*
*/



// INCLUDE FILES
#include    "cauiengfactimpl.h" 
#include    "cauiengine.h" 


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaUiEngFactImpl::CCaUiEngApiLD
// 
// 
// -----------------------------------------------------------------------------
//
CCaUiEngApi* CCaUiEngFactImpl::CCaUiEngApiLD()
    {
    CleanupStack::PushL( this );

    CCaUiEngine* caUiEng = CCaUiEngine::NewL();
    CleanupStack::PopAndDestroy( this );
    return caUiEng;
    }


// -----------------------------------------------------------------------------
// CCaUiEngFactImpl::~CCaUiEngFactImpl
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiEngFactImpl::~CCaUiEngFactImpl()
    {
    }



// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CreateCaUiEngFactoryL
// 
// Creates CallUIEngineFactory. This is only exported function.
// Returns: CCaUiEngFactory*: CallUIEngine Factory instance.
// -----------------------------------------------------------------------------
//
EXPORT_C CCaUiEngFactory* CreateCaUiEngFactoryL()
    {
    return new ( ELeave ) CCaUiEngFactImpl;
    }


//  End of File
