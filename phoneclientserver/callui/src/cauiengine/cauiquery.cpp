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
* Description:  Query that is used in asking call type (confirmation) 
*                from user.
*
*/




//  INCLUDES
#include "cauiquery.h" 


// -----------------------------------------------------------------------------
// CCaUiCreateListQuery::CCaUiListQuery
// 
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCaUiCreateListQuery::CCaUiCreateListQuery( TInt* aIndex )
:   CAknListQueryDialog( aIndex )
    {
    }


// -----------------------------------------------------------------------------
// CCaUiCreateListQuery::~CCaUiCreateListQuery
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiCreateListQuery::~CCaUiCreateListQuery()
    {
    }


// -----------------------------------------------------------------------------
// CCaUiCreateListQuery::OfferKeyEventL
// 
// 
// -----------------------------------------------------------------------------
//
TKeyResponse CCaUiCreateListQuery::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    TKeyResponse response = EKeyWasConsumed;

    if ( aType == EEventKeyDown )
        {
        // The first key event received will be a partial one, and will therefore be ignored.
        iValidKeyEvent = ETrue;
        }
    
    if ( iValidKeyEvent && aKeyEvent.iScanCode == EStdKeyYes )
        {
        if ( aType == EEventKey )
            {
            // The send key is specified to act (in Video Telephony UI specification) so that it 
            // initiates the call. This means that the event is modified and faked to be a select
            // key event, so that the list box can handle it correctly.
            TKeyEvent keyEvent = aKeyEvent;
            keyEvent.iCode = EKeyOK;
            keyEvent.iScanCode = EStdKeyDevice3;
            response = CAknListQueryDialog::OfferKeyEventL( keyEvent, EEventKey );
            }
        }
    else
        {
        response = CAknListQueryDialog::OfferKeyEventL( aKeyEvent, aType );
        }
    
    return response;
    }


// End of File
