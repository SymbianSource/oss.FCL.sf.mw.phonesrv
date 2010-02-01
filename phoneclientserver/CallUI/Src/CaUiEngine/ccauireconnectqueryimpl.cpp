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
* Description:  Implementation of CallUIEngine reconnect query.
*
*/



// INCLUDE FILES
#include    "ccauireconnectqueryimpl.h"
#include    "CaUiEngine.h"

CCaUiReconnectQueryImpl* CCaUiReconnectQueryImpl::NewL( 
    MCaUiReconnectQueryObserver& aObserver )
    {
    CCaUiReconnectQueryImpl* self = 
        new ( ELeave ) CCaUiReconnectQueryImpl( aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self;
    }
    
CCaUiReconnectQueryImpl::~CCaUiReconnectQueryImpl( )
    {
    if ( iEng )
        {
        iEng->CancelReconConfQuery();
        delete iEng;
        }
    }
    
void CCaUiReconnectQueryImpl::ShowL( 
    const TPtrC aPhoneNumber,
    const TBool aIncludeVideoCallOption )
    {
    iEng->LaunchReconConfQueryL(iObserver,
        NULL,
        aPhoneNumber,
        aIncludeVideoCallOption );
    }

void CCaUiReconnectQueryImpl::ConstructL()
    {
    iEng = CCaUiEngine::NewL();
    }

CCaUiReconnectQueryImpl::CCaUiReconnectQueryImpl(
            MCaUiReconnectQueryObserver& aObserver ):
                CCaUiReconnectQuery(aObserver)
    {
    
    }

// End of file


