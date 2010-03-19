/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  VoIP profile handler
*
*/



// INCLUDE FILES
#include    "cauivoipextension.h" 
#include    <callui.rsg> 
#include    <featmgr.h> 
#include    <spsettings.h>
#include    <spproperty.h>
#include    <stringloader.h> // String Loader. 



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaUiVoIPExtension::CCaUiVoIPExtension
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCaUiVoIPExtension::CCaUiVoIPExtension()
    {
    }

// -----------------------------------------------------------------------------
// CCaUiVoIPExtension::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaUiVoIPExtension::ConstructL()
    {    

    }

// -----------------------------------------------------------------------------
// CCaUiVoIPExtension::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaUiVoIPExtension* CCaUiVoIPExtension::NewL()
    {
    CCaUiVoIPExtension* self = new( ELeave ) CCaUiVoIPExtension;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// Destructor
CCaUiVoIPExtension::~CCaUiVoIPExtension()
    {

    }
        
// -----------------------------------------------------------------------------
// CCaUiVoIPExtension::IsVoIPProfilesL()
// Checks if any service supports internet call
// -----------------------------------------------------------------------------
//
TBool CCaUiVoIPExtension::IsVoIPProfilesL()
    {
    TBool isProfiles = EFalse;

    RIdArray voipServiceIds;
    CleanupClosePushL( voipServiceIds );
    GetVoIPServiceIdsL( voipServiceIds );
    if ( voipServiceIds.Count() )
        {
        isProfiles = ETrue;
        }
    CleanupStack::PopAndDestroy( &voipServiceIds );
          
    return isProfiles; 
    }

// -----------------------------------------------------------------------------
// CCaUiVoIPExtension::GetVoIPServiceIdsL()
// 
// -----------------------------------------------------------------------------
//
void CCaUiVoIPExtension::GetVoIPServiceIdsL( RIdArray& aVoipServiceIds ) const
    {
    aVoipServiceIds.Reset();

    // Fetch the settings api.
    CSPSettings* settingsApi = CSPSettings::NewLC();               
        
    // read all service ids to array
    RIdArray idArray;
    CleanupClosePushL( idArray );
        
    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        User::LeaveIfError( settingsApi->FindServiceIdsL( idArray ) );
    
        // go throught all the services and check if any
        // of them supports internet call        
        for ( TInt i = 0; idArray.Count() > i; i++)
            {                        
            // check if the service supports internet call                                                              
            CSPProperty* property = CSPProperty::NewLC();
            // get attribute mask of the service
            User::LeaveIfError( settingsApi->FindPropertyL( idArray[i], 
                EPropertyServiceAttributeMask, *property ) );
            
            // read the value of mask property
            TInt mask = 0;                    
            if ( KErrNone == property->GetValue( mask ) )
                {
                if ( ( mask & ESupportsInternetCall )
                    && ( mask & EIsVisibleInCallMenu ) ) 
                    {
                    aVoipServiceIds.Append( idArray[i] );
                    }
                }
            CleanupStack::PopAndDestroy( property );     
            }                                   
        }
    CleanupStack::PopAndDestroy( 2, settingsApi );
    }


// -----------------------------------------------------------------------------
// CCaUiVoIPExtension::GetVoipServiceNameL()
// 
// -----------------------------------------------------------------------------
//        
void CCaUiVoIPExtension::GetVoipServiceNameL( TServiceId aServiceId, TDes& aServiceName ) const
    {
    CSPSettings* settingsApi = CSPSettings::NewLC();
    CSPProperty* property = CSPProperty::NewLC();

    settingsApi->FindPropertyL( aServiceId, EServiceName, *property );
    User::LeaveIfError( property->GetValue( aServiceName ) );
    
    CleanupStack::PopAndDestroy( 2, settingsApi );
    }

//  End of File
