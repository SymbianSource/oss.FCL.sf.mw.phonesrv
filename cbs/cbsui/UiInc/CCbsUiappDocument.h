/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file for the CCbsUiappDocument class.
*
*
*/


#ifndef __CBSUIAPPDOCUMENT_H
#define __CBSUIAPPDOCUMENT_H

//  INCLUDES
#include <eikdoc.h>
  

// FORWARD DECLARATIONS
class MCbs;

// CLASS DECLARATION

/**
*  The Document class
*/
class CCbsUiappDocument : public CEikDocument
    {
    public:  // Constructors and destructor
        
        /**
        * Returns new instance of CbsUiDocument.
        *
        * @param aApp application
        * @return new instance of CbsUiDocument
        */
        static CCbsUiappDocument* NewL( CEikApplication& aApp );
     
        /**
        * Destructor.
        */
        ~CCbsUiappDocument();
        
    public: // From CEikDocument
        
        /**
        * Stop doc file being created
        * @param aDoOpen not used
        * @param aFilename not used 
        * @param aFs not used
        * @return NULL
        */
        inline CFileStore* OpenFileL( TBool /*aDoOpen*/, 
            const TDesC& /*aFilename*/,
            RFs& /*aFs*/);

        /**
        * Since 3.0
        * Stop doc file being created
        * @param aFileStore not used
        * @param aFile not used
        */
        inline void OpenFileL( CFileStore*& /*aFileStore*/, RFile& /*aFile*/ );

    private: // Constructors

        /**
        * C++ default constructor.
        *
        * @param aApp application
        */
        CCbsUiappDocument( CEikApplication& aApp );

        /**
        * Symbian OS constructor.
        */
        void ConstructL();
        
    private: // Functions from base classes

        /**
        * From CEikDocument
        * Creates and returns an instance of CBSUI application
        *
        * @return an instance of CBSUI application
        */
        CEikAppUi* CreateAppUiL();

    private: // Data

        // Own: For creating connection to the server
        MCbs* iServer;
    };


#include "CCbsUiappDocument.inl"

#endif      // __CBSUIAPPDOCUMENT_H

// End of File
