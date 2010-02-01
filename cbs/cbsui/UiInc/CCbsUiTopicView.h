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
* Description:  Header file for the CCbsUiTopicView class.
*
*
*/


#ifndef __CCBSUITOPICVIEW_H
#define __CCBSUITOPICVIEW_H

//  INCLUDES
#include <aknview.h>
#include <CbsCommon.h>
#include <AknNaviDecoratorObserver.h>
#include "MCbsUiTopicMonitorObserver.h"
#include "MSaveMessageController.h"
#include "CbsUiDebug.h"

  
//  FORWARD DECLARATIONS

class CCbsUiTopicViewContainer;
class MCbs;
class CAknNavigationDecorator;
class CAknNavigationControlContainer;

//  CLASS DEFINITIONS 

/**
* For showing message in one topic.
*/
class CCbsUiTopicView : public CAknView, 
                        public MCbsUiTopicMonitorObserver,
                        public MSaveMessageController,
                        public MAknNaviDecoratorObserver
    {
    private: // Data declaration

        enum TTopicViewFocusState 
            {
            ESetFocusToCurrentHandle, 
            ESetFocusToFirstItem, 
            ESetFocusToNewestMessage, 
            ENoFocusUpdate 
            };

    public: // Constructors and destructor        
      
        /**
        * Creates instance of the class.
        *    
        * @param aServer reference to the server
        * @return It is the created instance.
        */
        static CCbsUiTopicView* NewL( MCbs& aServer );
        
        /**
        * Destructor.
        */
        ~CCbsUiTopicView();
        
    public: // New functions        
     
        /**
        * Marks the current handle. This should be called only when 
        * the Options menu is created and when the Ok (selection) key
        * has been pressed.
        */
        void MarkCurrentlyFocusedHandle();

        /**
        * Handle changes in container when rect changes.
        */
        void HandleRectChangeInContainer();

    public: // From MCbsUiTopicMonitorObserver

        /**
        * Topic list or topic messages are changed.
        *
        * @param aEvent reason for event
        * @param aTopicNumber Topic number, which caused the event
        */
        void TopicListChangedL(
            TCbsTopicListEvent aEvent, TCbsTopicNumber aTopicNumber );

    public: // From MSaveMessageController

        /**
        *   Saves a message (the saved message won't be deleted to make 
        *   room for new messages).
        *
        *   Return codes:
        *   KErrNone            Message is saved.
        *   KErrGeneral         Message not saved -- total maximum of saved 
        *                       messages reached.
        *   KErrNotFound        Message not saved -- no message associated
        *                       with the given handle.
        *   KErrDiskFull        Message not saved -- FFS out of space.
        *
        *   @param  aHandle     Handle to the message to be saved.
        *   @return             Return code.
        */
        TInt SaveMessage( const TCbsMessageHandle& aHandle );

    private: // from MEikMenuObserver
    
        /**
        * Sets the options menu according to the current message.
        *
        * @param aResouceId Id of the resouce.
        * @param aMenuPane Pointer to the menu pane.
        */ 
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

        /**
        * Overrides MEikMenuObserver function SetEmphasis. Default 
        * implementation has been copied here. 
        * 
        * Checks if the menu pane is showing. If the menu pane is not
        * showing, message is unlocked, because it has been locked when
        * the menu pane was created. 
        * 
        * @param aMenuControl pointer to the menu control
        * @param aEmphasis ETrue if menu is showing; otherwise EFalse
        */
    	void SetEmphasis( CCoeControl* aMenuControl,TBool aEmphasis );
    
    public: // from CAknView
        
        /**
        * Returns the view id.
        */
        TUid Id() const;        

        /**
        * To catch which command is selected before menu pane is closed.
        *
        * @param aCommand command to be processed
        */
        void ProcessCommandL( TInt aCommand );
       
        /**
        * Processes the command.
        *    
        * @param aCommand It is the command to be processed.
        */
        void HandleCommandL( TInt aCommand );         
       
    protected: // from CAknView
        
        /**
        * Activates the view.
        *
        * @param aPrevViewId previous view id
        * @param aCustomMessageId id of the custom message
        * @param aCustomMessage message data
        */
        void DoActivateL(
            const TVwsViewId& aPrevViewId, 
            TUid aCustomMessageId, const TDesC8& aCustomMessage );
        
        /**
        * Deactivates the view.
        */
        void DoDeactivate();        

    private: // New functions

        /**
        * Sets focus to message, according to aIndex.
        *
        * @param aIndex Index of the focused message.
        */
        void SetFocusToMessage( TInt aIndex );
       
        /**
        * Reloads the messages.
        */
        void ReloadMessagesL();        
      
        /**
        * Return the total amount of messages. ("loaded").
        *
        * @return total amount of messages
        */
        TInt TotalMessages() const;

        /**
        * Locks the message in the server. Only one message can locked
        * at same time. When new message is locked, old message is unlocked.
        *
        * @param aHandle Message handle to be locked.
        */
        TInt LockMessage( TCbsMessageHandle aHandle );

        /**
        * Unlocks the message from the server.
        */
        void UnLockMessage();

        /**
        * Updates the status pane.
        */
        void UpdateTitlePaneL( const TCbsTopic& aTopic ) const;

        /**
        * Updates the title pane.
        */
        void UpdateNaviPaneL( const TCbsTopic& aTopic );
        
        /**
        * Shows the message in a dialog.
        */
        void ShowMessageL();

        /**
        * Finds message index by handle.
        *
        * @param aIndex it will contain the topic index
        * @param aMessageHandle handle to the message
        * @return KErrNone if handle exists
        */
        TInt FindMessageIndexByHandle(
            TInt& aIndex,
            const TCbsMessageHandle& aMessageHandle );
        
        /**
        * Loads all message titles from the server in one topic and offers
        * those to the container.
        *
        * @param aTopicNumber Message headers will be loaded from this topic.
        */
        void LoadAllMessageTitlesL( TCbsTopicNumber aTopicNumber );

        /**
        * Prepares container for reloading.
        */
        void PrepareReloading();

        /**
        * Sets focus in the list box according to current topic number.
        * It also set the focus according to the view focus state.
        *
        * @param aFocusState indicates where to put the focus
        */
        void SetFocusAfterReloading( TTopicViewFocusState aFocusState );

        /**
        * Sets a new current handle according to aDeletedHandle.
        *
        * @param aDeletedHandle A handle to message which has been deleted.
        */
        void SetNewCurrentHandle( TCbsMessageHandle aDeletedHandle );

        /**
        * Sets current topic number according to aToPrev
        * 
        * @param aToPrev Sets current topic number to previous if ETrue
        */
        void SetCurrentTopicNumber( TBool aToPrev );

        /**
        * Sets current topic number to aTopicNumberDes. It is used in
        * view activation.
        *
        * @param aTopicNumberDes new current topic number
        */
        void SetCurrentTopicNumberAndMessageHandle( const TDesC8& aTopicNumberDes );

        /**
        * Activates message view with the aMsgHandle.
        *
        * @param aMsgHandle A message handle to be activeted.
        */
        void ActivateMsgViewL( TCbsMessageHandle aMsgHandle ) const;

        /**
        * Activates topic list view with topic number.
        *
        * @param aTopicNumber sets the topic number in topic list view.
        */
        void ActivateTopicListViewL( TCbsTopicNumber aTopicNumber ) const;

        /**
        * Checks if it is possible to move to previous, next
        * or both directions.
        *
        * @param aToPrevious ETrue if it is possible to move to previous topic
        * @param aToNext ETrue if it is possible to move to next topic
        * @param error code
        */
        TInt CheckIfCanMove( TBool& aToPrevious, TBool& aToNext );

        /**
        * Activates the list box after changes. List box is owned by 
        * the container.
        */
        void ActivateListBoxL();
        
        /**
        * Set the middle soft key text.
        *
        * @param aResourceId    Resource to be read (text label)
        * @param aCommandId     Command ID for MSK command
        */
        void SetMiddleSoftKeyLabelL( const TInt aResourceId, const TInt aCommandId );
        
        /**
        * Removes MSK command mappings.       
        */
        void RemoveCommandFromMSK();

    private:
        
        /**
        * It defines the type for message handle lists.
        */
        typedef CArrayFixFlat< TCbsMessageHandle > TCbsUiMessageHandleList;       
      
        /**
        * Constructor.
        *
        * @param aServer reference to the server
        * @param aApplication reference to the application
        */
        CCbsUiTopicView( MCbs& aServer );
        
        /**
        * Finalizes the construction.
        */
        void ConstructL(); 
        
    private: // From MAknNaviDecoratorObserver
        
        /**
         * Method for handling touch events on navigator (navi pane)
         * 
         * @param aEventID Id of the event.
         */
        void HandleNaviDecoratorEventL( TInt aEventID );

    private: // Data
       
        // container to the CCbsUiTopicView
        CCbsUiTopicViewContainer* iContainer; 
        
        // navi pane variables
        CAknNavigationControlContainer* iNaviContainer;
      
        // navi pane
        CAknNavigationDecorator* iDecoratedLabel;
        
        // navi pane
        CAknNavigationDecorator* iPreviousDecoratedLabel;
     
        // reference to server
        MCbs& iServer;
        
        // array for message handles
        TCbsUiMessageHandleList* iHandles;

        // handle to a message, which is focused on.
        TCbsMessageHandle iCurrentHandle;

        // Topic number, which this view is currently handling.
        TCbsTopicNumber iCurrentTopicNumber;

        // If this is ETrue, message can be unlocked and there fore
        // deleted. It is set EFalse in ProcessCommandL, ETrue
        // in DoActivateL and in HandleCommandL.
        TBool iCanUnlock;
        
    };
    
    
#endif // __CCBSUITOPICVIEW_H
    
// End of File
