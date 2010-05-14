/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Creates CallUIEngine reconnect query implementation.
*
*/



// INCLUDE FILES
#include    "ccauireconnectquery.h"
#include    "ccauireconnectqueryimpl.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaUiReconnectQuery::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCaUiReconnectQuery* CCaUiReconnectQuery::NewL( 
    MCaUiReconnectQueryObserver& aObserver )
    {
    return CCaUiReconnectQueryImpl::NewL( aObserver );
    }

CCaUiReconnectQuery::~CCaUiReconnectQuery()
    {
    // No implementation needed
    }
// -----------------------------------------------------------------------------
// CCaUiReconnectQuery::CCaUiReconnectQuery
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCaUiReconnectQuery::CCaUiReconnectQuery( MCaUiReconnectQueryObserver& aObserver )
    : iObserver( aObserver )
    {
    }
    



//  End of File  
