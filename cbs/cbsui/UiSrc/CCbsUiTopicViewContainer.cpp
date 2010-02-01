/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCbsUiTopicViewContainer is container for CCbsUiTopicView. 
*               It has one control, a list box, which is showing information
*               about messages in one topic.
*
*
*/


//  INCLUDES
#include <AknsConstants.h>    // KAknsMessageSkinChange
#include <cbsuiapp.mbg>       // TMbmCbsuiapp
#include <AknsUtils.h>        // CreateIconL, SkinInstance
#include <akncontext.h>       // CAknContextPane
#include <AknUtils.h>         // AknLayoutUtils 
#include <AknDef.h>           // KEikDynamicLayoutVariantSwitch
#include <e32std.h>
#include <calslbs.h>
#include <barsread.h>
#include <gulicon.h>
#include <eikfrlbd.h>
#include <eikclbd.h>
#include <eikslb.h>
#include <eiklbx.h>
#include <avkon.hrh>
#include <aknlists.h>
#include <AknIconArray.h>
#include <data_caging_path_literals.hrh> // KDC_RESOURCE_FILES_DIR
#include <cbsuiApp.rsg>
#include "CbsUiId.h"
#include <csxhelp/cbs.hlp.hrh>
#include "Cbsuiapp.hrh"
#include "CCbsUiTopicViewContainer.h"
#include "CCbsUiTopicView.h"
#include "CbsUiUtility.h"
#include "CbsUiPanic.h"
#include "CbsUiConstants.h"
 

// CONSTANTS
_LIT(KDirAndCbsUiAppMbmFile,"z:cbsuiapp.mbm");
_LIT(KSeparator1, "1\t");
_LIT(KSeparator2, "0\t");
_LIT(KSeparator3, "\t2");

const TInt KNumberOfIconsInTopicViewcontainer = 6;


//DATA TYPES
enum TopicViewIconArrayIndexes
    {
    ECbsPropCbMsgUnread = 0,
    ECbsPropCbMsg,
    ECbsIndiCbKeptAdd
    };



// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiTopicViewContainer::CCbsUiTopicViewContainer( CCbsUiTopicView& aTopicView )
                                                   : iTopicView( aTopicView )
    {
    }

// Symbian OS default constructor can leave.
void CCbsUiTopicViewContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    SetMopParent(&iTopicView);
    
    iListBox = new (ELeave) CAknSingleGraphicStyleListBox;
    iListBox->SetContainerWindowL(*this);
    iListBox->ConstructL(this, EEikListBoxMultipleSelection);
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff,
        CEikScrollBarFrame::EAuto );
    
    CDesCArrayFlat* array = 
        new (ELeave) CDesCArrayFlat( KNumberOfTemporaryMessages );
    
    // changing the owner, shouldn't leave 
    iListBox->Model()->SetItemTextArray(array);
    iListBox->SetListBoxObserver( this );
    
    SetRect(aRect);
    LoadIconsL();
    }

// Two-phased constructor.
CCbsUiTopicViewContainer* CCbsUiTopicViewContainer::NewL( CCbsUiTopicView& aTopicView, const TRect& aRect )
    {
    CCbsUiTopicViewContainer* self = 
        new (ELeave) CCbsUiTopicViewContainer(aTopicView);
    
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsUiTopicViewContainer::~CCbsUiTopicViewContainer()
    {
    delete iListBox;
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::ReloadStarts
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicViewContainer::ReloadStarts() const
    {
    MDesCArray* itemList = iListBox->Model()->ItemTextArray();
    CDesCArray* itemArray = STATIC_CAST(CDesCArray*, itemList);
    itemArray->Reset();
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::ReloadOfferL
// 
// 
// ---------------------------------------------------------
// 
void CCbsUiTopicViewContainer::ReloadOfferL( const TCbsMessage& aMessage, const TDesC& aBuffer ) const
    {
    MDesCArray* itemList = iListBox->Model()->ItemTextArray();
    CDesCArray* itemArray = STATIC_CAST(CDesCArray*, itemList);
    
    TInt titleLength( (aBuffer.Length() > KMaxMessageTitleLength ) ? 
                       KMaxMessageTitleLength : aBuffer.Length() );
    
    HBufC* buffer = HBufC::NewMaxLC( titleLength + KSeparator1().Length() + KSeparator2().Length() +
        KSeparator3().Length() );
    
    TPtr ptr = buffer->Des();    
    
    // First column icon.
    if ( aMessage.iRead )
        {
        ptr = KSeparator1;
        }
    else
        {
        ptr = KSeparator2;
        }
    
    for ( TInt index(0); index < titleLength; index++ )
        {
        TChar character = aBuffer[index];
        
        // if character is line break, then stop
        if ( character == EKeyLineFeed
            || character == EKeyEnter )
            {
            break;
            }
        
        ptr.Append( character );
        }
    
    // And finally saved icon.
    if ( aMessage.iPermanent )
        {
        ptr.Append(KSeparator3);
        }
    
    // Finally append!
    itemArray->AppendL(*buffer);
    CleanupStack::PopAndDestroy(); // buffer
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::ReloadEndsL
// 
// 
// ---------------------------------------------------------
//    
void CCbsUiTopicViewContainer::ReloadEndsL()
    {  
    // Inform listbox that some items has been added.
    iListBox->HandleItemAdditionL();
    // check if listbox has any item. if numberOfItems == 0
    // then disable scrollbars
    if ( iListBox->Model()->NumberOfItems() == 0 )
    	{
    	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
    		CEikScrollBarFrame::EOff,
    		CEikScrollBarFrame::EOff );
    	}
    else
    	{
    	iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
    		CEikScrollBarFrame::EOff,
    		CEikScrollBarFrame::EAuto );
    	}	
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::CurrentPosition
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicViewContainer::CurrentPosition() const
    {
    return iListBox->CurrentItemIndex();
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::SetCurrentItemIndex
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicViewContainer::SetCurrentItemIndex( TInt aItemIndex ) const
    {
    __ASSERT_DEBUG( aItemIndex >= 0, CbsUiPanic(EErrInvalidValue) );
    
    MDesCArray* itemList = iListBox->Model()->ItemTextArray();
    CDesCArray* itemArray = STATIC_CAST(CDesCArray*, itemList);
    
    TInt count( itemArray->MdcaCount() );
    
    if ( count > aItemIndex && count >= 0 )
        {
        iListBox->SetCurrentItemIndexAndDraw(aItemIndex);
        }
    }

// ----------------------------------------------------
// CCbsUiTopicViewContainer::HandleListBoxEventL
//
// ----------------------------------------------------
void CCbsUiTopicViewContainer::HandleListBoxEventL(	CEikListBox* /*aListBox*/,
                                                     TListBoxEvent aEventType )
    {
    switch ( aEventType )
        {
        case EEventItemSingleClicked:
            {
            TKeyEvent keyEvent;
            keyEvent.iCode = EKeyOK;
            OfferKeyEventL( keyEvent, EEventKey );
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::GetListBox
// 
// 
// ---------------------------------------------------------
//
CEikColumnListBox* CCbsUiTopicViewContainer::GetListBox() 
    {
    return iListBox;
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::CountComponentControls
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicViewContainer::CountComponentControls() const
    {
    return 1;
    }

// ---------------------------------------------------------
// 
// 
// 
// ---------------------------------------------------------
//
CCoeControl* CCbsUiTopicViewContainer::ComponentControl( TInt /*aIndex*/ ) const 
    { 
    return iListBox;
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::OfferKeyEventL
// 
// 
// ---------------------------------------------------------
//
TKeyResponse CCbsUiTopicViewContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {    
    TKeyResponse result = EKeyWasNotConsumed;
    
    switch (aKeyEvent.iCode)
        {
        case EKeyLeftArrow:
            {
            if( AknLayoutUtils::LayoutMirrored() )
                {
                // Go to next topic.
                iTopicView.ProcessCommandL( ECbsUiappTopicViewCmdGoToNextTopic );
                }
            else
                {            
                // Go to previous topic.
                iTopicView.ProcessCommandL( ECbsUiappTopicViewCmdGoToPrevTopic );
                }
            result = EKeyWasConsumed;
            break;
            }            
        case EKeyRightArrow:
            {
            if( AknLayoutUtils::LayoutMirrored() )
                {
                // Go to previous topic.
                iTopicView.ProcessCommandL( ECbsUiappTopicViewCmdGoToPrevTopic );
                }
            else
                {            
                // Go to next topic.
                iTopicView.ProcessCommandL( ECbsUiappTopicViewCmdGoToNextTopic );
                }
            result = EKeyWasConsumed;
            break;
            } 
        case EKeyEnter:
        case EKeyOK:
            {
            // to open a right message
            iTopicView.MarkCurrentlyFocusedHandle();
            iTopicView.ProcessCommandL( ECbsUiappCmdTopicMenuRead );
            result = EKeyWasConsumed;
            break;
            }            
        case EKeyBackspace:
            {
            iTopicView.MarkCurrentlyFocusedHandle();
            iTopicView.ProcessCommandL( ECbsUiappCmdMessageDelete );
            result = EKeyWasConsumed;
            break;
            }
        default:
            {
            result = iListBox->OfferKeyEventL( aKeyEvent, aType );
            break;            
            }
        }
    
    return result;
    }

// ---------------------------------------------------------
// CCbsUiTopicViewContainer::SizeChanged
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicViewContainer::SizeChanged()
    {
    if (iListBox) 
        {
        iListBox->SetRect( Rect() );
        }
    }


// ---------------------------------------------------------
// CCbsUiTopicViewContainer::FocusChanged
// ---------------------------------------------------------
//
void CCbsUiTopicViewContainer::FocusChanged( TDrawNow aDrawNow )
    {
    CCoeControl::FocusChanged( aDrawNow );
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused(), aDrawNow );
        }
    }


// ---------------------------------------------------------
// CCbsUiTopicViewContainer::GetHelpContext
// 
// Gets the control's help context.
// ---------------------------------------------------------
//
void CCbsUiTopicViewContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = TUid::Uid(KUidCbsUiappDef);
    aContext.iContext = KCBS_HLP_TOPIC_VIEW;
    }


// ----------------------------------------------------
// CCbsUiTopicViewContainer::HandleResourceChange
//
//
// ----------------------------------------------------
void CCbsUiTopicViewContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );

    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        iTopicView.HandleRectChangeInContainer();  
        }
    
    if( aType == KAknsMessageSkinChange )
        {
        // change skin bitmap
        TRAPD( ignore, SkinBitmapChangesL( ) );
        if( ignore )
            {
            ignore = KErrNone; //This prevent only ARMV5 varnings!!
            }
        }    
    }



// ---------------------------------------------------------
// CCbsUiTopicViewContainer::LoadIconsL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicViewContainer::LoadIconsL()
    {
    CAknIconArray* icons = new (ELeave) CAknIconArray(KNumberOfIconsInTopicViewcontainer);
    CleanupStack::PushL(icons);

    TParse parse; 
    parse.Set(KDirAndCbsUiAppMbmFile, &KDC_APP_BITMAP_DIR, NULL); 
    TFileName mbmFileAndPathName(parse.FullName());

    // First icon & mask.
    AddIconL( KAknsIIDQgnPropCbMsgUnread, icons, mbmFileAndPathName, EMbmCbsuiappQgn_prop_cb_msg_unread,
              EMbmCbsuiappQgn_prop_cb_msg_unread_mask );

    // Second icon & mask.
    AddIconL( KAknsIIDQgnPropCbMsg, icons, mbmFileAndPathName, EMbmCbsuiappQgn_prop_cb_msg,
              EMbmCbsuiappQgn_prop_cb_msg_mask );

    // Third icon & mask.
    AddIconL( KAknsIIDQgnIndiCbKeptAdd, icons, mbmFileAndPathName, EMbmCbsuiappQgn_indi_cb_kept_add,
              EMbmCbsuiappQgn_indi_cb_kept_add_mask, ETrue );

    CleanupStack::Pop(); // icons
    
    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
    }


// ---------------------------------------------------------
// CCbsUiTopicViewContainer::AddIconL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicViewContainer::AddIconL(
    TAknsItemID aId,
    CAknIconArray* aIcons,
    const TDesC& aIconFileWithPath,
    TInt aIconGraphicsIndex,
    TInt aIconGraphicsMaskIndex,
    TBool aColorIcon )
    {    
    // icon & mask.   
    MAknsSkinInstance* skins = AknsUtils::SkinInstance();  
    
    // COnstruct the icon and append it to icon array
    CGulIcon* icon = ConstructIconL( aColorIcon, skins, aId, 
        aIconFileWithPath, aIconGraphicsIndex, aIconGraphicsMaskIndex );
    CleanupStack::PushL( icon );
    aIcons->AppendL( icon );    
    CleanupStack::Pop( ); // icon
    }



// ----------------------------------------------------
// CCbsUiTopicViewContainer::SkinBitmapChangesL
// ----------------------------------------------------
void CCbsUiTopicViewContainer::SkinBitmapChangesL()
    {
    CArrayPtr<CGulIcon> *icons = 0;
    icons = iListBox->ItemDrawer()->ColumnData()->IconArray();
    TAknsItemID id; 
    id.iMajor = 0;
    id.iMinor = 0;    
    TInt count = icons->Count();
    TInt bitmapIndex = 0;
    TInt bitmapMaskIndex = 0;

    //change list icons
    MAknsSkinInstance* skins = AknsUtils::SkinInstance();
    for ( TInt cc=count; --cc>=0; )
        {
        switch( cc )
            {
            case ECbsPropCbMsgUnread:                
                bitmapIndex = EMbmCbsuiappQgn_prop_cb_msg_unread;
                bitmapMaskIndex = EMbmCbsuiappQgn_prop_cb_msg_unread_mask;
                id = KAknsIIDQgnPropCbMsgUnread;
                break;
            case ECbsPropCbMsg:                
                bitmapIndex = EMbmCbsuiappQgn_prop_cb_msg;
                bitmapMaskIndex = EMbmCbsuiappQgn_prop_cb_msg_mask;
                id = KAknsIIDQgnPropCbMsg;
                break;
            case ECbsIndiCbKeptAdd:                
                bitmapIndex = EMbmCbsuiappQgn_indi_cb_kept_add;
                bitmapMaskIndex = EMbmCbsuiappQgn_indi_cb_kept_add_mask;
                id = KAknsIIDQgnIndiCbKeptAdd;
                break;
            default:                
                bitmapIndex = EMbmCbsuiappQgn_prop_cb_msg_unread;
                bitmapMaskIndex = EMbmCbsuiappQgn_prop_cb_msg_unread_mask;
                id = KAknsIIDQgnPropCbMsgUnread;
                break;           
            } 
        
        CFbsBitmap* bitmap;
        CFbsBitmap* mask;
        TParse parse; 
        parse.Set(KDirAndCbsUiAppMbmFile, &KDC_APP_BITMAP_DIR, NULL); 
        TFileName mbmFileAndPathName(parse.FullName());
        AknsUtils::CreateIconL( skins, id, bitmap,
            mask, mbmFileAndPathName, bitmapIndex, bitmapMaskIndex );      

        CGulIcon* icon = CGulIcon::NewL( bitmap, mask );
     
        CleanupStack::PushL( icon );
        CGulIcon* iconToBeDelete = icons->At( cc );
        icons->At( cc ) = icon;
        delete iconToBeDelete;
        iconToBeDelete = NULL;

        CleanupStack::Pop(); // icon
    
        }  
    }


// ----------------------------------------------------
// CCbsUiTopicViewContainer::ConstructIconL
// ----------------------------------------------------
CGulIcon* CCbsUiTopicViewContainer::ConstructIconL( 
    TBool& aColourIcon,
    MAknsSkinInstance* aSkins, 
    const TAknsItemID& aID,
    const TDesC& aFilename,
    const TInt aFileBitmapId, 
    const TInt aFileMaskId )
    {
    // icon & mask
    CFbsBitmap* bitmap;
    CFbsBitmap* mask;
        
    if ( aColourIcon )
        {
        AknsUtils::CreateColorIconL(
            aSkins, 
            aID, 
            KAknsIIDQsnIconColors, 
            EAknsCIQsnIconColorsCG13, // Check the used color group - this is usually correct group for D-column stuff
            bitmap, 
            mask,
            aFilename,
            aFileBitmapId,
            aFileMaskId ,
            KRgbBlack ); // This is default color used, if the color skinning fails, or there is no color defined.
        }
    else
        {
        AknsUtils::CreateIconL( aSkins, aID, bitmap,
        mask, aFilename, aFileBitmapId, 
        aFileMaskId );
        }

    CGulIcon* icon = CGulIcon::NewL( bitmap, mask );
    return icon;
    }


//  End of File 
