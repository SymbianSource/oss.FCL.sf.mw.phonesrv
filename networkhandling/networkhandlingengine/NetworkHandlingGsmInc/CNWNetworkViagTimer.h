/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CNWNetworkViagTimer class.
*
*/



#ifndef CNWNETWORKVIAGTIMER_H
#define CNWNETWORKVIAGTIMER_H

//  INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class MNWNetworkTimer;

// CLASS DECLARATION

/**
*  Utility class for timer 
*  
*
*  @lib euser.lib
*  @since Series 60 2.8
*/
class CNWNetworkViagTimer : public CTimer
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNWNetworkViagTimer* NewL(
            TInt aPriority = CActive::EPriorityStandard );
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkViagTimer();

    public: // New functions
        
        /**
        * Uses the mixin class to notify the timer expiration.
        *
        * @param    anInterval  Timer interval.
        * @param    aObserver   Observer to call after timer expiration.
        * @return
        */
        void After( const TTimeIntervalMicroSeconds32& anInterval, 
                    MNWNetworkTimer* aObserver );

    protected:  // Functions from CActive
        
        /**
        * Informs object that asyncronous request is ready.
        * 
        */
        virtual void RunL();

    private:

        /**
        * C++ default constructor.
        */
        CNWNetworkViagTimer( TInt aPriority );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        CNWNetworkViagTimer( const CNWNetworkViagTimer& );
        // Prohibit assigment operator if not deriving from CBase.
        CNWNetworkViagTimer& operator=( const CNWNetworkViagTimer& );

    private:    // Data
        // Pointer to the MNWNetworkTimer
        MNWNetworkTimer* iTimerObserver;
         
    };

#endif      // CNWNetworkViagTimer_H   
            
// End of File
