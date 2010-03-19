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
* Description:  Implementation of the Ut_vmbxCpPlugin class
*
*/

//qt
#include <QtGui>
//hb
#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbview.h>
//symbian
#include <e32base.h>

#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include <voicemailboxdefs.h>
#include <mvoicemailboxobserver.h>

#include "vmbxuiengine.h"
#include "vmbxcpgroup.h"
#include "Ut_vmbxCpPlugin.h"


/*------------------------------------------------------------------------------
This module testing project links to vmbxengine.dll.
------------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// Constants for verifying test case results
// -----------------------------------------------------------------------------
//


 // -----------------------------------------------------------------------------
// Ut_vmbxCpPlugin::Ut_vmbxCpPlugin
// Default constructor, remember to null new members here.
// -----------------------------------------------------------------------------
//
Ut_vmbxCpPlugin::Ut_vmbxCpPlugin()
{
    qDebug("Ut_vmbxCpPlugin::Ut_vmbxCpPlugin >");
    qDebug("Ut_vmbxCpPlugin::Ut_vmbxCpPlugin <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxCpPlugin::~Ut_vmbxCpPlugin
// -----------------------------------------------------------------------------
Ut_vmbxCpPlugin::~Ut_vmbxCpPlugin()
{
    qDebug("Ut_vmbxCpPlugin::~Ut_vmbxCpPlugin >");
    qDebug("Ut_vmbxCpPlugin::~Ut_vmbxCpPlugin <");
}


// -----------------------------------------------------------------------------
// Ut_vmbxCpPlugin::initTestCase
// QTestLib initialization method, called for each test case.
// -----------------------------------------------------------------------------
void Ut_vmbxCpPlugin::initTestCase()
{
    qDebug("Ut_vmbxCpPlugin::initTestCase >");
    qDebug("Ut_vmbxCpPlugin::Create Window ");
    mWindow = new HbMainWindow();
    QVERIFY(mWindow);
    mWindow->show();
    qDebug("Ut_vmbxCpPlugin::Create VmbxCpGroup");
    mCpGroup = new VmbxCpGroup();
    QVERIFY(mCpGroup);
    qDebug("Ut_vmbxCpPlugin::Create VmbxUiEngine");
    mUiEngine = new VmbxUiEngine();
    QVERIFY(mUiEngine);
    qDebug("Ut_vmbxCpPlugin::initTestCase <");
}


// -----------------------------------------------------------------------------
// Ut_vmbxCpPlugin::cleanupTestCase
// QTestLib cleanup method, called for each test case.
// -----------------------------------------------------------------------------
void Ut_vmbxCpPlugin::cleanupTestCase()
{
    qDebug("Mt_SatAppEngine::cleanupTestCase >");
    if ( mWindow ) {
        delete mWindow;
    }
    if ( mCpGroup ) {
        delete mCpGroup;
    }
    if ( mUiEngine ) {
        delete mUiEngine;
    }
    qDebug("Mt_SatAppEngine::cleanupTestCase <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxCpPlugin::testGetCsVoice1Number
// testCreateVmbxEntry test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxCpPlugin::testGetCsVoice1Number()
{
    qDebug("Ut_vmbxCpPlugin::testGetCsVoice1Number >");
    QString voiceNumber;
    mUiEngine->GetCsVoice1Number(voiceNumber);
    if ( voiceNumber.length()) {
    
    }
    //QVERIFY2(KErrNone == err, "create CVoiceMailboxEntry failed ");
    qDebug("Ut_vmbxCpPlugin::testGetCsVoice1Number <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxCpPlugin::testCreateVmbxMailbox
// CreateVmbxMailbox test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxCpPlugin::testGetCsVoice2Number()
{
    qDebug("Ut_vmbxCpPlugin::testGetCsVoice2Number >");
    qDebug("Ut_vmbxCpPlugin::testGetCsVoice2Number <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxCpPlugin::testGetCsVideo1Number
// testVmbxType test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxCpPlugin::testGetCsVideo1Number()
{
    qDebug("Ut_vmbxCpPlugin::testGetCsVideo1Number >");
    //QVERIFY(mVmbxEntry);
    //QCOMPARE(mVmbxEntry->VoiceMailboxType(), EVmbxVoice);
    qDebug("Ut_vmbxCpPlugin::testGetCsVideo1Number <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxCpPlugin::testDefaultMailboxType
// testServiceId test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxCpPlugin::testDefaultMailboxType()
{
    qDebug("Ut_vmbxCpPlugin::testDefaultMailboxType >");
    qDebug("Ut_vmbxCpPlugin::testDefaultMailboxType <");
}

// -----------------------------------------------------------------------------
// main()
// Main method implemented for directing test output to a file.
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
    {
    qDebug("Ut_vmbxCpPlugin.cpp: main() >");
    QApplication app(argc, argv);

    qDebug("Ut_vmbxCpPlugin.cpp: Ut_vmbxCpPlugin");
    Ut_vmbxCpPlugin tc; 
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\vmbx\\Ut_vmbxCpPlugin.txt";
    const int result = QTest::qExec(&tc, 3, pass);

    qDebug("Ut_vmbxCpPlugin.cpp: main() <, result=%d", result);
    return result;
    }

//End file
