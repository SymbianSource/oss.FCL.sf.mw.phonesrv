/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CallUI plugin.
*
*/



// INCLUDE FILES
#include    "CaUiPlugin.h"  // This class interface.
#include    "CaUiEng.h"     // CaUiEngine.
#include    <AiwCommon.hrh> // AIW constants.
#include    "CaUI.hrh"      // CallUI constants.
#include    <AiwMenu.h>     // AIW Menu pane.

#include	<AiwServiceHandler.h>
#include    <PhCltTypes.h>  // PhoneClient types.
#include    "CaUiLogger.h"  // Call Ui Logger
#include    <stringloader.h>
#include    <CallUI.rsg>
#include    <featmgr.h>      // FeatureManager.

// CONSTANTS

// Empty filename for initialization.
_LIT( KCaUiNullResFile, "" );

// The library to be loaded.
_LIT( KCaUiLoadedLib, "CaUiEngine.dll" );



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaUiPlugin::CCaUiPlugin
// 
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCaUiPlugin::CCaUiPlugin()
    {
    }

// -----------------------------------------------------------------------------
// CCaUiPlugin::ConstructL
// 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaUiPlugin::ConstructL()
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::ConstructL() Begin");
    User::LeaveIfError( iLibrary.Load( KCaUiLoadedLib ) );

    // Call function CreateCaUiEngFactoryL()
    TInt res = iLibrary.Lookup( 1 )();
    CCaUiEngFactory* caUiEngFactory = 
        reinterpret_cast< CCaUiEngFactory* >( res );

    iCaUiEngine = caUiEngFactory->CCaUiEngApiLD();

    if( !iCaUiEngine )
        {
        // Null returned, so leave.
        User::Leave( KErrNotSupported );
        }
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::ConstructL() End");
    }


// -----------------------------------------------------------------------------
// CCaUiPlugin::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaUiPlugin* CCaUiPlugin::NewL()
    {
    CCaUiPlugin* self = new( ELeave ) CCaUiPlugin;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// -----------------------------------------------------------------------------
// CCaUiPlugin::~CCaUiPlugin
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiPlugin::~CCaUiPlugin()
    {
    delete iCaUiEngine;

    iLibrary.Close();
    }


// -----------------------------------------------------------------------------
// CCaUiPlugin::InitialiseL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiPlugin::InitialiseL(
    MAiwNotifyCallback& /*aFrameworkCallback*/,
    const RCriteriaArray& /*aInterest*/)
    {
    // Stub. Not used.
    }


// -----------------------------------------------------------------------------
// CCaUiPlugin::HandleServiceCmdL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiPlugin::HandleServiceCmdL(
    const TInt& aCmdId,
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    const MAiwNotifyCallback* aCallback )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::HandleServiceCmdL() Begin");
    // Handle only call command.
    if ( aCmdId == KAiwCmdCall )
        {
        if ( aCmdOptions & KAiwOptCancel )
            {
            CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::HandleServiceCmdL() Cancel");
            return;
            }
        // Check if there is CallUI Dialdata parameter.
        TInt index = 0;
        const TAiwGenericParam* genericParam = NULL; 
            
        genericParam = aInParamList.FindFirst( 
            index,
            EGenericParamCallDialData,
            EVariantTypeDesC8 );
            
        if ( index >= 0 && genericParam )
            {
            // Get the data.
            TPtrC8 ptr = genericParam->Value().AsData();

            // If this function was called through HandleMenuCmdL,
            // iMenuCmdId was set, otherwise it is zero.
            TCaUiCallType callType = ECaUiCallTypeUnknown;

            if ( iMenuCmdId == ECallUIVoice )
                {
                callType = ECaUiCallTypeVoice;
                }
            else if ( iMenuCmdId == ECallUIVideo )
                {
                callType = ECaUiCallTypeVideo;
                }
            else if ( iMenuCmdId == ECallUIInternet )
                {
                callType = ECaUiCallTypeInternet;
                }

            iMenuCmdId = 0;

            // Save the parameters given.
            iAiwNotifyCallback = aCallback;
            iInParamList = &aInParamList;
            iOutParamList = &aOutParamList;

            // Perform the dial operation.
            iCaUiEngine->DialL( *this, ptr, callType );
            }
        }
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::HandleServiceCmdL() End");
    }



// -----------------------------------------------------------------------------
// CCaUiPlugin::InitializeMenuPaneL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiPlugin::InitializeMenuPaneL(
    CAiwMenuPane& aMenuPane,
    TInt aIndex,
    TInt /* aCascadeId */,
    const CAiwGenericParamList& aInParamList )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::InitializeMenuPaneL() Begin");

    // By default we want all internet, voice and video call resources
    TBool csVoiceMenuItemAvailable( ETrue );
    TBool csVideoMenuItemAvailable( ETrue );
    TBool voipMenuItemAvailable( ETrue );
    
    // If there is PhoneNumber parameter given, then only voice call 
    // resource is wanted.
    TInt count = aInParamList.Count();
    if ( count )
        {
        TInt index = 0;
        aInParamList.FindFirst(
            index,
            EGenericParamPhoneNumber,
            EVariantTypeAny );

        if ( index >= 0  )
            {
            csVideoMenuItemAvailable = EFalse;
            voipMenuItemAvailable = EFalse;
            }
            
        if ( index == KErrNotFound )
            {
            index = 0;    
            aInParamList.FindFirst(
                index,
                EGenericParamSIPAddress,
                EVariantTypeAny );

            if ( index >= 0  )
                {
                // Internet number parameter was found, 
                csVoiceMenuItemAvailable = EFalse;
                csVideoMenuItemAvailable = EFalse;
                }
            }
        }
    
    if ( csVideoMenuItemAvailable && 
    		!FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
    	{
    	csVideoMenuItemAvailable = EFalse;
    	}
    
    // Add menu items

    TInt menuIndex = aIndex;

    if ( csVoiceMenuItemAvailable )
         {
         AddAiwMenuItemL( aMenuPane, menuIndex, ECSVoice );
         menuIndex++;
         }
    
    if ( csVideoMenuItemAvailable )
        {
        AddAiwMenuItemL( aMenuPane, menuIndex, ECSVideo );
        menuIndex++;
        }
    
    if ( voipMenuItemAvailable )
        {
        RIdArray voipServiceIds;
        CleanupClosePushL( voipServiceIds );
        iCaUiEngine->GetVoIPServiceIdsL( voipServiceIds );
        TInt numberOfVoipServices = voipServiceIds.Count();
        
        if ( 1 == numberOfVoipServices )
            {
            // Single VoIP service, use service name in menu item
             AddAiwMenuItemL( aMenuPane, menuIndex, EInternetWithName, voipServiceIds[0] );
             menuIndex++;
            }
        else if ( numberOfVoipServices > 1 )
            {
            // Regular internet call menu
            AddAiwMenuItemL( aMenuPane, menuIndex, EInternet );
            menuIndex++;
            }    

        CleanupStack::PopAndDestroy( &voipServiceIds );
        }

    // Set submenu title
    if ( csVoiceMenuItemAvailable ||
         csVideoMenuItemAvailable || 
         voipMenuItemAvailable )
        {
        HBufC* menuTitle = StringLoader::LoadLC( R_CALLUI_CALL_SUBMENU_TITLE );
        aMenuPane.AddTitleItemL( *menuTitle, aIndex );
        CleanupStack::PopAndDestroy( menuTitle );
        }

    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::InitializeMenuPaneL() End");
    }


// -----------------------------------------------------------------------------
// CCaUiPlugin::HandleMenuCmdL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiPlugin::HandleMenuCmdL(
    TInt aMenuCmdId, 
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    const MAiwNotifyCallback* aCallback )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::HandleMenuCmdL() Begin");
    // Handle only call commands.
    if ( ( aMenuCmdId == ECallUIVideo ) || 
         ( aMenuCmdId == ECallUIVoice ) ||
         ( aMenuCmdId == ECallUIInternet ) )
        {
        // Save the menu command.
        iMenuCmdId = aMenuCmdId;

        // Menu commands are handled as service commands.
        HandleServiceCmdL(
            KAiwCmdCall,
            aInParamList,
            aOutParamList,
            aCmdOptions,
            aCallback );
        }
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::HandleMenuCmdL() End");
    }


// -----------------------------------------------------------------------------
// CCaUiPlugin::HandleDialResultL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiPlugin::HandleDialResultL( const TInt aStatus )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::HandleDialResultL() Start");
    // If callback exists, inform client of call result.
    if ( iAiwNotifyCallback )
        {
        // Add the call result to the out parameter list.
        TAiwVariant variant( aStatus );
        TAiwGenericParam genericParam( EGenericParamError, variant );
        iOutParamList->AppendL( genericParam);

        // R&D solution: Remove constness.
        MAiwNotifyCallback* callback = 
            const_cast< MAiwNotifyCallback* >( iAiwNotifyCallback );

        // Notify client.
        callback->HandleNotifyL(
            KAiwCmdCall,
            KAiwEventStarted,
            *iOutParamList,
            *iInParamList );
        }

    // Callback not active anymore, make clearing.
    iAiwNotifyCallback = NULL;
    iInParamList = NULL;
    iOutParamList = NULL;
    CAUILOGSTRING("CALLUI: >>>CCaUiPlugin::HandleDialResultL() End");
    }

// -----------------------------------------------------------------------------
// CCaUiPlugin::AddAiwMenuItemL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiPlugin::AddAiwMenuItemL( CAiwMenuPane& aMenuPane, TInt aIndex, EMenuItemType aType, TServiceId aServiceId )
    {
    CEikMenuPaneItem::SData data;
    data.iCascadeId = 0;
    data.iFlags = 0;
    data.iExtraText = KNullDesC();

    HBufC* menuItemText = NULL;
    
    switch ( aType )
        {
        case ECSVoice:
            {
            data.iCommandId = ECallUIVoice;
            menuItemText = StringLoader::LoadLC( R_CALLUI_CS_VOICE_CALL );
            data.iText.Copy( *menuItemText );
            break;
            }

        case ECSVideo:
            {
            data.iCommandId = ECallUIVideo;
            menuItemText = StringLoader::LoadLC( R_CALLUI_CS_VIDEO_CALL );
            data.iText.Copy( *menuItemText );
            break;
            }

        case EInternet:
            {
            data.iCommandId = ECallUIInternet;
            menuItemText = StringLoader::LoadLC( R_CALLUI_INTERNET_CALL );
            data.iText.Copy( *menuItemText );
            break;
            }

        case EInternetWithName:
            {
            data.iCommandId = ECallUIInternet;
            
            // Get the service provider name
            TBuf<100> buf;
            iCaUiEngine->GetVoipServiceNameL( aServiceId, buf );
            menuItemText = StringLoader::LoadLC( R_CALLUI_XSP_CALL_WITH_SERVICE_NAME, buf );
            data.iText.Copy( *menuItemText );
            break;
            }

        default:
            break;
        }

    if ( NULL != menuItemText )
        {
        CleanupStack::PopAndDestroy( menuItemText );
        }
    
    aMenuPane.AddMenuItemL(
       KAiwCmdCall,
       data,
       aIndex );
    }

// End of file
