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
*       Implementation of CCbsUiTopicView.
*
*
*/


// INCLUDES
#include <AknUtils.h>        // AknTextUtils
#include <bldvariant.hrh>    // for feature flags
#include <featmgr.h>         // for feature manager
#include <aknViewAppUi.h>
#include <aknnavide.h>
#include <calslbs.h>
#include <eikclb.h>
#include <akntitle.h>
#include <eikmenub.h>
#include <eikmenup.h>
#include <hlplch.h>
#include <AknQueryDialog.h>
#include <barsread.h>        // TResourceReader
#include <cbsuiApp.rsg>
#include <e32std.h>
#include "CCbsUiTopicView.h"
#include "CCbsUiTopicViewContainer.h"
#include "Cbsuiapp.hrh"
#include "CbsUiPanic.h"
#include "CbsUiUtility.h"
#include "CbsUiConstants.h"
#include "CbsUiDebug.h"
#include "MCbs.h"
#include <StringLoader.h>
#include <AknNaviDecoratorObserver.h>
#include <aknnavi.h>



// CONSTANTS
const TUint KZeroFill = '0';
const TUint KCbsMSKControlID = 3;


// MODULE DATA STRUCTURES  

// buffer to be used to unpack action information
typedef TPckgBuf<TCbsUiTopicActivationPckg> TTopicNumberBuf;

// index for setting the focus
enum TTopicViewFocusIndexes 
    { 
    EFirstMessage = 0
    };



//=============================================================================
//  MEMBER FUNCTIONS
// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiTopicView::CCbsUiTopicView( MCbs& aServer )
: iServer( aServer )
    {
    }

// ---------------------------------------------------------
// CCbsUiTopicView::ConstructL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::ConstructL()
    {
    BaseConstructL( R_CBSUI_TOPICVIEW );
    
    // Create array for handles.
    iHandles = new (ELeave) TCbsUiMessageHandleList(
        KNumberOfTemporaryMessages); 
    
    CEikStatusPane* sp = StatusPane();
 
    iNaviContainer = 
        STATIC_CAST( CAknNavigationControlContainer*, sp->
        ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );    
    }


// Two-phased constructor.
CCbsUiTopicView* CCbsUiTopicView::NewL( MCbs& aServer )
    {
    __ASSERT_DEBUG( &aServer, CbsUiPanic(EErrNullPointer) );
    
    // Normal two phase construction.
    CCbsUiTopicView* self = 
        new (ELeave) CCbsUiTopicView(aServer);
    
    CleanupStack::PushL( self );
    self->ConstructL(); 
    CleanupStack::Pop();
    
    return self;
    }

// Destructor
CCbsUiTopicView::~CCbsUiTopicView()
    {	
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    delete iHandles;
    
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
    }

// ---------------------------------------------------------
// CCbsUiTopicView::ReloadMessagesL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::ReloadMessagesL()
    {
    TCbsTopic topic;
    TInt err( iServer.FindTopicByNumber(iCurrentTopicNumber, topic) );
    
    if ( err != KErrNone )
        {
        ActivateTopicListViewL(iCurrentTopicNumber);
        return;
        }
    
    UpdateTitlePaneL(topic);
    UpdateNaviPaneL(topic);
    
    PrepareReloading();
    LoadAllMessageTitlesL(iCurrentTopicNumber);
    ActivateListBoxL();
    
    // Set the MSK according to message count
    if ( !iHandles->Count() )
        {
        RemoveCommandFromMSK();
        SetMiddleSoftKeyLabelL( R_TEXT_SOFTKEY_EMPTY, ECbsUiappNullCmd );
        }
    else
        {
        RemoveCommandFromMSK();
        SetMiddleSoftKeyLabelL( R_QTN_MSK_OPEN, ECbsUiappCmdTopicMSKRead );
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::TotalMessages
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicView::TotalMessages() const
    {
    return iHandles->Count();
    }

// ---------------------------------------------------------
// CCbsUiTopicView::UpdateTitlePaneL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::UpdateTitlePaneL( const TCbsTopic& aTopic ) const
    {
    CEikStatusPane* sp = StatusPane();
    CAknTitlePane* title = 
        STATIC_CAST(
        CAknTitlePane*, sp->ControlL( TUid::Uid(EEikStatusPaneUidTitle) ) );
    
    __ASSERT_DEBUG( sp && title, CbsUiPanic(EStatusOrTitlePaneError) );
    
    title->SetTextL(aTopic.iName);
    
    }

// ---------------------------------------------------------
// CCbsUiTopicView::UpdateNaviPaneL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::UpdateNaviPaneL( const TCbsTopic& aTopic )
    {   
    TBool next( EFalse );
    TBool previous( EFalse );
    TInt err( CheckIfCanMove( previous, next ) );
    
    if ( err != KErrNone )
        {
        iDecoratedLabel = CbsUiUtility::CreateEmptyNavigationPaneL();
        return;
        }
    
    TBuf<EMaxTopicNumberLength> number;
    TBuf<EMaxTopicNumberLength> buffer;
    
    buffer.AppendNum( TInt( aTopic.iNumber ) );
    number.Justify( buffer, EMaxTopicNumberLength, ERight, KZeroFill );
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( number );
    
    //  Create a new decorator
    CAknNavigationDecorator* newDecoratedLabel = 
    						iNaviContainer->CreateNavigationLabelL ( number );
    CleanupStack::PushL( newDecoratedLabel );
    
    // Set new decorator on top of navi pane's decorator stack 
    // (topmost decorator is the one to show)
    iNaviContainer->PushL( *newDecoratedLabel );
    
    // remove previous decorator from decorator stack and delete it
    if ( iPreviousDecoratedLabel )
    	{
    	iNaviContainer->Pop( iPreviousDecoratedLabel );
    	delete iPreviousDecoratedLabel;
    	iPreviousDecoratedLabel = NULL;
    	}
    
    // update decorator pointers to new ones
    iPreviousDecoratedLabel = iDecoratedLabel;
    iDecoratedLabel = newDecoratedLabel;
    
    // set observer for navi pane
    iDecoratedLabel->SetNaviDecoratorObserver( this );
    
    CleanupStack::Pop( newDecoratedLabel );
      
    iDecoratedLabel->MakeScrollButtonVisible( ETrue );

    if( AknLayoutUtils::LayoutMirrored() )
        {
        // Should be arrows.
        iDecoratedLabel->SetScrollButtonDimmed( CAknNavigationDecorator::ELeftButton, !next );
        // And these should also be arrows.
        iDecoratedLabel->SetScrollButtonDimmed( CAknNavigationDecorator::ERightButton, !previous );
        }
    else
        {
        // Should be arrows.
        iDecoratedLabel->SetScrollButtonDimmed( CAknNavigationDecorator::ELeftButton, !previous );
        // And these should also be arrows.
        iDecoratedLabel->SetScrollButtonDimmed( CAknNavigationDecorator::ERightButton, !next );
        }   
    
    } 

// ---------------------------------------------------------
// CCbsUiTopicView::MarkCurrentlyFocusedHandle
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::MarkCurrentlyFocusedHandle()
    {
    if ( iContainer->CurrentPosition() >= 0 &&          
        TotalMessages() > iContainer->CurrentPosition() )
        {
        iCurrentHandle = iHandles->At( iContainer->CurrentPosition() );
        }
    }


// ---------------------------------------------------------
// CCbsUiTopicView::HandleRectChangeInContainer
//
//
// ---------------------------------------------------------
//
void CCbsUiTopicView::HandleRectChangeInContainer()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }


// ---------------------------------------------------------
// CCbsUiTopicView::SetFocusToMessage
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::SetFocusToMessage( TInt aIndex )
    {
    if ( iContainer && TotalMessages() > 0 )
        {
        __ASSERT_DEBUG( aIndex >= 0 && aIndex < TotalMessages(), 
            CbsUiPanic(EErrIndexOutOfBounds) );
        // first one in the list is the newest
        iContainer->SetCurrentItemIndex(aIndex);
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::TopicListChangedL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::TopicListChangedL( TCbsTopicListEvent aEvent, TCbsTopicNumber aTopicNumber )
                                        
    {
    if ( iContainer && iCurrentTopicNumber == aTopicNumber )
        {
        if ( aEvent == ECbsTopicReceivedNewMessage ) 
            {
            ReloadMessagesL();
            }
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::SaveMessage
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicView::SaveMessage( const TCbsMessageHandle& aHandle )
    {
    return iServer.SaveMessage( aHandle );
    }

// ---------------------------------------------------------
// CCbsUiTopicView::LockMessage
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicView::LockMessage( TCbsMessageHandle aHandle )
    {
    return iServer.LockMessage(aHandle);
    }

// ---------------------------------------------------------
// CCbsUiTopicView::UnLockMessage
// 
// 
// ---------------------------------------------------------
// 
void CCbsUiTopicView::UnLockMessage()
    {
    if ( iServer.Connected() )
        {        
        iServer.LockMessage(NULL);
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::ShowMessageL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::ShowMessageL()
    {
    if ( TotalMessages() == 0 )
        {
        return;
        }
    
    User::LeaveIfError( LockMessage(iCurrentHandle) );
    
    ActivateMsgViewL(iCurrentHandle);
    }

// ---------------------------------------------------------
// CCbsUiTopicView::DynInitMenuPaneL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::DynInitMenuPaneL( TInt aResourceId,
                                       CEikMenuPane* aMenuPane )
    {
    __ASSERT_DEBUG( aMenuPane, CbsUiPanic(EMenuPaneNullError) );
    __ASSERT_DEBUG( aResourceId >= 0, CbsUiPanic( EErrInvalidValue) );
    if ( aResourceId == R_CBSUI_TOPICVIEW_MENU )
        {      
        if ( TotalMessages() > 0 )
            {
            // for using a right message
            MarkCurrentlyFocusedHandle();
            
            // Lock message that it is not deleted automatically.
            // If error, close the menu pane
            User::LeaveIfError( LockMessage(iCurrentHandle) );
            
            if( CbsUiUtility::IsSavedL(iCurrentHandle, iServer) )
                {
                aMenuPane->DeleteMenuItem(ECbsUiappCmdTopicMenuKeep);
                }
            }      
        else 
            {
            // If no messages, then not possible to do anything..
            aMenuPane->DeleteMenuItem(ECbsUiappCmdTopicMenuRead);
            aMenuPane->DeleteMenuItem(ECbsUiappCmdTopicMenuKeep);
            aMenuPane->DeleteMenuItem(ECbsUiappCmdMessageDelete);
            }
        if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            aMenuPane->DeleteMenuItem( EAknCmdHelp );
            }
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::SetEmphasis
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::SetEmphasis( CCoeControl* aMenuControl,TBool aEmphasis )
    {
    __ASSERT_DEBUG( aMenuControl, CbsUiPanic(EErrNullPointer) );
    CAknView::SetEmphasis(aMenuControl, aEmphasis);
    
    // Checks if the menu pane is showing.
    if ( !aEmphasis && iCanUnlock )
        {
        // Has to be unlocked, because otherwise message could stay as locked
        // and could not be deleted automatically by the server.
        UnLockMessage();
        }
    }


// ---------------------------------------------------------
// CCbsUiTopicView::ProcessCommandL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::ProcessCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case ECbsUiappCmdTopicMenuRead: // same operation
        case ECbsUiappCmdTopicMSKRead:
        case ECbsUiappCmdTopicMenuKeep: // cannot unlock current message
        case ECbsUiappCmdMessageDelete:
            iCanUnlock = EFalse; // prevents SetEmphasis to unlock message
            break;
        default:
            break;
        };
    
    CAknView::ProcessCommandL(aCommand);
    
    switch ( aCommand )
        {
        case ECbsUiappCmdTopicMenuKeep: // same operation
        case ECbsUiappCmdMessageDelete:
            // Have to unlock message, because in error situation
            // message would be locked after these two operations.
            UnLockMessage();
            break;
        default:
            break;
        };
    }

// ---------------------------------------------------------
// CCbsUiTopicView::HandleCommandL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::HandleCommandL( TInt aCommandId )
    {
    TTopicViewFocusState focusState = ENoFocusUpdate;
    
    switch (aCommandId)
        {
        case EAknSoftkeyBack:			    
            {
            ActivateTopicListViewL(iCurrentTopicNumber);
            break;
            }
        // MSK
        case ECbsUiappCmdTopicMSKRead:            
        case ECbsUiappCmdTopicMenuRead:
            {
            // to open a right message
            MarkCurrentlyFocusedHandle();
            if ( TotalMessages() > 0 )                     
                {
                ShowMessageL();
                }
            break;        
            }
        case ECbsUiappCmdTopicMenuKeep:
            {
            CbsUiUtility::SaveCurrentMessageL(iCurrentHandle, *this);
            focusState = ESetFocusToCurrentHandle;
            break;
            }
        case ECbsUiappCmdMessageDelete:
            {           
            if ( TotalMessages() > 0 )
                {                
                CAknQueryDialog* dlg = CAknQueryDialog::NewL();            
                
                if ( dlg->ExecuteLD(R_DELETE_MSG_CONFIRMATION_QUERY) )
                    {              
                    // Unlock the message, so that it could be deleted
                    UnLockMessage();                
                    
                    if ( iServer.DeleteMessage(iCurrentHandle) == KErrNone )
                        {
                        SetNewCurrentHandle(iCurrentHandle);
                        }                
                    }
                focusState = ESetFocusToCurrentHandle;
                }
            break;
            }
        case ECbsUiappTopicViewCmdGoToPrevTopic:
            {
            TBool toPrevious(ETrue);
            SetCurrentTopicNumber(toPrevious);
            focusState = ESetFocusToFirstItem;
            break;
            }               
        case ECbsUiappTopicViewCmdGoToNextTopic:
            {
            TBool toPrevious(EFalse);
            SetCurrentTopicNumber(toPrevious);
            focusState = ESetFocusToFirstItem;
            break;
            }
        case EAknCmdHelp:
            AppUi()->HandleCommandL(aCommandId);
            break;
        case EAknCmdExit:	
            {
            ( (MEikCommandObserver*)AppUi() )->
                ProcessCommandL(EAknCmdExit);
            break;
            }
        default:
            {
            break;
            }
        }
    
    if ( focusState != ENoFocusUpdate )
        {
        ReloadMessagesL();
        SetFocusAfterReloading(focusState);
        }
    
    }

// ---------------------------------------------------------
// CCbsUiTopicView::DoActivateL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::DoActivateL( const TVwsViewId& /*aPrevViewId*/, 
                                  TUid aCustomMessageId,
                                  const TDesC8& aCustomMessage )
    {
    iCanUnlock = ETrue;
    if ( !iContainer )
        {
        iContainer = CCbsUiTopicViewContainer::NewL(*this, ClientRect());
        AppUi()->AddToViewStackL( *this, iContainer );
        }
    
    SetCurrentTopicNumberAndMessageHandle(aCustomMessage);
    
    ReloadMessagesL();    
                
    TTopicViewFocusState focus(ESetFocusToFirstItem);
    if ( aCustomMessageId == KSetFocusToId )
        {
        focus = ESetFocusToCurrentHandle;
        }
    
    SetFocusAfterReloading(focus);
    
    // if the list box focus is changed, then container should be activetd
    // after ReloadMessagesL
    iContainer->ActivateL();
    }

// ---------------------------------------------------------
// CCbsUiTopicView::DoDeactivate
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::DoDeactivate()
    {	
    if (iContainer)
        {
        AppUi()->RemoveFromViewStack(*this, iContainer);
        delete iContainer;
        iContainer = NULL;	
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::Id
// 
// 
// ---------------------------------------------------------
//
TUid CCbsUiTopicView::Id() const
    {
    return TUid::Uid( ECbsUiTopicViewId );
    }

// ---------------------------------------------------------
// CCbsUiTopicView::FindMessageIndexByHandle
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicView::FindMessageIndexByHandle( TInt& aIndex,
                                               const TCbsMessageHandle& aMsgHandle )
    {
    __ASSERT_DEBUG( &aIndex, CbsUiPanic(EErrNullPointer) );
    
    TInt ret(KErrGeneral);
    
    for ( TInt i(0); i < TotalMessages() && ret != KErrNone; i++ )
        {
        if ( iHandles->At(i) == aMsgHandle )
            {
            ret = KErrNone;
            aIndex = i;
            }
        }
    
    return ret;
    }

// ---------------------------------------------------------
// CCbsUiTopicView::LoadAllMessageTitlesL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::LoadAllMessageTitlesL( TCbsTopicNumber aTopicNumber )
    {
    TBuf<KMaxMessageTitleLength> buffer;
    TCbsMessage message;
    iHandles->Reset();
    
    // Then take the total amount of messages in the topic.
    TInt count(0);
    User::LeaveIfError( iServer.GetMessageCount(aTopicNumber, count) );    
    
    for ( TInt index(0); index < count; index++ )
        {        
        if ( iServer.GetMessage(aTopicNumber, index, message) == KErrNone )
            {
            if ( iContainer )
                {
                // Add contents.
                TInt err = 
                    iServer.GetMessageContents(message.iHandle, buffer);
                
                __CBSUI_ASSERT( 
                    err == KErrNone, CbsUiPanic(EMessageContentsError) );
                User::LeaveIfError(err);
                
                iContainer->ReloadOfferL(message, buffer);
                }
            
            iHandles->AppendL(message.iHandle);
            }
        
        buffer.Zero();
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::PrepareReloading
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::PrepareReloading()
    {
    if ( iContainer )
        {
        iContainer->ReloadStarts();
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::SetFocusAfterReloading
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::SetFocusAfterReloading( TTopicViewFocusState aFocusState )
    {
    TInt focusedItem(KCbsErrValue);
    
    // has to find index from the ui side, because in the
    // server side the index could be an other.
    TBool setFocusToFirstItem(
        (aFocusState == ESetFocusToFirstItem) || 
        (aFocusState == ESetFocusToNewestMessage) ||
        (FindMessageIndexByHandle(focusedItem, iCurrentHandle) != KErrNone)
        );
    
    if ( setFocusToFirstItem )
        {
        focusedItem = EFirstMessage;
        }
    
    __ASSERT_DEBUG( 
        focusedItem != KCbsErrValue, CbsUiPanic(EErrInvalidValue) );
    
    SetFocusToMessage(focusedItem);
    }

// ---------------------------------------------------------
// CCbsUiTopicView::SetNewCurrentHandle
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::SetNewCurrentHandle( TCbsMessageHandle aDeletedHandle )
    {
    
    TInt messagePosition(KCbsErrValue);
    TInt err( FindMessageIndexByHandle(messagePosition, aDeletedHandle) );
    
    TBool isLastMessage = 
        ( (messagePosition + 1) == TotalMessages() ) &&
        (messagePosition >= 0);
    
    if ( isLastMessage )
        {
        // then previous handle should be focused after deletion
        messagePosition--;
        }
    else
        {
        // then the next handle will be focused after deletion
        messagePosition++;
        }
    
    if ( messagePosition >= TotalMessages() ||
        messagePosition < 0 || 
        err != KErrNone )
        {
        messagePosition = 0;
        }
    
    __ASSERT_DEBUG( err == KErrNone, CbsUiPanic(EErrMsgNotFound) );
    
    iCurrentHandle = iHandles->At(messagePosition);
    }

// ---------------------------------------------------------
// CCbsUiTopicView::SetCurrentTopicNumber
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::SetCurrentTopicNumber( TBool aToPrev )
    {
    TCbsTopicNumber prev;
    TCbsTopicNumber next;
    TInt position;
    
    TInt err( iServer.GetNextAndPrevTopicNumber(
        iCurrentTopicNumber, prev, next, position) );
    
    if ( err == KErrNone ) // handle exists
        {        
        if ( aToPrev )
            {
            if ( position != ECbsHead )
                {
                iCurrentTopicNumber = prev;
                }
            }
        else 
            {
            if ( position != ECbsTail )
                {
                iCurrentTopicNumber = next;
                }
            }
        }
    else 
        {
        __CBSUI_ASSERT( 0, CbsUiPanic(EErrTopicNumberNotFound) );
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::SetCurrentTopicNumberAndMessageHandle
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::SetCurrentTopicNumberAndMessageHandle( const TDesC8& aTopicNumberDes )
    {
    TTopicNumberBuf aTopicNumberBuf;
    aTopicNumberBuf.Copy(aTopicNumberDes);
    iCurrentTopicNumber = aTopicNumberBuf().iTopicNumber;
    iCurrentHandle = aTopicNumberBuf().iMessageHandle;
    }

// ---------------------------------------------------------
// CCbsUiTopicView::ActivateMsgViewL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::ActivateMsgViewL( TCbsMessageHandle aMsgHandle ) const
    {
    TPckgBuf<TCbsMessageHandle> pckgMessageHandle(aMsgHandle);
    
    AppUi()->ActivateLocalViewL( 
        TUid::Uid(ECbsUiMsgViewId), KCbsUiCustomMessageId, pckgMessageHandle);
    }

// ---------------------------------------------------------
// CCbsUiTopicView::ActivateTopicListViewL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::ActivateTopicListViewL( TCbsTopicNumber aTopicNumber ) const
    {
    TPckgBuf<TCbsTopicNumber> pckgTopicNumber(aTopicNumber);
    
    AppUi()->ActivateLocalViewL( 
        TUid::Uid(ECbsUiTopicListViewId), KSetFocusToId, pckgTopicNumber);
    }

// ---------------------------------------------------------
// CCbsUiTopicView::CheckIfCanMove
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicView::CheckIfCanMove( TBool& aToPrevious, TBool& aToNext )
    {
    TCbsTopicNumber prevDummy; // not used
    TCbsTopicNumber nextDummy;
    TInt position;
    
    TInt err( iServer.GetNextAndPrevTopicNumber(
        iCurrentTopicNumber, prevDummy, nextDummy, position) );
    
    __CBSUI_ASSERT( err == KErrNone, CbsUiPanic(EErrTopicNumberNotFound) );
    __CBSUI_ASSERT( 
        position >= 0 && position <= (ECbsTail | ECbsHead), 
        CbsUiPanic(EErrInvalidValue) );
    
    aToPrevious = EFalse;
    aToNext = EFalse;
    
    if ( err == KErrNone )
        {
        if ( !position ) // topic in both sides
            {
            aToPrevious = ETrue;
            aToNext = ETrue;
            }
        else if ( position == ECbsTail )
            {
            aToPrevious = ETrue;
            }
        else if ( position == ECbsHead )
            {
            aToNext = ETrue;
            }
        else
            {
            //Does nothing
            }
        }
    
    return err;
    }

// ---------------------------------------------------------
// CCbsUiTopicView::ActivateListBoxL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::ActivateListBoxL()
    {
    iContainer->ReloadEndsL();
    }

// ---------------------------------------------------------
// CCbsUiTopicView::SetMiddleSoftKeyLabelL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::SetMiddleSoftKeyLabelL( 
    const TInt aResourceId, const TInt aCommandId )
    {
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        HBufC* mskText = StringLoader::LoadLC( aResourceId );
        TPtr mskPtr = mskText->Des();
        cbaGroup->AddCommandToStackL( 
            KCbsMSKControlID, 
            aCommandId, 
            mskPtr );
        CleanupStack::PopAndDestroy( mskText );
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::RemoveCommandFromMSK
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::RemoveCommandFromMSK()
    {
    if ( Cba() )
        {
        Cba()->RemoveCommandFromStack( KCbsMSKControlID, ECbsUiappCmdTopicMSKRead );
        Cba()->RemoveCommandFromStack( KCbsMSKControlID, ECbsUiappNullCmd );
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicView::HandleNaviDecoratorEventL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicView::HandleNaviDecoratorEventL( TInt aEventID )
	{
	TTopicViewFocusState focusState = ENoFocusUpdate;
	
    switch ( aEventID )
    	{
		case MAknNaviDecoratorObserver::EAknNaviDecoratorEventLeftTabArrow:
		    {
		    SetCurrentTopicNumber( ETrue );
		    focusState = ESetFocusToFirstItem;
		    break;
		    }               
		case MAknNaviDecoratorObserver::EAknNaviDecoratorEventRightTabArrow:
		    {
		    SetCurrentTopicNumber( EFalse );
		    focusState = ESetFocusToFirstItem;
		    break;
		    }
		default:
		    {
		    break;
		    }
    	}
    
	if ( focusState != ENoFocusUpdate )
		{
		ReloadMessagesL();
		SetFocusAfterReloading( focusState );
		}  
	}
    
//  End of File  
