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
* Description: 
*   Dialog which shows topics, which are not yet 
*   in the topic list view. Enables topics adding manually.
*
*
*/


#ifndef __CCBSADDFROMINDEXDIALOG_H
#define __CCBSADDFROMINDEXDIALOG_H

// INCLUDES
#include <akndialog.h>
#include <CbsCommon.h>
#include "CbsUiConstants.h"
 

// FORWARD DECLARATIONS
class MCbs;
class CEikColumnListBox;
class CAknNavigationDecorator;


// CLASS DECLARATION


/**
*  Class CCbsUiAddFromIndexDialog is used to add topics. Topics
*  are shown in listbox. User can select topics which will be added.
*/
class CCbsUiAddFromIndexDialog: public CAknDialog
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        *
        * @param aServer Reference to cbs server.
        */
        CCbsUiAddFromIndexDialog( MCbs& aServer );

        /**
        * Destructor.
        */
        virtual ~CCbsUiAddFromIndexDialog();

    private: // From CEikDialog

        /**
        * Creates the controls, which are shown in this dialog.
        */
        void PreLayoutDynInitL();

        /**
        * Checks if the user has selected the ok softkey. In that case,
        * the marked topics are send to AddTopicL member function.
        *
        * @param aKeycode pressed key code
        * @return ETrue, to close the dialog
        */
        TBool OkToExitL( TInt aKeycode );
        
    private: // New functions
        
        /**
        * Parses IndexNumber from TopicInfo.
        *
        * @param aTopicInfo Topic information 
        * @param aIndexNumber the pared index number
        * @return none
        */
        void GetIndexNumber( 
            const TCbsTopicInfo& aTopicInfo , 
            TBuf<KTopicNumberWidth>& aIndexNumber) const;

        /**
        * Adds a Topic to the server.
        *
        * @param aTopicInfo Topic information         
        * @return error code
        */
        TInt AddTopicL( const TCbsTopicInfo aTopicInfo );

        /**
        * Sets the icons to the listbox.        
        */
        void SetIconsL() const;

        /**
        * Sets the topics to the listbox. Set Topic reads topic informations
        * from cbsserver.
        */
        void SetTopicsL();

        /**
        * Creates a title pane.
        */
        void CreateAddFromIndexTitlePaneL() const;

    private: // Data

        // List of handles
        typedef CArrayFixFlat< TCbsTopicInfo > TCbsTopicInfoHandleList;

        // pointer to a handle list
        TCbsTopicInfoHandleList* iHandles;
    
        // Server
        MCbs& iServer;

        // ListBox
        CEikColumnListBox* iListBox;                
        
        /// Own: Navi pane control
        CAknNavigationDecorator* iNaviDecorator;
    };

#endif      // __CCBSADDFROMINDEXDIALOG_H
            

// End of File
