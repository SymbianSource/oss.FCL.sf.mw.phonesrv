/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Shows notes and lists for Divert supplementary service
*
*/


// INCLUDE FILES
#include <Psui.rsg>
#include <mmlist.h>
#include <featmgr.h> 
#include <PsuiContainer.h>
#include <etelsat.h>

#include "PsuiNoteController.h"


// ---------------------------------------------------------------------------
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
EXPORT_C CPsuiDivertObs* CPsuiDivertObs::NewL( CPsuiContainer& aContainer )
    {
    CPsuiDivertObs* self = new ( ELeave ) CPsuiDivertObs;
    self->SetContainer( aContainer );
    return self;
    }

// ---------------------------------------------------------------------------
// Sets container.
// ---------------------------------------------------------------------------
void CPsuiDivertObs::SetContainer( CPsuiContainer& aContainer )
    {
    iContainer = &aContainer;
    }

// ---------------------------------------------------------------------------
// Handles changing of diverting.
// ---------------------------------------------------------------------------
void CPsuiDivertObs::HandleDivertingChangedL( 
    const TCallDivertSetting& aSetting, 
    TBool aPlural )
    {    
    switch( aSetting.iStatus )
        {
        case EDivertingStatusActive:
            if ( aPlural )
                {
                CPsuiNoteController::InstanceL()->ShowNoteL( 
                    EPsuiConfirmationNote, R_PLURAL_ACTIVATE_NOTE );
                }
            else
                {
                CPsuiNoteController::InstanceL()->ShowNoteL( 
                    EPsuiConfirmationNote, R_ACTIVATE_NOTE );
                }
            
            // Diverting calls does not affect Internet calls    
            if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) &&
                 aSetting.iServiceGroup == EServiceGroupVoice )
                {
                CPsuiNoteController::InstanceL()->ShowNoteL( 
                    EPsuiInformationNote, R_VOIP_DIVERT_NOT_SUPPORTED );
                }
            break;
        case EDivertingStatusNotRegistered:
        case EDivertingStatusInactive:
            if ( aPlural )
                {
                CPsuiNoteController::InstanceL()->ShowNoteL( 
                    EPsuiConfirmationNote, R_PLURAL_CANCEL_NOTE );
                }
            else
                {
                CPsuiNoteController::InstanceL()->ShowNoteL( 
                    EPsuiConfirmationNote, R_CANCEL_NOTE );
                }
            break;
        case EDivertingStatusNotProvisioned:
        case EDivertingStatusUnknown:
        default:
            CPsuiNoteController::InstanceL()->ShowNoteL( 
                EPsuiConfirmationNote, R_NOT_DONE_NOTE );
        }
    }  

// ---------------------------------------------------------------------------
// Handles the get status notifications.
// ---------------------------------------------------------------------------
/*****************************************************
*    Series 60 Customer / ETel
*    Series 60  ETel API
*****************************************************/ 
void CPsuiDivertObs::HandleDivertingStatusL( CMobilePhoneCFList&  aSetting, 
                                            TBool /*aPlural*/ )
    {
    TInt index = 0;
    TCFType action = EPsuiCFOther;
    
    //check the first item
    RMobilePhone::TMobilePhoneCFInfoEntryV1 cfInfo;
    cfInfo = aSetting.GetEntryL( index );
    switch( cfInfo.iStatus )
        {
        case RMobilePhone::ECallForwardingStatusNotActive:
            CPsuiNoteController::InstanceL()->ShowBscCFListL( 
                R_TEXT_DIVERT_INACTIVE, 
                aSetting, 
                EPsuiNoCF, 
                index );
            break;
        case RMobilePhone::ECallForwardingStatusActive:
            while ( action != EPsuiNoCF )
                {
                if ( cfInfo.iTimeout > 0 ) //cfnry involved
                    {
                    action = EPsuiCFNry;
                    }
                else // other type of cf
                    {
                    action = EPsuiCFOther;
                    }
                action = CPsuiNoteController::InstanceL()->ShowBscCFListL(
                    R_TEXT_DIVERT_ACTIVE,  
                    aSetting,
                    action, 
                    index );
                
                if ( action != EPsuiNoCF ) //EPsuiCFNry or EPsuiCFOther                
                    {
                    cfInfo = aSetting.GetEntryL( index );                
                    // display details-view with time and number
                    CPsuiNoteController::InstanceL()->ShowCfInfoL( action, cfInfo );
                    }
                }
            break;
        case RMobilePhone::ECallForwardingStatusNotRegistered:
            CPsuiNoteController::InstanceL()->ShowNoteL( 
                EPsuiConfirmationNote, 
                R_NOT_ACTIVE_NOTE );
            break;
        case RMobilePhone::ECallForwardingStatusNotProvisioned:
            CPsuiNoteController::InstanceL()->ShowNoteL( 
                EPsuiConfirmationNote, 
                R_NOT_ALLOWED );
            break;
        default:
            CPsuiNoteController::InstanceL()->ShowNoteL( 
                EPsuiConfirmationNote, 
                R_NOT_DONE_NOTE );
            break;
        }        
    }

// ---------------------------------------------------------------------------
// Handles showing of error notes.
// ---------------------------------------------------------------------------
void CPsuiDivertObs::HandleDivertingErrorL( TInt aReason )
    {
    if ( aReason != KErrSatControl  )
        {
        TInt errorResource = 
            CPsuiNoteController::InstanceL()->SetErrorNoteResourceID( aReason );
        CPsuiNoteController::InstanceL()->ShowNoteL( EPsuiErrorNote, errorResource );
        }
    }

// ---------------------------------------------------------------------------
// Handles requesting notes (show/hide).
// ---------------------------------------------------------------------------
void CPsuiDivertObs::HandleCFRequestingL( TBool aTrue, TBool aInterrupted )
    {
    if ( aInterrupted )
        {
        CPsuiNoteController::InstanceL()->RemoveNotesL();
        return;
        }
    if ( aTrue )
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
// Sets engine contact for observer.
// ---------------------------------------------------------------------------
void CPsuiDivertObs::SetEngineContact( MPsetCallDiverting* aDivertEngine )
    {
    CPsuiNoteController::InstanceL()->SetEngineContacts( 
        NULL, 
        NULL, 
        aDivertEngine, 
        NULL );
    }

// End of File
