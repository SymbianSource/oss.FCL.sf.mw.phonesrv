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
* Description:  Implementation of the Ut_VmbxUiEngine class
*
*/

// System includes
#include <QtTest/QtTest>

// User includes
#include "vmbxuiengine.h"
#include "ut_vmbxuiengine.h"

/*!
    Ut_VmbxUiEngine::Ut_VmbxUiEngine
    Default constructor, remember to null new members here.
*/
Ut_VmbxUiEngine::Ut_VmbxUiEngine()
{
    qDebug("Ut_VmbxUiEngine <>");
}

/*!
    Ut_VmbxUiEngine::~Ut_VmbxUiEngine
*/
Ut_VmbxUiEngine::~Ut_VmbxUiEngine()
{
    qDebug("~Ut_VmbxUiEngine <>");
}

/*!
    Ut_VmbxUiEngine::initTestCase
    QTestLib initialization method, called for each test case.
*/
void Ut_VmbxUiEngine::initTestCase()
{
    qDebug("initTestCase >");
    mUiEngine = new VmbxUiEngine();
    QVERIFY(mUiEngine);
    qDebug("initTestCase <");
}

/*!
    Ut_VmbxUiEngine::cleanupTestCase
    QTestLib cleanup method, called for each test case.
*/
void Ut_VmbxUiEngine::cleanupTestCase()
{
    qDebug("cleanupTestCase >");
    if ( mUiEngine ) {
        delete mUiEngine;
        mUiEngine = NULL;
    }
    qDebug("cleanupTestCase <");
}

/*!
    Ut_VmbxUiEngine::testGetCsVoice1Number
    testCreateVmbxEntry test case
    Connects to test object signal and verifies received data.
*/
void Ut_VmbxUiEngine::testGetCsVoice1Number()
{
    qDebug("testGetCsVoice1Number >");
    QString voice1;
    QVERIFY(mUiEngine);
    mUiEngine->getCsVoice1Number(voice1);
    qDebug("testGetCsVoice1Number <");
}

/*!
    Ut_VmbxUiEngine::testCreateVmbxMailbox
    CreateVmbxMailbox test case
    Connects to test object signal and verifies received data.
*/
void Ut_VmbxUiEngine::testGetCsVoice2Number()
{
    qDebug("testGetCsVoice2Number >");
    QString voice2;
    QVERIFY(mUiEngine);
    mUiEngine->getCsVoice2Number(voice2);
    qDebug("testGetCsVoice2Number <");
}

/*!
    Ut_VmbxUiEngine::testGetCsVideo1Number
    testVmbxType test case
    Connects to test object signal and verifies received data.
*/
void Ut_VmbxUiEngine::testGetCsVideo1Number()
{
    qDebug("testGetCsVideo1Number >");
    QString video1;
    QVERIFY(mUiEngine);
    mUiEngine->getCsVideo1Number(video1);

    qDebug("testGetCsVideo1Number <");
}

/*!
    Ut_VmbxUiEngine::testUiCsVoice1Changed
    testServiceId test case
    Connects to test object signal and verifies received data.
*/
void Ut_VmbxUiEngine::testUiCsVoice1Changed()
{
    qDebug("testUiCsVoice1Changed >");
    QVERIFY(mUiEngine);
    QString newNumber("123456");
    mUiEngine->uiCsVoice1Changed(newNumber);
    qDebug("testUiCsVoice1Changed <");
}

//End file
