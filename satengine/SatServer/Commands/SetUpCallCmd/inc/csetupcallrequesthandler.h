/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Create the call and emergency call to ETelMM
*
*/


#ifndef CSETUPCALLREQUESTHANDLER_H
#define CSETUPCALLREQUESTHANDLER_H

#include <e32base.h>
#include "msatmultimodeapi.h"

#include "csetupcallstatushandler.h"

class CSetUpCallHandler;
class MSatAsyncToSync;

/**
*  This is the handler for the ETel MM api Request.
*  This active objects is registered with ETelMM Api to send request and 
*  receive notifications about some Request Complete.
*
*  @lib CallControlCmd
*  @since S60 v5.0.1
*/

class CSetupCallRequestHandler : public CActive,
                                 public MSetupCallStatusObserver
    {

public:

    /**
     * Two-phased constructor.
     * @param aPhone A reference to the MSatMultiModeApi.
     * @param aDispatcher Pointer to Ss handler
     * @return a pointer to the newly created object.
     */
    static CSetupCallRequestHandler* NewL( MSatMultiModeApi& aPhone,
        CSetUpCallHandler* aDispatcher );

    /**
     * Destructor.
     */
    virtual ~CSetupCallRequestHandler();

    /**
     * Access RMobileCall::DialNoFdnCheck by MSatMultiModeApi
     * for the paramter information please see the etelmm.h
     */
    void DialNumber( const TDesC8& aCallParams, const TDesC& aTelNumber,
            TBool aTerminateOtherCall, MSatAsyncToSync* aAsyncToSync );

    /**
     * Access RMobileCall::DialEmergencyCall by MSatMultiModeApi
     * for the paramter information please see the etelmm.h
     */
    void DialEmergencyCall( const TDesC& aTelNumber );    
    
    /**
     * Cancel the asynchronous operations that required to the ETel MM api  
     */
    void CancelOperation();
    
    /**
     *  From MSetupCallStatusObserver
     *  The call status update notification from ETel MM
     */
    void CallSatatusChanged( const TInt status );
 
protected:

    /**
     * From CActive, handles the request completion.
     */
    void RunL();
    
    
    /**
     * From CActive, handle the request cancel
     */
    void DoCancel();

private:

    /**
     * C++ default constructor.
     * @param aPriority An active object priority value.
     * @param aPhone A reference to MSatMultiModeApi.
     */
    CSetupCallRequestHandler( MSatMultiModeApi& aPhone,
        CSetUpCallHandler* aDispatcher );
        
    /**
     * Two phase contruction.
     */    
    void ConstructL();

private: //new method
    
    void HandleSetupCallStatusChange();
        
private: // Data

    /**
     * Reference to the MSatMultiModeApi
     */
    MSatMultiModeApi& iPhone;
        
    /**
     * Pointer to SendSs command handler
     */
    CSetUpCallHandler* iDispatcher;

    /**
     * Use to monite the status of call
     * Own
     */
    CSetupCallStatusHandler *iStatusHandler;
    
    /**
     * Current call is an emergency call.
     */
    TBool iEmergencyCall;
    
    /**
     * Terminal response to the SIM has been send
     */
    TBool iResponsed;
    
    };

#endif      // CSETUPCALLREQUESTHANDLER_H
