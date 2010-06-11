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
* Description: Custom button
*
*/


#include <hbevent.h>
#include <hbframeitem.h>

#include "dialpadbutton.h"

DialpadButton::DialpadButton(QGraphicsItem *parent)
    : HbPushButton(parent), mButtonType(NumericButton)
{
}

DialpadButton::~DialpadButton()
{
}

DialpadButton::DialpadButtonType DialpadButton::buttonType() const
{
    return mButtonType;
}

void DialpadButton::setButtonType(DialpadButtonType type)
{
    mButtonType = type;
}

void DialpadButton::changeEvent(QEvent *event)
{
    if (event->type() == HbEvent::ThemeChanged) {
        updatePrimitives();
    }

    HbPushButton::changeEvent(event);
}

bool DialpadButton::sceneEvent(QEvent *event)
{
    bool result = HbPushButton::sceneEvent(event);

    if (event->type() == QEvent::UngrabMouse) {
        if (isVisible() && isDown()) {
            // this is needed in situation, where
            // longpress launches a dialog (vmbx)
            // and button release event goes to
            // dialog (HbPopup grabs mouse).
            setDown(false);
            emit clicked();
            emit released();
        }
    }

    return result;
}

void DialpadButton::polish(HbStyleParameters& params)
{
    // HbPushButton::polish() prevents layouting
    // text and additional-text horizontally.
    HbAbstractButton::polish( params );
}

void DialpadButton::updatePrimitives()
{
    HbPushButton::updatePrimitives();

    HbFrameItem* frame =
        qgraphicsitem_cast<HbFrameItem*>(HbWidget::primitive("background"));

    if (!frame) {
        return;
    }

    QString graphicsName;

    if (!isEnabled()) {
        graphicsName = "qtg_fr_input_btn_function_disabled";
    } else if (isDown()) {
        if (buttonType()==CallButton) {
            graphicsName = "qtg_fr_btn_green_pressed";
        } else if (buttonType()==FunctionButton) {
            graphicsName = "qtg_fr_input_btn_function_pressed";
        } else {
            graphicsName = "qtg_fr_input_btn_keypad_pressed";
        }
    } else {
        if (buttonType()==CallButton) {
            graphicsName = "qtg_fr_btn_green_normal";
        } else if (buttonType()==FunctionButton) {
            graphicsName = "qtg_fr_input_btn_function_normal";
        } else {
            graphicsName = "qtg_fr_input_btn_keypad_normal";
        }
    }

    frame->frameDrawer().setFrameGraphicsName(graphicsName);
}
