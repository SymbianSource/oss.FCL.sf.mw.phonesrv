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
* Description: Unit test for satappmainhandler
*
*/


#include <hbmainwindow.h>
#include "ut_satappmainhandler.h"
//test class
#include "satappmainhandler.h"

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::Ut_SatAppMainHandler
// -----------------------------------------------------------------------------
//
Ut_SatAppMainHandler::Ut_SatAppMainHandler(QObject *parent):QObject(parent) 
{
    qDebug("Ut_SatAppMainHandler::Ut_SatAppMainHandler >");
    qDebug("Ut_SatAppMainHandler::Ut_SatAppMainHandler <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::~Ut_SatAppMainHandler
// -----------------------------------------------------------------------------
//
Ut_SatAppMainHandler::~Ut_SatAppMainHandler()
{
    qDebug("Ut_SatAppMainHandler::~Ut_SatAppMainHandler >");
    qDebug("Ut_SatAppMainHandler::~Ut_SatAppMainHandler <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::initTestCase
// QTestLib cleanup method, called after the last testfunction .
// -----------------------------------------------------------------------------
void Ut_SatAppMainHandler::initTestCase()
{
    qDebug("Ut_SatAppMainHandler::initTestCase >");
    qDebug("Ut_SatAppMainHandler::initTestCase <");
}


// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::cleanupTestCase
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppMainHandler::cleanupTestCase()
{
    qDebug("Ut_SatAppMainHandler::cleanupTestCase >");
    if (mMainHandler) {
        qDebug("Ut_SatAppMainHandler::cleanupTestCase delete mMainHandler...");
        delete mMainHandler;
        mMainHandler = 0;
    }
    qDebug("Ut_SatAppMainHandler::cleanupTestCase <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::testProfileState
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppMainHandler::testCreateMainHandler()
{
    qDebug("Ut_SatAppMainHandler::testProfileState >");
    HbMainWindow* window = new HbMainWindow();
    QVERIFY(window);
    mMainHandler = new SatAppMainHandler(*window);
    QVERIFY(mMainHandler); 
    qDebug("Ut_SatAppMainHandler::testProfileState <");
}

// End of file
