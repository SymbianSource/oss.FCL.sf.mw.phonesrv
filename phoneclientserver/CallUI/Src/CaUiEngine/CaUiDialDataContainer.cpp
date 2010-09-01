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
* Description:  Contains PhoneClient Dial Data.
*
*/



// INCLUDE FILES
#include    "CaUiDialDataContainer.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaUiDialDataContainer::CCaUiDialDataContainer
// 
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCaUiDialDataContainer::CCaUiDialDataContainer()
    {
    }


// -----------------------------------------------------------------------------
// CCaUiDialDataContainer::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaUiDialDataContainer* CCaUiDialDataContainer::NewL()
    {
    CCaUiDialDataContainer* self = new( ELeave ) CCaUiDialDataContainer;

    return self;
    }

    
// -----------------------------------------------------------------------------
// CCaUiDialDataContainer::~CCaUiDialDataContainer
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiDialDataContainer::~CCaUiDialDataContainer()
    {
    }


// -----------------------------------------------------------------------------
// CCaUiDialDataContainer::DialData
// 
// 
// -----------------------------------------------------------------------------
//
TPhCltExtPhoneDialData& CCaUiDialDataContainer::DialData()
    {
    return iDialData;
    }


//  End of File  
