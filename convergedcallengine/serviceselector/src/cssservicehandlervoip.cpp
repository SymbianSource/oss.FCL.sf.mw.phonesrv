/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of 
*              : CSsServiceHandlerVoip class
*
*/


//  INCLUDE FILES
#include "cssservicehandlervoip.h"
#include "cssconnectionhandler.h"
#include "cssuiutilities.h"
#include "cssserviceutilities.h"
#include "csssettingshandler.h"
#include "cssbshandler.h"
#include "csslogger.h"
#include <serviceselector.rsg>
#include <spdefinitions.h>
#include <spsettingsvoiputils.h>
#include <data_caging_path_literals.hrh>
#include <eikenv.h>


// CONSTANTS
// Selection list array granularity
const TInt KSsArrayGranularity = 2;
// CS service id
const TInt KGSMServiceId = 1;
// List item array size
const TInt KSsLbxItemsArraySize = 3;
// Format of the selection list query.
_LIT( KQueryItemFormat, "%d\t%S" );
// Branding id
const TInt KSsBrandingIdLength = 512;
// Integer length
const TInt KSsIntegerLength = 10;


// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::CSsServiceHandlerVoip
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSsServiceHandlerVoip::CSsServiceHandlerVoip
        ( 
        // None.
        )
    {
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoip::ConstructL()
    {
    iConnectionHandler = CSsConnectionHandler::NewL();
    iVoipUtils = CSPSettingsVoIPUtils::NewL();
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSsServiceHandlerVoip* CSsServiceHandlerVoip::NewL
        ( 
        // None.
        )
    {
    CSsServiceHandlerVoip* self = new ( ELeave ) CSsServiceHandlerVoip();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CSsServiceHandlerVoip::~CSsServiceHandlerVoip
        (
        // None.
        )
    {
    delete iVoipUtils;
    delete iConnectionHandler;
    delete iUiUtils;
    delete iSettings;
    delete iBsHandler;
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::ExecuteServiceSelectionL
// Gets the calling service by current call type and service id.
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoip::ExecuteServiceSelectionL
        ( 
        CConvergedServiceSelector::TSsResult& aResult, 
        CConvergedServiceSelector::TSsCallType /*aCallType*/,
        TUint aServiceId,
        TBool aSendKey,
        const TDesC& aString 
        )
    {
    CSSLOGSTRING("CSSelector::ExecuteVoip");
    
    if ( 0 != aServiceId && KGSMServiceId != aServiceId )
        {
        // Service id is known, just set correct calltype
        // and registration status.
        GetServiceByServiceId( aResult, aServiceId, aSendKey, aString );
        }
    else
        {
        // Service id is not known or CS service
        // so we have to choce from available voip services.
        GetCallingServiceL( aResult );
        }

    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::GetServiceByServiceId
// Gets calling service by known service id.
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoip::GetServiceByServiceId
        ( 
        CConvergedServiceSelector::TSsResult& aResult, 
        TUint aServiceId,
        TBool aSendKey,
        const TDesC& aString
        ) const
    {
    if ( aSendKey && ChangeTypeAsCsVoice( aServiceId, aString ) )
        {
        // Set (CS)voice call parameters
        // CallType, Service id and GSM service is always "enabled"
        SsServiceUtilities::SetServiceResult( 
                aResult,
                CConvergedServiceSelector::ESsVoiceCall,
                KGSMServiceId,
                ETrue ); 
        }
    else
        {
        // Get current registration status for the service.
        // This may be unnecessary at the moment because we don't show  
        // "Register now?" note anymore and currently clients don't use the 
        // registration status for anything.
        TBool registered( iConnectionHandler->IsVoipServiceRegistered( 
                                                            aServiceId ) );
    
        // Set results    
        SsServiceUtilities::SetServiceResult( 
                aResult,
                CConvergedServiceSelector::ESsVoipCall,
                aServiceId,
                registered );
        }
    }
            
// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::GetCallingServiceL
// Gets calling service without service id.
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoip::GetCallingServiceL
        ( 
        CConvergedServiceSelector::TSsResult& aResult
        )
    {
    TInt error( KErrNone );
    TBool registered( EFalse );
    RArray<TUint> services;
    CleanupClosePushL( services );
    
    // First try to find registered services to be used if not any available
    // then try to find unregistered services.
    error = SettingsHandlerL().GetRegisteredServices( 
                *iConnectionHandler, services );
    
    if ( KErrNone != error )
        {
        error = SettingsHandlerL().GetAllServices( services );
        }
    else
        {
        registered = ETrue;
        }
    
    CSSLOGSTRING2("CSSelector::Service error:%d", error);
    if ( KErrNone != error )
        {
        CSSLOGSTRING("CSSelector::Services not available");
        if ( KErrNoMemory == error )
            {
            User::Leave( error );
            }
        // No voip services available/configured. 
        // Launch information note and leave.
        UiUtilitiesL().ShowInformationNoteL( R_NO_SERVICE_AVAILABLE );
        User::Leave( KErrNotFound );
        }

   
    // Select service. Show list query
    // if more than one services available.
    SelectServiceL( aResult,
                    services,
                    registered );
    
    CleanupStack::PopAndDestroy( &services );
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::SelectServiceL
// Makes service selection.
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoip::SelectServiceL
        ( 
        CConvergedServiceSelector::TSsResult& aResult, 
        RArray<TUint>& aServices,
        TBool aRegistered 
        )
    {
    CSSLOGSTRING2("CSSelector::Service count:%d",aServices.Count());
    if ( 0 == aServices.Count() )
        {
        // Empty array, leave
        User::Leave( KErrNotFound );
        }
    
    TUint serviceId( 0 );
    if ( 1 < aServices.Count() )
        {
        // Because more than one service available we have to
        // make selection list for the user.
        TInt index( KErrNotFound );

        CDesCArrayFlat* lbxItems =
            new (ELeave) CDesCArrayFlat( KSsLbxItemsArraySize );
        CleanupStack::PushL( lbxItems );
        CArrayPtr<CGulIcon>* icons =
            new (ELeave) CAknIconArray( KSsLbxItemsArraySize );
        CleanupStack::PushL( icons );
        
        GetListBoxItemsAndIconsL( aServices,
                                  lbxItems,
                                  icons );
        
        // List query takes ownership for icon array
        CleanupStack::Pop( icons );
        
        // Select service by list query
        TBool ret = UiUtilitiesL().ShowListQueryL( 
                index, R_PHONE_VOIP_CALL_TYPE_QUERY, lbxItems, icons );

        CSSLOGSTRING2("CSSelector::Service list ret:%d", ret);
        CSSLOGSTRING2("CSSelector::Service list index:%d", index);
        
        
        // Make sure that the index is on the correct range and user has 
        // not canceled the list query.
        if ( !ret || ( index < 0 || index >= aServices.Count() ) )
            {
            // User cancels the query -> leave
            User::Leave( KErrCancel );
            }
        
        CleanupStack::PopAndDestroy( lbxItems );
        
        // Use the selected service.
        serviceId = aServices[index];
        }
    else
        {
        // One service available, use it.
        serviceId = aServices[0];
        }
    
    
    // Set results    
    SsServiceUtilities::SetServiceResult( 
            aResult,
            CConvergedServiceSelector::ESsVoipCall,
            serviceId,
            aRegistered );
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::UiUtilitiesL
// Returns uiutilities reference.
// -----------------------------------------------------------------------------
//
CSsUiUtilities& CSsServiceHandlerVoip::UiUtilitiesL() 
    {
    if ( !iUiUtils )
        {
        // Create uiutils for ui dialogs.
        // UiUtils is created only for the real need.
        iUiUtils = CSsUiUtilities::NewL();
        }
    
    return *iUiUtils;
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::SettingsHandlerL
// Returns settings handler reference.
// -----------------------------------------------------------------------------
//
CSsSettingsHandler& CSsServiceHandlerVoip::SettingsHandlerL() 
    {
    if ( !iSettings )
        {
        // Create settings handler
        iSettings = CSsSettingsHandler::NewL();
        }
    
    return *iSettings;
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::BrandingHandlerL
// Returns settings handler reference.
// -----------------------------------------------------------------------------
//
CSsBsHandler& CSsServiceHandlerVoip::BrandingHandlerL() 
    {
    if ( !iBsHandler )
        {
        // Create settings handler
        iBsHandler = CSsBsHandler::NewL();
        }
    
    return *iBsHandler;
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::ChangeTypeAsCsVoice
// Checks should voip call type be changed as cs voice.
// -----------------------------------------------------------------------------
//
TBool CSsServiceHandlerVoip::ChangeTypeAsCsVoice
        ( 
        TUint aServiceId,
        const TDesC& aString
        ) const
    {
    TBool csCall( EFalse );
    TUint prefService( 0 );
    
    if ( SsServiceUtilities::IsValidCsNumber( aString ) &&
         iVoipUtils->IsPreferredTelephonyVoIP() &&
         KErrNone == iVoipUtils->GetPreferredService( prefService ) )
        {
        csCall = prefService == aServiceId &&
                 !iConnectionHandler->IsVoipServiceRegistered( aServiceId );
            
        }
    
    return csCall;
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::AddBrandIconL
//
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoip::AddBrandIconL
        (
        CArrayPtr<CGulIcon>* aIcons,
        TUint aServiceId,
        TInt& aIconId 
        )
    {
    CFbsBitmap* brandedBitmap = NULL;
    CFbsBitmap* brandedBitmapMask = NULL;

    // Get branded bitmap
    TRAPD( err, GetBrandIconL( aServiceId,
                               brandedBitmap, 
                               brandedBitmapMask ) );

    if ( KErrNone == err )
        {
        // The count of list icons(before adding branding icon to the list)
        // must be the branded icon list id
        aIconId = aIcons->Count();
        // Create new icon and add it to the icon list
        CGulIcon* icon = CGulIcon::NewL( brandedBitmap, brandedBitmapMask );
        CleanupStack::PushL( icon );
        aIcons->AppendL( icon );
        CleanupStack::Pop( icon );
        }
    else
        {
        // Use default icon for this service
        aIconId = 0;
        }
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::GetListBoxItemsAndIconsL
//
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoip::GetListBoxItemsAndIconsL
        (
        RArray<TUint>& aServices,
        CDesCArrayFlat* aLbxItems,
        CArrayPtr<CGulIcon>* aIcons       
        )
    {
    CDesCArray* names = new(ELeave) CDesCArrayFlat( KSsArrayGranularity );
    CleanupStack::PushL( names );
    
    // Load default listbox icon
    UiUtilitiesL().LoadDefaultIconL( *aIcons );
    
    // Get service names by service id array
    SettingsHandlerL().GetServiceNamesL( aServices, *names );

    for ( TInt i=0; i< aServices.Count() && 
            names->MdcaCount() == aServices.Count(); i++ )
        {
        RBuf16 buf;
        CleanupClosePushL( buf );
        TInt iconId( KErrNotFound );
        
        // Add brand icon. Default is used if brand icon not found
        AddBrandIconL( aIcons, aServices[ i ], iconId );

        HBufC* name = HBufC::NewLC( names->MdcaPoint(i).Length() );
        name->Des().Copy( names->MdcaPoint(i) );
        
        buf.CreateL( names->MdcaPoint(i).Length() +
                     KQueryItemFormat().Length() +
                     KSsIntegerLength );
        
        // Create final list box item data.
        buf.Format( KQueryItemFormat, iconId, name );
       
        //Append text to the item array.
        aLbxItems->AppendL( buf );
        
        CleanupStack::PopAndDestroy( name );
        CleanupStack::PopAndDestroy( &buf );
        }

    CleanupStack::PopAndDestroy( names );
    }

// -----------------------------------------------------------------------------
// CSsServiceHandlerVoip::GetBrandIconL
//
// -----------------------------------------------------------------------------
//
void CSsServiceHandlerVoip::GetBrandIconL
        (
        TUint aServiceId,
        CFbsBitmap*& aBrandedBitmap,
        CFbsBitmap*& aBrandedBitmapMask  
        )
    {
    TBuf8<KSsBrandingIdLength> brandId( KNullDesC8 );

    SettingsHandlerL().BrandIdL( aServiceId, brandId );

    // Get branded bitmap
    BrandingHandlerL().GetBrandedIconL( brandId, 
                                        aBrandedBitmap, 
                                        aBrandedBitmapMask );
    }


// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
