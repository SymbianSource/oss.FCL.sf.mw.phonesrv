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
*              : CSsServiceHandlerVoice class.
*
*/



#ifndef CSSSERVICEHANDLERVOICE_H
#define CSSSERVICEHANDLERVOICE_H

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
class CSPSettingsVoIPUtils;

// CLASS DECLARATION

/**
*  Voice service handler class for service selector
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( CSsServiceHandlerVoice )
        : public CServiceHandler
    {
    public:  // Constructors and destructor
      
        /**
        * Two-phased constructor.
        */
        static CSsServiceHandlerVoice* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CSsServiceHandlerVoice();

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
            
    private:  // New functions

        /**
        * Checks is the service registered
        * @param aServiceId Id of the service.
        */     
        TBool IsServiceRegistered( TUint aServiceId ) const;     

    private:

        /**
        * C++ default constructor.
        */
        CSsServiceHandlerVoice();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


    private:    // Data
        
        // Pointer to voip utils
        CSPSettingsVoIPUtils*   iVoipUtils;

    };

#endif      // CSSSERVICEHANDLERVOICE_H
            
// End of File
