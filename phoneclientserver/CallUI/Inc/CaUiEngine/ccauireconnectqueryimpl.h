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
* Description:  Provides reconnectquery implementation for CallUI.
*
*/


#ifndef CCAUIRECONNECTQUERYIMPL_H
#define CCAUIRECONNECTQUERYIMPL_H

//  INCLUDES
#include <ccauireconnectquery.h>

#include    <e32std.h>


// FORWARD DECLARATIONS
class CCaUiEngine;

// CONSTANTS



   
// CLASS DECLARATION

/**
*  CaUIEngine implementation.
*  
*  @lib CaUiReconnectQueryImpl.lib
*  @since 5.0
*/
NONSHARABLE_CLASS( CCaUiReconnectQueryImpl )
:   public CCaUiReconnectQuery
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @return an instance of CallUI.
        */
        static CCaUiReconnectQueryImpl* NewL( 
            MCaUiReconnectQueryObserver& aObserver );
        

        /**
        * Destructor.
        */
        virtual ~CCaUiReconnectQueryImpl();



    private: // Functions from base classes
    
        /**
        * Launch reconnect confirmation query.
        * 
        */
        void ShowL( 
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption );
            
    private: // internal functions
        
        /**
        * Constructor.
        */
        CCaUiReconnectQueryImpl(MCaUiReconnectQueryObserver& aObserver);

        /**
        * Second phase construction.
        */
        void ConstructL();


    private:    // Data
        
        CCaUiEngine* iEng;

    };

#endif  // CCAUIRECONNECTQUERYIMPL_H

// End of file
