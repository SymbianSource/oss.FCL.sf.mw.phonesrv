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
*              : CSsListQueryDialog class
*
*/


//  INCLUDE FILES
#include "csslistquerydialog.h"
#include "cssconnectionhandler.h"
#include "cssuiutilities.h"
#include "cssserviceutilities.h"

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CSsListQueryDialog::CSsListQueryDialog
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSsListQueryDialog::CSsListQueryDialog
        ( 
        CEikDialog** aSelf,
        TInt& aIndex
        )
    : CAknListQueryDialog(&aIndex),
      iSelf( aSelf )
    {
    }

// Destructor
CSsListQueryDialog::~CSsListQueryDialog
        (
        // None.
        )
    {
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
// CSsListQueryDialog::ExecuteListLD
// Runs the dialog
// -----------------------------------------------------------------------------
//
TInt CSsListQueryDialog::ExecuteListLD
        ( 
        TInt aResourceId,
        MDesCArray* aItemTextArray,
        CArrayPtr<CGulIcon>* aIcons
        )
    {
    CAknListQueryDialog::PrepareLC( aResourceId );
    SetTone( EConfirmationTone );
    if ( aIcons )
        {
        SetIconArrayL( aIcons );
        }
    SetItemTextArray( aItemTextArray );
    SetOwnershipType( ELbmDoesNotOwnItemArray );
    return CAknListQueryDialog::RunLD();
    }


// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
