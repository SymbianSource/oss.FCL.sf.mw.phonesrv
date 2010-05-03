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

#ifndef DIALPADBUTTON_H
#define DIALPADBUTTON_H

#include "hbpushbutton.h"

class DialpadButton : public HbPushButton
{
    Q_OBJECT

    Q_PROPERTY(DialpadButtonType buttonType READ buttonType WRITE setButtonType)
    Q_ENUMS(DialpadButtonType)

public:
    enum DialpadButtonType {
        NumericButton = 0x1,
        FunctionButton = 0x2,
        CallButton = 0x3
    };

public:
    explicit DialpadButton(QGraphicsItem *parent = 0 );
    virtual ~DialpadButton( );

    DialpadButtonType buttonType() const;
    void setButtonType(DialpadButtonType type);

protected:
    void changeEvent(QEvent *event);
    bool sceneEvent(QEvent *event);
    void polish(HbStyleParameters& params);

private:
    DialpadButtonType mButtonType;
};

#endif // DIALPADBUTTON_H
