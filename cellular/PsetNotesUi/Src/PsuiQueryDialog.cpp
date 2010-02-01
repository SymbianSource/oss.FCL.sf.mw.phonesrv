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
* Description:  Query dialog for handling key events.
*
*/


// INCLUDE FILES
#include "PsuiQueryDialog.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPsuiQueryDialog::CPsuiQueryDialog( CAknQueryDialog::TTone aTone ) :
	CAknQueryDialog( aTone ) 
    {
    }

// ---------------------------------------------------------------------------
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
CPsuiQueryDialog* CPsuiQueryDialog::NewL( CAknQueryDialog::TTone aTone )
    {  
    CPsuiQueryDialog* self = new( ELeave )CPsuiQueryDialog( aTone );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
// 
CPsuiQueryDialog::~CPsuiQueryDialog()
    {
    }

// ---------------------------------------------------------------------------
// CPsuiQueryDialog::OfferKeyEventL
// ---------------------------------------------------------------------------
//
TKeyResponse CPsuiQueryDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
	if ( ( aKeyEvent.iScanCode == EStdKeyNo || aKeyEvent.iCode == EKeyNo  ) &&
		 aType == EEventKeyUp )
		{
		// End -key was pressed, so exit this query dialog
		TryExitL( EKeyNo );
		}

	return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------------------------
// CPsuiQueryDialog::OkToExitL
// ---------------------------------------------------------------------------
//
TBool CPsuiQueryDialog::OkToExitL( TInt /*aCommand*/ )
    {
	// Dismiss query
    return ETrue;
    }

// End of File
