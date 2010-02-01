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
* Description:  Active object to handle global list query results.
*
*/


#ifndef CAUIACTIVEOBJECT_H
#define CAUIACTIVEOBJECT_H


//  INCLUDES
#include <e32base.h>
#include <badesca.h>


// DATA TYPES
typedef CArrayFixFlat<TInt> CCaUiTIntArray;


// FORWARD DECLARATIONS
class MCaUiReconnectQueryObserver;
class CCoeEnv;
class CPhCntFactory;
class CPhCntMatcher;
class CCaUiMessageSender;

// CLASS DECLARATION

/**
*  Active object to handle global list query results.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
NONSHARABLE_CLASS( CCaUiGlnActiveObject )
:   public CActive
    {
    public: // Data types

        /* 
        * The reconnect query options.
        */
        enum TCaUiReconnectType
            {
            ECaUiVoiceCall = 0,  // Voice call.
            ECaUiVideoCall = 1,  // Video call.
  #ifdef RD_UNIFIED_EDITOR
            ECaUiMsg       = 2   // Unified editor.
  #else // RD_UNIFIED_EDITOR
            ECaUiMms       = 2,  // MMS.
            ECaUiSms       = 3   // SMS.
  #endif // // RD_UNIFIED_EDITOR
            };


        /**
        *   The editor types that can be launched.
        *   
        *   ECaUiEditorSMS SMS editor.
        *   ECaUiEditorMMS MMS editor.
        */
        enum TCaUiEditorType
            {
  #ifdef RD_UNIFIED_EDITOR
            ECaUiEditorMSG = 0
  #else // RD_UNIFIED_EDITOR
            ECaUiEditorSMS = 0,
            ECaUiEditorMMS = 1
  #endif // RD_UNIFIED_EDITOR      
            };

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * 
        * @param aObserver The observer to handle query results.
        * @param aPhCntMatcher The Phone Contact Matcher. 
        *        Ownership not transferred. If NULL, then matcher is created 
        *        inside CCaUiGlnActiveObject.
        * @param aPhoneNumber Pointer to the number where dial was tried.
        * @param aAlias The alias corresponding the number. Can be NULL.
        * @return Constructed CCaUiGlnActiveObject instance.
        */
        static CCaUiGlnActiveObject* NewL( 
            MCaUiReconnectQueryObserver& aObserver,
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TPtrC aAlias );
        
        /**
        * Destructor.
        */
        virtual ~CCaUiGlnActiveObject();


    public: // New functions


        /**
        * Returns reconnect query options array, that must include options
        * in the same order and in the same locations than they are in 
        * reconnect query. Insert only TCaUiReconnectType members.
        * 
        * @return Reconnect query options array.
        */
        CCaUiTIntArray* ReconQueryOptionsArray();


    public: // Functions from base classes

        /**
        * @see CActive::DoCancel.
        */
        void DoCancel();

        /**
        * @see CActive::RunL.
        */
        void RunL();


        /**
        * @see CActive::SetActive.
        */
        void SetActive();
        

    private:

        /**
        * C++ constructor.
        * 
        * @param aObserver The observer to handle query results.
        */
        CCaUiGlnActiveObject( 
            MCaUiReconnectQueryObserver& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @see CCaUiGlnActiveObject::NewL().
        */
        void ConstructL( 
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TPtrC aAlias );


        /**
        * Launch the desired editor.
        * 
        * @param aEditorType The editor type to be launched.
        */
        void LaunchEditorL( TCaUiEditorType aEditorType );

        /**
        * Creates message sender.
        * 
        */        
        void CreateMessageSenderL();

    private:    // Data


        // The query observer.
        MCaUiReconnectQueryObserver*    iObserver;

        // Storage for real addresses.
        CDesCArrayFlat*                 iRealAddress;

        // Storage for aliases.
        CDesCArrayFlat*                 iAliases;

        // Storage for reconnect query options.
        CCaUiTIntArray*                 iReConQueryOptions; 

        // Phone Contact finder factory.
        CPhCntFactory*                  iPhCntFactory;

        // Phone Contact matcher.
        CPhCntMatcher*                  iPhCntMatcher;

        // Is CPhCntMatcher owned.
        TBool   						iOwnMatcher;
        
        // Message sender
        CCaUiMessageSender*				iMessageSender;

    };

/**
*  Active object to handle note results.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
NONSHARABLE_CLASS( CCaUiNoteActiveObject )
:   public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * 
        * @return Constructed CCaUiNoteActiveObject instance.
        */
        static CCaUiNoteActiveObject* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCaUiNoteActiveObject();



    public: // Functions from base classes

        /**
        * @see CActive::DoCancel.
        */
        void DoCancel();

        /**
        * @see CActive::RunL.
        */
        void RunL();

        /**
        * @see CActive::SetActive.
        */
        void SetActive();
        

    private:

        /**
        * C++ default constructor.
        * 
        */
        CCaUiNoteActiveObject();
    };

#endif      // CCAUIACTIVEOBJECT_H 
            
// End of File
