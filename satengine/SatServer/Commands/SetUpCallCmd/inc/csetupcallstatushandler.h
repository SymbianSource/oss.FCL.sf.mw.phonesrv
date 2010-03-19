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
* Description:  Minite the call creating status by ETelMM
*
*/



#ifndef CSETUPCALLSTATUSHANDLER_H
#define CSETUPCALLSTATUSHANDLER_H

#include <e32base.h>
#include "msatmultimodeapi.h"


class MSetupCallStatusObserver
    {
public:
    /**
     *  The call status update notification from ETel MM
     */
    virtual void CallSatatusChanged( const TInt status ) = 0;
    };

/**
*  This is the handler for the ETel MM api Request.
*  This active objects is registered with ETelMM Api to send request and 
*  receive notifications about some Request Complete.
*
*  @lib SetupCallCmd
*  @since S60 v5.0.1
*/

class CSetupCallStatusHandler : public CActive
    {

public:

    /**
     * Two-phased constructor.
     * @param aPhone A reference to the MSatMultiModeApi.
     * @param aDispatcher Pointer to Ss handler
     * @return a pointer to the newly created object.
     */
    static CSetupCallStatusHandler* NewL( MSatMultiModeApi& aPhone,
        MSetupCallStatusObserver* aDispatcher );

    /**
     * Destructor.
     */
    virtual ~CSetupCallStatusHandler();

    /**
     * Start to monite the call status by using ETel MM api
     */    
    void Start();
    /**
     * Cancel the asynchronous operations that required to the ETel MM api  
     */
    void CancelOperation();
 
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
    CSetupCallStatusHandler( MSatMultiModeApi& aPhone,
        MSetupCallStatusObserver* aDispatcher );
        
private: //new method
        
    void HandleConnectingStatusChange();
    
private: // Data

    /**
     * Reference to the MSatMultiModeApi
     */
    MSatMultiModeApi& iPhone;
        
    /**
     * Pointer to SendSs command handler
     */
    MSetupCallStatusObserver* iDispatcher;

    /**
     * Use this to get the RMobileCall status in 
     * 
     */    
    RMobileCall::TMobileCallStatus iMobileCallStatus;

    };

#endif      // CSETUPCALLSTATUSHANDLER_H
