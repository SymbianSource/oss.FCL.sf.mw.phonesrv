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
#include "dialpadnumericbutton.h"

class ut_DialpadNumericButton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSetText();
    void testSetSecondaryText();
    void testSetSecondary2ndRowText();
    void testSetIcon();

private:
    DialpadNumericButton *mButton;
};

void ut_DialpadNumericButton::initTestCase()
{
    mButton = new DialpadNumericButton(0,QPoint(1,1),QSize(1,1));
}

void ut_DialpadNumericButton::cleanupTestCase()
{
    delete mButton;
}

void ut_DialpadNumericButton::testSetText()
{
    QVERIFY(mButton->text().isNull());
    mButton->setText(QLatin1String("Primary text"));
    QVERIFY(mButton->text() == QLatin1String("Primary text"));
}

void ut_DialpadNumericButton::testSetSecondaryText()
{
    QVERIFY(mButton->secondaryText().isNull());
    mButton->setSecondaryText(QLatin1String("Secondary text"));
    QVERIFY(mButton->secondaryText() == QLatin1String("Secondary text"));
}

void ut_DialpadNumericButton::testSetIcon()        
{
    HbIcon icon(QLatin1String("dummy.svg"));

    QVERIFY(mButton->icon().isNull());
    mButton->setIcon(icon);
    mButton->icon().setColor(Qt::white);
    QVERIFY(mButton->icon().iconName() == QLatin1String("dummy.svg"));
    QVERIFY(mButton->icon().color() == Qt::white);
}

void ut_DialpadNumericButton::testSetSecondary2ndRowText()
{
    QVERIFY(mButton->secondary2ndRowText().isNull());
    mButton->setSecondary2ndRowText(QLatin1String("Secondary 2nd row text"));
    QVERIFY(mButton->secondary2ndRowText() == QLatin1String("Secondary 2nd row text"));
}

DIALPAD_TEST_MAIN(ut_DialpadNumericButton)
#include "ut_dialpadnumericbutton.moc"
