/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Container object for the PSetNotes objects. 
*
*/


//INCLUDES
#include <psuicontainer.h> 

#include "PsuiResourceLoader.h" 

// ---------------------------------------------------------------------------
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiContainer* CPsuiContainer::NewL()
    {
    CPsuiContainer* self = new ( ELeave ) CPsuiContainer;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian OS 2nd phase constructor.
// ---------------------------------------------------------------------------
void CPsuiContainer::ConstructL()
    {
    iAppEngineResourceLoader = CPsuiResourceLoader::NewL();
    }

// ---------------------------------------------------------------------------
// Destructor. 
// ---------------------------------------------------------------------------
CPsuiContainer::~CPsuiContainer()
    {
    delete iAppEngineResourceLoader;
    }

// ---------------------------------------------------------------------------
// Creates cli observer. Ownership of the class is transferred.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiCliObserver* CPsuiContainer::CreateCliObsL()
    {    
    return CPsuiCliObserver::NewL( *this );
    }

// ---------------------------------------------------------------------------
// Creates call waiting observer. Ownership of the class is transferred.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiWaitingObs* CPsuiContainer::CreateCWObsL()
    {    
    return CPsuiWaitingObs::NewL( *this );
    }

// ---------------------------------------------------------------------------
// Creates call forward observer. Ownership of the class is transferred.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiDivertObs* CPsuiContainer::CreateCFObsL()
    {    
    return CPsuiDivertObs::NewL( *this );
    }

// ---------------------------------------------------------------------------
// Creates call barring observer. Ownership of the class is transferred.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiBarringObs* CPsuiContainer::CreateCBObsL()
    {    
    return CPsuiBarringObs::NewL( *this );
    }

// End of file
