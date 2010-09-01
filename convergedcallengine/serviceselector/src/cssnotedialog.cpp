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
* Description:  This module contains the implementation of CSsNoteDialog class
*
*/


//  INCLUDE FILES
#include "cssnotedialog.h"
#include "cssconnectionhandler.h"
#include "cssuiutilities.h"
#include "cssserviceutilities.h"
#include "csslogger.h"
#include <StringLoader.h>
#include <avkon.rsg>

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CSsNoteDialog::CSsNoteDialog
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSsNoteDialog::CSsNoteDialog( 
        CEikDialog** aSelf )
    : CAknNoteDialog(EConfirmationTone, ELongTimeout),
      iSelf( aSelf )
    {
    }

// Destructor
CSsNoteDialog::~CSsNoteDialog()
    {
    CSSLOGSTRING("CSsNoteDialog::~CSsNoteDialog");
    // Set self pointer as null.
    // Query could be destroyed by client application or by
    // the user selection.
    if ( iSelf )
       {
       *iSelf = NULL;
       iSelf = NULL;
       }
    }

// -----------------------------------------------------------------------------
// CSsNoteDialog::RunDialogLD
// Runs the dialog
// -----------------------------------------------------------------------------
//
void CSsNoteDialog::RunDialogLD( 
        TInt aResourceId )
    {
    CSSLOGSTRING("CSsNoteDialog::RunDialogLD <");
    HBufC* text = StringLoader::LoadLC( aResourceId );
    SetTextL( *text );
    CAknNoteDialog::PrepareLC( R_AKN_INFORMATION_NOTE_DIALOG_WAIT );
    CAknNoteDialog::RunLD();
    CleanupStack::PopAndDestroy( text );
    CSSLOGSTRING("CSsNoteDialog::RunDialogLD >");
    }

// -----------------------------------------------------------------------------
// CSsNoteDialog::OfferKeyEventL
// Send key handling overriden
// -----------------------------------------------------------------------------
//
TKeyResponse CSsNoteDialog::OfferKeyEventL(
        const TKeyEvent& aKeyEvent,
        TEventCode aType )
    {    
    CSSLOGSTRING2("CSsNoteDialog::OfferKeyEventL, aKeyEvent.iScanCode: %d"
            ,aKeyEvent.iScanCode );    
    TKeyResponse ret = EKeyWasNotConsumed;
    
    if ( aKeyEvent.iScanCode == EStdKeyYes && aType == EEventKey )
        {
        CSSLOGSTRING("CSsNoteDialog::OfferKeyEventL, EKeyWasConsumed");
        ret = EKeyWasConsumed;
        }
    return ret; 
    } 
	
// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
