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
* Description:  Launches Send Ui.
*
*/


#ifndef CAUIMESSAGESENDER_H
#define CAUIMESSAGESENDER_H

//  INCLUDES
#include <e32base.h>
#include <msvapi.h>
#include <MsgArrays.h>  // CUidNameArray

// CLASS DECLARATION
class CMsvSession;
class CSendUi;

// CLASS DECLARATION

/**
*  CaUIMessageSender implementation.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
class CCaUiMessageSender : 
    public CBase,
    private MMsvSessionObserver
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

        /**
        * Get messgae types array
        */		    
		const CUidNameArray& MessageTypesArrayL();    
	
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

    protected:  // Functions from base classes

        /**
        * From MMsvSessionObserver
        */
        virtual void HandleSessionEventL(TMsvSessionEvent aEvent, 
            TAny* aArg1, TAny* aArg2, TAny* aArg3);

	private:
	
		// Message service session
        CMsvSession* iSession;
        
        // Send UI
        CSendUi*    iSendUi;
        
        // Array of MTM types
        CUidNameArray* iMsgTypes;	

    };

#endif      // CAUIMESSAGESENDER_H 
            
// End of File
