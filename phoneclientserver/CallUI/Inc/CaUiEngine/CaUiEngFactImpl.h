/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the CaUiEngine factory interface.
*
*/



#ifndef CAUIENGFACTIMPL_H
#define CAUIENGFACTIMPL_H


//  INCLUDES
#include    "CaUiEng.h"


/**
*  Implementation of the PhoneClient Extension factory.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
class CCaUiEngFactImpl 
:   public CCaUiEngFactory
    {
    public:

        /**
        * @see CCaUiEngFactory.
        */
        CCaUiEngApi* CCaUiEngApiLD();


    private:

        /* 
        * Destructor 
        */
        ~CCaUiEngFactImpl();
    };


#endif      // CAUIENGFACTIMPL_H

  
// End of File
