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
* Description:  Defines the query used in CallUI to ask call type from user.
*
*/


#ifndef CAUIQUERY_H
#define CAUIQUERY_H


//  INCLUDES
#include <aknlistquerydialog.h> 



// CLASS DECLARATION

/**
*  Defines create list query that is used in CaUiEngine to ask 
*  call type confirmation from user.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
NONSHARABLE_CLASS( CCaUiCreateListQuery )
    : public CAknListQueryDialog
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        * 
        * @param aIndex After the query is dismissed, the index will hold
        *               the value of selected item.
        */
        CCaUiCreateListQuery( TInt* aIndex );
        
        /**
        * Destructor.
        */
        ~CCaUiCreateListQuery();


    public: // Functions from base classes

        /**
        * Loaded version to enable Send key handling.
        * 
        * @see CAknListQueryDialog::OfferKeyEventL.
        */
        TKeyResponse OfferKeyEventL(
            const TKeyEvent& aKeyEvent,
            TEventCode aType );
            
    private: //data

        /**
         * Flag indicating the validity of the received key event.
         */
        TBool iValidKeyEvent;
    };

#endif      // CAUIQUERY_H 
            
// End of File
