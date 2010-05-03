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


#include <msatuiobserver.h>
#include "ut_playtoneprovider.h"
//test class
#include "satappplaytoneprovider.h"

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::Ut_SatAppPlayToneProvider
// -----------------------------------------------------------------------------
//
Ut_SatAppPlayToneProvider::Ut_SatAppPlayToneProvider(QObject *parent):QObject(parent) 
{
    qDebug("Ut_SatAppPlayToneProvider::Ut_SatAppPlayToneProvider >");
    qDebug("Ut_SatAppPlayToneProvider::Ut_SatAppPlayToneProvider <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::~Ut_SatAppPlayToneProvider
// -----------------------------------------------------------------------------
//
Ut_SatAppPlayToneProvider::~Ut_SatAppPlayToneProvider()
{
    qDebug("Ut_SatAppPlayToneProvider::~Ut_SatAppPlayToneProvider >");
    qDebug("Ut_SatAppPlayToneProvider::~Ut_SatAppPlayToneProvider <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::initTestCase
// QTestLib cleanup method, called after the last testfunction .
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::initTestCase()
{
    qDebug("Ut_SatAppPlayToneProvider::initTestCase >");
    qDebug("Ut_SatAppPlayToneProvider::initTestCase <");
}


// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::cleanupTestCase
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::cleanupTestCase()
{
    qDebug("Ut_SatAppPlayToneProvider::cleanupTestCase >");
    if (mPlayTone) {
        qDebug("Ut_SatAppPlayToneProvider::cleanupTestCase delete mPlayTone...");
        delete mPlayTone;
        mPlayTone = 0;
    }
    qDebug("Ut_SatAppPlayToneProvider::cleanupTestCase <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testCreatePlayTone
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testCreatePlayTone()
{
    qDebug("Ut_SatAppPlayToneProvider::testCreatePlayTone >");
    mPlayTone = new SatAppPlayToneProvider();
    QVERIFY(mPlayTone); 
    qDebug("Ut_SatAppPlayToneProvider::testCreatePlayTone <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testPlayStandardTone_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testPlayStandardTone_data()
{
    qDebug("Ut_SatAppPlayToneProvider::testPlayStandardTone_data >");
    QTest::addColumn<QString>("text");//enum 0~3
    QTest::addColumn<int>("duration");
    QTest::addColumn<bool>("isRequestedIconDisplayed");
    QTest::addColumn<int>("expectReturnValue");
    // test duration 0
    QTest::newRow("duration_0") << "Play Standard tone" << 0 << false << 0;
    QTest::newRow("duration") << "Play Standard tone" << 1000000 << false << 0;
    QTest::newRow("textnull") << "" << 10000000 << false << 0;
    qDebug("Ut_SatAppPlayToneProvider::testPlayStandardTone_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testPlayStandardTone
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testPlayStandardTone()
{
    qDebug("Ut_SatAppPlayToneProvider::testPlayStandardTone >");
    QVERIFY(mPlayTone);
    QFETCH(QString, text);
    QFETCH(int, duration);
    QFETCH(bool, isRequestedIconDisplayed);
    QFETCH(int, expectReturnValue);
    qDebug("Ut_SatAppPlayToneProvider::testPlayTone after fetch");
    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // text
    _LIT( KPlayToneSequence, "Play tone sequence");
    TBuf8<32> aSequence;
    aSequence.Copy(KPlayToneSequence);

    // aDuration
    TUint aDuration= static_cast<TUint>(duration);
    CFbsBitmap*  bitmap(NULL);
    qDebug("Ut_SatAppPlayToneProvider::testPlayStandardTone call");
    TSatUiResponse result(ESatSuccess);
    TRAPD(err, result = mPlayTone->PlayStandardToneL(
            aText,
            aSequence,
            aDuration,
            bitmap,
            isRequestedIconDisplayed));
    QCOMPARE(KErrNone, err);
    TSatUiResponse exValue = static_cast<TSatUiResponse>(expectReturnValue);
    QCOMPARE(exValue, result);
    delete hText;
    qDebug("Ut_SatAppPlayToneProvider::testPlayStandardTone <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testPlayUserSelectedTone_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testPlayUserSelectedTone_data()
{
    qDebug("Ut_SatAppPlayToneProvider::testPlayUserSelectedTone_data >");
    QTest::addColumn<QString>("text");//enum 0~3
    QTest::addColumn<int>("duration");
    QTest::addColumn<int>("satTone");
    QTest::addColumn<bool>("isaSelfExplanatory");
    QTest::addColumn<int>("expectReturnValue");
    //QTest::newRow("duration_0") << "Play UserSelectedTone" << 0 << 1 <<false << 0;
    QTest::newRow("duration") << "Play UserSelectedTone" << 1000000 << 2 << false << 0;
    QTest::newRow("true") << "Play UserSelectedTone" << 20000000 << 4 << true << 0;
    QTest::newRow("textnull") << "" << 10000000 << 3 << true << 0;
    qDebug("Ut_SatAppPlayToneProvider::testPlayUserSelectedTone_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testPlayUserSelectedTone
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testPlayUserSelectedTone()
{
    qDebug("Ut_SatAppPlayToneProvider::testPlayUserSelectedTone >");
    QVERIFY(mPlayTone);
    QFETCH(QString, text);
    QFETCH(int, duration);
    QFETCH(int, satTone);
    QFETCH(bool, isaSelfExplanatory);
    QFETCH(int, expectReturnValue);
    qDebug("Ut_SatAppPlayToneProvider::testPlayTone after fetch");
    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // aTone
    TSatTone aTone = static_cast<TSatTone>(satTone);
    // aDuration
    TUint aDuration= static_cast<TUint>(duration);
    CFbsBitmap*  bitmap(NULL);
    TBool aSelfExplanatory(isaSelfExplanatory);
    TSatUiResponse result(ESatSuccess);
    TRAPD(err, result = mPlayTone->PlayUserSelectedToneL(
                        aText,
                        aDuration,
                        aTone,
                        bitmap,
                        aSelfExplanatory ));
    QCOMPARE(KErrNone, err);
    delete hText;
    hText = 0;
    TSatUiResponse exValue = static_cast<TSatUiResponse>(expectReturnValue);
    QCOMPARE(exValue, result);
    qDebug("Ut_SatAppPlayToneProvider::testPlayUserSelectedTone <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testPlayUserSelectedTone
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testCloseSatUI()
{
    qDebug("Ut_SatAppPlayToneProvider::closeSatUI >");
    QVERIFY(mPlayTone);
    mPlayTone->closeSatUI();
    qDebug("Ut_SatAppPlayToneProvider::closeSatUI <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testPlayUserSelectedTone
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testClearScreen()
{
    qDebug("Ut_SatAppPlayToneProvider::clearScreen >");
    QVERIFY(mPlayTone);
    mPlayTone->clearScreen();
    qDebug("Ut_SatAppPlayToneProvider::clearScreen <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testMapcPlayComplete_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testMapcPlayComplete_data()
{
    qDebug("Ut_SatAppPlayToneProvider::testMapcPlayComplete_data >");
    QTest::addColumn<int>("error");
    QTest::newRow("error_0") << 0;
    QTest::newRow("error_-6") << -6;
    qDebug("Ut_SatAppPlayToneProvider::testMapcPlayComplete_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testMapcPlayComplete
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testMapcPlayComplete()
{
    qDebug("Ut_SatAppPlayToneProvider::testMapcPlayComplete >");
    QVERIFY(mPlayTone);
    QFETCH(int, error);
    mPlayTone->MapcPlayComplete(error);
    qDebug("Ut_SatAppPlayToneProvider::testMapcPlayComplete <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testMapcInitComplete_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testMapcInitComplete_data()
{
    qDebug("Ut_SatAppPlayToneProvider::testMapcInitComplete_data >");
    QTest::addColumn<int>("error");
    QTest::newRow("error_0") << 0;
    QTest::newRow("error_-6") << -6;
    qDebug("Ut_SatAppPlayToneProvider::testMapcInitComplete_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppPlayToneProvider::testMapcInitComplete
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppPlayToneProvider::testMapcInitComplete()
{
    qDebug("Ut_SatAppPlayToneProvider::testMapcInitComplete >");
    QVERIFY(mPlayTone);
    QFETCH(int, error);
    mPlayTone->MapcInitComplete(error, 0);
    qDebug("Ut_SatAppPlayToneProvider::testMapcInitComplete <");
}

// End of file

