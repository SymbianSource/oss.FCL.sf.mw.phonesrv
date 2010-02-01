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
*              : CSsServiceUtilities class.
*
*/



#ifndef CSSSERVICEUTILITIES_H
#define CSSSERVICEUTILITIES_H

//  INCLUDES
#include "cconvergedserviceselector.h"
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
//None

// CLASS DECLARATION

/**
*  Static interface class for service selector utilities
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( SsServiceUtilities )
    {

    public: // New functions
        
        /**
        * Returns service selector by calltype. Leaves on failure.
        * @param aCallType        Call type
        * @param aSendKey         ETrue if SendKey used
        * @return CServiceHandler Call type handler.
        */
        static CServiceHandler* ServiceHandlerByCallTypeL( 
            CConvergedServiceSelector::TSsCallType aCallType,
            TBool aSendKey );
            
        /**
        * Sets the results to TSsResult by params (calltype, service id and 
        * registration status).
        * @param aResult        Results filled by parameters.
        * @param aCallType      Call type
        * @param aServiceId     Current service if of the call.
        * @param aEnabled       ETrue if service enabled
        */
        static void SetServiceResult( 
            CConvergedServiceSelector::TSsResult& aResult, 
            CConvergedServiceSelector::TSsCallType aCallType,
            TUint aServiceId,
            TBool aEnabled );
        
        /**
        * Checks is the string valid for cs voice call.
        * @param aString String to be checked.
        * @return ETrue if valid cs number.
        */
        static TBool IsValidCsNumber( const TDesC& aString );
        
    };

#endif      // CSSSERVICEUTILITIES_H
            
// End of File
