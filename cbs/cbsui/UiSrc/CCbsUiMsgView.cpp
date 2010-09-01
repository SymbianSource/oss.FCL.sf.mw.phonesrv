/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Implementation of CCbsUiMsgView.
*
*
*/



// INCLUDES
#include <AknUtils.h>        //AknTextUtils
#include <bldvariant.hrh>    // for feature flags
#include <featmgr.h>  	     // for feature manager
#include <aknappui.h>
#include <aknViewAppUi.h>
#include <eikmenup.h>
#include <avkon.hrh>
#include <aknnavide.h>
#include <txtrich.h>         // CRichText
#include <AknProgressDialog.h>
#include <eikmenub.h>
#include <hlplch.h>
#include <avkon.rsg>
#include <StringLoader.h>
#include <cbsuiApp.rsg>
#include <barsread.h>
#include <AknQueryDialog.h>
#include <eikmop.h>
#include <FindItemmenu.rsg>
#include <finditemmenu.h>
#include <finditemdialog.h>
#include <akntitle.h>
#include "CCbsUIMsgView.h"
#include "Cbsuiapp.hrh"
#include "CCbsUIMsgViewContainer.h"
#include "CbsUiPanic.h"
#include "CbsUiConstants.h"
#include "CbsUiUtility.h"
#include "CbsUiDebug.h"
#include "MCbs.h"
#include "CbsUiId.h"  //KUidCbsUiappDef

 

// CONSTANTS

static const TUid KUidCbsUiappApp = { KUidCbsUiappDef };
const TInt KCbsImmediateMessageIdInt = 313;
const TInt KMaxAssumedDateTimeLength = 30;

_LIT(KSpace, " ");
_LIT(KLineBreak, "\n");
const TInt KMaxSeparatorLenght = 5;
const TInt KOneHotMarkedMessage = 1;





// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.

CCbsUiMsgView::CCbsUiMsgView( MCbs& aServer )
: 
iServer( aServer )
    {
    }

// Symbian OS default constructor can leave.
void CCbsUiMsgView::ConstructL()
    {
    BaseConstructL( R_CBSUI_MSGVIEW );   
    iFindItemMenu = CFindItemMenu::NewL( ECbsUiCmdFindItem );
    iFromOtherApplication = EFalse;

    CEikStatusPane* statusPane = StatusPane();
        
    iNaviContainer = 
    STATIC_CAST( CAknNavigationControlContainer*, statusPane->
    		ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );    
    
    }

// Two-phased constructor.
CCbsUiMsgView* CCbsUiMsgView::NewL( MCbs& aServer )
    {
    CCbsUiMsgView* self = new (ELeave) CCbsUiMsgView( aServer );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsUiMsgView::~CCbsUiMsgView()
    {
    CloseContextMenu();
    
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    delete iFindItemMenu;

    if ( iNaviContainer && iDecoratedLabel )
    	{
    	iNaviContainer->Pop ( iDecoratedLabel );
    	delete iDecoratedLabel;
    	}
	iDecoratedLabel = NULL;

	if ( iNaviContainer && iPreviousDecoratedLabel )
		{
		iNaviContainer->Pop ( iPreviousDecoratedLabel );
		delete iPreviousDecoratedLabel;
		}
	iPreviousDecoratedLabel = NULL;

    delete iMessage;
    }

// ---------------------------------------------------------
// CCbsUiMsgView::HandleNaviDecoratorEventL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::HandleNaviDecoratorEventL( TInt aEventID )
	{
	
    switch ( aEventID )
    	{
		// Left arrow tapped from the navi pane
    	case MAknNaviDecoratorObserver::EAknNaviDecoratorEventLeftTabArrow:
		    {
		    if( AknLayoutUtils::LayoutMirrored() )
		    	{
		        MoveToNextMessageL();
		        }
		    else
		        {
		        MoveToPreviousMessageL();
		        }
		    break;
		    }               
		// Right arrow tapped from the navi pane
    	case MAknNaviDecoratorObserver::EAknNaviDecoratorEventRightTabArrow:
		    {
		    if( AknLayoutUtils::LayoutMirrored() )
		    	{
		        MoveToPreviousMessageL();
		        }
		    else
		        {
		        MoveToNextMessageL();
		        }
		    break;
		    }
		default:
		    {
		    break;
		    }
    	}

	}

// ---------------------------------------------------------
// CCbsUiMsgView::DynInitMenuPaneL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::DynInitMenuPaneL( TInt aResourceId,
                                     CEikMenuPane* aMenuPane )
    {
    __ASSERT_DEBUG( aMenuPane, CbsUiPanic( EMenuPaneNullError ) );
    __ASSERT_DEBUG( iFindItemMenu, CbsUiPanic( EFindItemNullError ) );
    
    switch ( aResourceId )
        {
        case R_CBSUI_MSGVIEW_MENU:
            {
            iFindItemMenu->DisplayFindItemMenuItemL( 
                *aMenuPane, 
                ECbsUiappCmdMsgMenuDelete );
            
            if ( CbsUiUtility::IsSavedL(iCurrentHandle, iServer) )
                {
                aMenuPane->DeleteMenuItem( ECbsUiappCmdMsgMenuKeep );
                } 
            if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                aMenuPane->DeleteMenuItem( EAknCmdHelp );
                }
            break;
            }                           
        case R_FINDITEMMENU_MENU:
            {
            iFindItemMenu->DisplayFindItemCascadeMenuL( *aMenuPane );
            break;
            }
        default:
            break;
        }
    }    

// ---------------------------------------------------------
// CCbsUiMsgView::ProcessCommandL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::ProcessCommandL( TInt aCommand )
    {            
    if ( iContextMenu )
        {
        iContextMenu->StopDisplayingMenuBar();
        CloseContextMenu();
        }            

    // Context sensitive menu should be opened when MSK pressed
    switch ( aCommand )
        {    
        case EAknSoftkeyContextOptions:
            HandleCommandL( EAknSoftkeyContextOptions );
            break;            
        default:
            break;
        }
    // Call base class
    CAknView::ProcessCommandL( aCommand );
    }

// ---------------------------------------------------------
// CCbsUiMsgView::Id
// 
// 
// ---------------------------------------------------------
//
TUid CCbsUiMsgView::Id() const
    {
    return TUid::Uid( ECbsUiMsgViewId );
    }

// ---------------------------------------------------------
// CCbsUiMsgView::MoveToNextMessage
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::MoveToNextMessageL()
    {   
    // if a message is class 0, then changing the message
    // content is not allowed.
    if ( iIsClass0 )
        {
        return;
        }
    
    if ( SetHandleTo(iCurrentHandle, EToNext) )
        {
        DoLoadMessageL();
        }
    
    }

// ---------------------------------------------------------
// CCbsUiMsgView::MoveToPreviousMessage
// 
//
// ---------------------------------------------------------
//
void CCbsUiMsgView::MoveToPreviousMessageL()
    {
    // if a message is class 0, then changing the message
    // content is not allowed.
    if ( iIsClass0 )
        {
        return;
        }
    
    if ( SetHandleTo(iCurrentHandle, EToPrevious) )
        {
        DoLoadMessageL();
        }
    }

// ---------------------------------------------------------
// CCbsUiMsgView::HandleCommandL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::HandleCommandL( TInt aCommand )
    {
    if ( iFindItemMenu && iFindItemMenu->CommandIsValidL( aCommand ) )
        {
        DoSearchL( aCommand );
        }
    
    TBool exit(EFalse); // program exits if true
    
    switch ( aCommand )
        {
        case EAknSoftkeyBack:
            {
            ActivateTopicViewL( CurrentTopicNumber(), iCurrentHandle );
            break;
            }
        case ECbsUiappCmdMsgMenuKeep:
            {            
            if ( CbsUiUtility::SaveCurrentMessageL(iCurrentHandle, *this) ==
                KErrNone && iIsClass0 )
                {
                exit = ETrue;
                
                // show modal note
                CbsUiUtility::ShowConfirmationNoteL(
                    R_TEXT_NOTE_MSG_KEPT, ETrue);
                }
            break;
            }
        case ECbsUiCmdCopyToClipboard:
            {
            iContainer->CopyToClipboardL();   
            CbsUiUtility::ShowConfirmationNoteL(R_TEXT_NOTE_MSG_TO_CLIP);
            break;
            }
        case ECbsUiappCmdMsgMenuDelete:
            {            
            if ( DeleteMessageL(iCurrentHandle) && iIsClass0 )
                {
                exit = ETrue;
                }
            break;
            }
        case EAknSoftkeyContextOptions:
        case ECbsUiappCmdOkKey:
            {
            LaunchContextMenuL();
            break;
            }
        case EAknCmdHelp:
            AppUi()->HandleCommandL(aCommand);
            break;
        case EAknSoftkeyExit: // let it run through
        case EAknCmdExit:
            {
            if ( !iFromOtherApplication )
                {
                // if it has been CBSUI then do this
                ActivateTopicListViewL();
                }
            else
                {
                exit = ETrue;
                }
            iFromOtherApplication = EFalse;
            break;
            }         					   		    
        default:
            break;
        }
    
    if ( exit )
        {
        STATIC_CAST(
            MEikCommandObserver*, AppUi() )->ProcessCommandL(EAknCmdExit);
        }
    }

// ---------------------------------------------------------
// CCbsUiMsgView::DoActivateL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::DoActivateL( const TVwsViewId& aPrevViewId,
                                TUid aCustomMessageId,
                                const TDesC8& aCustomMessage )
    {   
    iIsClass0 = EFalse;
    TBool topicListViewActivated(EFalse);
    
    switch ( aCustomMessageId.iUid )
        {
        case KCbsUiCustomMessageIdInt:
            iFromOtherApplication = EFalse;
            Cba()->SetCommandSetL(R_AVKON_SOFTKEYS_OPTIONS_BACK);
            SetCurrentMessageHandle(aCustomMessage);
            break;
        case KCbsImmediateMessageIdInt:
            if ( aPrevViewId.iAppUid != KUidCbsUiappApp )
                {
                iFromOtherApplication = ETrue;
                }
            SetCurrentMessageHandle(aCustomMessage);
            Cba()->SetCommandSetL(R_AVKON_SOFTKEYS_OPTIONS_EXIT);
            Cba()->DrawDeferred();
            iIsClass0 = ETrue;
            break;
        default: // this should be soft notification
            iFromOtherApplication = EFalse;
            Cba()->SetCommandSetL(R_AVKON_SOFTKEYS_OPTIONS_BACK);
            topicListViewActivated = HandleSoftNotificationL();
            break;
        }
    
    if ( !topicListViewActivated )
        {
        if ( !iContainer )
            {
            iContainer = CCbsUiMsgViewContainer::NewL(*this, ClientRect());
            AppUi()->AddToViewStackL(*this, iContainer);
            }
        
        TInt err( DoLoadMessageL() );
        
        // if it is class 0 message and some error occurs
        if ( err != KErrNone && iIsClass0 )
            {
            User::Leave(err);
            }
        
        if ( iMessage )
            {
            iContainer->ActivateL();
            }
        else
            {
            // some error occured
            ActivateTopicViewL( CurrentTopicNumber(), iCurrentHandle );
            }
        }
    }

// ---------------------------------------------------------
// CCbsUiMsgView::DoDeactivate()
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::DoDeactivate()
    {
    iServer.LockMessage(NULL);
    
    CloseContextMenu();
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
    
    delete iDecoratedLabel;
    iDecoratedLabel = NULL;
    
    DeleteOldMessage();
    }

// ---------------------------------------------------------
// CCbsUiMsgView::UpdateNaviPaneL
// 
//  
// ---------------------------------------------------------
//
void CCbsUiMsgView::UpdateNaviPaneL()
    {
    
    TInt currentPosition( KCbsErrValue );
    TInt totalMessages( KCbsErrValue );
    
    TInt err (
        GetMessagePosition( iCurrentHandle, currentPosition, totalMessages ) );

    TBool prev( currentPosition != 0 );
    TBool next( ( currentPosition + 1 ) < totalMessages );
    
    // If a message is class 0, then the navipane is empty
    if ( iIsClass0 || err != KErrNone )
        {
        // Delete the existed label before creating a new one 
        // in case of memory leak
        CAknNavigationDecorator* oldLabel = iDecoratedLabel;
        iDecoratedLabel = CbsUiUtility::CreateEmptyNavigationPaneL();
        delete oldLabel;
        return;
        }
    
    // Create a new decorator    
    CArrayFixFlat<TInt>* array = new( ELeave ) CArrayFixFlat<TInt>( 2 );
    CleanupStack::PushL( array );
    array->AppendL( currentPosition + 1 );
    array->AppendL( totalMessages );
    HBufC* messageLocationInTopic = StringLoader::LoadL( R_MESSAGE_LOCATION_IN_TOPIC, *array, iCoeEnv );
    CleanupStack::PopAndDestroy(); // array
    CleanupStack::PushL( messageLocationInTopic );

    CAknNavigationDecorator* newDecoratedLabel = iNaviContainer->CreateNavigationLabelL( *messageLocationInTopic );
    CleanupStack::PopAndDestroy(); // messageLocationInTopic

    CleanupStack::PushL( newDecoratedLabel );
    
    // Set new decorator on top of navi pane's decorator stack
    // ( topmost decorator is the one to show )
    iNaviContainer->PushL( *newDecoratedLabel );

    // Remove previous decorator from decorator stack and delete it
    if ( iPreviousDecoratedLabel )
    	{
    	iNaviContainer->Pop( iPreviousDecoratedLabel );        
    	delete iPreviousDecoratedLabel;
    	iPreviousDecoratedLabel = NULL;
    	}

    // Update decorator pointers to new ones
    iPreviousDecoratedLabel = iDecoratedLabel;
    iDecoratedLabel = newDecoratedLabel;
    
    __ASSERT_DEBUG( iDecoratedLabel, CbsUiPanic( EErrNullPointer ) );
   
    // Set observer for navi pane
    iDecoratedLabel->SetNaviDecoratorObserver( this );

    CleanupStack::Pop( newDecoratedLabel );
    
    iDecoratedLabel->MakeScrollButtonVisible( ETrue );

    if( AknLayoutUtils::LayoutMirrored() )
        {
        iDecoratedLabel->SetScrollButtonDimmed( CAknNavigationDecorator::ELeftButton, !next );
        iDecoratedLabel->SetScrollButtonDimmed( CAknNavigationDecorator::ERightButton, !prev );
        }
    else
        {
    	iDecoratedLabel->SetScrollButtonDimmed( CAknNavigationDecorator::ELeftButton, !prev );
    	iDecoratedLabel->SetScrollButtonDimmed( CAknNavigationDecorator::ERightButton, !next );
        }

    }


// ---------------------------------------------------------
// CCbsUiMsgView::SaveMessage
// 
//  
// ---------------------------------------------------------
//
TInt CCbsUiMsgView::SaveMessage( const TCbsMessageHandle& aHandle )
    {
    return iServer.SaveMessage(aHandle);
    }

// ---------------------------------------------------------
// CCbsUiMsgView::DoSearchL
// 
//  
// ---------------------------------------------------------
//
void CCbsUiMsgView::DoSearchL( TInt aCommand )
    {
    CFindItemDialog* dialog =  CFindItemDialog::NewL( 
        iContainer->GetEditor()->Read(0), 
        iFindItemMenu->SearchCase(aCommand) );
    dialog->ExecuteLD();
    }

// ---------------------------------------------------------
// CCbsUiMsgView::TopicListChangedL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::TopicListChangedL( TCbsTopicListEvent /*aEvent*/, TCbsTopicNumber aTopicNumber )
    {
    if ( iContainer && (CurrentTopicNumber() == aTopicNumber) )
        {
        UpdateNaviPaneL();
        }
    }



// ---------------------------------------------------------
// CCbsUiMsgView::HandleRectChangeInContainerL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::HandleRectChangeInContainerL()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        iContainer->UpdateMessageL( *iMessage );
        }
    
    }

// ---------------------------------------------------------
// CCbsUiMsgView::CloseContextMenu
// 
//  
// ---------------------------------------------------------
//
void CCbsUiMsgView::CloseContextMenu()
    {
    if ( iContextMenu )
        {
        AppUi()->RemoveFromViewStack(*this, iContextMenu);
        delete iContextMenu;
        iContextMenu = NULL;
        }
    }

// ---------------------------------------------------------
// CCbsUiMsgView::LaunchContextMenuL
// 
//  
// ---------------------------------------------------------
//
void CCbsUiMsgView::LaunchContextMenuL()
    {        
    TInt rssName = R_CBSUI_MSG_KEEP_AND_DELETE_CONTEXT_MENUBAR;
    
    if ( CbsUiUtility::IsSavedL(iCurrentHandle, iServer) )
        {                        
        rssName = R_CBSUI_MSG_DELETE_CONTEXT_MENUBAR;
        }   
    
    CloseContextMenu();
    
    __ASSERT_DEBUG( !iContextMenu, CbsUiPanic(EErrAllreadyExists) );
    
    iContextMenu = new (ELeave) CEikMenuBar;    
    iContextMenu->ConstructL(
        this,
        0,
        rssName );
    
    // Set type to context sensitive
    iContextMenu->SetMenuType( CEikMenuBar::EMenuContext );
    
    AppUi()->AddToViewStackL( 
        *this, 
        iContextMenu, 
        ECoeStackPriorityMenu, 
        ECoeStackFlagRefusesFocus );
    
    iContextMenu->TryDisplayMenuBarL();
    }

// ---------------------------------------------------------
// CCbsUiMsgView::DeleteOldMessage
// 
//
// ---------------------------------------------------------
//
void CCbsUiMsgView::DeleteOldMessage()
    {
    delete iMessage;
    iMessage = NULL;    
    }

// ---------------------------------------------------------
// CCbsUiMsgView::DoLoadMessageL
// 
//
// ---------------------------------------------------------
//
TInt CCbsUiMsgView::DoLoadMessageL()
    {
    TRAPD( err, LoadMessageL() );
    
    switch ( err ) 
        {
        case KErrNotFound:
            {
            // If the message cannot be found
            // it is not necessary to do anything.
            // It is just showing some other message (current).
            break;
            }
        case KErrNone:
            {
            // normal case
            __ASSERT_DEBUG( iMessage, CbsUiPanic(EErrNullPointer) );
            iContainer->UpdateMessageL(*iMessage);
            // navi pane has to be updated in all cases; expect when leave
            UpdateNaviPaneL();
            Cba()->SetCommandSetL( R_CBS_SOFTKEYS_OPTIONS_BACK__CONTEXTOPTIONS );
            break;
            }
        default:
            User::Leave( err );
            break;
        }
    
    return err;
    }

// ---------------------------------------------------------
// CCbsUiMsgView::ActivateTopicViewL
// 
//
// ---------------------------------------------------------
//
void CCbsUiMsgView::ActivateTopicViewL( TCbsTopicNumber aTopicNumber, TCbsMessageHandle aHandle )
    {  
    TPckgBuf<TCbsUiTopicActivationPckg> message;
    message().iTopicNumber = aTopicNumber;
    message().iMessageHandle = aHandle;
    
    AppUi()->ActivateLocalViewL( 
        TUid::Uid(ECbsUiTopicViewId), KSetFocusToId, message );
    }

// ---------------------------------------------------------
// CCbsUiMsgView::SetCurrentMessageHandle
// 
//
// ---------------------------------------------------------
//
void CCbsUiMsgView::SetCurrentMessageHandle( const TDesC8& aMessageHandle )
    {
    TPckgBuf<TCbsMessageHandle> pckgMessageHandle;
    pckgMessageHandle.Copy(aMessageHandle);
    iCurrentHandle = pckgMessageHandle();
    }

// ---------------------------------------------------------
// CCbsUiMsgView::LoadMessageL
// 
//
// ---------------------------------------------------------
//
void CCbsUiMsgView::LoadMessageL()
    {      
    User::LeaveIfError( iServer.LockMessage(iCurrentHandle) );
    
    TCbsMessage message;
    TInt err( iServer.FindMessageByHandle(iCurrentHandle, message) );
    __CBSUI_ASSERT( err == KErrNone, CbsUiPanic(EErrMsgHandleNotFound) );
    User::LeaveIfError(err);
    
    // mark the message as read 
    err = iServer.ReadMessage(iCurrentHandle);
    User::LeaveIfError(err);
    
    DeleteOldMessage();
    iMessage = HBufC::NewMaxL(message.iLength);
    TPtr messageContent = iMessage->Des();
    
    err = iServer.GetMessageContents(iCurrentHandle, messageContent);
    
    __CBSUI_ASSERT( err == KErrNone, 
        CbsUiPanic(EMessageContentsError) );
    
    if ( err != KErrNone )
        {
        DeleteOldMessage();
        User::Leave(err);
        }
    
    AddDateAndTimeL( 
        message.iDateTime,
        &iMessage, // address of pointer variable
        R_CBS_DATE_FORMAT_ARRAY );
    }

// ---------------------------------------------------------
// CCbsUiMsgView::AddDateAndTime
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::AddDateAndTimeL( const TInt64 aTime, HBufC** aMessage, TUint aResourceArrayId ) const
    {
    __ASSERT_DEBUG( aTime >= 0, CbsUiPanic(EErrInvalidValue) );
    __ASSERT_DEBUG( *aMessage, CbsUiPanic(EErrNullPointer) );
    __ASSERT_DEBUG( (*aMessage)->Length() > 0, CbsUiPanic(EErrEmptyDesc) );
    __ASSERT_DEBUG( aResourceArrayId > 0, CbsUiPanic(EErrInvalidValue) );
    
    HBufC* timeBuffer = HBufC::NewMaxLC( KMaxAssumedDateTimeLength );
    TInt bufLen( 0 );
    TTime time( aTime );
    
    // change universal time to local time
    TLocale locale;
    time += locale.UniversalTimeOffset();
    
    TPtr des = timeBuffer->Des();    
    TInt length( ( *aMessage )->Length() );
    
    // for adding date and time to message
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC( reader, aResourceArrayId );
    TInt count( reader.ReadInt16() );
    
    TBuf<KMaxSeparatorLenght> separator( KLineBreak );
    TInt separatorsLength( KLineBreak().Length() + KSpace().Length() );
    
    for ( TInt i( 0 ); i < count; i++ )
        {
        TUint resourceId( reader.ReadUint32() );
        
        HBufC* format = iCoeEnv->AllocReadResourceLC( resourceId );
        
        time.FormatL( des, *format );
        AknTextUtils::DisplayTextLanguageSpecificNumberConversion( des );
        
        if ( i > 0 )
            {
            separator = KSpace;
            }
        
        bufLen += timeBuffer->Length();
        (*aMessage) = (*aMessage)->ReAllocL( 
            length + bufLen +  separatorsLength );
        
        // have to get a new pointer after reallocation
        TPtr ptr2 = ( *aMessage )->Des();
        ptr2.Append( separator );
        ptr2.Append( *timeBuffer );
        
        CleanupStack::PopAndDestroy(); // format
        }
    
    CleanupStack::PopAndDestroy( 2 ); // timeBuffer, reader
    }

// ---------------------------------------------------------
// CCbsUiMsgView::CurrentTopicNumber
// 
// 
// ---------------------------------------------------------
//
TCbsTopicNumber CCbsUiMsgView::CurrentTopicNumber() const
    {
    return STATIC_CAST( TCbsTopicNumber, (iCurrentHandle >> 16) & 0xFFFF );
    }

// ---------------------------------------------------------
// CCbsUiMsgView::GetMessagePosition
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiMsgView::GetMessagePosition( TCbsMessageHandle aHandle, TInt& aPosition, TInt& aTotal )
    {
    TInt err( iServer.GetMessageIndexByHandle(aHandle, aPosition) );
    
    __CBSUI_ASSERT( err == KErrNone, CbsUiPanic(EErrMsgHandleNotFound) );
    __CBSUI_ASSERT( aPosition != KCbsErrValue, CbsUiPanic(EErrInvalidValue) );
    
    if ( err == KErrNone )
        {
        err = iServer.GetMessageCount( CurrentTopicNumber(), aTotal );
        }
    
    __CBSUI_ASSERT( err == KErrNone, CbsUiPanic(EErrMsgHandleNotFound) );
    __CBSUI_ASSERT( aPosition + 1 <= aTotal, CbsUiPanic(EErrMsgImbalance) );
    
    return err;
    }

// ---------------------------------------------------------
// CCbsUiMsgView::SetHandleTo
// 
// 
// ---------------------------------------------------------
//
TBool CCbsUiMsgView::SetHandleTo( TCbsMessageHandle& aHandle, TMsgViewSetHandle aTo )
    {
    TCbsMessageHandle prev;
    TCbsMessageHandle next;
    TInt position(KCbsErrValue);
    TBool result(EFalse);
    
    TInt err( iServer.GetNextAndPrevMessageHandle(
        aHandle, prev, next, position) );
    
    if ( err == KErrNone )
        {
        __ASSERT_DEBUG( 
            position >= 0 && position <= (ECbsTail | ECbsHead), 
            CbsUiPanic(EErrInvalidValue) );           
        
        switch ( aTo )
            {
            case EToPrevious:
                // if position is head it cannot go thru
                if ( (position & ECbsHead) == 0 )
                    {
                    aHandle = prev;
                    result = ETrue;
                    }
                break;
            case EToNext:
                // if position is tail it cannot go thru
                if ( (position & ECbsTail) == 0 )
                    {
                    aHandle = next;
                    result = ETrue;
                    }
                break;
            default:
                __ASSERT_DEBUG(0, CbsUiPanic(EErrInvalidValue));
                break;
            }
        }        
    
    return result;
    }

// ---------------------------------------------------------
// CCbsUiMsgView::DeleteMessageL
// 
// 
// ---------------------------------------------------------
//
TBool CCbsUiMsgView::DeleteMessageL( TCbsMessageHandle aHandle )
    {
    TBool result(EFalse);
    
    CAknQueryDialog* dlg = CAknQueryDialog::NewL();    
    
    if ( dlg->ExecuteLD(R_DELETE_MSG_CONFIRMATION_QUERY) )
        {
        TCbsMessageHandle handleToBeFocused(aHandle);        
        
        if ( !SetHandleTo(handleToBeFocused, EToNext) )
            {
            if ( !SetHandleTo(handleToBeFocused, EToPrevious) )
                {
                // next or previous could not be set, 0 will unlock message
                handleToBeFocused = 0;
                }
            }
        
        // Unlock message to be deleted and lock next message before delete
        __ASSERT_DEBUG(handleToBeFocused != aHandle, 
            CbsUiPanic(EErrInvalidValue));
        
        iServer.LockMessage(handleToBeFocused);
        iServer.DeleteMessage(aHandle);
        
        // free handleToBeFocused message
        if ( handleToBeFocused != 0)
            {
            iServer.LockMessage(NULL);
            }
        
        // Topic number is still the same even if the message is
        // deleted.
        ActivateTopicViewL( CurrentTopicNumber(), handleToBeFocused );
        result = ETrue;
        }
    
    return result;
    }

// ---------------------------------------------------------
// CCbsUiMsgView::ActivateTopicListViewL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiMsgView::ActivateTopicListViewL() const
    {
    AppUi()->ActivateLocalViewL( TUid::Uid(ECbsUiTopicListViewId) );
    }

// ---------------------------------------------------------
// CCbsUiMsgView::HandleSoftNotification
// 
// 
// ---------------------------------------------------------
//
TBool CCbsUiMsgView::HandleSoftNotificationL()
    {
    TCbsTopic topic;
    TInt err;
    TBool result( EFalse );
    TInt count( iServer.NumberOfUnreadHotmarkedMessages() );
    __ASSERT_DEBUG( 
        count >= KOneHotMarkedMessage, CbsUiPanic(EErrInvalidValue) );
    
    switch ( count )
        {
        case KOneHotMarkedMessage:
            iServer.GetHotmarkedMessageHandle( iCurrentHandle );
            err = iServer.FindTopicByNumber( CurrentTopicNumber(), topic );
            if ( err == KErrNone )
                {
                //Update Title pane
                CEikStatusPane* sp = StatusPane();
                CAknTitlePane* title = 
                    STATIC_CAST(CAknTitlePane*, sp->ControlL( TUid::Uid( EEikStatusPaneUidTitle) ) );
                title->SetTextL( topic.iName );
                }
            break;
        default: // Many hot marked messages
            ActivateTopicListViewL();
            result = ETrue;
            break;
        }
    
    return result;
    }


//  End of File  
