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
    QGraphicsWidget* getWidgetByName(const QString& name);
    void mouseClickDialpad(int key, bool pause=true);

private:
    HbMainWindow*  mMainWindow; 
    Dialpad*       mDialpad;
    DialpadEmergencyCallEventFilter *mEventFilter;
    QMap<int,QString> mKeyNames;
};

void ut_DialpadEmergencyCallEventFilter::initTestCase()
{
    mMainWindow = new HbMainWindow;

    mDialpad = new Dialpad();
    mEventFilter = new DialpadEmergencyCallEventFilter(mDialpad, this);
    //hbInstance->allMainWindows().at(0)->installEventFilter(mEventFilter);
    mMainWindow->installEventFilter(mEventFilter);
    
    QRectF rect(mMainWindow->contentsRect());
    rect.setHeight(rect.height()*0.7);
    rect.moveTop((mMainWindow->contentsRect().height()-rect.height())/2);

    mDialpad->setPreferredSize(mMainWindow->layoutRect().width(),
                               mMainWindow->layoutRect().height()/2);
    mDialpad->setPos(0,mMainWindow->layoutRect().height()/4);

    mKeyNames.insert(Qt::Key_1,"49");
    mKeyNames.insert(Qt::Key_2,"50");
    mKeyNames.insert(Qt::Key_3,"51");
    mKeyNames.insert(Qt::Key_4,"52");
    mKeyNames.insert(Qt::Key_5,"53");
    mKeyNames.insert(Qt::Key_6,"54");
    mKeyNames.insert(Qt::Key_7,"55");
    mKeyNames.insert(Qt::Key_8,"56");
    mKeyNames.insert(Qt::Key_9,"57");
    mKeyNames.insert(Qt::Key_Asterisk,"42");
    mKeyNames.insert(Qt::Key_0,"48");
    mKeyNames.insert(Qt::Key_NumberSign,"35");
    mKeyNames.insert(Qt::Key_Backspace,"16777219");
    mKeyNames.insert(Qt::Key_Yes,"16842753");

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
}

void ut_DialpadEmergencyCallEventFilter::cleanup()
{
    mDialpad->editor().setText(QString());
    QTest::qWait( WAIT_TIME ); // delay between tests
}

QGraphicsWidget* ut_DialpadEmergencyCallEventFilter::getWidgetByName(const QString& name)
{
    Q_ASSERT(mMainWindow!=0);

    QGraphicsWidget* widget = 0;

    QList<QGraphicsItem*> items = mMainWindow->scene()->items();
    foreach (QGraphicsItem* item, items) {
        if (item->isWidget()) {
            QGraphicsWidget *w = static_cast<QGraphicsWidget*>(item);
            if (w->objectName()==name) {
                widget = w;
            }
        }
    }

    return widget;
}

void ut_DialpadEmergencyCallEventFilter::mouseClickDialpad(int key, bool pause)
{
    QString name = mKeyNames.value(key);

    QGraphicsWidget* widget = getWidgetByName(name);

    if ( widget ) {
        QPointF widgetPos = widget->scenePos() +
                            widget->rect().center();

        QPoint windowPos = mMainWindow->mapFromScene( widgetPos );

        QTest::mouseClick( mMainWindow->viewport(), Qt::LeftButton,
                           0, windowPos );

        if (pause) {
            QTest::qWait( WAIT_TIME );
        }
    } else {
        QFAIL( "Button could not be accessed!" );
    }
}


void ut_DialpadEmergencyCallEventFilter::testEmergencyCallEventFilter()
{
    mDialpad->openDialpad();
    QTest::qWait(WAIT_TIME);
    mouseClickDialpad(Qt::Key_1);
    QTest::qWait(1000);
    mouseClickDialpad(Qt::Key_1);
    QTest::qWait(1000);
    mouseClickDialpad(Qt::Key_2);
    QTest::qWait(1000);
    mouseClickDialpad(Qt::Key_Yes);
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

