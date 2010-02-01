/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPsuiNoteController class.
*
*/


// INCLUDE FILES
#include "PsuiNoteController.h"
#include "PsuiInternalConstants.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPsuiNoteController::CPsuiNoteController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPsuiNoteController::CPsuiNoteController() : CCoeStatic( KUidNoteControllerSingleton )
    {
    }

// Destructor
CPsuiNoteController::~CPsuiNoteController()
    {
    delete iNote;
    iNote = NULL; 
    }

// ---------------------------------------------------------
// CPsuiNoteController::InstanceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPsuiNoteController* CPsuiNoteController::InstanceL()
    {
    CPsuiNoteController* instance = static_cast<CPsuiNoteController*>(
        CCoeEnv::Static( KUidNoteControllerSingleton ) );
    if ( !instance )
        {
        instance = new ( ELeave ) CPsuiNoteController;
        CleanupStack::PushL( instance );
        instance->ConstructL();
        CleanupStack::Pop( instance );
        }
    return instance;
    }

// -----------------------------------------------------------------------------
// CPsuiNoteController::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPsuiNoteController::ConstructL()
    {
    iNote = CPsuiNoteMaster::NewL();
    }

// ---------------------------------------------------------
// CPsuiNoteController::ShowNoteL
// ---------------------------------------------------------
//
void CPsuiNoteController::ShowNoteL( TPsuiNoteTypes aType, TInt aResourceID )
    {
	  iNote->ShowNoteL( aType, aResourceID );
    }       

// ---------------------------------------------------------
// CPsuiNoteController::ShowBscListL
// ---------------------------------------------------------
//
void CPsuiNoteController::ShowBscListL( TInt aResourceHeading, TUint8 aBsc[KPSUIMaxBscNumber] )
    {
	  iNote->ShowBscListL( aResourceHeading, aBsc );
    }

// ---------------------------------------------------------
// CPsuiNoteController::SetErrorNoteResourceID
// ---------------------------------------------------------
//
TInt CPsuiNoteController::SetErrorNoteResourceID ( const TInt aErrorCode )
    {
	  return iNote->SetErrorNoteResourceID( aErrorCode );
    }

// ---------------------------------------------------------
// CPsuiNoteController::RemoveNotesL
// ---------------------------------------------------------
//
void CPsuiNoteController::RemoveNotesL()
    {
	  delete iNote;
	  iNote = NULL;
	  iNote = CPsuiNoteMaster::NewL();
    }

// ---------------------------------------------------------
// CPsuiNoteController::KillStaticNoteL
// ---------------------------------------------------------
//
void CPsuiNoteController::KillStaticNoteL()
    {
	  iNote->KillStaticNoteL();
    }

// ---------------------------------------------------------
// CPsuiNoteController::SetEngineContacts
// ---------------------------------------------------------
//
void CPsuiNoteController::SetEngineContacts( MPsetCallWaiting* aWaitingEngine,
    MPsetCallBarring* aBarringEngine, MPsetCallDiverting* aDivertEngine,
    MPsetCli* aCliEngine )
    {
    iNote->SetEngineContacts( aWaitingEngine, aBarringEngine,
        aDivertEngine, aCliEngine );	
    }

// ---------------------------------------------------------
// CPsuiNoteController::ShowBscCFListL
// ---------------------------------------------------------
//
TCFType CPsuiNoteController::ShowBscCFListL( TInt aResourceHeading, 
    CMobilePhoneCFList& aList, TCFType aCFType, TInt& aIndex )
    {
    return iNote->ShowBscCFListL( aResourceHeading, aList,
        aCFType, aIndex );	
    }

// ---------------------------------------------------------
// CPsuiNoteController::ShowCfInfoL
// ---------------------------------------------------------
//
void CPsuiNoteController::ShowCfInfoL( TCFType& aCFType,
    const RMobilePhone::TMobilePhoneCFInfoEntryV1& aSetting )
    {
    iNote->ShowCfInfoL( aCFType, aSetting );	
    }

// End of File
