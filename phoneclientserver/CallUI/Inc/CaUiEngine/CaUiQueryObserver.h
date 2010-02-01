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
* Description:  Defines the query observer interfaces used in CallUI 
*                confirmation query handling.
*
*/


#ifndef CAUIQUERYOBSERVER_H
#define CAUIQUERYOBSERVER_H


/**
*  Defines the observer interface for reconnect confirmation query observers.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
class MCaUiReconConfQueryObserver
    {
    public:    // Data types

        enum TCaUiReconType
            {
            ECallUiReconTypeCancel = -1, // Query canceled.
            ECallUiReconTypeVoice = 0,   // Voice call.
            ECallUiReconTypeVideo = 1,   // Video call.
#ifndef RD_UNIFIED_EDITOR
            ECallUiReconTypeMms   = 2,   // MMS.
            ECallUiReconTypeSms   = 3    // SMS.
#else // RD_UNIFIED_EDITOR
            ECallUiReconTypeMsg   = 2
#endif // RD_UNIFIED_EDITOR
            };

    public:  // New functions
        
        /**
        * Called when CCaUiReconConfQuery has got a key press.
        * 
        * @param aReconType The reconnect choice of the user.
        */
        virtual void HandleReconConfQueryResultL( 
            TCaUiReconType aReconType ) = 0;
    };



#endif      // CAUIQUERYOBSERVER_H 
            
// End of File
