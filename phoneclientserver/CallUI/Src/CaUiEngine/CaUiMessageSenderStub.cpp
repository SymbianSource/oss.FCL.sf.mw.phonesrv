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
* Description:  Message sender stub
*
*/


//  INCLUDES
#include    "cauimessagesenderstub.h" 

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
    }


// -----------------------------------------------------------------------------
// CCaUiMessageSender::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaUiMessageSender* CCaUiMessageSender::NewL()
    {
    CCaUiMessageSender* self = new( ELeave ) CCaUiMessageSender ();
    
    return self;
    }

// ---------------------------------------------------------
// CCaUiMessageSender::CreateNewMessageL
// Create a new message of the given type, in the given location
// ---------------------------------------------------------
//
void CCaUiMessageSender::CreateNewMessageL(
    TInt /*aMessageType*/,
    MDesC16Array* /*aRealAddress*/,
    MDesC16Array* /*aAliases*/,
    CRichText* /*aRichText*/ )
    {
    }

// ---------------------------------------------------------
// CCaUiMessageSender::CreateSendUiL
// ---------------------------------------------------------
//
void CCaUiMessageSender::CreateSendUiL()
    {   
    }


// ---------------------------------------------------------
// CCaUiMessageSender::UpdateMessageTypesArrayL
// Updates the array of UIDs which defines the MTM types for which the user
// can create new messages.
// ---------------------------------------------------------
void CCaUiMessageSender::UpdateMessageTypesArrayL(
    CUidNameArray* /*aMsgTypes*/ )
    {
    }

// ---------------------------------------------------------
// CCaUiMessageSender::CreateMessageServiceConnectionL
// ---------------------------------------------------------
//
void CCaUiMessageSender::CreateMessageServiceConnectionL()
    {    
    }

// ---------------------------------------------------------
// CCaUiMessageSender::IsMailboxDefinedL
// ---------------------------------------------------------
//
TBool CCaUiMessageSender::IsMailboxDefinedL()
    {
    TBool isDefined( EFalse );
    return isDefined;
    }

// End of File
