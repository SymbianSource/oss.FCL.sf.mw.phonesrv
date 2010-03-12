/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CNetworkResetHandler is a async activeobject which handles reseting network call.
*
*
*/


//  INCLUDE FILES
#include "CNetworkResetHandler.h"
#include "PhoneSettingsLogger.h"

//  MEMBER FUNCTIONS

// ---------------------------------------------------------------------------
// 
// 1st phase constructor for network reseting observer.
// 
// ---------------------------------------------------------------------------
//
CNetworkResetHandler* CNetworkResetHandler::NewL( RMobilePhone& aPhone )
    {
    __PHSLOGSTRING("[PHS]CNetworkResetHandler::NewL");

    CNetworkResetHandler* self = new ( ELeave ) CNetworkResetHandler( aPhone );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;   
    }


// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
//
CNetworkResetHandler::CNetworkResetHandler( RMobilePhone& aPhone ) 
        : CActive( EPriorityStandard ), iPhone ( aPhone )                      
    {
    __PHSLOGSTRING("[PHS]CNetworkResetHandler::CNetworkResetHandler");
    }

// ---------------------------------------------------------------------------
// 
// Destructor.
// 
// ---------------------------------------------------------------------------
//
CNetworkResetHandler::~CNetworkResetHandler()
    {
    __PHSLOGSTRING("[PHS]CNetworkResetHandler::~CNetworkResetHandler");

    Cancel();
    iCustomApi.Close();
    }

// ---------------------------------------------------------------------------
// 
// ConstructL.
// 
// ---------------------------------------------------------------------------
//
void CNetworkResetHandler::ConstructL()
	{
    __PHSLOGSTRING("[PHS]CNetworkResetHandler::ConstructL");
    CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// 
// Close connect to ctsy
// 
// ---------------------------------------------------------------------------
//
void CNetworkResetHandler::RunL()
    {
    __PHSLOGSTRING("[PHS]CNetworkResetHandler::RunL");
    iCustomApi.Close();
    }

// ---------------------------------------------------------------------------
// 
// Requests ETEL to Reset back to the previously used network.
// 
// ---------------------------------------------------------------------------
//
TInt CNetworkResetHandler::ResetNetworkSearch()
	{
    __PHSLOGSTRING("[PHS]--> CNetworkResetHandler::ResetNetworkSearch");
    
    TInt error = KErrNone;

    if( IsActive() )
        {
        error = KErrInUse;
        }
    else
        {
        error = iCustomApi.Open( iPhone );
        }
    if( KErrNone == error )
        {
        // if there wasn´t already connect open to ETEL or active object wasn´t already in use, 
        // we can thus send request for reset network server
        iCustomApi.ResetNetServer( iStatus );
        SetActive();
        }
    __PHSLOGSTRING("[PHS]<-- CNetworkResetHandler::ResetNetworkSearch");
    return error;
	}

// ---------------------------------------------------------------------------
// 
// DoCancel.
// 
// ---------------------------------------------------------------------------
//
void CNetworkResetHandler::DoCancel()
	{
    __PHSLOGSTRING("[PHS]CNetworkResetHandler::DoCancel");
    iCustomApi.Close();
	}

// End of file
