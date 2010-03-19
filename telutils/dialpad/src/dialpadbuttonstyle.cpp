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
#include <hbiconitem.h>
#include <hbtextitem.h>
#include <hbcolorscheme.h>
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
                frameGraphicsName = "qtg_fr_btn_disabled";
            } else if (mode == QIcon::Normal && state == QIcon::On) {
                if (mButtonStyle==CallButtonStyle) {
                    frameGraphicsName = ":/qtg_fr_btn_green_pressed";
                } else if (mButtonStyle==NormalButtonStyle) {
                    frameGraphicsName = "qtg_fr_button_keypad_pressed";
                } else {
                    frameGraphicsName = "qtg_fr_button_function_pressed";
                }
            } else if (mode == QIcon::Selected && state == QIcon::Off) {
                frameGraphicsName = "qtg_fr_btn_highlight";
            } else {
                if (mButtonStyle==CallButtonStyle) {
                    frameGraphicsName = ":/qtg_fr_btn_green_normal";
                } else if (mButtonStyle==NormalButtonStyle) {
                    frameGraphicsName = "qtg_fr_button_keypad_normal";
                } else {
                    frameGraphicsName = "qtg_fr_button_function_normal";
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

    case P_PushButton_icon: {
        HbStyle::updatePrimitive(item,primitive,option);

        // override color (todo: via css, when supported by fw)
        const HbStyleOptionPushButton *opt =
            qstyleoption_cast<const HbStyleOptionPushButton *>(option);
        QIcon::Mode mode = QIcon::Disabled;
        QIcon::State state = QIcon::Off;
        if (opt->state & QStyle::State_Enabled)
            mode = QIcon::Normal;
        if (opt->state & QStyle::State_On)
            state = QIcon::On;

        if (mButtonStyle==CallButtonStyle &&
            mode == QIcon::Normal) {
            if (state==QIcon::On) {
                QColor color(HbColorScheme::color("answer_pressed"));
                setIconColor(item,color);
            } else {
                QColor color(HbColorScheme::color("answer_normal"));
                setIconColor(item,color);
            }
        } else if (mButtonStyle==NormalButtonStyle) {
            QColor color;
            color = HbColorScheme::color("input_button_normal");
            setIconColor(item,color);
        } else { // function button
            QColor color;
            color = HbColorScheme::color("input_function_normal");
            setIconColor(item,color);
        }
        break;
    }

    case P_PushButton_text: {
        HbStyle::updatePrimitive(item,primitive,option);
        // override color (todo: via css, when supported by fw)
        setTextColor(item);
        break;
    }

    case P_PushButton_additionaltext: {
        HbStyle::updatePrimitive(item,primitive,option);
        // override color (todo: via css, when supported by fw)
        setTextColor(item);
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

void DialpadButtonStyle::setTextColor(QGraphicsItem *item) const
{
    HbTextItem *textPrim = qgraphicsitem_cast<HbTextItem*>(item);
    if (textPrim) {
        QColor color;
        color = HbColorScheme::color("input_button_normal");
        if (color.isValid()) {
            textPrim->setTextColor(color);
        } else {
            textPrim->setTextColor(Qt::black);
        }
    }
}

void DialpadButtonStyle::setIconColor(
    QGraphicsItem *item,
    const QColor &color ) const
{
    HbIconItem *iconItem = qgraphicsitem_cast<HbIconItem*>(item);

    if (iconItem) {
        if (color.isValid()) {
            iconItem->setColor(color);
        } else {
            iconItem->setColor(Qt::black);
        }
    }
}
