/*
* Copyright (c) 2002 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A text query where empty query can be approved.
*
*/


#ifndef VMDIALOG_H
#define VMDIALOG_H

// INCLUDES
#include <AknQueryDialog.h>

// CLASS DECLARATION

/**
*  Text query dialog
*
*  @lib vmbx.lib
*  @since 
*/
class CVmTextQueryDialog : public CAknTextQueryDialog
    {
    public: // Constructors and destructor
    
        /**
        * Two-phased constructor.
        */
        static CVmTextQueryDialog* NewL( TDes& aDataText, const TDesC& aPrompt,        
                                                 const TBool aSearchEnabled );
                                                 
        /**
        * Destructor.
        */                                                 
        virtual ~CVmTextQueryDialog();

    public:// Functions from base classes

        /**
        * From CCoeControl Key event handler
        * @since
        * @param aKeyEvent The key event
        * @param aType The type of key event
        * @return Response of the key event handling
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                         TEventCode aType );

    public:// New method

        /**
        * Dissmiss the dialog with EAknSoftkeyCancel
        * @since 5.0
        */
        void DismissQueryDlg();

    protected: // Functions from base classes

        /**
        * From CEikdialog This function is called by the EIKON dialog framework
        * just before the dialog is activated, after it has called
        * PreLayoutDynInitL() and the dialog has been sized. 
        * Overridden method to enable accepting an empty query
        * @since
        */  
        void PreLayoutDynInitL();

        /**
        * From MAknQueryControlObserver Gets called when editor sends
        * state event. Overridden method to enable accepting an empty query
        * @since
        * @param aQueryControl  Pointer to query control which sent the event  
        * @param aEventType  Type of the event  
        * @param aStatus  Editor validation status
        * @return ETrue if success
        */
        TBool HandleQueryEditorStateEventL( CAknQueryControl* aQueryControl,
                                            TQueryControlEvent aEventType,
                                            TQueryValidationStatus aStatus );

    private:
    
        /**
        * C++ default constructor.
        */
        CVmTextQueryDialog( TDes& aDataText, const TBool aSearchEnabled );

    private:  // Data
    
        // Flag to allow search
        TBool iSearchEnabled;
    };



// CLASS DECLARATION

/**
*  Query dialog for sim
*  added to fix JKAN-6KU5C2
*
*  @lib vmbx.lib
*  @since
**/
class CVMSimQueryDilaog:public CAknQueryDialog
    {
    
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CVMSimQueryDilaog* NewL();                
        
        /**
        * Destructor.
        */
        virtual ~CVMSimQueryDilaog();

    public: // New functions
                    
        /**
        * Sets query caption on the screen asking for confirmation to copy voice
        * mailbox number from sim. 
        *
        * @since 3.0
        * @param aPrompt query heading to display on dialog.
        **/
        void SetVMSimPromptL( const TDesC& aPrompt );
        
        /**
        * Destroys the query dialog.
        *
        * @since 3.0
        **/
        void VMSimDismissDialogL();

    private:

        /**
        * C++ default constructor.
        */
        CVMSimQueryDilaog();
    
    };

#endif  // VMDIALOG_H
            
// End of File
