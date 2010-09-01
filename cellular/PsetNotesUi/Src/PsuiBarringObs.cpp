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
* Description:  Shows notes related to Call Barrings operations.
*
*/


// INCLUDE FILES
#include <Psui.rsg>
#include <PsuiContainer.h>
#include <exterror.h>
#include <etelsat.h>

#include "PsuiInternalConstants.h" 
#include "PsuiNoteController.h"

//  MEMBER FUNCTIONS
// ---------------------------------------------------------------------------
// Symbian OS two-phased Constructor.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiBarringObs* CPsuiBarringObs::NewL( CPsuiContainer& aContainer )
    {
    CPsuiBarringObs* self = new( ELeave ) CPsuiBarringObs;
    self->SetContainer( aContainer );
    return self;
    }

// ---------------------------------------------------------------------------
// Sets container.
// ---------------------------------------------------------------------------
void CPsuiBarringObs::SetContainer( CPsuiContainer& aContainer )
    {
    iContainer = &aContainer;
    }

// ---------------------------------------------------------------------------
// Handles changing of barring mode.
// ---------------------------------------------------------------------------
void CPsuiBarringObs::HandleBarringModeChangedL( TBarringProgram /* aType */, 
                                                TBarringStatus aStatus, 
                                                TBool aPlural )
    {
    if ( aStatus == EBarringStatusActive )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL(
            EPsuiConfirmationNote, 
            R_CB_ACTIVE_NOTE );
        }
    else if ( aStatus == EBarringStatusInactive && !aPlural )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL(
            EPsuiConfirmationNote, 
            R_CB_CANCEL_NOTE );
        }
    
    else if ( aStatus == EBarringStatusInactive && aPlural )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL(
            EPsuiConfirmationNote, 
            R_CB_PLURAL_CANCEL_NOTE );
        }
    }  

// ---------------------------------------------------------------------------
// Handles the get status notifications.
// ---------------------------------------------------------------------------
void CPsuiBarringObs::HandleBarringModeStatusL( TUint8 aBsc[KPSUIMaxBscNumber],
                                               TBarringStatus aStatus )
    {
    if ( aStatus != EBarringStatusActive )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL(
            EPsuiConfirmationNote, 
            R_BARRING_NOT_ACTIVE_NOTE );
        }
    else
        {
        //if no bsc-list, show "Barring active" note
        if ( aBsc[0] == KPSUINoBscGroup )
            {
            CPsuiNoteController::InstanceL()->ShowNoteL(
                EPsuiConfirmationNote, 
                R_BARRING_ACTIVE_NOTE );
            }
        //otherwise show bsc-list
        else
            {
            CPsuiNoteController::InstanceL()->ShowBscListL(
                R_BARR_STATUS_HEADING, 
                aBsc );
            }
        }
    }

// ---------------------------------------------------------------------------
// Handles showing of error notes.
// ---------------------------------------------------------------------------
void CPsuiBarringObs::HandleBarringErrorL( TInt aReason )
    {
    if ( aReason != KErrSatControl  )
        {
        TInt errorResource( 
            CPsuiNoteController::InstanceL()->SetErrorNoteResourceID( aReason ) ); 
        CPsuiNoteController::InstanceL()->ShowNoteL( EPsuiErrorNote, errorResource );
        }
    } 

// ---------------------------------------------------------------------------
// Handles requesting notes (show/hide).
// ---------------------------------------------------------------------------
void CPsuiBarringObs::HandleCBRequestingL( TBool aOngoing, TBool aInterrupted )
    {
    if ( aInterrupted )
        {
        // if interrupted, means that incoming call and all notes must
        // be immediately destroyed.
        CPsuiNoteController::InstanceL()->RemoveNotesL();
        return;
        }
    if ( aOngoing )
        {
        // show requesting note
        CPsuiNoteController::InstanceL()->ShowNoteL( 
            EPsuiWaitingNote, 
            -1 );
        }
    else
        {
        // hide requesting note
        CPsuiNoteController::InstanceL()->KillStaticNoteL();
        }
    }

// ---------------------------------------------------------------------------
// Handles call barring password notes.
// ---------------------------------------------------------------------------
void CPsuiBarringObs::CbPasswordChangedL( TBool aSuccess )
    {
    if ( aSuccess )
        {
        CPsuiNoteController::InstanceL()->ShowNoteL( 
            EPsuiConfirmationNote, 
            R_PASSWORD_CHANGED_NOTE );
        }
    else
        {
        HandleBarringErrorL( KErrGsmSSNegativePasswordCheck );
        }
    }

// ---------------------------------------------------------------------------
// Sets engine contact for observer.
// ---------------------------------------------------------------------------
void CPsuiBarringObs::SetEngineContact( MPsetCallBarring* aBarringEngine )
    {
    CPsuiNoteController::InstanceL()->SetEngineContacts( 
        NULL, 
        aBarringEngine, 
        NULL, 
        NULL );
    }

// End of File
