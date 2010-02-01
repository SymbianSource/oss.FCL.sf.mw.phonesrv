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
*     Implements the feature for investigating length of the input. Softkeys
*     are set based on this information.
*     
*
*/


#ifndef CSATUIWAITDIALOG_H
#define CSATUIWAITDIALOG_H

//  INCLUDES
#include <AknWaitDialog.h>

// FORWARD DECLARATION
class MSatUiAdapter;

// CLASS DECLARATION

/**
 *  Defines the SAT spesific data query class.
 */
class CSatUiWaitDialog : public CAknWaitDialog
    {
    public:  // Constructors and destructor

         /**
         * C++ default constructor.
         */
        CSatUiWaitDialog( 
            CEikDialog** aSelfPtr,
            MSatUiAdapter& aAdapter,                
            TBool aVisibilityDelayOff );

        /**
         * Destructor.
         */
        virtual ~CSatUiWaitDialog();

    public: // Override functions from base classes
    
        /**
         * From @c CEikDialog.
         * Handles a dialog button press for the specified button.
         * @param aButtonId The ID of the button that was activated
         * @return ETrue to validate and exit the dialog
         */
        TBool CSatUiWaitDialog::OkToExitL( TInt aButtonId );
        
    private:    // Data

        // Reference to adapter. Used to inform SAT Client if user 
        // presses Cancel during dialog is showing
        MSatUiAdapter& iAdapter;
    };

#endif      // CSATUIWAITDIALOG_H

// End of File
