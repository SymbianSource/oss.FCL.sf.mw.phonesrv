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
*              : CSsListQueryDialog class.
*
*/



#ifndef CSSLISTQUERYDIALOG_H
#define CSSLISTQUERYDIALOG_H

//  INCLUDES
#include <aknlistquerydialog.h>

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
*  List query class for service selector
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( CSsListQueryDialog )
        : public CAknListQueryDialog
    {
    public:  // Constructors and destructor
      
        /**
        * C++ constructor.
        * @param aSelf pointer to itself.
        * @param aIndex Index for user selection.
        */
        CSsListQueryDialog( CEikDialog** aSelf, TInt& aIndex );
       
        /**
        * Destructor.
        */
        virtual ~CSsListQueryDialog();

    public: // New functions

        /**
        * Executes the list query.
        * @param aResourceId Resource for the list query.
        * @param aItemTextArray Item array. Ownership not
        *                       trasferred.
        * @param aIcons Icon array. Ownership transferred.
        * @return Id of the key. 0 if list canceled.
        */     
        TInt ExecuteListLD( TInt aResourceId, 
                            MDesCArray* aItemTextArray,
                            CArrayPtr<CGulIcon>* aIcons );

        
    private:    // Data
        
        // Pointer to itself.
        CEikDialog** iSelf;

    };

#endif      // CSSLISTQUERYDIALOG_H
            
// End of File
