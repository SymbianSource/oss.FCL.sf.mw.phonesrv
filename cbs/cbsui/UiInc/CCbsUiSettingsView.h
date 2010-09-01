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
* Description:  Header file for the CCbsSettingsView class.
*
*
*/


#ifndef __CBSSETTINGSVIEW_H
#define __CBSSETTINGSVIEW_H

//  INCLUDES
#include <aknsettingpage.h>  // MAknSettingPageObserver
#include <CbsCommon.h>
#include <aknview.h>
#include "MCbsUiSettingsObserver.h"



// FORWARD DECLARATIONS
class CCbsUiSettingsViewContainer;
class CLanguageViewContainer;
class CAknSettingPage;
class MCbs;
class CCbsUiLanguageSettingPage;
class CAknNavigationDecorator;
class CCbsUiSettingsMonitor;


// CLASS DECLARATION

/**
*  This class is used from messenge center. 
*  With settings view cbs-settings 
*/
class CCbsUiSettingsView : public CAknView, 
                           public MCbsUiSettingsObserver
	{
    public: // Constructors and destructor

        /**
        * C++ constructor.
        *
        * @param aServer cbs server
        */
        CCbsUiSettingsView( MCbs& aServer );    

        /**
        * Two-phased constructor.
        *
        * @param aServer cbs server
        */
        static CCbsUiSettingsView* NewL( MCbs& aServer );

        /**
        * Destructor.
        */
        ~CCbsUiSettingsView();

    public: // From CAknView

        /**
        * Returns the ID of this view.
        * 
        * @return ID
        */
        TUid Id() const;

        /**
        * Handles commands from options menu
        *
        * @param aCommand Command user has selected from options menu
        */
        void HandleCommandL( TInt aCommand );
        
        /**
        * WindowServer calls when the wiev is activated
        *
        * @param aPrevViewId Previous view id.
        * @param aCustomMessageId message id
        * @param aCustomMessage message content
        */
	    void DoActivateL( const TVwsViewId& aPrevViewId, 
                          TUid aCustomMessageId, 
                          const TDesC8& aCustomMessage );

        /**
        * WindowServer calls when the wiev is deactivated
        */
        void DoDeactivate();

    public: // New functions

        /**
        * Called when navigation pane needs be updated
        */
        void UpdateNaviPaneL();

        /**
        * If there has come any changes to settings, then 
        * this function will be called.
        */ 
        void UpdateSettingsL();     

        /**
        * Sets the context pane icon. State can reception on or
        * off.
        */
        void UpdateContextPaneL();

        /**
        * Sets title pane text. 
        */        
        void UpdateTitlePaneL() const;

    public: // From MCbsUiSettingsObserver

        /**
        * Settings monitor calls this function when settings has been
        * changed.
        *
        * @param aEvent What kind of event caused the settings change
        */        
        void SettingsChangedL( TCbsSettingsEvent aEvent );

        /**
        * Handle changes in container when rect changes.
        */
        void HandleRectChangeInContainer();

	private: // MEikMenuObserver 

        /**
        * Delete help menu item if help is not in use
        *
        * @param aResourceId resource id of the menu
        * @param aMenuPane used to modify menu dynamically
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane ); 

    private: // New functions

        /**
        * Activates topic list view.
        */
        void ActivateTopicListViewL() const;

    private:

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

    private: // Data
        
        // Pointer to the container class        
        CCbsUiSettingsViewContainer* iContainer;

        // NaviPane        
        CAknNavigationDecorator* iDecoratedLabel;

        // Reference to the server session. It's owned by uiappappui        
        MCbs& iServer;

        // Previous application and view which has louch this view
        TVwsViewId iPrevViewId;

        // True if launched from the other application.
        TBool iFromOtherApplication;

        // Own: Where the images are located.
        HBufC* iImageLibrary;

        // Settings Monitor
        CCbsUiSettingsMonitor* iSettingsMonitor;

	};

#endif // __CBSSETTINGSVIEW_H


// End of file
