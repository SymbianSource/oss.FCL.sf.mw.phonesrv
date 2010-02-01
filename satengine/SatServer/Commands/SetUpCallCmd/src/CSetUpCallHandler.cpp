/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles SetUpCall command
*
*/


#include    <aiwinternaldialdata.h>
#include    <aiwdialdataext.h>

#include    <exterror.h>
#include    <ccpdefs.h>

#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatSystemState.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CSetUpCallHandler.h"
#include    "TSatExtErrorUtils.h"
#include    "SatLog.h"

const TUint8 KBCDAsterisk( 0x0A );
const TUint8 KBCDDash( 0x0B );
const TUint8 KBCDDTMF( 0x0C );
const TUint8 KBCDWild( 0x0D );
const TUint8 KBCDExpansion( 0x0E );

const TUint8 KAsteriskChar( 0x2A );
const TUint8 KDashChar( 0x23 );
const TUint8 KDTMFChar( 0x70 );
const TUint8 KWildChar( 0x77 );
const TUint8 KExpansionChar( 0x2E );

_LIT( KFixedSimEmergencyNumber, "112" );
_LIT8( KContentType, "*" );
// 3GPP TS 24.008
const TInt KMaximumPhoneNumberLength( 80 );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetUpCallHandler* CSetUpCallHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::NewL calling" )

    CSetUpCallHandler* self = new( ELeave ) CSetUpCallHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CSetUpCallHandler::~CSetUpCallHandler()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::~CSetUpCallHandler calling" )

    Cancel();

    if ( iServiceHandler )
        {
        delete iServiceHandler;
        iServiceHandler = NULL;
        }

    if ( iEmergencyCallApi )
        {
        delete iEmergencyCallApi;
        iEmergencyCallApi = NULL;
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::~CSetUpCallHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatEventObserver.
// Event notification.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::Event( TInt aEvent )
    {
    LOG2( SIMPLE, "SETUPCALL: CSetUpCallHandler::Event calling,aEvent: %i",
          aEvent )

    if ( MSatUtils::ECallControlExecuting == aEvent )
        {
        LOG( SIMPLE,
            "SETUPCALL: CSetUpCallHandler::Event: ECallControlExecuting" )
        iCallControlActive = ETrue;
        }
    else if ( MSatUtils::ECallControlDone == aEvent )
        {
        LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::Event: ECallControlDone" )
        iCallControlActive = EFalse;

        // Check if SetUpCall command has arrived from SIM during CC execution
        // Emergency call is made immediate regardless of call control.
        if ( !IsActive() && iPCmdPending && !iEmergencyCall )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::Event: setupcall" )
            iPCmdPending = EFalse;
            // Execute the setupcall.
            DoHandleCommand();
            }
        }

    CSatCommandHandler::Event( aEvent );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::ClientResponse()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::ClientResponse calling" )

    if ( iQueryRsp.iAccepted )
        {
        // User accepted the call, make the call
        TRAPD( error, DoSetupCallL() );

        if ( KErrNone != error )
            {
            LOG2( NORMAL, 
            "SETUPCALL: CSetUpCallHandler::ClientResponse Dial failed: %i", 
            error )

            CompleteSetupCall( RSat::KCmdDataNotUnderstood );
            }
        }
    else
        {
        LOG( NORMAL, 
        "SETUPCALL: CSetUpCallHandler::ClientResponse User Cancel" )

        if ( iQueryRsp.iSessionTerminatedByUser )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::ClientResponse TerminatedByUser" )
            // Notify sim session end command that next sim session end
            // should close the ui session.
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            }

        // End key is pressed during confirmation or user denied call setup,
        // KPCmdNotAcceptedByUser is an expected response,
        CompleteSetupCall( RSat::KPCmdNotAcceptedByUser );
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::DoCancel()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySetUpCallCancel();

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iSetUpCallData) RSat::TSetUpCallV1();
    iPCmdPending = EFalse;
    iQueryRsp.iAccepted = EFalse;

    iUtils->USatAPI().NotifySetUpCall( aStatus, iSetUpCallPckg );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// SetUpCall command is not allowed in following situations:
//      - Phone is not registered to homenetwork and roaming.
//      - phone is ringing, alerting or call is on and SetUpCall command
//        is "make call only if not busy", "make call only if not busy wiht
//        redial" or "Call type not set".
// -----------------------------------------------------------------------------
//
TBool CSetUpCallHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CommandAllowed calling" )
    TBool commandAllowed( ETrue );

    // Emergency call can newer be denied.
    // The usage of constant emergency number is according to ETSI TS 31.111.
    iEmergencyCall =
        ( 0 == iSetUpCallData.iAddress.iTelNumber.Compare(
            KFixedSimEmergencyNumber ) );

    if ( !iEmergencyCall )
        {
        RMobilePhone::TMobilePhoneRegistrationStatus networkStatus(
            iUtils->SystemState().GetNetworkRegistrationStatus() );

        if ( ( ( RSat::EAlphaIdProvided !=
                     iSetUpCallData.iAlphaIdConfirmationPhase.iStatus ) &&
               ( ( RSat::ESelfExplanatory ==
                       iSetUpCallData.iIconIdConfirmationPhase.iQualifier ) ||
                 ( RSat::ENotSelfExplanatory ==
                       iSetUpCallData.iIconIdConfirmationPhase.iQualifier ) ) )
                 ||
             ( ( RSat::EAlphaIdProvided !=
                     iSetUpCallData.iAlphaIdCallSetUpPhase.iStatus ) &&
               ( ( RSat::ESelfExplanatory ==
                       iSetUpCallData.iIconIdCallSetUpPhase.iQualifier ) ||
                 ( RSat::ENotSelfExplanatory ==
                       iSetUpCallData.iIconIdCallSetUpPhase.iQualifier ) ) ) )
            {
            // Icon data is received without alpha id.
            CompleteSetupCall( RSat::KCmdDataNotUnderstood );

            commandAllowed = EFalse;
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CommandAllowed Icon received \
            without alpha id" )
            }
        else if ( ( RMobilePhone::ERegisteredOnHomeNetwork != networkStatus ) &&
                  ( RMobilePhone::ERegisteredRoaming != networkStatus ) )
            {
            // Not registered to network.
            CompleteSetupCall( RSat::KMeUnableToProcessCmd, RSat::KNoService );

            commandAllowed = EFalse;
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CommandAllowed Registration not \
            valid" )
            }
        else
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CommandAllowed others" )
            // Call type
            const RSat::TSetUpCallType callType( iSetUpCallData.iType );

            // Command does not allow to make a call if busy
            const TBool dontMakeCallIfBusy(
                ( RSat::EOnlyIfNotBusy == callType ) ||
                ( RSat::EOnlyIfNotBusyWithRedial == callType ) ||
                ( RSat::ESetUpCallTypeNotSet == callType ) );

            if ( dontMakeCallIfBusy )
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CommandAllowed \
                dontMakeCallIfBusy true" )
                // Is the call ongoing
                const TBool callIsOngoing(
                    iUtils->SystemState().IsCallActive() );

                // Is the call incoming
                const TBool callIsIncoming(
                    iUtils->SystemState().IsCallIncoming() );

                if ( callIsOngoing || callIsIncoming )
                    {
                    // Set the terminal response info.
                    CompleteSetupCall(
                        RSat::KMeUnableToProcessCmd,
                        RSat::KMeBusyOnCall );

                    commandAllowed = EFalse;
                    LOG( SIMPLE, 
                    "SETUPCALL: CSetUpCallHandler::CommandAllowed Busy" )
                    }
                }
            }

        if ( commandAllowed )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CommandAllowed Allowed to call" )

            // Set icon command flag whether icon data was received and
            // set qualifier to no icon id
            // To be removed when icons are allowed in this command
            if ( ( RSat::ESelfExplanatory ==
                     iSetUpCallData.iIconIdConfirmationPhase.iQualifier ) ||
                 ( RSat::ENotSelfExplanatory ==
                     iSetUpCallData.iIconIdConfirmationPhase.iQualifier ) ||
                 ( RSat::ESelfExplanatory ==
                     iSetUpCallData.iIconIdCallSetUpPhase.iQualifier ) ||
                 ( RSat::ENotSelfExplanatory ==
                     iSetUpCallData.iIconIdCallSetUpPhase.iQualifier ) )
                {
                LOG( SIMPLE, "SETUPCALL:   ENoIconId" )
                iIconCommand = ETrue;
                iSetUpCallData.iIconIdConfirmationPhase.iQualifier =
                    RSat::ENoIconId;
                iSetUpCallData.iIconIdCallSetUpPhase.iQualifier =
                    RSat::ENoIconId;
                }
            else
                {
                iIconCommand = EFalse;
                }
            }
        }
    else
        {
        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::CommandAllowed Emergency call" )
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CommandAllowed exiting" )
    
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CSetUpCallHandler::NeedUiSession()
    {
    LOG( NORMAL, "SETUPCALL: CSetUpCallHandler::NeedUiSession calling" )

    // Notify Cover UI if it's supported
    if ( iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, 
        "SETUPCALL: CSetUpCallHandler::NeedUiSession CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ESetUpCall;
        medEventData.iAlphaId = iSetUpCallData.iAlphaIdCallSetUpPhase;
        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
        medEventData.iIconID = iSetUpCallData.iIconIdCallSetUpPhase;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG( NORMAL, "SETUPCALL: CSetUpCallHandler::NeedUiSession exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::HandleCommand()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::HandleCommand calling" )

    if ( iEmergencyCall )
        {
        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::HandleCommand iEmergencyCall true" )
        TRAPD( err, CreateEmergencyCallL() );
        if ( KErrNone != err )
            {
            LOG2( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand DialEmergencyCallL \
            failed: %d", err )

            iEmergencyCall = EFalse;

            // Set the terminal response info.
            CompleteSetupCall(
                RSat::KMeUnableToProcessCmd,
                RSat::KNoSpecificMeProblem );
            }
        }
    else
        {
        const RSat::TAlphaIdStatus alphaIdStatus(
            iSetUpCallData.iAlphaIdCallSetUpPhase.iStatus );

        // If the alpha id is null, then use SAT name as a alpha identifier.
        if ( ( RSat::EAlphaIdNull == alphaIdStatus )  ||
             ( RSat::EAlphaIdNotPresent == alphaIdStatus ) )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand set AlphaId" )
            iSetUpCallData.iAlphaIdCallSetUpPhase.iAlphaId =
                iUtils->SatAppName();
            iSetUpCallData.iAlphaIdCallSetUpPhase.iStatus =
                RSat::EAlphaIdProvided;
            }

        // Copy the data to package, which is sent to client.
        iQueryData.iCommand = ESatSSetUpCallQuery;
        iQueryData.iQueryText.Copy(
            iSetUpCallData.iAlphaIdConfirmationPhase.iAlphaId );
        iQueryData.iSimApplicationName.Copy( iUtils->SatAppName() );
        iQueryData.iAlphaIdStatus = ESatAlphaIdNotNull; // Always
        iQueryData.iIconId.iIdentifier =
            iSetUpCallData.iIconIdConfirmationPhase.iIdentifier;

        LOG2( SIMPLE, 
        "SETUPCALL: iSetUpCallData.iIconIdConfirmationPhase.iQualifier: %d",
        iSetUpCallData.iIconIdConfirmationPhase.iQualifier )
        switch ( iSetUpCallData.iIconIdConfirmationPhase.iQualifier )
            {
            case RSat::ESelfExplanatory:
                {
                // Icon qualifier is self explanatory (to display instead
                // of the alpha id or text string).
                iQueryData.iIconId.iIconQualifier = ESatSelfExplanatory;
                break;
                }

            case RSat::ENotSelfExplanatory:
                {
                // Icon qualifier is not self explanatory.
                iQueryData.iIconId.iIconQualifier = ESatNotSelfExplanatory;
                break;
                }

            default:
                {
                // Icon qualifier not present
                iQueryData.iIconId.iIconQualifier = ESatENoIconId;
                break;
                }
            }

        // If call control is active, set up call is made
        // after the call control note is showed in ui ie
        // ECallControlDone event is notified.
        if ( !iCallControlActive )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand iCallControlActive \
            false" )
            iUtils->NotifyEvent( MSatUtils::ESetUpCallExecuting );

            TRAPD( regErr, iUtils->RegisterServiceRequestL(
                ESatSProactiveQuery,
                ESatSProactiveQueryResponse,
                this ) );
            LOG2( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand regErr: %d", regErr )
            if ( KErrNone != regErr )
                {
                // Possible memory allocation error. Send error terminal
                // response
                UiLaunchFailed();
                }
            else
                {
                // Send query to UI
                iUtils->SatUiHandler().UiSession()->SendCommand(
                    &iQueryPckg,
                    &iQueryRspPckg,
                    ESatSProactiveQuery );
                }
            }
        else
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand iCallControlActive \
            true" )
            // Set pending flag on
            iPCmdPending = ETrue;
            }
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indication that UI lanching failed.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::UiLaunchFailed calling" )

    CompleteSetupCall( RSat::KMeUnableToProcessCmd, 
                       RSat::KNoSpecificMeProblem );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// From class MAiwNotifyCallback.
// Called when dial request is completed.
// -----------------------------------------------------------------------------
//
TInt CSetUpCallHandler::HandleNotifyL(
    const TInt aCmdId,
    const TInt aEventId,
    CAiwGenericParamList& aEventParamList,
    const CAiwGenericParamList& /*aInParamList*/ )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::HandleNotifyL calling" )

    if ( KAiwCmdCall == aCmdId )
        {
        LOG2( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::HandleNotifyL  event: %d", aEventId )
        switch ( aEventId )
            {
            case KAiwEventError:
            case KAiwEventCompleted:
            case EGenericParamError: // This is due CCaUiPlugin behaviour.
                                     // Also overlaps event KAiwEventStarted.
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::HandleNotifyL SetupCall \
                Completed" )

                // Fetch completion status.
                TInt index( 0 );
                const TAiwGenericParam* param = aEventParamList.FindFirst(
                    index, EGenericParamError );

                if ( param )
                    {
                    LOG( SIMPLE, 
                    "SETUPCALL: CSetUpCallHandler::HandleNotifyL param true" )
                    
                    // Converts error number to symbian-formatted before 
                    // calling CompleteSetupCallWithStatus.
                    TInt error = TccpErrorToSymbianError( 
                        param->Value().AsTInt32() );
                    CompleteSetupCallWithStatus( error );
                    }
                else
                    {
                    CompleteSetupCall( RSat::KSuccess );
                    }
                }
                break;

            case KAiwEventCanceled:
            case KAiwEventStopped:
            case KAiwEventQueryExit:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::HandleNotifyL SetupCall \
                cancelled" )

                CompleteSetupCallWithStatus( KErrAbort );
                }
                break;

            case KAiwEventOutParamCheck:
            case KAiwEventInParamCheck:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::HandleNotifyL SetupCall param \
                fail" )

                CompleteSetupCall( RSat::KCmdDataNotUnderstood );
                }
                break;

            default:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::HandleNotifyL Unknown event \
                id" )
                }
                break;

            }
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::HandleNotifyL exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// From class MAiwNotifyCallback.
// Called when dial request is completed.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::HandleEmergencyDialL( const TInt aStatus )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::HandleEmergencyDialL calling" )

    iEmergencyCall = EFalse;

    if ( KErrNone == aStatus )
        {
        // Set result
        CompleteSetupCall( RSat::KSuccess );
        }
    else
        {
        CompleteSetupCall(
            RSat::KNetworkUnableToProcessCmd,
            RSat::KNoSpecificMeProblem );

        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::HandleEmergencyDialL Network unable \
        to process this" )
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::HandleEmergencyDialL exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CSetUpCallHandler::CSetUpCallHandler() :
    CSatCommandHandler(),
    iSetUpCallData(),
    iSetUpCallPckg( iSetUpCallData ),
    iSetUpCallRsp(),
    iSetUpCallRspPckg( iSetUpCallRsp ),
    iQueryData(),
    iQueryPckg( iQueryData ),
    iQueryRsp(),
    iQueryRspPckg( iQueryRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "SETUPCALL: CSetUpCallHandler::CSetUpCallHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::ConstructL()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::ConstructL calling" )

    // Create request handler. This is same that LaunchBrowser uses, so this
    // is needed also in HandleCommand - function.
    iUtils->RegisterServiceRequestL(
        ESatSProactiveQuery,
        ESatSProactiveQueryResponse,
        this );

    iUtils->RegisterL( this, MSatUtils::ECallControlExecuting );
    iUtils->RegisterL( this, MSatUtils::ECallControlDone );

    // Create service handler for normal call setup.
    iServiceHandler = CAiwServiceHandler::NewL();

    // Create dynamic resource to attach service handler.
    RCriteriaArray interest;
    CAiwCriteriaItem* item = CAiwCriteriaItem::NewLC(
        KAiwCmdCall,
        KAiwCmdCall,
        KContentType );

    TUid serviceClassBase;
    serviceClassBase.iUid = KAiwClassBase;
    item->SetServiceClass( serviceClassBase );

    interest.AppendL( item );

    // Attach to call service.
    iServiceHandler->AttachL( interest );

    CleanupStack::PopAndDestroy( item );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// Performs the request to dial
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::DoSetupCallL()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::DoSetupCallL calling" )

    RSat::TSetUpCallType callType( iSetUpCallData.iType );

    TDes& telNumber( iSetUpCallData.iAddress.iTelNumber );
    CheckNumber( telNumber );

    // 80 is max length in SAT calls, AIW max length is 100
    if ( telNumber.Length() > KMaximumPhoneNumberLength )
        {
        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::DoSetupCallL telNumber too long" )
        User::Leave( KErrArgument );
        }

    TBuf< AIWDialDataExt::KMaximumNameLength > name;
    if ( RSat::EAlphaIdProvided ==
         iSetUpCallData.iAlphaIdCallSetUpPhase.iStatus )
        {
        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::DoSetupCallL EAlphaIdProvided" )
        name = iSetUpCallData.iAlphaIdCallSetUpPhase.iAlphaId;
        }
    else
        {
        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::DoSetupCallL set AlphaId" )
        name = iUtils->SatAppName();
        }

    // Make the phone call parameters.
    CAiwInternalDialData* dialData = CAiwInternalDialData::NewLC();

    dialData->SetCallType( CAiwDialData::EAIWVoice );
    dialData->SetWindowGroup( AIWDialData::KAiwGoToIdle );
    dialData->SetPhoneNumberL( telNumber );
    dialData->SetSATCall( ETrue );
    dialData->SetShowNumber( EFalse );
    dialData->SetNameL( name );
    dialData->SetAllowMatch( EFalse );
    // Remove the redial mechanism from S60 5.x.
    dialData->SetRedial( AIWDialDataExt::KAIWRedialOff );

    // check if we need to disconnect other calls
    if ( ( RSat::EDisconnectOtherCalls == callType ) ||
         ( RSat::EDisconnectOtherCallsWithRedial == callType ) )
        {
        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::DoSetupCallL end other call" )
        dialData->SetEndOtherCalls( ETrue );
        }

    if ( AIWInternalDialData::KAiwBearerLength >=
         iSetUpCallData.iCapabilityConfigParams.Length() )
        {
        LOG( NORMAL, 
        "SETUPCALL: CSetUpCallHandler::DoSetupCallL SetBearerL" )
        dialData->SetBearerL( iSetUpCallData.iCapabilityConfigParams );
        }

    if ( AIWInternalDialData::KAiwSubAddressLength >=
         iSetUpCallData.iSubAddress.Length() )
        {
        LOG( NORMAL, 
        "SETUPCALL: CSetUpCallHandler::DoSetupCallL SetSubAddressL" )
        dialData->SetSubAddressL( iSetUpCallData.iSubAddress );
        }

    CAiwGenericParamList& paramList = iServiceHandler->InParamListL();
    dialData->FillInParamListL( paramList );

    iServiceHandler->ExecuteServiceCmdL(
        KAiwCmdCall,
        paramList,
        iServiceHandler->OutParamListL(),
        0,
        this );

    CleanupStack::PopAndDestroy( dialData );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::DoSetupCallL exiting" )
    }

// -----------------------------------------------------------------------------
// Return terminal response filled according to dial result.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CompleteSetupCallWithStatus(
    const TInt32 aStatus )
    {
    LOG2( SIMPLE,
        "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus calling: \
        %i", aStatus )

    // Form Terminal Response
    if ( KErrNone != aStatus )
        {
        switch ( aStatus )
            {
            case KErrGsmCCCallRejected:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus Call \
                Control not allowed" )
                // If SetUpCall is not allowed by SIM in Call Control, then
                // give a correct response.
                CompleteSetupCall(
                    RSat::KInteractionWithCCPermanentError,
                    RSat::KActionNotAllowed );
                break;
                }

            case KErrGsmCCBearerCapabilityNotAuthorised:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                Beyond ME Capabilities" )
                // If bearer capability is not authorized, return correct value
                CompleteSetupCall( RSat::KCmdBeyondMeCapabilities );
                break;
                }

            case KErrAbort:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus User \
                abort" )
                if ( iQueryRsp.iSessionTerminatedByUser )
                    {
                    LOG( SIMPLE, 
                    "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                    TerminatedByUser" )
                    // Notify sim session end command that next sim session end
                    // should close the ui session.
                    iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
                    }

                // User has ended redial mechanism.
                CompleteSetupCall( RSat::KCallClearedBeforeConnectionOrReleased );
                break;
                }

            case KErrGeneral:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus Data \
                not understood" )
                CompleteSetupCall( RSat::KCmdDataNotUnderstood );
                break;
                }

            case KErrAccessDenied:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                Network unable to process this" )
                CompleteSetupCall(
                    RSat::KNetworkUnableToProcessCmd,
                    RSat::KNoSpecificMeProblem );
                break;
                }
                
            case KErrSatControl:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                Call Command performed, but modified by Call Control" )
                // Call control modified the type of request indicated in
                // the proactive command, and the action requested by 
                // call control was performed successfully.
                CompleteSetupCall( RSat::KModifiedByCallControl );
                break;
                }
                
            default:
                {
                LOG( SIMPLE,
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                Network unable to process with error info" )

                iSetUpCallRsp.iGeneralResult = RSat::KNetworkUnableToProcessCmd;
                iSetUpCallRsp.iInfoType = RSat::KSatNetworkErrorInfo;

                // Lower byte contains the error cause.
                iSetUpCallRsp.iAdditionalInfo.SetLength( 1 );

                // Get mapped additional info
                TUint8 addInfo( TSatExtErrorUtils::MapError( aStatus ) );
                iSetUpCallRsp.iAdditionalInfo[0] =
                    static_cast<TUint16>( addInfo );

                // Send terminal response
                TerminalRsp( RSat::ESetUpCall, iSetUpCallRspPckg );
                break;
                }
            }
        }
    else
        {
        // Convert terminal rsp if icon used
        RSat::TIconQualifier qualifier1(
            iSetUpCallData.iIconIdConfirmationPhase.iQualifier );
        RSat::TIconQualifier qualifier2(
            iSetUpCallData.iIconIdCallSetUpPhase.iQualifier );

        RSat::TPCmdResult result( RSat::KSuccess );

        // Icon support for call confirmtion phase not done.
        if ( !iQueryRsp.iRequestedIconDisplayed &&
           ( ( RSat::ESelfExplanatory == qualifier1 ) ||
             ( RSat::ENotSelfExplanatory == qualifier1 ) ) )
            {
            result = RSat::KSuccessRequestedIconNotDisplayed;
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
            Confirmation icon not shown" )
            }
        // Icon support for call setup phase not done.
        else if ( ( RSat::ESelfExplanatory == qualifier2 ) ||
                  ( RSat::ENotSelfExplanatory == qualifier2 ) )
            {
            result = RSat::KSuccessRequestedIconNotDisplayed;
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus Icon \
            not shown" )
            }
        // If command had icon data and was done succesfully, report that icon
        // was not shown.
        // To be removed when icons are allowed in this command.
        else
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
            others" )
            if ( iIconCommand )
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                iIconCommand true" )
                result = RSat::KSuccessRequestedIconNotDisplayed;
                }
            }

        // Set result
        CompleteSetupCall( result );
        }

    LOG( SIMPLE,
        "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus exiting" )
    }

// -----------------------------------------------------------------------------
// Return terminal response filled according to result.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CompleteSetupCall(
    const RSat::TPCmdResult aGeneralResult )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CompleteSetupCall calling" )

    // Fill Terminal Response.
    iSetUpCallRsp.iGeneralResult = aGeneralResult;
    iSetUpCallRsp.iInfoType = RSat::KNoAdditionalInfo;
    iSetUpCallRsp.iAdditionalInfo.Zero();
    iSetUpCallRsp.SetPCmdNumber( iSetUpCallData.PCmdNumber() );

    // Send terminal response.
    TerminalRsp( RSat::ESetUpCall, iSetUpCallRspPckg );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CompleteSetupCall exiting" )
    }

// -----------------------------------------------------------------------------
// Return terminal response filled according to result.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CompleteSetupCall(
    const RSat::TPCmdResult aGeneralResult,
    const TInt16 aAdditionalInfo )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CompleteSetupCall calling" )

    // Fill Terminal Response.
    iSetUpCallRsp.iGeneralResult = aGeneralResult;
    iSetUpCallRsp.iInfoType = RSat::KMeProblem;
    iSetUpCallRsp.iAdditionalInfo.SetLength( 1 );
    iSetUpCallRsp.iAdditionalInfo[0] = aAdditionalInfo;
    iSetUpCallRsp.SetPCmdNumber( iSetUpCallData.PCmdNumber() );

    // Send terminal response.
    TerminalRsp( RSat::ESetUpCall, iSetUpCallRspPckg );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CompleteSetupCall exiting" )
    }

// -----------------------------------------------------------------------------
// Checks the dialling number string for extended BCD
// values. Changes them to correct characters
// see ETSI 11.11 10.5.1
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CheckNumber( TDes& aNumber ) const
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckNumber calling" )

    for ( TInt i = 0; i < aNumber.Length(); i++ )
        {
        // check values
        if ( KBCDAsterisk == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KAsteriskChar" )
            aNumber[i] = KAsteriskChar;
            }

        if ( KBCDDash == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KDashChar" )
            aNumber[i] = KDashChar;
            }

        if ( KBCDDTMF == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KDTMFChar" )
            aNumber[i] = KDTMFChar;
            }

        if ( KBCDWild == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KWildChar" )
            aNumber[i] = KWildChar;
            }

        if ( KBCDExpansion == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KExpansionChar" )
            aNumber[i] = KExpansionChar;
            }
        }
    LOG2( SIMPLE, 
    "SETUPCALL: CSetUpCallHandler::CheckNumber length of aNumber: %d",
     aNumber.Length() )
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckNumber exiting" )
    }

// -----------------------------------------------------------------------------
// Converts a TCCP error to the corresponding symbian error.
// -----------------------------------------------------------------------------
//
TInt CSetUpCallHandler::TccpErrorToSymbianError( const TInt aTccpError ) const
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::TccpErrorToSymbianError \
        calling" )
    
    // Error to be returned after mapped from aTccpError;
    // Initialized with default value KErrAccessDenied.
    TInt retValue( KErrAccessDenied );
    
    // Convert TCCP Error to Symbian Error in the switch braces.
    switch ( aTccpError )
        {
        case ECCPErrorNone:
        case ECCPErrorNotFound:
        case ECCPErrorGeneral:
        case ECCPErrorCancel:
        case ECCPErrorNoMemory:
        case ECCPErrorNotSupported:
        case ECCPErrorAlreadyInUse:
        case ECCPErrorNotReady:
            {
            retValue = aTccpError;
            break;
            }
            
        case ECCPErrorCCCallRejected:
            {
            retValue = KErrGsmCCCallRejected;
            break;
            }
            
        case ECCPRequestFailure:
            {
            retValue = KErrGsmMMCongestion;
            break;
            }
            
        case ECCPErrorCCBearerCapabilityNotAuthorised:
        case ECCPErrorBadRequest:
            {
            retValue = KErrGsmCCBearerCapabilityNotAuthorised;
            break;
            }
            
        case ECCPErrorAccessDenied:
            {
            retValue = KErrAccessDenied;
            break;
            }
        
         //The following are KErrGsmCallControlBase group.
        case ECCPErrorNotReached:
            {
            retValue = KErrGsmCCUnassignedNumber;
            break;
            }
            
        case ECCPErrorBusy:
            {
            retValue = KErrGsmCCUserBusy;
            break;
            }
            
        case ECCPErrorMovedPermanently:
            {
            retValue = KErrGsmCCNumberChanged;
            break;
            }
            
        case ECCPErrorInvalidURI:
            {
            retValue = KErrGsmCCInvalidNumberFormat;
            break;
            }
            
        case ECCPErrorNetworkOutOfOrder:
            {
            retValue = KErrGsmCCNetworkOutOfOrder;
            break;
            }
            
        case ECCPErrorCCNoRouteToDestination:
            {
            retValue = KErrGsmCCNoRouteToDestination;
            break;
            }

        case ECCPErrorCCDestinationOutOfOrder:
            {
            retValue = KErrGsmCCDestinationOutOfOrder;
            break;
            }
            
        case ECCPErrorCCResourceNotAvailable:
            {
            retValue = KErrGsmCCResourceNotAvailable;
            break;
            }
            

        case ECCPErrorCCInvalidTransitNetworkSelection:
            {
            retValue = KErrGsmCCInvalidTransitNetworkSelection;
            break;
            }
            
        case ECCPErrorCCIncompatibleDestination:
            {
            retValue = KErrGsmCCIncompatibleDestination;
            break;
            }
            
        case ECCPErrorCCIncompatibleMessageInCallState:
            {
            retValue = KErrGsmCCIncompatibleMessageInCallState;
            break;
            }

        case ECCPErrorCCIncompatibleMessageInProtocolState:
            {
            retValue = KErrGsmCCIncompatibleMessageInProtocolState;
            break;
            }
            
        case ECCPErrorCCNormalCallClearing:
            {
            retValue = KErrGsmCCNormalCallClearing;
            break;
            }
            
        case ECCPErrorCCUserAlertingNoAnswer:
            {
            retValue = KErrGsmCCUserAlertingNoAnswer;
            break;
            }

        case ECCPErrorCCUserNotResponding:
            {
            retValue = KErrGsmCCUserNotResponding;
            break;
            }

        case ECCPErrorCCPreemption:
            {
            retValue = KErrGsmCCPreemption;
            break;
            }
            
        case ECCPErrorCCFacilityRejected:
            {
            retValue = KErrGsmCCFacilityRejected;
            break;
            }

        case ECCPErrorCCResponseToStatusEnquiry:
            {
            retValue = KErrGsmCCResponseToStatusEnquiry;
            break;
            }
            
        case ECCPErrorCCInvalidMandatoryInformation:
            {
            retValue = KErrGsmCCInvalidMandatoryInformation;
            break;
            }
            
        case ECCPErrorCCNonExistentMessageType:
            {
            retValue = KErrGsmCCNonExistentMessageType;
            break;
            }

        case ECCPErrorCCNonExistentInformationElement:
            {
            retValue = KErrGsmCCNonExistentInformationElement;
            break;
            }

        case ECCPErrorCCNoChannelAvailable:
            {
            retValue = KErrGsmCCNoChannelAvailable;
            break;
            }

        case ECCPErrorCCRequestedFacilityNotSubscribed:
            {
            retValue = KErrGsmCCRequestedFacilityNotSubscribed;
            break;
            }

        case ECCPErrorCCIncomingCallsBarredInCug:
            {
            retValue = KErrGsmCCIncomingCallsBarredInCug;
            break;
            }
            
        case ECCPErrorUserNotInCug:
            {
            retValue = KErrGsmCCUserNotInCug;
            break;
            }
            
        case ECCPErrorCCRecoveryOnTimerExpiry:
            {
            retValue = KErrGsmCCRecoveryOnTimerExpiry;
            break;
            }

        case ECCPErrorCCBearerCapabilityNotCurrentlyAvailable:
            {
            retValue = KErrGsmCCBearerCapabilityNotCurrentlyAvailable;
            break;
            }

        case ECCPErrorCCServiceNotAvailable:
            {
            retValue = KErrGsmCCServiceNotAvailable;
            break;
            }

        case ECCPErrorCCBearerServiceNotImplemented:
            {
            retValue = KErrGsmCCBearerServiceNotImplemented;
            break;
            }

        case ECCPErrorCCOnlyRestrictedDigitalInformationBCAvailable:
            {
            retValue = KErrGsmCCOnlyRestrictedDigitalInformationBCAvailable;
            break;
            }
            
        case ECCPErrorCCServiceNotImplemented:
            {
            retValue = KErrGsmCCServiceNotImplemented;
            break;
            }
            
        case ECCPErrorCCUnspecifiedInterworkingError:
            {
            retValue = KErrGsmCCUnspecifiedInterworkingError;
            break;
            }
            
        case ECCPErrorSatControl:
            {
            retValue = KErrSatControl;
            break;
            }
                
        default:
            {
            retValue = KErrAccessDenied;
            break;
            }
        }
        
    LOG2( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::TccpErrorToSymbianError TCCP error:%d",
         aTccpError)
    LOG2( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::TccpErrorToSymbianError \
        mapped Symbian Error:%d", retValue)
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::TccpErrorToSymbianError \
        exiting" )
    
    return retValue;
    }

// -----------------------------------------------------------------------------
// Create emergency call
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CreateEmergencyCallL()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CreateEmergencyCallL calling" )
    
    if( !iEmergencyCallApi )
        {
        // Create service handler for emergency call setup..
        LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CreateEmergencyCallL\
         create emergency call handler" )
        iEmergencyCallApi = CPhCltEmergencyCall::NewL( this );
        }
    
    iEmergencyCallApi->DialEmergencyCallL( 
        iSetUpCallData.iAddress.iTelNumber );
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CreateEmergencyCallL exiting" )    
    }

// End Of File
