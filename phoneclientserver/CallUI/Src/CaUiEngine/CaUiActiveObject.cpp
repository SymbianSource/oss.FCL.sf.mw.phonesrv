/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Active object to handle global list query results.
*
*/




// INCLUDE FILES
#include    "CaUiActiveObject.h"
#include    "CaUiQueryObserver.h"
#include    <coemain.h>

#include    <mcauireconnectqueryobserver.h>
#include    <cphonecntfactory.h>
#include    <cphcntmatcher.h>
#include    <mphcntmatch.h>
#include	"CaUiMessageSender.h"
#include	"cauilogger.h"
// CONSTANTS



// The message editor granularity.
const TInt KCaUiMessageEditorArrayGranularity = 1;

// The reconnect query options granularity.
#ifdef RD_UNIFIED_EDITOR
const TInt KCaUiReConQueryOptionsArrayGranularity = 3;
#else // RD_UNIFIED_EDITOR
const TInt KCaUiReConQueryOptionsArrayGranularity = 4;
#endif // RD_UNIFIED_EDITOR
// The editor values that this active object supports.
enum
    {
#ifdef RD_UNIFIED_EDITOR
    ECaUiEditorMSG = 0   // Unified editor.
#else  // RD_UNIFIED_EDITOR
    ECaUiEditorSMS = 0,  // SMS editor.
    ECaUiEditorMMS = 1   // MMS editor.
#endif // RD_UNIFIED_EDITOR
    };


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::CCaUiGlnActiveObject
// 
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCaUiGlnActiveObject::CCaUiGlnActiveObject( 
    MCaUiReconnectQueryObserver& aObserver )
:   CActive( EPriorityStandard ),
    iObserver( &aObserver )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::ConstructL
// 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaUiGlnActiveObject::ConstructL(
    CPhCntMatcher* aPhCntMatcher,
    const TPtrC aPhoneNumber,
    const TPtrC aAlias )
    {
    CAUILOGSTRING("CALLUI: >>> CCaUiGlnActiveObject::ConstructL() begin");
    iReConQueryOptions = new ( ELeave ) CCaUiTIntArray( 
        KCaUiReConQueryOptionsArrayGranularity );

    // Get recipient info - is always available here
    iRealAddress = 
        new ( ELeave ) CDesCArrayFlat( KCaUiMessageEditorArrayGranularity );
    iRealAddress->AppendL( aPhoneNumber );

    iAliases = new ( ELeave ) CDesC16ArrayFlat( 
        KCaUiMessageEditorArrayGranularity );

    if ( aAlias.Length() != 0 )
        {
        // Alias was given, so matcher is not needed.
        iAliases->AppendL( aAlias );

        // Matcher does not exist.
        iOwnMatcher = EFalse;
        }
    else
        {
        // Create matcher objects if needed.
        if ( !aPhCntMatcher )
            {
            iOwnMatcher = ETrue;
            iPhCntFactory = CreateCntFactoryL();
            iPhCntMatcher = iPhCntFactory->CreateContactMatcherL();
            }
        else
            {
            iOwnMatcher = EFalse;
            iPhCntMatcher = aPhCntMatcher;
            }
        }
    CAUILOGSTRING("CALLUI: >>> CCaUiGlnActiveObject::ConstructL() end");
    }


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaUiGlnActiveObject* CCaUiGlnActiveObject::NewL( 
    MCaUiReconnectQueryObserver& aObserver,
    CPhCntMatcher* aPhCntMatcher,
    const TPtrC aPhoneNumber,
    const TPtrC aAlias )
    {
    CCaUiGlnActiveObject* self = new( ELeave ) CCaUiGlnActiveObject (
        aObserver );
    
    CleanupStack::PushL( self );
    self->ConstructL( 
        aPhCntMatcher,
        aPhoneNumber,
        aAlias );
    CleanupStack::Pop();

    return self;
    }


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::~CCaUiGlnActiveObject
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiGlnActiveObject::~CCaUiGlnActiveObject()
    {
    Cancel();

    delete iReConQueryOptions;

    // Delete arrays.
    delete iRealAddress;
    delete iAliases;

    // Delete Phone Contact Finder objects if owned.
    if ( iOwnMatcher )
        {
        delete iPhCntMatcher;
        delete iPhCntFactory;
        }

    if( iMessageSender )
        {
        delete iMessageSender;
        }

    }


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::ReconQueryOptionsArray
// 
// 
// -----------------------------------------------------------------------------
//
CCaUiTIntArray* CCaUiGlnActiveObject::ReconQueryOptionsArray()
    {
    return iReConQueryOptions;
    }


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::DoCancel
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiGlnActiveObject::DoCancel()
    {
    iObserver = NULL;
    }


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::RunL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiGlnActiveObject::RunL()
    {
    CAUILOGSTRING("CALLUI: >>> CCaUiGlnActiveObject::RunL() Begin");
    
    MCaUiReconnectQueryObserver::TCaUiReconType recontype = 
        MCaUiReconnectQueryObserver::ECallUiReconTypeCancel;
    TInt result = iStatus.Int();
    TInt count = ReconQueryOptionsArray()->Count();

    // If some option was chosen, change the result to contain right value.
    if ( result >= 0 && result <= count )
        {
        result = ( *ReconQueryOptionsArray() )[ result ];
        }
    else
        {
        result = KErrCancel;
        }
        
    CAUILOGSTRING2("CALLUI: CCaUiGlnActiveObject::RunL(): result: %d", result);    
    switch ( result )
        {
        case ECaUiVoiceCall:
            recontype = MCaUiReconnectQueryObserver::ECallUiReconTypeVoice;
            break;

        case ECaUiVideoCall:
            recontype = MCaUiReconnectQueryObserver::ECallUiReconTypeVideo;
            break;
#ifndef RD_UNIFIED_EDITOR
        case ECaUiMms:
            CAUILOGSTRING("CALLUI: <<< CCaUiGlnActiveObject::RunL() ECaUiMms");
            recontype = MCaUiReconConfQueryObserver::ECallUiReconTypeMms;
            LaunchEditorL( ECaUiEditorMMS );
            break;

        case ECaUiSms:
            CAUILOGSTRING("CALLUI: <<< CCaUiGlnActiveObject::RunL() ECaUiSms");
            recontype = MCaUiReconConfQueryObserver::ECallUiReconTypeSms;
            LaunchEditorL( ECaUiEditorSMS );
            break;
#else // RD_UNIFIED_EDITOR
        case ECaUiMsg:
            CAUILOGSTRING("CALLUI: <<< CCaUiGlnActiveObject::RunL() ECaUiMsg");
            recontype = MCaUiReconnectQueryObserver::ECallUiReconTypeMsg;
            LaunchEditorL( ECaUiEditorMSG );
            break;
#endif // RD_UNIFIED_EDITOR
        default:
            break;
        }

    iObserver->OptionSelected( recontype );
    
    Cancel();
    
    CAUILOGSTRING("CALLUI: <<< CCaUiGlnActiveObject::RunL() end");
    }


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::SetActive
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiGlnActiveObject::SetActive()
    {
    if( !IsActive() )
        {
        CActive::SetActive();
        }
    }

// ---------------------------------------------------------
// CCaUiGlnActiveObject::CreateMessageSenderL
// ---------------------------------------------------------
//
void CCaUiGlnActiveObject::CreateMessageSenderL()
    {
    // Create instance of send ui.
    if( !iMessageSender )
        {
        iMessageSender = CCaUiMessageSender::NewL();
        }
    }


// -----------------------------------------------------------------------------
// CCaUiGlnActiveObject::LaunchEditorL
// -----------------------------------------------------------------------------
//
void CCaUiGlnActiveObject::LaunchEditorL( TCaUiEditorType aEditorType )
    {
    CAUILOGSTRING("CALLUI: >>> CCaUiGlnActiveObject::LaunchEditorL()");
    
    CreateMessageSenderL();
     
    const CUidNameArray& msgTypes = iMessageSender->MessageTypesArrayL();   

    if ( !( iAliases->Count() ) )
        {
        // Name not yet known, so try to solve it.
        MPhCntMatch* match = NULL;
        TInt ret = iPhCntMatcher->MatchNumber( 
            match, 
            ( *iRealAddress )[0] ); // First and only phonenumber.
        CleanupStack::PushL( match );
    
        if ( ret == KErrNone && match )
            {
            HBufC* cliText = NULL;

            MPhCntMatch::TCliType cliType = match->Cli( cliText );
            CleanupStack::PushL( cliText );

            if ( cliType == MPhCntMatch::ECliName )
                {
                iAliases->AppendL( *cliText );
                }
            CleanupStack::PopAndDestroy( cliText );
            }

        // Pop and destroy match.
        CleanupStack::Pop( match );
        if ( match )
            {
            match->Release();
            }
        }


    TInt messageType = 0; // Editor type, default value.
    TBool launchQuery = EFalse;

    switch ( aEditorType )
        {
 #ifndef RD_UNIFIED_EDITOR
        case ECaUiEditorSMS:
            messageType = msgTypes[ ECaUiEditorSMS ].iUid.iUid;  // SMS editor.
            launchQuery = ETrue;
            break;

        case ECaUiEditorMMS:
            messageType = msgTypes[ ECaUiEditorMMS ].iUid.iUid;  // MMS editor.
            launchQuery = ETrue;
            break;
 #else // RD_UNIFIED_EDITOR          
        case ECaUiEditorMSG:
            messageType = msgTypes[ ECaUiEditorMSG ].iUid.iUid;  // Unified message editor.
            launchQuery = ETrue;
            break;
 #endif // RD_UNIFIED_EDITOR
        default:
            break;
        }

	CAUILOGSTRING2("CALLUI: CCaUiGlnActiveObject::LaunchEditorL(): Message type: %d", messageType);
	CAUILOGSTRING2("CALLUI: CCaUiGlnActiveObject::LaunchEditorL(): Launch query: %d", launchQuery);

    // If the editor type was valid, launch the editor.
    if ( launchQuery )
        {
        //Launch SMS Editor.
        iMessageSender->CreateNewMessageL( 
            messageType, 
            iRealAddress, 
            iAliases,
            NULL );
        }
        
    CAUILOGSTRING("CALLUI: <<< CCaUiGlnActiveObject::LaunchEditorL()");    
    }

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaUiNoteActiveObject::CCaUiNoteActiveObject
// 
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCaUiNoteActiveObject::CCaUiNoteActiveObject()
:   CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CCaUiNoteActiveObject::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaUiNoteActiveObject* CCaUiNoteActiveObject::NewL()
    {
    CCaUiNoteActiveObject* self = new( ELeave ) CCaUiNoteActiveObject ();

    return self;
    }


// -----------------------------------------------------------------------------
// CCaUiNoteActiveObject::~CCaUiNoteActiveObject
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiNoteActiveObject::~CCaUiNoteActiveObject()
    {
    Cancel();
    }


// -----------------------------------------------------------------------------
// CCaUiNoteActiveObject::DoCancel
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiNoteActiveObject::DoCancel()
    {
    }


// -----------------------------------------------------------------------------
// CCaUiNoteActiveObject::RunL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiNoteActiveObject::RunL()
    {
    Cancel();
    }



// -----------------------------------------------------------------------------
// CCaUiNoteActiveObject::SetActive
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiNoteActiveObject::SetActive()
    {
    if( !IsActive() )
        {
        CActive::SetActive();
        }
    }


// End of File
