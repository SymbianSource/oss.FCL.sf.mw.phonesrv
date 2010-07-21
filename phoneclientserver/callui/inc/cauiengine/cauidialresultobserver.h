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
* Description:  It defines the interface for the observers of 
*                the CCaUiEngApi DialL function results.
*
*/


#ifndef MCAUIDIALRESULTOBSERVER_H
#define MCAUIDIALRESULTOBSERVER_H

// CLASS DECLARATION

/**
*  Defines the interface for the observers of the CCaUiEngApi DialL function.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
class MCaUiDialResultObserver
    {          
    public:  // New functions   
        
        /**
        * It is called whenever dial request is completed.
        *
        * @param aStatus It is the status of the operation.
        *                KErrNone if successful.
        *                < 0 means Symbian OS error code.
        */       
        virtual void HandleDialResultL( const TInt aStatus ) = 0;
        
    };

#endif      // MCAUIDIALRESULTOBSERVER_H
            
// End of File
