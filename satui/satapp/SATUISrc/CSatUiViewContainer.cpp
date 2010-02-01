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
*    Defines CSatUiViewContainer class
*    
*
*/


// INCLUDE FILES
#include    <eiktxlbx.h>
#include    <eiklbx.h>              //for CEikListBox
#include    <barsread.h>
#include    <eikenv.h>
#include    <aknlists.h>

#include    <SatUi.rsg>

#include    <AknIconArray.h>
#include    <eikclbd.h>
#include    <AknDef.h>              // New Akn header
#include    <csxhelp/sat.hlp.hrh>   // For help

#include    "CSatUiApplication.h"
#include    "CSatUiViewContainer.h"
#include    "CSatUiView.h"
#include    "tflogger.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSatUiViewContainer::CSatUiViewContainer( CSatUiView* aView ) :
    iView( aView )
    {
    }

// ---------------------------------------------------------
// CSatUiViewContainer::ConstructL
// Symbian OS default constructor can leave.
// (other items were commented in a header).
// ---------------------------------------------------------.
void CSatUiViewContainer::ConstructL( TBool aIconOn )
    {
    TFLOGSTRING( "CSatUiViewContainer::ConstructL called" )
    CreateWindowL();
    iIconOn = aIconOn;

    TResourceReader resourceReader;

    iCoeEnv->CreateResourceReaderLC( resourceReader , 
        R_SATUI_LISTBOX );
    
    //different listbox depends on item icon
    if ( iIconOn )
        {
        TFLOGSTRING( "CSatUiViewContainer::ConstructL iIconOn true" )
        iListBox = new (ELeave) 
            CAknSingleGraphicStyleListBox;
        }
    else
        {        
        TFLOGSTRING( "CSatUiViewContainer::ConstructL iIconOn false" )        
        iListBox = new (ELeave) CAknSingleStyleListBox;
        }

    iListBox->SetContainerWindowL( *this );

    iListBox->ConstructFromResourceL( resourceReader );

    iListBox->CreateScrollBarFrameL( ETrue );

    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );

    // resourceReader. Cannot add this as a parameter since 
    // TResourceReader is not derived from CBase
    CleanupStack::PopAndDestroy();
    TFLOGSTRING( "CSatUiViewContainer::ConstructL exit" )
    }

// ---------------------------------------------------------
// CSatUiViewContainer::NewL
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------.
CSatUiViewContainer* CSatUiViewContainer::NewL( TBool aIconOn,
    CSatUiView* aView )
    {
    TFLOGSTRING( "CSatUiViewContainer::NewL called-exit" )
    CSatUiViewContainer* self =
        new (ELeave) CSatUiViewContainer( aView );

    CleanupStack::PushL( self );
    self->ConstructL( aIconOn );
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------
// CSatUiViewContainer::~CSatUiViewContainer
// Destructor.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSatUiViewContainer::~CSatUiViewContainer()
    {
    TFLOGSTRING( "CSatUiViewContainer::~CSatUiViewContainer called" )
    //need to reset first if contains icons
    if ( iListBox->ItemDrawer()->ColumnData()->IconArray() )
        {
        iListBox->ItemDrawer()->ColumnData()->IconArray()->Reset();
        }
    delete iListBox;
    TFLOGSTRING( "CSatUiViewContainer::~CSatUiViewContainer exit" )
    }

// ---------------------------------------------------------
// CSatUiViewContainer::ListBox
// Returns a pointer to a listbox object.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CAknColumnListBox* CSatUiViewContainer::ListBox()
    {
    TFLOGSTRING( "CSatUiViewContainer::ListBox called-exit" )
    return iListBox;
    }

// ---------------------------------------------------------
// CSatUiViewContainer::CountComponentControls
// The only control is a listbox.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSatUiViewContainer::CountComponentControls() const
    {
    TFLOGSTRING( "CSatUiViewContainer::CountComponentControls called-exit" )
    return 1;
    }

// ---------------------------------------------------------
// CSatUiViewContainer::ComponentControl
// The only control is a listbox.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCoeControl* CSatUiViewContainer::ComponentControl(
    TInt /* aIndex */) const
    { 
    TFLOGSTRING( "CSatUiViewContainer::ComponentControl called-exit" )
    return iListBox;
    }

// ---------------------------------------------------------
// CSatUiViewContainer::SizeChanged
// Sets the available size.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewContainer::SizeChanged()
    {
    TFLOGSTRING( "CSatUiViewContainer::SizeChanged called" )    
    TRect parentRect( Rect() );
	if ( iListBox )
        {
        iListBox->SetRect( parentRect );
        }
    TFLOGSTRING( "CSatUiViewContainer::SizeChanged exit" ) 
    }

// ---------------------------------------------------------
// CSatUiViewContainer::OfferKeyEventL
// Listbox takes all event even if it doesn't use them.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CSatUiViewContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    TFLOGSTRING( "CSatUiViewContainer::OfferKeyEventL called" ) 
    TKeyResponse response( EKeyWasNotConsumed );
    
    if ( ( EKeyLeftArrow != aKeyEvent.iCode ) &&
        ( EKeyRightArrow != aKeyEvent.iCode ) )
        {
        response = iListBox->OfferKeyEventL( aKeyEvent, aType );
        }
    
    if ( ( EKeyUpArrow == aKeyEvent.iCode ) ||
        ( EKeyDownArrow == aKeyEvent.iCode ) )
        {
        iView->UpdateMskL( iListBox->CurrentItemIndex(), EFalse );
        }
    TFLOGSTRING2( "CSatUiViewContainer::OfferKeyEventL exit, return: %d", \
        response ) 
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewContainer::GetHelpContext
// Returns SatUi help context.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewContainer::GetHelpContext(
    TCoeHelpContext& aContext ) const
    {
    TFLOGSTRING( "CSatUiViewContainer::GetHelpContext called" ) 
	aContext.iMajor = KUidSATUI;
    aContext.iContext = KSAT_HLP_APP;
    TFLOGSTRING( "CSatUiViewContainer::GetHelpContext exit" ) 
    }

// ---------------------------------------------------------
// CSatUiViewContainer::HandleResourceChange
// Handles control's layout change.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewContainer::HandleResourceChange( TInt aType )
    {
    TFLOGSTRING2( "CSatUiViewContainer::HandleResourceChange called, \
        aType: %d", aType ) 
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( 
            AknLayoutUtils::EMainPane, mainPaneRect );
        SetRect( mainPaneRect );
        DrawDeferred();
        }
    
    CCoeControl::HandleResourceChange( aType );
    TFLOGSTRING( "CSatUiViewContainer::HandleResourceChange exit" ) 
    }

// ---------------------------------------------------------
// CSatUiViewContainer::FocusChanged
// Indicate the requirements for control redrawing.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewContainer::FocusChanged( TDrawNow aDrawNow )
    {
    TFLOGSTRING( "CSatUiViewContainer::FocusChanged called" ) 
    CCoeControl::FocusChanged( aDrawNow );
    if ( iListBox )
        {
        iListBox->SetFocus( IsFocused(), aDrawNow );
        }
    TFLOGSTRING( "CSatUiViewContainer::FocusChanged exit" ) 
    }

//  End of File
