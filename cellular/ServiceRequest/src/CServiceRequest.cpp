/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for service request
*
*/



// INCLUDE FILES
#include    "cservicerequest.h" 
#include    "mservicerequestobserver.h" 
#include    <servicerequest.rsg> 

#include    <AknWaitDialog.h> 
#include    <AknGlobalNote.h> 
#include    <aknnotewrappers.h>
#include    <avkon.hrh>

#include    <StringLoader.h> 
#include    <gsmerror.h>
#include    <exterror.h>
#include    <ConeResLoader.h> 
#include    <textresolver.h>


// CONSTANTS
_LIT( KSerqResourcesWithFullPath,"z:\\resource\\ServiceRequest.rsc" );
 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CServiceRequest::CServiceRequest
// C++ default constructor can NOT contain any code that
// might leave.
// -----------------------------------------------------------------------------
//
CServiceRequest::CServiceRequest( MServiceRequestObserver& aObserver )
: CActive(EPriorityStandard) , iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CServiceRequest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CServiceRequest::ConstructL()
    {
    iResourceLoader = new(ELeave) RConeResourceLoader( *CCoeEnv::Static() ); 
    TFileName fileName( KSerqResourcesWithFullPath ); 
    iResourceLoader->OpenL(fileName);
    }

// -----------------------------------------------------------------------------
// CServiceRequest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CServiceRequest* CServiceRequest::NewL( MServiceRequestObserver& aObserver )
    {
    CServiceRequest* self = new( ELeave ) CServiceRequest( aObserver );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
EXPORT_C CServiceRequest::~CServiceRequest()
    {
    Reset();

    if ( iResourceLoader )
        {
        iResourceLoader->Close();
        }
    delete iResourceLoader;
    iResourceLoader = NULL;

    // If instance is deleted in call back - inform the function.
    if ( iDestroyed )
        {
        *iDestroyed = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CServiceRequest::Reset
// Deletes used member data
// -----------------------------------------------------------------------------
//
void CServiceRequest::Reset()
    {
    Cancel();

    delete iRequestingNote;
    iRequestingNote = NULL;
    
    delete iNoteText;
    iNoteText = NULL;
    }

// -----------------------------------------------------------------------------
// CServiceRequest::StartL
// Launches the request note.
// -----------------------------------------------------------------------------
//
EXPORT_C void CServiceRequest::StartL( const TDesC& aText )
    {
 
    // delete possible old data:
    Reset();

    // Choose alternative "requesting" text if given
    if ( aText.Length() )
        {
        iNoteText = aText.AllocL();
        }

    // Create note and launch it.
    CAknWaitDialog* requestingNote = new(ELeave) CAknWaitDialog(
        (REINTERPRET_CAST(CEikDialog**,&iRequestingNote)) , ETrue );
    iRequestingNote = requestingNote;
    requestingNote->PrepareLC( R_SERQ_REQUESTING_WAIT_NOTE );
    if ( iNoteText )
        {
        requestingNote->SetTextL( iNoteText->Des() );
        }
    requestingNote->SetCallback( this );
    requestingNote->RunLD();
    }

// -----------------------------------------------------------------------------
// CServiceRequest::DialogDismissedL
// Called when the note is cancelled. Observer call made asyncronously.
// -----------------------------------------------------------------------------
//
void CServiceRequest::DialogDismissedL( TInt aButtonId )
    {
    Cancel();

    if ( aButtonId == EAknSoftkeyQuit || aButtonId == EEikBidCancel )
        { 
        // Activate new request
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, aButtonId );
        SetActive();
        }
    }

// -----------------------------------------------------------------------------
// CServiceRequest::ShowCompletionNoteL
// Removes the wait note and shows another note if needed.
// -----------------------------------------------------------------------------
//
EXPORT_C void CServiceRequest::ShowCompletionNoteL( 
    TBool aShowNote,
    TInt aError,
    TSerqNoteType aNoteType,
    const TDesC& aText )
    {
    // delete old instances
    Reset();

    // if new notes are not required
    if ( !aShowNote )
        {
        return;
        }

    // Check default note appearance
    TInt defaultTextId;
    TSerqNoteType defaultNoteType;
    GetDefaultNote( aError , defaultNoteType , defaultTextId );

    // Choose text
    if ( aText.Length() )
        {
        iNoteText = aText.AllocL();
        }
    else
        {
        // read from resources
        iNoteText = StringLoader::LoadL( defaultTextId, CCoeEnv::Static() );
        }

    // Choose note type
    TSerqNoteType noteType;
    if ( aNoteType != ESerqDefault )
        {
        noteType = aNoteType;
        }
    else
        {
        noteType = defaultNoteType;
        }

    // Start note
    LaunchNoteL( noteType );
    }

// -----------------------------------------------------------------------------
// CServiceRequest::GetDefaultNote
// Decides note type based on the given error value.
// -----------------------------------------------------------------------------
//
void CServiceRequest::GetDefaultNote( 
    const TInt aError,
    TSerqNoteType& aNoteType,
    TInt& aResourceId )
    {
    switch ( aError  )
        {
        case KErrNone:
            aResourceId = R_TEXT_DONE;
            aNoteType = ESerqGlobalConfirmationNote;
            break;
        case KErrCancel:
            aResourceId = R_TEXT_UNCONFIRMED;
            aNoteType = ESerqLocalInformationNote;
            break;
        case KErrInUse:
        case KErrAccessDenied:
        case KErrGsmSSUnknownSubscriber:
        case KErrGsmSSAbsentSubscriber:        
        case KErrGsmSSIllegalOperation:
        case KErrGsmSSIllegalSubscriber:
        case KErrGsmSSSubscriptionViolation:
        case KErrGsmSSIllegalEquipment:
            aResourceId = R_TEXT_NOT_ALLOWED;
            aNoteType = ESerqGlobalInformationNote;
            break;
        case KErrGsmSSCallBarred:       
        case KErrGsmSSIncompatibility:
            aResourceId = R_QTN_SRVRQ_INFO_CONFL;
            aNoteType = ESerqGlobalInformationNote;
            break;
        case KErrGsmSSSystemFailure:
            aResourceId = R_TEXT_RESULT_UNKNOWN;
            aNoteType = ESerqGlobalInformationNote;
            break;        
        case KErrGsmSSErrorStatus:
        case KErrGsmSSUnexpectedDataValue:
        case KErrGsmSSResourcesUnavailable:
            aResourceId = R_TEXT_REQUEST_REJECTED;
            aNoteType = ESerqGlobalInformationNote;
            break;
        case KErrGsmSSPasswordAttemptsViolation:
        case KErrGsmSSNegativePasswordCheck:
        case KErrGsmSSPasswordRegistrationFailure:
            aResourceId = R_PASSWORD_ERROR_NOTE;
            aNoteType = ESerqGlobalErrorNote;
            break;
        case KErrGsmNoService:
            aResourceId = R_TEXT_NO_SERVICE;
            aNoteType = ESerqGlobalInformationNote;
            break;
        default:
            aResourceId = R_TEXT_NOT_DONE;
            aNoteType = ESerqGlobalInformationNote;
            break; 
        }
    }

// -----------------------------------------------------------------------------
// CServiceRequest::LaunchNoteL
// Launches given note type. Text is got from iNoteText member variable.
// -----------------------------------------------------------------------------
//
void CServiceRequest::LaunchNoteL( const TSerqNoteType aNoteType)
    {
    
    switch ( aNoteType )
        {
        case ESerqLocalInformationNote:
        case ESerqLocalConfirmationNote:
            {
            CAknResourceNoteDialog* dlg = NULL;
            if ( aNoteType == ESerqLocalInformationNote )
                {
                dlg = new ( ELeave ) CAknInformationNote( ETrue );
                }
            else
                {
                dlg = new ( ELeave ) CAknConfirmationNote( ETrue );
                }
            dlg->ExecuteLD(  iNoteText->Des() );

            delete iNoteText;
            iNoteText = NULL;
            break;
            }
        case ESerqGlobalInformationNote:
        case ESerqGlobalErrorNote:
        case ESerqGlobalConfirmationNote:
            {
            CAknGlobalNote* dlg = CAknGlobalNote::NewLC();
            if ( aNoteType == ESerqGlobalInformationNote )
                {
                dlg->ShowNoteL( EAknGlobalInformationNote , iNoteText->Des() );
                }
            else if ( aNoteType == ESerqGlobalErrorNote )
                {
                dlg->ShowNoteL( EAknGlobalErrorNote , iNoteText->Des() );
                }
            else
                {
                dlg->ShowNoteL( EAknGlobalConfirmationNote , iNoteText->Des() );
                }
            CleanupStack::PopAndDestroy( dlg );
            delete iNoteText;
            iNoteText = NULL;
            break;
            }
        default:
            User::Invariant();
            break;
        }
    }

// -----------------------------------------------------------------------------
// CServiceRequest::RunL
// Calls observer interface to tell that the notes has been dismissed.
// -----------------------------------------------------------------------------
//
void CServiceRequest::RunL()
    {
    __ASSERT_ALWAYS( !iDestroyed, User::Invariant() );

    TBool destr = EFalse; // Destructor changes to ETrue if deleted.
    iDestroyed = &destr;

    // Object still exists and the note is cancelled:
    TRAPD( error , 
        iObserver.MServiceRequestNoteCancelledL( iStatus.Int() == EEikBidCancel ) );
    
    // All OK
    if ( error == KErrNone )
        {
        if ( !destr )
            {
            iDestroyed = NULL;
            }
        return;
        }

    // We got an error value.
    if ( !destr )
        {
        iDestroyed = NULL;
        User::Leave( error );
        }
    else
        {
        CTextResolver* textResolver = CTextResolver::NewLC();
        HBufC* buffer = textResolver->ResolveErrorString( error ).AllocLC();
        CAknGlobalNote* note = CAknGlobalNote::NewLC();
        note->ShowNoteL( EAknGlobalErrorNote, *buffer );
        CleanupStack::PopAndDestroy( note );
        CleanupStack::PopAndDestroy( buffer ); 
        CleanupStack::PopAndDestroy( textResolver );
        }
    }

// ---------------------------------------------------------
// CServiceRequest::RunError
// Called when RunL leaves
// (other items were commented in a header). 
// ---------------------------------------------------------
// 
TInt CServiceRequest::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CServiceRequest::DoCancel
// Does nothing. Must be implemented due inheritance.
// -----------------------------------------------------------------------------
//
void CServiceRequest::DoCancel()
    {
    }

//  End of File  
