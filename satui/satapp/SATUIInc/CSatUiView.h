/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*    Defines CSatUiView class
*
*
*/


#ifndef CSATUIVIEW_H
#define CSATUIVIEW_H

// INCLUDES
#include    <msatuiobserver.h>
#include    <aknview.h>        // For CAknView class 

// CONSTANTS
static const TInt KTabLength = 4;     // Items tab length

// FORWARD DECLARATIONS
class AknStaticNoteDialog;
class MSatUiAdapter;
class CSatUiViewContainer;
class CSatUiViewAppUi;

// CLASS DECLARATION

/**
* SatUi view (SetUpMenu and SelectItem).
*/
class CSatUiView : public CAknView, 
                   public MEikListBoxObserver
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSatUiView* NewL( TBool aSetUpMenu, 
                                 CSatUiViewAppUi* aAppUi, 
                                 TBool aEmpty );

        /*
        * Destructor.
        */
        virtual ~CSatUiView();

    public: // New functions

        /**
        * Sets the title needed for showing the view.
        * @param aTitle The Title pane text.
        */
        void SetTitle( const HBufC* aTitle );

        /**
        * Sets all parameters needed for showing the view.
        * @param aTitle The Title pane text.
        * @param aMenuItems The Main pane list items.
        * @param aMenuItemNextActions The Options items.
        * @param aHelpIsAvailable The help flag.
        * @return Operation result.
        */
        TSatUiResponse SetItemsL( HBufC* aTitle,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TBool aHelpIsAvailable );

        /**
        * Activates SetUpMenu view.
        * @param aTitle The Title pane text.
        * @param aMenuItems The Main pane list items.
        * @param aMenuItemNextActions The Options items.
        * @param aHelpIsAvailable The help flag.
        * @return Operation result.
        */
        TSatUiResponse SetUpMenuL( HBufC* aTitle,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TBool aHelpIsAvailable );

        /**
        * Activates SelectItem view.
        * @param aTitle The Title pane text.
        * @param aMenuItems The Main pane list items.
        * @param aMenuItemNextActions The Options items.
        * @param aDefaultItem The item in focus.
        * @param aSelection The selected item.
        * @param aHelpIsAvailable The help flag.
        * @return Operation result.
        */
        TSatUiResponse SelectItemL( HBufC* aTitle,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TInt aDefaultItem, TUint8& aSelection,
            const TBool aHelpIsAvailable );

        /**
        * Sets the pointer to adapter provided by SatCli.
        * @param aAdapter A pointer to an abstract adapter interface.
        */
        void SetAdapter( MSatUiAdapter* aAdapter );

        /**
        * Sets the icon bitmap pointer.
        * @param aIconBitmap Context Pane icon.
        */
        void SetIconL( const CFbsBitmap* aIconBitmap );

        /**
        * Sets the items icon array.
        * @param aItemsIconArray Items iconarray.
        * @param aSelfExplanatoryItems Indicate if only icons are shown
        */
        void SetItemsIconL( const CAknIconArray* aItemsIconArray, 
            const TBool aSelfExplanatoryItems );

        /**
        * Hides and makes list box visible if needed.
        * @param aVisible Visible or hidden
        */
        void MakeListBoxVisible( TBool aVisible );
        
        /**
        * Updates CBA according to current next action
        * @param aIndex current row on the list
        * @param aNew is the view just created
        */
        void UpdateMskL( TInt aIndex, TBool aNew );

    public: // Functions from base classes

        /**
        * From CAknView Returns the UId of the view.
        * @return The UId of the view.
        */
        TUid Id() const;

        /**
        * From CAknView Handles view commands.
        * @param aCommand The command identifier.
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CAknView Handles the options menu.
        * @param aResourceId Resource id of the menu pane.
        * @param aMenuPane The menu pane.
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    protected: // New functions

        /**
        * Updates the view container.
        */
        void RefreshL();

        /**
        * Draws the title and list items of the view.
        */
        void DrawL();

    protected: // Functions from base classes

        /**
        * From MEikListBoxObserver Handles list box events.
        * @param aEventType The event identifier.
        */
        void HandleListBoxEventL( CEikListBox* /*aListBox*/,
            TListBoxEvent aEventType );

        /**
        * From CAknView Activates the view.
        */
        void DoActivateL( const TVwsViewId& /*PrevViewId*/,
            TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/);

        /**
        * From CAknView Deactivates the view.
        */
        void DoDeactivate();

    private:

        /** 
        * called when user has been
        * inactive for a pre-defined period
        */
        static TInt DispatchUserInactive( TAny* aPtr );

        /**
        * C++ default constructor.
        */
        CSatUiView( TBool aSetUpMenu, CSatUiViewAppUi* aAppUi, TBool aEmpty );

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

    public: //Data
        
        //CUserActivityManager user response wait scheduler
        CActiveSchedulerWait iWait;

    private: //Data

        // Own: indicates that a note was cleared after a delay
        TBool iNoResponseFromUserAfterDelay;

        //Ref: adapter provided by SatCli
        MSatUiAdapter* iAdapter;

        //To block the keypad abter the user action and unblock when
        //the new SatServer command comes
       TBool iKeypadBlocked;

        //Type of the view (SetUpMenu or SelectItem)
        TBool iSetUpMenu;

        //Ref: UI object
        CSatUiViewAppUi* iSatUiAppUi;

        //Own: items of the main pain list
        CDesCArrayFlat* iMenuItems;
        //Own: codes of options menu items
        CArrayFixFlat<TSatAction>* iMenuItemNextActions;

        //Number of items in the maind pane
        TInt iNItems;

        //Ref: view title text
        const HBufC* iTitle;

        TBool iHelpIsAvailable;

        //Own: view container
        CSatUiViewContainer* iContainer;

        //Ref: list box model
        CTextListBoxModel* iModel;

        //The response to SatServer
        TSatUiResponse iResponse;

        //default item in the Select Item list
        TInt iDefaultItem;

        //selected item in the list
        TInt iSelection;

        //client rectangle
        TRect iRect;

        //Items icon array
        CAknIconArray* iItemsIconArray;

        //Icons exists on view
        TBool iIconsOn;

        //Icons state changed
        TBool iIconStateChange;
        
        //If only icons are shown
        TBool iSelfExplanatoryItems;
        
        //Value for displaying list box.
        TBool iListBoxVisible;
        
        //Indication that view is first created empty view
        TBool iEmpty;

    };

#endif    // CSATUIVIEW_H

//  End of File
