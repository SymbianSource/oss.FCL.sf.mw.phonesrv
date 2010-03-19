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
* Description:  Implementation of the VmbxUtilities class
*
*/


// INCLUDE FILES
#include <featmgr.h>
#include <msssettingsobserver.h> // ALS changes

#include "vmbxlogger.h"
#include "vmbxcenrephandler.h"
#include "vmbxemergencycall.h"
#include "vmbxutilities.h"

// CONSTANTS
const TInt KVmbxPhoneNumMinLength = 2;
const TInt KVmbxPhoneNumMaxLength = 40;
const TInt KVmbxPhoneCharMaxLength = 48;

_LIT( KAllowedTelNumChars, "0123456789" );
_LIT( KAllowedDtmfChars, "0123456789+pwPW" );
_LIT( KAllowedSSChars, "*+0123456789#" );

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// VmbxUtilities::IsValidPhoneNumber
// Validates phone number
// ---------------------------------------------------------------------------
//
TBool VmbxUtilities::IsValidPhoneNumber( const TDesC& aNumber )
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::IsValidPhoneNumber: =>" );
    TBool result( EFalse );
    if ( KVmbxPhoneNumMinLength < aNumber.Length() 
        && KVmbxPhoneCharMaxLength >= aNumber.Length() )
        {
        VMBLOGSTRING2( "VMBX: VmbxUtilities::IsValidPhoneNumber: aNumber %S",
        &aNumber );
        TLex lexer( aNumber );
        lexer.SkipSpace();
        TChar current = lexer.Peek();
        // If no SS code then consider this is valid and return true.    
        if ( '#' == current || '*' == current )
            {
            // check whether accord ss regulation,
            result = ValidateSsNum( lexer );
            }
        else
            {
            result = ValidateTelNum( lexer );
            // If zero length then consider as valid and return value of 
            // result, If not the end of the string, check if it's DTMF numbers
            if ( !lexer.Eos() && result )
                {
                result = ValdateDtmfPart( lexer );
                }
            }
        }
    VMBLOGSTRING2( "VMBX: VmbxUtilities::IsValidPhoneNumber: result = %d<=",
        result );
    return result;
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::AlsLine
// 
// ---------------------------------------------------------------------------
//
TVmbxAlsLineType VmbxUtilities::AlsLine()
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::AlsLine: =>" );
    TInt ssLine( ESSSettingsAlsNotSupported );
    TVmbxAlsLineType alsLine( EVmbxAlsLineDefault );
    RSSSettings ssSettings;
    TInt res = ssSettings.Open();
    if( KErrNone == res )
        {
        res = ssSettings.Get( ESSSettingsAls, ssLine );
        if( KErrNone == res )
            {
            if ( ESSSettingsAlsAlternate == ssLine )
                 {
                 alsLine = EVmbxAlsLine2;
                 }
            else if ( ESSSettingsAlsPrimary == ssLine )
                {
                 alsLine = EVmbxAlsLine1;
                }
            }
        }
    ssSettings.Close();
    VMBLOGSTRING2( "VMBX: VmbxUtilities::AlsLine: alsLine=%I <=",
       alsLine );
    return alsLine;
    }

// ---------------------------------------------------------------------------
// CVmbxUiUtilities::VideoSupported
// 
// ---------------------------------------------------------------------------
//
TBool VmbxUtilities::VideoSupported()
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::VideoSupported: =>" );
    TBool result( EFalse );
    
    if ( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        CVmbxCenRepHandler* cenRepHandler( NULL );
        // TRAP_IGNORE for no leave function
        TRAP_IGNORE( cenRepHandler = CVmbxCenRepHandler::NewL() );
        if ( cenRepHandler )
            {
            result = cenRepHandler->VideoSupported();
            }
        delete cenRepHandler;
        cenRepHandler = NULL;
        }
    VMBLOGSTRING2( "VMBX: VmbxUtilities::VideoSupported: %d <=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::IsEmergencyNumber
// Verifies if the given number is an emergency number
// ---------------------------------------------------------------------------
//
TBool VmbxUtilities::IsEmergencyNumber( const TDesC& aNumber )
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::IsEmergencyNumber: =>" );
    TBool result( EFalse );
    CVmbxEmergencyCall* emergencyCall( NULL );
    // TRAP_IGNORE for no leave function
    TRAP_IGNORE( emergencyCall = CVmbxEmergencyCall::NewL() );
    if ( emergencyCall )
        {
        result = emergencyCall->IsEmergencyNumber( aNumber );
        }
    delete emergencyCall;
    emergencyCall = NULL;
    VMBLOGSTRING2( "VMBX: VmbxUtilities::IsEmergencyNumber: result%d <=", 
            result);
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxUiUtilities::VoIPSupported
// 
// ---------------------------------------------------------------------------
//
TBool VmbxUtilities::VoIPSupported()
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::VoIPSupported: <=>" );
    return FeatureManager::FeatureSupported( KFeatureIdCommonVoip );
    }

// -----------------------------------------------------------------------------
// VmbxUtilities::ValidateSSfix
// Skips over SS code if it exists.
// -----------------------------------------------------------------------------
//  
TBool VmbxUtilities::ValidateSsNum( TLex& aLexer )
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::ValidateSsNum: =>" );
    //The procedure always starts with *, #, **, ## or *# and is finished by #.
    // Each part within the procedure is separated by *.

    TBool result( EFalse );
    // Get and skip the first '#' or '*' separator
    TChar current = aLexer.Get();

    current = aLexer.Peek();

    if ( '#' == current || '*' == current )
        {
        aLexer.Inc(); // Skip the second '#' or '*' separator
        }
    // Define another string which aready skip the prefix '*' or
    // '#', the sring to judge the string whether end of  '#' and
    // valid
    TLex nextLexer( aLexer );
    TChar nextChar = nextLexer.Peek();
    TBool invalidCharFound( EFalse );
    while ( !nextLexer.Eos() && !invalidCharFound )
        {
        nextChar = nextLexer.Get();
        VMBLOGSTRING2( "VMBX: VmbxUtilities::ValidateTelNum:\
            nextChar %S", &nextChar );
        // Check the string valid or invalid for SS string
        if ( KErrNotFound == KAllowedSSChars().Locate( nextChar ) )
            {
            invalidCharFound = ETrue;
            }
        }

    // Check if the string end of '#' and check if it's validate ss code.
    if ( nextLexer.Eos() && ( '#' == nextChar ) && !invalidCharFound )
        {
        result = ETrue;
        if ( result && !aLexer.Eos() )
            {
            // It already skip prefix and 
            // Check SC(Service Code) length,it should be more than 2 digits
            result = ValidateTelNum( aLexer );
            }
        }
    VMBLOGSTRING2( "VMBX: VmbxUtilities::ValidateSsNum: result = %d<=",
        result );
    return result;
    }

// -----------------------------------------------------------------------------
// VmbxUtilities::ValidateTelNum
// Parses string until end or invalid tel number character is found.
// Check number length.
// -----------------------------------------------------------------------------
//  
TBool VmbxUtilities::ValidateTelNum( TLex& aLexer ) 
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::ValidateTelNum: =>" );
    TBool result( ETrue );

    // Skip country code prefix '+'
    if ( '+' == aLexer.Peek() )
        {
        aLexer.Inc(); // Skip country code separator
        }

    TInt telNumDigits( 0 );
    TBool invalidCharFound( EFalse );
    // Parse until invalid telnumber char found
    while ( !aLexer.Eos() && !invalidCharFound )
        {
        const TChar nextChar = aLexer.Peek();
        // Check validSS chars
        if ( KErrNotFound == KAllowedTelNumChars().Locate( nextChar ) )
            {
            // Invalid char found so string before it is the tel number part
            invalidCharFound = ETrue;
            }
        else
            {
            aLexer.Inc();
            telNumDigits++;
            }
        }

    VMBLOGSTRING2( "VMBX: VmbxUtilities::ValidateTelNum:\
        telNumDigits %d", telNumDigits );

    if ( KVmbxPhoneNumMinLength > telNumDigits 
        || KVmbxPhoneNumMaxLength < telNumDigits
        || invalidCharFound )
        {
        result = EFalse;
        }

    VMBLOGSTRING2( "VMBX: VmbxUtilities::ValidateTelNum:\
         result %d<=", result );
    return result;
    }

// -----------------------------------------------------------------------------
// VmbxUtilities::ValdateDtmfPart
// Parse string until end and validate for allowed characters.
// -----------------------------------------------------------------------------
//       
TBool VmbxUtilities::ValdateDtmfPart( TLex& aLexer )
    {
    VMBLOGSTRING( "VMBX: VmbxUtilities::ValdateDtmfPart: =>" );
    // DTMF string consists of following three parts:
    // the first part of the string is a phone number,
    // the second part of the string a DTMF special character (p, w or +),
    // the last third part is an actual DTMF tone string, which is sent to the
    // remote end.
    TBool result( ETrue );
    TLex lexer( aLexer );
    TInt telNumDigitsBeforeDtmf( 0 );
    TBool isDtmfNumberDiscover( EFalse );
    TBool invalidCharFound( EFalse );
    // check the second part of the string
    while ( !lexer.Eos() && !invalidCharFound )
        {
        const TChar nextChar = aLexer.Peek();
        // Check valid DTMF chars
        if ( KErrNotFound == KAllowedDtmfChars().Locate( lexer.Get() ) )
            {
            invalidCharFound = ETrue;
            }
        else 
            {
            // Check DTMF number discover or not
            if ( 'p' == nextChar || 'P'== nextChar 
                || 'w'== nextChar || 'w'== nextChar )
                {
                isDtmfNumberDiscover = ETrue;
                }
            // Telnumber count without '+' before 'p' or 'w'
            if ( '+' != aLexer.Peek() && !isDtmfNumberDiscover )
                { 
                telNumDigitsBeforeDtmf++;
                }
             aLexer.Inc();
            }
         VMBLOGSTRING2( "VMBX: VmbxUtilities::ValdateDtmfPart:\
         telNumDigitsBeforeDtmf %d", telNumDigitsBeforeDtmf );
        }
    
    if ( KVmbxPhoneNumMinLength > telNumDigitsBeforeDtmf 
        || KVmbxPhoneNumMaxLength < telNumDigitsBeforeDtmf 
        || invalidCharFound )
        {
        result = EFalse;
        }
    VMBLOGSTRING2( "VMBX: VmbxUtilities::ValdateDtmfPart: result = %d<=",
        result );
    return result;
    }

// End of file
