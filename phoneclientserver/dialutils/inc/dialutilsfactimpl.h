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
* Description:  Implementation of the DialUtils factory interface.
*
*/



#ifndef DIALUTILSFACTIMPL_H
#define DIALUTILSFACTIMPL_H


//  INCLUDES
#include    <dialutils.h> 


/**
*  Implementation of the DialUtils factory.
*
*  @lib DialUtils.lib
*  @since 2.6
*/
class CDialUtilsFactImpl 
:   public CDialUtilsFactory
    {
    public:

        /**
        * @see CDialUtilsFactory::CDialUtilsApiLD.
        */
       CDialUtilsApi* CDialUtilsApiLD();


    private:

        /* 
        * Destructor 
        */
        ~CDialUtilsFactImpl();
    };


#endif      // DIALUTILSFACTIMPL_H

  
// End of File
