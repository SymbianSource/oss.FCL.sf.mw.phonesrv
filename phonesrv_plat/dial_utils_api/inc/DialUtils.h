/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  API for DialUtils usage.
*
*/


#ifndef DIALUTILS_H
#define DIALUTILS_H


//  INCLUDES
#include <e32base.h>


// CLASS DECLARATION

/**
*  The API for DialUtils usage.
*
*  @lib DialUtils.lib
*  @since 2.6
*/
class CDialUtilsApi 
:   public CBase
    {
    public: // New functions

        /**
        * Check and if needed modify the given phone number according to
        * current configuration and location. Currently supports:
        * 1) Japan prefix modifications.
        * 
        * @param aPhoneNumber The phone number to be checked and modified.
        * @return - KErrNone if successful,
        *         - KErrOverflow if the aPhoneNumber is too short for the 
        *           combined original phone number + prefix number,
        *         - Symbian error code otherwise.
        *         In error case the aPhoneNumber is untouched.
        */
        virtual TInt CheckNumber( HBufC& aPhoneNumber ) = 0;
    };



/**
*  Interface for DialUtils creation.
*
*  @lib DialUtils.lib
*  @since 2.6
*/
class CDialUtilsFactory
:   public CBase
    {
    public:

        /**
        * Creates DialUtils.
        * CDialUtilsFactory instance is deleted even in leave case.
        * 
        * @return CDialUtilsApi implementation. NULL if not supported.
        *         Ownership transferred.
        */
        virtual CDialUtilsApi* CDialUtilsApiLD() = 0;

    };

/**
* Only exported function. This is in index 1 in the lookup table.
* Call this function to create CDialUtilsFactory instance.
* 
* IMPORT_C CDialUtilsFactory* CreateDialUtilsFactoryL();
*/ 


#endif   // DIALUTILS_H

// End of File
