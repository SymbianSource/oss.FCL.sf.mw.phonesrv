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
*   Class for viewing messages.
*
*
*/


#ifndef __CBSMSGVIEW_H
#define __CBSMSGVIEW_H

//  INCLUDES
#include <CbsCommon.h>
#include <aknview.h>
#include <AknNaviDecoratorObserver.h>
#include "MCbsUiTopicMonitorObserver.h"
#include "MSaveMessageController.h"
#include "CbsUiDebug.h"


// FORWARD DECLARATIONS
class CCbsUiTopicView;
class CCbsUiMsgViewContainer;
class CAknNavigationDecorator;
class CFindItemMenu;
class MCbs;
class CAknNavigationControlContainer;

// CLASS DECLARATION
/**
*	It is the class for viewing messages.
*/
class CCbsUiMsgView : public CAknView,
                      public MCbsUiTopicMonitorObserver,
                      public MSaveMessageController,
                      public MAknNaviDecoratorObserver                      
    {
    public: // Constructors and destructor   
        
        /**
        * Destructor.
        */
        ~CCbsUiMsgView();
        
        /**
        * Two-phased constructor.
        *
        * @param aServer Reference to cbs server.
        */
        static CCbsUiMsgView* NewL( MCbs& aServer );
        
    private: // From MEikMenuObserver

        /*
        * Adds find item and checks weather delete, keep or both of
        * them should be removed.
        *
        * @param aResourceId resource ID for identifying the menu pane being 
        *                    initialised.
        * @param aMenuPane the in-memory representation of the menu pane.
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
        
    private: // From MAknNaviDecoratorObserver
        
		/**
		* Method for handling touch events on navigator (navi pane)
		* 
		* @param aEventID Id of the event.
		*/
    	void HandleNaviDecoratorEventL( TInt aEventID );
        
    public: // From MEikCommandObserver
        
        /**
        * Closes context menu if it exists.
        *
        * @param aCommandId Command id, which has been pressed.
        */
        void ProcessCommandL( TInt aCommandId );
       
    public: // From CAknView

        /**
        * Returns view id.
        * 
        * @return View ID.
        */
        TUid Id() const;
        
        /**
        * Handles the command.
        *
        * @param aCommand menu command which user has selected from 
        *                 option menu
        */
        void HandleCommandL( TInt aCommand );        
        
        /**
        * Activates the message view. Actiovation depends on the message.
        * Three variations are: normal, immediate and 
        *                       soft notification messages.
        *
        * @param aPrevViewId Previous view id.
        * @param aCustomMessageId With this different mechanism of activations
        *                          are used. Immediate message uses 
        *                          KCbsImmediateMessageIdInt and normal message
        *                          uses KCbsUiCustomMessageId. If the message id
        *                          differ from these, the message is activated
        *                          by soft notification
        * @param aCustomMessage Message content.
        */
        void DoActivateL(
            const TVwsViewId& aPrevViewId, 
            TUid aCustomMessageId, 
            const TDesC8& aCustomMessage );
        
        /**
        * Deactivates the view and destroys needed objects.
        */
        void DoDeactivate();     

    public: // From MSaveMessageController, these are called from 
            // CbsUiUtility

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

    public:        

        /**
        * Sets the message information to navi pane. e.g. Message 1/3.
        */    
        void UpdateNaviPaneL();
        
        /**
        * When called, next message is displayed
        */
        void MoveToNextMessageL();
        
        /**
        * When called, previous message is displayed
        */
        void MoveToPreviousMessageL();
        
        /**
        * Topic list or topic messages are changed.
        *
        * @param aEvent reason for event
        * @param aTopicNumber topic number, which caused the event
        */
        void TopicListChangedL(
            TCbsTopicListEvent aEvent, TCbsTopicNumber aTopicNumber );

        /**
        * Handle changes in container when rect changes.
        * Update message and set new rect to container.
        */
        void HandleRectChangeInContainerL();

    private: // Data structures

        enum TMsgViewSetHandle
            {
            EToNext,
            EToPrevious
            };
      
    private:  // New functions

        /**
        * Creates a find item dialog and pass a message to
        * it and gives the search case, what to look.
        *
        * @param aCommand search case
        */
        void DoSearchL( TInt aCommand );
        
        /**
        * Removes and destroys the context menu.
        */
        void CloseContextMenu();

        /**
        * Launch the context menu.
        */
        void LaunchContextMenuL();

        /**
        * Deletes a old message.
        */ 
        void DeleteOldMessage();

        /**
        * Loads a message from topic view.
        *
        * @return error code if Loading of message has been leaved and catched.
        */
        TInt DoLoadMessageL();

        /**
        * Activates topic view with defined topic number and message handle.
        *
        * @param aTopicNumber which topic the message belongs
        * @param aHandle which message handle is activating topic view.
        */
        void ActivateTopicViewL(
            TCbsTopicNumber aTopicNumber, TCbsMessageHandle aHandle);

        /**
        * Sets current message handle to aMessageHandle.
        *
        * @param aMessageHandle message handle         
        */
        void SetCurrentMessageHandle( const TDesC8& aMessageHandle );

        /**
        * Loads current message buffer.
        */
        void LoadMessageL();

        /**
        * Adds date and time to a aMessage.
        *
        * @param aTime A time when message was received.
        * @param aMessage Address of actual pointer variable. So after 
        *                 reallocation real message content is also changed.
        * @param aResourceArrayId Used resource array for date and time format.
        */
        void AddDateAndTimeL( 
            const TInt64 aTime, HBufC** aMessage, TUint aResourceArrayId ) const;

        /**
        * Returns topic number of current message.
        *
        * @return current topic number
        */
        TCbsTopicNumber CurrentTopicNumber() const;

        /**
        * Gets message position in topic where th aHandle belongs.
        *
        * @param aHandle message handle
        * @param aPosition It will contain the message position in topic.
        * @param aTotal It will contain the total message count in topic,
        * @return error code
        */
        TInt GetMessagePosition(
            TCbsMessageHandle aHandle, TInt& aPosition, TInt& aTotal );

        /**
        * Sets aHandle to previous or next according to aTo
        * if possible.
        *
        * @param aTo sets aHandle to prev or next
        * @return ETrue if aHandle has been set.
        */
        TBool SetHandleTo( TCbsMessageHandle& aHandle, TMsgViewSetHandle aTo );

        /**
        * Deletes a message according to a aHandle.
        *
        * @param aHandle handle to be deleted.
        * @return ETrue if message was deleted
        */
        TBool DeleteMessageL( TCbsMessageHandle aHandle );

        /**
        * Activates topic list view.
        */
        void ActivateTopicListViewL() const;

        /**
        * Sets current message to newest hot marked message or
        * activates the topic list view.
        *
        * @return ETrue if new view is launched
        */
        TBool HandleSoftNotificationL();

    private: 

        /**
        * C++ constructor.
        *
        * @param aServer Reference to cbs server.   
        */
        CCbsUiMsgView( MCbs& aServer );

        /**
        * Two-phased constructor.
        */
        void ConstructL();
        
    private: // Data
        
        // message buffer
        HBufC* iMessage;
        
        // Container class
        CCbsUiMsgViewContainer* iContainer;

        // Navi pane variables
        CAknNavigationControlContainer* iNaviContainer;

        // NaviPaneDecorator
        CAknNavigationDecorator* iDecoratedLabel;
        
        // NaviPaneDecorator
        CAknNavigationDecorator* iPreviousDecoratedLabel;        
      
        // Find item common component        
        CFindItemMenu* iFindItemMenu;
        
        // Context sensitive menu, which appears when pushed ok key        
        CEikMenuBar* iContextMenu;

        // To get a topic count and messages inside
        // a topic.
        MCbs& iServer;

        // Tells if the message is class 0.
        TBool iIsClass0;

        // Current message handle
        TCbsMessageHandle iCurrentHandle;

        // True if launched from the other application.
        TBool iFromOtherApplication;

    };
    
#endif //__CBSMSGVIEW_H


// End of file
