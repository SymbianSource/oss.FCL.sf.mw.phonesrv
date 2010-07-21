/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides implementation for DialUtils.
*
*/


#ifndef DIALUTILSIMPL_H
#define DIALUTILSIMPL_H

//  INCLUDES
#include    <e32std.h>
#include    <dialutils.h> 


// FORWARD DECLARATIONS
class RSharedDataClient;
class CRepository;


// CONSTANTS

// The prefix text.
typedef TBuf<10> TDiUtPrefixText;


// CLASS DECLARATION

/**
*  DialUtils implementation.
*  
*  @lib DialUtils.lib
*  @since 2.6
*/
class CDialUtilsImpl
:   public CDialUtilsApi
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @return an instance of DialUtilsImpl.
        */
        static CDialUtilsImpl* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CDialUtilsImpl();


    public:  // New functions

        /**
        * Check and modify the phone number.
        * 
        * @see CDialUtilsApi::CheckNumber.
        */
        TInt CheckNumber( HBufC& aPhoneNumber );


   
    private:

        /**
        * C++ default constructor.
        */
        CDialUtilsImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Combine the given texts if needed.
        * 
        * @param aPhoneNumber The number to be modified.
        * @param aPrefixText The prefixText to be added 
        *                    to aPhoneNumber if needed.
        */
        TInt CombineTexts(
            TPtr aPhoneNumber,
            const TDiUtPrefixText& aPrefixText ) const;


    private:    // Data

        // The shared data client.
        RSharedDataClient*              iSharedDataClient;

        // The central repository.
        CRepository*                    iRepository;
    };

#endif  // DIALUTILSIMPL_H

// End of file
