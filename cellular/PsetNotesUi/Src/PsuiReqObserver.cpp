/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Request observer.
*
*/


// INCLUDE FILES
#include <PsuiContainer.h>

#include "PsuiReqObserver.h"
  
// MACROS

// MEMBER FUNCTIONS

// ---------------------------------------------------------------------------
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
CPsuiReqObserver* CPsuiReqObserver::NewL( CPsuiNoteMaster& aNoteMaster )
    {  
    return new( ELeave ) CPsuiReqObserver( aNoteMaster );
    }

CPsuiReqObserver::CPsuiReqObserver( CPsuiNoteMaster& aNoteMaster ):
    iNoteMaster(aNoteMaster)
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
CPsuiReqObserver::~CPsuiReqObserver()
    {
    }
 
// -----------------------------------------------------------------------------
// CPsuiNoteMaster::MServiceRequestNoteCancelledL
// Called when the Requesting note is cancelled by the user.
// -----------------------------------------------------------------------------
//
void CPsuiReqObserver::MServiceRequestNoteCancelledL( TBool /*aCancelledWithEscape*/ )
    {
    iNoteMaster.RequestNoteCancelledL();
    }    
// End of file
