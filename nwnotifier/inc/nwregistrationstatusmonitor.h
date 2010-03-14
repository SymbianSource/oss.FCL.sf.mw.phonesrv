/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: This file contains the header file of the NWRegistrationStatusMonitor class.
*
*/

#ifndef NWREGISTRATION_STATUS_H
#define NWREGISTRATION_STATUS_H

//  SYSTEM INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <etelmm.h>

// INCLUDES
#include "networknotifiernotewrapper.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  Monitors network registration status change event
*  from MMEtel
*/
class CNWRegistrationStatusMonitor : public CActive, 
                                            MNoteWrapperObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNWRegistrationStatusMonitor* NewL();

        /**
        * Destructor.
        */
        virtual ~CNWRegistrationStatusMonitor();
        
    
    protected:  // Functions from CActive

        /**
        * Cancels asyncronous request(s).
        * 
        */
        void DoCancel();

        /**
        * Informs object that asyncronous request is ready.
        * 
        */
        void RunL();
        
        /**
         * From CActive, RunError.
         * Processes any errors.
         * @param aError The leave code reported.
         * @result return KErrNone if leave is handled.
         */
         TInt RunError( TInt aError );

    private:
   
        /**
        * C++ default constructor.
        * 
        */
        CNWRegistrationStatusMonitor(); 
                     
        /**
        * Second-phase constructor.
        * 
        */
        void ConstructL();  
        
        /**
        * Issues a request to ETel to provide NW registration 
        * status information.
        * 
        */
        void IssueRequest();

        /**
        * Maps network registration status errors to resource ids.
        * 
        */
        void ParseErrCode();
        
        // from MNoteWrapperObserver
        void PopupNoteClosed();
       
        /**
        * Open the connection to RMobilePhone
        * 
        */
        void OpenConnectionL();
        
        /**
        * Closes the connection to RMobilePhone
        * 
        */
        void CloseConnection();
        
        
    private: // Data
        
        // Current registration status info.
        RMobilePhone::TMobilePhoneRegistrationStatus iRegistrationStatus;
        
        // Handle to RMobilePhone
        RMobilePhone iPhone;
  
        // Handle to Telephony server
        RTelServer iTelServer;
        
        // Parsed res id#
        TInt iResID;
        
        // Note wrapper
        CNetworkNotifierNoteWrapper* iNoteWrapper;
   };
#endif      // NWREGISTRATION_STATUS_H
            
// End of File

