/*!
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
* Description:
*
*/

#include <QtGui>
#include <QtTest/QtTest>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbview.h>
#include <hblineedit.h>
#include <hbinstance.h>

#ifdef Q_OS_SYMBIAN
#include "xqservicerequest.h"
#endif

#include "dialpadtest.h"
#include "dialpadtestutil.h"
#include "dialpademergencycalleventfilter.h"
#include "dialpad.h"

const int WAIT_TIME = 300;
QString mService;
QString mMessage;
bool mXQServiceConstructed;
bool mSendCalled;

#ifdef Q_OS_SYMBIAN
XQServiceRequest::XQServiceRequest(const QString& service, const QString& message, const bool& synchronous) { mService=service; mMessage=message; mXQServiceConstructed=true; }
XQServiceRequest::~XQServiceRequest() {}
bool XQServiceRequest::send(QVariant& retValue) { mSendCalled=true; return true; }
void XQServiceRequest::addArg(const QVariant& v) {}
#endif

// test cases
class ut_DialpadEmergencyCallEventFilter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();

    // These are actual emergency call event filter unit tests
    void testEmergencyCallEventFilter();

private:
    HbMainWindow*  mMainWindow; 
    Dialpad*       mDialpad;
    DialpadEmergencyCallEventFilter *mEventFilter;
    DialpadTestUtil* mUtil;
};

void ut_DialpadEmergencyCallEventFilter::initTestCase()
{
    mMainWindow = new HbMainWindow;

    mDialpad = new Dialpad();
    mEventFilter = new DialpadEmergencyCallEventFilter(mDialpad, this);
    //hbInstance->allMainWindows().at(0)->installEventFilter(mEventFilter);
    mMainWindow->installEventFilter(mEventFilter);

    mUtil = new DialpadTestUtil(*mMainWindow);
    
    QRectF rect(mMainWindow->contentsRect());
    rect.setHeight(rect.height()*0.7);
    rect.moveTop((mMainWindow->contentsRect().height()-rect.height())/2);

    mDialpad->setPreferredSize(mMainWindow->layoutRect().width(),
                               mMainWindow->layoutRect().height()/2);
    mDialpad->setPos(0,mMainWindow->layoutRect().height()/4);

    mMainWindow->show();
    mDialpad->show();
    mDialpad->hide();
}

void ut_DialpadEmergencyCallEventFilter::init()
{
    mService = QString("");
    mMessage = QString("");
    mXQServiceConstructed = false;
    mSendCalled = false;
}

void ut_DialpadEmergencyCallEventFilter::cleanupTestCase()
{
    delete mDialpad;
    delete mMainWindow;
    delete mUtil;
}

void ut_DialpadEmergencyCallEventFilter::cleanup()
{
    mDialpad->editor().setText(QString());
    QTest::qWait( WAIT_TIME ); // delay between tests
}


void ut_DialpadEmergencyCallEventFilter::testEmergencyCallEventFilter()
{
    mDialpad->openDialpad();
    QTest::qWait(WAIT_TIME);
    mUtil->mouseClickDialpad(Qt::Key_1);
    QTest::qWait(1000);
    mUtil->mouseClickDialpad(Qt::Key_1);
    QTest::qWait(1000);
    mUtil->mouseClickDialpad(Qt::Key_2);
    QTest::qWait(1000);
    mUtil->mouseClickDialpad(Qt::Key_Yes);
    QTest::qWait(1000);
    
    mDialpad->closeDialpad();

#ifdef Q_OS_SYMBIAN
    QVERIFY(mXQServiceConstructed == true);
    QVERIFY(mSendCalled == true);
    QCOMPARE(mService, QString("com.nokia.symbian.ICallDial"));
    QCOMPARE(mMessage, QString("dial(QString)"));
#endif
}

DIALPAD_TEST_MAIN(ut_DialpadEmergencyCallEventFilter)
#include "ut_dialpademergencycalleventfilter.moc"

