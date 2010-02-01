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
*              : CSsUiUtilities class.
*
*/



#ifndef CSSUIUTILITIES_H
#define CSSUIUTILITIES_H

// INCLUDES
#include <e32base.h>
#include <bamdesca.h>
#include <gulicon.h>

// FORWARD DECLARATIONS
class CEikDialog;
class CCoeEnv;

// CLASS DECLARATION

/**
*  This class provides UI utilities for service selector
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( CSsUiUtilities )
        : public CBase
    {
    public:  // Constructors and destructor
      
        /**
        * Two-phased constructor.
        */
        static CSsUiUtilities* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CSsUiUtilities();

    public:

        /**
        * Shows a information note.
        * @param    aResource Resource id for the note text.
        */
        void ShowInformationNoteL( TInt aResource );
        
        /**
        * Shows a list query.
        * @param    aIndex Index of the selection.
        * @param    aResource Resource id for the query.
        * @param    aItemTextArray List item array. Ownership
        *                          not trasferred.
        * @param    aIcons Icon array. Ownership transferred.
        * @return   ETrue if list item selected.
        */        
        TBool ShowListQueryL(
                TInt& aIndex,
                TInt aResource,
                MDesCArray* aItemTextArray,
                CArrayPtr<CGulIcon>* aIcons );
        
        /**
        * Loads default icon for service selection list.
        * @param    aIcons Default icon is added to the array.
        *                  Icon array takes ownership for 
        *                  the added icon.
        */        
        void LoadDefaultIconL( 
                CArrayPtr<CGulIcon>& aIcons );

    private:
        
        /**
        * Loads resources for the dialogs.
        */
        void LoadResourceL();
        
    private:
    
        /**
        * C++ default constructor.
        */
        CSsUiUtilities();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Pointer to current dialog.
        CEikDialog* iDlg; 
        
        // Control environment, not owned
        CCoeEnv*    iCoeEnv;
        
        // Resource file offset
        TInt        iResourceFileOffset;
                     
    };

#endif      // CSSUIUTILITIES_H
            
// End of File
