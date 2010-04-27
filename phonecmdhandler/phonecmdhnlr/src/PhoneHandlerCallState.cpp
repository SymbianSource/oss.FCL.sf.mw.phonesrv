/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Listens to call state changes and informs control. 
*
*/


// INCLUDE FILES
#include "PhoneHandlerCallState.h"
#include "PhoneHandlerControl.h"
#include "PhoneHandlerDebug.h"
#include <ctsydomainpskeys.h>

#include <ccallinformation.h>
#include <mcall.h>
#include <ccallinfoiter.h>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::CPhoneHandlerCallState
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerCallState::CPhoneHandlerCallState( CPhoneHandlerControl& aControl )
:	iControl( aControl )
	{
	} 

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallState::ConstructL()
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::ConstructL() start" );
	
	// Allows CPhoneHandlerCallState to start to listen call states.
	iInfo = CCallInformation::NewL();
	iInfo->NotifyCallInformationChanges(*this);
	
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::ConstructL() end" );
	}

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
CPhoneHandlerCallState* CPhoneHandlerCallState::NewL( CPhoneHandlerControl& aControl )
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::NewL()" );
	
	CPhoneHandlerCallState* self = new(ELeave) CPhoneHandlerCallState( aControl );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

// Destructor
CPhoneHandlerCallState::~CPhoneHandlerCallState()
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::~CPhoneHandlerCallState() start" );
	    
    if ( iInfo )
    	{
    	iInfo->CancelNotification();
    	delete iInfo;
    	}    

	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerCallState::~CPhoneHandlerCallState() end" );
	}

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::CallInformationChanged
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallState::CallInformationChanged()
	{
    TRAP_IGNORE( CallInformationChangedL() );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerCallState::CallInformationChangedL
// -----------------------------------------------------------------------------
//
void CPhoneHandlerCallState::CallInformationChangedL()
    {
     CCallInfoIter& iter = iInfo->GetCallsL();    
     for( iter.First(); !iter.IsDone(); iter.Next() )
         {
         const MCall* call ( &iter.Current() );
         if ( call )
        	 {
        	 iControl.NotifyCallStateL( call );
        	 }
         }
    }

// End of file
