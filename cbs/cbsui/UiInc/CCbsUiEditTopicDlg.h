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
* Description:  Header file for the CCbsUiEditTopicDlg class.
*
*
*/


#ifndef __CBS_UI_TOPIC_QUERY_H
#define __CBS_UI_TOPIC_QUERY_H

//  INCLUDES
#include <AknQueryDialog.h>  // CAknMultiLineDataQueryDialog


// FORWARD DECLARATIONS
class CCbsUiTopicListView;


// CLASS DECLARATION

/**
* Class CCbsUiEditTopicDlg implements a dialog for 
* Edit and Add topics.
*/
class CCbsUiEditTopicDlg : public CAknMultiLineDataQueryDialog
    {
    public:  // Constructors and destructor
        
        /** 
        * Two-phased constructor.
        *
        * @param aDataText topic name
        * @param aDataText2 topic number
        * @param aTopicListView for adding a topic
        * @param aType EFalse is Edit and ETrue is Add query
        */
        static CCbsUiEditTopicDlg* NewL(            
                    TDes& aDataText,
                    TDes& aDataText2,
                    CCbsUiTopicListView& aTopicListView,
                    TBool aType);
        
        /**
        * Destructor.
        */
        virtual ~CCbsUiEditTopicDlg();
   
    public: // From CAknNoteDialog
       
        /**
        * Adds or over writes topic. On error shows a error note.
        *
        * @param aButtonId button which has been selected
        */
        TBool OkToExitL( TInt aButtonId );

        // From CeikDialog
        /**
        * Handles changes required when switching to a specified line.
        *
        * @param aControlId The Id of the control being switched to.
        */
        void LineChangedL( TInt aControlId );

        /**
        * Handles a change to the control's resources.
        * @param aType A message UID value.
        */
        void HandleResourceChange( TInt aType );


	protected: // From CAknMultiLineDataQueryDialog

        /**
        * Changes left soft key to visible if number line is valid
        */
	    void UpdateLeftSoftKeyL();

    private: // New functions

        /**
        * Sets cursor in the end of number line.
        */
        void SetCursorAndFocusL();

    private: 

        /**
        * C++ constructor.
        *
        * @param aTopicListView for adding a topic
        * @param aType EFalse is Edit and ETrue is Add query
        */
        CCbsUiEditTopicDlg(
            CCbsUiTopicListView& aTopicListView,
            TBool aType );

        /**
        * Symbian OS constructor
        *
        * @param aDataText topic name
        * @param aDataText2 topic number
        */
        void ConstructL( TDes& aDataText, TDes& aDataText2 );

    private: // Data

        // to add or change a topic
        CCbsUiTopicListView& iTopicListView;

        // EFalse is Edit query and ETrue is Add query
        const TBool iType;

        // iUpdateLeftSoftKeyFirstTime is used when update first time Left soft key method.
        TBool iUpdateLeftSoftKeyFirstTime; 

    };

#endif      // __CBS_UI_TOPIC_QUERY_H
            
// End of File
