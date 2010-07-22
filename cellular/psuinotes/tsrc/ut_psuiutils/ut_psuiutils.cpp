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

#include "ut_psuiutils.h"
#include "qtestmains60.h"
#include <gsmerror.h>
#include <exterror.h>
#include <etelmm.h>
#include <btsapdomainpskeys.h>
#define private public
#include "psuiutils.h"

/*!
  UT_PsUiUtils::UT_PsUiUtils
 */
UT_PsUiUtils::UT_PsUiUtils() :
    mWrapper(NULL)
{
}

/*!
  UT_PsUiUtils::~UT_PsUiUtils
 */
UT_PsUiUtils::~UT_PsUiUtils()
{
    delete mWrapper;
}

/*!
  UT_PsUiUtils::init
 */
void UT_PsUiUtils::init()
{
    initialize();
    
    mWrapper = new PsUiSettingsWrapper();
}

/*!
  UT_PsUiUtils::cleanup
 */
void UT_PsUiUtils::cleanup()
{
    reset();
}

/*!
  UT_PsUiUtils::t_memleak
 */
void UT_PsUiUtils::t_memleak()
{
    
}

/*!
  UT_PsUiUtils::t_errorCodeTextMapping
 */
void UT_PsUiUtils::t_errorCodeTextMapping()
{
    QString text = "";
    PsUiUtils::errorCodeTextMapping(KErrGsmSSSubscriptionViolation, text);
    QCOMPARE( text, QString(
            "txt_phone_info_barring_operation_not_successful"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSUnknownSubscriber, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSAbsentSubscriber, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSIllegalOperation, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSIllegalSubscriber, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSIllegalEquipment, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSCallBarred, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSDataMissing, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSIncompatibility, text);
    QCOMPARE( text, QString(
            "txt_phone_info_conflict_error"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSSystemFailure, text);
    QCOMPARE( text, QString(
            "txt_phone_info_result_unknown"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSUnexpectedDataValue, text);
    QCOMPARE( text, QString(
            "txt_phone_info_request_rejected"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSResourcesUnavailable, text);
    QCOMPARE( text, QString(
            "txt_phone_info_request_rejected"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSNegativePasswordCheck, text);
    QCOMPARE( text, QString(
            "Password error"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSPasswordRegistrationFailure, text);
    QCOMPARE( text, QString(
            "Password error"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSPasswordAttemptsViolation, text);
    QCOMPARE( text, QString(
            "txt_phone_info_barring_password_blocked"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSMSNoNetworkService, text);
    QCOMPARE( text, QString(
            "No network coverage"));
    PsUiUtils::errorCodeTextMapping(KErrGsmNoService, text);
    QCOMPARE( text, QString(
            "No network coverage"));
    PsUiUtils::errorCodeTextMapping(KErrSsActivationDataLost, text);
    QCOMPARE( text, QString(
            "Check network services"));
    PsUiUtils::errorCodeTextMapping(KErrGsmSSUnknownAlphabet, text);
    QCOMPARE( text, QString("txt_phone_info_invalid_phone_number"));
    
    expect("XQSysInfo::isSupported").returns(false);
    PsUiUtils::errorCodeTextMapping(KErrGsmOfflineOpNotAllowed, text);
    QCOMPARE( text, QString(
            "txt_phone_info_request_not_completed"));
    QVERIFY( verify() );
    
    expect("XQSysInfo::isSupported").returns(true);
    expect("XQSettingsManager::readItemValue").returns(QVariant(EBTSapConnected));
    PsUiUtils::errorCodeTextMapping(KErrGsmOfflineOpNotAllowed, text);
    QCOMPARE( text, QString(
            "Operation not possible in SIM access profile mode"));
    QVERIFY( verify() );
    
    expect("XQSysInfo::isSupported").returns(true);
    expect("XQSettingsManager::readItemValue").returns(QVariant(EBTSapNotConnected));
    PsUiUtils::errorCodeTextMapping(KErrGsmOfflineOpNotAllowed, text);
    QCOMPARE( text, QString(
            "Operation not possible in Off-line mode"));
    
    PsUiUtils::errorCodeTextMapping(-1, text);
    QCOMPARE( text, QString(
            "txt_phone_info_request_not_completed"));
    
    QVERIFY( verify() );
        
}

/*!
  UT_PsUiUtils::::t_isFeatureCallWaitingDistiquishNotProvisionedEnabled
 */
void UT_PsUiUtils::t_isFeatureCallWaitingDistiquishNotProvisionedEnabled()
{
    expect("XQSettingsManager::readItemValue").
                returns(QVariant(1));
    mWrapper->isFeatureCallWaitingDistiquishNotProvisionedEnabled();
    QVERIFY( verify() );
}

/*!
  UT_PsUiUtils::::t_isPhoneOffline
 */
void UT_PsUiUtils::t_isPhoneOffline()
{
    expect("XQSysInfo::isSupported");
    mWrapper->isPhoneOffline();
    QVERIFY( verify() );
}

QTEST_MAIN_S60(UT_PsUiUtils)
