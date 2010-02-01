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
*  This is the application main view. Shows all the topics which
*  has been received and their status: subscribed, hotmarked, new messages
*
*
*/


#ifndef __CCBSTOPICLISTVIEW_H
#define __CCBSTOPICLISTVIEW_H

// INCLUDES
#include <aknview.h>
#include <CbsCommon.h>
#include "MCbsUiTopicMonitorObserver.h"
#include "CbsUiDebug.h"
	
// CONSTANTS  
const TInt KNumberOfStandardTopics = 17;

 
// FORWARD DECLARATIONS

class CSelectionList;
class CListBoxNumbers;
class CCbsUiTopicListViewContainer;
class MCbs;
class CAknNavigationDecorator;
class CAknNavigationControlContainer;

// CLASS DECLARATION

/**
* This is the application main view. Shows all the topics which
* has been received and their status: subscribed, hotmarked, new messages
*  
*/
class CCbsUiTopicListView : public CAknView, 
                            public MCbsUiTopicMonitorObserver
    {
    private: // Data declaration

        enum TTopicListViewFocusState
            {
            ESetFocusToCurrentHandle,
            ESetFocusToFirstItem,
            ESetFocusToNewestItem,
            ENoFocusUpdate 
            };

    public: // Constructors and destructor

        /**
        * Creates an instance of the class.
        *
        * @param aServer It is the server.
        * @return It returns a pointer to the created class.
        */
        static CCbsUiTopicListView* NewL( MCbs& aServer );
        
        /**
        * Destructor.
        */
        virtual ~CCbsUiTopicListView();
        
    public: // New functions

        /**
        * Calls the server to add or change a topic. Which is called
        * depends on the type of the dialog.
        *
        * @param aType is it a edit or add query. True is add query
        * @param aNumber topic number to add or change
        * @param aName topic name to add or change
        * @return error code if something goes wrong
        */
        TInt EditTopic( 
            TBool aType, 
            TCbsTopicNumber aNumber, 
            const TCbsTopicName aName );

        /** 
        * Adds a topic with specified number and name.
        *
        * @param  aNumber It is the number of the topic.
        * @param  aName It is the name of the topic.
        * @return a error code if the addition fails
        */
        TInt AddTopic( 
            const TCbsTopicNumber& aNumber, 
            const TCbsTopicName& aName );       

        /**
        * Marks topic which is focused. This should be called only when 
        * the Options menu is created and when the Ok (selection) key
        * has been pressed. Focus behaviour is based on this.
        */
        void MarkCurrentHandle();
        
    public: // From MCbsUiTopicMonitorObserver

        /**
        * Topic list or topic messages are changed.
        *
        * @param aEvent reason for event
        * @param aTopicNumber Topic number, which caused the event
        */
        void TopicListChangedL(
            TCbsTopicListEvent aEvent, TCbsTopicNumber aTopicNumber );
     
    public: // from CAknView
        
        /**
        * Returns the identifier of the view.
        *
        * @return It returns the identifier.
        */
        TUid Id() const;        
    
        /**
        * Handles the options menu commands.
        *
        * @param aCommand command to be executed
        */
        void HandleCommandL( TInt aCommand );

    public: // New function
        /**
        * Handle changes in container when rect changes.
        */
        void HandleRectChangeInContainer();

    private: // from CAknView
        
        /**
        * Activates the view.
        *
        * @param aPrevViewId
        * @param aCustomMessageId
        */
        void DoActivateL(
            const TVwsViewId& aPrevViewId, 
            TUid aCustomMessageId, 
            const TDesC8& aCustomMessage );
        
        /**
        * Deactivates the view.
        */
        void DoDeactivate();     

        /**
        * Checks new topic count, if it has been changed after
        * application switch.
        */
        void ViewActivatedL(
            const TVwsViewId& aPrevViewId,
            TUid aCustomMessageId,const 
            TDesC8& aCustomMessage );

        void ViewDeactivated();
      
    private: // MEikMenuObserver 

        /**
        * Sets the menu items according to the state of the topic, which
        * is focused on. Used when options menu is opened. Marks also 
        * a current topic handle.
        *
        * @param aResourceId resource id of the menu
        * @param aMenuPane used to modify menu dynamically
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );         

    private: // New functions

        /**
        * Sets current topic number according to aTopicNumber.
        *
        * @param aTopicNumber contains current topic number.
        */
        void SetCurrentTopicNumber( const TDesC8& aTopicNumber );

        /**
        * Sets current topic number according to aTopicNumber.
        *
        * @param aTopicNumber new current topic number.
        */
        void SetCurrentTopicNumber( const TCbsTopicNumber aTopicNumber );

        /**
        * Updates the topics information.
        */
        void ReloadTopicsL();

        /**
        * Prepares container for reloading.
        */
        void PrepareReloading();

        /**
        * Reloads topics from the server.
        */
        void ReloadL();

        /**
        * Ends reloading.
        */
        void EndReloadingL();

        /**
        * Sets focus after reload. Should be called after ReloadTopicsL().
        *
        * @param aFocusState focus is set according to this
        */
        void SetFocusAfterReload( TTopicListViewFocusState aFocusState );

        /**
        * Sets focus to aIndex.
        *
        * @param aIndex new focused index
        */
        void SetFocusToTopic( TInt aIndex );

        /**
        * Updates the focus according to Selection Service after deletion.
        *
        * @param aDeletedTopic Calculates the new focus according to deleted
        *                      topic.
        */
        void UpdateCurrentTopicAfterDeletion( TCbsTopicNumber aDeletedTopic );
        
        /**
        * Returns the total amount of topics.
        *
        * @return It is the amount of topics.
        */
        TInt TotalTopics() const;                      
       
        /**
        * Requests the view to move to the previous topic.
        */
        void MoveToPreviousTopic();
   
        /**
        * Writes a CBS to title pane.
        */
        void UpdateTitlePaneL() const;

        /**
        * Writes to navi pane a right message:
        * No new messages
        * One new message
        * %N new messages
        */
        void UpdateNaviPaneL();

        /**
        * Gets resouce id for text to be displayed.
        *
        * @param aUnreadMessages Number of unread messages.
        * @return resource id for text
        */ 
        TInt GetInfoResourceId( TInt aUnreadMessages ) const;

        /**
        * Creates a new decorated label to navi pane.
        *
        * @param aResourceId text resource id.
        * @param aUnreadMessages number of unread messages
        */
        void CreateNewLabelL( TInt aResourceId, TInt aUnreadMessages );

        /**
        * Launches an edit or add topic query dialog.
        *
        * @param aType          type of the query. 
        *                       ETrue is add and EFalse is edit query.
        * @param aTopicName     name of the topic
        * @param aTopicNumber   number of the topic
        */
        void ShowTopicQueryL( 
            TBool aType, 
            TDes& aTopicName, 
            TDes& aTopicNumber );

        /**
        * Launchs the edit topic query with the right parameters.
        */
        void ShowEditTopicQueryL();

        /**
        * Checks if there has come new topics while this function was
        * last called. Shows a note how many new topics has been added,
        * since last been in TopicListView.
        */
        void CheckNewTopicCountL();

        /**
        * Returns the new topics count from server.
        * 
        * @return new topic count
        */
        TInt GetNewTopicsCount();

        /**
        * Changes the topic name and number to the specified.
        *
        * @param  aHandle It is the handle to the topic.
        * @param  aNumber It is the new number of the topic.
        * @param  aName It is the name of the topic.
        * @return error code in case fail
        */
        TInt UpdateTopic( 
            const TCbsTopicNumber& aOldNumber,
            const TCbsTopicNumber& aNumber,
            const TCbsTopicName& aName );

        /**
        * Finds topic index by topic number.
        *
        * @param aIndex it will contain the topic index
        * @param aTopicNumber topic number
        * @return KErrNone if no errors
        */
        TInt FindTopicIndexByNumber( 
            TInt& aIndex, 
            const TCbsTopicNumber& aTopicNumber );

        /**
        * Activates the topic view.
        */
        void ActivateTopicViewL() const;

        /**
        * Activates settings view.
        */
        void ActivateSettingsViewL() const;

        /**
        * Activates a add from index dialog.
        */
        void ActivateAddFromIndexViewL();

        /**
        * Activates an idle object to show an information note. 
        * Used in call back function.
        *
        * @param aNewTopics count of new topics
        * @return always KErrNone
        */
        static TInt ActivateBackgroundNoteL( TAny* aNewTopics );
        
    private:
        
        /**
        * It is type for topic handles array.
        */
        typedef CArrayFixFlat< TCbsTopicNumber > TCbsUiTopicNumberList;
        
        /**
        * Constructor.
        * 
        * @param aServer connection to the server
        */
        CCbsUiTopicListView(MCbs& aServer);
        
        /**
        * Finalizes the construction.
        *
        * Note that it is assumed that the server exists for the life 
        * time of the class.
        */
        void ConstructL();
     
    private: // Data        

        // It is the control for the view.
        CCbsUiTopicListViewContainer* iContainer;
        
        // It is the label in status pane.
        CAknNavigationDecorator* iDecoratedLabel;        
   
        // Pointer to server session. Owned by UiApp
        MCbs& iServer;        
       
        // It is the array that contains the topic numbers.
        TCbsUiTopicNumberList* iTopicNumberList;
       
        // Handle that is focused.        
        TCbsTopicNumber iCurrentTopicNumber;

        // For updating information about messages to navi pane.
        CAknNavigationControlContainer* iNaviPane;

        // iFocusState is set in HandleCommandL and focus is set in 
        // TopicListChangedL.
        TTopicListViewFocusState iFocusState;

        // To see wheather it is needed to update navi pane.
        // if ETrue: navi pane is not updated
        TBool iAddFromIndexDialogIsActive;

        // for showing background note
        CIdle* iBackgroundNote;

        // Count of new topics.
        TInt iNewTopicsCount;


    };
    
    
#endif // __CCBSTOPICLISTVIEW_H
    
    
// End of File
