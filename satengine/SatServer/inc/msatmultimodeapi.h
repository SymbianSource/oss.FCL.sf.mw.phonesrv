/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  interface that access to etelmm API in sat
*
*/


#ifndef MSATMULTIMODEAPI_H
#define MSATMULTIMODEAPI_H

#include <e32base.h>
#include <etel.h>
#include <etelmm.h>
#include <rmmcustomapi.h>


/**
 *  Encapsule the access to etelmm API in sat
 *
 *  @lib satengine.lib
 *  @since S60 v5.0
 */
class MSatMultiModeApi
    {

public:
    
    /**
     * Gives reference to active RMobilePhone session
     *
     * @return Reference to active RMobilePhone server session
     */
    virtual RMobilePhone* Phone() = 0;

    /**
     * Gives reference to active Dummy RMobilePhone session
     *
     * @return Reference to active Dummy RMobilePhone server session
     */
    virtual RMobilePhone* DummyPhone() = 0;

    /**
     * Gives reference to active RMmCustomApi session
     *
     * @return Reference to active RMmCustomApi server session
     */
    virtual RMmCustomAPI* CustomApi() = 0;

    /**
     * Lowers error granularity to basic errors
     */
    virtual void LowerErrorGranularity() = 0;

    /**
     * Raises error granularity to extended errors
     */
    virtual void RaiseErrorGranularity() = 0;
    
    /**
     * Access RMobilePhone::GetNetworkRegistrationStatus
     * for the paramter information please see the etelmm.h
     */
    virtual void GetNetworkRegistrationStatus( TRequestStatus& aReqStatus, 
                RMobilePhone::TMobilePhoneRegistrationStatus& aStatus ) = 0;
    
    /**
     * Access RMobilePhone::NotifyNetworkRegistrationStatusChange
     * for the paramter information please see the etelmm.h
     */ 
    virtual void NotifyNetworkRegistrationStatusChange(
                TRequestStatus& aReqStatus,
                RMobilePhone::TMobilePhoneRegistrationStatus& aStatus) = 0;
                
    /**
     * Access RMobilePhone::GetSubscriberId
     * for the paramter information please see the etelmm.h
     */ 
    virtual void GetSubscriberId( TRequestStatus& aReqStatus, 
                    RMobilePhone::TMobilePhoneSubscriberId& aId ) = 0;              

    /**
     * Access RMobilePhone::SendDTMFTones
     * for the paramter information please see the etelmm.h
     */ 
    virtual void SendDTMFTones(TRequestStatus& aReqStatus, 
                    const TDesC& aTones) = 0;    
    /**
     * Access RMobilePhone::CancelAsyncRequest
     * for the paramter information please see the etelmm.h
     */
    virtual void CancelAsyncRequest(TInt aReqToCancel) = 0;
    
    
    /**
     * Access RMobilePhone::SendNetworkServiceRequestNoFdnCheck
     * for the paramter information please see the etelmm.h
     */
    virtual void SendNetworkServiceRequestNoFdnCheck( 
            TRequestStatus& aReqStatus, const TDesC& aServiceString) = 0;
            
    /**
     * Access RMobileCall::DialNoFdnCheck
     * for the paramter information please see the etelmm.h
     */
    virtual void DialNoFdnCheck(TRequestStatus& aStatus,
                const TDesC8& aCallParams,const TDesC& aTelNumber) = 0;
                                            
    /**
     * Access RMobileUssdMessaging::SendNetworkServiceRequestNoFdnCheck
     * for the paramter information please see the etelmm.h
     */
    virtual void SendMessageNoFdnCheck( TRequestStatus& aReqStatus,
                const TDesC8& aMsgData, const TDesC8& aMsgAttributes ) = 0 ;
    /**
     * Access RMobileCall::DialCancel
     */
    virtual void DialCancel() = 0;
    /**
     * Access RMobilePhone::GetCurrentActiveUSimApplication
     * for the paramter information please see the etelmm.h
     */
    virtual void GetCurrentActiveUSimApplication( TRequestStatus& aReqStatus, 
                RMobilePhone::TAID& aAID ) = 0;

    /**
     * Check if there is an incoming call 
     * @return ETrue if there is an incoming call
     */
    virtual TBool IsCallIncoming() = 0;
    };

#endif // MSATMULTIMODEAPI_H
