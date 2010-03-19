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
*              : CSsIconFileProvider class.
*
*/

#ifndef C_SSICONFILEPROVIDER_H
#define C_SSICONFILEPROVIDER_H

#include <e32base.h>
#include <AknIconUtils.h> 

/**
 *  Declarition of CSsIconFileProvider.
 *
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CSsIconFileProvider ) : public CBase,
                                                     public MAknIconFileProvider
    {
    public:

        /**
         * Two-phased constructor.
         * @param aFile for handle to file
         */
        static CSsIconFileProvider* NewL( RFile& aFile );

        /**
         * Destructor.
         */
        virtual ~CSsIconFileProvider();

    public: // from base class MAknIconFileProvider

        /**
         * from MAknIconFileProvider.
         */
        void RetrieveIconFileHandleL( RFile& aFile, const TIconFileType aType );

        /**
         * from MAknIconFileProvider.
         */
        void Finished();

    private:

        /**
         * C++ default constructor.
         */
        CSsIconFileProvider();

         /**
          * By default Symbian 2nd phase constructor is private.
          */
        void ConstructL( RFile& aFile );

    private: // data

        // File handle. Own.
        RFile iFile;

    };

#endif // C_SSICONFILEPROVIDER_H

// End of File