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
* Description:  Observer for reconnect query.
*
*/


#ifndef MCAUIRECONNECTQUERYOBSERVER_H   
#define MCAUIRECONNECTQUERYOBSERVER_H   

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Defines the interface for the observers of the CCaUiReconnectQuery.
*
*  @lib cauiengine.lib
*  @since 5.0
*/
class MCaUiReconnectQueryObserver
    {          
    public:  // New functions   
        
        enum TCaUiReconType
            {
            ECallUiReconTypeCancel = -1, // Query canceled.
            ECallUiReconTypeVoice = 0,   // Voice call.
            ECallUiReconTypeVideo = 1,   // Video call.
            ECallUiReconTypeMsg   = 2
            };
        
        /**
        * User selected option notification.
        *
        * @param aOption User selected option for reconnect query. 
        */       
        virtual void OptionSelected( TCaUiReconType aOption ) = 0;
        
    };

#endif      // MCAUIRECONNECTQUERYOBSERVER_H   
            
// End of File
