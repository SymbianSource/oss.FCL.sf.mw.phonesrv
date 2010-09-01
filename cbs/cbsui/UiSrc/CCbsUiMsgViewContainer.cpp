/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CCbsUiMsgViewContainer class.
*
*
*/


// INCLUDES
#include <bldvariant.hrh>       // for feature flags
#include <eikenv.h>
#include <AknLayoutDef.h>       // enum TAknLayoutId
#include <aknenv.h>             // for iAvkonEnv
#include <AknUtils.h>           // AknLayoutUtils
#include <sysutil.h>            // FFS critical level check
#include <eikrted.h>
#include <barsread.h>
#include <txtrich.h>
#include <calslbs.h>
#include <baclipb.h>
#include <aknkeys.h>
#include <applayout.cdl.h>      // Multiline_Message_writing_texts_Line_3
#include <AknLayout2ScalableDef.h>        // TAknLayoutScalableParameterLimits
#include <aknlayoutscalable_apps.cdl.h>   // list_single_mce_message_pane_ParamLimits().iRows
#include <AknDef.h>                       // KEikDynamicLayoutVariantSwitch
#include <AknsConstants.h>      // KAknsMessageSkinChange
#include <aknappui.h>
#include <AknsBasicBackgroundControlContext.h> // CAknsBasicBackgroundControlContext
#include <AknsUtils.h>                    // MAknsSkinInstance
#include <AknsDrawUtils.h>                // AknsDrawUtils
#include <cbsuiApp.rsg>
#include "CbsUiId.h"
#include <csxhelp/cbs.hlp.hrh>
#include "CCbsUIMsgViewContainer.h"
#include "CCbsUIMsgView.h"
#include "Cbsuiapp.hrh"
#include "CbsUiPanic.h"
#include <gdi.h>



// LOCAL CONSTANTS AND MACROS
const TInt KEditorNumOfLines = 7;
const TInt KFirstScreen = 0;
const TInt KOneScreen = 1;
const TInt KMsgNumberOfControls = 1;
const TInt KMsgControlIndexEditor = 0;


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
CCbsUiMsgViewContainer::CCbsUiMsgViewContainer( CCbsUiMsgView& aMsgView )
: iMsgView(aMsgView)      
    {
    }

// Symbian OS default constructor can leave.
void CCbsUiMsgViewContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    SetMopParent(&iMsgView);
    
    iEditor = CreateEditorL();
    SetReadOnly(ETrue);
    
    SetRect(aRect);

    // for Skin Support, Create background control context:
    iBackGroundControlContext = CAknsBasicBackgroundControlContext::NewL(
                    KAknsIIDQsnBgAreaMain,  // Default mainpane skinning
                    Rect(),                 // Layout to the entire client rect
                    EFalse );
    if ( iBackGroundControlContext )
        {
        iEditor->SetSkinBackgroundControlContextL( iBackGroundControlContext );
        }
    }


// Two-phased constructor.
CCbsUiMsgViewContainer* CCbsUiMsgViewContainer::NewL( CCbsUiMsgView& aMsgView, 
                                                     const TRect& aRect )
    {    
    CCbsUiMsgViewContainer* self = new (ELeave) 
        CCbsUiMsgViewContainer(aMsgView);    
    
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsUiMsgViewContainer::~CCbsUiMsgViewContainer()
    {
    delete iBackGroundControlContext;
    delete iEditor;
    delete iSBFrame;
    }


// ---------------------------------------------------------
// CCbsUiMsgViewContainer::CountComponentControls
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiMsgViewContainer::CountComponentControls() const
    {
    TInt countScrollBarComponents( 0 );
    if ( iSBFrame )
        {
        countScrollBarComponents = iSBFrame->CountComponentControls();
        }
    return KMsgNumberOfControls + countScrollBarComponents;
    }


// ---------------------------------------------------------
// CCbsUiMsgViewContainer::ComponentControl
// 
// 
// ---------------------------------------------------------
//
CCoeControl* CCbsUiMsgViewContainer::ComponentControl( TInt aIndex ) const
    {
    CCoeControl* control = NULL;
    
    TInt countScrollBarComponents( 0 );
    if ( iSBFrame )
        {
        countScrollBarComponents = iSBFrame->CountComponentControls();
        }
    
    switch ( aIndex )
        {
        case KMsgControlIndexEditor:
            control = iEditor;
            break;
        default:
            if ( iSBFrame
                &&  aIndex >= KMsgNumberOfControls 
                &&  aIndex < countScrollBarComponents + KMsgNumberOfControls )
                {
                control = iSBFrame->ComponentControl( aIndex - KMsgNumberOfControls );
                }
            else
                {
                control = NULL;
                }
            break;
        }
    return control;
    }


// ---------------------------------------------------------
// CCbsUiMsgViewContainer::OfferKeyEventL
// 
// 
// ---------------------------------------------------------
//
TKeyResponse CCbsUiMsgViewContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                                    TEventCode /*aType*/ )
    {    
    __ASSERT_DEBUG( iEditor, CbsUiPanic(EEditorNullError) );
    
    switch ( aKeyEvent.iCode )
        {
        case EKeyLeftArrow:
            {
            if( AknLayoutUtils::LayoutMirrored() )
                {
                iMsgView.MoveToNextMessageL();
                }
            else
                {
                iMsgView.MoveToPreviousMessageL();
                }
            break;
            }
        case EKeyRightArrow:
            {
            if( AknLayoutUtils::LayoutMirrored() )
                {
                iMsgView.MoveToPreviousMessageL();
                }
            else
                {
                iMsgView.MoveToNextMessageL();
                }
            break;
            }
        case EKeyUpArrow :
            {
            ScrollMessageUpL();
            break;
            }
        case EKeyDownArrow:
            {
            ScrollMessageDownL();
            break;
            }
        case EKeyOK:
            {
            iMsgView.ProcessCommandL( ECbsUiappCmdOkKey );
            break;
            }
        case EKeyBackspace:
            {
            iMsgView.ProcessCommandL( ECbsUiappCmdMsgMenuDelete );
            break;
            }
        default:
            {
            break;
            }
        }
    
    return EKeyWasConsumed;
    }


// ---------------------------------------------------------
// CCbsUiMsgViewContainer::UpdateMessageL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::UpdateMessageL( const TDesC& aMessage )
    {
    SetData16L(aMessage);
    
    InitializeMessageInfoL();
    
    CreateScrollBarFrameL();
    UpdateScrollIndicatorL();
    
    iEditor->DrawNow();
    }

// ---------------------------------------------------------
// CCbsUiMsgViewContainer::SizeChanged
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::SizeChanged()
    {
    if ( iEditor ) 
        {
        TRAPD( ignore, iEditor->SelectAllL() ); // This is needed before calling layoutedwin!
        if( ignore )
            {
            ignore = KErrNone; //Prevent ARMV5 compilation varning
            }
        if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
            {
            TAknLayoutScalableParameterLimits scalbleParamLimits;
            scalbleParamLimits = AknLayoutScalable_Apps::list_single_mce_message_pane_ParamLimits();
            AknLayoutUtils::LayoutEdwin( iEditor, Rect(), AppLayout::Multiline_Message_writing_texts_Line_3(scalbleParamLimits.LastRow() ) );
            }
        else
            {
            AknLayoutUtils::LayoutEdwin( iEditor, Rect(), AppLayout::Multiline_Message_writing_texts_Line_3( KEditorNumOfLines ) );
            }
        if ( iBackGroundControlContext )
            {
            TRAPD( ignore, iEditor->SetSkinBackgroundControlContextL( iBackGroundControlContext ) );
            if( ignore )
                {
                ignore = KErrNone; //Prevent ARMV5 compilation varning
                }
            }
        iEditor->DrawNow();
        }
    }


// ---------------------------------------------------------
// CCbsUiMsgViewConctainer::CreateEditorL
// 
// 
// ---------------------------------------------------------
//
CEikRichTextEditor* CCbsUiMsgViewContainer::CreateEditorL() const
    {
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC(reader, R_MSG_READER);
    
    CEikRichTextEditor* editor = new (ELeave) CEikRichTextEditor;
    CleanupStack::PushL( editor );
    editor->SetContainerWindowL(*this);
    editor->ConstructFromResourceL( reader );
    CleanupStack::Pop(); // editor
    CleanupStack::PopAndDestroy(); // reader    
    
    __ASSERT_DEBUG( editor, CbsUiPanic(EEditorNullError) );
    
    return editor;
    }

// ---------------------------------------------------------
// CCbsUiMsgViewConctainer::Draw
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::Draw( const TRect& aRect ) const
    {
    CWindowGc& aGc = SystemGc();
    
    // for clearing gap between editor and the view rect
    if ( IsReadyToDraw() )
        {
        aGc.SetBrushColor( AKN_LAF_COLOR (0) );
        DrawUtils::ClearBetweenRects( aGc, aRect, iEditor->Rect() );
        if ( iBackGroundControlContext )
            {//Draw the skin background
            MAknsSkinInstance* skin = AknsUtils::SkinInstance();
            AknsDrawUtils::Background( skin, iBackGroundControlContext, this, aGc, aRect );
            }
        }
    }


// ---------------------------------------------------------
// CCbsUiMsgViewContainer::InitializeMessageInfoL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::InitializeMessageInfoL()
    {
    
    iEditor->TextLayout()->SetAmountToFormat( CTextLayout::EFFormatAllText );
    
    iEditor->CreateTextViewL();
    iEditor->NotifyNewFormatL();
    TInt numberOfLines = iEditor->TextLayout()->NumFormattedLines();
    
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
        {
        TInt linesInScreen;
        TAknLayoutScalableParameterLimits scalbleParamLimits;
        scalbleParamLimits = AknLayoutScalable_Apps::list_single_mce_message_pane_ParamLimits();
        linesInScreen = scalbleParamLimits.LastRow();
        iNumberOfScreens = ( numberOfLines - 1 ) / linesInScreen + 1;
        }
    else
        {
        iNumberOfScreens = ( numberOfLines - 1 ) / KEditorNumOfLines + 1;
        }

    iCurrentScreen = KFirstScreen;
    }


// ---------------------------------------------------------
// CCbsUiMsgViewContainer::CreateScrollBarFrameL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::CreateScrollBarFrameL()
    {        
    if ( !iSBFrame )
        {
        iSBFrame = new (ELeave) CEikScrollBarFrame( iEditor, this, ETrue );
        }
    
    CAknAppUiBase* appUi = static_cast<CAknAppUiBase*>( iEikonEnv->EikAppUi() );
    if ( AknLayoutUtils::DefaultScrollBarType( appUi ) == CEikScrollBarFrame::EDoubleSpan )
        {
        // For EDoubleSpan type scrollbar
        iSBFrame->CreateDoubleSpanScrollBarsL( EFalse, EFalse ); // non-window owning scrollbar            
        iSBFrame->SetTypeOfVScrollBar( CEikScrollBarFrame::EDoubleSpan );
        iSBFrame->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto );
        TAknWindowLineLayout scrollPane = AknLayoutScalable_Apps::scroll_pane_cp017().LayoutLine();
        AknLayoutUtils::LayoutVerticalScrollBar( iSBFrame, Rect(), scrollPane );
        }
    else
        {
        // For EArrowHead type scrollbar
        iSBFrame->SetTypeOfVScrollBar( CEikScrollBarFrame::EArrowHead );
        iSBFrame->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto );
        }
    ActivateL();
    }


// ---------------------------------------------------------
// CCbsUiMsgViewContainer::UpdateScrollIndicatorL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::UpdateScrollIndicatorL() const
    {
    if ( !iSBFrame )
        {
        return;
        }
    
    
    TEikScrollBarModel hSbarModel;
    TEikScrollBarModel vSbarModel;
    vSbarModel.iThumbSpan = 1;
    
    vSbarModel.iThumbPosition = iCurrentScreen; // where is the position  
    // how many items
    // if there is only one screen we don't need scroll bars (= 0)
    
    if ( iNumberOfScreens == KOneScreen )
        {
        vSbarModel.iScrollSpan = 0;
        iSBFrame->SetScrollBarVisibilityL( 
            CEikScrollBarFrame::EOff, 
            CEikScrollBarFrame::EOff );
        }
    else
        {
        vSbarModel.iScrollSpan = iNumberOfScreens;
        iSBFrame->SetScrollBarVisibilityL( 
            CEikScrollBarFrame::EOff, 
            CEikScrollBarFrame::EAuto );
        }
    
    if ( iSBFrame->TypeOfVScrollBar() == CEikScrollBarFrame::EDoubleSpan )
        {    
        // For EDoubleSpan type scrollbar
        TAknDoubleSpanScrollBarModel hDsSbarModel( hSbarModel );
        TAknDoubleSpanScrollBarModel vDsSbarModel( vSbarModel );
        
        TEikScrollBarFrameLayout layout;
        layout.iTilingMode = TEikScrollBarFrameLayout::EInclusiveRectConstant;
        TRect inclusiveRect( Rect( ) ); // We need to have two of these variables
        TRect clientRect( Rect( ) );
        
        iSBFrame->TileL( &hDsSbarModel, &vDsSbarModel, clientRect, inclusiveRect, layout );
        iSBFrame->SetVFocusPosToThumbPos( vSbarModel.iThumbPosition );
        }
    else
        {
        iSBFrame->Tile( &vSbarModel );
        iSBFrame->SetVFocusPosToThumbPos( vSbarModel.iThumbPosition );
        }
    iSBFrame->DrawScrollBarsNow();
    
    }

// ---------------------------------------------------------
// CCbsUiMsgViewContainer::UpdateCurrentScreen
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::UpdateCurrentScreen( TBool aIncrease )
    {
    if ( aIncrease )
        {
        if ( iCurrentScreen < iNumberOfScreens - 1 )
            {
            iCurrentScreen++;
            }
        }
    else
        {
        if ( iCurrentScreen > KFirstScreen )
            {
            iCurrentScreen--;
            }
        }
    
    }

// ---------------------------------------------------------
// CCbsUiMsgViewContainer::SetReadOnly
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::SetReadOnly( TBool aReadOnly )
    {
    __ASSERT_DEBUG(iEditor, CbsUiPanic(EErrNullPointer) );
    iEditor->SetReadOnly(aReadOnly);
    }

// ---------------------------------------------------------
// CCbsUiMsgViewContainer::SetDataContentL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::SetDataContentL( CRichText& aText )
    {
    iEditor->SetDocumentContentL(aText);
    iEditor->SelectAllL(); // This is needed before calling layoutedwin!


    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
        {
        TAknLayoutScalableParameterLimits scalbleParamLimits;
        scalbleParamLimits = AknLayoutScalable_Apps::list_single_mce_message_pane_ParamLimits();
        AknLayoutUtils::LayoutEdwin( iEditor, Rect(), AppLayout::Multiline_Message_writing_texts_Line_3(scalbleParamLimits.LastRow() ) );
        }
    else
        {
        AknLayoutUtils::LayoutEdwin( iEditor, Rect(), AppLayout::Multiline_Message_writing_texts_Line_3( KEditorNumOfLines ) );
        }
    }

// ---------------------------------------------------------
// CCbsUiMsgViewContainer::SetData16L
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::SetData16L( const TDesC& aBuf )
    {
    __ASSERT_DEBUG(aBuf.Length() > 0, CbsUiPanic(EErrInvalidValue) );
    
    CParaFormatLayer* globalParaLayer = CParaFormatLayer::NewL();
    CleanupStack::PushL(globalParaLayer);
                   
    CCharFormatLayer* globalCharLayer = NULL;   
    TRgb textColor;

    TCharFormat charFormat;
    TCharFormatMask formatMask;
        
    ( reinterpret_cast< CGlobalText* >( iEditor->Text() ) )
        ->GetCharFormat( charFormat, formatMask, 0, 0 );                        

    TInt error = AknsUtils::GetCachedColor(
        AknsUtils::SkinInstance(),
        textColor,
        KAknsIIDQsnTextColors,
        EAknsCIQsnTextColorsCG6 );
        
    if ( error != KErrNone )
        {
        textColor = KRgbBlack;
        }
                
    charFormat.iFontPresentation.iTextColor = textColor;
    formatMask.SetAll();

    globalCharLayer = CCharFormatLayer::NewL( charFormat, formatMask );
    CleanupStack::PushL( globalCharLayer );
     
    CRichText* richText = CRichText::NewL( globalParaLayer, globalCharLayer );
    CleanupStack::PushL( richText );
    
    TInt length( aBuf.Size() / 2 );
    
    for (TInt i = 0; i < length; i++)
        {     
        if ( aBuf[i] == EKeyLineFeed )
            {            
            richText->InsertL( i, CEditableText::EParagraphDelimiter );
            }
        else 
            {
            richText->InsertL( i, aBuf[i] );
            }
        }
    
    SetDataContentL( *richText );
    
    iEditor->SetCharFormatLayer( globalCharLayer );
                
    CleanupStack::PopAndDestroy();  // richText
    CleanupStack::Pop();            // globalCharLayer
    CleanupStack::PopAndDestroy();  // globalParaLayer
    }

// ---------------------------------------------------------
// CCbsUiMsgViewContainer::GetEditor
// 
// 
// ---------------------------------------------------------
//
CRichText* CCbsUiMsgViewContainer::GetEditor() const
    {
    __ASSERT_DEBUG(iEditor->RichText()->LdDocumentLength() > 0, 
        CbsUiPanic(EErrInvalidValue) );
    
    return iEditor->RichText();
    }


// ---------------------------------------------------------
// CCbsUiMsgViewContainer::CopyToClipboard
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::CopyToClipboardL() const
    {
    TInt msgLength;
    
    msgLength = iEditor->TextLength();
    if ( SysUtil::FFSSpaceBelowCriticalLevelL(  &iCoeEnv->FsSession(), 2 * msgLength ) )
        {
        User::Leave( KErrDiskFull );
        }
    
    CPlainText* plainText = CPlainText::NewL();
    
    CleanupStack::PushL(plainText);
    plainText->InsertL( 0, iEditor->RichText()->Read(0) );
    plainText->ConvertAllFieldsToText();
    
    CClipboard* cb = CClipboard::NewForWritingLC( iCoeEnv->FsSession() );
    plainText->CopyToStoreL( 
        cb->Store(), 
        cb->StreamDictionary(), 
        0, 
        plainText->DocumentLength() );
    
    cb->CommitL();
    CleanupStack::PopAndDestroy(2);  //cb, plainText
    }


// ----------------------------------------------------
// CCbsUiMsgViewContainer::HandleResourceChange
//
//
// ----------------------------------------------------
void CCbsUiMsgViewContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        TRAPD( ignore, iMsgView.HandleRectChangeInContainerL() );
        if ( iBackGroundControlContext )
            {
            iBackGroundControlContext->SetRect( Rect() );
            }
        
        if( ignore )
            {
            ignore = KErrNone; //Prevent ARMV5 compilation varning
            }
        }
    else
        {
        if( ( aType == KAknsMessageSkinChange ) && ( iBackGroundControlContext ) )
            {
            TRAPD( ignore, iEditor->SetSkinBackgroundControlContextL( iBackGroundControlContext ) );
            if( ignore )
                {
                ignore = KErrNone; //Prevent ARMV5 compilation varning
                }
            } 
        }
    
     }


// ----------------------------------------------------
// CCbsUiMsgViewContainer::HandleScrollEventL
// ----------------------------------------------------
void CCbsUiMsgViewContainer::HandleScrollEventL( CEikScrollBar* aScrollBar, 
                                         TEikScrollEvent aEventType )
    {
    switch( aEventType )
        {
        case EEikScrollUp:
        case EEikScrollPageUp:
            {
            ScrollMessageUpL();
            break;
            }
        case EEikScrollDown:
        case EEikScrollPageDown:
            {
            ScrollMessageDownL();
            break;
            }
        case EEikScrollThumbDragVert:
            {
            if( aScrollBar->ThumbPosition() > iCurrentScreen )
                {
                ScrollMessageDownL();
                }
            else if ( aScrollBar->ThumbPosition() < iCurrentScreen )
                {
                ScrollMessageUpL();    
                }
            break;
            }
        default:
            break;
        }
    }

// ----------------------------------------------------
// CCbsUiMsgViewContainer::ScrollMessageUpL
// ----------------------------------------------------
void CCbsUiMsgViewContainer::ScrollMessageUpL()
    {
    if( iCurrentScreen > 0 )
        {
        iEditor->MoveDisplayL( TCursorPosition::EFPageUp );
        iEditor->MoveDisplayL( TCursorPosition::EFLineUp );
        UpdateCurrentScreen( EFalse );
        UpdateScrollIndicatorL();
        }
    }

// ----------------------------------------------------
// CCbsUiMsgViewContainer::ScrollMessageDownL
// ----------------------------------------------------
void CCbsUiMsgViewContainer::ScrollMessageDownL()
    {
    if( iCurrentScreen < iNumberOfScreens-1 )
        {
        iEditor->MoveDisplayL( TCursorPosition::EFPageDown );
        iEditor->MoveDisplayL( TCursorPosition::EFLineDown );
        UpdateCurrentScreen( ETrue );
        UpdateScrollIndicatorL();
        }
    }

// ---------------------------------------------------------
// CCbsUiMsgViewContainer::GetHelpContext
// 
// Gets the control's help context.
// ---------------------------------------------------------
//
void CCbsUiMsgViewContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = TUid::Uid(KUidCbsUiappDef);
    aContext.iContext = KCBS_HLP_MSG;
    }


//  End of File 
