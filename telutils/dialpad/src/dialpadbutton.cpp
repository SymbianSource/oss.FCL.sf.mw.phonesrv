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

#include <hbtextitem.h>
#include <hbiconitem.h>
#include <hbstyle.h>
#include <hbcolorscheme.h>
#include <hbevent.h>

#include "dialpadbutton.h"

DialpadButton::DialpadButton(QGraphicsItem *parent)
    : HbPushButton(parent)
{
}

DialpadButton::~DialpadButton()
{
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
        if (isDown()) {
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


