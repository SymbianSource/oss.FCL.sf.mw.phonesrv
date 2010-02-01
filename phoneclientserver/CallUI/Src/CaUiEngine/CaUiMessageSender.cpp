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
* Description:  Launches Sendui Editor.
*
*/




//  INCLUDES
#include 	"CaUiMessageSender.h" 

#include    <coemain.h>     	// CCoeEnv
#include    <CallUI.rsg>  		// Resources.

#include    <msvids.h>    		// KUidMsvLocalServiceMtmValue

#include    <sendui.h>     		// CSendAppUi, CSendUi
#include    <CMessageData.h>    // Message data for CSendUi
#include    <featmgr.h>   		// Feature manager
#include 	"CaUiLogger.h"		// Call Ui Logger

// CONSTANTS
// The reconnect query options granularity.
#ifdef RD_UNIFIED_EDITOR
const TInt KCaUiReConQueryOptionsArrayGranularity = 3;
#else // RD_UNIFIED_EDITOR
const TInt KCaUiReConQueryOptionsArrayGranularity = 4;
#endif // RD_UNIFIED_EDITOR

const TInt KMtmPop3Uid = 0x10001029;
const TInt KMtmImap4Uid = 0x1000102a;
const TInt KUidEmail = 0x10001028;
const TInt KUidPostCard = 0x10207245; 

// -----------------------------------------------------------------------------
// CCaUiMessageSender::CCaUiMessageSender
// 
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCaUiMessageSender::CCaUiMessageSender()
    {
    }


// -----------------------------------------------------------------------------
// CCaUiMessageSender::~CCaUiMessageSender
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiMessageSender::~CCaUiMessageSender()
    {
    
    CAUILOGSTRING("CALLUI: >>> CCaUiMessageSender::~CCaUiMessageSender()");
    
    if ( iMsgTypes )
        {
        iMsgTypes->Reset();
        delete iMsgTypes;
        }

    if( iSession ) 
        {
        iSession->Cancel();    
        }
    delete iSession;
        
    if( iSendUi ) 
        {    
    	delete iSendUi;   
        }
        
    CAUILOGSTRING("CALLUI: <<< CCaUiMessageSender::~CCaUiMessageSender()");    
    }


// -----------------------------------------------------------------------------
// CCaUiMessageSender::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaUiMessageSender* CCaUiMessageSender::NewL()
    {
    
    CAUILOGSTRING("CALLUI: >>> CCaUiMessageSender::NewL()");
    
    CCaUiMessageSender* self = new( ELeave ) CCaUiMessageSender ();
	
	CAUILOGSTRING("CALLUI: <<< CCaUiMessageSender::NewL()");
	
    return self;
    }

// ---------------------------------------------------------
// CCaUiMessageSender::CreateNewMessageL
// Create a new message of the given type, in the given location
// ---------------------------------------------------------
//
void CCaUiMessageSender::CreateNewMessageL(
    TInt aMessageType,
    MDesC16Array* aRealAddress,
    MDesC16Array* aAliases,
    CRichText* aRichText )
    {
    
    CAUILOGSTRING("CALLUI: >>> CCaUiMessageSender::CreateNewMessageL()");
       
    TRAPD( err, CreateSendUiL());
    if ( err != KErrNone )
	    User::Leave( KErrGeneral );		
    
    //Launch message editor standalone
    TUid mtmUid = { aMessageType };
    
    CMessageData* messageData = CMessageData::NewL();
    CleanupStack::PushL( messageData );
    if ( aRichText )
        {
        messageData->SetBodyTextL( aRichText );
        }
    
    // Set real address and check if aliases exist
    if ( aRealAddress )
        {
        TBool aAliasesExist = EFalse;
        if ( aAliases &&
             aAliases->MdcaCount() == aRealAddress->MdcaCount() )
            {
            aAliasesExist = ETrue;
            }
        for ( TInt i=0; i < aRealAddress->MdcaCount(); i++ )
            {
            if ( aAliasesExist )
                {
                messageData->AppendToAddressL(
                    aRealAddress->MdcaPoint( i ),
                    aAliases->MdcaPoint( i ) );
                }
            else
                {
                messageData->AppendToAddressL(
                    aRealAddress->MdcaPoint( i ) );
                }
            }
        }
        
    iSendUi->CreateAndSendMessageL( mtmUid, messageData, KNullUid, EFalse );
    CleanupStack::PopAndDestroy(); // messageData
    
    CAUILOGSTRING("CALLUI: <<< CCaUiMessageSender::CreateNewMessageL()");
    }

// ---------------------------------------------------------
// CCaUiMessageSender::CreateSendUiL
// ---------------------------------------------------------
//
void CCaUiMessageSender::CreateSendUiL()
    {
    CAUILOGSTRING("CALLUI: >>> CCaUiMessageSender::CreateSendUiL()");
    
    // Create instance of send ui.
    if( !iSendUi )
        {
        iSendUi = CSendUi::NewL();
        }
        
    CAUILOGSTRING("CALLUI: <<< CCaUiMessageSender::CreateSendUiL()");    
    }

// ---------------------------------------------------------
// CCaUiMessageSender::MessageTypesArray
// ---------------------------------------------------------
const CUidNameArray& CCaUiMessageSender::
    MessageTypesArrayL()
    {
    
    CAUILOGSTRING("CALLUI: >>> CCaUiMessageSender::MessageTypesArrayL()");
    
    delete iMsgTypes;
    iMsgTypes = NULL;

    CUidNameArray* msgTypes = new ( ELeave ) 
        CUidNameArray( KCaUiReConQueryOptionsArrayGranularity ); 
    CleanupStack::PushL( msgTypes );
    UpdateMessageTypesArrayL( msgTypes );
    CleanupStack::Pop();
    iMsgTypes = msgTypes;

	CAUILOGSTRING("CALLUI: <<< CCaUiMessageSender::MessageTypesArrayL()");

    return *iMsgTypes;
    }

// ---------------------------------------------------------
// CCaUiMessageSender::UpdateMessageTypesArrayL
// Updates the array of UIDs which defines the MTM types for which the user
// can create new messages.
// ---------------------------------------------------------
void CCaUiMessageSender::UpdateMessageTypesArrayL(
    CUidNameArray* aMsgTypes )
    {
    
    CAUILOGSTRING("CALLUI:>>> CCaUiMessageSender::UpdateMessageTypesArrayL()");
    
    aMsgTypes->Reset();

    FeatureManager::InitializeLibL();
    //Read MTM types from resource file
    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, 
        NEW_MESSAGE_TYPES_ARRAY );
    
    TInt count = resReader.ReadInt16();

    for ( TInt i = 0; i < count; i++ )
        {
        TInt32 id = resReader.ReadInt32();
        HBufC* name = resReader.ReadHBufCL();
        CleanupStack::PushL( name );

        switch( id )
            {
            case KUidEmail:
                {
                if ( IsMailboxDefinedL() )
                    {
                    aMsgTypes->AppendL( TUidNameInfo( TUid::Uid( id ), *name ) );
                    }
                break;
                }
            case KUidPostCard:
                {
                if ( FeatureManager::FeatureSupported( KFeatureIdMmsPostcard ) )
                    {
                    aMsgTypes->AppendL( TUidNameInfo( TUid::Uid( id ), *name ) );
                    }
                break;
                }
            default:
                {
                aMsgTypes->AppendL( TUidNameInfo( TUid::Uid( id ), *name ) );
                break;  
                }
            }
            
        CleanupStack::PopAndDestroy( name ); 
        }
    
    CleanupStack::PopAndDestroy();//resReader 
    FeatureManager::UnInitializeLib();
    
    CAUILOGSTRING("CALLUI:<<< CCaUiMessageSender::UpdateMessageTypesArrayL() end");
    }

// ---------------------------------------------------------
// CCaUiMessageSender::CreateMessageServiceConnectionL
// ---------------------------------------------------------
//
void CCaUiMessageSender::CreateMessageServiceConnectionL()
    {
    CAUILOGSTRING("CALLUI:>>> CCaUiMessageSender::CreateMessageServiceConnectionL()");
    
    if ( !iSession )
        {
        iSession = CMsvSession::OpenSyncL( *this );
        }
    CAUILOGSTRING("CALLUI:<<< CCaUiMessageSender::CreateMessageServiceConnectionL()");    
    }

// ---------------------------------------------------------
// CCaUiMessageSender::IsMailboxDefinedL
// ---------------------------------------------------------
//
TBool CCaUiMessageSender::IsMailboxDefinedL()
    {
    CAUILOGSTRING("CALLUI:>>> CCaUiMessageSender::IsMailboxDefinedL()");
    
    CreateMessageServiceConnectionL();
    
    TBool isDefined = EFalse;
    CMsvEntry* root = iSession->GetEntryL( KMsvRootIndexEntryId );

    TInt count = root->Count();
    for ( TInt i = 0; i < count; i++ )
        {
        const TMsvEntry& tentry = (*root)[i];
        if ( tentry.iMtm.iUid == KMtmPop3Uid || 
            tentry.iMtm.iUid == KMtmImap4Uid )
            {
            isDefined = ETrue;
            break;
            }
        }
    delete root;
    
    CAUILOGSTRING("CALLUI:<<< CCaUiMessageSender::IsMailboxDefinedL()");
    return isDefined;
    }


// ---------------------------------------------------------
// CCaUiMessageSender::HandleSessionEventL
// ---------------------------------------------------------
void CCaUiMessageSender::HandleSessionEventL(TMsvSessionEvent aEvent, 
        TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
    {
    CAUILOGSTRING("CALLUI:>>> CCaUiMessageSender::HandleSessionEventL()");
    
    CreateMessageServiceConnectionL();

    switch ( aEvent )
        {
        case EMsvCloseSession:
            //Server close.
            delete iSession;
            iSession = NULL;
            CAUILOGSTRING("CALLUI:<<< CCaUiMessageSender::HandleSessionEventL(): case: EMsvCloseSession");
            break;
        default:
        	CAUILOGSTRING("CALLUI:<<< CCaUiMessageSender::HandleSessionEventL(): case: default");
            break;
        }
        
    }


// End of File
