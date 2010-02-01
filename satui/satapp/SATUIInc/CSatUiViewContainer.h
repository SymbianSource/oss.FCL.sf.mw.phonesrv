/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*    Defines CSatUiViewContainer class
*    
*
*/


#ifndef CSATUIVIEWCONTAINER_H
#define CSATUIVIEWCONTAINER_H

// FORWARD DECLARATIONS
class CCoeControl;
class CAknView;
class MSatUiAdapter;
class CEikTextListBox;
class MEikListBoxObserver;
class CSatUiView;

class CSatUiTest;

// CLASS DECLARATION

/**
*  Container for SatUi View.
*/
class CSatUiViewContainer : public CCoeControl
    {
    public: // Constructors and destructor

         /**
        * Two-phased constructor.
        * @param aIconOn If item icons available
        * @param aView Pointer to view
        */
        static CSatUiViewContainer* NewL( TBool aIconOn,
            CSatUiView* aView );

        /**
        * Destructor.
        */
        virtual ~CSatUiViewContainer();

    public: // New functions

        /**
        * Returns a pointer to the listbox object.
        * @return The listbox object pointer.
        */
        CAknColumnListBox* ListBox();

    public: // Functions from base classes

        /**
        * From CCoeControl Counts controls.
        * @return A number of controls.
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl Provides a pointer to a control.
        * @param aIndex Index of the control.
        * @return A pointer to a control.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

        /**
        * From CCoeControl Sets the size of the main pane.
        */
        void SizeChanged();

        /**
        * From CCoeControl Handles list box key events.
        * @param aKeyEvent The key event.
        * @param aType The type of key event.
        * @return Was a key consumed or not.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
            TEventCode aType);

        /**
        * From CCoeControl Returns SatUi help context.
        * @param aContext Help context.
        */
        void GetHelpContext(TCoeHelpContext& aContext) const;

        /**
        * From CCoeControl Handles a change to the control's resources.
        * @param aType The messages type id
        */
        void HandleResourceChange( TInt aType );

        /**
        * From CCoeControl Indicate the requirements for control redrawing.
        * @param aDrawNow Control redraw flags.
        */
        void FocusChanged( TDrawNow aDrawNow );

    private:

        /**
        * C++ default constructor.
        */
        CSatUiViewContainer( CSatUiView* aView );

        /**
        * Symbian OS constructor.
        */
        void ConstructL( TBool aIconOn );

    private: //Data

        //Own: listbox for SetUpMenu and SelectItem view
        CAknColumnListBox* iListBox;
        
        //If item icons available
        TBool iIconOn;

        //Pointer to view        
        CSatUiView* iView;

    };

#endif    // CSATUIVIEWCONTAINER_H

//  End of File
