/*
* Copyright (c) 2002 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CVmTextQueryDialog class.
*
*/


// INCLUDE FILES
#include <AknQueryDialog.h>
#include <featmgr.h>
#include "vmdialog.h"
#include <vmbx.rsg>
#include "vmblogger.h"


// MEMBER FUNCTIONS

// -----------------------------------------------------------------------------
// CVmTextQueryDialog::CVmTextQueryDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVmTextQueryDialog::CVmTextQueryDialog( TDes& aDataText, 
                                        const TBool aSearchEnabled )
                             : CAknTextQueryDialog( aDataText ),
                                         iSearchEnabled( aSearchEnabled )
    {
    // no tone as default
    }

// -----------------------------------------------------------------------------
// CVmTextQueryDialog::~CVmTextQueryDialog
// Destructor
// -----------------------------------------------------------------------------
//
CVmTextQueryDialog::~CVmTextQueryDialog()
{

}

// -----------------------------------------------------------------------------
// CVmTextQueryDialog::NewL
// Two-phased constructor
// -----------------------------------------------------------------------------
//
CVmTextQueryDialog* CVmTextQueryDialog::NewL( TDes& aDataText, 
                                                const TDesC& aPrompt, 
                                                  const TBool aSearchEnabled )
    {
    VMBLOGSTRING( "VMBX: CVmTextQueryDialog::NewL =>" );
    CVmTextQueryDialog* query = new( ELeave ) CVmTextQueryDialog( aDataText,
                                                               aSearchEnabled );
    CleanupStack::PushL( query );
    query->SetPromptL( aPrompt );
    CleanupStack::Pop( query );
    VMBLOGSTRING( "VMBX: CVmTextQueryDialog: <=:NewL " );
    return query;
    }

// -----------------------------------------------------------------------------
// CVmTextQueryDialog::PreLayoutDynInitL
// Pre-layout dialog initialisation
// -----------------------------------------------------------------------------
//
void CVmTextQueryDialog::PreLayoutDynInitL()
    {
    VMBLOGSTRING( "VMBX: CVmTextQueryDialog::PreLayoutDynInitL =>" );
    CAknTextQueryDialog::PreLayoutDynInitL();

    // Change command buttons if needed
    if ( iSearchEnabled && !QueryControl()->GetTextLength() )
        {
        ButtonGroupContainer().SetCommandSetL( 
                                        R_VMBX_SOFTKEYS_SEARCH_CANCEL_SELECT );
        }

    MakeLeftSoftkeyVisible( ETrue );
                    
    // Set Pen support flags.
    if ( FeatureManager::FeatureSupported( KFeatureIdPenSupport ) )
        {
        VMBLOGSTRING( "VMBX: CVmTextQueryDialog::PreLayoutDynInitL Pen" );
        CAknQueryControl* control = QueryControl();
        if( control )
            {
            VMBLOGSTRING( "VMBX: CVmTextQueryDialog::PreLayoutDynInitL ctr" );
            CCoeControl *controlByLayout = 
                control->ControlByLayoutOrNull( control->QueryType() );
            if ( controlByLayout )
                {         
                VMBLOGSTRING( "VMBX: CVmTextQueryDialog::PreLayoutDynInitL\
                    edit" );
                CEikEdwin* numberEditor = 
                    static_cast< CEikEdwin* > ( controlByLayout );
                VMBLOGSTRING( "VMBX: CVmTextQueryDialog::PreLayoutDynInitL\
                    cast" );
                TInt flags = numberEditor->AknEdwinFlags() 
                    | EAknEditorFlagDeliverVirtualKeyEventsToApplication;
                VMBLOGSTRING( "VMBX: CVmTextQueryDialog::PreLayoutDynInitL\
                    flag" );
                numberEditor->SetAknEditorFlags( flags );
                VMBLOGSTRING( "VMBX: CVmTextQueryDialog::PreLayoutDynInitL\
                    set done" );                
                }
            }
        }

    VMBLOGSTRING( "VMBX: CVmTextQueryDialog::PreLayoutDynInitL <=" );
    }

// -----------------------------------------------------------------------------
// CVmTextQueryDialog::HandleQueryEditorStateEventL
// Called when query editor event happens
// -----------------------------------------------------------------------------
//
TBool CVmTextQueryDialog::HandleQueryEditorStateEventL( 
                                            CAknQueryControl* /*aQueryControl*/,
                                            TQueryControlEvent /*aEventType*/,
                                            TQueryValidationStatus /*aStatus*/ )
    {
    VMBLOGSTRING( "VMBX: CVmTextQueryDialog::HandleQueryEditorStateEventL =>" );
    MakeLeftSoftkeyVisible( ETrue );
    if ( iSearchEnabled )
        {
        if ( !QueryControl()->GetTextLength() )
            {
            ButtonGroupContainer().SetCommandSetL( 
                                       R_VMBX_SOFTKEYS_SEARCH_CANCEL_SELECT );
            }
        else
            {
            ButtonGroupContainer().SetCommandSetL( 
                                             R_AVKON_SOFTKEYS_OK_CANCEL__OK );
            }
        ButtonGroupContainer().DrawDeferred();
        }
    VMBLOGSTRING( "VMBX: CVmTextQueryDialog::HandleQueryEditorStateEventL <=" );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CVmTextQueryDialog::OfferKeyEventL
// Handles key events
// -----------------------------------------------------------------------------
//
TKeyResponse CVmTextQueryDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                            TEventCode aType )
    {
    VMBLOGSTRING( "VMBX: CVmTextQueryDialog::OfferKeyEventL =>" );
    TKeyResponse response = EKeyWasConsumed;
    switch( aKeyEvent.iScanCode )
        {
        case EStdKeyYes:
            // Catch this key event. EStdKeyYes would leak to other controls.
            break;
                
        default:
            response = CAknTextQueryDialog::OfferKeyEventL( aKeyEvent, aType );
        break; 
        }

    VMBLOGSTRING( "VMBX: CVmTextQueryDialog::OfferKeyEventL <=" );
    return response;
    }

// -----------------------------------------------------------------------------    
// CVmTextQueryDialog::DissmissQueryDlg
// Dissmiss the query with cancel
// -----------------------------------------------------------------------------       
//
void CVmTextQueryDialog::DismissQueryDlg()
     {
     // try to dismiss query,leave will be ignored
     TRAP_IGNORE( TryExitL( EAknSoftkeyCancel ) );
     }

// -----------------------------------------------------------------------------
// CVMSimQueryDilaog::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------    
//
CVMSimQueryDilaog* CVMSimQueryDilaog::NewL()
    {
    CVMSimQueryDilaog* query = new( ELeave )CVMSimQueryDilaog( );
    CleanupStack::PushL( query );
    
    CleanupStack::Pop( query );
    return query;    
    }


// -----------------------------------------------------------------------------
// CVMSimQueryDilaog::CVMSimQueryDilaog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
// 
CVMSimQueryDilaog::CVMSimQueryDilaog(): CAknQueryDialog( 
                                        CAknQueryDialog::EConfirmationTone )
    {
    // no tone as default
    }

// -----------------------------------------------------------------------------
// CVMSimQueryDilaog:: ~CVMSimQueryDilaog
// Destructor
// -----------------------------------------------------------------------------
//
CVMSimQueryDilaog:: ~CVMSimQueryDilaog()
    {
        
    }

// -----------------------------------------------------------------------------
// CVMSimQueryDilaog::SetVMSimPromptL
// Sets query caption on the screen asking for confirmation to copy voice
// mailbox number from sim. 
// -----------------------------------------------------------------------------
//
void CVMSimQueryDilaog::SetVMSimPromptL( const TDesC& aDesC )
    {
    SetPromptL( aDesC );
    }

// -----------------------------------------------------------------------------
// CVMSimQueryDilaog::VMSimDismissDialogL
// Destroys the query dialog
// -----------------------------------------------------------------------------
//
void CVMSimQueryDilaog::VMSimDismissDialogL()
    {
    TryExitL( EEikBidCancel );
    }
 

//  End of File
