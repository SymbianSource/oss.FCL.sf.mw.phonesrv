/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the VmbxUtilities class
*
*/


// INCLUDE FILES
#include <cvoicemailboxentry.h>

#include "voicemailboxdefsinternal.h"
#include "vmbxutilities.h"
#include "vmbxlogger.h"
#include "vmbxuiutilities.h"
#include "vmbxuihandler.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// VmbxUtilities::CVmbxUiUtilities
// C++ default constructor can NOT contain any code
// ---------------------------------------------------------------------------
CVmbxUiUtilities::CVmbxUiUtilities()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::CVmbxUiUtilities <=>" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::~CVmbxUiUtilities
// Destructor
// ---------------------------------------------------------------------------
CVmbxUiUtilities::~CVmbxUiUtilities()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::~CVmbxUiUtilities =>" );
    delete iUiHandler;
    //If there are still dialog is running, The dialogs should be dismissed.
    //The leave will be ignored as the CVmbxUiUtilities is distructing
    TRAP_IGNORE( DismissDialogL() );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::~CVmbxUiUtilities <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxUiUtilities* CVmbxUiUtilities::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::NewL =>" );
    CVmbxUiUtilities* vmbx = new( ELeave )CVmbxUiUtilities();
    CleanupStack::PushL( vmbx );
    vmbx->ConstructL();
    CleanupStack::Pop( vmbx );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::NewL <=" );
    return vmbx;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::ConstructL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ConstructL =>" );
    // create resource handler
    iUiHandler = CVmbxUiHandler::NewL();
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowQuery
// Show query
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowQueryL( const TVmbxType& aType,
        const TVmbxQueryMode & aMode, TDes& aNumber )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryL =>" );
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryL: aNumber =%S ", &aNumber );
    TInt result( KErrNone );
    FOREVER
        {
        ShowQueryDialogL( aType, aMode, aNumber, result );
        if ( KErrCancel == result )
            {
            break;
            }

        if ( KErrNone == result )
            {
            if ( aNumber.Length() )
                {
                // Convert back to western digits
                // check the given number here (emergency numbers not allowed)
                if ( VmbxUtilities::IsValidPhoneNumber( aNumber )
                     && !VmbxUtilities::IsEmergencyNumber( aNumber ) )
                    {
                    break;
                    }// valid number
                else
                    {
                    //invalid dialog
                    ShowInvalidWarningDialogL();
                    continue;
                    }// invalid number
                }
            else
                {
                result = KErrNone;
                break;
                }// number NULL
            }
        }// End FOREVER;
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryL: result=%I ", result );
    User::LeaveIfError( result );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowQueryDialogL
// Show query dialog
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowQueryDialogL( const TVmbxType& aType,
                                         const TVmbxQueryMode& aMode,
                                         TDes& aNumber, TInt& aResult )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryDialogL =>" );

    if( EVmbxVideo != aType && EVmbxVoice != aType  )
        {
        VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryDialogL: \
                            User::Leave( KErrNotSupported )" );
        User::Leave( KErrNotSupported );
        }
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryDialogL: in\
    aNumber = %S", &aNumber );
    // to show dialog via qt part
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryDialogL to show qt" );
    iUiHandler->ShowVmbxQueryDialog( aType, aMode, aNumber, aResult );
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryDialogL: out\
    aNumber = %S", &aNumber );
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryDialogL: aResult=%I <=",
     aResult );
    }


// ---------------------------------------------------------------------------
// VmbxUtilities::ShowSaveEmptyNoteL
// Show dialog when save empty data
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowSaveEmptyNoteL( const TVmbxType& aType )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowSaveEmptyNoteL =>" );
    if ( EVmbxVoice != aType && EVmbxVideo != aType )
        {
        User::Leave( KErrNotSupported );
        }
    iUiHandler->ShowSaveEmptyNote( aType );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowSaveEmptyNoteL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowInvalidWarningDialogL
// Check the characters
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowInvalidWarningDialogL()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowInvalidWarningDialogL =>" );
    //TODO comment out the following code to avoid crash, to be tested
    //and uncommented
    //iUiHandler->ShowInvalidWarningNote();
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowInvalidWarningDialogL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowSaveToPhoneNote
// Check the characters
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowSaveToPhoneNote()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowSaveToPhoneConfirmDialog =>" );
    //TODO comment out the following code to avoid crash, to be tested
    //and uncommented
    //iUiHandler->ShowSaveToPhoneNote();
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowSaveToPhoneConfirmDialog <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowSaveToSimConfirmDialog
// Check the characters
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowSaveToSimNote()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowSaveToSimNote =>" );
    //TODO comment out the following code to avoid crash, to be tested
    //and uncommented
    //iUiHandler->ShowSaveToSimNote();
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowSaveToSimNote <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowVideoSaveConfirmDialog
// Check the characters
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowVideoSavedNote()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowVideoSavedNote =>" );
    //TODO comment out the following code to avoid crash, to be tested
    //and uncommented
    //iUiHandler->ShowVideoSavedNote();
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowVideoSavedNote <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowDefineSelectionDialog
// Show define number in selection list
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowDefineSelectionDialogL( TVmbxType& aType,
                                                         TInt& aResult )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowDefineSelectionDialogL =>" );
    iUiHandler->ShowDefineSelectionDialog( aType, aResult );
    VMBLOGSTRING3( "VMBX: CVmbxUiUtilities::ShowDefineSelectionDialogL: \
                     aType=%I, aResult=%I <=", aType, aResult );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowDefineSelectionDialogL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowCallSelectionDialog
// Show call number in selection list
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowCallSelectionDialogL(
                    const RPointerArray<CVoiceMailboxEntry>& aArray,
                    TVoiceMailboxParams& aParams,
                    TInt& aResult )
    {
    VMBLOGSTRING(
    "VMBX: CVmbxUiUtilities::ShowCallSelectionDialog EMPTY IMPLEMENTATION! =>"
    );
    // TODO: not supported yet, to be implemented.
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowCallSelectionDialog <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::DismissDialogL
// Tries to close all open dialogs
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::DismissDialogL()
    {
    VMBLOGSTRING( 
    "VMBX: CVmbxUiUtilities::DismissDialogL EMPTY IMPLEMENTATION!=>" );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::DismissDialogL <=" );
    }
    
// Commented out because branding is not supported yet.
// ---------------------------------------------------------------------------
// CVmbxUiUtilities::GetVmbxImageL
//
// ---------------------------------------------------------------------------
/*
CGulIcon* CVmbxUiUtilities::GetVmbxImageL(
                        const TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::GetVmbxImageL" );
*/

// --------------------------------------------------------------------------
// CVmbxUiUtilities::ShowErrorDialogL
// --------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowErrorDialogL()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowErrorDialogL =>" );
    //TODO comment out the following code to avoid crash, to be tested
    //and uncommented
    iUiHandler->ShowInvalidNumberNote();// temporary dialog
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowErrorDialogL <=" );
    }

// -----------------------------------------------------------------------------
// CVmbxUiUtilities::FetchNumberFromPhonebook2L
// Create and show Phonebook's Single Fetch Dialog
// -----------------------------------------------------------------------------
//
void CVmbxUiUtilities::FetchNumberFromPhonebook2L( 
                                          TDes& aPhoneNumber )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::FetchNumberFromPhonebook2L =>" );
    /* TODO: Later remove this or replace with some Qt Phonebook API
        */
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::FetchNumberFromPhonebook2L <=" );
    }
// End of file
