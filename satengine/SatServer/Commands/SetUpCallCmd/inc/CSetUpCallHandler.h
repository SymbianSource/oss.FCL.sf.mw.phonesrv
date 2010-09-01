/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SetUpCall command handler
*
*/


#ifndef CSETUPCALLHANDLER_H
#define CSETUPCALLHANDLER_H

#include <etelsat.h>

#include <AiwServiceHandler.h>
#include <CPhCltEmergencyCall.h>
#include <MPhCltEmergencyCallObserver.h>

#include "CSatCommandHandler.h"
#include "SatSTypes.h"

/**
*  Command handler for SetUpCall command.
*
*  @lib SetUpCallCmd.lib
*  @since S60 v3.0
*/
class CSetUpCallHandler : public CSatCommandHandler,
                          public MAiwNotifyCallback,
                          public MPhCltEmergencyCallObserver
    {

public:

    static CSetUpCallHandler* NewL( MSatUtils* aUtils );

    virtual ~CSetUpCallHandler();

// from base class MSatEventObserver

    /**
     * From MSatEventObserver.?
     * Event notification.
     *
     * @param aEvent Identifies the event.
     */
    void Event( TInt aEvent );

// from base class MSatCommand

    /**
     * From MSatCommand.
     * Response from the client
     *
     */
    void ClientResponse();

protected:

// from base class CActive

    /**
     * From CActive
     * Cancels the usat request.
     *
     */
    void DoCancel();

// from base class CSatCommandHandler

    /**
     * From CSatCommandHandler
     * Requests the command notification.
     *
     * @param aStatus Request status
     */
    void IssueUSATRequest( TRequestStatus& aStatus );

    /**
     * From CSatCommandHandler
     * Precheck before executing the command.
     *
     * @return TBool indicating command is currently allowed.
     */
    TBool CommandAllowed();

    /**
     * From CSatCommandHandler
     * Need for ui session.
     *
     * @return TBool indicating need of ui session.
     */
    TBool NeedUiSession();

    /**
     * From CSatCommandHandler
     * Called when USAT API notifies that command.
     *
     */
    void HandleCommand();

    /**
     * From CSatCommandHandler
     * Indication that UI launching failed.
     *
     */
    void UiLaunchFailed();

// from base class MAiwNotifyCallback

    /**
     * From MAiwNotifyCallback
     * Called when dial request is completed.
     *
     * @param aCmdId Identifier of requested Aiw operation.
     * @param aEventId Identifier of status event.
     * @param aEventParamList Parameters of status event.
     * @param aInParamList Parameters of Aiw operation.
     */
    TInt HandleNotifyL(
        const TInt aCmdId,
        const TInt aEventId,
        CAiwGenericParamList& aEventParamList,
        const CAiwGenericParamList& aInParamList );

// from base class MPhCltEmergencyCallObserver

    /**
     * From MPhCltEmergencyCallObserver
     * Called when emergency dial request is completed.
     *
     * @param aStatus Non zero value means failure.
     */
    void HandleEmergencyDialL( const TInt aStatus );

private:

    CSetUpCallHandler();

    void ConstructL();

    /**
     * Performs the request to dial
     *
     * @since S60 3.2
     */
    void DoSetupCallL();

    /**
     * Return terminal response filled according to dial result.
     *
     * @since S60 3.2
     * @param aStatus Dial status from phone.
     */
    void CompleteSetupCallWithStatus(
        const TInt32 aStatus );

    /**
     * Return terminal response filled according to result.
     *
     * @since S60 3.2
     * @param aGeneralResult Completion status.
     */
    void CompleteSetupCall(
        const RSat::TPCmdResult aGeneralResult );

    /**
     * Return terminal response filled according to result.
     *
     * @since S60 3.2
     * @param aGeneralResult Completion status.
     * @param aAdditionalInfo Completion status additional info.
     */
    void CompleteSetupCall(
        const RSat::TPCmdResult aGeneralResult,
        const TInt16 aAdditionalInfo );

    /**
     * Checks the dialling number string for extended BCD
     * values. Changes them to correct characters
     * see ETSI 11.11 10.5.1
     *
     * @param aNumber dialling number string
     */
    void CheckNumber( TDes& aNumber ) const;
    
    /**
     * Converts a TCCP error to the corresponding symbian error.
     *
     * @param aTccpError A TCCP error number to be converted into
     * a symbian one.
     *
     * @return The corresponding symbian error from TCCP error.
     */
    TInt TccpErrorToSymbianError( const TInt aTccpError ) const;
    
    /**
     * Create emergency call
     */
    void CreateEmergencyCallL();
    
private: // data

    /**
     * SetUpCall command data from SIM.
     */
    RSat::TSetUpCallV1 iSetUpCallData;

    /**
     * SetUpCall command package.
     */
    RSat::TSetUpCallV1Pckg iSetUpCallPckg;

    /**
     * SetUpCall response data from client.
     */
    RSat::TSetUpCallRspV2 iSetUpCallRsp;

    /**
     * SetUpCall response package
     */
    RSat::TSetUpCallRspV2Pckg iSetUpCallRspPckg;

    /**
     * Query command data.
     */
    TSatQueryV1 iQueryData;

    /**
     * Query package.
     */
    TSatQueryV1Pckg iQueryPckg;

    /**
     * Query response.
     */
    TSatQueryRspV1 iQueryRsp;

    /**
     * Query rsp package.
     */
    TSatQueryRspV1Pckg iQueryRspPckg;

    /**
     * Service handler to make normal call.
     */
    CAiwServiceHandler* iServiceHandler;

    /**
     * Utility to handle emergency calls.
     */
    CPhCltEmergencyCall* iEmergencyCallApi;

    /**
     * Current call is an emergency call.
     */
    TBool iEmergencyCall;

    /**
     * Indicates if call control is active
     */
    TBool iCallControlActive;

    /**
     * Dial completion status
     */
    TInt iSetUpCallStatus;

    /**
     * wait scheduler
     */
    CActiveSchedulerWait iWait;

    /**
     * Boolean indicating is command pending for CallControl completion
     */
    TBool iPCmdPending;

    /**
     * Flag to signal that command has icon data
     * To be removed when icons are allowed in this command
     */
    TBool iIconCommand;

    };

#endif      // CSETUPCALLHANDLER_H
