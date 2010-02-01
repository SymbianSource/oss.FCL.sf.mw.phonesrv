/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Query dialog for handling key events.
*
*/

/*
*/

#ifndef CPSUIQUERYDIALOG_H
#define CPSUIQUERYDIALOG_H

//  INCLUDES
#include <AknQueryDialog.h>

NONSHARABLE_CLASS ( CPsuiQueryDialog ) : public CAknQueryDialog
    {
    public:  // Constructors and destructor

        /**
        * Symbian OS two-phased constructor.
        * 
        * @return Returns CPsuiQueryDialog -pointer.
        */
        static CPsuiQueryDialog* NewL( CAknQueryDialog::TTone aTone );

        /**
        * Constructor
        */
        CPsuiQueryDialog( CAknQueryDialog::TTone aTone );  
        
        /**
        * Destructor.
        */
        virtual ~CPsuiQueryDialog();  

        
    protected:

        /**
        * From CCoeControl  Handle key events. When a key event occurs, 
        *                   CONE calls this function for each control on the control stack, 
        *                   until one of them returns EKeyWasConsumed to indicate that it processed the key event.  
        * @param aKeyEvent  The key event.
        * @param aType      The type of the event: EEventKey, EEventKeyUp or EEventKeyDown.
        * @return           Indicates whether or not the key event was used by this control.
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

        /**
        * From CEikdialog   This function is called by the EIKON framework 
        *                   if the user activates a button in the button panel. 
        *                   It is not called if the Cancel button is activated, 
        *                   unless the EEikDialogFlagNotifyEsc flag is set.
        * @param aButtonId  The ID of the button that was activated
        * @return           Should return ETrue if the dialog should exit, and EFalse if it should not.
        */
        TBool OkToExitL( TInt aCommand );        
    };

#endif // CPSUIQUERYDIALOG_H   
            
// End of File
