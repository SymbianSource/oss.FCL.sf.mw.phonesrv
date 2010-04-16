/*
* Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the Ut_vmbxCpPlugin class
*
*/

// System includes
#include <QtGui>
#include <e32base.h>

// User includes
#include "vmbxcpplugin.h"
#include "ut_vmbxcpplugin.h"

// load plugin
#include <cpitemdatahelper.h>

/*!
    Ut_vmbxCpPlugin::Ut_vmbxCpPlugin
    Default constructor, remember to null new members here.
*/
Ut_vmbxCpPlugin::Ut_vmbxCpPlugin()
{
    qDebug("Ut_vmbxCpPlugin::Ut_vmbxCpPlugin >");
    qDebug("Ut_vmbxCpPlugin::Ut_vmbxCpPlugin <");
}

/*!
    Ut_vmbxCpPlugin::~Ut_vmbxCpPlugin
*/
Ut_vmbxCpPlugin::~Ut_vmbxCpPlugin()
{
    qDebug("Ut_vmbxCpPlugin::~Ut_vmbxCpPlugin >");
    qDebug("Ut_vmbxCpPlugin::~Ut_vmbxCpPlugin <");
}

/*!
    Ut_vmbxCpPlugin::initTestCase
    QTestLib initialization method, called for each test case.
*/
void Ut_vmbxCpPlugin::initTestCase()
{
    qDebug("Ut_vmbxCpPlugin::initTestCase >");
    mCpPlugin = new VmbxCpPlugin();
    QVERIFY(mCpPlugin);
    qDebug("Ut_vmbxCpPlugin::initTestCase <");
}

/*!
    Ut_vmbxCpPlugin::cleanupTestCase
    QTestLib cleanup method, called for each test case.
*/
void Ut_vmbxCpPlugin::cleanupTestCase()
{
    qDebug("Ut_SatAppEngine::cleanupTestCase >");
    if ( mCpPlugin ) {
        delete mCpPlugin;
    qDebug("Ut_SatAppEngine::cleanupTestCase <");
    }
}

/*!
    Ut_vmbxCpPlugin::t_createSettingFormItemData
    testServiceId test case
    Connects to test object signal and verifies received data.
*/
void Ut_vmbxCpPlugin::testCreateSettingFormItemData()
{
    qDebug("Ut_vmbxCpPlugin::testCreateSettingFormItemData >");
    CpItemDataHelper itemDataHelper;
    QVERIFY(mCpPlugin);
    QList<CpSettingFormItemData *> list;
    list = mCpPlugin->createSettingFormItemData(itemDataHelper);
    QVERIFY2(0 != list[0], "createSettingFormItemData failed");
    qDebug("Ut_vmbxCpPlugin::testCreateSettingFormItemData <");
}

//End of file
