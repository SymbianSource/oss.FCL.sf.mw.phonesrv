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
* Description: Extends HbInputButton
*
*/


#include "dialpadnumericbutton.h"

DialpadNumericButton::DialpadNumericButton(
    int keyCode, const QPoint &position, const QSize &size)
    : HbInputButton(keyCode, position, size)
{
}

DialpadNumericButton::~DialpadNumericButton()
{
}

void DialpadNumericButton::setIcon(const HbIcon& icon)
{
    mIcon = icon;
}

HbIcon& DialpadNumericButton::icon()
{
    return mIcon;
}

void DialpadNumericButton::setText(const QString& text)
{
    mText = text;
}

QString DialpadNumericButton::text() const
{
    return mText;
}

void DialpadNumericButton::setSecondaryText(const QString& text)
{
    mSecondaryText = text;
}

QString DialpadNumericButton::secondaryText() const
{
    return mSecondaryText;
}

void DialpadNumericButton::setSecondary2ndRowText(const QString& text)
{
    mSecondaryTextRow2 = text;
}

QString DialpadNumericButton::secondary2ndRowText() const
{
    return mSecondaryTextRow2;
}
