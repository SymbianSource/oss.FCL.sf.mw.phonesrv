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
* Description:  Header file for the CCbsUiSettingsViewContainer class.
*
*
*/


#ifndef __CCBSUISETTINGSVIEWCONTAINER_H
#define __CCBSUISETTINGSVIEWCONTAINER_H

//  INCLUDES
#include <bldvariant.hrh>
#include <coecntrl.h>
#include <eiklbo.h>            // MEikListBoxObserver
#include <CbsCommon.h>
#include "CbsUiConstants.h"


// FORWARD DECLARATIONS
class CCbsUiMsgView;
class MCbs;
class CCbsUiSettingsView;
class CAknSettingStyleListBox;
class CSelectionItemList;

// CLASS DECLARATION

/**
*  Container for settings view components.
*  
*/
class CCbsUiSettingsViewContainer : public CCoeControl,
                                    public MEikListBoxObserver
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aView settings view
        * @param aRect containers area
        * @param aServer cbs server
        */
        static CCbsUiSettingsViewContainer* NewL( CCbsUiSettingsView& aView,
                                                  const TRect& aRect,
                                                  MCbs& aServer );
        
        /**
        * Destructor.
        */
	    ~CCbsUiSettingsViewContainer();
    
    public: // From CCoeControl

        /**
        * Returns the numbers of components.
        * 
        * @return Number of components in the container
        */
	    TInt CountComponentControls() const;

        /**
        * Returns a component.  
        *
        * @param aIndex component index which is required
        * @return required component
        */
	    CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * Sets a new size to list box.
        */
	    void SizeChanged();

        /**
        * Informs that the focus is changed.
        * @param aDrawNow Contains the value that was passed to it by SetFocus()
        */
		void FocusChanged( TDrawNow aDrawNow );

        /**
        * Offers key event to list box.
        * 
        * @param aKeyEvent what kind of key produced this event
        * @param aType Which kind of action produced this event
        * @return EKeyWasConsumed
        */
	    TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent, TEventCode aType );

        /**
        * Gets the control's help context.
        * 
        * @param aContext The control's help context
        */
	    void GetHelpContext( TCoeHelpContext& aContext ) const;

        /**
        * Handles a change to the control's resources.
        * @param aType A message UID value.
        */
        void HandleResourceChange( TInt aType );

    public: // New functions

        /**
        * Changes the value which the user has pushed and asks
        * server to save that value. If the user has choosed the 
        * language item the language page will show up. Calls
        * private HandleListViewActionL.
        *
        * @param aLaunchedFromOptionMenu if ETrue pop_up_setting_item 
        *                                will be launhed
        * @return EKeyWasConsumed
        */
        TKeyResponse HandleListViewActionL( TBool aLaunchedFromOptionMenu );

        /**
        * Offers public interface to update all settings.
        * Is needed if operator changes some values during in settings view.
        */
        void UpdateSettingsL();

        /**
        * Sets focus to first item in settings list box. Needed, because
        * sometimes view is not activated correctly.
        */
        void SetFocusToFirstItem() const;

    protected: // From MEikListBoxObsever
        
        /**
        * Handles list box events.
        *
        * @param aListBox the originating list box
        * @param aEventType a code for the event.
        */
        void HandleListBoxEventL( CEikListBox* aListBox, 
                                  TListBoxEvent aEventType ); 

    private: // New functions

        /**
        * With this settings view can update the text in the 
        * list box for language item. This can only be called from 
        * GetSettingsL!
        */
        void SetLanguageSelectionL( CDesCArray& aItemArray );

        /**
        * Creates a list box.
        */
        void CreateListBoxL();

        /**
        * Gets the settings of reception, language and detection from the 
        * sserver and sets these values to list box.
        */
        void GetSettingsL();

        /**
        * Changes the value which the user has pushed and asks
        * server to save that value. If the user has choosed the 
        * language item the language page will show up.
        *
        * @param aListBox changes are focused on this list box
        * @param aLaunchedFromOptionMenu if ETrue pop_up_setting_item 
        *                                will be launhed
        * @return EKeyWasConsumed
        */
        TKeyResponse HandleListViewActionL( 
            const CEikListBox* aListBox, TBool aLaunchedFromOptionMenu );

        /**
        * Launches a dialog for On and Off setting items.
        * 
        * @param aValue it will contain selected value in dialog
        * @param aUpdate if aValue has changed in dialog
        * @param aResouceId setting text resource id
        */
        void LaunchDialogOnOffL(
            const TBool& aValue, TBool& aUpdate, TInt aResouceId ) const;

        /**
        * Launches a topic detection dialog for On and Off setting items.
        * 
        * @param aValue it will contain selected value in dialog
        * @param aUpdate if aValue has changed in dialog
        * @param aResouceId setting text resource id
        */
        void LaunchTopicDetectionDialogOnOffL(
            const TBool& aValue, TBool& aUpdate, TInt aResouceId ) const;

        /**
        * Prepares the language setting page with the values stored in 
        * the server.
        */
		void PrepareLanguageSettingPageL();

        /**
        * Returns selected item in settings page.
        *
        * @return selected item
        */
        TCbsUiLanguageSelectionIndex GetSelected();

        /**
        * Launchs language setting page with aItems.
        *
        * @param aItems items in language setting page.
        * @param aCurrentSelectionIndex item to be selected
        */
        void LaunchLanguageSettingPageL(
            CDesCArrayFlat& aItems, TInt& aCurrentSelectionIndex );

        /**
        * Sets items to a aItemList from resource array.
        *
        * @param aResourceId resource array
        * @param aItemList list to be filled
        */
        void AddItemsToListL( 
            TInt aResourceId, 
            CDesCArrayFlat& aItemList ) const;

    private:

        /**
        * C++ constructor.
        *
        * @param aView settings view
        * @param aServer cbs server
        */
        CCbsUiSettingsViewContainer( CCbsUiSettingsView& aView, 
                                     MCbs& aServer );

        /**
        * Symbian OS default constructor.
        */ 
        void ConstructL( const TRect& aRect );


        /**
        * Checks if phone is in offline mode or not
        * @return ETrue if phone is in offline mode
        *         Otherwise EFalse.
        */
        TBool IsPhoneOfflineL() const;


    private: // Data
        
        // List box object in dialog.        
        CAknSettingStyleListBox* iListBox;

        // For setting mop parent
        CCbsUiSettingsView& iView;

        // With the help of server it is possible to set and get
        // setting information.        
        MCbs& iServer;

        // If ofline feature is supported then this is ETrue.
        TBool iOfflineFeatureSupported;

	};


#endif //__CCBSUISETTINGSVIEWCONTAINER_H

// End of file
