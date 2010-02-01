/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*     Implements the feature for investigating length of the input. Softkeys
*     are set based on this information.
*     
*
*/


#include    <avkon.hrh>
#include    <avkon.rsg>
#include    <eikbtgpc.h>
#include    <msatuiadapter.h>
#include    <msatuiobserver.h>
#include    "CSatUiWaitDialog.h"
#include    "tflogger.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSatUiWaitDialog::CSatUiWaitDialog(
    CEikDialog** aSelfPtr,
    MSatUiAdapter& aAdapter, 
    TBool aVisibilityDelayOff ) :
    CAknWaitDialog( aSelfPtr, aVisibilityDelayOff ),
    iAdapter( aAdapter )
    {
    }

// Destructor.
CSatUiWaitDialog::~CSatUiWaitDialog()
    {
    }

// ---------------------------------------------------------
// CSatUiWaitDialog::OkToExitL()
// ---------------------------------------------------------
//
TBool CSatUiWaitDialog::OkToExitL( TInt aButtonId )
    {
    TFLOGSTRING( "CSatUiWaitDialog::OkToExitL called" )
    if ( aButtonId == EAknSoftkeyCancel )
        {        
        iAdapter.SessionTerminated( ESessionCancel );
        }
    TFLOGSTRING( "CSatUiWaitDialog::OkToExitL exit" )
    return ETrue;
    }
//  End of File
