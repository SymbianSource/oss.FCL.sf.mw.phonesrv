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


// INCLUDE FILES
#include    <eikenv.h>
#include    <barsread.h>
#include    <badesca.h> // for descriptor arrays
#include    <e32std.h>
#include    <StringLoader.h>
#include    <AknQueryDialog.h>
#include    <aknQueryControl.h>
#include    <eikcapc.h>
#include    <avkon.hrh>
#include    <fepbase.h>
#include    <Aknnumseced.h>
#include    <aknedsts.h>
#include    <featmgr.h> // Used for FeatureManager

#include    "CSatUiTextQueryDialog.h"
#include    "tflogger.h"

const TInt KSatMinTextLengthForT9 = 1;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSatUiTextQueryDialog::CSatUiTextQueryDialog(
    TDes& aDataText,
    const TTone aTone,
    TInt aMinLength,
    TInt aMaxLength) :
    CAknTextQueryDialog(aDataText, aTone),
    iTextMinLength(aMinLength)
    {
    SetMaxLength( aMaxLength );
    }

// Destructor.
CSatUiTextQueryDialog::~CSatUiTextQueryDialog()
    {
    }

// ---------------------------------------------------------
// CSatUiTextQueryDialog::HandleQueryEditorStateEventL
// Show left CBA only if editor text is valid
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CSatUiTextQueryDialog::HandleQueryEditorStateEventL(
    CAknQueryControl* aQueryControl,
    TQueryControlEvent /*aEventType*/,
    TQueryValidationStatus /*aStatus*/)
    {
    TFLOGSTRING( "CSatUiTextQueryDialog::HandleQueryEditorStateEventL calling" )
    if ( aQueryControl->GetTextLength() < iTextMinLength )
        {
        TFLOGSTRING( "CSatUiTextQueryDialog::HandleQueryEditorStateEventL \
            hide softkey" )
        // Hide both left and middle softkey
        ButtonGroupContainer().MakeCommandVisibleByPosition( 
            CEikButtonGroupContainer::ELeftSoftkeyPosition, EFalse );
        ButtonGroupContainer().MakeCommandVisibleByPosition( 
            CEikButtonGroupContainer::EMiddleSoftkeyPosition, EFalse );
        }
    else
        {
        TFLOGSTRING( "CSatUiTextQueryDialog::HandleQueryEditorStateEventL \
            reveal softkey" )
        // Reveal both left and middle softkey
        ButtonGroupContainer().MakeCommandVisibleByPosition( 
            CEikButtonGroupContainer::ELeftSoftkeyPosition, ETrue );
        ButtonGroupContainer().MakeCommandVisibleByPosition( 
            CEikButtonGroupContainer::EMiddleSoftkeyPosition, ETrue );
        }
    TFLOGSTRING( "CSatUiTextQueryDialog::HandleQueryEditorStateEventL exiting" )
    return ETrue;
    }


// ---------------------------------------------------------
// CSatUiTextQueryDialog::PostLayoutDynInitL
// Checks if min length was 0 and shows the OK key
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiTextQueryDialog::PostLayoutDynInitL()
    {
    TFLOGSTRING( "CSatUiTextQueryDialog::PostLayoutDynInitL calling" )
    CAknTextQueryDialog::PostLayoutDynInitL();
    if ( iTextMinLength == 0 )
        {
        TFLOGSTRING( "CSatUiTextQueryDialog::PostLayoutDynInitL \
            reveal softkey" )
        // Reveal both left and middle softkey
        ButtonGroupContainer().MakeCommandVisibleByPosition( 
            CEikButtonGroupContainer::ELeftSoftkeyPosition, ETrue );
        ButtonGroupContainer().MakeCommandVisibleByPosition( 
            CEikButtonGroupContainer::EMiddleSoftkeyPosition, ETrue );
        }
    TFLOGSTRING( "CSatUiTextQueryDialog::PostLayoutDynInitL exiting" )
    }

// ---------------------------------------------------------
// CSatUiTextQueryDialog::PreLayoutDynInitL
// Initializations needed to accept sathidden keymap
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiTextQueryDialog::PreLayoutDynInitL()
    {
    TFLOGSTRING( "CSatUiTextQueryDialog::PreLayoutDynInitL calling" )
    CAknTextQueryDialog::PreLayoutDynInitL();
    
    // Get KFeatureIdAvkonApac value
    FeatureManager::InitializeLibL();      
          
    TBool isApac( FeatureManager::FeatureSupported( KFeatureIdAvkonApac ) );
            
    FeatureManager::UnInitializeLib();
    
    CAknQueryControl* control = QueryControl();
    
    if ( control )
        {
        CCoeControl* controlByLayout = NULL;
        
        if ( EPinLayout == control->QueryType() )
            {
            TFLOGSTRING( "CSatUiTextQueryDialog::PreLayoutDynInitL \
                EPinLayout" )
            controlByLayout = 
                QueryControl()->ControlByLayoutOrNull( EPinLayout );
            if ( controlByLayout )
                {
                TFLOGSTRING( "CSatUiTextQueryDialog::PreLayoutDynInitL \
                    controlByLayout" )
                CEikSecretEditor* editor = 
                    ( CEikSecretEditor* ) controlByLayout;
                CAknEdwinState* editorState = new(ELeave) CAknEdwinState();

                editorState->SetFlags( 
                    EAknEditorFlagNoLRNavigation | EAknEditorFlagNoT9 );
                editorState->SetDefaultInputMode( EAknEditorNumericInputMode );
                editorState->SetCurrentInputMode( EAknEditorNumericInputMode );
                editorState->SetPermittedInputModes( 
                    EAknEditorNumericInputMode );
                editorState->SetDefaultCase( EAknEditorLowerCase );
                editorState->SetCurrentCase( EAknEditorLowerCase );
                editorState->SetPermittedCases( 
                    EAknEditorLowerCase|EAknEditorUpperCase );
                editorState->SetSpecialCharacterTableResourceId( 0 );
                editorState->SetNumericKeymap( 
                    EAknEditorSATHiddenNumberModeKeymap );
                editorState->SetObjectProvider( editor );
                
                // ...............
                MCoeFepAwareTextEditor_Extension1 *ext
                     = ( MCoeFepAwareTextEditor_Extension1* )editor;
                CleanupStack::PushL( ext );
                CleanupStack::PushL( editorState );
                ext->SetStateTransferingOwnershipL( editorState, KNullUid );
                CleanupStack::Pop( editorState );         // editorState
                CleanupStack::Pop( ext ); // ext
                }
            }
        else if ( EDataLayout == control->QueryType() )
            {
            TFLOGSTRING( "CSatUiTextQueryDialog::PreLayoutDynInitL \
                EDataLayout" )
            controlByLayout = 
                QueryControl()->ControlByLayoutOrNull( EDataLayout );
            if ( controlByLayout )
                {
                CEikEdwin* editor = ( CEikEdwin* ) controlByLayout;
                if ( !isApac )
                    {
                    if ( KSatMinTextLengthForT9 >= iTextMaxLength )
                        {
                        editor->SetAknEditorFlags( EAknEditorFlagNoT9 );
                        }
                    editor->AddFlagToUserFlags( EEikEdwinNoHorizScrolling );
                    editor->AddFlagToUserFlags( EEikEdwinResizable );
                    editor->SetAknEditorInputMode( EAknEditorTextInputMode );
                    editor->SetAknEditorAllowedInputModes( 
                        EAknEditorTextInputMode );
                    }
                else
                    {
                    TFLOGSTRING( "CSatUiTextQueryDialog::PreLayoutDynInitL \
                        isApac ETrue" )
                    if ( KSatMinTextLengthForT9 >= iTextMaxLength )
                        {
                        editor->SetAknEditorFlags( EAknEditorFlagNoT9 );
                        }
                    else
                        {
                        editor->SetAknEditorFlags( 0 );    
                        }
                    }
                }
            }
        else if ( EPhoneLayout == control->QueryType() )
            {
            TFLOGSTRING( "CSatUiTextQueryDialog::PreLayoutDynInitL \
                EPhoneLayout" )
            controlByLayout = 
                QueryControl()->ControlByLayoutOrNull( EPhoneLayout );
            if ( controlByLayout )
                {
                CEikEdwin* editor = ( CEikEdwin* ) controlByLayout;
                
                if ( !isApac )
                    {                    
                    editor->AddFlagToUserFlags( EEikEdwinNoHorizScrolling );
                    editor->AddFlagToUserFlags( EEikEdwinResizable );
                    editor->AddFlagToUserFlags( EEikEdwinAlternativeWrapping );
                    }
                else
                    {
                    TFLOGSTRING( "CSatUiTextQueryDialog::PreLayoutDynInitL \
                        isApac ETrue" )
                    editor->SetAknEditorFlags( 0 );
                    }
                }
            }
        }
    TFLOGSTRING( "CSatUiTextQueryDialog::PreLayoutDynInitL exiting" )
    }

// ---------------------------------------------------------
// CSatUiTextQueryDialog::NeedToDismissQueryL
// Checks for hash key (workaround)
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CSatUiTextQueryDialog::NeedToDismissQueryL( const TKeyEvent& /* aKeyEvent */ )
    {
    TFLOGSTRING( "CSatUiTextQueryDialog::NeedToDismissQueryL calling-exiting" )
    return EFalse;
    }

// ---------------------------------------------------------
// CSatUiTextQueryDialog::OfferKeyEventL
// Hash key accepted (workaround)
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CSatUiTextQueryDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                TEventCode aType )
    {
    TFLOGSTRING( "CSatUiTextQueryDialog::OfferKeyEventL calling" )
    TKeyEvent keyEvent = aKeyEvent;
    if ( aKeyEvent.iScanCode == EStdKeyHash && aType==EEventKey )
        {
        keyEvent.iScanCode = 0;
        }

    TFLOGSTRING( "CSatUiTextQueryDialog::OfferKeyEventL exiting" )
    return CAknTextQueryDialog::OfferKeyEventL( keyEvent, aType );
    }


//  End of File
