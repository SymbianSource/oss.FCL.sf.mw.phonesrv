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
*     Implementation of CCbsUiEditTopicDlg.
*
*
*/


// INCLUDES
#include    <avkon.hrh>             // EMultilineFirstLine, EMultilineSecondLine
#include    <layoutmetadata.cdl.h>  // Layout_Meta_Data::IsLandscapeOrientation()  
#include    <AknUtils.h>            // AknTextUtils
#include    <aknQueryControl.h>
#include    <cbsuiApp.rsg>
#include    <StringLoader.h>
#include    <AknDef.h>              // KEikDynamicLayoutVariantSwitch
#include    "Cbsuiapp.hrh"
#include    "CCbsUiEditTopicDlg.h"
#include    "CCbsUiTopicListView.h"
#include    "CbsUiPanic.h"
#include    "CbsUiUtility.h"


//CONSTANTS
_LIT(KDummyString,"");

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiEditTopicDlg::CCbsUiEditTopicDlg( CCbsUiTopicListView& aTopicListView,
                                       TBool aType ) 
                                       : 
CAknMultiLineDataQueryDialog(ENoTone),    
iTopicListView(aTopicListView),
iType(aType)   
    {
    }

// ---------------------------------------------------------
// CCbsUiEditTopicDlg::ConstructL
// 
// ---------------------------------------------------------
//
void CCbsUiEditTopicDlg::ConstructL( TDes& aDataText, TDes& aDataText2 )
    {
    SetDataL(aDataText, aDataText2);
    iUpdateLeftSoftKeyFirstTime = ETrue;   
    }

// ---------------------------------------------------------
// CCbsUiEditTopicDlg::NewL
// 
// ---------------------------------------------------------
//
CCbsUiEditTopicDlg* CCbsUiEditTopicDlg::NewL( TDes& aDataText, TDes& aDataText2,
                                             CCbsUiTopicListView& aTopicListView, TBool aType )
    {
    CCbsUiEditTopicDlg* self = 
        new (ELeave) CCbsUiEditTopicDlg(aTopicListView, aType);
    CleanupStack::PushL(self);
    self->ConstructL(aDataText, aDataText2);
    CleanupStack::Pop(); // self
    
    return self;
    }

// Destructor
CCbsUiEditTopicDlg::~CCbsUiEditTopicDlg()
    {
    }

// ---------------------------------------------------------
// CCbsUiEditTopicDlg::OkToExitL
// 
// ---------------------------------------------------------
//
TBool CCbsUiEditTopicDlg::OkToExitL( TInt aButtonId )
    {
    // just to update the values
    TBool okToExit( CAknMultiLineDataQueryDialog::OkToExitL( aButtonId ) );
    if ( okToExit )
        {
        TBuf<KCbsDbTopicNameLength> dummy( KDummyString );
        TBuf<KCbsDbTopicNameLength> topicNumberStr;
        topicNumberStr = SecondData( dummy );
        AknTextUtils::ConvertDigitsTo( topicNumberStr, EDigitTypeWestern );
        TLex lex( topicNumberStr );
        TCbsTopicNumber tn;
        
        // No need to panic!
        User::LeaveIfError( lex.Val( tn, EDecimal ) );
        
        TCbsTopicName topicName;
        
        topicName.Append( FirstData( dummy ) );
        
        TInt ret( KErrGeneral ); // just to differ from KErrNone
        
        ret = iTopicListView.EditTopic( iType, tn, topicName );
        
        switch ( ret )
            {
            case KErrDiskFull:
                User::Leave( KErrDiskFull );
                break;
            case KErrNone:
                // do nothing
                break;
            default:
                // shows the error note and the dialog is not closed
                CbsUiUtility::ShowErrorNoteL( R_TEXT_NOTE_NUMBER_ERROR );
                okToExit = EFalse;
                SetCursorAndFocusL();
                break;
            }
        }
    
    return okToExit;    
    }


// ---------------------------------------------------------
// CCbsUiEditTopicDlg::LineChangedL
// 
// ---------------------------------------------------------
//
void CCbsUiEditTopicDlg::LineChangedL( TInt /*aControlId*/ )
    {
    UpdateLeftSoftKeyL();
    }


// ----------------------------------------------------
// CCbsUiEditTopicDlg::HandleResourceChange
//
// ----------------------------------------------------
void CCbsUiEditTopicDlg::HandleResourceChange( TInt aType )
    {
    CAknMultiLineDataQueryDialog::HandleResourceChange( aType );
    
    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        TRAPD( ignore, UpdateLeftSoftKeyL( ) );
        if( ignore )
            {
            ignore = KErrNone; //Prevent ARMV5 compilation varning
            }
        }    
    }


// ---------------------------------------------------------
// CCbsUiEditTopicDlg::UpdateLeftSoftKeyL
// Note: iUpdateLeftSoftKeyFirstTime must use because when we
// go to UpdateLeftSoftKeyL() method first time then IdOfFocusControl()
// method doesn't work ok yet.
// ---------------------------------------------------------
//
void CCbsUiEditTopicDlg::UpdateLeftSoftKeyL()
    {
    if( ( Layout_Meta_Data::IsLandscapeOrientation() ) && 
        ( ( IdOfFocusControl() == EMultilineFirstLine) || iUpdateLeftSoftKeyFirstTime ) )
        {
        MakeLeftSoftkeyVisible( ETrue );
        }
    else
        {
        TBuf<EMaxTopicNumberLength> buf;
        SecondControl()->GetText( buf );
        AknTextUtils::ConvertDigitsTo( buf, EDigitTypeWestern );
        TLex lex( buf );
        TInt topicNumber( -1 );
        if ( lex.Val( topicNumber ) == KErrNone )
            {
            MakeLeftSoftkeyVisible( ETrue );
            }
        else
            {
            MakeLeftSoftkeyVisible( EFalse );
            }
        }
    iUpdateLeftSoftKeyFirstTime = EFalse;  
    }

// ---------------------------------------------------------
// CCbsUiEditTopicDlg::SetCursorAndFocusL
// 
// ---------------------------------------------------------
//
void CCbsUiEditTopicDlg::SetCursorAndFocusL()
    {
    TryChangeFocusToL(EMultilineSecondLine);
    CAknQueryControl* control = 
        STATIC_CAST( CAknQueryControl*, 
        Control(EMultilineSecondLine) );
    CEikEdwin* edwin = STATIC_CAST(
        CEikEdwin*, control->ControlByLayoutOrNull(EDataLayout) );
    edwin->SetSelectionL( control->GetTextLength(),0 );  
    }


//  End of File  
