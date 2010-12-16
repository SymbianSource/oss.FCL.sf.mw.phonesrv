/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CNWGsmMessageHandler
*                class member functions.
*
*/



// INCLUDE FILES
#include "NWHandlingEngine.h"
#include "CNWGsmMessageHandler.h"
#include "CNWNetworkFailureMonitor.h"
#include "NWLogger.h"


// CONSTANTS
// Number of monitors in Network Handling module and the running
// storage slot number in iMonitorContainer pointer array

#ifdef RD_PHONE_NG
const TInt KNWNetworkFailureMonitor = 4;
#else // RD_PHONE_NG
const TInt KNWNetworkFailureMonitor = 3;
#endif // RD_PHONE_NG


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWGsmMessageHandler::CNWGsmMessageHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWGsmMessageHandler::CNWGsmMessageHandler( 
            CNWGsmSessionImplementation& aNetworkData, 
            MNWMessageObserver& aMessageObserver,
            TNWInfo& aNWInfo,
            TBool aReceiveHzData )
            : CNWMessageHandler( aNetworkData,
                                 aMessageObserver,
                                 aNWInfo ),
            iReceiveHzData( aReceiveHzData )
    {
    NWLOGSTRING2( KNWOBJECT, "NW: CNWGsmMessageHandler::CNWGsmMessageHand\
        ler() Begin, iReceiveHzData=%d ", 
        iReceiveHzData );

    iNetworkInfo.iViagIndicatorType = ENWViagIndicatorTypeNone;
    iNetworkInfo.iMCNIndicatorType = ENWMCNIndicatorTypeNone;
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWGsmMessageHandler::CNWGsmMessageHand\
        ler() End " );
    }

// ----------------------------------------------------------------------------
// CNWGsmMessageHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWGsmMessageHandler::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWGsmMessageHandler::ConstructL() Begin " );
    
    //Initializes base class member variables
    BaseConstructL();

    iCellReselectionHandler = CNWGsmNetworkCellReselectionHandler::NewL( 
            *this,
            iPhone,
            iNetworkInfo,
            iCustomAPI,
            iInterNetworkInfo,
            iReceiveHzData );

    // Create CNWNetworkFailureMonitor object and insert it to monitor container.
    User::LeaveIfError( iMonitorContainer.Insert( 
        ( new ( ELeave ) CNWNetworkFailureMonitor( *this, iPhone, iNetworkInfo, iCustomAPI ) ), 
        KNWNetworkFailureMonitor ) );

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWGsmMessageHandler::ConstructL() End " );
    }

// ----------------------------------------------------------------------------
// CNWGsmMessageHandler::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWGsmMessageHandler* CNWGsmMessageHandler::NewL(
            CNWGsmSessionImplementation& aNetworkData,
            MNWMessageObserver& aMessageObserver,
            TNWInfo& aNWInfo,
            TBool aReceiveHzData )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWGsmMessageHandler::NewL() Begin " );

    CNWGsmMessageHandler* self = new( ELeave ) CNWGsmMessageHandler(
            aNetworkData,
            aMessageObserver,
            aNWInfo,
            aReceiveHzData );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWGsmMessageHandler::NewL() End " );

    return self;
    }

    
// Destructor
CNWGsmMessageHandler::~CNWGsmMessageHandler()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWGsmMessageHandler::~CNWGsmMessageHandler() Begin " );

    delete iCellReselectionHandler;
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWGsmMessageHandler::~CNWGsmMessageHandler() End " );
    }


// ----------------------------------------------------------------------------
// CNWGsmMessageHandler::SendMessage
// Reroutes messages to the Client object
// The function controls the starting of the monitors other than registration 
// status monitor. Other monitors are starter when registration status message 
// is received and phone has not been registered.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWGsmMessageHandler::SendMessage(
    MNWMessageObserver::TNWMessages aMessage ) // send message
    {
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWGsmMessageHandler::SendMessage() Begin" );
    if ( !iIsRegistered && ( aMessage == 
        MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange ) )
        {
        // Start other monitors only after we are successfully 
        // registered to network.
        iMonitorContainer[ KNWNetworkFailureMonitor ]->Initialise();
        }

    if ( aMessage == MNWMessageObserver::ENWMessageNetworkInfoChange )
        {
        NWLOGSTRING( KNWMESOUT,
            "NW: CNWGsmMessageHandler::SendMessage: Handle CellReselection" );
        iCellReselectionHandler->HandleCellReselection();

        CNWMessageHandler::SendMessage( 
            MNWMessageObserver::ENWMessageNetworkCellReselection );
        }

    CNWMessageHandler::SendMessage( aMessage );
    
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWGsmMessageHandler::SendMessage() End" );
    }


// ----------------------------------------------------------------------------
// CNWGsmMessageHandler::HandleSubscriberIdChange
// Method is called, if subscriber ID ( IMSI code ) is changed.
// ----------------------------------------------------------------------------
//
void CNWGsmMessageHandler::HandleSubscriberIdChange()
    {
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWGsmMessageHandler::HandleSubscriberIdChange() Begin" );

    iCellReselectionHandler->HandleCellReselection();
    
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWGsmMessageHandler::HandleSubscriberIdChange() End" );
    }

// ----------------------------------------------------------------------------
// CNWGsmMessageHandler::HandleUpdateReadingStatus
// Updates Operator name's read status
// ----------------------------------------------------------------------------
//
void CNWGsmMessageHandler::HandleUpdateReadingStatus( 
        const TNWRead& aElementFile, TBool aReadStatus )
    {
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWGsmMessageHandler::HandleUpdateReadingStatus() Begin" );
    
    iCellReselectionHandler->UpdateReadingStatus( aElementFile, aReadStatus );
    
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWGsmMessageHandler::HandleUpdateReadingStatus() End" );
    }

//  End of File  
