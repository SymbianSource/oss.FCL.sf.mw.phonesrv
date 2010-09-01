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
* Description:  Encapsulates an active object used in controlling notes.
*
*/


#ifndef CPHCLTUSSDNOTECONTROLLER_H
#define CPHCLTUSSDNOTECONTROLLER_H

// INCLUDES
#include <e32base.h>
#include "MPhCltUssdNoteControllerCallBack.h"

// FORWARD DECLARATIONS
class CAknGlobalNote;
class RConeResourceLoader;

// CLASS DECLARATION

/**
*   Encapsulates an active object used in controlling notes.
*
*  @lib phoneclient.lib
*  @since 9.1
*/
NONSHARABLE_CLASS( CPhCltUssdNoteController ) : public CActive
    {
    public:  // Constructor and destructor
        
        /**
        * Two-phased constructor.
        * 
        * @param aCallBack a callback to be notified events from note controller.
        * @param aPriority active object priority.
        * @return Pointer to created CPhCltUssdImp instance.
        */
        static CPhCltUssdNoteController* NewL( 
                MPhCltUssdNoteControllerCallBack& aCallBack,
                TInt aPriority );
        
        /**
        * Destructor.
        */
        ~CPhCltUssdNoteController();

        
    public:
        
        // Shows information note.
        void ShowInformationNoteL( TInt aResourceId );
        
        // Shows global information note.
        void ShowGlobalInformationNoteL( TInt aResourceId );
        
        // Shows global confirmation note.
        void ShowGlobalConfirmationNoteL( TInt aResourceId );
        
        // Shows global wait note.
        void ShowGlobalWaitNoteL( TInt aResourceId, TInt aSoftkeyResourceId = 0 );
        
        // Destroys global wait note.
        void DestroyGlobalWaitNote();
        
    private: 
        
        /**
        * C++ constructor.
        */
        CPhCltUssdNoteController( 
            MPhCltUssdNoteControllerCallBack& aCallBack,
            TInt aPriority );
        
        // Symbian 2nd phase constructor.
        void ConstructL();
        
        // The note type enumeration, used inside the class.
        enum TPhCltUssdNoteType
            {
            EPhCltUssdInformationNote,
            EPhCltUssdGlobalInformationNote,
            EPhCltUssdGlobalConfirmationNote
            };

        // Shows the note of given type. 
        void ShowNoteL( TPhCltUssdNoteType aType, TInt aResourceId );
        
        // Loads the resource file.
        void LoadResourceFileL();
        
        // From base classes
       
        /*
        * From CActive. Called when dialog is dismissed by soft key.
        */
        void RunL();

        /**
        * From CActive. 
        */
        void DoCancel();


    private:    // Data
        
        // Buffer for messages.
        HBufC* iMessageBuffer;
        
        // Global wait note.
        CAknGlobalNote* iGlobalWaitNote;
        
        // Id of an active wait note.
        TInt iWaitNoteId;
        
        // The callback to be notified when wait note is dismissed.  
        MPhCltUssdNoteControllerCallBack& iCallBack;

        // Resource loader.
        RConeResourceLoader* iResourceLoader;
    };

#endif // CPHCLTUSSDNOTECONTROLLER_H

// End of File
