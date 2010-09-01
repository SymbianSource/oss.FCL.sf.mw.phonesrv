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


#ifndef CSATUITEXTQUERYDIALOG_H
#define CSATUITEXTQUERYDIALOG_H

//  INCLUDES
#include <AknQueryDialog.h>

// CLASS DECLARATION

/**
*  Defines the SAT spesific data query class.
*/
class CSatUiTextQueryDialog : public CAknTextQueryDialog
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CSatUiTextQueryDialog(TDes& aDataText, const TTone aTone,
            TInt aMinLength, TInt aMaxLength);

        /**
        * Destructor.
        */
        virtual ~CSatUiTextQueryDialog();

    public: // Functions from base classes

        /**
        * From CAknQueryDialog
        * @param aQueryControl 
        * @param aEventType 
        * @param aStatus 
        * @return 
        */
        TBool HandleQueryEditorStateEventL(CAknQueryControl* aQueryControl,
            TQueryControlEvent aEventType, TQueryValidationStatus aStatus);

    protected: // Functions from base classes

        /**
        * From CEikDialog, gets called when dialog initializations are done
        */
        void PostLayoutDynInitL();

        /**
        * From CEikDialog, gets called before initializations
        */
        void PreLayoutDynInitL();

        /**
        * From CAknDialog, Hash key is accepted
        */
   		TBool NeedToDismissQueryL( const TKeyEvent& aKeyEvent );

        /**
        * From CAknDialog, Hash key not ignored (workaround)
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                           TEventCode aType );


    private:    // Data

        //Minimum length of input
        TInt iTextMinLength;
    };

#endif      // CSATUITEXTQUERYDIALOG_H

// End of File
