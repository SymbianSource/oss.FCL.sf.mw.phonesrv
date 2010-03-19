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
* Description:  Shows notes related to Call Identity operations.
*
*/


// INCLUDE FILES
#include <psui.rsg> 
#include <featmgr.h>
#include <etelsat.h>
 
#include "psuicliobserver.h" 
#include "psuicontainer.h" 
#include "PsuiNoteController.h" 

//  MEMBER FUNCTIONS

// ---------------------------------------------------------------------------
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiCliObserver* CPsuiCliObserver::NewL( CPsuiContainer& aContainer )
    {
    CPsuiCliObserver* self = new( ELeave ) CPsuiCliObserver;
    CleanupStack::PushL( self );
    // Sets up TLS, must be done before FeatureManager is used.
    FeatureManager::InitializeLibL();    
    CleanupStack::Pop( self );
    self->SetContainer( aContainer );
    return self;
    }

// ---------------------------------------------------------------------------
// Handles calling identity notes.
// ---------------------------------------------------------------------------
void CPsuiCliObserver::CliInformationL( TPsuiCli aType )
    {
    TInt resourceID = R_NOT_DONE_NOTE;
    switch( aType ) 
        {
        case EPsuiClirOn:
            resourceID = R_CLIR_ACTIVE_NOTE;
            break;
        case EPsuiClirOff:
            resourceID = R_CLIR_INACTIVE_NOTE;
            break;
        case EPsuiClipOn:
            resourceID = R_CLIP_ACTIVE_NOTE;
            break;
        case EPsuiClipOff:
            resourceID = R_CLIP_INACTIVE_NOTE;
            break;
        case EPsuiColrOn:
            resourceID = R_COLR_ACTIVE_NOTE;
            break;
        case EPsuiColrOff:
            resourceID = R_COLR_INACTIVE_NOTE;
            break;
        case EPsuiColpOn:
            resourceID = R_COLP_ACTIVE_NOTE;
            break;
        case EPsuiColpOff:
            resourceID = R_COLP_INACTIVE_NOTE;
            break;
        case EPsuiCliUnknown:
        default:
            resourceID = R_NOT_DONE_NOTE;
            break;
        }
    CPsuiNoteController::InstanceL()->ShowNoteL( 
        EPsuiConfirmationNote, 
        resourceID );
    }

// ---------------------------------------------------------------------------
// Handles requesting notes (show/hide).
// ---------------------------------------------------------------------------
void CPsuiCliObserver::HandleCliRequestingL( TBool aOngoing, TBool aInterrupted )
    {
    if ( aInterrupted )
        {
        CPsuiNoteController::InstanceL()->RemoveNotesL();
        }
    else
        {
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
    }

// ---------------------------------------------------------------------------
// Sets engine contact for observer.
// ---------------------------------------------------------------------------
void CPsuiCliObserver::SetEngineContact( MPsetCli* aEngine )
    {
    CPsuiNoteController::InstanceL()->SetEngineContacts( 
        NULL, 
        NULL, 
        NULL, 
        aEngine );
    }

// ---------------------------------------------------------------------------
// Handles Cnap status requests.
// ---------------------------------------------------------------------------
void CPsuiCliObserver::HandleCnapStatusL( TInt 
    aStatus 
    )
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdPhoneCnap ) )
        {
        switch ( aStatus )
            {
            case RMobilePhone::EIdServiceActivePermanent:        
            case RMobilePhone::EIdServiceActiveDefaultAllowed:
                CPsuiNoteController::InstanceL()->ShowNoteL( 
                    EPsuiConfirmationNote, 
                    R_CNAP_ACTIVE );
                break;
            case RMobilePhone::EIdServiceActiveDefaultRestricted:
            case RMobilePhone::EIdServiceNotProvisioned:
                CPsuiNoteController::InstanceL()->ShowNoteL( 
                    EPsuiConfirmationNote, 
                    R_CNAP_INACTIVE );
                break;
            default:
                CPsuiNoteController::InstanceL()->ShowNoteL( 
                    EPsuiConfirmationNote, 
                    R_NOT_DONE_NOTE );
                break;
            }
        }
    else
        {
        CPsuiNoteController::InstanceL()->ShowNoteL( 
            EPsuiConfirmationNote, 
            R_NOT_DONE_NOTE );
        }
    }

// ---------------------------------------------------------------------------
// Handles errors.
// ---------------------------------------------------------------------------
void CPsuiCliObserver::HandleCliErrorL( TInt aError )
    {
    if ( aError != KErrSatControl  )
        {
        TInt errorResource = 
            CPsuiNoteController::InstanceL()->SetErrorNoteResourceID( aError );
        CPsuiNoteController::InstanceL()->ShowNoteL( EPsuiErrorNote, errorResource );
        }
    }

// ---------------------------------------------------------------------------
// Sets container.
// ---------------------------------------------------------------------------
void CPsuiCliObserver::SetContainer( CPsuiContainer& aContainer )
    {
    iContainer = &aContainer;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
CPsuiCliObserver::~CPsuiCliObserver()
    {
    //Remove FeatureManager
    FeatureManager::UnInitializeLib();    
    }

// End of File
