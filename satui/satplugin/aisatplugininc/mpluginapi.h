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
* Description:  Defines interface for the operator info engine user
*
*/


#ifndef MPLUGINAPI_H
#define MPLUGINAPI_H

// CLASS DECLARATION
/** 
* It defines interface for the plugin, contains virtual method which offers plugin 
* a way to notify the client e.g. P&S data changes
*
* @since S60 v3.2
*/
class MPluginAPI 
    {
    public: 

    /**
    * Virtual method, which the plugin class has to implement.
    * Method is used to send notification of P&S data's change event
    * from engine to plugin.
    *
    */
    inline virtual void NotifyContentUpdate() = 0;  
    };

#endif //MPLUGINAPI_H     
            
