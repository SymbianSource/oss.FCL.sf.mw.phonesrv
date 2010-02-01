/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The engine class of operator info plugin. Handles the
*                communication with Publish&Subscribe data.
*
*/


// INCLUDE FILES
#include    <AknIconArray.h>                // For itemsicon
#include    <gulicon.h>                     // For itemsicon
#include    <aknlayout.cdl.h>               // For TAknWindowLineLayout
#include    <aknlayoutscalable_avkon.cdl.h> // For scalable layouts
#include    <AknUtils.h>
#include    <AknIconUtils.h>

#include    "caisatengine.h"
#include    "caisatnotifier.h"
#include    "mpluginapi.h"
#include    "tflogger.h" 

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
CAiSatEngine::CAiSatEngine( MPluginAPI& aPlugin ) :
    iPlugin ( aPlugin ),
    iIcon( NULL ),
    iPreviousIconId( 0 )
    {
    }
    
// -----------------------------------------------------------------------------
// Two-phased constructor.
// Create instance of concrete ECOM interface implementation
// -----------------------------------------------------------------------------
CAiSatEngine* CAiSatEngine::NewL( MPluginAPI& aPlugin )
    {
    TFLOGSTRING( "CAiSatEngine::NewL called" )
    
    CAiSatEngine* self = new( ELeave )CAiSatEngine( aPlugin );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    TFLOGSTRING( "CAiSatEngine::NewL exits" )
    return self;
    }
    
// -----------------------------------------------------------------------------
// Destructor.
// Closes the connection to the P&S and deletes
// all the dynamic content.
// -----------------------------------------------------------------------------
//
CAiSatEngine::~CAiSatEngine()
    {
    TFLOGSTRING( "CAiSatEngine::~CAiSatEngine called" )

    delete iNotifier;
    
    // Delete icon and null the pointer.
    delete iIcon;
    iIcon = NULL;
           
    iSatService.Close();
    iSatSession.Close();
    
    TFLOGSTRING( "CAiSatEngine::~CAiSatEngine exits" )  
    }
    
// -----------------------------------------------------------------------------
// Offers information about the content
// Returns ETrue if there is content available, 
// EFalse if not.
// -----------------------------------------------------------------------------
//
TBool CAiSatEngine::HasContent()
    {
    TFLOGSTRING( "CAiSatEngine::HasContent called - exits" )
    return iNotifier->HasContent();
    }

// -----------------------------------------------------------------------------
// Retrieves the operator info text from RSatService
// -----------------------------------------------------------------------------
//
void CAiSatEngine::ContentText( TPtrC& aString )
    {
    TFLOGSTRING( "CAiSatEngine::ContentText called" )

    iNotifier->GetIdleTextString( aString );
    
    TFLOGSTRING( "CAiSatEngine::ContentText exits" )
    }
    
// -----------------------------------------------------------------------------
// Offers the Idle Mode icon
// -----------------------------------------------------------------------------
//
void CAiSatEngine::ContentIconL( CGulIcon*& aGulIcon )
    {
    TFLOGSTRING( "CAiSatEngine::ContentIconL called" )
            
    // Active Idle framework will release the bitmap we would
    // publish. So a new bitmap should be created to avoid
    // destroying the bitmap owned by UAA.
    if ( iIcon )
        {
        // The bitmap for publishing.
        CFbsBitmap* bitmapToPublish( NULL );    
                  
        bitmapToPublish = new (ELeave) CFbsBitmap;
        CleanupStack::PushL( bitmapToPublish );

        TSize size = iIcon->SizeInPixels();
        TInt error = bitmapToPublish->Create( size, iIcon->DisplayMode() ); 
         
        // Duplicate the bitmap owned by UAA into the new bitmap.
        if ( !error )
            {
            error = DuplicateBitmap( bitmapToPublish, iIcon );        
            if ( !error )
                {
                // Create a icon from the bitmap and publish it.
                aGulIcon = CGulIcon::NewL( bitmapToPublish );
                CleanupStack::Pop( bitmapToPublish );        
                TFLOGSTRING( "CAiSatEngine::ContentIconL create GulIcon" )                
                }
            } 
        if ( error )  
            {
            CleanupStack::PopAndDestroy( bitmapToPublish );
            aGulIcon = NULL;
            }        
        }   
    TFLOGSTRING( "CAiSatEngine::ContentIconL exits" )    
    }

//------------------------------------------------------------------------------
// Loads icon provided by SIM from RSatIcon
//------------------------------------------------------------------------------
CFbsBitmap* CAiSatEngine::LoadIconL( TUint8 aIconId, 
    RSatService::TSatIconQualifier aIconQualifier  )
    {
    TFLOGSTRING( "CAiSatEngine::LoadIconL called" )
    CFbsBitmap* simIconBuffer( NULL ); 
    
    // If icon allready has been got from SIM (via satIcon and SATServer)
    // Because mechanism for fetching icon is very slow, it has been 
    // stored in class variable to make reasonable speed for 
    // Active Idle refreshing.
    if ( RSatService::ESatIconSelfExplanatory == aIconQualifier ||
         RSatService::ESatIconNotSelfExplanatory == aIconQualifier )
        {
        TFLOGSTRING( "CAiSatEngine::LoadIconL having icon" )
        RIconEf iconEf;
        CleanupClosePushL( iconEf );
    
        // Get icon info
        iSatService.GetIconInfoL( aIconId, iconEf );
        // Get the an available icon from info array.
        TInt infoCount( iconEf.Count() );
        TInt error( KErrNotFound );
        for ( TInt i = 0; i < infoCount && KErrNone != error; i++ )
            {
            TRAP( error, simIconBuffer = iSatService.GetIconL( iconEf[i] ) );
            }
            
        // Check whether the for loop ends without icon gotton.
        if ( error != KErrNone )
            {
            TFLOGSTRING2( "CAiSatEngine::LoadIconL: error=%d",error )
            simIconBuffer = NULL;
            }
            
        CleanupStack::PopAndDestroy( &iconEf );
        } 
    TFLOGSTRING( "CAiSatEngine::LoadIconL exit" )
    return simIconBuffer;
    }
    
// ---------------------------------------------------------------------------
// Formulates response to SatIcon API
// ---------------------------------------------------------------------------
//
void CAiSatEngine::HandleNotifyL()
    {
    TFLOGSTRING( "CAiSatEngine::HandleNotifyL called" )
    TUint8 id( iNotifier->IdleIconId() );
    RSatService::TSatIconQualifier iconQualifier( 
        iNotifier->IdleIconQualifier() );

    if ( !HasContent() && RSatService::ESatIconNoIcon == iconQualifier )
        {
        // Removal of text = No text and no icon
        TFLOGSTRING( "CAiSatNotifier::HasContent ETrue remove text success" )
        SendSatResponseL( RSatService::ESATIdleSuccess );
        }
    else
        {
        TFLOGSTRING2( "CAiSatEngine::HandleNotifyL Icon: %d", iconQualifier )
        if ( RSatService::ESatIconNoIcon == iconQualifier)
            {
            TFLOGSTRING( "CAiSatEngine::HandleNotifyL ESatIconNoIcon" )
            SendSatResponseL( RSatService::ESATIdleSuccess );
            }        
        else if ( RSatService::ESatIconSelfExplanatory == iconQualifier ||
                  RSatService::ESatIconNotSelfExplanatory == iconQualifier )
            {
            SendSatResponseL( RSatService::ESATIdleSuccess );
            TFLOGSTRING( "CAiSatEngine::HandleNotify icon & success resp" )
            }
        else if ( RSatService::ESatIconNotSelfExplanatory != iconQualifier &&
                  RSatService::ESatIconNoIcon != iconQualifier )
            {
            SendSatResponseL( RSatService::ESATIdleSuccessRequestedIconNotDisplayed );
            TFLOGSTRING( "CAiSatEngine: KSATIdleSuccessRequestedIconNotDisplayed" )
            }
        else
            {
            TFLOGSTRING( "CAiSatEngine::HandleNotifyL others" )
            // Report error
            SendSatResponseL( RSatService::ESATIdleMeUnableToProcessCmd );
            }            
        }

    TFLOGSTRING( "CAiSatEngine::HandleNotify exit" )
    }
       
// ---------------------------------------------------------------------------
// Get Idle mode data first, and load icon for publishing
// ---------------------------------------------------------------------------
//       
void CAiSatEngine::PrepareIdleModeDataL( TBool& aDupIcon, TBool& aDupText )
    {
    TFLOGSTRING( "CAiSatEngine::PrepareGetIdleModeDataL called" )
    
    // Get icon Id, icon qualifier and text.
    iNotifier->GetIdleModeDataL( aDupIcon, aDupText );
    
    // Load icon, and check whether it is the same with previous one.
    // if not, Load icon, otherwise, do nothing.
    TUint8 id ( iNotifier->IdleIconId() );   
    RSatService::TSatIconQualifier iconQualifier( 
        iNotifier->IdleIconQualifier() );
        
    if ( !( RSatService::ESatIconNoIcon == iconQualifier ) ||
         !( id == iPreviousIconId && iIcon ) ) 
        {
        TFLOGSTRING( "CAiSatEngine::PrepareIdleModeDataL load icon" )
        delete iIcon;
        iIcon = NULL;
        
        // The bitmap is fetched from UAA.
        iIcon = LoadIconL( id, iconQualifier ); 
        iPreviousIconId = id;  
        }
    TFLOGSTRING( "CAiSatEngine::PrepareGetIdleModeDataL exits" )
    }
    
// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave
// ---------------------------------------------------------------------------
//
void CAiSatEngine::ConstructL()
    {
    TFLOGSTRING( "CAiSatEngine::ConstructL called" )
    
    iSatSession.ConnectL();    
    TFLOGSTRING( "CAiSatEngine::ConstructL connect sat session" )
    
    iSatService.OpenL( iSatSession );
    TFLOGSTRING( "CAiSatEngine::ConstructL open sat service" )    
    
    iNotifier = CAiSatNotifier::NewL( iPlugin, iSatService );   
    User::LeaveIfError( iNotifier->Start() );
    
    TFLOGSTRING( "CAiSatEngine::ConstructL exits" )
    }
    
// ---------------------------------------------------------------------------
// Sends SAT response to RSatIcon
// ---------------------------------------------------------------------------
//
void CAiSatEngine::SendSatResponseL(
    const RSatService::TSATIdleResult& aResponse ) const
    {
    TFLOGSTRING( "CAiSatEngine::SendSatResponseL called")

    iSatService.SetIdleModeTextResponse( aResponse );

    TFLOGSTRING( "CAiSatEngine::SendSatResponseL exits" )
    }
      
// ---------------------------------------------------------------------------
// Duplicate a bitmap by copying memory.
// ---------------------------------------------------------------------------
//
TInt CAiSatEngine::DuplicateBitmap( const CFbsBitmap* aDestBmp,
                                    const CFbsBitmap* aSrcBmp )
    {
    TFLOGSTRING( "CAiSatPlugin::DuplicateBitmap() called" )

    // Get size and scan line length of the source bitmap.
    TSize size = aSrcBmp->SizeInPixels();
    TInt scanLineLength = CFbsBitmap::ScanLineLength( 
                            size.iWidth, aSrcBmp->DisplayMode() );
    TInt error( KErrNone );
    // Copy the data area of the source bimap to the dest bitmap.
    if ( aSrcBmp && aSrcBmp->DataAddress() )
        {
        TAny* bitmapData( NULL );
        
        bitmapData = memcpy(
            (TAny*)aDestBmp->DataAddress(), 
            (TAny*)aSrcBmp->DataAddress(), 
            scanLineLength * size.iHeight );
        
        if ( NULL == bitmapData )
            {
            TFLOGSTRING( "CAiSatPlugin::DuplicateBitmap() \
                Failed to copy memory data of bitmap." )
            error = KErrBadHandle;
            }
        }

    TFLOGSTRING( "CAiSatPlugin::DuplicateBitmap() exits" )

    return error;
    }  
