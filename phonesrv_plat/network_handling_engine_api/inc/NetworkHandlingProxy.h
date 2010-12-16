/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the class NetworkHandlingProxy.
*
*/


#ifndef NETWORKHANDLINGPROXY_H
#define NETWORKHANDLINGPROXY_H

//  INCLUDES
#include "NWHandlingEngine.h"


// FORWARD DECLARATIONS
class CNWSession;
class MNWMessageObserver;

    
        /**
        *  Creates protocol specific Network handling.
        *  This constructor equals to using the second
        *  constructor with aReceiveHzData==false
        *
        *  @param aMessage Observer callback
        *  @param aTNWInfo Container for updated data
        *  @return networkHandling: Instance of the NetworkHandling
        */
        IMPORT_C CNWSession* CreateL( MNWMessageObserver& aMessage,
                                     TNWInfo& aTNWInfo );

        /**
        *  Creates protocol specific Network handling.
        *  This overload lets client choose whether HOMEZONE
        *  functionality is required or not.
        *
        *  Note: only one client is allowed to use HOMEZONE
        *  functionality (set aReceiveHzData true) at a time.
        *  If your client does not use HOMEZONE, please use
        *  the first constructor.
        *  
        *  @param aMessage Observer callback
        *  @param aTNWInfo Container for updated data
        *  @param aReceiveHzData Must be set ETrue just by one dedicated client!
        *  @return networkHandling: Instance of the NetworkHandling
        */
        IMPORT_C CNWSession* CreateL( MNWMessageObserver& aMessage,
                                     TNWInfo& aTNWInfo,
                                     TBool aReceiveHzData );


#endif      // NETWORKHANDLINGPROXY_H  
            
// End of File
