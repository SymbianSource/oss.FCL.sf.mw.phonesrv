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
#include "dialpadvoicemailboxeventfilter.h"
#include "dialpad.h"
#include "dialpadsymbianwrapper.h"

const int WAIT_TIME = 300;
QString mService;
QString mMessage;
bool mXQServiceConstructed;
bool mSendCalled;

DialpadSymbianWrapper::DialpadSymbianWrapper(QObject *parent) : d_ptr(NULL) {}
DialpadSymbianWrapper::~DialpadSymbianWrapper() {}
int DialpadSymbianWrapper::getMailboxNumber(QString &vmbxNumber) { vmbxNumber=QString("12345678"); return 0; }
int DialpadSymbianWrapper::defineMailboxNumber(QString &vmbxNumber) { return 0; }

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
class ut_DialpadVoiceMailboxEventFilter : public QObject
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
    void cleanupTestCase();
    void cleanup();

    // These are actual voice mailbox event filter unit tests
    void testNumericKeyOneLongPress();
    void testNumericKeyOneShortPress();
    void testNumericKeyOneShortThenLongPress();

private:
    QGraphicsWidget* getWidgetByName(const QString& name);
    void mouseClickDialpad(int key, MouseEventType type=Click,
                           bool pause=true);

private:
    HbMainWindow*  mMainWindow;
    Dialpad*       mDialpad;
    DialpadVoiceMailboxEventFilter *mEventFilter;
    KeyEventCatcher* mKeyCatcher;
    QMap<int,QString> mKeyNames;
};

void ut_DialpadVoiceMailboxEventFilter::initTestCase()
{
    mMainWindow = new HbMainWindow;

    mKeyCatcher = new KeyEventCatcher;
    mMainWindow->installEventFilter(mKeyCatcher);

    mDialpad = new Dialpad();
    mEventFilter = new DialpadVoiceMailboxEventFilter(mDialpad, this);
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

void ut_DialpadVoiceMailboxEventFilter::init()
{
    mService = QString("");
    mMessage = QString("");
    mXQServiceConstructed = false;
    mSendCalled = false;
}

void ut_DialpadVoiceMailboxEventFilter::cleanupTestCase()
{
    delete mDialpad;
    delete mMainWindow;
    delete mKeyCatcher;
}

void ut_DialpadVoiceMailboxEventFilter::cleanup()
{
    mKeyCatcher->mKeyPresses.clear();
    mKeyCatcher->mKeyReleases.clear();
    mDialpad->editor().setText(QString());
    QTest::qWait( WAIT_TIME ); // delay between tests
}

QGraphicsWidget* ut_DialpadVoiceMailboxEventFilter::getWidgetByName(const QString& name)
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

void ut_DialpadVoiceMailboxEventFilter::mouseClickDialpad(int key, MouseEventType type, bool pause)
{
    QString name = mKeyNames.value(key);

    QGraphicsWidget* widget = getWidgetByName(name);

    if ( widget ) {
        QPointF widgetPos = widget->scenePos() +
                            widget->rect().center();

        QPoint windowPos = mMainWindow->mapFromScene( widgetPos );

        if (type==Press) {
            QTest::mousePress( mMainWindow->viewport(), Qt::LeftButton,
                               0, windowPos );
        } else if (type==Release) {
            QTest::mouseRelease( mMainWindow->viewport(), Qt::LeftButton,
                                 0, windowPos );
        } else {
            QTest::mouseClick( mMainWindow->viewport(), Qt::LeftButton,
                               0, windowPos );
        }

        if (pause) {
            QTest::qWait( WAIT_TIME );
        }
    } else {
        QFAIL( "Button could not be accessed!" );
    }
}


void ut_DialpadVoiceMailboxEventFilter::testNumericKeyOneLongPress()
{
    mDialpad->openDialpad();
    QTest::qWait(WAIT_TIME);
    mouseClickDialpad(Qt::Key_1, Press);
    QTest::qWait(2000);
    mouseClickDialpad(Qt::Key_1, Release);
    QTest::qWait(1000);
    QCOMPARE(mDialpad->editor().text(), QString(""));
    mDialpad->closeDialpad();

#ifdef Q_OS_SYMBIAN
    QVERIFY(mXQServiceConstructed == true);
    QVERIFY(mSendCalled == true);
    QCOMPARE(mService, QString("com.nokia.symbian.ICallDial"));
    QCOMPARE(mMessage, QString("dial(QString)"));
#endif
}

void ut_DialpadVoiceMailboxEventFilter::testNumericKeyOneShortPress()
{
    mDialpad->openDialpad();
    QTest::qWait(WAIT_TIME);
    mouseClickDialpad(Qt::Key_1, Press);
    mouseClickDialpad(Qt::Key_1, Release);
    // Check that character '1' is in editor.
    QCOMPARE(mDialpad->editor().text(), QString("1"));
}

void ut_DialpadVoiceMailboxEventFilter::testNumericKeyOneShortThenLongPress()
{
    // Then one short and one long press
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );
    mouseClickDialpad(Qt::Key_1, Press);
    mouseClickDialpad(Qt::Key_1, Release);
    mouseClickDialpad(Qt::Key_1, Press);
    QTest::qWait(2000);
    mouseClickDialpad(Qt::Key_1, Release);
    QTest::qWait(1000);
    QVERIFY(mDialpad->editor().text()=="11");
    mDialpad->closeDialpad();
}

DIALPAD_TEST_MAIN(ut_DialpadVoiceMailboxEventFilter)
#include "ut_dialpadvoicemailboxeventfilter.moc"

