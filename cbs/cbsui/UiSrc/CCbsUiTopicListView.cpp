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
*  Implementation of CCbsUiTopicLIstView class.
*
*
*/


// INCLUDES
#include <AknUtils.h>           // AknTextUtils
#include <bldvariant.hrh>       // for feature flags
#include <featmgr.h>            // for feature manager
#include <e32def.h>
#include <akntitle.h>
#include <aknViewAppUi.h>
#include <aknnavide.h>
#include <akntitle.h>
#include <avkon.hrh>
#include <barsread.h>
#include <StringLoader.h>
#include <eikmenup.h>
#include <aknnotewrappers.h>
#include <hlplch.h>
#include <cbsuiApp.rsg>
#include "MCbs.h"
#include "CCbsUiTopicListView.h"
#include "CCbsUiTopicListViewContainer.h"
#include "CCbsUiEditTopicDlg.h"
#include "Cbsuiapp.hrh"
#include "CbsUiPanic.h"
#include "CbsUiConstants.h"
#include "CCbsUiAddFromIndexDialog.h"
#include "CbsUiDebug.h"
#include <AknNotifyStd.h>       // ECellBroadcastNotification
#include <AknSoftNotifier.h>    // Soft Notification API


// CONSTANTS
const TInt KTopicNameLenght = 20;
const TInt KCountOfAllTopics = 1000;
const TInt KIndexTopic = 0;
const TInt KTwoTopics = 2;

// MODULE DATA STRUCTURES
enum TMessageCounts { EManyNewMessages = -1, ENoNewMessages = 0, EOneNewMessage = 1 };


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiTopicListView::CCbsUiTopicListView( MCbs& aServer ) : iServer( aServer ), iFocusState( ENoFocusUpdate )
    {
    }

// Symbian OS default constructor can leave.
void CCbsUiTopicListView::ConstructL()
    {	
    BaseConstructL(R_CBSUI_TOPICLISTVIEW);
    iTopicNumberList = new (ELeave) TCbsUiTopicNumberList(KNumberOfStandardTopics);
    CEikStatusPane* statusPane = StatusPane();
    iNaviPane = STATIC_CAST( CAknNavigationControlContainer*, statusPane->
        ControlL( TUid::Uid(EEikStatusPaneUidNavi) ) );
    __ASSERT_DEBUG( iNaviPane, CbsUiPanic(ENaviPaneError) );    
    }

// Two-phased constructor.
CCbsUiTopicListView* CCbsUiTopicListView::NewL( MCbs& aServer )
    {
    CCbsUiTopicListView* self = new (ELeave) CCbsUiTopicListView(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsUiTopicListView::~CCbsUiTopicListView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack(*this, iContainer);
        }
    delete iContainer;
    delete iDecoratedLabel;
    delete iTopicNumberList;
    delete iBackgroundNote;
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::ReloadTopicsL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::ReloadTopicsL()
    {
    __ASSERT_DEBUG( iTopicNumberList, CbsUiPanic(ETopicHandleError) );
    PrepareReloading();
    ReloadL();
    EndReloadingL();
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::PrepareReloading
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::PrepareReloading()
    {
    if ( iContainer )
        {
        iContainer->ReloadStarts();
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::ReloadL
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::ReloadL()
    {
    iTopicNumberList->Reset();
    
    TInt count(0);
    iServer.GetTopicCount(count);
    __ASSERT_DEBUG( count >= 0, CbsUiPanic(ETopicCountError) );
    
    TCbsTopic topic;
    
    for ( TInt index(0); index < count; index++ )
        {
        // First, get the topic.
        TInt rr( iServer.GetTopic(index, topic) );
        __CBSUI_ASSERT( rr == KErrNone, CbsUiPanic(EErrTopicNotFound) );
        
        if ( rr == KErrNone )
            {
            // Offer the topic to the view.
            if ( iContainer )
                {
                iContainer->ReloadOfferTopicL(topic);
                }
            
            // And append to the handles list.
            iTopicNumberList->AppendL(topic.iNumber);
            }
        }
    }

void CCbsUiTopicListView::EndReloadingL()
    {
    iContainer->ReloadEndsL();
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::SetFocusAfterReload
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::SetFocusAfterReload( TTopicListViewFocusState aFocusState )
    {
    if ( aFocusState == ENoFocusUpdate )
        {
        return;
        }
    
    // if the focus need to be changed
    TInt focusedItem(KCbsErrValue);
    
    TBool setFocusToFirstItem(
        (aFocusState == ESetFocusToFirstItem) ||
        (FindTopicIndexByNumber(focusedItem, iCurrentTopicNumber) != KErrNone)
        );
    
    if ( setFocusToFirstItem )
        {
        // If there is some error let's try to put focus to first item
        focusedItem = 0;
        }
    
    __ASSERT_DEBUG( focusedItem != KCbsErrValue, CbsUiPanic(EErrInvalidValue) );    
    
    SetFocusToTopic(focusedItem);
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::SetFocusToTopic
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::SetFocusToTopic( TInt aIndex )
    {
    if ( iTopicNumberList->Count() <= 0)
        {
        return;
        }
    
    __ASSERT_DEBUG( aIndex >= 0 && aIndex < iTopicNumberList->Count(), 
        CbsUiPanic(EErrIndexOutOfBounds) );
    
    if (iContainer)
        {
        // first one in the list is the newest
        iContainer->SetCurrentItemIndex(aIndex);
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::UpdateCurrentTopicAfterDeletion
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::UpdateCurrentTopicAfterDeletion( TCbsTopicNumber aDeletedTopic )
    {
    
    TInt topicPosition(-1);
    TInt err( FindTopicIndexByNumber(topicPosition, aDeletedTopic) );
    
    TBool isLastTopic(  
        ( (topicPosition + 1) == TotalTopics() ) &&
        (topicPosition >= 0)
        );
    
    if ( isLastTopic )
        {
        // then previous topic should be focused after deletion
        topicPosition--;
        }
    else
        {
        // then the next topic will be focused after deletion
        topicPosition++;
        }
    
    // if there is some error lets put the focus to the first item
    if ( topicPosition >= TotalTopics() ||
        topicPosition < 0 || 
        err != KErrNone )
        {
        topicPosition = 0;
        }
    
    __ASSERT_DEBUG( err == KErrNone, CbsUiPanic(EErrMsgNotFound) );
    
    iCurrentTopicNumber = iTopicNumberList->At(topicPosition);
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::TotalTopics
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListView::TotalTopics() const
    {
    __ASSERT_DEBUG( iTopicNumberList->Count() >= 0, CbsUiPanic(EErrTopicsCountError) );
    return iTopicNumberList->Count();
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::UpdateTitlePaneL
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::UpdateTitlePaneL() const
    {
    CEikStatusPane *sp = StatusPane();
    CAknTitlePane *title = STATIC_CAST(CAknTitlePane*, sp->
        ControlL( TUid::Uid(EEikStatusPaneUidTitle) ) );
    
    __ASSERT_DEBUG( sp && title, CbsUiPanic(EStatusOrTitlePaneError) );
    
    TResourceReader titleReader;
    iCoeEnv->CreateResourceReaderLC(titleReader, R_QTN_CB_TITLE_CBS);
    title->SetFromResourceL(titleReader);
    CleanupStack::PopAndDestroy(); // titleReader
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::UpdateNaviPaneL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::UpdateNaviPaneL()
    {	
    
    if ( iAddFromIndexDialogIsActive )
        {
        // No need to update navi pane
        return;
        }
    
    // let's keep a address of the previous 
    // label so it is possible to delete 
    // it, after the new label has been created.
    CAknNavigationDecorator* oldLabel = iDecoratedLabel;    
    
    TInt unreadMessages(0);
    iServer.GetUnreadMessageCount(unreadMessages);
    
    CreateNewLabelL( GetInfoResourceId(unreadMessages), unreadMessages );
    
    // Old label has to be deleted and 
    // the final label is destroyed in the destructor.
    if ( oldLabel )
        {
        delete oldLabel;
        }	
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::GetInfoResourceId
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListView::GetInfoResourceId( TInt aUnreadMessages ) const
    {
    __ASSERT_DEBUG( aUnreadMessages >= 0, CbsUiPanic(EErrInvalidValue) );
    
    TInt result(EManyNewMessages);
    
    switch ( aUnreadMessages )
        {
        case ENoNewMessages:
            result = R_CB_TL_NO_NEW_MESSAGES;
            break;
        case EOneNewMessage:
            result = R_CB_TL_ONE_NEW_MESSAGE;
            break;
        default:
            break;
        }  
    
    return result;
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::CreateNewLabelL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::CreateNewLabelL( TInt aResourceId, TInt aUnreadMessages )
    {
    __ASSERT_DEBUG( 
        aResourceId == EManyNewMessages || 
        aResourceId == R_CB_TL_NO_NEW_MESSAGES ||
        aResourceId == R_CB_TL_ONE_NEW_MESSAGE, 
        CbsUiPanic(EErrInvalidValue) );
    
    HBufC* stringHolder = NULL;
    
    if ( aResourceId == EManyNewMessages )
        {
        stringHolder = StringLoader::LoadLC( 
            R_CB_TL_NEW_MESSAGES, aUnreadMessages);
        }
    else 
        {
        stringHolder = iCoeEnv->AllocReadResourceLC(aResourceId);
        }
    
    __ASSERT_DEBUG( stringHolder, CbsUiPanic(EErrNullPointer) );
    if ( stringHolder )
        {
        iDecoratedLabel = iNaviPane->CreateNavigationLabelL(*stringHolder);
        CleanupStack::PopAndDestroy(); // stringholder
        }
    
    __ASSERT_DEBUG( iDecoratedLabel, CbsUiPanic(EErrNullPointer) );
    iNaviPane->PushL(*iDecoratedLabel);
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::ShowTopicQueryL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::ShowTopicQueryL( TBool aType, 
                                          TDes& aTopicName, 
                                          TDes& aTopicNumber )
    {
    CCbsUiEditTopicDlg* dlg = CCbsUiEditTopicDlg::NewL(
        aTopicName, aTopicNumber, *this, aType );
    dlg->ExecuteLD( R_EDIT_TOPIC_QUERY );
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::ShowEditTopicQueryL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::ShowEditTopicQueryL()
    {
    TCbsTopic topic;
    TInt err( iServer.FindTopicByNumber( iCurrentTopicNumber, topic ) );
    
    if ( err != KErrNone )
        {
        User::Leave( KErrNone );
        }
    
    TBuf<KTopicNumberWidth> number;
    number.AppendNum( TInt( topic.iNumber ) );
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( number );  
    ShowTopicQueryL( EFalse, topic.iName, number );
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::EditTopic
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListView::EditTopic( TBool aType, 
                                    TCbsTopicNumber aNumber, 
                                    const TCbsTopicName aName )
    {
    TInt ret( KErrGeneral );
    
    if ( aType ) // add query
        {
        ret = AddTopic(aNumber, aName);
        }
    else // edit query
        {
        ret = UpdateTopic( iCurrentTopicNumber, aNumber, aName );
        if( ret == KErrNone )
            {
            iCurrentTopicNumber = aNumber;
            }
        }
    
    return ret;
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::CheckNewTopicCountL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::CheckNewTopicCountL()
    {
    iNewTopicsCount = GetNewTopicsCount();
    
    __CBSUI_ASSERT( iNewTopicsCount >= 0, CbsUiPanic(EErrInvalidValue) );
    
    if ( iNewTopicsCount > ENoNewMessages )
        {
        if ( !iBackgroundNote )
            {
            iBackgroundNote = CIdle::NewL(CActive::EPriorityIdle);
            }
        
        TCallBack cb(ActivateBackgroundNoteL, this);
        iBackgroundNote->Start(cb);
        }
    
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::GetNewTopicsCount
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListView::GetNewTopicsCount()
    {
    TInt newTopics(KCbsErrValue);
    if ( iServer.Connected() )
        {
#ifdef _DEBUG
        TInt ret = 
#endif
            iServer.GetNewTopicsCount(newTopics);
        
        __CBSUI_ASSERT( ret == KErrNone, CbsUiPanic(EErrTopicsCountError) );
        }
    
    return newTopics;
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::MarkCurrentHandle
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::MarkCurrentHandle()
    {
    TInt index( iContainer->CurrentPosition() );
    
    if ( index >= 0 && iTopicNumberList->Count() > index )
        {
        iCurrentTopicNumber = iTopicNumberList->At( index );
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::TopicListChangedL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::TopicListChangedL( TCbsTopicListEvent /*aEvent*/, TCbsTopicNumber /*aTopicNumber*/ )
    {
    if ( iContainer )
        {
        ReloadTopicsL();
        UpdateNaviPaneL();
        SetFocusAfterReload(iFocusState);
        iFocusState = ENoFocusUpdate;
        }
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::HandleCommandL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::HandleCommandL( TInt aCommandId )
    {
    
    switch ( aCommandId )
        {
        case ECbsUiappCmdSwitchOpen:			
            {
            // To open a right topic
            MarkCurrentHandle();
            ActivateTopicViewL();
            break;
            }
        case ECbsUiappCmdSwitchSubscribe:
            {
            // Change subscription status
            User::LeaveIfError(
                iServer.ChangeTopicSubscriptionStatus( 
                iCurrentTopicNumber, ETrue ) );
            
            iFocusState = ESetFocusToCurrentHandle;
            break;
            }     
        case ECbsUiappCmdSwitchUnsubscribe:
            {
            // Change subscription status
            User::LeaveIfError( 
                iServer.ChangeTopicSubscriptionStatus( 
                iCurrentTopicNumber, EFalse ));
            iFocusState = ESetFocusToCurrentHandle;
            break;
            }            
        case ECbsUiappCmdSwitchHotmark: // Implementation is almost the same with
            // ECbsUiappCmdSwitchHotmark and 
            // ECbsUiappCmdSwitchUnhotmark.
        case ECbsUiappCmdSwitchUnhotmark:
            {
            // Requires refresh
            TBool hot(EFalse);
            
            if ( aCommandId == ECbsUiappCmdSwitchHotmark )
                {
                hot = ETrue;
                TInt focusedItemIndex;
                if(FindTopicIndexByNumber(focusedItemIndex, iCurrentTopicNumber) == KErrNone)
                    {
                    TCbsTopic topic;
                    // First, get the topic.
                    TInt rr( iServer.GetTopic(focusedItemIndex, topic) );
                    if ( rr == KErrNone )
                        {
                        if ( !topic.iSubscribed )
                            {
                            // Change subscription status to subscribed
                            User::LeaveIfError(iServer.ChangeTopicSubscriptionStatus( 
                                iCurrentTopicNumber, ETrue )) ;
                            }
                        }
                    }
                }
            
            User::LeaveIfError(iServer.ChangeTopicHotmarkStatus(iCurrentTopicNumber, hot));
            iFocusState = ESetFocusToCurrentHandle;
            break;
            }            
        case ECbsUiappCmdSwitchSettings:
            {                            
            ActivateSettingsViewL();
            break;			
            }
            // Topic sub menu pop up window commands.
        case ECbsUiappCmdSwitchAddManually:
            {
            TBuf<KTopicNameLenght> name;	
            TBuf<KTopicNumberWidth> number;
            
            ShowTopicQueryL( ETrue, name, number );
            AknTextUtils::ConvertDigitsTo( number, EDigitTypeWestern );             
            
            TInt topicNumber(KCbsErrValue);
            if ( number.Length() > 0 )
                {
                TLex lex(number);                
                TInt err( lex.Val(topicNumber) );
                
                if ( err == KErrNone )
                    {
                    SetCurrentTopicNumber( (TCbsTopicNumber) topicNumber );
                    }
                __CBSUI_ASSERT( err == KErrNone, CbsUiPanic(EErrNotANumber) );
                }
            
            iFocusState = ESetFocusToCurrentHandle;
            break;
            }
        case ECbsUiappCmdSwitchAddFromIndex:
            {		
#ifdef _DEBUG
            GetNewTopicsCount(); // just to reset
#endif
            // topic detection need to shut down when 
            // add from index dialog is shown.
            TBool topicDetection;
            iServer.GetTopicDetectionStatus(topicDetection);
            
            if ( topicDetection )
                {
                User::LeaveIfError( iServer.SetTopicDetectionStatus(EFalse) );
                }
            
            iAddFromIndexDialogIsActive = ETrue;
            ActivateAddFromIndexViewL();
            
            __ASSERT_DEBUG(GetNewTopicsCount() == 0, CbsUiPanic(ETopicCountError) );
            
            iAddFromIndexDialogIsActive = EFalse;
            UpdateTitlePaneL();
            UpdateNaviPaneL();
            
            if ( topicDetection )
                {
                User::LeaveIfError( iServer.SetTopicDetectionStatus(topicDetection) );
                }
            
            iFocusState = ESetFocusToCurrentHandle;
            break;
            }
        case ECbsUiappCmdSwitchEdit:
            {
            ShowEditTopicQueryL();
            iFocusState = ESetFocusToCurrentHandle;
            break;
            }
        case ECbsUiappCmdSwitchDelete:
            {            
            if ( iCurrentTopicNumber != KIndexTopic )
                {                
                CAknQueryDialog* dlg = CAknQueryDialog::NewL();
                
                if ( dlg->ExecuteLD(R_DELETE_TOPIC_CONFIRMATION_QUERY) )
                    {
                    TInt unreadHotmarkedMessages;		         
                    User::LeaveIfError(
                        iServer.DeleteTopic(iCurrentTopicNumber) );
                    UpdateCurrentTopicAfterDeletion(iCurrentTopicNumber);
                    unreadHotmarkedMessages = iServer.NumberOfUnreadHotmarkedMessages();
                    if( unreadHotmarkedMessages <= 0 )
                        {
                        CAknSoftNotifier* notifier = CAknSoftNotifier::NewLC();
                        notifier->CancelSoftNotificationL( ECellBroadcastNotification);
                        CleanupStack::PopAndDestroy( notifier );
                        }
                    }
                iFocusState = ESetFocusToCurrentHandle;
                }
            break;
            }
        case ECbsUiappCmdSwitchDeleteAll:
            {
            TInt queryOk;
            CAknQueryDialog* dlg = CAknQueryDialog::NewL();
            queryOk= dlg->ExecuteLD(R_DELETE_ALL_TOPICS_CONFIRMATION_QUERY );
            
            if( queryOk )
                {             
                User::LeaveIfError( iServer.DeleteAllTopics() );
                CAknSoftNotifier* notifier = CAknSoftNotifier::NewLC(); // on CS
                notifier->CancelSoftNotificationL( ECellBroadcastNotification);
                CleanupStack::PopAndDestroy( notifier );
                }
            iFocusState = ESetFocusToCurrentHandle;
            break;            
            }
        case EAknCmdHelp:
            AppUi()->HandleCommandL(aCommandId);
            break;
        case EAknSoftkeyExit: // same functionality
        case EAknCmdExit:	
            {
            ( (MEikCommandObserver*)AppUi() )->ProcessCommandL(EAknCmdExit);
            break;
            }
        default:
            break;
        }
    }



// ---------------------------------------------------------
// CCbsUiTopicListView::HandleRectChangeInContainer
//
//
// ---------------------------------------------------------
//
void CCbsUiTopicListView::HandleRectChangeInContainer()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }




// ---------------------------------------------------------
// CCbsUiTopicListView::DoActivateL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                       TUid aCustomMessageId,
                                       const TDesC8& aCustomMessage )
    {
    if ( !iContainer )
        {
        iContainer = CCbsUiTopicListViewContainer::NewL( *this, ClientRect() );
        AppUi()->AddToViewStackL( *this, iContainer );
        }        

    TTopicListViewFocusState focus = ESetFocusToFirstItem;

    if ( aCustomMessageId == KSetFocusToId )
        {
        focus = ESetFocusToCurrentHandle;
        SetCurrentTopicNumber(aCustomMessage);
        }
    else if ( aCustomMessageId == KSetFocusToCurrentId )
        {
        focus = ESetFocusToCurrentHandle;
        }
    else 
        {
        focus = ESetFocusToFirstItem;
        }
    
    ReloadTopicsL();
    SetFocusAfterReload(focus);
    UpdateTitlePaneL();
    UpdateNaviPaneL();
    iContainer->ActivateL();
    }


// ---------------------------------------------------------
// CCbsUiTopicListView::AddTopic
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListView::AddTopic( const TCbsTopicNumber& aNumber, const TCbsTopicName& aName )
    {
    __ASSERT_DEBUG( &aName, CbsUiPanic(EErrNullPointer) );

    TCbsTopic topic;
    topic.iHotmarked = EFalse;
    topic.iName = aName;
    topic.iNumber = aNumber;
    topic.iProtected = EFalse;
    topic.iSavedMessages = 0;
    topic.iUnreadMessages = 0;
    topic.iSubscribed = 0;
    return iServer.AddTopic( topic );
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::UpdateTopic
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListView::UpdateTopic( const TCbsTopicNumber& aOldNumber, 
                                       const TCbsTopicNumber& aNumber,
                                       const TCbsTopicName& aName )
    {
    __ASSERT_DEBUG( &aName, CbsUiPanic(EErrNullPointer) );
    return iServer.ChangeTopicNameAndNumber( aOldNumber, aNumber, aName );
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::DoDeactivate
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::DoDeactivate()
    {
    if ( iContainer )
        {
	AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    delete iContainer;
    iContainer = NULL;
    }

void CCbsUiTopicListView::ViewActivatedL(
            const TVwsViewId& aPrevViewId,
            TUid aCustomMessageId,const 
            TDesC8& aCustomMessage)
    {
    CheckNewTopicCountL();
    CAknView::ViewActivatedL(aPrevViewId, aCustomMessageId, aCustomMessage);
    }

void CCbsUiTopicListView::ViewDeactivated()
    {
    // just to update
    GetNewTopicsCount();
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::DynInitMenuPaneL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::DynInitMenuPaneL( TInt aResourceId,
                                            CEikMenuPane* aMenuPane )
    {  
    __ASSERT_DEBUG( aMenuPane, CbsUiPanic( EMenuPaneNullError ) );

    TCbsTopic topic;
    TInt ret( iServer.GetTopic(iContainer->CurrentPosition() , topic) );

    // If topic doesn't exists it is better to leave and close the 
    // menu.
    User::LeaveIfError(ret); 

    if ( aResourceId == R_CBSUI_TOPICLISTVIEW_MENU )
        {        
        MarkCurrentHandle(); // to freeze a selected item
        
        // If topic is subscribed,..
        if ( topic.iSubscribed )
            {
            // then remove subscribed item.
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchSubscribe );
            }
        else
            {
            // otherwise remove unsubscribed item.
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchUnsubscribe );
            }
        
        // And the same for hotmark/unhotmark.
        if ( topic.iHotmarked )
            {
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchHotmark );
            }
        else
            {
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchUnhotmark );
            }
        
        if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            aMenuPane->DeleteMenuItem( EAknCmdHelp );
            }
        }
    else if ( aResourceId == R_CBSUI_TOPIC_MENU )
        {
        TInt topics;
        topics = iTopicNumberList->Count();

        // If there is no room for topics, then remove add manually.
        if ( topics >= KCountOfAllTopics )
            {
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchAddManually );
            }

        // If only index topic in topic list then "Delete all" menuitem is not shown
        if ( topics < KTwoTopics )
            {
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchDeleteAll );
            }

        // Protected topic can not be edited or deleted.
        if ( topic.iProtected )
            {
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchEdit );
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchDelete );
            }

        // Check if there are Index message received.
        iServer.StartCollectionBrowsing();

        if( !iServer.HasNextCollectionTopic() )
            {
            aMenuPane->DeleteMenuItem( ECbsUiappCmdSwitchAddFromIndex );
            }
        }
    else
	{
	//Does nothing
	}
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::SetCurrentTopicNumber
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::SetCurrentTopicNumber( const TDesC8& aTopicNumber )
    {
    TPckgBuf<TCbsTopicNumber> pckgTopicNumber;
    pckgTopicNumber.Copy(aTopicNumber);
    iCurrentTopicNumber = pckgTopicNumber();
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::SetCurrentTopicNumber
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::SetCurrentTopicNumber( const TCbsTopicNumber aTopicNumber)
    {
    iCurrentTopicNumber = aTopicNumber;
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::Id
// 
// 
// ---------------------------------------------------------
//
TUid CCbsUiTopicListView::Id() const
    {
    return TUid::Uid( ECbsUiTopicListViewId );	
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::FindTopicIndexByNumber
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListView::FindTopicIndexByNumber( TInt& aIndex,
                                                  const TCbsTopicNumber& aTopicNumber )
    {
    TInt ret(KErrGeneral);

    for ( TInt i(0); i < iTopicNumberList->Count() && ret != KErrNone; i++ )
        {
        if ( iTopicNumberList->At(i) == aTopicNumber )
            {
            ret = KErrNone;
            aIndex = i;
            }
        }
    return ret;
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::ActivateTopicViewL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::ActivateTopicViewL() const
    {   
    TPckgBuf<TCbsTopicNumber> message( iCurrentTopicNumber );
    
    AppUi()->ActivateLocalViewL( 
        TUid::Uid(ECbsUiTopicViewId), KCbsUiCustomMessageId, message );
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::ActivateSettingsViewL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::ActivateSettingsViewL() const
    {  
    AppUi()->ActivateLocalViewL( TUid::Uid(ECbsUiSettingsViewId) );
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::ActivateAddFromIndexViewL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiTopicListView::ActivateAddFromIndexViewL()
    {
    CCbsUiAddFromIndexDialog* dialog = 
        new ( ELeave ) CCbsUiAddFromIndexDialog( iServer );
    
    dialog->ExecuteLD(R_CBS_ADD_FROM_INDEX_DIALOG);
    }

// ---------------------------------------------------------
// CCbsUiTopicListView::ActivateBackgroundNote
// 
// 
// ---------------------------------------------------------
//
TInt CCbsUiTopicListView::ActivateBackgroundNoteL( TAny* aTopicList )
    {
    __ASSERT_DEBUG( aTopicList, CbsUiPanic(EErrNullPointer) );
    TInt newTopics( STATIC_CAST(CCbsUiTopicListView*, aTopicList)->iNewTopicsCount );
        
    if ( newTopics > ENoNewMessages )
        {
        HBufC* stringHolder = NULL;
        if ( newTopics > EOneNewMessage )
            {
            stringHolder = StringLoader::LoadLC(
                R_PLURAL_TOPIC_ADDED, newTopics );
            }
        else 
            {
            stringHolder = STATIC_CAST(CCbsUiTopicListView*, aTopicList)->
                iCoeEnv->AllocReadResourceLC(
                R_TEXT_NOTE_NEW_TOPIC_ADDED );
            }
        
        CAknConfirmationNote* note = new (ELeave) CAknConfirmationNote;
        note->ExecuteLD(*stringHolder);
        
        CleanupStack::PopAndDestroy(); // stringholder
        }

    return KErrNone;
    }


//  End of File  
