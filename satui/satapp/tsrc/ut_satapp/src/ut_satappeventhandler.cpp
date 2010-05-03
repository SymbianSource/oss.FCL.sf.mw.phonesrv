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
* Description: Unit test for satappeventprovider
*
*/

#include "ut_satappeventhandler.h"
//test class
#include "satappeventprovider.h"

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::Ut_SatAppEventProvider
// -----------------------------------------------------------------------------
//
Ut_SatAppEventProvider::Ut_SatAppEventProvider(QObject *parent):
        QObject(parent), mEventHandler(0)
{
    qDebug("Ut_SatAppEventProvider::Ut_SatAppEventProvider >");
    qDebug("Ut_SatAppEventProvider::Ut_SatAppEventProvider <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::~Ut_SatAppEventProvider
// -----------------------------------------------------------------------------
//
Ut_SatAppEventProvider::~Ut_SatAppEventProvider()
{
    qDebug("Ut_SatAppEventProvider::~Ut_SatAppEventProvider >");
    qDebug("Ut_SatAppEventProvider::~Ut_SatAppEventProvider <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::initTestCase
// QTestLib cleanup method, called after the last testfunction .
// -----------------------------------------------------------------------------
void Ut_SatAppEventProvider::initTestCase()
{
    qDebug("Ut_SatAppEventProvider::initTestCase >");
    qDebug("Ut_SatAppEventProvider::initTestCase <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::cleanupTestCase
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppEventProvider::cleanupTestCase()
{
    qDebug("Ut_SatAppEventProvider::cleanupTestCase >");
    delete mEventHandler;
    mEventHandler = 0;
    qDebug("Ut_SatAppEventProvider::cleanupTestCase <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::testProfileState
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppEventProvider::testCreateEventHandler()
{
    qDebug("Ut_SatAppEventProvider::testCreateEventHandler >");
    mEventHandler = new SatAppEventProvider;
    QVERIFY(mEventHandler);
    qDebug("Ut_SatAppEventProvider::testCreateEventHandler <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::testShowSsWaitNote_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppEventProvider::testShowSsWaitNote_data()
{
    qDebug("Ut_SatAppEventProvider::testShowSsWaitNote_data >");
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("isSelfExplanatoryIcon");
    QTest::newRow("first") << "Show Ss Wait note First!" << false;
    QTest::newRow("second") << "Show Ss Wait note seconde!" << true;
    QTest::newRow("third") << "" << false;
    qDebug("Ut_SatAppEventProvider::testShowSsWaitNote_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::testProfileState
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppEventProvider::testShowSsWaitNote()
{
    qDebug("Ut_SatAppEventProvider::testShowSsWaitNote >");
    QVERIFY(mEventHandler);
    QFETCH(QString, text);
    QFETCH(bool, isSelfExplanatoryIcon);
    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    CFbsBitmap* bitmap(NULL);
    TRAPD(err, mEventHandler->ShowSsWaitNoteL(aText,bitmap,
          isSelfExplanatoryIcon));
    delete hText;
    hText = 0;
    QCOMPARE(err, KErrNone);
    qDebug("Ut_SatAppEventProvider::testShowSsWaitNote <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::testProfileState
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppEventProvider:: testStopShowWaitNote()
{
    qDebug("Ut_SatAppEventProvider::testStopShowWaitNote >");
    QVERIFY(mEventHandler);
    mEventHandler->StopShowWaitNote();
    qDebug("Ut_SatAppEventProvider::testStopShowWaitNote <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::ShowWaitNoteWithoutDelayL
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppEventProvider::testShowWaitNoteWithoutDelay()
{
    qDebug("Ut_SatAppEventProvider::testShowWaitNoteWithoutDelay >");
    QVERIFY(mEventHandler);
    TRAPD(err,mEventHandler->ShowWaitNoteWithoutDelayL());
    QCOMPARE(err, KErrNone);
    qDebug("Ut_SatAppEventProvider::testShowWaitNoteWithoutDelay <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppEventProvider::testShowSsErrorNote
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppEventProvider::testShowSsErrorNote()
{
    qDebug("Ut_SatAppEventProvider::testShowSsErrorNote >");
    QVERIFY(mEventHandler);
    TRAPD(err, mEventHandler->ShowSsErrorNoteL());
    QCOMPARE(err, KErrNone);
    qDebug("Ut_SatAppEventProvider::testShowSsErrorNote <");
}
    
// End of file
