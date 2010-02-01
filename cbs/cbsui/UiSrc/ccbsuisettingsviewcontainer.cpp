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
* Description:  
*       Implementation of CCbsUiSettingsViewContainer.
*
*
*/


// INCLUDES
#include <bldvariant.hrh>         // For feature flags
#include <featmgr.h>              // FeatureManager
#include <aknnotewrappers.h>      // CAknErrorNote
#include <e32def.h>
#include <aknlists.h> 
#include <akncheckboxsettingpage.h>
#include <cbsuiApp.rsg>
#include <barsread.h>
#include <centralrepository.h>           // CRepository 
#include <CoreApplicationUIsSDKCRKeys.h> // KCRUidCoreApplicationUIs, TCoreAppUIsNetworkConnectionAllowed
#include "CbsUiId.h"
#include <csxhelp/mce.hlp.hrh>
#include <AknDef.h>               // KEikDynamicLayoutVariantSwitch
#include "MCbs.h"
#include "ccbsuisettingsviewcontainer.h"
#include "CCbsUiSettingsView.h"
#include "CbsUiConstants.h"
#include "CCbsUiLanguageSettingPage.h"
#include "CbsUiPanic.h"



// CONSTANTS

// exact number of items in selection list
const TInt KNumberOfItemsInSelectionPage = 3;

// There should be only one item in the list box before, which
// is reception.
const TInt KPreciseNumberOfItemsBeforeLanguage = 1;
const TInt KNumberOfOnOfItems = 2;




// MODULE DATA STRUCTURES
enum TCbsUiSettingValues { EReception, ELanguage, ETopicDetection };

// to select right item in setting_items_with_two_available_values
enum TCbsUiSettingOnOff 
    {
    ESettingOn = 0, 
    ESettingOff
    };

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiSettingsViewContainer::CCbsUiSettingsViewContainer( 
    CCbsUiSettingsView& aView, 
    MCbs& aServer )
    : iView( aView ), iServer( aServer )
    {
    }

// Symbian OS default constructor can leave.
void CCbsUiSettingsViewContainer::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    SetMopParent(&iView);
    CreateListBoxL();
    SetRect( aRect );
    
    // Prepare FeatureManager, read values to members and uninitialize FeatureManager
    FeatureManager::InitializeLibL();
    iOfflineFeatureSupported = FeatureManager::FeatureSupported( KFeatureIdOfflineMode );
    }

// Two-phased constructor.
CCbsUiSettingsViewContainer* CCbsUiSettingsViewContainer::NewL( 
    CCbsUiSettingsView& aView,
    const TRect& aRect,
    MCbs& aServer )
    {
    CCbsUiSettingsViewContainer* self = 
        new (ELeave) CCbsUiSettingsViewContainer(aView, aServer);
    
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    CleanupStack::Pop();
    
    return self;
    }

// Destructor
CCbsUiSettingsViewContainer::~CCbsUiSettingsViewContainer()
    {
    delete iListBox;
    FeatureManager::UnInitializeLib();
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::CountComponentControls
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiSettingsViewContainer::CountComponentControls() const
    {
    return 1;
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::ComponentControl
// 
// 
// ---------------------------------------------------------
//
CCoeControl* CCbsUiSettingsViewContainer::ComponentControl( TInt aIndex ) const 
    {
    CCoeControl* control = NULL;
    
    switch ( aIndex ) 
        {
        case 0:
            control = iListBox;
            break;
        default:                
            break;
        }
    
    return control;
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::OfferKeyEventL
// 
// 
// ---------------------------------------------------------
//
TKeyResponse CCbsUiSettingsViewContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    return iListBox->OfferKeyEventL( aKeyEvent, aType );
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::SizeChanged
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::SizeChanged()
    {
    iListBox->SetRect( Rect() );
    }


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::FocusChanged
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::FocusChanged( TDrawNow aDrawNow )
    {
    CCoeControl::FocusChanged( aDrawNow );
    if( iListBox )
        {
        iListBox->SetFocus( IsFocused(), aDrawNow );
        }
    }


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::CreateListBoxL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::CreateListBoxL()
    {
    iListBox = new (ELeave) CAknSettingStyleListBox;
    iListBox->SetContainerWindowL( *this );
    iListBox->ConstructL( this, EAknListBoxSelectionListWithShortcuts );
    iListBox->SetListBoxObserver( this );
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, 
                                                         CEikScrollBarFrame::EAuto );
    GetSettingsL();
    }


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::GetSettingsL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::GetSettingsL()
    {
    __ASSERT_DEBUG( iListBox, CbsUiPanic(EListBoxNullError) );
    
    MDesCArray* itemList = 
        STATIC_CAST(CEikFormattedCellListBox*, 
        iListBox)->Model()->ItemTextArray();
    
    CDesCArray* itemArray = STATIC_CAST(CDesCArray*, itemList);
    
    itemArray->Reset();
    
    TBool status;
    iServer.GetReceptionStatus(status);
    
    TInt resourceId(R_TEXT_RECEPTION_ON);
    
    if ( !status )
        {
        resourceId = R_TEXT_RECEPTION_OFF;
        }
    
    HBufC* reception = iCoeEnv->AllocReadResourceLC(resourceId);
    itemArray->AppendL(*reception);
    CleanupStack::PopAndDestroy(); // reception
    
    // This must be here, don't move; otherwise order
    // is different in the list box.
    SetLanguageSelectionL(*itemArray);
    
    iServer.GetTopicDetectionStatus(status);
    resourceId = R_TEXT_DETECTION_ON;
    
    if ( !status ) 
        {
        resourceId = R_TEXT_DETECTION_OFF;
        }
    
    HBufC* detection = iCoeEnv->AllocReadResourceLC(resourceId);
    
    itemArray->AppendL(*detection);
    CleanupStack::PopAndDestroy(); // detection
    
    iListBox->HandleItemAdditionL();
    
    }


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::PrepareLanguageSettingPageL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::PrepareLanguageSettingPageL()
    {
    
    CDesCArrayFlat* items = 
        new (ELeave) CDesCArrayFlat(KNumberOfItemsInSelectionPage);
    CleanupStack::PushL(items);
    
    AddItemsToListL( 
        R_CBS_LANGUAGE_SETTING_PAGE_ARRAY, *items);
    
    TInt selected( GetSelected() );
    
    LaunchLanguageSettingPageL(*items, selected);
    
    CleanupStack::PopAndDestroy(); // items
    
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::GetSelected
// 
// 
// ---------------------------------------------------------
//
TCbsUiLanguageSelectionIndex CCbsUiSettingsViewContainer::GetSelected()
    {
    TCbsDbLanguages dbLanguages;
    iServer.GetLanguages(dbLanguages);
    
    TCbsUiLanguageSelectionIndex result(ECbsUiSelected);
    
    if ( dbLanguages.iLanguages[ECbsAll] )
        {
        result = ECbsUiAll;
        }
    else if ( dbLanguages.iLanguages[ECbsOther] )
        {
        result = ECbsUiOther;
        }
    else
		{
		result = ECbsUiSelected;
		}
    
    return result;
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::LaunchLanguageSettingPageL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::LaunchLanguageSettingPageL(
    CDesCArrayFlat& aItems, TInt& aCurrentSelectionIndex )
    {
    __ASSERT_DEBUG(
        aCurrentSelectionIndex >= ECbsUiAll && 
        aCurrentSelectionIndex <= ECbsUiOther,
        CbsUiPanic(EErrIndexOutOfBounds) );
    __ASSERT_DEBUG( aItems.Count() == 3, CbsUiPanic(EErrInvalidValue) );
    
    CCbsUiLanguageSettingPage* languagePage = 
        new (ELeave) CCbsUiLanguageSettingPage(
        R_LANGUAGE_SETTING_PAGE, aCurrentSelectionIndex, 
        &aItems, iServer);
    
    languagePage->ExecuteLD(CAknSettingPage::EUpdateWhenChanged);
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::AddItemsToListL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::AddItemsToListL( 
    TInt aResourceId, 
    CDesCArrayFlat& aItemList ) const
    {
    __ASSERT_DEBUG(aResourceId >= 0, CbsUiPanic(EErrInvalidValue) );
    __ASSERT_DEBUG( aItemList.MdcaCount() == 0, CbsUiPanic(EErrInvalidValue) );
    
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC(reader, aResourceId);
    TInt count( reader.ReadInt16() );
    
    for ( TInt i(0); i < count; i++ )
        {
        HBufC* selectableItem = reader.ReadHBufCL();
        
        CleanupStack::PushL(selectableItem);        
        aItemList.AppendL(*selectableItem);
        CleanupStack::PopAndDestroy(); // selectableItem
        }
    
    CleanupStack::PopAndDestroy(); // reader   
    }


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::HandleListViewActionL
// 
// 
// ---------------------------------------------------------
//
TKeyResponse CCbsUiSettingsViewContainer::HandleListViewActionL(
    const CEikListBox* aListBox, TBool aLaunchedFromOptionMenu )
    {	
    __ASSERT_DEBUG( aListBox, CbsUiPanic(EErrNullPointer) );    
    
    switch ( aListBox->CurrentItemIndex() )
        {
        case EReception:  
            {
            if ( IsPhoneOfflineL() )
                {
                // If is in offline mode, don't set reception on or off(Only error note)
                HBufC* text = iCoeEnv->AllocReadResourceLC( R_QTN_OFFLINE_NOT_POSSIBLE );
                CAknErrorNote* note = new (ELeave) CAknErrorNote( ETrue );
                note->ExecuteLD( *text );
                CleanupStack::PopAndDestroy( ); //text
                }
            else
                {
                TBool reception;
                TBool update(ETrue); // if value need to be updated
                iServer.GetReceptionStatus(reception);
                if ( aLaunchedFromOptionMenu )
                    {
                    LaunchDialogOnOffL(reception, update, R_TEXT_RECEPTION);
                    }
                if ( update )
                    {
                    if ( !reception )
                        {
                        // Reception is off. -> Set reception On
                        User::LeaveIfError( iServer.SetReceptionStatus( ETrue ) );
                        }
                    else
                        {
                        // Reception is On. -> Set reception Off
                        User::LeaveIfError( iServer.SetReceptionStatus( EFalse ) );
                        }
                    }
                }
            break;
            }
        case ELanguage:
            {
            PrepareLanguageSettingPageL();
            break;
            }
        case ETopicDetection: 
            {  
            TBool detection;
            TBool update(ETrue);

            iServer.GetTopicDetectionStatus(detection);
            
            if ( aLaunchedFromOptionMenu )
                {
                LaunchTopicDetectionDialogOnOffL(detection, update, R_TEXT_TOPIC_DETECTION);
                }

            if ( update )
                {           
                User::LeaveIfError( iServer.SetTopicDetectionStatus(!detection) );
                }
            break;
            }
        default:
            break;
        }   
    
    GetSettingsL();
    
    return EKeyWasConsumed;
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::LaunchDialogOnOffL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::LaunchDialogOnOffL(
    const TBool& aValue, TBool& aUpdate, TInt aResourceId ) const
    {
    TInt selected( aValue ? ESettingOn : ESettingOff );
    
    CDesCArrayFlat* items = 
        new (ELeave) CDesCArrayFlat(KNumberOfOnOfItems);
    CleanupStack::PushL(items);
    
    AddItemsToListL(R_CBS_ON_OFF_ARRAY, *items);

    HBufC* settingText = iCoeEnv->AllocReadResourceL(aResourceId);
    CleanupStack::PushL(settingText);
    
    CAknRadioButtonSettingPage* dlg = 
        new (ELeave) CAknRadioButtonSettingPage(
        R_ON_OFF_SETTING_PAGE, selected, items);

    CleanupStack::PushL(dlg);
    dlg->SetSettingTextL(*settingText);
    CleanupStack::Pop(); // dlg
    
    dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged);

    CleanupStack::PopAndDestroy(); // settingText
    
    TBool valueNotChanged(
        selected == (aValue ? ESettingOn : ESettingOff) );
    if ( valueNotChanged )
        {
        aUpdate = EFalse;
        }
    
    CleanupStack::PopAndDestroy(); // items
    } 


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::LaunchTopicDetectionDialogOnOffL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::LaunchTopicDetectionDialogOnOffL(
    const TBool& aValue, TBool& aUpdate, TInt aResourceId ) const
    {
    TInt selected( aValue ? ESettingOn : ESettingOff );
    
    CDesCArrayFlat* items = 
        new (ELeave) CDesCArrayFlat(KNumberOfOnOfItems);
    CleanupStack::PushL(items);
    
    AddItemsToListL(R_CBS_TOPIC_DETECTION_ON_OFF_ARRAY, *items);

    HBufC* settingText = iCoeEnv->AllocReadResourceL(aResourceId);
    CleanupStack::PushL(settingText);
    
    CAknRadioButtonSettingPage* dlg = 
        new (ELeave) CAknRadioButtonSettingPage(
        R_ON_OFF_SETTING_PAGE, selected, items);

    CleanupStack::PushL(dlg);
    dlg->SetSettingTextL(*settingText);
    CleanupStack::Pop(); // dlg
    
    dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged);

    CleanupStack::PopAndDestroy(); // settingText
    
    TBool valueNotChanged(
        selected == (aValue ? ESettingOn : ESettingOff) );
    if ( valueNotChanged )
        {
        aUpdate = EFalse;
        }
    
    CleanupStack::PopAndDestroy(); // items
    }     

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::HandleListViewActiontL
// 
// 
// ---------------------------------------------------------
//
TKeyResponse CCbsUiSettingsViewContainer::HandleListViewActionL(
    TBool aLaunchedFromOptionMenu )
    {
    if ( iListBox )
        {
        return HandleListViewActionL(iListBox, aLaunchedFromOptionMenu);
        }
    return EKeyWasNotConsumed;
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::UpdateSettingsL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::UpdateSettingsL()
    {
    GetSettingsL();
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::SetFocusToFirstItem
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::SetFocusToFirstItem() const
    {
    __ASSERT_DEBUG( iListBox, CbsUiPanic(EErrNullPointer) );
    iListBox->SetCurrentItemIndexAndDraw(0);
    }

// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::HandleListBoxEventL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::HandleListBoxEventL( 
    CEikListBox* aListBox,
    TListBoxEvent aEventType )
    {
    if ( EEventEnterKeyPressed == aEventType
        || EEventItemSingleClicked == aEventType )
        {
        HandleListViewActionL(aListBox, EFalse);
        }
    }


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::SetLanguageSelectionL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::SetLanguageSelectionL(
    CDesCArray& aItemArray )
    {
    TInt count( aItemArray.MdcaCount() );
    
    __ASSERT_ALWAYS( count == KPreciseNumberOfItemsBeforeLanguage, 
        CbsUiPanic(EErrIndexOutOfBounds) );
    
    if ( count == KPreciseNumberOfItemsBeforeLanguage )
        {        
        TInt textId(R_TEXT_LANGUAGE_ALL);
        
        switch ( GetSelected() )
            {   
            case ECbsUiAll:
                // do nothing
                break;
            case ECbsUiSelected:
                textId = R_TEXT_LANGUAGE_SELECTED;
                break;
            case ECbsUiOther:
                textId = R_TEXT_LANGUAGE_OTHER;
                break;
            default:
                break;
            }        
        
        HBufC* language = iCoeEnv->AllocReadResourceLC(textId);
        aItemArray.AppendL(*language);
        
        CleanupStack::PopAndDestroy(); // language
        }
    }


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::GetHelpContext
// 
// Gets the control's help context.
// ---------------------------------------------------------
//
void CCbsUiSettingsViewContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = TUid::Uid(KUidCbsUiappDef);
    aContext.iContext = KMCE_HLP_SETTINGS_CB;
    }



// ----------------------------------------------------
// CCbsUiSettingsViewContainer::HandleResourceChange
//
// ----------------------------------------------------
void CCbsUiSettingsViewContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    
    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        iView.HandleRectChangeInContainer();  
        }
    }


// ---------------------------------------------------------
// CCbsUiSettingsViewContainer::IsPhoneOfflineL
// 
// Checks if phone is in offline mode or not.
// Return ETrue if phone is in offline mode.
// Return EFalse if phone is not in offline mode.
// ---------------------------------------------------------
//
TBool CCbsUiSettingsViewContainer::IsPhoneOfflineL() const
    {
    if ( iOfflineFeatureSupported )
        {
        CRepository* repository = CRepository::NewLC( KCRUidCoreApplicationUIs );
        TInt connAllowed = 1;
        repository->Get( KCoreAppUIsNetworkConnectionAllowed, connAllowed );
        CleanupStack::PopAndDestroy();  // repository
        if ( !connAllowed )
            {
            return ETrue;
            }
        }
    return EFalse;
    }


//  End of File 
