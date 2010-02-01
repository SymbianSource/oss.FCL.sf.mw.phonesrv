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
* Description:  Observer interface for service request
*
*/



#ifndef MSERVICEREQUESTOBSERVER_H
#define MSERVICEREQUESTOBSERVER_H

// CLASS DECLARATION

/**
*  Observer interface for service request
*
*  @lib ServiceRequest.lib
*  @since 1.2
*/
class MServiceRequestObserver
    {
    public: // New functions
        
        /**
        * Called when user cancel the service request.
        * @since 1.2
        * @param aCancelledWithEscape ETrue: cancalled with esc key. 
        *                             EFalse: cancelled with softkey (normal).
        */
        virtual void MServiceRequestNoteCancelledL( 
            TBool aCancelledWithEscape ) = 0;
    };

#endif      // MSERVICEREQUESTOBSERVER_H   
            
// End of File
