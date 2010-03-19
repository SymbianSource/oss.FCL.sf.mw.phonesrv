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
#include <hbmainwindow.h>
#include <hbframeitem.h>
#include <hbiconitem.h>
#include <hbtextitem.h>
#include <hbstyle.h>
#include <hbstyleoptionpushbutton.h>
#include <hbcolorscheme.h>

#include "dialpadtest.h"
#include "dialpadbuttonstyle.h"

class ut_DialpadButtonStyle : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testPushButtonFrame();
    void testTextColor();
    void testAdditionalTextColor();
    void testPushButtonIcon();

private:
    DialpadButtonStyle *mStyle;
};

void ut_DialpadButtonStyle::initTestCase()
{
    mStyle = new DialpadButtonStyle();
}

void ut_DialpadButtonStyle::cleanupTestCase()
{
    delete mStyle;
}

void ut_DialpadButtonStyle::testPushButtonFrame()
{
    HbFrameItem* item = new HbFrameItem();
    HbStyleOptionPushButton option;
    option.backgroundFrameDrawer = 0;

    // Enabled ON
    option.state |= QStyle::State_On;
    option.state |= QStyle::State_Enabled;

    mStyle->setButtonStyle(DialpadButtonStyle::NormalButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_button_keypad_pressed");

    mStyle->setButtonStyle(DialpadButtonStyle::FunctionButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_button_function_pressed");

    mStyle->setButtonStyle(DialpadButtonStyle::CallButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()==":/qtg_fr_btn_green_pressed");

    // Enabled OFF
    option.state &= ~QStyle::State_On;
    option.state |= QStyle::State_Off;

    mStyle->setButtonStyle(DialpadButtonStyle::NormalButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_button_keypad_normal");

    mStyle->setButtonStyle(DialpadButtonStyle::FunctionButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_button_function_normal");

    mStyle->setButtonStyle(DialpadButtonStyle::CallButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()==":/qtg_fr_btn_green_normal");

    // Disabled
    option.state &= ~QStyle::State_Enabled;
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_disabled");

    // Selected
    option.state |= QStyle::State_Selected;
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_background,&option);
    QVERIFY(item->frameDrawer().frameGraphicsName()=="qtg_fr_btn_highlight");

    delete item;

    // exception type
    HbIconItem* item2 = new HbIconItem();
    mStyle->updatePrimitive(item2,HbStyle::P_PushButton_background,&option);
    delete item2;
}

void ut_DialpadButtonStyle::testTextColor()
{
    HbTextItem* item = new HbTextItem();
    HbStyleOptionPushButton option;
    option.backgroundFrameDrawer = 0;
    QColor color;
    color = HbColorScheme::color("input_button_normal");

    // Enabled ON
    option.state |= QStyle::State_On;
    option.state |= QStyle::State_Enabled;

    mStyle->updatePrimitive(item,HbStyle::P_PushButton_text,&option);

    QVERIFY(item->textColor()==color);
}

void ut_DialpadButtonStyle::testAdditionalTextColor()
{
    HbTextItem* item = new HbTextItem();
    HbStyleOptionPushButton option;
    option.backgroundFrameDrawer = 0;
    QColor color;
    color = HbColorScheme::color("input_button_normal");

    // Enabled ON
    option.state |= QStyle::State_On;
    option.state |= QStyle::State_Enabled;

    mStyle->updatePrimitive(item,HbStyle::P_PushButton_additionaltext,&option);

    QVERIFY(item->textColor()==color);
}

void ut_DialpadButtonStyle::testPushButtonIcon()
{
    HbIconItem* item = new HbIconItem();
    HbStyleOptionPushButton option;
    QColor funcColor(HbColorScheme::color("input_function_normal"));
    QColor normColor(HbColorScheme::color("input_button_normal"));
    QColor answerNormal(HbColorScheme::color("answer_normal"));
    QColor answerPressed(HbColorScheme::color("answer_pressed"));

    // Normal
    option.state |= QStyle::State_Enabled;
    option.state |= QStyle::State_Off;

    mStyle->setButtonStyle(DialpadButtonStyle::NormalButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_icon,&option);
    QVERIFY(item->color()==normColor);

    mStyle->setButtonStyle(DialpadButtonStyle::FunctionButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_icon,&option);
    QVERIFY(item->color()==funcColor);

    mStyle->setButtonStyle(DialpadButtonStyle::CallButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_icon,&option);
    QVERIFY(item->color()==answerNormal);

    // Pressed
    option.state |= QStyle::State_On;
    option.state &= ~QStyle::State_Off;
    mStyle->setButtonStyle(DialpadButtonStyle::CallButtonStyle);
    mStyle->updatePrimitive(item,HbStyle::P_PushButton_icon,&option);
    QVERIFY(item->color()==answerPressed);
}

DIALPAD_TEST_MAIN(ut_DialpadButtonStyle)
#include "ut_dialpadbuttonstyle.moc"
