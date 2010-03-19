/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  
 *
 */

#include <mpsetdivertobs.h>
#include <mphcltemergencycallobserver.h>
#include <mmlist.h>
#include "psetcalldivertingwrapper_p.h"
#include "logging.h"

/*!
  PSetCallDivertingWrapperPrivate::PSetCallDivertingWrapperPrivate
 */
PSetCallDivertingWrapperPrivate::PSetCallDivertingWrapperPrivate(
        PSetCallDivertingWrapper &owner) :
    m_Owner(owner)
{
    DPRINT;
}

/*!
  PSetCallDivertingWrapperPrivate::~PSetCallDivertingWrapperPrivate
 */
PSetCallDivertingWrapperPrivate::~PSetCallDivertingWrapperPrivate()
{
    DPRINT;
    while (!m_DivertingStatusList.empty()) {
        DPRINT << "m_DivertingStatusList item to be deleted";
        delete  m_DivertingStatusList.takeFirst();
    }
}

/*!
  PSetCallDivertingWrapperPrivate::HandleDivertingChangedL
 */
void PSetCallDivertingWrapperPrivate::HandleDivertingChangedL(
        const TCallDivertSetting& aSetting, TBool aPlural)
{
    DPRINT << ": IN ";
    
    m_callDivertingSetting.iCondition = convert(aSetting.iCondition);
    m_callDivertingSetting.iStatus = convert(aSetting.iStatus);
    m_callDivertingSetting.iSetting = 
        static_cast<PsCallDivertingSetting>(aSetting.iSetting);
    m_callDivertingSetting.iServiceGroup = convert(aSetting.iServiceGroup);
    m_callDivertingSetting.iNumber = 
        QString::fromUtf16(aSetting.iNumber.Ptr(), aSetting.iNumber.Length());
    m_callDivertingSetting.iNoReplyTimer = aSetting.iNoReplyTimer;
    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleDivertingChanged(m_callDivertingSetting, aPlural));

    DPRINT << ": OUT ";
}

/*!
  PSetCallDivertingWrapperPrivate::HandleDivertingStatusL
 */
void PSetCallDivertingWrapperPrivate::HandleDivertingStatusL(
        CMobilePhoneCFList& aSetting, TBool aPlural)
{
    DPRINT << ": IN ";

    // reset list
    while (!m_DivertingStatusList.empty()) {
        PSCallDivertingStatus* divertingStatus =
            m_DivertingStatusList.takeFirst();
        DPRINT << ": delete :  " << divertingStatus;
        delete divertingStatus;
    }
    TInt entries = aSetting.Enumerate();
    TInt numberOfBscs = 0;
    RMobilePhone::TMobilePhoneCFInfoEntryV1 cfInfo;
    while (entries > numberOfBscs) {
        cfInfo = aSetting.GetEntryL(numberOfBscs);
        PSCallDivertingStatus* divertingStatus =
            new (ELeave) PSCallDivertingStatus();
        divertingStatus->iCondition = convert(cfInfo.iCondition);
        divertingStatus->iServiceGroup = cfInfo.iServiceGroup;
        divertingStatus->iStatus = convert(cfInfo.iStatus);
        divertingStatus->iNumber = QString::fromUtf16(
            cfInfo.iNumber.iTelNumber.Ptr(),
            cfInfo.iNumber.iTelNumber.Length());
        divertingStatus->iTimeout = cfInfo.iTimeout;
        m_DivertingStatusList.append(divertingStatus);
        numberOfBscs++;
    }

    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleDivertingStatus(m_DivertingStatusList,aPlural));

    DPRINT << ": OUT";
}

/*!
  PSetCallDivertingWrapperPrivate::HandleDivertingErrorL
 */
void PSetCallDivertingWrapperPrivate::HandleDivertingErrorL(TInt aReason)
{
    DPRINT << ": IN : aReason" << aReason;
    
    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleDivertingError(aReason));
    
    DPRINT << ": OUT ";
}

/*!
  PSetCallDivertingWrapperPrivate::HandleCFRequestingL
 */
void PSetCallDivertingWrapperPrivate::HandleCFRequestingL(TBool aOngoing,
        TBool aInterrupted)
{
    DPRINT << ": IN ";

    QT_TRYCATCH_LEAVING(
        emit m_Owner.handleCFRequesting(aOngoing, aInterrupted));

    DPRINT << ": OUT ";
}

/*!
  PSetCallDivertingWrapperPrivate::SetEngineContact
 */
void PSetCallDivertingWrapperPrivate::SetEngineContact(
        MPsetCallDiverting* aDivertEngine)
{
    DPRINT << ": IN ";
    
    Q_UNUSED(aDivertEngine);
    
    DPRINT << ": OUT ";
}

/*!
  PSetCallDivertingWrapperPrivate::HandleEmergencyDialL
 */
void PSetCallDivertingWrapperPrivate::HandleEmergencyDialL(const TInt aStatus)
{
    DPRINT << ": IN ";
    
    Q_UNUSED(aStatus);
    
    DPRINT << ": OUT ";
}

/*!
  PSetCallDivertingWrapperPrivate::RequestComplete
 */
void PSetCallDivertingWrapperPrivate::RequestComplete()
{
    DPRINT << ": IN ";
    QT_TRYCATCH_LEAVING(
        emit m_Owner.requestDone());
    DPRINT << ": OUT ";
}

/*!
  PSetCallDivertingWrapperPrivate::RequestStatusChanged
 */
void PSetCallDivertingWrapperPrivate::RequestStatusChanged(TInt aNewStatus)
{
    DPRINT << ": IN ";
    Q_UNUSED(aNewStatus);
    DPRINT << ": OUT ";
}

/*!
  PSetCallDivertingWrapperPrivate::convert
 */
PsCallDivertingCondition PSetCallDivertingWrapperPrivate::convert(
         TCallDivertingCondition type)
{
    switch (type) {
    case EDivertConditionUnconditional:
        return DivertConditionUnconditional;
    case EDivertConditionBusy:
        return DivertConditionBusy;
    case EDivertConditionNoReply:
        return DivertConditionNoReply;
    case EDivertConditionNotReachable:
        return DivertConditionNotReachable;
    case EDivertConditionAllCalls:
        return DivertConditionAllCalls;
    case EDivertConditionAllConditionalCases:
        return DivertConditionAllConditionalCases;
    default:
        break;
    }
    return DivertConditionUnknown;
}

/*!
  PSetCallDivertingWrapperPrivate::convert
 */
TCallDivertingCondition PSetCallDivertingWrapperPrivate::convert(
        PsCallDivertingCondition type)
{
    switch (type) {
    case DivertConditionUnconditional:
        return EDivertConditionUnconditional;
    case DivertConditionBusy:
        return EDivertConditionBusy;
    case DivertConditionNoReply:
        return EDivertConditionNoReply;
    case DivertConditionNotReachable:
        return EDivertConditionNotReachable;
    case DivertConditionAllCalls:
        return EDivertConditionAllCalls;
    case DivertConditionAllConditionalCases:
        return EDivertConditionAllConditionalCases;
    default:
        break;
    }
    return EDivertConditionUnconditional;
}

/*!
  PSetCallDivertingWrapperPrivate::convert
 */
PsCallDivertingCondition PSetCallDivertingWrapperPrivate::convert(
        RMobilePhone::TMobilePhoneCFCondition type)
{
    switch (type) {
    case RMobilePhone::ECallForwardingUnconditional:
        return DivertConditionUnconditional;
    case RMobilePhone::ECallForwardingBusy:
        return DivertConditionBusy;
    case RMobilePhone::ECallForwardingNoReply:
        return DivertConditionNoReply;
    case RMobilePhone::ECallForwardingNotReachable:
        return DivertConditionNotReachable;
    case RMobilePhone::ECallForwardingAllCases:
        return DivertConditionAllCalls;
    case RMobilePhone::ECallForwardingAllConditionalCases:
        return DivertConditionAllConditionalCases;
    default:
        break;
    }
    return DivertConditionUnknown;
}

/*!
  PSetCallDivertingWrapperPrivate::convert
 */
PsCallDivertingStatus PSetCallDivertingWrapperPrivate::convert(
        TDivertingStatus type)
{
    switch (type) {
    case EDivertingStatusActive:
        return DivertingStatusActive;
    case EDivertingStatusInactive:
        return DivertingStatusInactive;
    case EDivertingStatusNotRegistered:
        return DivertingStatusNotRegistered;
    case EDivertingStatusNotProvisioned:
        return DivertingStatusNotProvisioned;
    default:
        break;
    }
    return DivertingStatusUnknown;
}

/*!
  PSetCallDivertingWrapperPrivate::convert
 */
TDivertingStatus PSetCallDivertingWrapperPrivate::convert(
        PsCallDivertingStatus type)
{
    switch (type) {
    case DivertingStatusActive:
        return EDivertingStatusActive;
    case DivertingStatusInactive:
        return EDivertingStatusInactive;
    case DivertingStatusNotRegistered:
        return EDivertingStatusNotRegistered;
    case DivertingStatusNotProvisioned:
        return EDivertingStatusNotProvisioned;
    default:
        break;
    }
    return EDivertingStatusUnknown;
}

/*!
  PSetCallDivertingWrapperPrivate::convert
 */
PsCallDivertingStatus PSetCallDivertingWrapperPrivate::convert(
        RMobilePhone::TMobilePhoneCFStatus type)
{
    switch (type) {
    case RMobilePhone::ECallForwardingStatusActive:
        return DivertingStatusActive;
    case RMobilePhone::ECallForwardingStatusNotActive:
        return DivertingStatusInactive;
    case RMobilePhone::ECallForwardingStatusNotRegistered:
        return DivertingStatusNotRegistered;
    case RMobilePhone::ECallForwardingStatusNotProvisioned:
        return DivertingStatusNotProvisioned;
    default:
        break;
    }
    return DivertingStatusUnknown;
}

/*!
  PSetCallDivertingWrapperPrivate::convert
 */
PsServiceGroup PSetCallDivertingWrapperPrivate::convert(TServiceGroup type)
{
    PsServiceGroup ret(ServiceGroupAllTeleservices);
    ret &= type;
    return ret;
}

/*!
  PSetCallDivertingWrapperPrivate::convert
 */
TServiceGroup PSetCallDivertingWrapperPrivate::convert(PsServiceGroup type)
{
    const int tmp = type;
    TServiceGroup ret = (TServiceGroup)tmp;
    return ret;
}

TDivertingSetting PSetCallDivertingWrapperPrivate::convert(
        PsCallDivertingSetting type)
{
    switch(type){
     case ActivateDiverting:
        return EActivateDiverting;
    case CancelDiverting:
        return ECancelDiverting;
    case RegisterDiverting:
        return ERegisterDiverting;
    case EraseDiverting:
        return EEraseDiverting;
    case CheckStatus:
    default:
        return ECheckStatus;
    }
}

// end of file
