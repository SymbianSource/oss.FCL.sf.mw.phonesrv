/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Message sender stub.
*
*/


#ifndef CAUIMESSAGESENDER_H
#define CAUIMESSAGESENDER_H

//  INCLUDES
#include <e32base.h>
#include <msgarrays.h> // cuidnamearray 

// CLASS DECLARATION

/**
*  CaUIMessageSender implementation.
*
*  @lib CaUiEngine.lib
*  @since 3.1
*/
class CCaUiMessageSender : 
    public CBase
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        ~CCaUiMessageSender();

        /**
        * Two-phased constructor.
        * @return an instance of CCaUiMessageSender.
        */        
        static CCaUiMessageSender* NewL();

    public: // New functions
     
         /**
        * Create new message by given Id. The message editor will always 
        * be lauched stand alone.
        * @param aMessageType mtmUid indicate the message type
        *        aRealAddress address list to send message
        */      
        void CreateNewMessageL( TInt aMessageType,
             MDesC16Array* aRealAddress,
             MDesC16Array* aAliases,
             CRichText* aRichText );

    private:

        /**
        * C++ constructor.
        * 
        * @param aIndex After the query is dismissed, the index will hold
        *               the value of selected item.
        */
        CCaUiMessageSender( );

        /**
        * Create SendUi
        */
        void CreateSendUiL();

        /**
        * Get message types from mtm registry
        *
        * @param aMsgTypes array of UIDs (MTM types)
        *
        */
        void UpdateMessageTypesArrayL( CUidNameArray* aMsgTypes );

        /**
        * Check if mail box is defined
        */      
        TBool IsMailboxDefinedL();

        /**
        * Create connection to message service
        */
        void CreateMessageServiceConnectionL();

    private:

    };

#endif      // CAUIMESSAGESENDER_H 
            
// End of File
