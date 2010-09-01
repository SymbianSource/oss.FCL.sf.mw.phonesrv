/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Active object wrapper to handle popup note.
*
*/

#ifndef __NETWORKNOTIFIER_WRAPPER_H__
#define __NETWORKNOTIFIER_WRAPPER_H__

// SYSTEM INCLUDES
#include <e32base.h>
#include <f32file.h>

// INCLUDES
#include <nwnotifier_0x20026826.rsg>

// FORWARD CLASS DECLARATIONS
class CAknGlobalConfirmationQuery;

// CONSTANTS
                           
_LIT( KResFile, "\\resource\\apps\\NWNotifier_0x20026826.rsc" );   

    
// OBSERVER DECLARATION
class MNoteWrapperObserver
    {
    public:

        virtual void PopupNoteClosed( ) = 0;
    };

// CLASS DECLARATION
/** 
 * This Class provides implementation for handling the error notes
 */

class CNetworkNotifierNoteWrapper : public CActive
    {
    public:
    
        /**
        * Two-phased constructor.
        */
        static CNetworkNotifierNoteWrapper* NewL( MNoteWrapperObserver& aObserver,
                                                  TInt aResID );
        
        /**
        * Destructor.
        */
        ~CNetworkNotifierNoteWrapper();
        
        /**
        * Monitor inserts the latest error notification err# from ETEL to this stack.
        * @param aResID Resource id of the note to be added to the notestack.
        */
        void AppendNote( const TInt aResID );
        
        
    private:
        
        /**
        * C++ default constructor.
        * @param aObserver Reference to observer class. CNWRegistrationStatus is listening.
        * @param aResID Resource id number to be inserted into the notestack.
        */
        CNetworkNotifierNoteWrapper( MNoteWrapperObserver& aObserver,
                                     TInt aResID );
           
        /**
        * Second-phase constructor.
        * 
        */
        void ConstructL();
        
        /**
        * Informs object that asyncronous request is ready.
        * 
        */
        void RunL();
        
        /**
        * Cancels asyncronous request(s).
        * 
        */
        void DoCancel();
        
        /**
        * From CActive, RunError.
        * Processes any errors.
        * @param aError The leave code reported.
        * @result return KErrNone if leave is handled.
        */
        TInt RunError( TInt aError );
        
        /**
        * Reads the localized error strings from resource. 
        * @return The localized error string.
        */
        HBufC* ReadResourceLC();
              
        /**
        * Displays the error note passed as a parameter.
        * @param aNote Pointer to the note.
        */
        void DisplayPopupNoteL( HBufC* aNote );

    private:    // Data
        
        // Handle to file server
        RFs iFs;
        
        // Note
        CAknGlobalConfirmationQuery* iGlobalQuery; 
        
        // Callback receiver
        MNoteWrapperObserver& iObserver;
         
        // Error note stack<resourceID>
        RArray<TInt> iErrNotes; 
    };

#endif // __NETWORKNOTIFIER_WRAPPER_H__
