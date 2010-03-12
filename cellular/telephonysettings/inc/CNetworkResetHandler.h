/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CNetworkResetHandler is a async activeobject which handles reseting network call.
*
*
*/


#ifndef     CNETWORKRESETHANDLER_H
#define     CNETWORKRESETHANDLER_H

//  INCLUDES
#include "PsetNetwork.h"
#include <rmmcustomapi.h>

// FORWARD DECLARATIONS
class CPsetNetwork;

/**
*  CNetworkResetHandler class is used to handle network reset search requests.´
*/
class CNetworkResetHandler : public CActive
    {
    
    public: // constructor & destructor

    	static CNetworkResetHandler* NewL( RMobilePhone& aPhone );

        /* Destructor */
        ~CNetworkResetHandler();
    	
        /* 
        * Calls ETEL to start reseting the network.
        * @return Error code, or KErrNone.
        */        
        TInt ResetNetworkSearch();
        
    private:
    	
    	CNetworkResetHandler( RMobilePhone& aPhone );
    	
    	void ConstructL();
    	
    	void RunL();
    
        void DoCancel();
        
        // Custom phone.       
        RMmCustomAPI iCustomApi;
        
        //Provides client access to mobile phone functionality provided by TSY.
        RMobilePhone& iPhone;
 
    };
#endif // CNETWORKRESETHANDLER_H

// end of file
