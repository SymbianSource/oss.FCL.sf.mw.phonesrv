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
* Description:  Handles requests for showing Call Waiting notes.
*
*/


// INCLUDE FILES
#include <psui.rsg> 
#include <psuicontainer.h> 
#include <etelsat.h>
 
#include "psuiinternalconstants.h" 
#include "psuinotecontroller.h" 
#include "psuivariationproxy.h" 

//  MEMBER FUNCTIONS
// ---------------------------------------------------------------------------
// Symbian OS two-phased Constructor.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiWaitingObs* CPsuiWaitingObs::NewL( CPsuiContainer& aContainer )
    {
    CPsuiWaitingObs* self = new( ELeave ) CPsuiWaitingObs;
    CleanupStack::PushL( self );
    self->ConstructL( aContainer );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian OS 2nd phase Constructor.
// ---------------------------------------------------------------------------
void CPsuiWaitingObs::ConstructL( CPsuiContainer& aContainer )
    {
    iContainer = &aContainer;
  
    CPSuiVariationProxy* variationProxy = CPSuiVariationProxy::NewL();
    CleanupStack::PushL( variationProxy );
    iCallWaitingDistinguishEnabled = variationProxy->FeatureEnabled( KCallWaitingDistiquishNotProvisioned);
    CleanupStack::PopAndDestroy( variationProxy );
    
    }

// ---------------------------------------------------------------------------
// Handles changing of call waiting.
// ---------------------------------------------------------------------------
void CPsuiWaitingObs::HandleCallWaitingChangedL
( const MPsetCallWaiting::TSetCallWaiting aSetting, const TInt /* aResult */ )
    {
    if ( aSetting == MPsetCallWaiting::EActivateCallWaiting )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL( 
            EPsuiConfirmationNote, R_CW_ACTIVE_NOTE );
        }
    else if ( aSetting == MPsetCallWaiting::EDeactivateCallWaiting )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL( 
            EPsuiConfirmationNote, R_CW_CANCEL_NOTE );
        }
    }  

// ---------------------------------------------------------------------------
// Sets engine contact for observer.
// ---------------------------------------------------------------------------
void CPsuiWaitingObs::SetEngineContact( MPsetCallWaiting& aEngineContact )
    {
    CPsuiNoteController::InstanceL()->SetEngineContacts( 
        &aEngineContact, NULL, NULL, NULL );
    }

// ---------------------------------------------------------------------------
// Handles requesting notes (show/hide).
// ---------------------------------------------------------------------------
void CPsuiWaitingObs::HandleCWRequestingL( TBool aOngoing, TBool aInterrupted )
    {
    if ( aInterrupted )
        {
        CPsuiNoteController::InstanceL()->RemoveNotesL();
        return;
        }
    
    if ( aOngoing )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL( 
            EPsuiWaitingNote, -1 );
        }
    else
        {
        CPsuiNoteController::InstanceL()->KillStaticNoteL();
        }
    }

// ---------------------------------------------------------------------------
// Handles the get status notifications.
// ---------------------------------------------------------------------------
//
void CPsuiWaitingObs::HandleCallWaitingGetStatusL( 
    const MPsetCallWaiting::TGetCallWaitingStatus aStatus, 
    TUint8 aBsc[KPSUIMaxBscNumber] )
    {
    
    if ( iCallWaitingDistinguishEnabled && aStatus == MPsetCallWaiting::EStatusNotProvisioned )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL( 
            EPsuiConfirmationNote, R_CW_NOT_PROVISIONED_NOTE );
        }
    else if ( aStatus != MPsetCallWaiting::EStatusActive )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL( 
            EPsuiConfirmationNote, R_CW_NOT_ACTIVE_NOTE );
        }
    else
        {
        if( iCallWaitingDistinguishEnabled )
            {
                
            //show only "CW active" even if the bsc list is present. 
            //This change is to fix the error ENYI-6DYFKZ
            CPsuiNoteController::InstanceL()->ShowNoteL( EPsuiConfirmationNote, 
                R_CW_ACTIVE_NO_BSC_NOTE );
            }
        else
            {
                
            //if no bsc-list present, show only "CW active"
            if ( aBsc[0] == KPSUINoBscGroup )
                {
                CPsuiNoteController::InstanceL()->ShowNoteL( EPsuiConfirmationNote, 
                R_CW_ACTIVE_NO_BSC_NOTE );
                }
            else
                {
            
                CPsuiNoteController::InstanceL()->ShowBscListL( 
                R_CW_STATUS_HEADING, 
                aBsc );
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// This function handles errors in call waiting requests.
// ---------------------------------------------------------------------------
//
void CPsuiWaitingObs::HandleCWErrorL( TInt aReason )
    {
    if ( aReason != KErrSatControl  )
        {
        TInt errorResource(
            CPsuiNoteController::InstanceL()->SetErrorNoteResourceID( aReason ) );
            
        CPsuiNoteController::InstanceL()->ShowNoteL( EPsuiErrorNote, errorResource );
        }
    }

// End of File
