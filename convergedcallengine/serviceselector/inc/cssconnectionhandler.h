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
*              : CSsConnectionHandler class.
*
*/


#ifndef CSSCONNECTIONHANDLER_H
#define CSSCONNECTIONHANDLER_H

//  INCLUDES
#include <e32base.h>
// <-- QT PHONE START -->
//#include <cch.h>
// <-- QT PHONE END-->

// FORWARD DECLARATIONS
// None

// CLASS DECLARATION

/**
*  Interface to get registration info for services.
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( CSsConnectionHandler )
        : public CBase
    {
    public:  // Constructors and destructor
      
        /**
        * Two-phased constructor.
        */
        static CSsConnectionHandler* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CSsConnectionHandler();

    public: // New functions
        
        /**
        * Checks is the service registered.
        * @param aServiceId     Id of the service.
        * @return   ETrue if service registered.
        */
        TBool IsVoipServiceRegistered( TUint aServiceId ) const;

    private:  // New functions
   
        /**
        * C++ default constructor.
        */
        CSsConnectionHandler();
        
        /**
        * Constructor, second phase. 
        */
        void ConstructL();

    private:    // Data
    
        // Converged Connection Handler client interface
        // <-- QT PHONE START -->
        //CCch* iCch;
        // <-- QT PHONE END-->


    };

#endif      // CSSCONNECTIONHANDLER_H
            
// End of File
