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
* Description: 
*     This is a dialog which is used to shows different language options in the
*     settings view
*
*
*/


#ifndef __CCbsUiListQueryDialog_h
#define __CCbsUiListQueryDialog_h

//  INCLUDES
#include <aknlistquerydialog.h>


// CLASS DECLARATION

/**
* This is a dialog, which can be initialized with selected items.
*/
class CCbsUiListQueryDialog : public CAknListQueryDialog
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        *
        * @param aIndexArray Sets selection indexes and will contain
        *                    user changed indexes.
        */
        CCbsUiListQueryDialog( CArrayFix<TInt>& aIndexArray );

    private: // From CEikDialog
        
        /**
        * Sets selection indexes to list box.
        */
        void PostLayoutDynInitL();

        /**
        * If the user has pushed Ok, the dialog Stores the selected 
        * values in to the server.
        *
        * @param aKeyCode key code
        * @return ETrue if dialog is closed
        */
        TBool OkToExitL( TInt aKeycode );

    private: // Data

        // contains selected indexes
        CArrayFix<TInt>& iIndexArray;

    };

#endif      // __CCbsUiListQueryDialog_h
            
// End of File
