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
*              : CSsSettingsHandler class
*
*/


//  INCLUDE FILES
#include "csssettingshandler.h"
#include "csslogger.h"
#include <spdefinitions.h>
#include <spproperty.h>
#include <spentry.h>

// CONSTANTS
// Default voip service id that is not really a voip capable.
const TInt KSPDefaultVoIPServiceId = 2;

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CSsSettingsHandler::CSsSettingsHandler
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSsSettingsHandler::CSsSettingsHandler
        ( 
        // None.
        )
    {
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSsSettingsHandler::ConstructL()
    {
    iSettings = CSPSettings::NewL();
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSsSettingsHandler* CSsSettingsHandler::NewL
        ( 
        // None.
        )
    {
    CSsSettingsHandler* self = new ( ELeave ) CSsSettingsHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CSsSettingsHandler::~CSsSettingsHandler
        (
        // None.
        )
    {
    delete iSettings;
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::GetRegisteredServices
// Gets registered services.
// -----------------------------------------------------------------------------
//
TInt CSsSettingsHandler::GetRegisteredServices
        (
        CSsConnectionHandler& aConnection, 
        RArray<TUint>& aServiceIds
        ) const
    {
    TRAPD( ret, DoGetRegisteredServicesL( aConnection,
                                          aServiceIds ) );
    
    return ret;
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::GetRegisteredServices
// Gets all available services.
// -----------------------------------------------------------------------------
//
TInt CSsSettingsHandler::GetAllServices
        (
        RArray<TUint>& aServiceIds
        ) const
    {
    TRAPD( ret, DoGetAllServicesL( aServiceIds ));
    
    return ret;
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::DoGetRegisteredServicesL
// Gets registered services.
// -----------------------------------------------------------------------------
//
void CSsSettingsHandler::DoGetRegisteredServicesL
        (
        CSsConnectionHandler& aConnection, 
        RArray<TUint>& aServiceIds
        ) const
    {
    CSSLOGSTRING("CSSelector::RegisteredServices");
    
    //Clear array
    aServiceIds.Reset();
    
    RArray<TUint> temp;
    CleanupClosePushL( temp );
    
    // Get all voip service ids.
    GetVoipServicesL( temp );
    
    for ( TInt ind=0; ind< temp.Count(); ind++ )
        {
        // Make sure that the service is registered
        if ( aConnection.IsVoipServiceRegistered( temp[ind] ) )
            {
            // Registered service found
            User::LeaveIfError( aServiceIds.Append( temp[ind] ) );
            }
        
        } 
    
    CSSLOGSTRING2("CSSelector::RegisteredServices count:%d", 
            aServiceIds.Count());
    
    if ( 0 == aServiceIds.Count() )
        {
        // Not any services found, leave.
        User::Leave( KErrNotFound );
        }
    
    CleanupStack::PopAndDestroy( &temp );
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::DoGetAllServicesL
// Gets all available services.
// -----------------------------------------------------------------------------
//
void CSsSettingsHandler::DoGetAllServicesL
        (
        RArray<TUint>& aServiceIds
        ) const
    {
    CSSLOGSTRING("CSSelector::AllServices");
    //Clear array
    aServiceIds.Reset();
    
    // Get voip services
    GetVoipServicesL( aServiceIds );        
    
    CSSLOGSTRING2("CSSelector::AllServices count:%d", aServiceIds.Count());
    
    if ( 0 == aServiceIds.Count() )
        {
        // Not any services found, leave.
        User::Leave( KErrNotFound );
        }
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::GetVoipServicesL
// Gets voip services.
// -----------------------------------------------------------------------------
//
void CSsSettingsHandler::GetVoipServicesL
        (
        RArray<TUint>& aServiceIds
        ) const
    {
    //Clear array
    aServiceIds.Reset();
    
    RArray<TUint> temp;
    CleanupClosePushL( temp );
    
    // Get all service ids.
    User::LeaveIfError( iSettings->FindServiceIdsL( temp ) );
    
    for ( TInt ind=0; ind< temp.Count(); ind++ )
        {
        // Make sure that the service is a voip capable.
        // For some reason the service id 2 is not voip capable
        // service (although contains the voip subservice properties) 
        // so ignore it.
        if ( KSPDefaultVoIPServiceId != temp[ind] )
            {
            TBool voip( EFalse );
            TRAPD( err, voip = IsVoipServiceL( temp[ind] ) );
            if ( !err && voip )
                {
                // Add voip service
                User::LeaveIfError( aServiceIds.Append( temp[ind] ) );
                }
            }
        
        }
    
    CleanupStack::PopAndDestroy( &temp );
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::IsVoipServiceL
// Checks is the service as voip service.
// -----------------------------------------------------------------------------
//
TBool CSsSettingsHandler::IsVoipServiceL
        (
        TUint aServiceId 
        ) const
    {
    CSPProperty* property = CSPProperty::NewLC();
    
    TInt ret = iSettings->FindPropertyL( aServiceId, 
                                         EPropertyVoIPSubServicePluginId , 
                                         *property );

    CleanupStack::PopAndDestroy( property );
    
    // If Voip subservice properties found (ret = KErrNone) then return ETrue.
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSsSettingsHandler::GetServiceNamesL
// Checks is the service as voip service.
// -----------------------------------------------------------------------------
//
void CSsSettingsHandler::GetServiceNamesL
        (
        RArray<TUint>& aServiceIds,
        CDesCArray& aServiceNames
        ) const
    {
    if ( 0 == aServiceIds.Count() )
        {
        // Empty array, leave.
        User::Leave( KErrNotFound );
        }
    
    // Get names of the services.
    User::LeaveIfError( iSettings->FindServiceNamesL( aServiceIds,
                                                      aServiceNames ) );
    }

// ---------------------------------------------------------------------------
// CSsSettingsHandler::BrandIdL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CSsSettingsHandler::BrandIdL( TInt aServiceId, TDes8& aBrandId )
    {
    CSSLOGSTRING( "CSsSettingsHandler::BrandIdL: =>" );

    CSPEntry* entry = CSPEntry::NewLC();
    const CSPProperty* property;
    RBuf brandId;
    brandId.CleanupClosePushL();
    brandId.CreateL( KSPMaxDesLength );
    
    User::LeaveIfError(
        iSettings->FindEntryL( aServiceId, *entry ) );

    User::LeaveIfError(
        entry->GetProperty( property, EPropertyBrandId ) );

    if ( property )
        {
        property->GetValue( brandId );
        }

    if ( brandId.Length() )
        {
        aBrandId.Copy( brandId );
        }

    CleanupStack::PopAndDestroy( &brandId );
    CleanupStack::PopAndDestroy( entry );

    CSSLOGSTRING( "CSsSettingsHandler::BrandIdL: <=" );
    }

// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
