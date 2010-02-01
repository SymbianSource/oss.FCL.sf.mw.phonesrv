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
* Description:  This file contains the header file of the
*              : CConvergedServiceSelector class.
*
*/



#ifndef CCONVERGEDSERVICESELECTOR_H
#define CCONVERGEDSERVICESELECTOR_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS
//None

// MACROS
//None

// DATA TYPES
//None.

// FUNCTION PROTOTYPES
//None

// FORWARD DECLARATIONS
class CServiceHandler;

// CLASS DECLARATION

/**
*  Interface class for telephone to fetch calling service.
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
class CConvergedServiceSelector
        : public CBase
    {
    
    public:  // Enum and class.
    
        /**  Different type of calls. */ 
        enum TSsCallType
            {
            ESsVoiceCall = 0,       // Voice call.
            ESsVideoCall = 1,       // Video call.
            ESsVoipCall = 2         // Voip call.
            };
     
        /** Class for the results of the service selection.*/
        class TSsResult
            {
            public:
                // Call type
                TSsCallType iCallType;
                // Service id
                TUint iServiceId;
                // Current registration status
                // of the service
                TBool iServiceEnabled;
                
                       
            };
            
    public:  // Constructors and destructor
      
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CConvergedServiceSelector* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CConvergedServiceSelector();

    public: // New functions
        
        /**
        * Makes service selection for the call. 
        *  -If voice call is made by sendkey the call type may be changed as voip if
        *   there is preferred voip service available.
        *  -For voip calls checks the current connection status for the services and
        *   shows the confirmation queries (if needed) for the service registration.
        * @param aResult        Selection results are returned by this param.
        * @param aCallType      Current type of the call.
        * @param aServiceId     Current service of the call.
        * @param aSendKey       Optional parameter if call is started by SendKey.
        * @param aString        Phone number/address for call.
        * @return Error code indicating the success of the selection.
        *         KErrCancel if the user cancels a list/confirmation queries.
        */
        IMPORT_C TInt GetCallingServiceByCallType(
            TSsResult& aResult, 
            TSsCallType aCallType,
            TUint aServiceId,
            TBool aSendKey = ETrue,
            const TDesC& aString = KNullDesC );

        /**
        * Cancels selection.
        *  -Dismiss dialogs if any is currently shown and cancels the selection.
        *  -Selection can be canceled by deleting the
        *   CConvergedServiceSelector instance also.
        */
        IMPORT_C void CancelSelection();
        
    private:  // New functions
        
        
        void DoGetCallingServiceByCallTypeL(
            TSsResult& aResult, 
            TSsCallType aCallType,
            TUint aServiceId,
            TBool aSendKey,
            const TDesC& aString );

    private:
        
        /**
        * C++ default constructor.
        */
        CConvergedServiceSelector();


    private:    // Data
        
        CServiceHandler* iHandler;

    };

#endif      // CCONVERGEDSERVICESELECTOR_H
            
// End of File
