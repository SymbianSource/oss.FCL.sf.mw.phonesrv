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
*              : MServiceHandler class.
*
*/



#ifndef CSERVICEHANDLER_H
#define CSERVICEHANDLER_H

//  INCLUDES
#include "cconvergedserviceselector.h"


// CONSTANTS
//None

// MACROS
//None

// DATA TYPES
//None.

// FUNCTION PROTOTYPES
//None

// FORWARD DECLARATIONS
//None

// CLASS DECLARATION

/**
*  Interface class for service handlers
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
class CServiceHandler
    : public CBase
    {

    public: // New functions
        
        /**
        * Interface method for service handlers.
        *  -Leaves on failure, Leaves with error KErrCancel if the user
        *   cancels some of the queries.
        * @param aResult        Selection results are returned by this param.
        * @param aCallType      Current type of the call.
        * @param aServiceId     Service id of the call ( 0 meens not set ).
        * @param aSendKey       Is sendkey used for call.
        * @param aString        Phone number/address for call.
        */
        virtual void ExecuteServiceSelectionL( 
            CConvergedServiceSelector::TSsResult& aResult,
            CConvergedServiceSelector::TSsCallType aCallType, 
            TUint aServiceId,
            TBool aSendKey,
            const TDesC& aString = KNullDesC ) = 0;
   
        /**
        * Destructor.
        */
        virtual ~CServiceHandler(){};
        
    };

#endif      // CSERVICEHANDLER_H
            
// End of File
