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
* Description:  Minite the call creating status by ETelMM
*
*/


#include <etelmm.h>
#include <mmtsy_names.h>

#include "csetupcallstatushandler.h"
#include "CSetUpCallHandler.h"
#include "msatasynctosync.h"
#include "SatLog.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CSetupCallStatusHandler::CSetupCallStatusHandler
// The class constructor.
// -----------------------------------------------------------------------------
//
CSetupCallStatusHandler::CSetupCallStatusHandler(
                MSatMultiModeApi& aPhone, MSetupCallStatusObserver* aDispatcher )
    : CActive( EPriorityStandard ), iPhone( aPhone ), 
      iDispatcher( aDispatcher )
    {
    LOG( SIMPLE,
        "SETUPCALL: CSetupCallStatusHandler::CSetupCallStatusHandler \
        calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SETUPCALL: CSetupCallStatusHandler::CSetupCallStatusHandler \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallStatusHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetupCallStatusHandler* CSetupCallStatusHandler::NewL(
    MSatMultiModeApi& aPhone, MSetupCallStatusObserver* aDispatcher )
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallStatusHandler::NewL calling" )

    CSetupCallStatusHandler* self =
        new ( ELeave ) CSetupCallStatusHandler( aPhone, aDispatcher );
            
    LOG( SIMPLE, "SETUPCALL: CSetupCallStatusHandler::NewL exiting" )
    return self;
    }


// -----------------------------------------------------------------------------
// CSetupCallStatusHandler::~CSetupCallStatusHandler
// Destructor
// -----------------------------------------------------------------------------
//
CSetupCallStatusHandler::~CSetupCallStatusHandler()
    {
    LOG( SIMPLE, "SETUPCALL: \
            CSetupCallStatusHandler::~CSetupCallStatusHandler calling" )
    Cancel();

    LOG( SIMPLE, "SETUPCALL: \
            CSetupCallStatusHandler::~CSetupCallStatusHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallStatusHandler::DialNumber
// -----------------------------------------------------------------------------
//
void CSetupCallStatusHandler::Start()
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallStatusHandler::Start calling" )
    if( !IsActive() )
        {
        iPhone.NotifyMobileCallStatusChange( iStatus,
            iMobileCallStatus );
        SetActive();
        }
    LOG( SIMPLE, "SETUPCALL: CSetupCallStatusHandler::Start exiting" )
 
    }

// -----------------------------------------------------------------------------
// CSetupCallStatusHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSetupCallStatusHandler::RunL()
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallStatusHandler::RunL calling" )
    
    LOG2( NORMAL, "SETUPCALL: CSetupCallStatusHandler::RunL\
          iStatus == %i", iStatus.Int() )

    LOG2( NORMAL, "SETUPCALL: CSetupCallStatusHandler::RunL\
        iMobileCallStatus == %i", iMobileCallStatus )

    if( ( KErrNone == iStatus.Int() ) && iDispatcher )
        {
        iDispatcher->CallSatatusChanged ( iMobileCallStatus );
        }
    
    LOG( SIMPLE, "SETUPCALL: CSetupCallStatusHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetupCallStatusHandler::CancelOperation
// -----------------------------------------------------------------------------
//
void CSetupCallStatusHandler::CancelOperation()
    {
    LOG( SIMPLE, "SETUPCALL: \
                  CSetupCallStatusHandler::CancelOperation calling" )
    iPhone.NotifyCallStatusChangeCancel();
    LOG( SIMPLE, 
        "SETUPCALL: CSetupCallStatusHandler::CancelOperation exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSetupCallStatusHandler::DoCancel()
    {
    LOG( SIMPLE, "SETUPCALL: CSetupCallStatusHandler::DoCancel calling" )
    CancelOperation();
    LOG( SIMPLE, "SETUPCALL: CSetupCallStatusHandler::DoCancel exiting" )
    }

//  End of File
