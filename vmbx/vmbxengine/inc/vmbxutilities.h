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
* Description:  Utility methods for universal usage
*  Interface   : Private, VmbxUtilities
*
*/


#ifndef VMBXUTILITIES_H
#define VMBXUTILITIES_H

// INCLUDES
#include <voicemailboxdefs.h>

// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
*  Implementation of the auxiliary methods for the VMBX engine classes
*
*  @lib vmbxengine.lib
*  @since S60 v5.2
*/
NONSHARABLE_CLASS(  VmbxUtilities )
    {

public:  // New functions

    /**
     * Verifies that the given string is a valid phone number.
     * Rules:
     * - '+' is only allowed as the first character
     * - The phone number part must be between 3-40 digits
     *
     * @since S60 v5.2
     * @param in aNumber Vmbx number
     * @return ETrue if valid number
     */
    static TBool IsValidPhoneNumber( const TDesC& aNumber );

    /**
    /**
     * Returns active ALS
     *
     * @since S60 v5.2
     * @return Active ALS line
     */
    static TVmbxAlsLineType AlsLine();

    /**
     * Returns state of the video support
     *
     * @since S60 v5.2
     * @return ETrue if Video supported
     */
    static TBool VideoSupported();

    /**
     * Returns state of the VoIP Support
     *
     * @since S60 v5.2
     * @return ETrue if VoIP Supported
     */
    static TBool VoIPSupported();

    /**
     * Verifies that the given number is not an emergency number.
     *
     * @since S60 v5.2
     * @param in aNumber Mailbox number
     * @return ETrue if emergency number
     */
    static TBool IsEmergencyNumber( const TDesC& aNumber );

private:

    /**
     * Verifies that the given string is a valid SS.
     *
     * @since S60 v5.2
     * @param in&out aLexer string to be validate
     * @return ETrue if supplementary service code validate
     */
    static TBool ValidateSsNum( TLex& aLexer );

    /**
     * Verifies that the given string is a valid Tel number.
     *
     * @since S60 v5.2
     * @param in aLexer string to be validate
     * @return ETrue if tel number validate
     */
    static TBool ValidateTelNum( TLex& aLexer );

    /**
     * Verifies that the given string is a valid DTMF number.
     *
     * @since S60 v5.2
     * @param in aLexer string to be validate
     * @return ETrue if DTMf validate
     */
    static TBool ValdateDtmfPart( TLex& aLexer );

    };

#endif  // VMBXUTILITIES_H
