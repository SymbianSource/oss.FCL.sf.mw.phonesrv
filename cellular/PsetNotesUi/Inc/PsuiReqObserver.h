/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of CPsuiReqObserver class.
*
*/


#ifndef CPSUIREQOBSERVER_H
#define CPSUIREQOBSERVER_H

// INCLUDES
#include <mservicerequestobserver.h>          
#include "PsuiNoteMaster.h"

// FORWARD DECLARATIONS
 
// CLASS DECLARATION

/**
*  CPsuiReqObserver class is responsible for observe request note cancel.
*  @since 3.2
*/
NONSHARABLE_CLASS ( CPsuiReqObserver ) : public CBase , public MServiceRequestObserver
    {
    public: // Constructors and destructor
                
        /**
        * Symbian OS two-phased constructor.
        * 
        */
        static CPsuiReqObserver* NewL( CPsuiNoteMaster& aNoteMaster );
        
        /**
        * Destructor 
        */
        ~CPsuiReqObserver();
    
    private:
        CPsuiReqObserver( CPsuiNoteMaster& aNoteMaster );
    
    public: //new
          
    protected:        
        /**
        * From MServiceRequestObserver. Called when service request
        * is cancelled by the user.
        */
        void MServiceRequestNoteCancelledL( TBool aCancelledWithEscape );
        
    private: //new
           
    private: // Member variables
    
        CPsuiNoteMaster& iNoteMaster;
        
        };       
#endif    //CPSUIREQOBSERVER_H
