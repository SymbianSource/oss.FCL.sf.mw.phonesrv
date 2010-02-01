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
*              : CSsServiceHandlerGsm class.
*
*/



#ifndef CSSSERVICEHANDLERGSM_H
#define CSSSERVICEHANDLERGSM_H

//  INCLUDES
#include "cservicehandler.h"
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
*  Gsm service handler class for service selector
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( CSsServiceHandlerGsm )
        : public CServiceHandler
    {
    public:  // Constructors and destructor
      
        /**
        * Two-phased constructor.
        */
        static CSsServiceHandlerGsm* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CSsServiceHandlerGsm();

    public: // New functions
       

    protected:  // New functions

        /**
        * From CServiceHandler
        */     
        void ExecuteServiceSelectionL( 
            CConvergedServiceSelector::TSsResult& aResult,
            CConvergedServiceSelector::TSsCallType aCallType, 
            TUint aServiceId,
            TBool aSendKey,
            const TDesC& aString = KNullDesC );
            
    private:

        /**
        * C++ default constructor.
        */
        CSsServiceHandlerGsm();
        

    private:    // Data

    };

#endif      // CSSSERVICEHANDLERGSM_H
            
// End of File
