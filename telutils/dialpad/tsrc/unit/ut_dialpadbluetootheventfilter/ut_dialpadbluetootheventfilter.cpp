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
#include "dialpadbluetootheventfilter.h"
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

// helper class
class KeyEventCatcher : public QObject
{
public:
    bool eventFilter(QObject* watched, QEvent * event)
    {
        Q_UNUSED(watched);

        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            mKeyPresses.append(keyEvent->key());
            return false;
        } else if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            mKeyReleases.append(keyEvent->key());
            return false;
        }
        return false;
    }

public:
    QList<int> mKeyPresses;
    QList<int> mKeyReleases;
};

// test cases
class ut_DialpadBluetoothEventFilter : public QObject
{
    Q_OBJECT

public:
    enum MouseEventType
    {
        Press,
        Release,
        Click
    };

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testLongPressAsteriskKey();
    void testShortAndLongPressAsteriskKey();

private:
    QGraphicsWidget* getWidgetByName(const QString& name);
    void mouseClickDialpad(int key, MouseEventType type=Click, bool pause=true);

private:
    HbMainWindow*  mMainWindow;
    Dialpad*       mDialpad;
    DialpadBluetoothEventFilter *mEventFilter;
    KeyEventCatcher* mKeyCatcher;
    QMap<int,QString> mKeyNames;
};

void ut_DialpadBluetoothEventFilter::initTestCase()
{
    mMainWindow = new HbMainWindow;

    mKeyCatcher = new KeyEventCatcher;
    mMainWindow->installEventFilter(mKeyCatcher);

    mDialpad = new Dialpad();
    mEventFilter = new DialpadBluetoothEventFilter(mDialpad, this);
    hbInstance->allMainWindows().at(0)->installEventFilter(mEventFilter);
            
    QRectF rect(mMainWindow->contentsRect());
    rect.setHeight(rect.height()*0.7);
    rect.moveTop((mMainWindow->contentsRect().height()-rect.height())/2);

    mDialpad->setPreferredSize(360,400);
    mDialpad->setPos(0,100);

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

void ut_DialpadBluetoothEventFilter::init()
{
    mService = QString("");
    mMessage = QString("");
    mXQServiceConstructed = false;
    mSendCalled = false;
}

void ut_DialpadBluetoothEventFilter::cleanupTestCase()
{
    delete mDialpad;
    delete mMainWindow;
    delete mKeyCatcher;
}

void ut_DialpadBluetoothEventFilter::cleanup()
{
    mKeyCatcher->mKeyPresses.clear();
    mKeyCatcher->mKeyReleases.clear();
    mDialpad->editor().setText(QString());
    QTest::qWait(WAIT_TIME); // delay between tests
}

QGraphicsWidget* ut_DialpadBluetoothEventFilter::getWidgetByName(const QString& name)
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

void ut_DialpadBluetoothEventFilter::mouseClickDialpad(int key, MouseEventType type, bool pause)
{
    QString name = mKeyNames.value(key);

    QGraphicsWidget* widget = getWidgetByName(name);

    if (widget) {
        QPointF widgetPos = widget->scenePos() + widget->rect().center();

        QPoint windowPos = mMainWindow->mapFromScene(widgetPos);

        if (type==Press) {
            QTest::mousePress(mMainWindow->viewport(), Qt::LeftButton, 0, windowPos);
        } else if (type==Release) {
            QTest::mouseRelease(mMainWindow->viewport(), Qt::LeftButton, 0, windowPos);
        } else {
            QTest::mouseClick(mMainWindow->viewport(), Qt::LeftButton, 0, windowPos);
        }

        if (pause) {
            QTest::qWait(WAIT_TIME);
        }
    } else {
        QFAIL("Button could not be accessed!");
    }
}

void ut_DialpadBluetoothEventFilter::testLongPressAsteriskKey()
{
    mDialpad->openDialpad();
    QTest::qWait(2*WAIT_TIME);

    // Basic long press
    mouseClickDialpad(Qt::Key_Asterisk, Press);
    QTest::qWait(2000);
    mouseClickDialpad(Qt::Key_Asterisk, Release);
    QTest::qWait(1000);
    QCOMPARE(mDialpad->editor().text(), QString(""));
    mDialpad->closeDialpad();
    
    QVERIFY(mXQServiceConstructed == true);
    QCOMPARE(mService, QString("com.nokia.services.btservices.ToggleBluetooth"));
    QCOMPARE(mMessage, QString("toggleBluetooth()"));
    QVERIFY(mSendCalled == true);
}

void ut_DialpadBluetoothEventFilter::testShortAndLongPressAsteriskKey()
{
    mDialpad->openDialpad();

    // Short press and long press shouldn't do anything
    mouseClickDialpad(Qt::Key_Asterisk, Press);
    mouseClickDialpad(Qt::Key_Asterisk, Release);
    mouseClickDialpad(Qt::Key_Asterisk, Press);
    QTest::qWait(2000);
    mouseClickDialpad(Qt::Key_Asterisk, Release);
    QCOMPARE(mDialpad->editor().text(), QString("**"));
    mDialpad->closeDialpad();	
}

DIALPAD_TEST_MAIN(ut_DialpadBluetoothEventFilter)
#include "ut_dialpadbluetootheventfilter.moc"

