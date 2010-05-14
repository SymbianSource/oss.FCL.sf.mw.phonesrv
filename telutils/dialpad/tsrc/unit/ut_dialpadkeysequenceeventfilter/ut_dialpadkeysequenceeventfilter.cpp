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
#include <HbMainWindow>
#include <HbLineEdit>
#ifdef Q_OS_SYMBIAN
#include "xqservicerequest.h"
#endif
#include "ut_dialpadkeysequenceeventfilter.h"
#include "dialpadtest.h"
#include "dialpadkeysequenceeventfilter.h"
#include "dialpad.h"

UT_DialpadKeySequenceEventFilter::UT_DialpadKeySequenceEventFilter()
{
    
}

UT_DialpadKeySequenceEventFilter::~UT_DialpadKeySequenceEventFilter()
{
    
}

void UT_DialpadKeySequenceEventFilter::init()
{
    initialize();
    
    SmcDefaultValue<QString>::SetL(QString());
    HbMainWindow *dummyWindow = NULL;
    m_dialPad = new Dialpad(*dummyWindow);
    m_eventFilter = new DialpadKeySequenceEventFilter(m_dialPad);
}

void UT_DialpadKeySequenceEventFilter::cleanup()
{
    reset();
    
    SmcDefaultValue<QString>::Reset();
    delete m_dialPad;
    m_dialPad = NULL;
    delete m_eventFilter;
    m_eventFilter = NULL;
}

#ifdef Q_OS_SYMBIAN
void UT_DialpadKeySequenceEventFilter::eventFilterValidKeySequence()
{
    const QString KValidKeySequence("*#1#");
    HbLineEdit lineEdit;
    EXPECT(Dialpad::editor).returns(&lineEdit);
    EXPECT(HbLineEdit::text).returns(KValidKeySequence);
    EXPECT(XQServiceRequest::send).returns(true);
    QKeyEvent keyEvent(
        QEvent::KeyRelease,
        Qt::Key_NumberSign,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    QVERIFY(verify());
}

void UT_DialpadKeySequenceEventFilter::eventFilterNotAKeyEvent()
{
    HbLineEdit lineEdit;
    EXPECT(Dialpad::editor).returns(&lineEdit);
    EXPECT(XQServiceRequest::send).times(0);
    QMouseEvent mouseEvent(
        QEvent::MouseMove,
        QPoint(),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &mouseEvent);
    QVERIFY(!filtered);
    QVERIFY(verify());
}

void UT_DialpadKeySequenceEventFilter::eventFilterNotAHashKey()
{
    HbLineEdit lineEdit;
    EXPECT(Dialpad::editor).returns(&lineEdit);
    EXPECT(XQServiceRequest::send).times(0);
    QKeyEvent keyEvent(
        QEvent::KeyRelease,
        Qt::Key_Escape,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    QVERIFY(verify());
}

void UT_DialpadKeySequenceEventFilter::eventFilterNotValidKeySequence()
{
    const QString KInvalidKeySequence1("*##");
    const QString KInvalidKeySequence2("#1234#");
    const QString KInvalidKeySequence3("**1234#");
    const QString KInvalidKeySequence4("*#1234*");
    
    EXPECT(XQServiceRequest::send).times(0);
    
    HbLineEdit lineEdit;
    EXPECT(Dialpad::editor).returns(&lineEdit);
    EXPECT(HbLineEdit::text).returns(KInvalidKeySequence1);
    QKeyEvent keyEvent(
        QEvent::KeyRelease,
        Qt::Key_NumberSign,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    
    EXPECT(Dialpad::editor).returns(&lineEdit);
    EXPECT(HbLineEdit::text).returns(KInvalidKeySequence2);
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    
    EXPECT(Dialpad::editor).returns(&lineEdit);
    EXPECT(HbLineEdit::text).returns(KInvalidKeySequence3);
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    
    EXPECT(Dialpad::editor).returns(&lineEdit);
    EXPECT(HbLineEdit::text).returns(KInvalidKeySequence4);
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    
    QVERIFY(verify());
}
#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    UT_DialpadKeySequenceEventFilter tc;
    return QTest::qExec(&tc, argc, argv);
}
