/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file for the CCbsUiMsgViewContainer class.
*
*
*/


#ifndef __CCBSUIMSGVIEWCONTAINER_H
#define __CCBSUIMSGVIEWCONTAINER_H

//  INCLUDES
#include <coecntrl.h>


// FORWARD DECLARATIONS
class CCbsUiMsgView;
class CEikRichTextEditor;
class CRichText;
class CCbsUiMsgView;
class CEikButtonGroupContainer;
class CEikScrollBarFrame;
class CAknsBasicBackgroundControlContext;

// CLASS DECLARATION
/**
* It is the container class viewing messages.
*/
class CCbsUiMsgViewContainer : public CCoeControl,
                               public MEikScrollBarObserver
    {
    public: // Constructors and destructor
        
        /**
        * Two-phased constructor.
        *
        * @param aMsgView Referenceo to message view.
        * @param aRect Rectangle, which container can use.
        */
        static CCbsUiMsgViewContainer* NewL( 
            CCbsUiMsgView& aMsgView, 
            const TRect& aRect );

        /**
        * Destructor.
        */
        ~CCbsUiMsgViewContainer();
        
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
        * Sets new rectangle to the editor.
        */
        void SizeChanged();
        
        /**
        * Handles a needed key events.
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
        void GetHelpContext(TCoeHelpContext& aContext) const;

    public: // New functions    

        /**
        * Gets new message from messag view and sets the editor.
        */
        void UpdateMessageL( const TDesC& aMessage );
        
        /**
        * Returns the text in Rich Text editor.
        *
        * @return the text
        */
        CRichText* GetEditor() const;    
        
        /**
        * Copies the message to clipboard.
        */
        void CopyToClipboardL() const;  
        
        /**
        * From CCoeControl  Receives notification on resource changes
        * @param aType type of the resource change
        */
        void HandleResourceChange( TInt aType );
        
        /**
        * From MEikScrollBarObserver  Receives and handles
        * scroll bar events 
        * @param aScrollBar A pointer to scrollbar which created the event
        * @param aEventType The event occured on the scroll bar
        */
        void HandleScrollEventL( CEikScrollBar* aScrollBar, TEikScrollEvent aEventType );
        
    private: // New functions

        /**
        * Sets the viewing component to read only mode
        *
        * @param aReadOnly value to set to
        */
        void SetReadOnly( TBool aReadOnly );        
      
        /**
        * Sets the contents of viewing component
        *
        * @param aBuf A new content for editor.
        */
        void SetData16L( const TDesC& aBuf );
        
        /**
        * Sets the contents of viewing component
        * 
        * @param aText Text to be inserted.
        */
        void SetDataContentL( CRichText& aText );
        
        /**    
        * Created a new editor. 
        *
        * @return Component for viewing
        */
        CEikRichTextEditor* CreateEditorL() const;  
        
        /**
        * Clears the area between CEikRichTextEditor and Client regtangle.
        * 
        * @param aRect Outer rect.
        */
        void Draw( const TRect& aRect ) const;

        /**
        * Initializes a variables, which are needed to create
        * scroll indicator. Has to be called every time the 
        * content of the message has changed.
        */
        void InitializeMessageInfoL();
        
        /**
        * Creates scroll bar indicator.
        */
        void CreateScrollBarFrameL();    
        
        /**
        * Updates the scroll indicator based on the iCurrentScreen and 
        * information, which has been set in the InitializeMessageInfoL()
        */
        void UpdateScrollIndicatorL() const;

        /**
        * Updates the current screen position. 
        * 
        * @param aIncrease direction where to modify current screen
        */
        void UpdateCurrentScreen( TBool aIncrease );
        
        /**
        * Scrolls message up. 
        */
        void ScrollMessageUpL();
        
        /**
        * Scrolls message down. 
        */
        void ScrollMessageDownL();

    private: 

        /**
        * Constructor.
        *
        * @param aListView reference to the msg view.
        */
        CCbsUiMsgViewContainer( CCbsUiMsgView& aListView );

        /**
        * Two-phased constructor.
        *
        * @param aRect Rect to be used by this container.
        */
        void ConstructL( const TRect& aRect );            
        
    private: // Data 

        // It is the view class
        CCbsUiMsgView& iMsgView;
        
        // It is the component for viewing message
        CEikRichTextEditor* iEditor;
        
        // ScrollBarFrame
        CEikScrollBarFrame* iSBFrame;

        // How many screens the message contains.
        TInt iNumberOfScreens;

        // Current message screen. It is updated every time
        // the page is changed.
        TInt iCurrentScreen;

        CAknsBasicBackgroundControlContext* iBackGroundControlContext;
        
    };
    
    
#endif // __CCBSUIMSGVIEWCONTAINER_H
    
// End of file
