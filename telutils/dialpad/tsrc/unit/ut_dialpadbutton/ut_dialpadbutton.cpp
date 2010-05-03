/*!
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
* Description:
*
*/

#include <QtGui>
#include <QtTest/QtTest>

#include <hbapplication.h>

#include "dialpadtest.h"
#include "dialpadbutton.h"

class ut_DialpadButton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testButtonType();

private:
    DialpadButton *mButton;
};

void ut_DialpadButton::initTestCase()
{
    mButton = new DialpadButton();
}

void ut_DialpadButton::cleanupTestCase()
{
    delete mButton;
}

void ut_DialpadButton::testButtonType()
{
    QVERIFY(mButton->buttonType()==DialpadButton::NumericButton);
    mButton->setButtonType(DialpadButton::FunctionButton);
    QVERIFY(mButton->buttonType()==DialpadButton::FunctionButton);
}

DIALPAD_TEST_MAIN(ut_DialpadButton)
#include "ut_dialpadbutton.moc"
