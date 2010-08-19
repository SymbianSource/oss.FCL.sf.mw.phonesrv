/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SAT Active Idle publisher
*
*/


#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>
#include <aicontentobserver.h>
#include <aiutility.h>
#include <aipluginsettings.h>
#include <flogger.h>
#include <AknUtils.h>
#include <gulicon.h>            // For CGulIcon
#include <fbs.h>                // For CFbsBitmap
#include <SATInternalPSKeys.h>

#include "aisatcontentmodel.h"
#include "caisatplugin.h"
#include "caisatengine.h"
#include "tflogger.h"

const TImplementationProxy KImplementationTable[] =
    {
    //lint -e{611,1924} Warning "Suspicious cast" can not be avoided.
    IMPLEMENTATION_PROXY_ENTRY( KImplUidSatPlugin, CAiSatPlugin::NewL )
    };
    
// static cleanup function
static void TransactionCleanup( TAny* aAny )
    {
    static_cast<MAiContentObserver*>( aAny )->
        CancelTransaction( KImplUidSatPlugin );
    }

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave
// ---------------------------------------------------------------------------
//
CAiSatPlugin* CAiSatPlugin::NewL()
    {
    TFLOGSTRING( "CAiSatPlugin::NewL() starts" )

    CAiSatPlugin* self = new ( ELeave ) CAiSatPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    TFLOGSTRING( "CAiSatPlugin::NewL() exits" )
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// Deletes all data created to heap
// ---------------------------------------------------------------------------
//
CAiSatPlugin::~CAiSatPlugin()
    {
    TFLOGSTRING( "CAiSatPlugin::~CAiSatPlugin() starts" )

    Release( iContent );
    iContent = NULL;
    
    Release( iResources );
    iResources = NULL;
    
    Release( iEvents );
    iEvents = NULL;

    delete iEngine;
    
    iObservers.Close();

    TFLOGSTRING( "CAiSatPlugin::~CAiSatPlugin() exits" )
    }

// ---------------------------------------------------------------------------
// Publish a icon to the idle area.
// ---------------------------------------------------------------------------
//
TInt CAiSatPlugin::PublishIcon(
    MAiContentObserver& aObserver,
    CGulIcon* aIcon )
    {
    TFLOGSTRING( "CAiSatPlugin::PublishIcon() starts" )
    
    TInt error( KErrNotSupported );
    
    // Check whether the icon can be published or not.
    if ( aIcon && aObserver.CanPublish( *this, KSatContent[ESatContentIcon].id,
                                        ESatContentIcon ) )
        {
        // If the icon can be published, and then publish it.        
        error = aObserver.Publish( *this, KSatContent[ESatContentIcon].id,
                                    TPckgC<TAny*>( aIcon ), ESatContentIcon );
        }

    TFLOGSTRING2( "CAiSatPlugin::PublishIcon() exits, error: %d", error )
    
    return error;
    }

// ---------------------------------------------------------------------------
// Clean idle area icon
// ---------------------------------------------------------------------------
// 
TInt CAiSatPlugin::CleanIcon( MAiContentObserver& aObserver )
    {
    TFLOGSTRING( "CAiSatPlugin::CleanIcon() starts" )
    TInt error( KErrNotSupported );

    error = aObserver.Clean( *this, KSatContent[ESatContentIcon].id,
                             ESatContentIcon );    
                             
    TFLOGSTRING2( "CAiSatPlugin::CleanIcon() exits, error %d", error )    
    return error;                        
    }
    
// ---------------------------------------------------------------------------
// Publish a SetUpIdleModeText string.
// ---------------------------------------------------------------------------
//
TInt CAiSatPlugin::PublishText( MAiContentObserver& aObserver,
                                const TDesC& aText )
    {
    TFLOGSTRING( "CAiSatPlugin::PublishText() starts" )
    TFLOGSTRING2( "CAiSatPlugin::PublishText() length %i", aText.Length() )
    
    TInt error( KErrNotSupported );
    
    // Check whether the SetUpIdleModeText string can be published or not
    if ( aText.Length() && 
         aObserver.CanPublish( *this, KSatContent[ESatContentText].id,
                               ESatContentText ) )
        {
        // If SetUpIdleModeText can be published, and then publish it.
        error = aObserver.Publish( *this, KSatContent[ESatContentText].id,
                                   aText, ESatContentText );
        }        
    TFLOGSTRING2( "CAiSatPlugin::PublishText() exits, error: \
        %d", error )

    return error;
    }
 
// ---------------------------------------------------------------------------
// Clean idle area text
// ---------------------------------------------------------------------------
// 
TInt CAiSatPlugin::CleanText( MAiContentObserver& aObserver )
    {
    TFLOGSTRING( "CAiSatPlugin::CleanText() starts" )
    TInt error( KErrNotSupported );

    error = aObserver.Clean( *this, KSatContent[ESatContentText].id,
                             ESatContentText );  
                             
    TFLOGSTRING2( "CAiSatPlugin::CleanText() exits, error %d", error )   
    
    return error;                          
    }
    
// ---------------------------------------------------------------------------
// Publishes the SAT Set Up Idle Mode Text and or Icon
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::PublishSatL()
    {
    TFLOGSTRING( "CAiSatPlugin::PublishSatL() starts" )
    
    TInt error( KErrNone );
    TInt iconError( KErrNone );
    TInt textError( KErrNone );
    TInt observers( iObservers.Count() );
    TInt transactionId( KImplUidSatPlugin );
    MAiContentObserver* observer( NULL );
    CGulIcon* icon( NULL ); // The icon for publishing.
    TInt i( 0 );
    TBool commitNeeded( EFalse );
    
    // Publish sat data to all observers.
    TFLOGSTRING2( "CAiSatPlugin::PublishSatL() observer count: %i", observers )        
    for ( i = 0; i < observers; i++ )
        {
        commitNeeded = EFalse;
        observer = iObservers[i];
        TFLOGSTRING2( "CAiSatPlugin::PublishSatL() observer id: %i", observer )
        
        // Start publish transaciton.
        error = observer->StartTransaction( transactionId );
        TCleanupItem item( TransactionCleanup, observer );
        CleanupStack::PushL( item );

        // Publish icon and text to idle mode area.
        iEngine->ContentIconL( icon ); 
        TPtrC text( KNullDesC );
        iEngine->ContentText( text );  
        if ( !icon && !text.Length() ) // Nothing
            {
            if ( !iDupIcon ) 
                {
                error = CleanIcon( *observer ); 
                commitNeeded = ETrue;
                }
            if ( !iDupText )
                {
                error = CleanText( *observer );
                commitNeeded = ETrue;                
                }
            }
        else if ( icon && !text.Length() ) // Icon only
            {
            if ( !iDupIcon )                
                { 
                error = CleanIcon( *observer ); 
                iconError = PublishIcon( *observer, icon );
                commitNeeded = ETrue;
                }  
            if ( !iDupText )
                {
                error = CleanText( *observer );  
                commitNeeded = ETrue;
                }
            }            
        else if ( !icon && text.Length() ) // Text only
            {   
            if ( !iDupIcon ) 
                {
                error = CleanIcon( *observer );
                commitNeeded = ETrue;
                }
            if ( !iDupText )
                {
                error = CleanText( *observer );
                textError = PublishText( *observer, text );
                commitNeeded = ETrue;
                }
            }
        else // Text and icon
            {
            if ( !iDupIcon )                
                {
                error = CleanIcon( *observer );
                iconError = PublishIcon( *observer, icon );
                commitNeeded = ETrue;
                }            
            if ( !iDupText )
                {
                error = CleanText( *observer );
                textError = PublishText( *observer, text );
                commitNeeded = ETrue;
                }              
            }    
                     
        // Commit the publishing of the icon and text.
        if ( !textError && !iconError && commitNeeded )
            {
            TFLOGSTRING( "CAiSatPlugin::PublishSatL() commit transaction" )  
            error = observer->Commit( transactionId );
            }
        else
            {
            TFLOGSTRING( "CAiSatPlugin::PublishSatL() cancel transaction" )  
            error = observer->CancelTransaction( transactionId );
            }   
        CleanupStack::Pop();
        }

    TFLOGSTRING( "CAiSatPlugin::PublishSatL() exits" )
    }

// ---------------------------------------------------------------------------
// CAiSatPlugin::Start
// 
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::Start( TStartReason /*aReason*/ )
    {    
    TFLOGSTRING( "CAiSatPlugin::Start() starts" )
    
    iPublishRequired = ETrue;
    
    TFLOGSTRING( "CAiSatPlugin::Start() exits" )
    }

// ---------------------------------------------------------------------------
// CAiSatPlugin::Stop
// 
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::Stop( TStopReason /*aReason*/ )
    {
    TFLOGSTRING( "CAiSatPlugin::Stop() starts - exits" )
    }

// ---------------------------------------------------------------------------
// CAiSatPlugin::Resume
// 
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::Resume( TResumeReason aReason )
    {
    TFLOGSTRING2( "CAiSatPlugin::Resume() reason %d", aReason )

    if ( aReason == EForeground )
        {
        if ( iPublishRequired )
            {
            iPublishRequired = EFalse;
            
            TRAP_IGNORE( UpdateSatL() )                       
            }
        }
            
    TFLOGSTRING( "CAiSatPlugin::Resume() exits" )
    }

// ---------------------------------------------------------------------------
// CAiSatPlugin::Suspend
// 
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::Suspend( TSuspendReason aReason )
    {
    TFLOGSTRING( "CAiSatPlugin::Suspend() starts" )
    
    if ( aReason == EGeneralThemeChange )
        {
        iPublishRequired = ETrue;
        }
    
    TFLOGSTRING( "CAiSatPlugin::Suspend() exits" )
    }

// ---------------------------------------------------------------------------
// CAiSatPlugin::SubscribeL
// 
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::SubscribeL( MAiContentObserver& aObserver )
    {
    TFLOGSTRING( "CAiSatPlugin::SubscribeL() starts" )

    iObservers.AppendL( &aObserver );

    TFLOGSTRING( "CAiSatPlugin::SubscribeL() exits" )
    }

// ---------------------------------------------------------------------------
// CAiSatPlugin::ConfigureL
// 
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::ConfigureL( RAiSettingsItemArray& aSettings )
    {
    TFLOGSTRING( "CAiSatPlugin::ConfigureL() starts" )

    TInt count = aSettings.Count();
    TFLOGSTRING2( "CAiSatPlugin::ConfigureL  count: %d", count )
    if ( count > 0 )
        {
        TInt i( 0 );
        for ( i = 0; i < count; i++ )
            {
            MAiPluginSettings* settings = aSettings[i];
            MAiPluginSettingsItem& item = settings->AiPluginSettingsItem();
            item.SetValueL( item.Value(), EFalse );
            }
        TFLOGSTRING2( "CAiSatPlugin::ConfigureL  i: %d", i )    
        }
	
    TFLOGSTRING( "CAiSatPlugin::ConfigureL() exits" )
    }

// ---------------------------------------------------------------------------
// CAiSatPlugin::GetProperty
// 
// ---------------------------------------------------------------------------
//
TAny* CAiSatPlugin::GetProperty( TProperty aProperty )
    {
    TFLOGSTRING( "CAiSatPlugin::GetProperty() starts" )
    
    TAny* property( NULL );
    
    TFLOGSTRING2( "CAiSatPlugin::GetProperty aProperty: %d", aProperty )

    if ( aProperty == EPublisherContent )
        {            
        property = iContent;            
        }
    else if ( aProperty == EPublisherResources )
        {
        property = iResources;
        }
    else if ( aProperty == EPublisherEvents )
        {
        property = iEvents;
        }

    TFLOGSTRING( "CAiSatPlugin::GetProperty() exits" )

    return property;
    }

// ---------------------------------------------------------
// This method is called from the engine, when the P&S
// data content has been changed. Method call is made through
// the MPluginAPI interface.
// ---------------------------------------------------------
//
void CAiSatPlugin::NotifyContentUpdate()
    {
    TFLOGSTRING( "CAiSatPlugin::NotifyContentUpdate() starts" )
    
    if ( iEngine )
        {
        // Get the idle mode data first.
        TRAP_IGNORE( iEngine->PrepareIdleModeDataL( iDupIcon, iDupText ) );
        
        // Send response to client side
        TRAP_IGNORE( iEngine->HandleNotifyL() );   
                            
        // Publish set up idle mode data           
        TRAP_IGNORE( PublishSatL() );  
        }

    TFLOGSTRING( "CAiSatPlugin::NotifyContentUpdate() exits" )
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CAiSatPlugin::CAiSatPlugin()
    {
    TFLOGSTRING( "CAiSatPlugin::CAiSatPlugin() starts-exits" )
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::ConstructL()
    {
    TFLOGSTRING( "CAiSatPlugin::ConstructL() starts" )

    iContent = AiUtility::CreateContentItemArrayIteratorL( KSatContent );
    iResources = AiUtility::CreateContentItemArrayIteratorL( KSatResources );
    iEvents = AiUtility::CreateContentItemArrayIteratorL( KSatEvents );

    iEngine = CAiSatEngine::NewL( *this );

    TFLOGSTRING( "CAiSatPlugin::ConstructL() exits" )
    }

// ---------------------------------------------------------------------------
// Update idle area when plug in reload.
// ---------------------------------------------------------------------------
//
void CAiSatPlugin::UpdateSatL()
    {
    TFLOGSTRING( "CAiSatPlugin::UpdateSatL() starts" )
    iEngine->PrepareIdleModeDataL( iDupIcon, iDupText );
    PublishSatL();                
    TFLOGSTRING( "CAiSatPlugin::UpdateSatL() exits" )
    }

// ============================ GLOBAL FUNCTIONS =============================

// ---------------------------------------------------------------------------
// Constructs and returns an application object.
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount )
    {
    TFLOGSTRING( "CAiSatPlugin::ImplementationGroupProxy() starts-exits" )
    aTableCount =
        sizeof( KImplementationTable ) / sizeof( TImplementationProxy );

    return KImplementationTable;
    }