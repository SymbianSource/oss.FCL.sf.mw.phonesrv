/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CVmListQueryDialog class.
*
*/


// INCLUDE FILES
#include <aknlistquerydialog.h>
#include "vmlist.h"
#include <vmbx.rsg>

// MEMBER FUNCTIONS

// -----------------------------------------------------------------------------
// CVmListQueryDialog::CVmListQueryDialog()
// -----------------------------------------------------------------------------
CVmListQueryDialog::CVmListQueryDialog( TInt* aIndex ):CAknListQueryDialog( aIndex )
    {
    iOkToExit = EFalse;
    }

// -----------------------------------------------------------------------------
// CVmListQueryDialog::~CVmListQueryDialog()
// -----------------------------------------------------------------------------
CVmListQueryDialog::~CVmListQueryDialog()
{
}

// -----------------------------------------------------------------------------
// CVmListQueryDialog::NewL()
// -----------------------------------------------------------------------------
CVmListQueryDialog* CVmListQueryDialog::NewL( /*const TDesC& aPrompt,*/ TInt* aIndex )
    {
	CVmListQueryDialog* query = new( ELeave ) CVmListQueryDialog( aIndex );
	return query;
    }

// -----------------------------------------------------------------------------
// CVmListQueryDialog::OfferKeyEventL()
// -----------------------------------------------------------------------------
TKeyResponse CVmListQueryDialog::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    RDebug::Print( _L("CVmListQueryDialog::OfferKeyEventL: %d" ), aType);
    RDebug::Print( _L("CVmListQueryDialog::KeyCode: %d" ), aKeyEvent.iCode);
    RDebug::Print( _L("CVmListQueryDialog::KeyCode: %d" ), aKeyEvent.iScanCode);
    if( aKeyEvent.iCode == EKeyNo &&  aType == KErrNone )
        {
        iOkToExit = ETrue;
        RDebug::Print( _L("CVmListQueryDialog::OfferKeyEventL" ));
        DismissQueryL();
        return EKeyWasConsumed;
        }
    return CAknListQueryDialog::OfferKeyEventL( aKeyEvent, aType );
    }

TBool CVmListQueryDialog::OkToExitL(TInt aButtonId)
    {
    RDebug::Print( _L("CVmListQueryDialog::OkToExitL: %d" ), aButtonId);
    if( iOkToExit )
        {
        return ETrue;
        }
    return CAknListQueryDialog::OkToExitL(aButtonId);
    }
//  End of File
