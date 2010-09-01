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
*       TopicListViewContainer owns the list box which shows
*       topics.
*
*
*/



#ifndef __CCBSUITOPICLISTVIEWCONTAINER_H
#define __CCBSUITOPICLISTVIEWCONTAINER_H

//  INCLUDES
#include <coeccntx.h>
#include <CbsCommon.h>
#include <AknIconArray.h>  // CAknIconArray

//  CONSTANTS  
const TInt KTopicNumberLength = 3;

 
//  FORWARD DECLARATIONS

class CCbsUiTopicListView;
class CEikColumnListBox;



// CLASS DECLARATION

/**
*  TopicListView container class
* 
*/
class CCbsUiTopicListViewContainer : public CCoeControl,
									 public MEikListBoxObserver
	{

    public: // Constructors and destructor    

        /**
        * Two-phased constructor.
        *
        * @param aListView to update view
        * @param aRect rectangle for which container can use
        */
        static CCbsUiTopicListViewContainer* NewL( 
            CCbsUiTopicListView& aListView, 
            const TRect& aRect );
        
        /**
        * Destructor.
        */
        virtual ~CCbsUiTopicListViewContainer();

    public: // New functions
        
        /**
        * Resests the array of list box. This should be called when 
        * before ReloadOfferTopic.
        */  
        void ReloadStarts() const;

        /**
        * Adds topic information to the topic.
        *
        * @param aTopic for setting name and topic number to the list box
        */  
        void ReloadOfferTopicL( const TCbsTopic& aTopic ) const;

        /**
        * Handles a list box item addition. This should be called
        * when reload is done. 
        */
        void ReloadEndsL();

        /**
        * Returns current position, i.e. Current listbox item.
        *
        * @return currentposition
        */ 
        TInt CurrentPosition() const;

        /**
        * Sets the current item index in the list box.
        *
        * @param aItemIndex new focused item index
        */
        void SetCurrentItemIndex( TInt aItemIndex ) const;

    public: // From CCoeControl 

        /**
        * Returns the count of components.
        * 
        * @return number of components
        */    	   
        TInt CountComponentControls() const;

        /**
        * Returns a component. 
        *
        * @param aIndex component index to return
        * @return requested component
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
        * Handles a needed key events.
        *
        * @param aKeyEvent what kind of key produced this event
        * @param aType Which kind of action produced this event
        * @return EKeyWasConsumed or EKeyWasNotConsumed 
        */    	   
	    TKeyResponse OfferKeyEventL(
            const TKeyEvent& aKeyEvent, 
            TEventCode aType );	        	      

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
        * Handles list box events,
        * 
        * @param aEventType The event identifier.
        */
        void HandleListBoxEventL( CEikListBox* /*aListBox*/,
        						  TListBoxEvent aEventType );

    private: // New functions

        /**
        * Indicates, which icon to use:
        * '0' = subscribed and there are no messages
        * '1' = subscribed and there are messages
        * '2' = not subscribed and there are no messages
        * '3' = not subscribed, but are messages
        *
        * @param aTopic to check the topic status
        * @return char which indicates, which icon to use.
        */ 
	    TChar GetStatus( const TCbsTopic& aTopic ) const;

        /**
        * Sets topic index to given buffer.
        *
        * @param aTopic a topic, from which will be used topic number
        * @param aIndexNumber buffer to be filled with topic number
        */
	    void GetIndexNumber( 
            const TCbsTopic& aTopic, TBuf<KTopicNumberLength>& aIndexNumber ) const;

    private: 

        /**
        * Constructor.
        *
        * @param aListView to update view
        */
        CCbsUiTopicListViewContainer(CCbsUiTopicListView& aListView);
        
        /**
        * Symbian OS constructor.
        *
        * @param aRect size of client area
        */
        void ConstructL(const TRect& aRect);
        
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
         *  @param aColorIcon             Is icon colored
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

        // Reference to TopicListView.
        CCbsUiTopicListView& iListView;

        // For data to be shown in the view.
        CEikColumnListBox* iListBox;

	};


#endif // __CCBSUITOPICLISTVIEWCONTAINER_H


// End of File
