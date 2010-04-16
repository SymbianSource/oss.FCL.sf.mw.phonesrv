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
* Description:  Implementation of the Ut_vmbxEngine class
*
*/

#include <QtGui>


#include <hbapplication.h>
#include <hbmainwindow.h>

#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include <voicemailboxdefs.h>
#include <mvoicemailboxobserver.h>
#include "../../../inc/vmbxqtuihandler.h"
#include "../../../inc/vmbxuihandler.h"

#include "ut_vmbxengine.h"

// -----------------------------------------------------------------------------
// Constants for verifying test case results
// -----------------------------------------------------------------------------
//


 // -----------------------------------------------------------------------------
// Ut_vmbxEngine::Ut_vmbxEngine
// Default constructor, remember to null new members here.
// -----------------------------------------------------------------------------
//
Ut_vmbxEngine::Ut_vmbxEngine()
{
    qDebug("Ut_vmbxEngine::Ut_vmbxEngine >");
    qDebug("Ut_vmbxEngine::Ut_vmbxEngine <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxEngine::~Ut_vmbxEngine
// -----------------------------------------------------------------------------    
Ut_vmbxEngine::~Ut_vmbxEngine()
{
    qDebug("Ut_vmbxEngine::~Ut_vmbxEngine >");
    qDebug("Ut_vmbxEngine::~Ut_vmbxEngine <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxEngine::initTestCase
// QTestLib initialization method, called before the last testfunction .
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::initTestCase()
{
    qDebug("Ut_vmbxEngine::initTestCase >");
    mQtUiHandler = new VmbxQtUiHandler();
    QVERIFY(mQtUiHandler);
    qDebug("Ut_vmbxEngine::initTestCase <");
}


// -----------------------------------------------------------------------------
// Ut_vmbxEngine::cleanupTestCase
// QTestLib cleanup method, called after the last testfunction .
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::cleanupTestCase()
{
    qDebug("Ut_vmbxEngine::cleanupTestCase >");
    delete mQtUiHandler;
    mQtUiHandler = 0;
    qDebug("Ut_vmbxEngine::cleanupTestCase <");
}


// -----------------------------------------------------------------------------
// Ut_vmbxEngine::testCreateVmbxMailbox
// CreateVmbxMailbox test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::testCreateVmbxMailbox()
{
    qDebug("Ut_vmbxEngine::testCreateVmbxMailbox >");
    TRAPD(err,mVmbxEngine = CVoiceMailbox::NewL());
    QVERIFY2(KErrNone == err, "CVoiceMailbox::NewL failed");
    qDebug("Ut_vmbxEngine::testCreateVmbxMailbox <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxEngine::testCreateWindow
// testCreateWindow test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::testCreateWindow()
{
    qDebug("Ut_vmbxEngine::testCreateWindow >");
    mWindow = new HbMainWindow();
    QVERIFY(mWindow);
    mWindow->show();
    qDebug("Ut_vmbxEngine::testCreateWindow Windowshow");
	qDebug("Ut_vmbxEngine::testCreateWindow <");
}
// -----------------------------------------------------------------------------
// Ut_vmbxEngine::testshowVmbxQueryDialog
// Define selection dialog test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::testshowVmbxQueryDialog()
{
    qDebug("Ut_vmbxEngine::testshowVmbxQueryDialog >");
    QVERIFY(mQtUiHandler);
    TVmbxType vmbxtype(EVmbxVoice);
    TVmbxQueryMode queryMode(EVmbxDefineMode);
    QString number(tr(""));
    int result(0);
    mQtUiHandler->showVmbxQueryDialog(
            EVmbxVoice, EVmbxDefineMode, number, result);
    if ( KErrCancel == result ) {
        QCOMPARE(number, tr(""));
    }
    number = tr("123456");
    mQtUiHandler->showVmbxQueryDialog(
            EVmbxVoice, EVmbxChangeMode, number, result);

    if ( KErrCancel == result ) {
        QCOMPARE(number, tr(""));
    }
    qDebug("Ut_vmbxEngine::testshowVmbxQueryDialog <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxEngine::testshowDefineSelectionDialog
// Define selection dialog test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::testshowDefineSelectionDialog()
{
    qDebug("Ut_vmbxEngine::testshowDefineSelectionDialog >");
    QVERIFY(mQtUiHandler);
    TVmbxType vmbxtype;
    int result;
    mQtUiHandler->showDefineSelectionDialog(vmbxtype, result);
    if (KErrCancel == result &&  EVmbxNone != vmbxtype) {
        QFAIL("Ut_vmbxEngine::testshowDefineSelectionDialog:Cancel");
    } else if ( KErrNone == result &&  EVmbxNone == vmbxtype ){
        QFAIL("Ut_vmbxEngine::testshowDefineSelectionDialog:vmbxtype wrong");
    }
    qDebug("Ut_vmbxEngine::testshowDefineSelectionDialog:vmbxtype%d",
        vmbxtype);
    qDebug("Ut_vmbxEngine::testshowDefineSelectionDialog:result%d",
        result);
    qDebug("Ut_vmbxEngine::testshowDefineSelectionDialog <");
}


void Ut_vmbxEngine::testshowSaveEmptyNote_data()
{
    qDebug("Ut_vmbxEngine::testshowSaveEmptyNote_data >");
    QTest::addColumn<QString>("vmbx");
    QTest::newRow("CS voice") << "CsVoice";
    QTest::newRow("CS video") << "CsVideo";
    qDebug("Ut_vmbxEngine::testshowSaveEmptyNote_data <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxEngine::testshowSaveEmptyNote
// CreateVmbxMailbox test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::testshowSaveEmptyNote()
{
    qDebug("Ut_vmbxEngine::testshowSaveEmptyNote >");
    TRAPD(err,mUiHandler = CVmbxUiHandler::NewL());
    QVERIFY2(KErrNone == err, "create CVmbxUiHandler failed");
    TVmbxType vmbxtype(EVmbxVoice);
    QFETCH(QString, vmbx);
    if ("CsVoice" == vmbx) {
        vmbxtype = EVmbxVoice;
        qDebug("Ut_vmbxEngine::testshowSaveEmptyNote Voice");
    } else if ("CsVideo" == vmbx) { 
        vmbxtype = EVmbxVideo;
        qDebug("Ut_vmbxEngine::testshowSaveEmptyNote Video");
    }
    mUiHandler->ShowSaveEmptyNote(vmbxtype);

    delete mUiHandler;
    mUiHandler = 0;
    qDebug("Ut_vmbxEngine::testshowSaveEmptyNote <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxEngine::testDeleteWindow
// testDeleteWindow test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::testDeleteWindow()
{
    qDebug("Ut_vmbxEngine::testDeleteWindow >");
    delete mWindow;
    mWindow = 0;
    qDebug("Ut_vmbxEngine::testDeleteWindow <");
}

// -----------------------------------------------------------------------------
// Ut_vmbxEngine::testCreateVmbxMailbox
// CreateVmbxMailbox test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_vmbxEngine::testDeleteVmbxMailbox()
{
    qDebug("Ut_vmbxEngine::testDeleteVmbxMailbox >");
    delete mVmbxEngine;
    mVmbxEngine = 0;
    qDebug("Ut_vmbxEngine::testDeleteVmbxMailbox <");
}

// TODO: move main method to a separate file
        
// -----------------------------------------------------------------------------
// main()
// Main method implemented for directing test output to a file.
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
    {
    qDebug("ut_vmbxengine.cpp: main() >");
    QApplication app(argc, argv);

    qDebug("ut_vmbxengine.cpp: Ut_vmbxEngine");
    Ut_vmbxEngine tc;
    char *pass[3];  
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\vmbx\\ut_vmbxengine.txt";
    const int result = QTest::qExec(&tc, 3, pass);

    qDebug("ut_vmbxengine.cpp: main() <, result=%d", result);
    return result;
    }

// .moc file was not created during build for some reason, but looks like 
// we don't need it since this works.
//#include "ut_vmbxengine.moc"
//#include "moc_ut_vmbxengine.cpp"
