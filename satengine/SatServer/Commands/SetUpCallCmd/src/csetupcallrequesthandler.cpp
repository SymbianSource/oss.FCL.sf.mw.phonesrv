/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Create the call and emergency call to ETelMM
*
*/


#include <etelmm.h>
#include <mmtsy_names.h>


#include "csetupcallrequesthandler.h"
#include "CSetUpCallHandler.h"
#include "msatasynctosync.h"
#include "SatLog.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::CSetupCallRequestHandler
// The class constructor.
// -----------------------------------------------------------------------------
//
CSetupCallRequestHandler::CSetupCallRequestHandler(
                MSatMultiModeApi& aPhone, CSetUpCallHandler* aDispatcher )
    : CActive( EPriorityStandard ), iPhone( aPhone ), 
      iDispatcher( aDispatcher )
    {
    LOG( SIMPLE,
        "SETUPCALL: CSetupCallRequestHandler::CSetupCallRequestHandler \
        calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SETUPCALL: CSetupCallRequestHandler::CSetupCallRequestHandler \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetupCallRequestHandler* CSetupCallRequestHandler::NewL(
    MSatMultiModeApi& aPhone, CSetUpCallHandler* aDispatcher )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::NewL calling" )

    CSetupCallRequestHandler* self =
        new ( ELeave ) CSetupCallRequestHandler( aPhone, aDispatcher );
 
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::ConstructL()
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::ConstructL calling" )
    iStatusHandler = CSetupCallStatusHandler::NewL( iPhone, this );
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::ConstructL exiting" )    
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::~CSetupCallRequestHandler
// Destructor
// -----------------------------------------------------------------------------
//
CSetupCallRequestHandler::~CSetupCallRequestHandler()
    {
    LOG( SIMPLE, "SETUPCALL: \
            CSetupCallRequestHandler::~CSetupCallRequestHandler calling" )
    Cancel();
    iDispatcher = NULL;
    delete iStatusHandler;
    
    LOG( SIMPLE, "SETUPCALL: \
            CSetupCallRequestHandler::~CSetupCallRequestHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::DialNumber
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::DialNumber( const TDesC8& aCallParams,
                  const TDesC& aTelNumber, TBool aTerminateOtherCall,
                  MSatAsyncToSync* aAsyncToSync )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialNumber calling" )
 
    TInt terminateRes ( KErrNone );
    
    if( aTerminateOtherCall )
        {
        if( aAsyncToSync )
            {
            iPhone.TerminateAllCalls( aAsyncToSync->RequestStatus() );
            terminateRes = aAsyncToSync->SetActiveAndWait();
            }
        else
            {
            LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialNumber \
                    bad MSatAsyncToSync pointer" )
            terminateRes  = KErrGeneral;
            }
        }
    
    if( !IsActive() )
        {
        if( KErrNone == terminateRes )
            {
            iPhone.DialNoFdnCheck( iStatus, aCallParams, aTelNumber );
            if ( iStatusHandler )
                {
                iStatusHandler->Start();
                }
            SetActive();
            }
        else
            {
            iDispatcher->SetupCallRequestComplete( terminateRes );
            }
        }
    else
        {
        iDispatcher->SetupCallRequestComplete( KErrInUse );
        }
    
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialNumber exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::DialEmergencyCall
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::DialEmergencyCall( const TDesC& aTelNumber )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialNumber calling" )
     
    if( !IsActive() )
        {
        iEmergencyCall = ETrue;
        iPhone.DialEmergencyCall( iStatus, aTelNumber );
        SetActive();
        }
    else
        {
        iDispatcher->SetupCallRequestComplete( KErrInUse );
        }
    
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DialEmergencyCall exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::RunL()
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::RunL calling" )
    
    LOG2( NORMAL, "SETUPCALL: CSetupCallRequestHandler::RunL\
          iStatus == %i", iStatus.Int() )

    if( !iResponsed )
        {
        if ( iEmergencyCall || KErrNone == iStatus.Int() )
            {
            iEmergencyCall = EFalse;
            iDispatcher->SetupCallRequestComplete( iStatus.Int() );
            }
        else
            {
            RMobileCall::TMobileCallInfoV8 info;
            RMobileCall::TMobileCallInfoV8Pckg infoPkg( info );
            TInt res = iPhone.GetMobileCallInfo( infoPkg );
        
            LOG2( NORMAL, "SETUPCALL: CSetupCallRequestHandler::\
                  HandleSetupCallStatusChange exit code == %i",
                  info.iExitCode )
        
            if( ( KErrNone == res ) && 
                    (KErrNone != info.iExitCode) &&
                    (KErrNotFound != info.iExitCode) )
                {
                iDispatcher->SetupCallRequestComplete( info.iExitCode );
                }
            else
                {
                iDispatcher->SetupCallRequestComplete( iStatus.Int() );
                }
            }
        }
   
    if( iStatusHandler )
        {
        iStatusHandler->Cancel();
        }
    iResponsed = EFalse;
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::CancelOperation
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::CancelOperation()
    {
    LOG( SIMPLE, "SETUPCALL: \
                  CSetupCallRequestHandler::CancelOperation calling" )
    iPhone.DialCancel();
    if( iStatusHandler )
        {
        iStatusHandler->Cancel();
        }
    LOG( SIMPLE, 
        "SETUPCALL: CSetupCallRequestHandler::CancelOperation exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::DoCancel()
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DoCancel calling" )
    CancelOperation();
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallRequestHandler::HandleConnectingStatusChange
// -----------------------------------------------------------------------------
//
void CSetupCallRequestHandler::CallSatatusChanged( const TInt aStatus )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::\
            CallSatatusChanged  calling" )
    
    LOG2( NORMAL, "SETUPCALL: CSetupCallRequestHandler::\
        CallSatatusChanged status == %i", aStatus )
    // We only care about the connecting status, after call is
    // connecting, we can send the respones to the SIM. For other status we will
    // waiting for the status of DialNumber.    
    if( ( aStatus == RMobileCall::EStatusConnected )
        || ( aStatus == RMobileCall::EStatusConnecting ))
        {
        iResponsed = ETrue;
        iDispatcher->SetupCallRequestComplete( KErrNone );
        }
    else
        {
        if ( iStatusHandler )
            {
            iStatusHandler->Start();
            }
        }

    LOG( SIMPLE, "SETUPCALL: CSetupCallRequestHandler::\
            CallSatatusChanged  exiting" )
    }


//  End of File
