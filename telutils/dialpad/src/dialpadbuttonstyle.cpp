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
* Description: Custom button style
*
*/

#include <hbicon.h>
#include <hbstyleoptionpushbutton.h>
#include <hbframeitem.h>
#include <hbframedrawer.h>
#include "dialpadbuttonstyle.h"

DialpadButtonStyle::DialpadButtonStyle()
    : mButtonStyle(NormalButtonStyle)
{
}

DialpadButtonStyle::~DialpadButtonStyle()
{
}

void DialpadButtonStyle::updatePrimitive(
    QGraphicsItem *item,
    HbStyle::Primitive primitive,
    const QStyleOption *option ) const
{
    switch(primitive){
    case P_PushButton_background:{
        const HbStyleOptionPushButton *opt = 
            qstyleoption_cast<const HbStyleOptionPushButton *>(option);
        HbFrameItem *frameItem = qgraphicsitem_cast<HbFrameItem*>( item );
        if(!frameItem)
            return;

        frameItem->setZValue(-1.0);
        if(opt->backgroundFrameDrawer &&!(opt->backgroundFrameDrawer->isNull())) {
            frameItem->setFrameDrawer( (opt->backgroundFrameDrawer));
        } else {
            QString frameGraphicsName;
            QIcon::Mode mode = QIcon::Disabled;
            QIcon::State state = QIcon::Off;
            if (opt->state & QStyle::State_Enabled)
                mode = QIcon::Normal;
            if (opt->state & QStyle::State_Active)
                mode = QIcon::Active;
            if (opt->state & QStyle::State_Selected)
                mode = QIcon::Selected;
            if (opt->state & QStyle::State_On)
                state = QIcon::On;

            if (mode == QIcon::Disabled && state == QIcon::Off) {
                frameGraphicsName = "qtg_fr_input_btn_function_disabled";
            } else if (mode == QIcon::Normal && state == QIcon::On) {
                if (mButtonStyle==CallButtonStyle) {
                    frameGraphicsName = "qtg_fr_btn_green_pressed";
                } else if (mButtonStyle==NormalButtonStyle) {
                    frameGraphicsName = "qtg_fr_input_btn_keypad_pressed";
                } else {
                    frameGraphicsName = "qtg_fr_input_btn_function_pressed";
                }
            } else if (mode == QIcon::Selected && state == QIcon::Off) {
                frameGraphicsName = "qtg_fr_btn_highlight";
            } else {
                if (mButtonStyle==CallButtonStyle) {
                    frameGraphicsName = "qtg_fr_btn_green_normal";
                } else if (mButtonStyle==NormalButtonStyle) {
                    frameGraphicsName = "qtg_fr_input_btn_keypad_normal";
                } else {
                    frameGraphicsName = "qtg_fr_input_btn_function_normal";
                }
            }

            frameItem->frameDrawer().setFrameGraphicsName(frameGraphicsName);
            if (!opt->background.isNull()) {
                QString customName = opt->background.iconName(mode, state);
                frameItem->frameDrawer().setFrameGraphicsName(customName);
                frameItem->setGeometry(opt->rect);
                frameItem->frameDrawer().setFrameType(HbFrameDrawer::NinePieces);
                break;
            }
            frameItem->frameDrawer().setFrameType(HbFrameDrawer::NinePieces);
        }

        frameItem->setGeometry(opt->rect);
        break;
    }

    default:
        HbStyle::updatePrimitive(item,primitive,option);
        break;
    } // switch
}

void DialpadButtonStyle::setButtonStyle(ButtonStyle style)
{
    mButtonStyle = style;
}
