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
* Description: 
*          TopicListViewContainer owns the list box which shows topics.
*
*/


// INCLUDES
#include <akncontext.h>      // CAknContextPane
#include <cbsuiapp.mbg>      // TMbmCbsuiapp
#include <AknsConstants.h>   // KAknsMessageSkinChange
#include <AknsUtils.h>       // CreateIconL, SkinInstance
#include <AknUtils.h>        // AknTextUtils
#include <AknDef.h>          // KEikDynamicLayoutVariantSwitch
#include <barsread.h>
#include <aknlists.h>
#include <gulicon.h>
#include <eikclbd.h>
#include <AknIconArray.h>
#include <data_caging_path_literals.hrh> // KDC_RESOURCE_FILES_DIR
#include <cbsuiApp.rsg>
#include "CbsUiId.h"
#include <csxhelp/cbs.hlp.hrh>
#include "CCbsUiTopicListViewContainer.h"
#include "CCbsUiTopicListView.h"
#include "Cbsuiapp.hrh"
#include "CbsUiUtility.h"
#include "CbsUiPanic.h"
#include "CbsUiConstants.h"


// CONSTANTS
_LIT(KDirAndCbsUiAppMbmFile,"z:cbsuiapp.mbm");
_LIT(KItemSeparator1, "\t ");
_LIT(KItemSeparator2, "\t4");
const TInt KSizeOfStatus = 1;
const TInt KCountOfSeparator1 = 2;
const TInt KNumberOfIconsInTopicListContainer = 5;
const TUint KZeroFill = '0';
const TUint KSubscribedAndNoMessages = '0';
const TUint KSubscribedAndMessages = '1';
const TUint KNotSubscribedAndNoMessages = '2';
const TUint KNotSubscribedButMessagesExists = '3';


//DATA TYPES
enum TopicListViewIconArrayIndexes
    {
    ECbsPropCbSubs = 0,
    ECbsPropCbSubsUnread,
    ECbsPropCbUnsubs,
    ECbsPropCbUnsubsUnread,
    ECbsIndiCbHotAdd
    };


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
// 

CCbsUiTopicListViewContainer::CCbsUiTopicListViewContainer( CCbsUiTopicListView& aListView )
    : iListView( aListView )
    {
    }

// Symbian OS default constructor can leave.
void CCbsUiTopicListViewContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    
    SetMopParent(&iListView);
    
    iListBox = new(ELeave) CAknSingleGraphicHeadingStyleListBox;
    iListBox->SetContainerWindowL(*this);
    
    iListBox->ConstructL( this, EEikListBoxMultipleSelection );
    
    CDesCArrayFlat* array = 
        new (ELeave) CDesCArrayFlat(KNumberOfStandardTopics);
    
    // changing the owner, shouldn't leave 
    iListBox->Model()->SetItemTextArray( array ); 
    
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( 
        CEikScrollBarFrame::EOff,
        CEikScrollBarFrame::EAuto );
    iListBox->SetListBoxObserver( this );
    
    SetRect( aRect );
    LoadIconsL();
    }

// Two-phased constructor.
CCbsUiTopicListViewContainer* CCbsUiTopicListViewContainer::NewL( CCbsUiTopicListView& aListView, 
                                                                 const TRect& aRect )
    {
    CCbsUiTopicListViewContainer* self = 
        new (ELeave) CCbsUiTopicListViewContainer( aListView );
    
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop();
    
    return self;
    }

// Destructor
CCbsUiTopicListViewContainer::~CCbsUiTopicListViewContainer()
    {
    delete iListBox;
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::ReloadStarts
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::ReloadStarts() const
    {    
    MDesCArray* itemList = iListBox->Model()->ItemTextArray();
    CDesCArray* itemArray = STATIC_CAST(CDesCArray*, itemList);
    
    itemArray->Reset();
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::CountComponentControls
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListViewContainer::CountComponentControls() const
    {
    return 1;
    }

// ---------------------------------------------------------
// 
// 
// 
// ---------------------------------------------------------
//
CCoeControl* CCbsUiTopicListViewContainer::ComponentControl( TInt /*aIndex*/ ) const 
    {
    return iListBox;
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::OfferKeyEventL
// 
// 
// ---------------------------------------------------------
//
TKeyResponse CCbsUiTopicListViewContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                                          TEventCode aType )
    {
    TKeyResponse result = EKeyWasNotConsumed;
    
    switch ( aKeyEvent.iCode )
        {
        case EKeyEnter: // To support qwert keyboard
        case EKeyOK:
            // to open a right topic
            iListView.MarkCurrentHandle();
            iListView.ProcessCommandL(ECbsUiappCmdSwitchOpen);
            result = EKeyWasConsumed;
            break;
        case EKeyBackspace:
            iListView.MarkCurrentHandle();
            iListView.ProcessCommandL(ECbsUiappCmdSwitchDelete);
            result = EKeyWasConsumed;
            break;
        default:
            result = iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;
        }
    
    return result;
    }


// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::SetCurrentItemIndex
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::SetCurrentItemIndex( TInt aItemIndex ) const
    {    
    
    MDesCArray* itemList = iListBox->Model()->ItemTextArray();
    CDesCArray* itemArray = STATIC_CAST(CDesCArray*, itemList);
    
    TInt count( itemArray->MdcaCount() );
    
    __ASSERT_DEBUG( 
        aItemIndex >= EMinTopicNumber && 
        aItemIndex <= EMaxTopicNumber, 
        CbsUiPanic( EErrBadIndex ) );
    
    if ( aItemIndex < count && aItemIndex >= EMinTopicNumber )
        {
        iListBox->SetCurrentItemIndexAndDraw( aItemIndex );
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::ReloadOfferTopicL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::ReloadOfferTopicL( const TCbsTopic& aTopic ) const
    {
    
    MDesCArray* itemList = iListBox->Model()->ItemTextArray();
    CDesCArray* itemArray = STATIC_CAST(CDesCArray*, itemList);	    
    
    TBuf<KSizeOfStatus> ch;
    ch.Append( GetStatus(aTopic) );
    TBuf<KTopicNumberLength> index;
    GetIndexNumber(aTopic, index);
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( index );
    
    HBufC* buffer = HBufC::NewLC( 
        aTopic.iName.Length() + KItemSeparator1().Length() * 
        KCountOfSeparator1 + KItemSeparator2().Length() + 
        index.Length() + ch.Length() );
    
    TPtr ptr = buffer->Des();
    ptr = ch;
    ptr.Append(KItemSeparator1);	
    ptr.Append(index);
    ptr.Append(KItemSeparator1);
    ptr.Append(aTopic.iName);
    
    if ( aTopic.iHotmarked ) 
        {
        ptr.Append(KItemSeparator2);
        }
    
    itemArray->AppendL(*buffer);		
    CleanupStack::PopAndDestroy(); // buffer
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::ReloadEndsL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::ReloadEndsL()
    {
    iListBox->HandleItemAdditionL();
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::CurrentPosition
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListViewContainer::CurrentPosition() const
    {
    TInt result(KCbsErrValue);
    
    if ( iListBox )
        {
        result = iListBox->CurrentItemIndex();
        }
    
    return result;
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::GetStatus
// 
// Informs which icon to use.
// ---------------------------------------------------------
//
TChar CCbsUiTopicListViewContainer::GetStatus( const TCbsTopic& aTopic ) const
    {
    
    TChar retVal(KSubscribedAndNoMessages);
    
    
    if ( aTopic.iSubscribed )
        {
        if ( aTopic.iUnreadMessages == 0 )
            {
            retVal = KSubscribedAndNoMessages;
            }
        else 
            {
            retVal = KSubscribedAndMessages;
            }
        }
    else
        {
        if ( aTopic.iUnreadMessages == 0 )
            {
            retVal = KNotSubscribedAndNoMessages;
            }
        else 
            {
            retVal = KNotSubscribedButMessagesExists;    
            }
        }
    
    return retVal;
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::GetIndexNumber
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::GetIndexNumber( const TCbsTopic& aTopic , 
                                                   TBuf<KTopicNumberLength>& aIndexNumber ) const
    {
    TBuf<KTopicNumberLength> ind;
    
    __ASSERT_DEBUG( aIndexNumber.MaxSize() >= KTopicNumberLength, 
        CbsUiPanic(EErrBadLength) );
    
    ind.AppendNum( TInt(aTopic.iNumber) );
    aIndexNumber.Justify(ind , KTopicNumberLength, ERight, KZeroFill);
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::SizeChanged
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::SizeChanged()
    {
    if (iListBox) 
        {
        iListBox->SetRect( Rect() );
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::FocusChanged
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::FocusChanged( TDrawNow aDrawNow )
    {
    CCoeControl::FocusChanged( aDrawNow );
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused(), aDrawNow );
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::GetHelpContext
// 
// Gets the control's help context.
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::GetHelpContext(TCoeHelpContext& aContext) const
    {
    aContext.iMajor = TUid::Uid(KUidCbsUiappDef);
    aContext.iContext = KCBS_HLP_MAIN;
    }



// ----------------------------------------------------
// CCbsUiTopicListViewContainer::HandleResourceChange
//
// ----------------------------------------------------
void CCbsUiTopicListViewContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );

    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        iListView.HandleRectChangeInContainer(); 
        } 

    if( aType == KAknsMessageSkinChange )
        {
        // change skin bitmap
        TRAPD( ignore, SkinBitmapChangesL( ) );
        if( ignore )
            {
            ignore = KErrNone; //Prevent ARMV5 compilation varning
            }
        }    
    }

// ----------------------------------------------------
// CCbsUiTopicListViewContainer::HandleListBoxEventL
//
// ----------------------------------------------------
void CCbsUiTopicListViewContainer::HandleListBoxEventL(	CEikListBox* /*aListBox*/,
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
// CCbsUiTopicListViewContainer::LoadIconsL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::LoadIconsL()
    {
    CAknIconArray* icons = 
        new (ELeave) CAknIconArray(KNumberOfIconsInTopicListContainer);
    CleanupStack::PushL(icons);

    TParse parse; 
    parse.Set(KDirAndCbsUiAppMbmFile, &KDC_APP_BITMAP_DIR, NULL); 
    TFileName mbmFileAndPathName(parse.FullName());
    
    // First icon & mask.
    AddIconL( KAknsIIDQgnPropCbSubs, icons, mbmFileAndPathName, EMbmCbsuiappQgn_prop_cb_subs,
              EMbmCbsuiappQgn_prop_cb_subs_mask );

    // Second icon & mask.
    AddIconL( KAknsIIDQgnPropCbSubsUnread, icons, mbmFileAndPathName, EMbmCbsuiappQgn_prop_cb_subs_unread,
              EMbmCbsuiappQgn_prop_cb_subs_unread_mask );

    // Third icon & mask.
    AddIconL( KAknsIIDQgnPropCbUnsubs, icons, mbmFileAndPathName, EMbmCbsuiappQgn_prop_cb_unsubs,
              EMbmCbsuiappQgn_prop_cb_unsubs_mask );

    // Fourth icon & mask.
    AddIconL( KAknsIIDQgnPropCbUnsubsUnread, icons, mbmFileAndPathName, EMbmCbsuiappQgn_prop_cb_unsubs_unread,
              EMbmCbsuiappQgn_prop_cb_unsubs_unread_mask );

    // Fifth icon & mask.
    AddIconL( KAknsIIDQgnIndiCbHotAdd, icons, mbmFileAndPathName, EMbmCbsuiappQgn_indi_cb_hot_add,
              EMbmCbsuiappQgn_indi_cb_hot_add_mask, ETrue );

    CleanupStack::Pop(); // icons
    
    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
    }


// ---------------------------------------------------------
// CCbsUiTopicListViewContainer::AddIconL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListViewContainer::AddIconL(
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
// CCbsUiTopicListViewContainer::SkinBitmapChangesL
// ----------------------------------------------------
void CCbsUiTopicListViewContainer::SkinBitmapChangesL()
    {    
    CArrayPtr<CGulIcon> *icons = 0;
    icons = iListBox->ItemDrawer()->ColumnData()->IconArray();
    
    TAknsItemID id; 
    id.iMajor = 0;
    id.iMinor = 0;    
    
    TInt count = icons->Count();
    TInt bitmapIndex = 0;
    TInt bitmapMaskIndex = 0;
    TBool colourIcon( EFalse );
    
    // Change list icons
    MAknsSkinInstance* skins = AknsUtils::SkinInstance();
    for ( TInt cc=count; --cc>=0; )    
        {
        switch( cc )
            {
            case ECbsPropCbSubs:                
                bitmapIndex = EMbmCbsuiappQgn_prop_cb_subs;
                bitmapMaskIndex = EMbmCbsuiappQgn_prop_cb_subs_mask;
                id = KAknsIIDQgnPropCbSubs;
                break;
            case ECbsPropCbSubsUnread:                
                bitmapIndex = EMbmCbsuiappQgn_prop_cb_subs_unread;
                bitmapMaskIndex = EMbmCbsuiappQgn_prop_cb_subs_unread_mask;
                id = KAknsIIDQgnPropCbSubsUnread;
                break;
            case ECbsPropCbUnsubs:                
                bitmapIndex = EMbmCbsuiappQgn_prop_cb_unsubs;
                bitmapMaskIndex = EMbmCbsuiappQgn_prop_cb_unsubs_mask;
                id = KAknsIIDQgnPropCbUnsubs;
                break;
            case ECbsPropCbUnsubsUnread:                
                bitmapIndex = EMbmCbsuiappQgn_prop_cb_unsubs_unread;
                bitmapMaskIndex = EMbmCbsuiappQgn_prop_cb_unsubs_unread_mask;
                id = KAknsIIDQgnPropCbUnsubsUnread;
                break;
            case ECbsIndiCbHotAdd:                
                bitmapIndex = EMbmCbsuiappQgn_indi_cb_hot_add;
                bitmapMaskIndex = EMbmCbsuiappQgn_indi_cb_hot_add_mask;
                id = KAknsIIDQgnIndiCbHotAdd;
                colourIcon = ETrue;
                break;
            default:
                bitmapIndex = EMbmCbsuiappQgn_prop_cb_unsubs_unread;
                bitmapMaskIndex = EMbmCbsuiappQgn_prop_cb_unsubs_unread_mask;
                id = KAknsIIDQgnPropCbUnsubsUnread;
                break;           
            } 
        
        // icon & mask
        TParse parse; 
        parse.Set( KDirAndCbsUiAppMbmFile, &KDC_APP_BITMAP_DIR, NULL ); 
        TFileName mbmFileAndPathName( parse.FullName() );
        
        CGulIcon* icon = ConstructIconL( colourIcon, skins, id, 
            mbmFileAndPathName, bitmapIndex, bitmapMaskIndex );
            
        CleanupStack::PushL( icon );
        CGulIcon* iconToBeDelete = icons->At( cc );
        icons->At( cc ) = icon;
        delete iconToBeDelete;
        iconToBeDelete = NULL;

        CleanupStack::Pop(); // icon
        
        colourIcon = EFalse;
        }  
    }


// ----------------------------------------------------
// CCbsUiTopicListViewContainer::ConstructIconL
// ----------------------------------------------------
CGulIcon* CCbsUiTopicListViewContainer::ConstructIconL( 
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
