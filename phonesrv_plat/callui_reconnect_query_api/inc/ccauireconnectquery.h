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
* Description:  Shows the reconnect query used in CallUI.
*
*/


#ifndef CCAUIRECONNECTQUERY_H
#define CCAUIRECONNECTQUERY_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MCaUiReconnectQueryObserver;


// CLASS DECLARATION
class CCaUiReconnectQuery : public CBase
    {
    public:  // Constructors and destructor
 
        /**
        * Create an instance that implements the interface.
        *
        * @param aObserver It is the observer for the instance.
        * @return Returns the created instance.
        */
        IMPORT_C static CCaUiReconnectQuery* NewL( 
            MCaUiReconnectQueryObserver& aObserver );
            
        /**
        * Destructor.
        * If note has been shown, this removes the note away
        * and frees the resources.
        */
        ~CCaUiReconnectQuery();


    public: // New functions    
        
        /**
        * Shows the query and destroys it after use.
        * 
        * @param aPhoneNumber Number to be used for query.
        * @param aIncludeVideoCallOption flag if video call option is included
        */
        virtual void ShowL(
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption ) = 0;

    protected:

        /**
        * C++ constructor.
        * @param aObserver observer for query result
        */
        CCaUiReconnectQuery( MCaUiReconnectQueryObserver& aObserver );


    protected: // Data
        
        /** Observer for query result. */
        MCaUiReconnectQueryObserver& iObserver;

    };

#endif      // CCAUIRECONNECTQUERY_H
            
// End of File
