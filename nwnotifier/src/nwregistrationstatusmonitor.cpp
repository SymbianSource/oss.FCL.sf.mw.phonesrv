/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: This module contains the implementation of CNWRegistrationStatusMonitor 
*              class member functions.
*
*/

//  INCLUDE FILES
#include "nwregistrationstatusmonitor.h"
// System 
#include <exterror.h>
#include <MmTsy_names.h> // KMmTsyModuleName

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::CNWRegistrationStatusMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
//
// -----------------------------------------------------------------------------
//
CNWRegistrationStatusMonitor::CNWRegistrationStatusMonitor() : 
    CActive( EPriorityStandard )
    {
    }
    

// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CNWRegistrationStatusMonitor::ConstructL()
    {  
    // Add to scheduler.    
    CActiveScheduler::Add( this );
    
    // Open connection to ETel
    OpenConnectionL();
    
    // Start monitoring
    IssueRequest();
    }


// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CNWRegistrationStatusMonitor* CNWRegistrationStatusMonitor::NewL()
    {
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::NewL -START") );
    
    CNWRegistrationStatusMonitor* self = new ( ELeave ) CNWRegistrationStatusMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::NewL -END") );
    return self;
    }
 

// -----------------------------------------------------------------------------
// Destructor
// 
// -----------------------------------------------------------------------------
//
CNWRegistrationStatusMonitor::~CNWRegistrationStatusMonitor()
    {
    Cancel();     
    
    CloseConnection();
    
    delete iNoteWrapper;
    }

// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::OpenConnectionL()
// Open connection to ETel
// -----------------------------------------------------------------------------
//
void CNWRegistrationStatusMonitor::OpenConnectionL()
    {
    // Connect ETel server
    User::LeaveIfError( iTelServer.Connect() );

    // Load ETel TSY module
    User::LeaveIfError( iTelServer.LoadPhoneModule( KMmTsyModuleName ) );

    // Set this session to receive detailed errors
    User::LeaveIfError( iTelServer.SetExtendedErrorGranularity(
            RTelServer::EErrorExtended ) );

    // Opens a phone subsession by name, 
    User::LeaveIfError( iPhone.Open( iTelServer, KMmTsyPhoneName ) );
    }

// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::CloseConnection()
// Closes the connection to ETel
// -----------------------------------------------------------------------------
//
void CNWRegistrationStatusMonitor::CloseConnection()
    {
    iPhone.Close();
        
    if ( iTelServer.Handle() )
        {
        iTelServer.UnloadPhoneModule( KMmTsyModuleName );
        iTelServer.Close();
        }
    }

// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::DoCancel
// Cancels the pending async. command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CNWRegistrationStatusMonitor::DoCancel()
    {
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::DoCancel -START") );
    
    iPhone.CancelAsyncRequest( EMobilePhoneNotifyNetworkRegistrationStatusChange );
    
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::DoCancel -END") );
    }


// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::RunL
// Receives the completion of the pending async. command,
// saves possible values from async.
// 
// -----------------------------------------------------------------------------
//
void CNWRegistrationStatusMonitor::RunL()
    {
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::RunL -START") );
    
    if( iStatus.Int() != KErrNone ) // Display only errors
        {
        // Parse error code to iResId
        ParseErrCode();
        
        if( !iNoteWrapper )
            { // Instantiate a notewrapper and pass err code to be displayed
            iNoteWrapper = CNetworkNotifierNoteWrapper::NewL( *this, iResID );
            }
        else // If note wrapper exists already, add error code -stack in wrapper
            {
            iNoteWrapper->AppendNote( iResID );
            }
        }
    else
        {
        if( iStatus.Int() == KErrBadHandle || 
            iStatus.Int() == KErrServerBusy )
            {
            // Close the session to ETel
            CloseConnection();
            // Reconnect
            OpenConnectionL();
            }
        }
    
    // Keep monitoring
    IssueRequest();
        
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::RunL -END") );
    }


// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::ParseErrorCode()
// 
// -----------------------------------------------------------------------------
//
void CNWRegistrationStatusMonitor::ParseErrCode()
    {
    switch( iStatus.Int() )
        {
        case KErrGsmMMImsiUnknownInHlr:
            {
            iResID = R_SIM_NOT_PROV_MM2;
            RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::ParseErrCode() : R_SIM_NOT_PROV_MM2") );
            break;
            }
        case KErrGsmMMIllegalMs:
            {
            iResID = R_SIM_NOT_ALLOW_MM3;
            RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::ParseErrCode() : R_SIM_NOT_ALLOW_MM3") );
            break;
            }
        case KErrGsmMMImsiUnknownInVlr:
            {
            iResID = R_CALL_FAILED_MM4;
            RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::ParseErrCode() : R_CALL_FAILED_MM4") );
            break;       
            }
        case KErrGsmMMImeiNotAccepted:
            {
            iResID = R_PHONE_NOT_ALLOW_MM5;
            RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::ParseErrCode() : R_PHONE_NOT_ALLOW_MM5") );
            break; 
            }
        case KErrGsmMMIllegalMe:
            {
            iResID = R_PHONE_NOT_ALLOW_MM6;
            RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::ParseErrCode() : R_PHONE_NOT_ALLOW_MM6") );
            break;
            }
        default:
            {
            case KErrGeneral:
                {
                RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::ParseErrCode() : R_SIM_NOT_ALLOW") );
                iResID = R_SIM_NOT_ALLOW;
                break;  
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::IssueRequest
// Executes the async. NotifyNetworkRegistrationStatusChange function.
// 
// -----------------------------------------------------------------------------
//
void CNWRegistrationStatusMonitor::IssueRequest()
    {
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::IssueRequest -START") );
    
    if ( !IsActive() )
        {
        iPhone.NotifyNetworkRegistrationStatusChange( iStatus, iRegistrationStatus ); 
        SetActive();
        RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::IssueRequest : NotifyChange()-request set active - pending !") );
        }
    
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::IssueRequest -END") );
    }
 

// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::PopupNoteClosed
// 
// Callback from note wrapper class : all error notes are displayed since this monitor
// class can destruct note wrapper
// -----------------------------------------------------------------------------
//
void CNWRegistrationStatusMonitor::PopupNoteClosed()
    {
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::PopupNoteClosed -START") );
    
    delete iNoteWrapper;
    iNoteWrapper = NULL;
    
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::PopupNoteClosed -END") );
    }


// -----------------------------------------------------------------------------
// CNWRegistrationStatusMonitor::RunError()
// Processes any errors.
// -----------------------------------------------------------------------------
//
TInt CNWRegistrationStatusMonitor::RunError( TInt /*aError*/ )
    {
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::RunError -START") );
    
    // Keep monitoring
    IssueRequest();
    
    RDebug::Print( _L("NW: CNWRegistrationStatusMonitor::RunError -END") );
    return KErrNone;    // Handled the error fully
    }

//  End of File 
