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
* Description:  Implementation of CCbsUiSettingsView class.
*
*
*/


// INCLUDES
#include <AknsUtils.h>       // CreateIconL
#include <AknsConstants.h>   // KAknsIIDQgnMenuCbCxt, KAknsIIDQgnMenuCb2Cxt
#include <bldvariant.hrh>    // for feature flags
#include <featmgr.h>	     // for feature manager
#include <aknViewAppUi.h>
#include <avkon.hrh>
#include <aknnavide.h>
#include <hlplch.h>
#include <cbsuiApp.rsg>
#include <akntitle.h> 
#include <cbsuiapp.mbg>
#include <barsread.h>
#include "CCbsUiSettingsView.h"
#include "ccbsuisettingsviewcontainer.h"
#include "Cbsuiapp.hrh"
#include "CCbsUiappApplication.h"
#include "CbsUiId.h"
#include "CbsUiUtility.h"
#include "CbsUiPanic.h"
#include "CCbsUiSettingsMonitor.h"
#include "CbsUiConstants.h"
#include "MCbs.h"



// CONSTANTS
static const TUid KUidCbsUiappApp = { KUidCbsUiappDef };



// ================= MEMBER FUNCTIONS =======================

// C++ constructor
CCbsUiSettingsView::CCbsUiSettingsView( MCbs& aServer )
:  iServer(aServer)
    {
    }

// Symbian OS default constructor can leave.
void CCbsUiSettingsView::ConstructL() 
    {    
    BaseConstructL(R_CBSUI_SETTINGSVIEW);
    iImageLibrary = iCoeEnv->AllocReadResourceL(R_CBS_IMAGE_LIBRARY);
    iSettingsMonitor = CCbsUiSettingsMonitor::NewL(iServer, *this);
    }

// Two-phased constructor.
CCbsUiSettingsView* CCbsUiSettingsView::NewL( MCbs& aServer )
    {
    CCbsUiSettingsView* self = new (ELeave) CCbsUiSettingsView( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsUiSettingsView::~CCbsUiSettingsView()
    {
    if (iContainer)
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        
        // Destroy the control container
        delete iContainer;
        }
    
    delete iDecoratedLabel;
    delete iImageLibrary;
    delete iSettingsMonitor;
    }

// ---------------------------------------------------------
// CCbsUiSettingsView::Id
// 
// 
// ---------------------------------------------------------
//
TUid CCbsUiSettingsView::Id() const
    {
    return TUid::Uid( ECbsUiSettingsViewId );
    }

// ---------------------------------------------------------
// CCbsUiSettingsView::HandleCommandL
//
//
// ---------------------------------------------------------
//
void CCbsUiSettingsView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {
            // if it has been CBS then do this
            if ( !iFromOtherApplication )
                {
                ActivateTopicListViewL();
                }
            else
                {
                AppUi()->ProcessCommandL( EAknCmdExit );
                }
            break;
            }
        case EAknSoftkeyChange:
            {
            iContainer->HandleListViewActionL( EFalse );
            break;
            }
        case ECbsUiappSettingsViewCmdChange:
            {
            iContainer->HandleListViewActionL( ETrue );
            break;
            }
        case EAknCmdHelp:
            AppUi()->HandleCommandL( aCommand );
            break;
        case EAknCmdExit:	
            {
            AppUi()->ProcessCommandL( EAknCmdExit );
            break;
            }
        default:
            break;
        }   
    }

// ---------------------------------------------------------
// CCbsUiSettingsView::DoActivateL
// 
// Creates container, and calls updatenavipaneL()
// ---------------------------------------------------------
//
void CCbsUiSettingsView::DoActivateL( const TVwsViewId& aPrevViewId, 
                                     TUid /*aCustomMessageId*/, 
                                     const TDesC8& /*aCustomMessage*/ )
    {    
    __ASSERT_DEBUG( &aPrevViewId, CbsUiPanic(EErrNullPointer) );
    iPrevViewId = aPrevViewId;
    
    if ( iPrevViewId.iAppUid != KUidCbsUiappApp )
        {
        iFromOtherApplication = ETrue;
        }
    else
        {
        iFromOtherApplication = EFalse;
        }
    
    if ( !iContainer )
        {
        iContainer = CCbsUiSettingsViewContainer::NewL( 
            *this, 
            ClientRect(),
            iServer );        
        AppUi()->AddToViewStackL( *this, iContainer );
        }
    else 
        {
        iContainer->SetFocusToFirstItem();
        }
    
    iContainer->ActivateL();
    
    UpdateNaviPaneL(); 
    UpdateTitlePaneL();

    }

// ---------------------------------------------------------
// CCbsUiSettingsView::DoDeactivate
// 
// Deletes container and decorated labels
// ---------------------------------------------------------
//
void CCbsUiSettingsView::DoDeactivate()
    {   
    if ( iContainer )
        {
        // Remove view and its control from the view stack
        AppUi()->RemoveFromViewStack( *this, iContainer );
        // Destroy the container control
        delete iContainer;
        iContainer = NULL;
        }
    
    delete iDecoratedLabel;
    iDecoratedLabel = NULL;
    }

// ---------------------------------------------------------
// CCbsUiSettingsView::UpdateNaviPaneL
// 
// Must be implemented
// ---------------------------------------------------------
//
void CCbsUiSettingsView::UpdateNaviPaneL()
    {
    __ASSERT_DEBUG( !iDecoratedLabel, CbsUiPanic(EErrAllreadyExists) );
    
    delete iDecoratedLabel;
    iDecoratedLabel = NULL;
    
    iDecoratedLabel = CbsUiUtility::CreateEmptyNavigationPaneL();
    }

// ---------------------------------------------------------
// CCbsUiSettingsView::UpdateSettingsL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsView::UpdateSettingsL()
    {
    if ( iContainer )
        {
        iContainer->UpdateSettingsL();
        }
    }

// ---------------------------------------------------------
// CCbsUiSettingsView::UpdateContextPaneL
//
//
// ---------------------------------------------------------
//
void CCbsUiSettingsView::UpdateContextPaneL()
    {
    __ASSERT_DEBUG( iImageLibrary, CbsUiPanic(EErrNullPointer) );
    
    CEikStatusPane* sp = StatusPane();
    
    __ASSERT_DEBUG( sp, CbsUiPanic( EStatusPaneError ) );
    
    TBool reception;
    iServer.GetReceptionStatus( reception );

	CAknTitlePane* title = STATIC_CAST(CAknTitlePane*, sp->
        ControlL( TUid::Uid(EEikStatusPaneUidTitle) ));
        
    __ASSERT_DEBUG( title, CbsUiPanic(EErrNullPointer) );    
	
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    
    if (reception)
        {
        CFbsBitmap* bmp = NULL;
        CFbsBitmap* maskbmp = NULL;
#ifdef __SCALABLE_ICONS
        AknsUtils::CreateIconL( skin, KAknsIIDQgnMenuCbCxt, bmp,
            maskbmp, *iImageLibrary, EMbmCbsuiappQgn_menu_cb,
            EMbmCbsuiappQgn_menu_cb_mask );
#else
        AknsUtils::CreateIconL( skin, KAknsIIDQgnMenuCbCxt, bmp,
            maskbmp, *iImageLibrary, EMbmCbsuiappQgn_menu_cb_cxt,
            EMbmCbsuiappQgn_menu_cb_cxt_mask );
#endif  //__SCALABLE_ICONS
        title->SetSmallPicture(bmp, maskbmp, ETrue);
        }
    else
        {
        CFbsBitmap* bmp = NULL;
        CFbsBitmap* maskbmp = NULL;
#ifdef __SCALABLE_ICONS
        AknsUtils::CreateIconL( skin, KAknsIIDQgnMenuCb2Cxt, bmp,
            maskbmp, *iImageLibrary, EMbmCbsuiappQgn_menu_cb_2,
            EMbmCbsuiappQgn_menu_cb_2_mask);
#else
        AknsUtils::CreateIconL( skin, KAknsIIDQgnMenuCb2Cxt, bmp,
            maskbmp, *iImageLibrary, EMbmCbsuiappQgn_menu_cb_2_cxt,
            EMbmCbsuiappQgn_menu_cb_2_cxt_mask);
#endif  //__SCALABLE_ICONS
        title->SetSmallPicture(bmp, maskbmp, ETrue);
        }

    }

// ---------------------------------------------------------
// CCbsUiSettingsView::SettingsChangedL
//
//
// ---------------------------------------------------------
//
void CCbsUiSettingsView::SettingsChangedL( TCbsSettingsEvent aEvent )
    {
    if ( aEvent == ECbsModifiedLanguages )
        {
        UpdateSettingsL();
        }
    UpdateContextPaneL();
    }


// ---------------------------------------------------------
// CCbsUiSettingsView::HandleRectChangeInContainer
//
//
// ---------------------------------------------------------
//
void CCbsUiSettingsView::HandleRectChangeInContainer()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }


// ---------------------------------------------------------
// CCbsUiSettingsView::DynInitMenuPaneL
//
// Delete help menu item if help is not in use
//
// ---------------------------------------------------------
//
void CCbsUiSettingsView::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
    {
    switch ( aResourceId )
        {
        case R_CBSUI_SETTINGSVIEW_FILE_MENU:
            { 
            if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                aMenuPane->DeleteMenuItem( EAknCmdHelp );
                }
            break;
            } 
        default:
            break;
        }
    }


// ---------------------------------------------------------
// CCbsUiSettingsView::ActivateTopicListViewL
//
//
// ---------------------------------------------------------
//
void CCbsUiSettingsView::ActivateTopicListViewL() const
    {
    TPckgBuf<TCbsTopicNumber> pckgDummy(KCbsInvalidTopicNumber);
    
    AppUi()->ActivateLocalViewL( 
        TUid::Uid(ECbsUiTopicListViewId), 
        KSetFocusToCurrentId, 
        pckgDummy);
    }


// ---------------------------------------------------------
// CCbsUiTopicListView::UpdateTitlePaneL
// 
// ---------------------------------------------------------
//
void CCbsUiSettingsView::UpdateTitlePaneL() const
    {
    CEikStatusPane *sp = StatusPane();
    CAknTitlePane *title = STATIC_CAST( CAknTitlePane*, sp->
        ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
    
    __ASSERT_DEBUG( sp && title, CbsUiPanic( EStatusOrTitlePaneError ) );
    
    TResourceReader titleReader;
    iCoeEnv->CreateResourceReaderLC( titleReader, R_QTN_CB_TITLE_CBS );
    title->SetFromResourceL( titleReader );
    CleanupStack::PopAndDestroy(); // titleReader
    }


//  End of File  
