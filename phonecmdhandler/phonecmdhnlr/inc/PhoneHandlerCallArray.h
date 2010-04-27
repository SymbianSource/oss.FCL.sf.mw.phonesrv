/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declarations for class CPhoneHandlerCallArray
*
*/


#ifndef CPHONEHANDLERCALLARRAY_H
#define CPHONEHANDLERCALLARRAY_H

#include <e32base.h>

class TCallInfo;
class MCall;

/**
* Handles call adding 
*
*  @lib phonecmdhandler.dll
*/
class CPhoneHandlerCallArray: public CBase
    {
    
    public: //Constructors and descructor    
    
        /**
        * Two-phased constructing for the array.
        *
        */
        static CPhoneHandlerCallArray* NewL( );

        /**
        * C++ default destructor
        */
        ~CPhoneHandlerCallArray( );

        /**
         * Add call to array
         * @param aCall call to be added
         * @return result of appending
         */        
        void AddL( const MCall* aCall );
        
        /**
         * Remove call from array
         * @param aCall call to be removed
         * @return result of removal
         */
        TInt Remove( TCallInfo* aCall );
        
        /**
         * Get call count.
         */
        TInt CallCount( );
                
        /**
		 * Get call by state.
		 */
        TCallInfo* CallByState( TInt state );
        
        /**
        * Finds call pointer specified by call index.
        * @param aCallIndex
        * @return corresponding call pointer or NULL if not found.
        */
        TCallInfo* FindCall( TInt aCallIndex );
    
        /**
		* Get connected call count.
		*/
		TInt ConnectedCallCount( );
		
		/**
		* Get existing call count.
		*/
		TInt ExistingCallCount();
		              
    private:
        /**
        * C++ default constructor
        */
        CPhoneHandlerCallArray( );
                                          
    private: // data
        
        /**
        * Pointer array of call objects.     
        */
    	RPointerArray<TCallInfo> iCallArray;
        
    };

#endif // CPHONEHANDLERCALLARRAY_H
