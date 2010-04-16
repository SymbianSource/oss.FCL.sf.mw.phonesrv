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

#include "dialpadtest.h"
#include "dialpad.h"
#include "dialpadkeyhandler.h"
#include "dialpadvoicemailboxeventfilter.h"

bool mVmbxFilterConstructed;
Dialpad* mParameter1;
QObject* mParameter2;

#ifdef Q_OS_SYMBIAN
DialpadVoiceMailboxEventFilter::DialpadVoiceMailboxEventFilter(Dialpad* dialPad, QObject* parent)
{
    mVmbxFilterConstructed = true;
    mParameter1 = dialPad;
    mParameter2 = parent;
}
#endif

// test cases
class mt_KeyHandler : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

private:
    HbMainWindow*  mMainWindow;
    Dialpad*       mDialpad;
    DialpadKeyHandler *mKeyhandler;
};

void mt_KeyHandler::init()
{
    mVmbxFilterConstructed = false;
    mDialpad = new Dialpad(*mMainWindow);
    mKeyhandler = new DialpadKeyHandler(mDialpad, *hbInstance->allMainWindows().at(0), this);
    QVERIFY(mKeyhandler != NULL);
    QVERIFY(mVmbxFilterConstructed == true);
    QVERIFY(mParameter1 == mDialpad);
    QVERIFY(mParameter2 == this);
}

void mt_KeyHandler::cleanup()
{
    delete mDialpad;
    delete mMainWindow;
}


DIALPAD_TEST_MAIN(mt_KeyHandler)
#include "mt_keyhandler.moc"
