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
* Description:  Implementation of CallUIEngine.
*
*/


// INCLUDES

#include <e32base.h>
#include <e32cmn.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <CallUI.rsg>                // Resources.
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <AknGlobalListQuery.h>
#include <AknGlobalNote.h>
#include <mcauireconnectqueryobserver.h>
#include "CaUiDialogs.h"
#include "CaUiLogger.h"
#include "CaUiQuery.h"
#include "CaUiEngine.h"
#include "CPhCntMatcher.h"
#include "CaUiActiveObject.h"

// CLASS DECLARATION

/**
*  Container for informatioin needed in Reconnection query.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
NONSHARABLE_CLASS( CCaUiReConQueryInformation ) 
    : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        * 
        * @see CCaUiEngApi::LaunchNoteAndReconConfQueryL().
        */
        CCaUiReConQueryInformation(
            MCaUiReconnectQueryObserver& aObserver,
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption,
            const TPtrC aNoteText );
        
        /**
        * Destructor.
        */
        ~CCaUiReConQueryInformation();


    public:    // Data

        // Observer storage.
        MCaUiReconnectQueryObserver&    iObserver;

        // Matcher storage.
        CPhCntMatcher*                  iPhCntMatcher;

        // Phone number storage.
        const TPtrC                     iPhoneNumber;

        // Video call option storage.
        const TBool                     iIncludeVideoCallOption;
    
        // The name corresponding the phone number.
        const TPtrC                     iAlias;

    };


// -----------------------------------------------------------------------------
// CCaUiReConQueryInformation::CCaUiReConQueryInformation
// 
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCaUiReConQueryInformation::CCaUiReConQueryInformation(
    MCaUiReconnectQueryObserver& aObserver,
    CPhCntMatcher* aPhCntMatcher,
    const TPtrC aPhoneNumber,
    const TBool aIncludeVideoCallOption,
    const TPtrC aAlias )
    : iObserver( aObserver ),
      iPhCntMatcher( aPhCntMatcher ),
      iPhoneNumber( aPhoneNumber ),
      iIncludeVideoCallOption( aIncludeVideoCallOption ),
      iAlias( aAlias )
    {
    }

// -----------------------------------------------------------------------------
// CCaUiReConQueryInformation::~CCaUiReConQueryInformation
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiReConQueryInformation::~CCaUiReConQueryInformation()
    {
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::CCaUiDialogs
// 
//
// -----------------------------------------------------------------------------
//
CCaUiDialogs::CCaUiDialogs( CCaUiEngine& aCaUiEngine )
    : iCaUiEngine( aCaUiEngine )
    {
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::NewL
// 
// 
// -----------------------------------------------------------------------------
//
CCaUiDialogs* CCaUiDialogs::NewL( CCaUiEngine& aCaUiEngine )
    {
    CCaUiDialogs* self = new( ELeave ) CCaUiDialogs( aCaUiEngine );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiDialogs::ConstructL()
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::ConstructL() Begin");
    // Create reconnect query, not visible yet.
    iReconListQuery = CAknGlobalListQuery::NewL();
    
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::ConstructL() End");
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::~CCaUiDialogs
// 
// 
// -----------------------------------------------------------------------------
//
CCaUiDialogs::~CCaUiDialogs()
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::~CCaUiDialogs() Begin");
    // Cancel confirmation query if exists.
    CancelReconConfQuery();
    delete iReconListQuery;
    
    // Delete reconnection query information.
    delete iReConQueryInformation;
    
    // Cancel and delete note timer if exists.
    if ( iNoteTimer )
        {
        iNoteTimer->Cancel();
        delete iNoteTimer;
        }

    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::~CCaUiDialogs() End");
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::QueryVideoCallDefaultActionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CCaUiDialogs::QueryVideoCallDefaultActionL()
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::QueryVideoCallDefaultActionL() Begin");
    HBufC* vtstring = NULL;
    HBufC* header = NULL;
    vtstring = StringLoader::LoadLC( R_CALLUI_VT_SETTING_NOTE );
    header = StringLoader::LoadLC( R_CALLUI_POPUP_VT_TITLE_SETTING );

    CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL( *vtstring );
    CleanupStack::PushL( dlg );
    dlg->SetHeaderTextL( *header );
    CleanupStack::Pop( dlg );

    // Show query note to user
    TInt result = dlg->ExecuteLD( R_CALLUI_VT_SETTING_QUERY );

    CleanupStack::PopAndDestroy( 2, vtstring ); // header, vtstring
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::QueryVideoCallDefaultActionL() End");
    
    return result;
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::LaunchCreateListQueryL
// 
// 
// -----------------------------------------------------------------------------
//
TBool CCaUiDialogs::LaunchCreateListQueryL( TPhCltCallType& aCallType )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::LaunchCreateListQueryL() Begin");
    
    TInt choice = KErrNotFound;

    CListMappingArray* mappingList = new ( ELeave ) CListMappingArray( 4 );
    CleanupStack::PushL( mappingList ); 

    // Create query dialog.
    CCaUiCreateListQuery* dlg = 
        new ( ELeave ) CCaUiCreateListQuery( &choice );
    dlg->PrepareLC( R_CALLUI_CREATE_LIST_QUERY );
    
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::LaunchCreateListQueryL() 1");

    // new clean list item array
    CDesCArray* textArray = new( ELeave ) CDesCArrayFlat( 4 );
    CleanupStack::PushL( textArray );

    FillListQueryItemsL( *textArray, *mappingList );

    CleanupStack::Pop( textArray );

    // Dialog takes the ownership of textArray          
    if( textArray )
        {
        dlg->SetOwnershipType( ELbmOwnsItemArray );
        dlg->SetItemTextArray( textArray );
        }

    // Highlight preferred item from listbox
    if( aCallType == EPhCltVideo )
        {
        dlg->ListBox()->SetCurrentItemIndex( KCallUiVideoCall );
        }

    TBool ok = dlg->RunLD();
    CAUILOGSTRING2("CALLUI: >>>CCaUiDialogs::LaunchCreateListQueryL() ok = %d", ok );
    CAUILOGSTRING2("CALLUI: >>>CCaUiDialogs::LaunchCreateListQueryL() choice = %d", choice );
    if ( ok  )
        {
        // Check what call type was selected.

        // Dynamic list made because of voip support
        // take calltype from mappinglist
        aCallType = mappingList->At( choice ).iCallType;
        }

    CleanupStack::PopAndDestroy( mappingList );

    return ok;
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::FillListQueryItemsL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiDialogs::FillListQueryItemsL( 
    CDesCArray& aTextArray, 
    CListMappingArray& aMappingList )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::FillListQueryItemsL() Begin");
    
    // Pointer for strings to be loaded.
    HBufC* bufPtr = NULL;
    TInt index = 0;
    TListMappingItem item;

    // Voice call option.
    bufPtr = StringLoader::LoadLC( R_CALLUI_CREATE_LIST_VOICE );
    aTextArray.AppendL( *bufPtr );
    CleanupStack::PopAndDestroy( bufPtr );  
    bufPtr = NULL;
    item.iIndex = index;
    item.iCallType = EPhCltVoice;
    aMappingList.AppendL( item );
    index++;
    
     // Video call option.
    if ( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::FillListQueryItemsL() Adding video call option");
        bufPtr = StringLoader::LoadLC( R_CALLUI_CREATE_LIST_VIDEO );
        aTextArray.AppendL( *bufPtr );
        CleanupStack::PopAndDestroy( bufPtr );
        bufPtr = NULL;
        item.iIndex = index;
        item.iCallType = EPhCltVideo;
        aMappingList.AppendL( item );
        index++;
        }
    // Internet call option is shown if dynamic voip is set on
    // and there are VoIP profiles defined
    if( iCaUiEngine.IsVoIPProfiles() )
        {
        // Internet call option.
        CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::FillListQueryItemsL() Adding internet call option");
        bufPtr = StringLoader::LoadLC( R_CALLUI_CREATE_LIST_INTERNET );
        aTextArray.AppendL( *bufPtr );
        CleanupStack::PopAndDestroy( bufPtr );
        bufPtr = NULL;
        item.iIndex = index;
        item.iCallType = EPhCltCallVoIP;
        aMappingList.AppendL( item );
        index++;
        }
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::FillListQueryItemsL() End");
    }


/** not tested **/

// -----------------------------------------------------------------------------
// CCaUiDialogs::LaunchNoNetSupNoteL
// 
//
// -----------------------------------------------------------------------------
//
void CCaUiDialogs::LaunchNoNetSupNoteL()
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::LaunchNoNetSupNoteL() Begin");
    HBufC* string = StringLoader::LoadLC( R_CALLUI_NO_VIDEO_NETWORK );  

    CAknInformationNote* dlg = new ( ELeave ) CAknInformationNote( EFalse );
    dlg->ExecuteLD( *string );
    dlg = NULL;

    CleanupStack::PopAndDestroy( string );
    string = NULL;
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::LaunchNoNetSupNoteL() End");
    }



// -----------------------------------------------------------------------------
// CCaUiDialogs::LaunchReconConfQueryL
// 
//
// -----------------------------------------------------------------------------
//
void CCaUiDialogs::LaunchReconConfQueryL( 
    MCaUiReconnectQueryObserver& aObserver,
    CPhCntMatcher* aPhCntMatcher,
    const TPtrC aPhoneNumber,
    const TBool aIncludeVideoCallOption,
    const TPtrC aAlias )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::LaunchReconConfQueryL() Begin ");
    CDesCArray* textArray = new( ELeave ) CDesCArrayFlat( 4 );
    CleanupStack::PushL( textArray );

    // Delete the old query if exists.
    CancelReconConfQuery();
    
    // Create new active object to handle query results.
    iReconActObject = CCaUiGlnActiveObject::NewL( 
        aObserver,
        aPhCntMatcher,
        aPhoneNumber,
        aAlias );

    CCaUiTIntArray* objectArray = iReconActObject->ReconQueryOptionsArray();
    
    // Pointer for strings to be loaded.
    HBufC* bufPtr = NULL;

    // Voice call option.
    bufPtr = StringLoader::LoadLC( R_CALLUI_RECONNECT_LIST_ITEM_VOICE );
    textArray->AppendL( *bufPtr );
    CleanupStack::PopAndDestroy( bufPtr );
    bufPtr = NULL;
    objectArray->AppendL( CCaUiGlnActiveObject::ECaUiVoiceCall );

    // Video call option is included only if it is wanted.
    if ( aIncludeVideoCallOption )
        {
        bufPtr = StringLoader::LoadLC( R_CALLUI_RECONNECT_LIST_ITEM_VIDEO );
        textArray->AppendL( *bufPtr );
        CleanupStack::PopAndDestroy( bufPtr );
        bufPtr = NULL;
        objectArray->AppendL( CCaUiGlnActiveObject::ECaUiVideoCall );
        }

#ifndef RD_UNIFIED_EDITOR
    // MMS option.
    if (FeatureManager::FeatureSupported(KFeatureIdMMS))
        {   
        bufPtr = StringLoader::LoadLC( R_CALLUI_RECONNECT_LIST_ITEM_MMS  );
        textArray->AppendL( *bufPtr );
        CleanupStack::PopAndDestroy( bufPtr );
        bufPtr = NULL;
        objectArray->AppendL( CCaUiGlnActiveObject::ECaUiMms );
        }

    // SMS option.
    bufPtr = StringLoader::LoadLC( R_CALLUI_RECONNECT_LIST_ITEM_SMS  );
    textArray->AppendL( *bufPtr );
    CleanupStack::PopAndDestroy( bufPtr );
    bufPtr = NULL;
    objectArray->AppendL( CCaUiGlnActiveObject::ECaUiSms );

#else  
    // Send message option.
    bufPtr = StringLoader::LoadLC( R_CALLUI_RECONNECT_LIST_ITEM_MESSAGE  );
    textArray->AppendL( *bufPtr );
    CleanupStack::PopAndDestroy( bufPtr );
    bufPtr = NULL;
    objectArray->AppendL( CCaUiGlnActiveObject::ECaUiMsg );
#endif // RD_UNIFIED_EDITOR

    // Load query header.
    bufPtr = StringLoader::LoadLC( R_CALLUI_RECONNECT_QUERY_HEADER  );
    iReconListQuery->SetHeadingL( *bufPtr ); // Set query heading.
    CleanupStack::PopAndDestroy( bufPtr );
    bufPtr = NULL;

    // Show query.
    iReconActObject->SetActive();
    iReconListQuery->ShowListQueryL( textArray, iReconActObject->iStatus );

    CleanupStack::PopAndDestroy( textArray );
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::LaunchReconConfQueryL() End ");
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::CancelReconConfQuery
//
//
// -----------------------------------------------------------------------------
//
void CCaUiDialogs::CancelReconConfQuery()
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::CancelReconConfQuery() Begin ");
    // Cancel the earlier query if exists.
    if( iReconListQuery )
        {
        iReconListQuery->CancelListQuery();
        }

    // Cancel the active object.
    if( iReconActObject )
        {
        if ( iReconActObject->IsActive() )
            {
            iReconActObject->Cancel();
            }
        }
    iReconfQueryOngoing = EFalse;
    delete iReconActObject;
    iReconActObject = NULL;
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::CancelReconConfQuery() End ");
    }


// -----------------------------------------------------------------------------
// CCaUiDialogs::CancelNoteAndReconConfQuery
// 
//
// -----------------------------------------------------------------------------
//
void CCaUiDialogs::CancelNoteAndReconConfQuery()
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::CancelNoteAndReconConfQuery() Begin ");
    // Delete reconnection query information.
    delete iReConQueryInformation;
    iReConQueryInformation = NULL;

    // Cancel Note timer if exists.
    if ( iNoteTimer )
        {
        // Cancel note timer.
        iNoteTimer->Cancel();
        }

    // Cancel the reconnect query if exists.
    if( iReconListQuery )
        {
        iReconListQuery->CancelListQuery();
        }

    // Cancel the active object.
    if( iReconActObject )
        {
        if ( iReconActObject->IsActive() )
            {
            iReconActObject->Cancel();
            }
        }

    delete iReconActObject;
    iReconActObject = NULL;
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::CancelNoteAndReconConfQuery() End ");
    }



// -----------------------------------------------------------------------------
// CCaUiDialogs::LaunchNoteAndReconConfQueryL
// 
//
// -----------------------------------------------------------------------------
//
void CCaUiDialogs::LaunchNoteAndReconConfQueryL( 
    MCaUiReconnectQueryObserver& aObserver,
    CPhCntMatcher* aPhCntMatcher,
    const TPtrC aPhoneNumber,
    const TBool aIncludeVideoCallOption,
    const TPtrC aNoteText,
    const TPtrC aAlias )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::LaunchNoteAndReconConfQueryL() Begin ");
    iReconfQueryOngoing = ETrue;
    delete iReConQueryInformation;
    iReConQueryInformation = NULL;
    iReConQueryInformation = new ( ELeave ) CCaUiReConQueryInformation(
        aObserver,
        aPhCntMatcher,
        aPhoneNumber,
        aIncludeVideoCallOption,
        aAlias );

    HBufC* string = NULL;
    TInt noteTextLength = aNoteText.Length();

    // Use either given note string or the qtn.tel.no.video.network string.
    if ( noteTextLength )
        {
        string = HBufC::NewLC( noteTextLength );
        string->Des() = aNoteText;
        }
    else
        {
        string = StringLoader::LoadLC( R_CALLUI_NO_VIDEO_NETWORK );
        }

    if ( !iInfoNote )
        {
        iInfoNote = new ( ELeave ) CAknInformationNote( EFalse );
        iInfoNote->SetTimeout( CAknNoteDialog::ELongTimeout );
        iInfoNote->ExecuteLD( *string );
        }
    
    // Start note wait timer.
    if ( !iNoteTimer )
        {
        iNoteTimer = CPeriodic::NewL( CActive::EPriorityStandard );
        }
    iNoteTimer->Cancel();
    iNoteTimer->Start(
        KCaUiEngNoteTimeout, 
        KCaUiEngNoteTimeout,
        TCallBack( DoPerformNoteLaunchCallBackL, this ) );

    CleanupStack::PopAndDestroy( string );
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::LaunchNoteAndReconConfQueryL() End");
    }



// -----------------------------------------------------------------------------
// CCaUiDialogs::DoPerformNoteLaunchCallBackL
// 
//
// -----------------------------------------------------------------------------
//
TInt CCaUiDialogs::DoPerformNoteLaunchCallBackL( TAny* aAny )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::DPNoteLaunchCallBackL() Begin");

    CCaUiDialogs* self = static_cast< CCaUiDialogs* >( aAny );

    if ( self )
        {
        if ( self->iNoteTimer )
            {
            // Cancel note timer.
            self->iNoteTimer->Cancel();
            }

        if ( self->iInfoNote )
            {
            // Information note still displayed
            return KErrNone;
            }

        if ( self->iReConQueryInformation && self->IsReconfQueryOngoing() )
            {
            // Launch reconnect query.
            self->LaunchReconConfQueryL(
                self->iReConQueryInformation->iObserver,
                self->iReConQueryInformation->iPhCntMatcher,
                self->iReConQueryInformation->iPhoneNumber,
                self->iReConQueryInformation->iIncludeVideoCallOption,
                self->iReConQueryInformation->iAlias );
            }

        delete self->iReConQueryInformation;
        self->iReConQueryInformation = NULL;
        }
    CAUILOGSTRING("CALLUI: >>>CCaUiDialogs::DPNoteLaunchCallBackL() end");
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::IsReconfQueryOngoing
// 
//
// -----------------------------------------------------------------------------
//
TBool CCaUiDialogs::IsReconfQueryOngoing()
    {
    return iReconfQueryOngoing;
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::IsReconfQueryOngoing
// 
//
// -----------------------------------------------------------------------------
//
void CCaUiDialogs::SetReconfQueryOngoing( const TBool aOngoing )
    {
    iReconfQueryOngoing = aOngoing;
    }
