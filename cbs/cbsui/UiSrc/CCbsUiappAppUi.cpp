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
*  Implements the User Interface class.
*
*
*/


// INCLUDES
#include <bldvariant.hrh>       // KFeatureIdBtSap
#include <e32property.h>        // RProperty
#include <PSVariables.h>        // Property values
#include <featmgr.h>            // FeatureManager
#include <AknsConstants.h>      // KAknsMessageSkinChange
#include <AknsUtils.h>          // CreateIconL, SkinInstance
#include <cbsuiapp.mbg>         // TMbmCbsuiapp
#include <avkon.hrh>
#include <aknnotedialog.h>      // ELongTimeout
#include <aknnotewrappers.h>
#include <akntitle.h> 
#include <hlplch.h>
#include <BTSapDomainPSKeys.h>  // KPSUidBluetoothSapConnectionState, KBTSapConnectionState
#include <data_caging_path_literals.hrh> // KDC_RESOURCE_FILES_DIR
#include <cbsuiApp.rsg>                  // R_QTN_OFFLINE_NOT_POSSIBLE_SAP
#include "CCbsUiappAppUi.h"
#include "CCbsUiTopicListMonitor.h"
#include "CbsUiPanic.h"
#include "CCbsUiViewFactory.h"
#include "MCbs.h"


// CONSTANTS
_LIT(KDirAndCbsUiAppMbmFile,"z:cbsuiapp.mbm");



// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.

CCbsUiappAppUi::CCbsUiappAppUi( MCbs& aServer ) : iServer( aServer )
    {
    }

// Destructor
CCbsUiappAppUi::~CCbsUiappAppUi()
    {
    delete iTopicListMonitor;
    FeatureManager::UnInitializeLib();
    }

// ----------------------------------------------------
// CCbsUiappAppUi::HandleResourceChangeL
//
//
// ----------------------------------------------------
void CCbsUiappAppUi::HandleResourceChangeL( TInt aType )
    {
    CAknViewAppUi::HandleResourceChangeL( aType );

    if( aType == KAknsMessageSkinChange )
        {
        // change application icon dynamically
        TBool reception;
        CEikStatusPane *sp = ( (CAknAppUi*)CEikonEnv::Static()->EikAppUi() )->StatusPane();
		CAknTitlePane* title = STATIC_CAST(CAknTitlePane*, sp->
	        ControlL( TUid::Uid(EEikStatusPaneUidTitle) ));
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        iServer.GetReceptionStatus( reception );

        TParse parse; 
        parse.Set( KDirAndCbsUiAppMbmFile, &KDC_APP_BITMAP_DIR, NULL ); 
        TFileName mbmFileAndPathName(parse.FullName());
        
        if (reception)
            {
            
            CFbsBitmap* bmp = NULL;
            CFbsBitmap* maskbmp = NULL;
#ifdef __SCALABLE_ICONS
            AknsUtils::CreateIconL( skin, KAknsIIDQgnMenuCbCxt, bmp,
                maskbmp, mbmFileAndPathName, EMbmCbsuiappQgn_menu_cb,
                EMbmCbsuiappQgn_menu_cb_mask );
#else
            AknsUtils::CreateIconL( skin, KAknsIIDQgnMenuCbCxt, bmp,
                maskbmp, mbmFileAndPathName, EMbmCbsuiappQgn_menu_cb_cxt,
                EMbmCbsuiappQgn_menu_cb_cxt_mask );
#endif    //__SCALABLE_ICONS
            title->SetSmallPicture(bmp, maskbmp, ETrue);
            }
        else
            {
            CFbsBitmap* bmp = NULL;
            CFbsBitmap* maskbmp = NULL;
#ifdef __SCALABLE_ICONS
            AknsUtils::CreateIconL( skin, KAknsIIDQgnMenuCb2Cxt, bmp,
                maskbmp, mbmFileAndPathName, EMbmCbsuiappQgn_menu_cb_2,
                EMbmCbsuiappQgn_menu_cb_2_mask);
#else
            AknsUtils::CreateIconL( skin, KAknsIIDQgnMenuCb2Cxt, bmp,
                maskbmp, mbmFileAndPathName, EMbmCbsuiappQgn_menu_cb_2_cxt,
                EMbmCbsuiappQgn_menu_cb_2_cxt_mask);
#endif    //__SCALABLE_ICONS
            title->SetSmallPicture(bmp, maskbmp, ETrue);
            }
        }    
    }



// Symbian OS default constructor can leave.
void CCbsUiappAppUi::ConstructL()
    {
    BaseConstructL( EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible );

    FeatureManager::InitializeLibL();
    if ( FeatureManager::FeatureSupported( KFeatureIdBtSap ) )
        {
        TInt simStatus;
        simStatus = CheckSimStateL();
        if ( EBTSapNotConnected != simStatus )
            {
            HBufC* text = iCoeEnv->AllocReadResourceLC( R_QTN_OFFLINE_NOT_POSSIBLE_SAP );
            CAknInformationNote* note = new( ELeave ) CAknInformationNote( ETrue );
            note->SetTimeout( CAknNoteDialog::ELongTimeout );
            note->ExecuteLD( *text );
            CleanupStack::PopAndDestroy( ); //text
            User::Leave( KErrGeneral );
            }
        }
    
    __ASSERT_DEBUG( &iServer, CbsUiPanic(EServerCreationFailedError) );
    __ASSERT_DEBUG( iServer.Connected(), CbsUiPanic(EErrServerDisconnected) );
    
    iTopicListMonitor = CCbsUiTopicListMonitor::NewL(iServer);
    
    CCbsUiViewFactory* viewFactory = new (ELeave) CCbsUiViewFactory(
        iServer, *iTopicListMonitor);
    CleanupStack::PushL(viewFactory);
    
    AddViewL( viewFactory->CreateTopicListViewLC() );
    CleanupStack::Pop(); // view
    
    AddViewL( viewFactory->CreateTopicViewLC() );
    CleanupStack::Pop(); // view
    
    AddViewL( viewFactory->CreateMsgViewLC() );
    CleanupStack::Pop(); // view
    
    AddViewL( viewFactory->CreateSettingsViewLC() );
    CleanupStack::Pop(); // view
    
    CleanupStack::PopAndDestroy(); // viewFactory
    }

// ---------------------------------------------------------
// CCbsUiappAppUi::HandleCommandL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiappAppUi::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EAknCmdHelp:
            {
            HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), AppHelpContextL());
            break;
            }
        case EAknCmdExit:
        case EEikCmdExit:
            Exit();
            break;
        default:
            break;
        }
    }


// ---------------------------------------------------------
// CCbsUiappAppUi::CheckSimStateL
//
//
// ---------------------------------------------------------
TInt CCbsUiappAppUi::CheckSimStateL()
    {
    TInt btSapState( EBTSapNotConnected );
    TInt err = RProperty::Get( KPSUidBluetoothSapConnectionState, 
                               KBTSapConnectionState, 
                               btSapState );
    if ( err )
        {
        btSapState = EBTSapNotConnected;
        }
    return btSapState;
    }

//  End of File  
