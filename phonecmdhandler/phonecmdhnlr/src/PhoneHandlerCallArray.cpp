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
* Description:  Implements the class CPhoneHandlerCallArray
*
*/

#include <callinformation.h>
#include "PhoneHandlerCallArray.h"

// ---------------------------------------------------------------------------
// CPhoneHandlerCallArray::NewL.
// ---------------------------------------------------------------------------
//
CPhoneHandlerCallArray* CPhoneHandlerCallArray::NewL( )
    {
    CPhoneHandlerCallArray* self = new ( ELeave ) CPhoneHandlerCallArray(  );
    return self;    
    }

// ---------------------------------------------------------------------------
// Destructs the array including remaining calls.
// ---------------------------------------------------------------------------
//
CPhoneHandlerCallArray::~CPhoneHandlerCallArray( )
    {
     TInt callCount = iCallArray.Count();
    
    for (TInt callIndex = 0; callIndex < callCount; callIndex++)
        {
        TCallInfo* call = iCallArray[ callIndex ];
        delete call;
        }
        
    iCallArray.Reset();
    iCallArray.Close();
    }

// ---------------------------------------------------------------------------
// Iterates through call objects finding the call matching the given call index.
// ---------------------------------------------------------------------------
//    
TCallInfo* CPhoneHandlerCallArray::FindCall( TInt aCallIndex )
    {
    TInt callCount = iCallArray.Count();
    for (TInt callIndex = 0; callIndex < callCount; callIndex++)
        {
        TCallInfo* call = iCallArray[ callIndex ];        
        if ( call )
            {
            if ( call->CallIndex() == aCallIndex )
                {
                return call;
                }
            }
        }

    return NULL;
    }

// ---------------------------------------------------------------------------
// CPhoneHandlerCallArray::Add
// ---------------------------------------------------------------------------
//    
void CPhoneHandlerCallArray::AddL( const MCall* aCall )
    {
    //take heap based copy and add it to the array
    TCallInfo* call = new( ELeave )TCallInfo;    
	
    call->iIndex = aCall->CallIndex();
	call->iState = aCall->CallState();
	call->iType = aCall->CallType();
	call->iDirection = aCall->CallDirection();
	call->iEmergencyCall = aCall->IsEmergency();
	call->iServiceId = aCall->ServiceId();    

	CleanupStack::PushL( call );
	iCallArray.AppendL( call );    
	CleanupStack::Pop( call );
    }

// ---------------------------------------------------------------------------
// CPhoneHandlerCallArray::Remove
// ---------------------------------------------------------------------------
//    
TInt CPhoneHandlerCallArray::Remove( TCallInfo* aCall ) 
    {
    TInt index = iCallArray.Find( aCall );
    if ( index != KErrNotFound )
        {
        TCallInfo* call = iCallArray[ index ];
        delete call;
        iCallArray.Remove( index );
        return KErrNone;
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CPhoneHandlerCallArray::CallCount
// ---------------------------------------------------------------------------
//    
TInt CPhoneHandlerCallArray::CallCount( )
    {
    return iCallArray.Count();
    }

//---------------------------------------------------------------------------
// CPhoneHandlerCallArray::CallByState
// ---------------------------------------------------------------------------
//    
TCallInfo* CPhoneHandlerCallArray::CallByState( TInt aState )
    {
    TCallInfo* call;
    TInt callCount = iCallArray.Count();
    for (TInt callIndex = 0; callIndex < callCount; callIndex++)
        {
        call = iCallArray[ callIndex ];      
        
        if( call->CallState() == aState)
        	{
        	return call;
            }
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// Constructs the monitor.
// ---------------------------------------------------------------------------
//
CPhoneHandlerCallArray::CPhoneHandlerCallArray()
    {
    }
	
//---------------------------------------------------------------------------
// CPhoneHandlerCallArray::ConnectedCallCount
// ---------------------------------------------------------------------------
//    
TInt CPhoneHandlerCallArray::ConnectedCallCount()
    {
    TInt callCount = iCallArray.Count();
	TInt connectedCalls = 0;
    for ( TInt callIndex = 0; callIndex < callCount; callIndex++ )
        {
        MCall* call = iCallArray[ callIndex ];        
        if ( CCPCall::EStateConnected == call->CallState() )
            {
            connectedCalls++;
            }
        }
    return connectedCalls;
    }

//---------------------------------------------------------------------------
// CPhoneHandlerCallArray::ExistingCallCount
// ---------------------------------------------------------------------------
//    
TInt CPhoneHandlerCallArray::ExistingCallCount()
    {
    TInt callCount = iCallArray.Count();
	TInt existingCalls = 0;
    for ( TInt callIndex = 0; callIndex < callCount; callIndex++ )
        {
        TCallInfo* call = iCallArray[ callIndex ];      
        CCPCall::TCallState callState = call->CallState();
        
        if( CCPCall::EStateIdle != callState  
			&& CCPCall::EStateDisconnecting != callState )
			{
			existingCalls++;
            }
        }
    return existingCalls;
    }

// End of File
