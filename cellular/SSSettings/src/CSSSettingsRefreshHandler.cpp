/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The Refresh handler to inform SAT refresses.
*
*/



// INCLUDE FILES
#include    <centralrepository.h>
#include    <RSatRefresh.h>
#include    <BTSapDomainPSKeys.h>     // BT SAP Central Repository key.
#include    <featmgr.h>

#include    "CSSSettingsRefreshHandler.h"
#include    "SSSettingsLogger.h"
#include    "CSSSettingsActiveObject.h"

// ============================= LOCAL FUNCTIONS ===============================


// CLASS DECLARATION

/**
*  The Refresh notify observer info container.
*
*  @lib SsSettings.lib
*  @since 2.6
*/
class CSSSettingsNotifyInfo
:public CBase
    {
    public: // Data

        // Refresh observer.
        MSSSettingsRefreshObserver* iObserver;

        // Observed file.
        TSatElementaryFiles    iObservedFile;

        // Observed refresh type.
        TSatRefreshType      iObservedRefreshType;
    };



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::CSSSettingsRefreshHandler
// -----------------------------------------------------------------------------
//
CSSSettingsRefreshHandler::CSSSettingsRefreshHandler()
    {
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::ConstructL
// -----------------------------------------------------------------------------
//
void CSSSettingsRefreshHandler::ConstructL()
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::ConstructL");
    iNotifyInfo = new ( ELeave ) CSSSettingsNotifyInfo;

    User::LeaveIfError ( iRSSSettings.Open() );

    User::LeaveIfError ( iRCspCache.Open() );

    iRSatSession.ConnectL();

    iRSatRefresh = new ( ELeave ) RSatRefresh ( *this);
    iRSatRefresh->OpenL( iRSatSession );
    iRSatRefresh->NotifyFileChangeL();
    
    if ( FeatureManager::FeatureSupported ( KFeatureIdBtSap ) )
        {
        //create the active object instance
        iBtSapNotifier = CSSSettingsActiveObject::NewL(
            KPSUidBluetoothSapConnectionState,
            KBTSapConnectionState,
            *this );

        //subscribe for key event notifications
        iBtSapNotifier->NotifyKeyChange();        
        }        
    __SSSLOGSTRING("[SSS] <--CSSSettingsRefreshHandler::ConstructL");
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::NewL
// -----------------------------------------------------------------------------
//
CSSSettingsRefreshHandler* CSSSettingsRefreshHandler::NewL()
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::NewL");
    CSSSettingsRefreshHandler* self = new( ELeave ) CSSSettingsRefreshHandler;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    __SSSLOGSTRING("[SSS] <--CSSSettingsRefreshHandler::NewL");
    return self;
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::~CSSSettingsRefreshHandler
// -----------------------------------------------------------------------------
//
CSSSettingsRefreshHandler::~CSSSettingsRefreshHandler()
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::~CSSSettingsRefreshHandler");
    if ( iRSatRefresh )
        {
        iRSatRefresh->Cancel();
        iRSatRefresh->Close();
        delete iRSatRefresh;
        iRSatRefresh = NULL;
        }
    iRSatSession.Close();
    iRSSSettings.Close();

    delete iNotifyInfo;
    iNotifyInfo = NULL;

    if ( FeatureManager::FeatureSupported ( KFeatureIdBtSap ) )
        {
        if( iBtSapNotifier )
            {
            iBtSapNotifier->CancelNotify();
            }
        delete iBtSapNotifier;
        } 
        
    iRCspCache.Close();
               
    __SSSLOGSTRING("[SSS] <--CSSSettingsRefreshHandler::~CSSSettingsRefreshHandler");
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::NotifyFileChangeL
// -----------------------------------------------------------------------------
//
void CSSSettingsRefreshHandler::NotifyFileChangeL(
    MSSSettingsRefreshObserver& aObserver,
    TSatElementaryFiles aObservedFiles,
    TSatRefreshType aObservedRefreshType )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::NotifyFileChangeL");
    // Update variables.
    iNotifyInfo->iObserver = &aObserver;
    iNotifyInfo->iObservedFile = aObservedFiles;
    iNotifyInfo->iObservedRefreshType = aObservedRefreshType;
    __SSSLOGSTRING("[SSS] <--CSSSettingsRefreshHandler::NotifyFileChangeL");
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::CancelNotify
// -----------------------------------------------------------------------------
//
void CSSSettingsRefreshHandler::CancelNotify()
    {
    iNotifyInfo->iObserver = NULL;    
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::ChangedCspTable
// -----------------------------------------------------------------------------
//
TInt CSSSettingsRefreshHandler::ChangedCspTable( 
    TDes8& aChangedCsps,
    TDes8& aNewValues )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::ChangedCspTable");
    return iRCspCache.ChangedCspTable( 
        aChangedCsps,
        aNewValues );    
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::AllowRefresh
// -----------------------------------------------------------------------------
//
TBool CSSSettingsRefreshHandler::AllowRefresh(
    TSatRefreshType aType,
    const TSatRefreshFiles& aFiles )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::AllowRefresh");
    return DoHandleRefresh( aType, aFiles, ESSSettingsAllowRefresh );
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::Refresh
// -----------------------------------------------------------------------------
//
void CSSSettingsRefreshHandler::Refresh(
    TSatRefreshType aType,
    const TSatRefreshFiles& aFiles )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::Refresh");
    DoHandleRefresh(  aType, aFiles, ESSSettingsRefresh );
    __SSSLOGSTRING("[SSS] <--CSSSettingsRefreshHandler::Refresh");
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::DoHandleRefresh
// -----------------------------------------------------------------------------
//
TBool CSSSettingsRefreshHandler::DoHandleRefresh(
    TSatRefreshType aType,
    const TSatRefreshFiles& aFiles,
    const TSSSettingsRefreshFunction aFunctionality )
    {    
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::DoHandleRefresh");
    if ( aFunctionality == ESSSettingsRefresh )
        {
        // Refresh handling needed only if the refresh has happened.
        iRCspCache.HandleRefresh();
        iRSSSettings.HandleRefresh();
        }

    // By default refresh is allowed.
    TBool allow(ETrue);

    // Handle only if there is observer.
    if ( iNotifyInfo->iObserver )
        {
        // Handle only if observer wants to listen this event.
        if ( iNotifyInfo->iObservedRefreshType & aType )
            {
            TInt changedFiles(0);     
            // Only change notification will contain file list.
            if ( aType == EFileChangeNotification )
                {              
                if ( aFiles.Locate( KCspEf ) != KErrNotFound )
                    {
                    changedFiles += KCspEf;
                    }
                // Check if the changed files is what is wanted to be listened.
                changedFiles = changedFiles & iNotifyInfo->iObservedFile;
                }

            // For change notification the filelist must contain files, for
            // other refresh types it is empty.
            if ( ( aType != EFileChangeNotification ) ||
                 ( ( aType == EFileChangeNotification ) && 
                   changedFiles ) )
                {
                if ( aFunctionality == ESSSettingsRefresh )
                    {
                    // Inform the observer about the refresh event.
                    iNotifyInfo->iObserver->Refresh(
                        aType,
                        ( TSatElementaryFiles ) changedFiles  );
                        
                    if ( aFiles.Locate( KCsp1Ef ) != KErrNotFound )
	                    {
	                    iRSatRefresh->RefreshEFRead( ETrue );
	                    }
	                else if( aFiles.Locate( KCsp2Ef ) != KErrNotFound )
	                	{
	                	iRSatRefresh->RefreshEFRead( ETrue );
	                	}
	                else
	                	{
	                	iRSatRefresh->RefreshEFRead( EFalse );
	                	}
                    }
                else // ESSSettingsAllowRefresh
                    {
                    // Inform the observer about the coming refresh event.
                    allow = iNotifyInfo->iObserver->AllowRefresh(
                        aType,
                        ( TSatElementaryFiles ) changedFiles );
                    }
                }
            
            // If there is no client listening changed files SSSettings has to
            // check if some CSP file is into the list and send right response
            // since itself reads CSP files.    
            if ( !changedFiles )
            	{
            	if ( aFiles.Locate( KCsp1Ef ) != KErrNotFound )
                    {
                    iRSatRefresh->RefreshEFRead( EFalse );
                    }
                else if( aFiles.Locate( KCsp2Ef ) != KErrNotFound )
                	{
                	iRSatRefresh->RefreshEFRead( EFalse );
                	}
                else
                	{
                	iRSatRefresh->RefreshEFRead( ETrue );
                	}
            	}
            __SSSLOGSTRING1("[SSS]    DoHandleRefresh: changedFiles: %d", changedFiles);
            }
        else
            {
            iRSatRefresh->RefreshEFRead( EFalse );    
            }
        }

    if ( !iNotifyInfo->iObserver && ( aType == EFileChangeNotification 
    	|| aType == ESimInitFileChangeNotification 
    	|| aType == ESimInit ) )
    	{
        if ( aFiles.Locate( KCsp1Ef ) != KErrNotFound )
        	{
            iRSatRefresh->RefreshEFRead( EFalse );
            }
        else if( aFiles.Locate( KCsp2Ef ) != KErrNotFound )
          	{
           	iRSatRefresh->RefreshEFRead( EFalse );
          	}           
        else 
            {
            iRSatRefresh->RefreshEFRead( ETrue );
            }
    	}	
    	    	
    __SSSLOGSTRING("[SSS] <--CSSSettingsRefreshHandler::DoHandleRefresh");
    return allow;
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshHandler::HandlePubSubNotify
// -----------------------------------------------------------------------------
//
void CSSSettingsRefreshHandler::HandlePubSubNotify( const TUid aUid,
    const TUint32 aKeyId )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsRefreshHandler::HandlePubSubNotify");
    if ( aUid == KPSUidBluetoothSapConnectionState && 
         aKeyId == KBTSapConnectionState )
        {
        TInt btSapState(EBTSapNotConnected);
        //get the current SAP state
        RProperty::Get( aUid, aKeyId, btSapState );
        if ( btSapState == EBTSapNotConnected )
            {
            // Refresh handling needed only if the BT SAP 
            // state is not connected.
            iRCspCache.HandleRefresh();
            iRSSSettings.HandleRefresh();
            }
        __SSSLOGSTRING1("[SSS]    HandlePubSubNotify: btSapState: %d", btSapState);
        }
    __SSSLOGSTRING("[SSS] <--CSSSettingsRefreshHandler::HandlePubSubNotify");
    }


//  End of File  
