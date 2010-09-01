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
* Description:  Dialog which shows topics, which are not yet in 
*               the topic list view. Enables topics adding manually.
*
*
*/


// INCLUDES
#include <barsread.h>
#include <gulicon.h>
#include <eikclbd.h>
#include <eikspane.h>
#include <aknnavide.h>
#include <akntitle.h>
#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <cbsuiApp.rsg>
#include <aknlists.h>
#include <AknIconArray.h>
#include "CCbsUiAddFromIndexDialog.h"
#include "Cbsuiapp.hrh"
#include "CbsUiPanic.h"
#include "MCbs.h"

// CONSTANTS        
const TInt KAssumedAverageOfItems = 5;
const TInt KNumberOfIconsInAddFromIndexDialog = 4;
_LIT(KCbsIconListSeparator, "1\t");
_LIT(KCbsColumnListSeparator, " ");

// if the aTopicInfo.iNumber is shorter than KWidth,
// this character will be used to fill empty spaces.
const TUint KZeroFill = '0'; 




// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiAddFromIndexDialog::CCbsUiAddFromIndexDialog( MCbs& aServer ) : 
iServer(aServer)
    {
    }

// Destructor  
CCbsUiAddFromIndexDialog::~CCbsUiAddFromIndexDialog()
    {
    delete iHandles;  
    delete iNaviDecorator;
    }

//-----------------------------------------------------------------------------
//
//    CCbsUiAddFromIndexDialog::PreLayoutDynInitL()
//
//    Sets the choices to controls in AddFromIndexdialog
//
//-----------------------------------------------------------------------------
void CCbsUiAddFromIndexDialog::PreLayoutDynInitL()
    {
    CEikDialog::PreLayoutDynInitL();
    
    CAknNavigationControlContainer* naviPane = 
        STATIC_CAST( CAknNavigationControlContainer*, 
        iEikonEnv->AppUiFactory()->StatusPane()->ControlL(
        TUid::Uid( EEikStatusPaneUidNavi) ) );
    
    __ASSERT_DEBUG( naviPane, CbsUiPanic( ENaviPaneError ) );
    
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC( reader, R_QTN_CB_AD_SEL_TOP );
    
    delete iNaviDecorator;
    iNaviDecorator = NULL;
    
    iNaviDecorator = naviPane->
        ConstructNavigationDecoratorFromResourceL( reader );
    
    __ASSERT_DEBUG( iNaviDecorator, CbsUiPanic( ENaviDecoratorError ) );
    
    naviPane->PushL( *iNaviDecorator );
    CleanupStack::PopAndDestroy();      
    
    iListBox = STATIC_CAST(
        CAknSingleGraphicStyleListBox*, Control(ECbsUiAddFromIndexDlgLine) );
    
    __ASSERT_DEBUG( iListBox, CbsUiPanic( EListBoxInDialogError ) );
    
    SetIconsL();
    SetTopicsL();
    
    // Scrollbar
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
    
    // order has to be this; otherwise on error add from index title pane
    // remains.
    CreateAddFromIndexTitlePaneL();
    
    MEikAppUiFactory* appUiFactory = iEikonEnv->AppUiFactory();
    TRect rect = appUiFactory->ClientRect();
    iListBox->SetRect( rect );    
    }

//-----------------------------------------------------------------------------
//
//    CCbsUiAddFromIndexDialog::OkToExitL()
//
//    Handles keyevents. In case "Softkey OK" it
//    add selected topics using member function addtopic
//
//-----------------------------------------------------------------------------
TBool CCbsUiAddFromIndexDialog::OkToExitL( TInt aKeycode )
    {            
    TBool result( EFalse );
    switch ( aKeycode )
        {
        case EAknSoftkeyOk:
            {
            const CListBoxView::CSelectionIndexArray* selArray = 
            iListBox->SelectionIndexes();
        
            if ( selArray )
                {
                // Initialize the marked contact id array using selArray
                TInt count( selArray->Count() );
                TInt ret( KErrNone );
                for ( TInt i = 0; i < count && ret == KErrNone; ++i )
                    {
                    const TInt selIndex( (*selArray)[i] );
                    ret = AddTopicL( iHandles->At(selIndex) );
                    }
                }
            // Ok to exit, dialog is also removed from the stack
            result = ETrue;
            iEikonEnv->RemoveFromStack( this );
            break;
            }
        // If Mark or Unmark is pressed, the dialog must not exit.
        // Dialog must also stay in stack.
        case EAknSoftkeyMark:            
        case EAknSoftkeyUnmark:
            {
            result = EFalse;
            break;
            }
        default:
            break;
        }           
            
    return result;
    }


//-----------------------------------------------------------------------------
//
//    CCbsUiAddFromIndexDialog::GetIndexNumber()
//
//    Parses Indexnumber from topic info. Is used by
//    SetTopics-member function.
//
//-----------------------------------------------------------------------------
void CCbsUiAddFromIndexDialog::GetIndexNumber( const TCbsTopicInfo& aTopicInfo , 
                                              TBuf<KTopicNumberWidth>& aIndexNumber ) const
    {	    
    TBuf<KTopicNumberWidth> ind;
    ind.AppendNum( TInt(aTopicInfo.iNumber) );
    aIndexNumber.Justify(ind , KTopicNumberWidth, ERight, KZeroFill);
    }

//-----------------------------------------------------------------------------
//
//    CCbsUiAddFromIndexDialog::AddTopicL()
//
// 
//
//-----------------------------------------------------------------------------
TInt CCbsUiAddFromIndexDialog::AddTopicL( const TCbsTopicInfo aTopicInfo )
    {
    TCbsTopic topic;
    
    topic.iHotmarked = EFalse;
    topic.iName = aTopicInfo.iName;
    topic.iNumber = aTopicInfo.iNumber;
    topic.iProtected = EFalse;
    topic.iSavedMessages = 0;
    topic.iUnreadMessages = 0;
    topic.iSubscribed = 0;
    
    TInt ret( iServer.AddTopic(topic) );
    
    switch ( ret )
        {
        case KErrNone:
            // ok, do nothing
            break;
        case KErrAlreadyExists: // same handling
        case KErrArgument:      // same handling
            // after this error still tryes to append other items if exists
            ret = KErrNone;
            break;
        default:
            User::LeaveIfError(ret);
            break;
        }
    
    return ret;
    } 

//-----------------------------------------------------------------------------
//
//    CCbsUiAddFromIndexDialog::SetIconsL()
//
//    Sets the multiselction box-images.
//
//-----------------------------------------------------------------------------
void CCbsUiAddFromIndexDialog::SetIconsL() const
    {
    CAknIconArray* icons = new (ELeave) 
        CAknIconArray(KNumberOfIconsInAddFromIndexDialog);
    
    CleanupStack::PushL(icons);
    icons->ConstructFromResourceL(R_CHECKBOX_ICONS);
    CleanupStack::Pop(); // icons
    
    iListBox->ItemDrawer()->ColumnData()->SetIconArray(icons);
    }

//-----------------------------------------------------------------------------
//
//    CCbsUiAddFromIndexDialog::SetTopicsL()
//
//
//-----------------------------------------------------------------------------
void CCbsUiAddFromIndexDialog::SetTopicsL()
    {
    TCbsTopicInfo topicinfo;    
    
    iServer.StartCollectionBrowsing();
    
    iHandles = new (ELeave) TCbsTopicInfoHandleList(KAssumedAverageOfItems);
    
    __ASSERT_DEBUG( iHandles, CbsUiPanic( ETopicHandleError ) );
    
    iHandles->Reset();
    TInt err(KErrNone);
    
    while( iServer.HasNextCollectionTopic() && err == KErrNone )
        {
        err = iServer.NextCollectionTopic(topicinfo);
        if ( err == KErrNone )
            {
            MDesCArray* itemList = iListBox->Model()->ItemTextArray();
            CDesCArray* itemArray = STATIC_CAST(CDesCArray*, itemList);
            
            TBuf<KTopicNumberWidth> index;
            GetIndexNumber( topicinfo, index );
            
            HBufC* buffer = HBufC::NewMaxLC(
                KCbsIconListSeparator().Length() + index.Length() + 
                KCbsColumnListSeparator().Length() + topicinfo.iName.Length() );
            
            TPtr ptr = buffer->Des();
            
            ptr = KCbsIconListSeparator;
            ptr.Append(index);
            ptr.Append(KCbsColumnListSeparator);
            ptr.Append(topicinfo.iName);
            
            itemArray->AppendL(*buffer);	
            iHandles->AppendL(topicinfo);
            CleanupStack::PopAndDestroy();  // buffer
            }
        }
    
    iListBox->HandleItemAdditionL();
    if ( iListBox->Model()->NumberOfItems() == 0 )
        {
        // No warning, but we can close the dialog
        User::Leave(KErrNone);
        }
    }

//-----------------------------------------------------------------------------
//
//    CCbsUiAddFromIndexDialog::CreateAddFromIndexTitlePaneL()
//
//
//-----------------------------------------------------------------------------
void CCbsUiAddFromIndexDialog::CreateAddFromIndexTitlePaneL() const
    {
    CEikStatusPane* sp = STATIC_CAST( CAknAppUi*, CEikonEnv::Static()->
        EikAppUi() )->StatusPane();
    
    CAknTitlePane* title = STATIC_CAST(CAknTitlePane*, sp->
        ControlL( TUid::Uid(EEikStatusPaneUidTitle) ));
    
    __ASSERT_DEBUG( sp && title, CbsUiPanic( EStatusOrTitlePaneError ) );
    
    TResourceReader titlereader;
    iCoeEnv->CreateResourceReaderLC( titlereader, R_QTN_CB_TITLE_ADD_TOPICS );
    title->SetFromResourceL( titlereader );
    CleanupStack::PopAndDestroy();  // resource title
    }

//  End of File  
