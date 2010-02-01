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
* Description:  This file contains the header file of the
*              : CSsBsHandler class.
*
*/

#ifndef C_SSBSHANDLER_H
#define C_SSBSHANDLER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <fbs.h>

// CONSTANTS
// None

// MACROS
// None

// DATA TYPES
// None

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
 *  CSsBsHandler declaration.
 *
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CSsBsHandler ) : public CBase
    {

public: // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CSsBsHandler* NewL();

    /**
     * Two-phased constructor.
     */
    static CSsBsHandler* NewLC();

    /**
     * Destructor.
     */
    virtual ~CSsBsHandler();

public: // New functions

    /**
     * Get branded icon
     * @param aBrandingId Brandind Id.
     * @param aBrandedBitmap Branded bitmap.
     * @param aBrandedBitmapMask Bitmap mask.
     */
    void GetBrandedIconL( const TDesC8& aBrandingId,
                          CFbsBitmap*& aBrandedBitmap,
                          CFbsBitmap*& aBrandedBitmapMask ) const;

private:

    /**
     * C++ default constructor.
     */
    CSsBsHandler();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    };

#endif // C_SSBSHANDLER_H

// End of file
