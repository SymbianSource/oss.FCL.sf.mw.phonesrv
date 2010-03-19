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

#ifndef DIALPADBUTTONSTYLE_H_
#define DIALPADBUTTONSTYLE_H_

#include <hbstyle.h>

class DialpadButtonStyle : public HbStyle
    {
public:
    enum ButtonStyle
    {
        NormalButtonStyle,
        FunctionButtonStyle,
        CallButtonStyle
    };

public:
    explicit DialpadButtonStyle();
    virtual ~DialpadButtonStyle();

    void updatePrimitive(
        QGraphicsItem *item,
        HbStyle::Primitive primitive,
        const QStyleOption *option ) const;

    void setButtonStyle(ButtonStyle style);

private:
    void setTextColor(QGraphicsItem *item) const;
    void setIconColor(QGraphicsItem *item, const QColor &color) const;

private:
    int mButtonStyle;
    };

#endif /* DIALPADBUTTONSTYLE_H_ */
