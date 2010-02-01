/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the
*              : CSsNoteDialog class.
*
*/



#ifndef CSSNOTEDIALOG_H
#define CSSNOTEDIALOG_H

//  INCLUDES
#include <aknnotedialog.h>


// CONSTANTS
//None

// MACROS
//None

// DATA TYPES
//None.

// FUNCTION PROTOTYPES
//None

// FORWARD DECLARATIONS
//None

// CLASS DECLARATION

/**
*  Note dialog class for service selector
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( CSsNoteDialog )
        : public CAknNoteDialog
    {
    public:  // Constructors and destructor
      
        /**
        * C++ constructor.
        * @param aSelf pointer to itself.
        */
        CSsNoteDialog( CEikDialog** aSelf );
       
        /**
        * Destructor.
        */
        virtual ~CSsNoteDialog();
        
        /**
        * From CCoeControl
        * Handles key events. 
        * @param aKeyEvent Key event details.
        * @param aType Type of event.
        * @return Indicates is or not the key event was used
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                     TEventCode aType ); 

    public: // New functions

        /**
        * Runs a note dialog.
        * @param aResourceId Resource for the note text.
        */     
        void RunDialogLD( TInt aResourceId );
        
    private:    // Data
        
        // Pointer to itself.
        CEikDialog** iSelf;

    };

#endif      // CSSNOTEDIALOG_H
            
// End of File
