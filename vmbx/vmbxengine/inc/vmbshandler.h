/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Branding for voice mailbox
 *
*/

#ifndef C_VMBSHANDLER_H
#define C_VMBSHANDLER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
 *  CVmBSHandler declaration.
 *
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( CVmBSHandler ) : public CBase
    {

public: // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CVmBSHandler* NewL();

    /**
     * Two-phased constructor.
     */
    static CVmBSHandler* NewLC();

    /**
     * Destructor.
     */
    virtual ~CVmBSHandler();

public: // New functions

    /**
     * Get branded icon
     * @since S60 3.2
     * @param aBrandingId Brandind Id
     * @param aBrandedBitmap Bitmap for branded icon
     * @param aBrandedBitmapMask Mask to branded icon
     */
//     void GetBrandedIconL( const TDesC8& aBrandingId,
//                           CFbsBitmap*& aBrandedBitmap,
//                           CFbsBitmap*& aBrandedBitmapMask ) const;

private:

    /**
     * C++ default constructor.
     */
    CVmBSHandler();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    };

#endif // C_VMBSHANDLER_H

// End of file
