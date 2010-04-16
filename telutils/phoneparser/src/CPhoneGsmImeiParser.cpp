/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Parser for IMEI.
*
*/


// INCLUDE FILES
#include    "cphonegsmimeiparser.h" 
#include    "cphonegsmparserresult.h" 
#include    "cphonegsmoptioncontainer.h" 

// CONSTANTS

// IMEI code
_LIT( KPhoneImeiCode, "*#06#" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneGsmImeiParser::CPhoneGsmImeiParser
// -----------------------------------------------------------------------------
//
CPhoneGsmImeiParser::CPhoneGsmImeiParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneGsmImeiParser::NewLC
// -----------------------------------------------------------------------------
//
CPhoneGsmImeiParser* CPhoneGsmImeiParser::NewLC()
    {
    CPhoneGsmImeiParser* self = 
        new ( ELeave ) CPhoneGsmImeiParser;
    
    CleanupStack::PushL( self );
        
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneGsmImeiParser::ParseL
//
// Parse IMEI string.
// -----------------------------------------------------------------------------
//
TBool CPhoneGsmImeiParser::ParseL( 
        const TDesC& aString,
        CPhoneGsmParserResult& aResult,
        CPhoneGsmOptionContainerBase& aOptions )
    {
    aResult.ClearL();

    if ( !aOptions.FindOptionStatus( KPhoneOptionSend ))
        {
        if ( aString == KPhoneImeiCode )
            {
            aResult.SetUid( KPhoneUidIMEI );
            }
        }
   
    return ( aResult.Uid() == KPhoneUidIMEI );
    }

//  End of File  
