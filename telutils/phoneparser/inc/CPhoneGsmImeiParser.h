/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It is parser for Imei string.
*
*/


#ifndef CPHONEGSMIMEIPARSER_H_
#define CPHONEGSMIMEIPARSER_H_

// INCLUDES
#include    "CPhoneGsmParserBase.h"

// CONSTANTS


// CLASS DECLARATION

/**
* It is parser for IMEI string.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmImeiParser )
    : public CPhoneGsmParserBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmImeiParser* NewLC();
        
    public: // Functions from base classes

        /**
        * From CPhoneGsmParserBase, parses string.
        *
        * @param aString string to be parsed.
        * @param aResult It will contain result.
        * @param aOptions It contains all options related to parsing.
        * @return It returns boolean value indicating success of parsing.
        */
        TBool ParseL( 
            const TDesC& aString,
            CPhoneGsmParserResult& aResult,
            CPhoneGsmOptionContainerBase& aOptions );

    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmImeiParser();

    };

#endif /*CPHONEGSMIMEIPARSER_H_*/

// End of File
