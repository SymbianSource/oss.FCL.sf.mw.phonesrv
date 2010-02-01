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
*              : CSsServiceHandlerVoip class.
*
*/



#ifndef CSSSETTINGSHANDLER_H
#define CSSSETTINGSHANDLER_H

//  INCLUDES
#include "cssconnectionhandler.h"
#include <spsettings.h>
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

// CLASS DECLARATION

/**
*  VoIP settings handler class for service selector
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( CSsSettingsHandler )
        : public CBase
    {
    public:  // Constructors and destructor
      
        /**
        * Two-phased constructor.
        */
        static CSsSettingsHandler* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CSsSettingsHandler();

    public: // New functions
        
        /**
        * Gets registered services.
        * @param aConnection    Connection handler.
        * @param aServiceIds    Array of found services.
        * @return KErrNone if registered service(s) exists.
        */
        TInt GetRegisteredServices(
                CSsConnectionHandler& aConnection, 
                RArray<TUint>& aServiceIds ) const;
            
        /**
        * Gets all available services.
        * @param aServiceIds    Array of found services.
        * @param aServiceNames  Service name array of found services.
        * @return KErrNone if registered service(s) exists.
        */
        TInt GetAllServices(
                RArray<TUint>& aServiceIds ) const;
        
        /**
        * Gets service names from sp settings by service id array.
        * @param aServiceIds    Service id array.
        * @param aServiceNames  Names of the services are returned
        *                       by the parameter.
        */
        void GetServiceNamesL( RArray<TUint>& aServiceIds,
                               CDesCArray& aServiceNames ) const;

        /**
        * Gets brand id for service.
        * @since S60 5.0
        * @param aServiceId Used Service
        * @param aBrandId On completion, contains Brand Id
        */
        void BrandIdL( TInt aServiceId, TDes8& aBrandId );
        
    private:  // New functions
 
        /**
        * Gets registered services. Leaves on failure.
        * Leaves with KErrNotFound if not any registered
        * services available.
        * @param aConnection    Connection handler.
        * @param aServiceIds    Array of found services.
        */
        void DoGetRegisteredServicesL(
                CSsConnectionHandler& aConnection, 
                RArray<TUint>& aServiceIds ) const;
            
        /**
        * Gets all available services. Leaves on failure.
        * Leaves with KErrNotFound if not any VoIP capable
        * services available.
        * @param aServiceIds    Array of found services.
        */
        void DoGetAllServicesL(
                RArray<TUint>& aServiceIds ) const;
 
        /**
        * Gets voip capable services from sp settings.
        * @param aServiceIds    Array of the voip capable services.
        */
        void GetVoipServicesL( RArray<TUint>& aServiceIds ) const;
        
        /**
        * Checks is the service as voip service.
        * @param aServiceId     Id of the service.
        * @return ETrue if service is voip capable service.
        */
        TBool IsVoipServiceL( TUint aServiceId ) const;
 
        
    private:

        /**
        * C++ default constructor.
        */
        CSsSettingsHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        

    private:    // Data
        
        // Pointer to SP settings
        CSPSettings*  iSettings;

    };

#endif      // CSSSETTINGSHANDLER_H
            
// End of File
