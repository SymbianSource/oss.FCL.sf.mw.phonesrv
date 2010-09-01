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
* Description: 
*       Shows global confirmation note and instance of this class
*       has to be a member variable.
*
*
*/


#ifndef __CCbsUiConfirmationNote_h
#define __CCbsUiConfirmationNote_h

//  INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class CAknGlobalConfirmationQuery;

// CLASS DECLARATION

class CCbsUiConfirmationNote : public CBase
    {
    public: // Constructor and destructor

        /**
        * Constructor. Prohibited to make a instance.
        */
        CCbsUiConfirmationNote();

        /**
        * Destructor.
        */
        virtual ~CCbsUiConfirmationNote();

    public: // New functions

        /**
        * Shows global confirmation note, not enough memory...
        */
        void ShowGlobalConfirmationNoteL( CCoeEnv& aCoeEnv );

    private:

        // Own: Confirmation query
        CAknGlobalConfirmationQuery* iGlobalConfirmationQuery;

        // Forward declaration
        class CGlobalConfirmationQueryObserver;

        // Own: Confirmation query observer
        CGlobalConfirmationQueryObserver* iConfQueryObserver;

    };

#endif // __CCbsUiConfirmationNote_h

//  End of File  
