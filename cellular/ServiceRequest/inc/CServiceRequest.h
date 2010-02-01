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
* Description:  Interface for service request
*
*/



#ifndef CSERVICEREQUEST_H
#define CSERVICEREQUEST_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <AknProgressDialog.h>

// FORWARD DECLARATIONS
class MServiceRequestObserver;
class CAknWaitDialog;
class RConeResourceLoader;

// CLASS DECLARATION

/**
*  Interface for service request
*
* -Create instance by calling the static NewL method. User must implement the
*  observer interface MServiceRequestObserver. Cancelling the wait note is 
*  informed through this API.
*
*  iServiceRequest = CServiceRequest::NewL( *this );
*
*
* -The instance can be deleted at any time and all notes are dismissed.
*
*  delete iServiceRequest;
*
*
* -The wait note can be launched by calling StartL function:
*
*  iServiceRequest->StartL();
*
*  By default the text "Requesting" is used, but the user may give the text
*  as a parameter:
*
*  iServiceRequest->StartL( _L("Please wait...") );
*
*  Call to the StartL function will delete own notes visible at the call time.
*
*
* -Wait note can deleteted by calling the ShowCompletionNoteL function. This removes
*  all own notes. If the user wants to show a completion note for the request,
*  the parameter aShowNote must be set to ETrue. Creation of a new note may leave.
*  By deafult the Supplementary Service UI specification texts and note types are
*  used. The user may override some parameters, like note type and text. By default,
*  the error code is converted into note, e.g.:
*      KErrNone, Global confirmation note "Done"
*      KErrCancel, Local information note "Request not confirmed"
*      KErrAccessDenied, Global information note "Not allowed"
*  and so on.
*
*
*
*  @lib ServiceRequest.lib
*  @since 1.2
*/
class CServiceRequest : public CActive, public MProgressDialogCallback
    {
    public:  // Constructors and destructor

        // Used note types
        enum TSerqNoteType
            {
            ESerqDefault,
            ESerqLocalInformationNote,
            ESerqLocalConfirmationNote,
            ESerqGlobalInformationNote,
            ESerqGlobalErrorNote,
            ESerqGlobalConfirmationNote
            };

        
        /**
        * Two-phased constructor.
        * @param aObserver Oberver to inform cancelling.
        * @return New CServiceRequest object.
        */
        IMPORT_C static CServiceRequest* NewL( MServiceRequestObserver& aObserver );
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CServiceRequest();

    public: // New functions
        
        /**
        * Starts a wait note.
        * @since 1.2
        * @param aText "Requesting" text is used, if this descriptor's
        *              lenght is zero.
        */
        IMPORT_C void StartL( const TDesC& aText = KNullDesC() );

        /**
        * Removes wait note and shows new note if wanted.
        * If aShowNote = EFalse, this function won't leave!
        * Removal of the wait note is guaranteed in leave cases also.
        * @since 1.2
        * @param aShowNote ETrue: a note is shown 
        *                  EFalse: no additional note is shown
        * @param aError Used in default notes. The default note type and used
        *               text is decided based on this value. These vales are
        *               overridden if note type or used text is geven as a
        *               parameter.
        * @param aNoteType Type of the note. If EDefault is used, then the
        *                  aError parameter is checked.
        * @param aText Given text. If lenght is zero, aError parameter is checked.
        */
        IMPORT_C void ShowCompletionNoteL( 
            TBool aShowNote = ETrue,
            TInt aError = KErrNone,
            TSerqNoteType aNoteType = ESerqDefault,
            const TDesC& aText = KNullDesC() );

    protected: // From baseclasses

        // From MProgressDialogCallback
        void DialogDismissedL( TInt aButtonId );

        // From CActive
        void RunL();

        // From CActive
        void DoCancel();
        
        /**
        * From CActive
        * @param aError
        * @return TInt
        */
        TInt RunError(TInt aError);

    private:

        /**
        * C++ default constructor.
        */
        CServiceRequest( MServiceRequestObserver& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Zeroes data members
        */
        void Reset();

        /**
        * Gets default note implementation for give error value.
        */
        static void GetDefaultNote( 
            const TInt aError , 
            TSerqNoteType& aNoteType , 
            TInt& aResourceId );

        /**
        * Launches given note. Text is fetched from iNoteText.
        */
        void LaunchNoteL( const TSerqNoteType aNoteType);

    private:    // Data
        // Service request observer
        MServiceRequestObserver& iObserver;

        // Requesting wait note:
        CAknWaitDialog* iRequestingNote;

        // Resource loader
        RConeResourceLoader* iResourceLoader;

        // Text for note
        HBufC* iNoteText;

        // Enables instance deletion in call back.
        TBool* iDestroyed;
         
        // Reserved pointer for future extension
        TAny* iReserved;
    };

#endif      // CSERVICEREQUEST_H   
            
// End of File
