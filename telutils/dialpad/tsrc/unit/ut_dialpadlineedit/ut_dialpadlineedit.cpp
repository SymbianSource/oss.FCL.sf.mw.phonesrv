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
#include <qevent.h>
#include "ut_dialpadlineedit.h"
#include "testdialpadlineedit.h"
#include "phonesrvtestmain.h"


UT_dialpadlineedit::UT_dialpadlineedit()
    : 
    m_classUt(0)
{
    
}


UT_dialpadlineedit::~UT_dialpadlineedit()
{
    delete m_classUt;
}


void UT_dialpadlineedit::init()
{
    initialize();
    
    m_classUt = new TestDialpadLineEdit();
}


void UT_dialpadlineedit::cleanup()
{
    reset();
    
    delete m_classUt;
    m_classUt = 0;
}


void UT_dialpadlineedit::t_focusOutEvent()
{
    EXPECT(HbWidget, focusOutEvent);
    EXPECT(HbAbstractEdit, focusOutEvent).times(0);
    QFocusEvent event(QEvent::FocusOut);
    m_classUt->testFocusOutEvent(&event);
    QVERIFY(verify());
}


void UT_dialpadlineedit::t_focusInEvent()
{
    EXPECT(HbWidget, focusInEvent);
    EXPECT(HbAbstractEdit, focusInEvent).times(0);
    QFocusEvent event(QEvent::FocusIn);
    m_classUt->testFocusInEvent(&event);
    QVERIFY(verify());
}

PHONESRV_TEST_MAIN(UT_dialpadlineedit)
