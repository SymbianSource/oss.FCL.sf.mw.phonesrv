/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The handler for the SAT Plug-in
*                Set Up Idle Mode Text proactive command.
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <etelsat.h>
#include <RSatSession.h>
#include <rsatservice.h>

#include "caisatnotifier.h"
#include "mpluginapi.h" // Needed in cpp and header
#include "tflogger.h"

// ============================ MEMBER FUNCTIONS =============================
// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave
// ---------------------------------------------------------------------------
//
CAiSatNotifier* CAiSatNotifier::NewL( MPluginAPI& aPlugin, 
                                      RSatService& aSatService )
    {
    TFLOGSTRING( "CAiSatNotifier::NewL calling" )
    CAiSatNotifier* self = new ( ELeave ) CAiSatNotifier( aPlugin,
                                                          aSatService );

    TFLOGSTRING( "CAiSatNotifier::NewL exits" )
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// Cancels  and deletes
// all the dynamic content
// ---------------------------------------------------------------------------
//
CAiSatNotifier::~CAiSatNotifier()
    {
    TFLOGSTRING( "CAiSatNotifier::~CAiSatNotifier calling" )
    
    // Cancel any outstanding requests
    Cancel();
    delete iString;

    TFLOGSTRING( "CAiSatNotifier::~CAiSatNotifier exits" )
    }

// ---------------------------------------------------------------------------
// Starts the handler
// ---------------------------------------------------------------------------
//
TInt CAiSatNotifier::Start()
    {
    TFLOGSTRING( "CAiSatNotifier::Start calling" )
    
    TInt error( KErrNotReady );
    if ( !IsActive() )
        {
        error = iSatService.NotifySetupIdleModeTextChange( iStatus ); 
        TFLOGSTRING2( "CAiSatNotifier::Start error %i", error )
        if ( KErrNone == error )
            {
            // Set to active so that requests can be received    
            SetActive();       
            }        
        }
    
    TFLOGSTRING( "CAiSatNotifier::Start exits" )
    return error;
    }

// ---------------------------------------------------------------------------
// Get the idle mode data to member: string, icon qualifier, icon id.
// ---------------------------------------------------------------------------
//
TBool CAiSatNotifier::HasContent()
    {
    TFLOGSTRING( "CAiSatNotifier::HasContent calling" )
    TBool ret( EFalse );
    if ( ( iString && iString->Length() ) ||
           RSatService::ESatIconNoIcon != iIconQualifier )
        {
        TFLOGSTRING( "CAiSatNotifier::HasContent ETrue" )
        ret = ETrue;
        }
    TFLOGSTRING( "CAiSatNotifier::HasContent exits" )
    return ret;
    }
    
// ---------------------------------------------------------------------------
// Get the idle mode data to member: string, icon qualifier, icon id.
// ---------------------------------------------------------------------------
//
void CAiSatNotifier::GetIdleModeDataL( TBool& aDupIcon, TBool& aDupText )
    {
    TFLOGSTRING( "CAiSatNotifier::GetIdleModeData calling" )
    HBufC* string( NULL );
    TUint8 recordId( NULL );
    RSatService::TSatIconQualifier iconQualifier( 
        RSatService::ESatIconNoIcon );
    
    // Get setup idle mode text, icon qualifier and record id.
    iSatService.GetSetupIdleModeTextL( string, iconQualifier, recordId ); 
            
    // If the icon is self explanatory, NULL the string.
    if ( string && RSatService::ESatIconSelfExplanatory == iconQualifier )
        {
        TPtr ptr( string->Des() );
        ptr.Copy( KNullDesC );
        }       
    
    // Check if the icon or text is the same as previous one.
    if ( iFirstStartup )    
        {
        aDupIcon = EFalse;
        aDupText = EFalse;
        iFirstStartup = EFalse;
        }
    else
        {
        // Check if setup idle mode icon is changed.
        aDupIcon = ( iRecordId == recordId );   
        
        // Check if setup idle mode text is changed.
        // If both of the iString and string are NULL, it is duplicated.
        // If both of the iString and string are not NULL, and they have
        // the same content, it is duplicated.
        // Otherwise, it is not duplicated.    
            
        aDupText = ( ( ( !iString || !iString->Length() ) && 
                       ( !string || !string->Length() ) ) ||
                     ( iString && string && 
                       *iString == string->Des() ) );        
        }     

    // Save the setup idle mode text, icon qulifier 
    // and record id to member params.
    delete iString;
    iString = NULL; 
    if ( string )
        {
        CleanupStack::PushL( string );
        iString = HBufC::NewL( string->Size() );
        TPtr ptr( iString->Des() );
        ptr.Copy( string->Des() );  
        CleanupStack::PopAndDestroy( string );        
        }
        
    iIconQualifier = iconQualifier;
    
    iRecordId = recordId;
    
    TFLOGSTRING( "CAiSatNotifier::GetIdleModeData exits" ) 
    }
    
// ---------------------------------------------------------------------------
// Handles the Idle Text string command
// ---------------------------------------------------------------------------
//
void CAiSatNotifier::GetIdleTextString( TPtrC& aString )
    {
    TFLOGSTRING( "CAiSatNotifier::GetIdleTextString calling" )

    if ( iString )
        {
        TFLOGSTRING2( "CAiSatNotifier::IdleTextStringL exits, \
                      length of string: %i", iString->Length() )
        aString.Set( iString->Des() );
        } 
    else
        {
        aString.Set( KNullDesC );
        } 
    TFLOGSTRING( "CAiSatNotifier::GetIdleTextString exits" )                             
    }
   

// ---------------------------------------------------------------------------
// Handles the Idle Icon command.
// ---------------------------------------------------------------------------
//
TUint8 CAiSatNotifier::IdleIconId()
    {
    TFLOGSTRING( "CAiSatNotifier::IdleIconId calling" ) 
       
    TFLOGSTRING2( "CAiSatNotifier::IdleIconId exits, id: %i", iRecordId )  
    
    return iRecordId;
    }

// ---------------------------------------------------------------------------
// Handles the Idle Icon Qulifier command.
// ---------------------------------------------------------------------------
//
RSatService::TSatIconQualifier CAiSatNotifier::IdleIconQualifier()
    {
    TFLOGSTRING( "CAiSatNotifier::IdleIconQualifier calling" )  
        
    TFLOGSTRING2( "CAiSatNotifier::IdleIconQualifier exits, \
        iconQualifier: %d", iIconQualifier )
             
    return iIconQualifier;
    }
    
// ---------------------------------------------------------------------------
// Handles the Plugin's NotifyContentUpdate command.
// ---------------------------------------------------------------------------
//
void CAiSatNotifier::RunL()
    {
    TFLOGSTRING( "CAiSatNotifier::RunL calling" )

    // Notify the plugin about the event through the MPluginAPI interface
    if ( KErrNone == iStatus.Int() )
        {              
        // Notify plugin when the content update.
        // The plugin should get data again.
        iPlugin.NotifyContentUpdate();       
        }
    if ( !IsActive() )
        {
        Activate();
        }           
    TFLOGSTRING( "CAiSatNotifier::RunL exits" )

    }

// ---------------------------------------------------------------------------
// Cancels the pending request
// ---------------------------------------------------------------------------
//
void CAiSatNotifier::DoCancel()
    {
    TFLOGSTRING( "CAiSatNotifier::DoCancel calling" )

    iSatService.NotifySetupIdleModeTextChangeCancel();  

    TFLOGSTRING( "CAiSatNotifier::DoCancel exits" )
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CAiSatNotifier::CAiSatNotifier( MPluginAPI& aPlugin,  
                                RSatService& aSatService ):
    CActive( EPriorityStandard ),
    iString( NULL ),
    iRecordId( 0 ),
    iIconQualifier( RSatService::ESatIconNoIcon ),
    iPlugin( aPlugin ),
    iSatService( aSatService ),
    iFirstStartup( ETrue )
    {
    TFLOGSTRING( "CAiSatNotifier::CAiSatNotifier calling" )
    
    // Add to active scheduler
    CActiveScheduler::Add( this );

    TFLOGSTRING( "CAiSatNotifier::CAiSatNotifier exits" )
    }

// ---------------------------------------------------------------------------
// Continues listening to the Sat command
// ---------------------------------------------------------------------------
//
void CAiSatNotifier::Activate()
    {
    TFLOGSTRING( "CAiSatNotifier:Activate() calling" )
    
    Cancel();   
    TInt error = iSatService.NotifySetupIdleModeTextChange( iStatus );
    TFLOGSTRING2( "CAiSatNotifier::Activate error: %i", error )
    if ( KErrNone == error )
		{
    	SetActive();
		}
    TFLOGSTRING( "CAiSatNotifier:Activate() exits" )
    }
