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
        FunctionButton = 0x1, // Backspace
        CallButton = 0x2      // Dial button
    };

public:
    explicit DialpadButton(QGraphicsItem *parent = 0 );
    virtual ~DialpadButton( );

    DialpadButtonType buttonType() const;
    void setButtonType(DialpadButtonType type);

protected:
    bool sceneEvent(QEvent *event);
    void polish(HbStyleParameters& params);

    void updatePrimitives();

private:
    DialpadButtonType mButtonType;
};

#endif // DIALPADBUTTON_H
