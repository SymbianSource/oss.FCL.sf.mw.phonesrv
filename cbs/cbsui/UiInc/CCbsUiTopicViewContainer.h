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
*       CCbsUiTopicViewContainer is container for CCbsUiTopicView. 
*       It has one control, a list box, which is showing information
*       about messages in one topic.
*
*
*/


#ifndef CCBSUITOPICVIEWCONTAINER_H
#define CCBSUITOPICVIEWCONTAINER_H

//  INCLUDES
#include <AknIconArray.h>  // CAknIconArray
#include <coecntrl.h>
#include <eiklbo.h>
#include <CbsCommon.h>

  
//  FORWARD DECLARATIONS 

class CSelectionList;
class CEikColumnListBox;
class CListBoxNumbers;
class CFbsBitmap;
class CCbsUiTopicView;


//  CLASS DEFINITIONS 

/**
* Container for CCbsUiTopicView.
*/
class CCbsUiTopicViewContainer : public CCoeControl,
								 public MEikListBoxObserver
    {
    public: // Constructors and destructor

        /**
        * Creates an instance of the class.
        *  
        * @param aTopicView For handling key events.
        * @param aRect An area ment for container.
        */
        static CCbsUiTopicViewContainer* NewL( 
            CCbsUiTopicView& aTopicView, 
            const TRect& aRect );
        
        /**
        * Destructor.
        */
        ~CCbsUiTopicViewContainer();
        
    public: // New functions

        /**
        * Resets list box items. This should be called before ReloadOfferL.
        */
        void ReloadStarts() const;        
        
        /**
        * During reloading offers the view a single message and the 
        * beginning of it.
        *
        * Note that the text part must be less than 100 characters
        * (Well, there is only room for 20 characters, I suppose).
        *  
        * @param  aMessage                It is the message.
        * @param  aBuffer                 It is the beginning of the contents.
        */
        void ReloadOfferL( const TCbsMessage& aMessage, const TDesC& aBuffer ) const;
        
        /**
        * Handles a list box item addition. This should be called
        * when reload is done.
        */
        void ReloadEndsL();
        
        /**
        * Returns the current position.
        * 
        * @return Current position in the list.
        */
        TInt CurrentPosition() const;
        
        /**
        * Sets current item index and draws.
        *
        * @param aItemIndex index to be focused
        */
        void SetCurrentItemIndex( TInt aItemIndex ) const;
        
        /**
        * Returns the list box
        *
        * @return list box
        */
        CEikColumnListBox* GetListBox();        
        
    public: // from CCoeControl
        
        /**
        * Return the amount of controls.
        *    
        * @return amount of controls.
        */
        TInt CountComponentControls() const;
        
        /**
        * Return a control.
        *
        * @param aIndex It is the index to the control.
        * @return It returns the control.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * Sets a new size to the list box.
        */
        void SizeChanged();

        /**
        * Informs that the focus is changed.
        * @param aDrawNow Contains the value that was passed to it by SetFocus()
        */
		void FocusChanged( TDrawNow aDrawNow );
        
        /**
        * Offers a key to the view for handling.
        *
        * @param aKeyEvent what kind of key produced this event
        * @param aType Which kind of action produced this event
        * @return It tells whether the view consumed the keypress or not.
        */
        TKeyResponse OfferKeyEventL(
            const TKeyEvent& aKeyEvent, TEventCode aType );        

        /**
        * Gets the control's help context.
        * 
        * @param aContext The control's help context
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

    public: 
        /**
        * 
        */
        void HandleResourceChange( TInt aType );
        
    public: // From MEikListBoxObserver
    	
    	/**
        * Handles list box events.
        * 
        * @param aEventType The event identifier.
        */
        void HandleListBoxEventL( CEikListBox* /*aListBox*/,
        						  TListBoxEvent aEventType );

    private: 

        /**
        * Constructor.
        *
        * @param aTopicView It is the topic view.
        */
        CCbsUiTopicViewContainer( CCbsUiTopicView& aTopicView );
        
        /**
        * Finalizes the construction.
        *
        * @param aRect It is the client rectangle.
        */
        void ConstructL( const TRect& aRect );

        /**
        * LoadIconsL
        *
        * Loads icons to CAknIconArray and set this iconarray 
        * to list iListBox(CEikColumnListBox)
        */
        void LoadIconsL();

        /**
         *  Append icon to icon array.
         *
         *  @param aId                    Icon id
         *  @param aIcons                 Icon array
         *  @param aIconFileWithPath      Icon file name with path
         *  @param aIconGraphicsIndex     Picture index.
         *  @param aIconGraphicsMaskIndex Mask index.
         */
        void AddIconL( TAknsItemID aId, CAknIconArray* aIcons,
                       const TDesC& aIconFileWithPath,
                       TInt aIconGraphicsIndex,
                       TInt aIconGraphicsMaskIndex,
                       TBool aColorIcon = EFalse );

        /**
         * Change bitmaps when skin changes.
         */
        void SkinBitmapChangesL( );

        /**
        * Construct icon
        *
        * @param aColourIcon    Is icon a colour icon
        * @param aInstance      Current skin instance
        * @param aID            Icon ID
        * @param aFilename      Icon file name with path
        * @param aFileBitmapId  ID of the bitmap in the file   
        * @param aFileMaskId    ID of the mask in the file
        *
        * @return   The constructed icon
        */
        CGulIcon* ConstructIconL( TBool& aColourIcon, 
                            MAknsSkinInstance* aInstance, 
                            const TAknsItemID& aID,                            
                            const TDesC& aFilename,
                            const TInt aFileBitmapId, 
                            const TInt aFileMaskId );

    private: // Data
        
        // It is the listbox.
        CEikColumnListBox* iListBox;       
       
        // For handling some key pressures and for marking a current 
        // topic handle.
        CCbsUiTopicView& iTopicView;        
        
    };
    
#endif  // CCBSUITOPICVIEWCONTAINER_H
    
    // End of File
