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
*     Shows which of the language setting has been selected,
*     All, Selected or Other.
*
*
*/


#ifndef __CBS_UI_LANGUAGE_SETTING_PAGE_H
#define __CBS_UI_LANGUAGE_SETTING_PAGE_H

//  INCLUDES
#include <aknradiobuttonsettingpage.h> // CAknRadioButtonSettingPage
#include <CbsCommon.h>


// FORWARD DECLARATIONS
class MCbs;

// CLASS DECLARATION

/**
* This settings page is popped up if a user selects a language setting 
* from the SettingsView.
*/
class CCbsUiLanguageSettingPage : public CAknRadioButtonSettingPage
    {            
    public: // Constructor

        /**
        * C++ constructor.
        *
        * @param aRerourceID Resource id.
        * @param aCurrentSelectionIndex item to be selected
        * @param aItemArray array of items
        * @param aServer to update this level selections
        */
        CCbsUiLanguageSettingPage( 
            TInt aResourceID,     
            TInt& aCurrentSelectionIndex,
            CDesCArrayFlat* aItemArray,
            MCbs& aServer );


    protected:  // From CEikDialog

        /**
        * Sets the selected languages into the server.
        *
        * @param aAccept selected key
        */
        TBool OkToExitL( TBool aAccept );

    protected: // From CCoeControl

        /*
        * Launches a CCbsUiLanguageDlg if EKeyOk is pressed and the focus
        * is on the selected.
        *
        * @param aKeyEvent what kind of key produced this event
        * @param aType Which kind of action produced this event
        */
        TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent, TEventCode aType );

    private: // New functions

        /**
        * Gets languages from the server and marks selected languages to
        * aIndexArray.
        *
        * @param aIndexArray indexes of selected languages
        * @param aLanguages it will contain selected languages from the server
        */
        void GetLanguagesL( CArrayFix<TInt>& aIndexArray, TCbsDbLanguages& aLanguages );

        /**
        * Launches the language dialog.
        */
        void LaunchLanguageDialogL();

        /**
        * Sets selected languages into server according to aIndexArray.
        *
        * @param aIndexArray    contains selected languages
        * @param aLanguages     Contains information about previously selected
        *                       languages.
        */
        void SetLanguagesL( CArrayFix<TInt>& aIndexArray, TCbsDbLanguages& aLanguages );

    private: // Data

        // The server. This class doesn't own this.        
        MCbs& iServer;

        // Current selection index
        TInt& iSelectionIndex;

        // To update the selection
        CDesCArrayFlat& iArray;

        // it should be possible to launch language dialog only once
        TInt iCurrentLaunchingId;

    };

#endif      // __CBS_UI_LANGUAGE_SETTING_PAGE_H
            
// End of File
