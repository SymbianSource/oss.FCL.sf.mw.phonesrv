/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ussd note controller
*
*/


// INCLUDE FILES
#include <AknGlobalNote.h> //CAknGlobalNote 
#include <aknnotewrappers.h>
#include <ConeResLoader.h> 
#include <StringLoader.h> 

#include "phcltclientserver.h" 
#include "cphcltussdnotecontroller.h" 


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCltUssdNoteController* CPhCltUssdNoteController::NewL(
        MPhCltUssdNoteControllerCallBack& aCallBack,
        TInt aPriority )
    {
    CPhCltUssdNoteController* self = new( ELeave ) 
        CPhCltUssdNoteController( aCallBack, aPriority );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::ConstructL()
    {
    LoadResourceFileL();
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::CPhCltUssdNoteController
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CPhCltUssdNoteController::CPhCltUssdNoteController(
    MPhCltUssdNoteControllerCallBack& aCallBack,
    TInt aPriority )
    : CActive( aPriority ), 
    iCallBack( aCallBack ),
    iWaitNoteId( KErrNotFound )
    {
    CActiveScheduler::Add( this );
    }
    
    
// -----------------------------------------------------------------------------
// CPhCltUssdNoteController:~CPhCltUssdNoteController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltUssdNoteController::~CPhCltUssdNoteController()
    {
    Cancel();

    DestroyGlobalWaitNote();
    
    delete iMessageBuffer;
    iMessageBuffer = NULL;
    
    if ( iResourceLoader )
        {
        iResourceLoader->Close();
        delete iResourceLoader;
        iResourceLoader = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::RunL
//
// Called when dialog is dismissed by pressing a softkey.
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::RunL()
    {
    iCallBack.GlobalWaitNoteDismissedL( iStatus.Int() );
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::DoCancel
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::DoCancel()
    {
    DestroyGlobalWaitNote();
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowInformationNoteL
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::ShowInformationNoteL( TInt aResourceId )
    {
    ShowNoteL( EPhCltUssdInformationNote, aResourceId );
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowGlobalInformationNoteL
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::ShowGlobalInformationNoteL( TInt aResourceId )
    {
    ShowNoteL( EPhCltUssdGlobalInformationNote, aResourceId );
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowGlobalConfirmationNoteL
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::ShowGlobalConfirmationNoteL( TInt aResourceId )
    {
    ShowNoteL( EPhCltUssdGlobalConfirmationNote, aResourceId );
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowGlobalWaitNoteL
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::ShowGlobalWaitNoteL( 
    TInt aResourceId, 
    TInt aSoftkeyResourceId )
    {
    delete iMessageBuffer;
    iMessageBuffer = NULL;
    iMessageBuffer = StringLoader::LoadL( aResourceId );

    DestroyGlobalWaitNote();
    
    iGlobalWaitNote = CAknGlobalNote::NewL();    
   
    if ( aSoftkeyResourceId )
        {
        iGlobalWaitNote->SetSoftkeys( aSoftkeyResourceId );
        }
    
    SetActive();
    
    iWaitNoteId = iGlobalWaitNote->ShowNoteL( iStatus,
                                EAknGlobalWaitNote, 
                                *iMessageBuffer );
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::DestroyGlobalWaitNote
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::DestroyGlobalWaitNote()
    {
    if ( iGlobalWaitNote )
        {
        if ( iWaitNoteId != KErrNotFound ) 
            {
            TRAP_IGNORE( iGlobalWaitNote->CancelNoteL( iWaitNoteId ) );
            iWaitNoteId = KErrNotFound;
            }
        delete iGlobalWaitNote;
        iGlobalWaitNote = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowNoteL
//
// Launches a corresponding dialog with the given string.
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::ShowNoteL( TPhCltUssdNoteType aType, TInt aResourceId )
    {
    switch ( aType )
        {
        case EPhCltUssdInformationNote:
            {       
            delete iMessageBuffer;
            iMessageBuffer = NULL;
            iMessageBuffer = StringLoader::LoadL( aResourceId );
            
            if ( iMessageBuffer->Length() == 0 )
                {
                break;
                }
            CAknInformationNote* dlg = new ( ELeave ) 
                CAknInformationNote( ETrue );

            dlg->ExecuteLD( *iMessageBuffer );
            break;
            }
        case EPhCltUssdGlobalInformationNote:
        case EPhCltUssdGlobalConfirmationNote:
            {
            CAknGlobalNote* dlg = CAknGlobalNote::NewLC();
            
            delete iMessageBuffer;
            iMessageBuffer = NULL;
            iMessageBuffer = StringLoader::LoadL( aResourceId );
            
            if ( aType == EPhCltUssdGlobalInformationNote )
                {
                dlg->ShowNoteL( EAknGlobalInformationNote , *iMessageBuffer );
                }
            else if ( aType == EPhCltUssdGlobalConfirmationNote )
                {
                dlg->ShowNoteL( EAknGlobalConfirmationNote , *iMessageBuffer );
                }
            CleanupStack::PopAndDestroy(); // dlg
            break;
            }

        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::LoadResourceFileL
//
// Loads resource file via RConeResourceLoader.
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::LoadResourceFileL()
    {
    // load resource file for notes
    iResourceLoader = new( ELeave ) RConeResourceLoader( *CCoeEnv::Static() );
    TFileName fileName ( KPhCltServerZDrive );
    fileName.Append( KDC_RESOURCE_FILES_DIR );
    fileName.Append( KPhCltResourceFileNameAndPath );
    fileName.ZeroTerminate();
    iResourceLoader->OpenL( fileName );
    }

// End of file

