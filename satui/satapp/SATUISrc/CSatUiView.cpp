/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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


// INCLUDE FILES
#include    <coecntrl.h> //for CCoeControl class
#include    <akntitle.h>
#include    <eikmenup.h>
#include    <barsread.h>
#include    <badesca.h> // for descriptor arrays
#include    <aknlists.h>
#include    <eiktxlbx.h>
#include	<akncontext.h>	//for Context pane
#include	<fbs.h>			//for iocn bitmap
#include    <StringLoader.h>
#include    <avkon.hrh>
#include    "satui.hrh"
#include    <SatUi.rsg>
#include    <AknIconArray.h> 
#include    <gulicon.h>
#include    <eikclbd.h>
#include    <msatuiadapter.h>
#include    <aknnotedialog.h>
#include    <aknstaticnotedialog.h>
#include    <featmgr.h> // Used for FeatureManager
#include    <activitymanager.h>

#include    "CSatUiApplication.h"
#include    "CSatUiViewAppUi.h"
#include    "CSatUiViewContainer.h"
#include    "CSatUiView.h"
#include    "tflogger.h"

// CONSTANTS
// From Ui spec 60 sec delay user response
const TInt KNoResponseFromUserDelay = 60;

_LIT( KTab, "\t" );
_LIT( KIconTab, "%d\t" );
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSatUiView::CSatUiView( TBool aSetUpMenu, CSatUiViewAppUi* aAppUi, 
                        TBool aEmpty ):
    iSetUpMenu( aSetUpMenu ),
    iSatUiAppUi( aAppUi ),
	iModel( NULL ),
    iIconsOn( EFalse ),
    iIconStateChange( EFalse ),
    iSelfExplanatoryItems( EFalse ),
    iListBoxVisible( ETrue ),
    iEmpty( aEmpty )
    {
    }

// Symbian OS default constructor can leave.
void CSatUiView::ConstructL()
    {
	TFLOGSTRING( "CSatUiView::ConstructL() called" )

	TInt recource( R_SATUI_SELECTITEM_VIEW );

    if ( iSetUpMenu )
        {
        if ( iEmpty )
            {
            recource = R_SATUI_EMPTYMENU_VIEW;
            }
        else
            {
            recource = R_SATUI_SETUPMENU_VIEW;
            }
        }

    BaseConstructL( recource );

    iItemsIconArray = new(ELeave) CAknIconArray( 1 );
	TFLOGSTRING( "CSatUiView::ConstructL() exit" )
    }

// Two-phased constructor.
CSatUiView* CSatUiView::NewL( TBool aSetUpMenu, 
                              CSatUiViewAppUi* aAppUi, 
                              TBool aEmpty )
    {
	TFLOGSTRING( "CSatUiView::NewL called" )
    CSatUiView* self = new ( ELeave ) CSatUiView( aSetUpMenu, aAppUi, aEmpty );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
	TFLOGSTRING( "CSatUiView::NewL exit" )
    return self;
    }

// Destructor.
CSatUiView::~CSatUiView()
    {
	TFLOGSTRING( "CSatUiView::~CSatUiView() called" )

    delete iContainer;
    delete iMenuItemNextActions;
    delete iMenuItems;

    if ( iItemsIconArray )
        {
        iItemsIconArray->ResetAndDestroy();
        delete iItemsIconArray;
        iItemsIconArray = NULL;
        }

	TFLOGSTRING( "CSatUiView::~CSatUiView() exit" )
    }

// ---------------------------------------------------------
// CSatUiView::SetTitle
// Sets the title needed for showing the view.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::SetTitle( const HBufC* aTitle )
    {
    TFLOGSTRING( "CSatUiView::SetTitle called" )
    iTitle = aTitle;
    TFLOGSTRING( "CSatUiView::SetTitle exit" )
    }

// ---------------------------------------------------------
// CSatUiView::SetIconL
// Sets the icon needed to the view.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::SetIconL( const CFbsBitmap* aIconBitmap )
	{
    TFLOGSTRING( "CSatUiView::SetIconL called" )
	if ( StatusPane()->PaneCapabilities( TUid::Uid(
        EEikStatusPaneUidContext ) ).IsPresent() )
		{
		TFLOGSTRING( "CSatUiView::SetIconL pane is present" )
        CEikStatusPane* sp=iEikonEnv->AppUiFactory()->StatusPane();
        CleanupStack::PushL( sp );
		CAknContextPane* contextPane=( CAknContextPane * )sp->
            ControlL( TUid::Uid( EEikStatusPaneUidContext ) );
		CleanupStack::Pop( sp );    //sp
        if ( aIconBitmap )      //icon from SIM
            {
            TFLOGSTRING( "CSatUiView::SetIconL icon from SIM" )
            contextPane->SetPicture( aIconBitmap );
            }
        else                    //set default icon
            {
            TFLOGSTRING( "CSatUiView::SetIconL default icon" )
            contextPane->SetPictureToDefaultL();
            }
		}
	TFLOGSTRING( "CSatUiView::SetIconL exit" )
	}

// ---------------------------------------------------------
// CSatUiView::SetItemsIconL
// Sets the items icon array.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::SetItemsIconL( 
    const CAknIconArray* aItemsIconArray, 
    const TBool aSelfExplanatoryItems )
    {
    TFLOGSTRING( "CSatUiView::SetItemsIconL called" )
    iItemsIconArray->ResetAndDestroy();
    //if items icon array available
    if ( aItemsIconArray->Count() )
        {
        TFLOGSTRING( "CSatUiView::SetItemsIconL icon array available" )
        if( !iIconsOn )
            {
            //last view not with icons
            iIconStateChange = ETrue;
            }
        iItemsIconArray->InsertL( 0, &aItemsIconArray->operator[]( 0 ), 
            aItemsIconArray->Count() );
        iIconsOn = ETrue;
        iSelfExplanatoryItems = aSelfExplanatoryItems;
        }
    else
        {
        TFLOGSTRING( "CSatUiView::SetItemsIconL icon array not available" )
        if( iIconsOn )
            {
            TFLOGSTRING( "CSatUiView::SetItemsIconL iIconsOn true" )
            iIconStateChange = ETrue;
            }
        else
            {
            TFLOGSTRING( "CSatUiView::SetItemsIconL iIconsOn false" )
            iIconStateChange = EFalse;
            }
        iIconsOn = EFalse;
        iSelfExplanatoryItems = EFalse;
        }
    TFLOGSTRING( "CSatUiView::SetItemsIconL exit" )
    }

// ---------------------------------------------------------
// CSatUiView::SetItemsL
// Sets all parameters needed for showing the view.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiView::SetItemsL(
    HBufC* aTitle,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const TBool aHelpIsAvailable )
    {
	TFLOGSTRING( "CSatUiView::SetItemsL called" )
    iKeypadBlocked = EFalse;

    delete iMenuItemNextActions;
    iMenuItemNextActions = NULL;

    if( !iMenuItems )
		{
		iMenuItems = new ( ELeave ) CDesCArrayFlat( 1 );
		}
	// This reset is required
	iMenuItems->Reset();

    // Remove NULL characters from title
    TPtr titleptr = aTitle->Des();
    TInt nullLocation;
    while ( ( nullLocation = titleptr.Locate( NULL ) ) >= 0 )
        {
        titleptr.Delete( nullLocation, 1 );
        }

    iTitle = aTitle;

    //no memory is allocated to the array buffers here:
    iMenuItemNextActions = new ( ELeave ) CArrayFixFlat<TSatAction>( 1 );

    iNItems = aMenuItems.MdcaCount();

    TBool nextActions( EFalse );
    
    TFLOGSTRING2( "CSatUiView::SetItemsL, iNItems: %d", iNItems )
    if ( iNItems == ( aMenuItemNextActions->Count() ) )
        {
        nextActions = ETrue;
        }

    if (iNItems > 0)
        {
        for ( TInt i(0); i < iNItems; i++ )
            {
            TInt itemLength = aMenuItems.MdcaPoint(i).Length();
            HBufC* tmpBuf = HBufC::NewLC(
                aMenuItems.MdcaPoint(i).Length() + KTabLength );
            TPtr tmpPtr( tmpBuf->Des() );

            // Item text
            TPtrC16 text ( aMenuItems.MdcaPoint(i) );
                        
            // Needed for NULL check
            HBufC* correctText = HBufC::NewLC( itemLength );
            TPtr ptr = correctText->Des();

            // Check item text through
            for ( TInt j(0); j < itemLength; j++)
                {
                TChar ch = text[j];
                // Remove NULL characters from item text
                if ( ch )
                    {
                    ptr.Append( ch ); 
                    }
                }

            //if view includes icons
            if( iIconsOn )
                {
                TFLOGSTRING( "CSatUiView::SetItemsL iIconsOn true" )
                //with icons
                TBuf<KTabLength> lineTab;
                lineTab.Format( KIconTab,i );
                tmpPtr.Copy( lineTab );
                }
            else
                {
                TFLOGSTRING( "CSatUiView::SetItemsL iIconsOn false" )
                //without icons
                tmpPtr.Copy( KTab );
                }

            if( !iSelfExplanatoryItems )
                {
                //icons not selfexplanatory show text
                tmpPtr.Append( ptr );
                }

            //memory is allocated here:
            TRAPD( err, iMenuItems->AppendL(tmpPtr) );

            if ( err )
                {
                iMenuItems->Reset();
                iMenuItemNextActions->Reset();
                CleanupStack::PopAndDestroy( correctText );
                CleanupStack::PopAndDestroy( tmpBuf ); //tmpBuf, correctText
                TFLOGSTRING( "CSatUiView::SetItemsL having error" )
                return ESatFailure;
                }
            TFLOGSTRING2( "CSatUiView::SetItemsL tmpPtr=%S", &tmpPtr )
            CleanupStack::PopAndDestroy( correctText );
            CleanupStack::PopAndDestroy( tmpBuf ); //tmpBuf, correctText
            //select options menu command
            if ( nextActions )
                {
                TRAP( err, iMenuItemNextActions->AppendL(
                    aMenuItemNextActions->At( i ) ) );

                if ( err )
                    {
                    iMenuItems->Reset();
                    iMenuItemNextActions->Reset();
    				TFLOGSTRING( "CSatUiView::SetItemsL return ESatFailure" )
                    return ESatFailure;
                    }
                }
            }
        }

    iHelpIsAvailable = aHelpIsAvailable;

	TFLOGSTRING( "CSatUiView::SetItemsL return ESatSuccess" )
    return ESatSuccess;
    }

// ---------------------------------------------------------
// CSatUiView::SetUpMenuL
// Checks whether the same view should be refreshed or another
// one activated and do it.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiView::SetUpMenuL(
    HBufC* aTitle,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const TBool aHelpIsAvailable )
    {
	TFLOGSTRING( "CSatUiView::SetUpMenuL called" )
    iDefaultItem = 0; //could be iSelection

    CSatUiView* setUpMenuView = STATIC_CAST( CSatUiView*,
        AppUi()->View( KSetUpMenuViewId ) );

    TVwsViewId setUpMenuViewId( KUidSATUI, KSetUpMenuViewId );
    TVwsViewId viewId;

    AppUi()->GetActiveViewId( viewId );

    setUpMenuView->SetItemsL( aTitle, aMenuItems, aMenuItemNextActions,
        aHelpIsAvailable );
        
    // Incase empty view is created before the actual SetUpMenu view
    // CBA resource has to be updated
    if ( iEmpty )
        {
        TFLOGSTRING( "CSatUiView::SetUpMenuL empty view" )
        iEmpty = EFalse;
        CEikButtonGroupContainer* cba = Cba();
        
        if ( cba )
            {
            TFLOGSTRING( "CSatUiView::SetUpMenuL cba" )
            cba->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_EXIT );
            cba->DrawDeferred();
            }
        }

    if ( viewId.iAppUid == setUpMenuViewId.iAppUid )
        {        
        TFLOGSTRING( "CSatUiView::SetUpMenuL activate SetUpMenuView" )        
        //Activate SetUpMenuView
        AppUi()->ActivateLocalViewL( KSetUpMenuViewId );

        if ( iContainer )
            {
            RefreshL();
            }
        }
	TFLOGSTRING( "CSatUiView::SetUpMenuL return ESatSuccess" )
    return ESatSuccess;
    }

// ---------------------------------------------------------
// CSatUiView::SetAdapter
// Sets a pointer to MSatUiAdapter provided by SatCli.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::SetAdapter( MSatUiAdapter* aAdapter )
    {
    TFLOGSTRING( "CSatUiView::SetAdapter called" )
    iAdapter = aAdapter;
    TFLOGSTRING( "CSatUiView::SetAdapter exit" )
    }

// ---------------------------------------------------------
// CSatUiSelectItemView::SelectItemL
// Checks whether the same view should be refreshed or another
// one activated and do it.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiView::SelectItemL(
    HBufC* aTitle,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const TInt aDefaultItem, TUint8& aSelection,
    const TBool aHelpIsAvailable )
    {
	TFLOGSTRING( "CSatUiView::SelectItemL called" )
    iDefaultItem = aDefaultItem;
    TFLOGSTRING2( "CSatUiView::SelectItemL in aDefaultItem = %d", aDefaultItem )
    CSatUiView* selectItemView = STATIC_CAST( CSatUiView*,
        AppUi()->View( KSelectItemViewId ) );

    TVwsViewId selectItemViewId( KUidSATUI,KSelectItemViewId );
    TVwsViewId viewId;

    AppUi()->GetActiveViewId( viewId );

    selectItemView->SetItemsL( aTitle, aMenuItems, aMenuItemNextActions,
        aHelpIsAvailable);

    if ( viewId.iAppUid == selectItemViewId.iAppUid )
        {        
        TFLOGSTRING( "CSatUiView::SelectItemL activate SelectItemview" )        
        //Activate SelectItemview
        AppUi()->ActivateLocalViewL( KSelectItemViewId );
    
        if ( iContainer )
            {
            RefreshL();
            }
        }

    iResponse = ESatSuccess;

    iNoResponseFromUserAfterDelay = EFalse;

    //activitymanager for wait user actions
    CUserActivityManager* activityManager = 
        CUserActivityManager::NewL( EPriorityNormal );

    CleanupStack::PushL( activityManager );

	User::ResetInactivityTime();
    activityManager->Start( TTimeIntervalSeconds( KNoResponseFromUserDelay ), 
        TCallBack( DispatchUserInactive, this ), 0 );

    if ( !iWait.IsStarted() )
        {
        TFLOGSTRING( "CSatUiView::SelectItemL start iWait" )
        iSatUiAppUi->DispatchTimer( iWait );
        iWait.Start();
        }

    CleanupStack::PopAndDestroy( activityManager ); // activityManager

    if ( iNoResponseFromUserAfterDelay )
        {
        iResponse = ESatNoResponseFromUser;
        }

    aSelection= STATIC_CAST(TUint8,iSelection);
    TFLOGSTRING2( "CSatUiView::SelectItemL out aDefaultItem = %d", aDefaultItem )
	TFLOGSTRING2( "CSatUiView::SelectItemL exit, return: %d", iResponse )
    return iResponse;
    }

// ---------------------------------------------------------
// CSatUiView::Id
// Returns the id of the view.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUid CSatUiView::Id() const
    {
	TFLOGSTRING( "CSatUiView::Id() called" )
    //this should be an UID.
    if ( iSetUpMenu )
        {
        TFLOGSTRING( "CSatUiView::Id() setupmenu-exit" )
        return KSetUpMenuViewId;
        }
    else
        {
        TFLOGSTRING( "CSatUiView::Id() selectitem-exit" )
        return KSelectItemViewId;
        }
    }

// ---------------------------------------------------------
// CSatUiView::HandleCommandL
// Called by the UI framework when a command has been issued.
// A command can originate by pressing a softkey or 
// by selection of the options menu item.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::HandleCommandL(
    TInt aCommandId)
    {
	TFLOGSTRING( "CSatUiView::HandleCommandL called" )
    if ( iKeypadBlocked || ( EAknSoftkeyEmpty == aCommandId ) )
        {
        return;
        }
        
    TFLOGSTRING2( "CSatUiView::HandleCommandL iNItems: %d", iNItems )
    if ( iNItems > 0 )
        {
        iSelection = iContainer->ListBox()->CurrentItemIndex();
        }

    TBool taskSwapper ( EFalse );
    
    TFLOGSTRING2( "CSatUiView::HandleCommandL aCommandId: %d", aCommandId )
    switch ( aCommandId )
        {
        case ESatUiCmdSelect:
            {
            iSatUiAppUi->HelpStatus( EFalse );

            if ( iSetUpMenu )
                {
                TFLOGSTRING( "CSatUiView::HandleCommandL setupmenu" )
                iSatUiAppUi->ShowWaitNoteL();
                iAdapter->MenuSelection( iSelection, EFalse );
                }
            else
                {
                TFLOGSTRING( "CSatUiView::HandleCommandL others" )
                iResponse = ESatSuccess;
				iWait.AsyncStop();
                }
            break;
            }

        case EAknSoftkeyBack:
            {
            iResponse = ESatBackwardModeRequestedByUser;
            iWait.AsyncStop();
            break;
            }

        case EAknSoftkeyExit:
        case EEikCmdExit:
            {
            AknSelectionService::HandleMarkableListProcessCommandL( 
                EAknCmdExit, iContainer->ListBox() );
            STATIC_CAST( CSatUiViewAppUi*,
                AppUi())->HandleCommandL( EEikCmdExit );
            break;
            }
        case ESatUiCmdExit:
            {
            TFLOGSTRING( "CSatUiView::HandleCommandL ESatUiCmdExit" )
            iResponse = ESatSessionTerminatedByUser;
            if ( iWait.IsStarted() )
                {
                iWait.AsyncStop();
                TFLOGSTRING( "CSatUiView::HandleCommandL iWait stopped" )
                }
            break;
            }
        case EAknCmdHelp:
			if ( iHelpIsAvailable )
                {
                TFLOGSTRING( "CSatUiView::HandleCommandL help available" )
                iSatUiAppUi->HelpStatus( ETrue );

                if ( iSetUpMenu )
                    {
                    TFLOGSTRING( "CSatUiView::HandleCommandL setupmenu" )
                    iAdapter->MenuSelection( iSelection, ETrue );
                    }
                else
                    {
                    TFLOGSTRING( "CSatUiView::HandleCommandL others" )
                    iResponse = EHelpRequestedByUser;
                    iWait.AsyncStop();
                    }
                }
            else
                {
                TFLOGSTRING( "CSatUiView::HandleCommandL help not available" )
                // Show Help
                STATIC_CAST( CSatUiViewAppUi*, 
								AppUi())->HandleCommandL( EAknCmdHelp );
                }
			break;
        case EAknCmdTaskSwapper:
            {
            taskSwapper = ETrue;
            break;
            }
		default:
            break;
        }

    if ( ( ( iNItems > 0 )) && ( iContainer->ListBox()->ScrollBarFrame() ) )
        {
        iContainer->ListBox()->ScrollBarFrame()->MoveVertThumbTo(
            iSelection );
        }
        
    if ( ( ( EAknCmdHelp == aCommandId ) && !iHelpIsAvailable ) || taskSwapper )
        {
        TFLOGSTRING( "CSatUiView::HandleCommandL iKeypadBlocked false" )
        iKeypadBlocked = EFalse;
        }
    else
        {
        TFLOGSTRING( "CSatUiView::HandleCommandL iKeypadBlocked true" )
        iKeypadBlocked = ETrue;
        }
        
	TFLOGSTRING( "CSatUiView::HandleCommandL exit" )
    }

// ---------------------------------------------------------
// CSatUiView::DynInitMenuPaneL
// Update the options menu when opened.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::DynInitMenuPaneL(
    TInt aResourceId,
    CEikMenuPane* aMenuPane )
    {
    TFLOGSTRING( "CSatUiView::DynInitMenuPaneL called" )
    if (( aResourceId == R_SATUI_SETUPMENU_OPTIONS_MENU ) ||
        ( aResourceId == R_SATUI_SELECTITEM_OPTIONS_MENU ) )
        {
        
        FeatureManager::InitializeLibL();      
          
        if ( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }

        FeatureManager::UnInitializeLib();

        TInt nItems( aMenuPane->NumberOfItemsInPane() );

        TFLOGSTRING2( "CSatUiView::DynInitMenuPaneL nItems: %d", nItems )
        if ( nItems > 0 )    //number of items in the options menu
            {
            TFLOGSTRING2( "CSatUiView::DynInitMenuPaneL iNItems: %d", \
                iNItems )
            if ( iNItems > 0 )    //number of items in the main pane
                {
                TInt sItem = iContainer->ListBox()->CurrentItemIndex();
                TFLOGSTRING2( "CSatUiView::DynInitMenuPaneL sItem: %d", sItem )
                HBufC* menuItemNextActions;
                TInt resource( R_QTN_SAT_OPTION_SELECT );

                if ( iMenuItems->MdcaCount() ==
                    iMenuItemNextActions->Count() )
                    {
                    switch ( ( *iMenuItemNextActions )[ sItem ] )
                        {
                        case ESatSetUpCallAction:
                            {                            
                            TFLOGSTRING( "CSatUiView::DynInitMenuPaneL \
                                setupcall" )
                            resource = R_QTN_SAT_OPTION_CALL;
                            break;
                            }
                        case ESatSendSsAction:   // flow through
                        case ESatSendUssdAction: // flow through
                        case ESatSendSmAction:
                            {                            
                            TFLOGSTRING( "CSatUiView::DynInitMenuPaneL \
                                sendss,sendussd or sendsm" )
                            resource = R_QTN_SAT_OPTION_SEND;
                            break;
                            }
                        case ESatPlayToneAction:
                            {                            
                            TFLOGSTRING( "CSatUiView::DynInitMenuPaneL \
                                playtone" )
                            resource = R_QTN_SAT_OPTION_PLAY_TONE;
                            break;
                            }
                        case ESatDisplayTextAction:
                            {                            
                            TFLOGSTRING( "CSatUiView::DynInitMenuPaneL \
                                displaytext" )
                            resource = R_QTN_SAT_OPTION_READ;
                            break;
                            }
                        case ESatSetUpMenuAction:
                        case ESatSelectItemAction:
                            {                            
                            TFLOGSTRING( "CSatUiView::DynInitMenuPaneL \
                                setupmenu or selectitem" )
                            resource = R_QTN_SAT_OPTION_OPEN;
                            break;
                            }
                        case ESatLaunchBrowserAction:
                            {                            
                            TFLOGSTRING( "CSatUiView::DynInitMenuPaneL \
                                launchbrowser" )
                            resource = R_QTN_SAT_OPTION_GOTO;
                            break;
                            }
                        default:
                            {                            
                            TFLOGSTRING( "CSatUiView::DynInitMenuPaneL \
                                others" )
                            resource = R_QTN_SAT_OPTION_SELECT;
                            break; 
                            }
                        }
                    }
                menuItemNextActions = StringLoader::LoadLC(
                    resource, iCoeEnv );
                aMenuPane->SetItemTextL( ESatUiCmdSelect,
                    *menuItemNextActions );
                aMenuPane->SetItemSpecific( ESatUiCmdSelect, ETrue );
                // menuItemNextActions    
                CleanupStack::PopAndDestroy( menuItemNextActions ); 
                }
            else
                {
                aMenuPane->SetItemDimmed( ESatUiCmdSelect, ETrue );
                aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
                }
            }
        }
    TFLOGSTRING( "CSatUiView::DynInitMenuPaneL exit" )
    }

// ---------------------------------------------------------
// CSatUiView::HandleListBoxEventL
// Handles events that occur by selection of the menu item.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::HandleListBoxEventL(
    CEikListBox* /*aListBox*/,
    TListBoxEvent aEventType)
    {
    TFLOGSTRING( "CSatUiView::HandleListBoxEventL called" )

    if ( iKeypadBlocked || 
        ( EEventEnterKeyPressed != aEventType &&
        EEventItemSingleClicked != aEventType ) )
        {
        return;
        }

    TFLOGSTRING2( "CSatUiView::HandleListBoxEventL aEventType: %d", \
        aEventType )
    switch ( aEventType )
        {
        case EEventItemSingleClicked:
        case EEventEnterKeyPressed:

            iSatUiAppUi->HelpStatus( EFalse );

            if ( iNItems > 0 )
                {
                iSelection = iContainer->ListBox()->CurrentItemIndex();
                }

            if ( iSetUpMenu )
                {
                TFLOGSTRING( "CSatUiView::HandleListBoxEventL setupmenu" )
                iSatUiAppUi->ShowWaitNoteL();
                iAdapter->MenuSelection( iSelection, EFalse );
                }
            else
                {
                TFLOGSTRING( "CSatUiView::HandleListBoxEventL others" )
                iResponse = ESatSuccess;

                iWait.AsyncStop();
                }
            break;

        default:
            break;
        }

    iKeypadBlocked = ETrue;

    TFLOGSTRING( "CSatUiView::HandleListBoxEventL exit" )
    }

// ---------------------------------------------------------
// CSatUiView::DoActivateL
// Called by the view framework when this view is activated.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::DoActivateL(
    const TVwsViewId& PrevViewId,
    TUid /*aCustomMessageId*/,
    const TDesC8& /*aCustomMessage*/)
    {
	TFLOGSTRING( "CSatUiView::DoActivateL called" )
	
    TVwsViewId viewId;
    AppUi()->GetActiveViewId( viewId );
    
    if ( ( viewId == PrevViewId ) && ( !iIconStateChange ) )
        {
        TFLOGSTRING( "CSatUiView::DoActivateL refresh" )
        //if selectitems and icon state changed 
        RefreshL();
        }
    else
        {
        TFLOGSTRING( "CSatUiView::DoActivateL draw" )
        iContainer = CSatUiViewContainer::NewL( iIconsOn, this );

        iContainer->SetMopParent( this );
        iContainer->ListBox()->SetListBoxObserver( this );

        iModel = STATIC_CAST( CTextListBoxModel*,
            iContainer->ListBox()->Model() );

        if ( !iMenuItems )
            {
            //no memory is allocated to the array buffers here:
            iMenuItems = new (ELeave) CDesCArrayFlat(1);
            iMenuItemNextActions = new (ELeave) CArrayFixFlat<TSatAction>( 1 );
            }

        iRect = ClientRect();

        AppUi()->AddToStackL( iContainer );

        DrawL();
        }
	TFLOGSTRING( "CSatUiView::DoActivateL exit" )
    }

// ---------------------------------------------------------
// CSatUiView::DoDeactivateL
// Called by the view framework when this view is deactivated.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::DoDeactivate()
    {
	TFLOGSTRING( "CSatUiView::DoDeactivate() called" )
    if ( iContainer )
        {
        TFLOGSTRING( "CSatUiView::DoActivateL iContainer" )
        AppUi()->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }
	TFLOGSTRING( "CSatUiView::DoDeactivate() exit" )
    }

// ---------------------------------------------------------
// CSatUiView::MakeListBoxVisible
// Hides and makes list box visible if needed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::MakeListBoxVisible( TBool aVisible )
    {
    
    TFLOGSTRING( "CSatUiView::MakeListBoxVisible() called" )  
    iListBoxVisible = aVisible;
    TFLOGSTRING( "CSatUiView::MakeListBoxVisible() exit" )
    
    }

// ---------------------------------------------------------
// CSatUiView::RefreshL
// Called by SetUpMenuL and SelectItemL when view is the same but
// data is changed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::RefreshL()
    {
	TFLOGSTRING( "CSatUiView::RefreshL() called" )
    if ( !iIconStateChange )
        {
        TFLOGSTRING( "CSatUiView::RefreshL() iIconStateChange false" )
        iRect = ClientRect();
        DrawL();
        }
    else
        {
        TFLOGSTRING( "CSatUiView::RefreshL() iIconStateChange true" )
        //deactivate last selectitem view with other icon state
        DoDeactivate();

        iContainer = CSatUiViewContainer::NewL( iIconsOn, this );

        iContainer->SetMopParent( this );
        iContainer->ListBox()->SetListBoxObserver( this );

        iModel = STATIC_CAST( CTextListBoxModel*,
            iContainer->ListBox()->Model() );

        if ( !iMenuItems )
            {
            //no memory is allocated to the array buffers here:
            iMenuItems = new ( ELeave ) CDesCArrayFlat(1);
            iMenuItemNextActions = new ( ELeave ) CArrayFixFlat<TSatAction>( 1 );
            }

        iRect = ClientRect();

        AppUi()->AddToStackL( iContainer );

        DrawL();
        }
	TFLOGSTRING( "CSatUiView::RefreshL() exit" )
    }

// ---------------------------------------------------------
// CSatUiView::DrawL
// Called by DoActivateL and RefreshL to draw the view.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::DrawL()
    {
	TFLOGSTRING( "CSatUiView::DrawL() called" )
	
    if ( iContainer )
        {
        TFLOGSTRING( "CSatUiView::DrawL make ListBox visible" )
        iContainer->ListBox()->MakeVisible( iListBoxVisible );
        }
    	
    if ( ( iTitle ) && ( StatusPane()->PaneCapabilities( TUid::Uid(
        EEikStatusPaneUidTitle ) ).IsPresent() )
        && iListBoxVisible )
        {
        TFLOGSTRING( "CSatUiView::DrawL set title pane" )       
        CAknTitlePane* titlePane = STATIC_CAST( CAknTitlePane*,
            StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        titlePane->SetTextL( *iTitle );
        titlePane->DrawNow();
        }

    if( iIconsOn )
        {
        TFLOGSTRING( "CSatUiView::DrawL set icons" )
        //set icons to view
        CAknIconArray* itemIconsArray = new( ELeave ) CAknIconArray(1);
        CleanupStack::PushL( itemIconsArray );
        itemIconsArray->InsertL( 0, &iItemsIconArray->operator[]( 0 ),
            iItemsIconArray->Count() );
        iContainer->ListBox()->ItemDrawer()->ColumnData()->
            SetIconArray( itemIconsArray );
        CleanupStack::Pop( itemIconsArray );
        iItemsIconArray->Reset();
        }

    iModel->SetItemTextArray( iMenuItems );
    iModel->SetOwnershipType( ELbmDoesNotOwnItemArray );

    iContainer->MakeVisible( ETrue );
    iContainer->SetRect( iRect );
    iContainer->ActivateL();
    
    if ( iSetUpMenu )
	    {
	    TFLOGSTRING( "CSatUiView::DrawL() SetupMenu" )
	    UpdateMskL( 0, ETrue ); // SetupMenu starts at beginning
	    }
	else
	    {
	    TFLOGSTRING( "CSatUiView::DrawL() others" )
	    UpdateMskL( iDefaultItem, ETrue ); // SelectItem may have default item
	    }
    iContainer->ListBox()->SetCurrentItemIndex( iDefaultItem );
    iContainer->ListBox()->DrawNow();
    TFLOGSTRING2( "CSatUiView::DrawL() iDefaultItem=%d", iDefaultItem )
	TFLOGSTRING( "CSatUiView::DrawL() exit" )
    }

// ---------------------------------------------------------
// CSatUiView::DispatchUserInactive
// Called when user has been inactive for a pre-defined time period
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSatUiView::DispatchUserInactive( TAny* aPtr )
    {
    TFLOGSTRING( "CSatUiView::DispatchUserInactive called-exit" )
    if ( ( ( CSatUiView* )aPtr )->iWait.IsStarted() )
        {
        ( ( CSatUiView* )aPtr )->iNoResponseFromUserAfterDelay = ETrue;
        ( ( CSatUiView* )aPtr )->iWait.AsyncStop();
        }
    return ( EFalse );
    }
    

// ---------------------------------------------------------
// CSatUiView::UpdateMskL
// Updates the middle soft key
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiView::UpdateMskL( TInt aIndex, TBool aNew )
    {
    TFLOGSTRING( "CSatUiView::UpdateMskL() enter" )
    CEikButtonGroupContainer* cba = Cba();

    if ( cba && ( aNew  || ( iMenuItems->MdcaCount() ==
        iMenuItemNextActions->Count() ) ) )
        {
        TInt resource ( R_QTN_MSK_SELECT  );
        TInt commandId ( ESatUiCmdSelect );
        
        if ( iEmpty )
            {
            TFLOGSTRING( "CSatUiView::UpdateMskL iEmpty" )
            resource = R_TEXT_SOFTKEY_EMPTY;
            commandId = EAknSoftkeyEmpty;
            } 
        else if ( iMenuItems->MdcaCount() ==
            iMenuItemNextActions->Count() && iNItems )
            {
            switch ( ( *iMenuItemNextActions )[ aIndex ] )
                {
                case ESatSetUpCallAction:
                    {                    
                    TFLOGSTRING( "CSatUiView::UpdateMskL \
                                setupcall" )
                    resource = R_QTN_MSK_CALL_SAT;
                    break;
                    }
                case ESatSendSsAction:   // flow through
                case ESatSendUssdAction: // flow through
                case ESatSendSmAction:
                    {                    
                    TFLOGSTRING( "CSatUiView::UpdateMskL \
                                sendss,sendussd or sendsm" )
                    resource = R_QTN_MSK_SEND;
                    break;
                    }                        
                case ESatPlayToneAction:
                    {                    
                    TFLOGSTRING( "CSatUiView::UpdateMskL \
                                playtone" )
                    resource = R_QTN_MSK_PLAY;
                    break;
                    }
                case ESatDisplayTextAction:
                    {                    
                    TFLOGSTRING( "CSatUiView::UpdateMskL \
                                displaytext" )
                    resource = R_QTN_MSK_READ_SAT;
                    break;
                    }
                case ESatSetUpMenuAction:
                case ESatSelectItemAction:
                    {                    
                    TFLOGSTRING( "CSatUiView::UpdateMskL \
                                setupmenu or selectitem" )
                    resource = R_QTN_MSK_OPEN;
                    break;
                    }
                case ESatLaunchBrowserAction:
                    {                    
                    TFLOGSTRING( "CSatUiView::UpdateMskL \
                                launchbrowser" )
                    resource = R_QTN_MSK_GOTO_SAT;
                    break;
                    }
                default:
                    {                    
                    TFLOGSTRING( "CSatUiView::UpdateMskL \
                                others" )
                    resource = R_QTN_MSK_SELECT ;
                    break; 
                    }
                }
            TFLOGSTRING2( "CSatUiView::UpdateMskL:aIndex=%d", aIndex )
            }
        else // Basic aNew
            {
            // No need to to do anything here, default values are used
            }
            
        HBufC* msk = StringLoader::LoadLC( resource, iCoeEnv );
        cba->SetCommandL( CEikButtonGroupContainer::EMiddleSoftkeyPosition,
            commandId, *msk );
        cba->DrawDeferred();
        CleanupStack::PopAndDestroy( msk );
        }
    
    TFLOGSTRING( "CSatUiView::UpdateMskL() exit" )
    }

