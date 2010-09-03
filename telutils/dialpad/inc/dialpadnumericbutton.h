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

#ifndef DIALPADNUMERICBUTTON_H
#define DIALPADNUMERICBUTTON_H

#include <hbicon.h>
#include <hbinputbutton.h>

class DialpadNumericButton : public HbInputButton
{
    Q_OBJECT

public:
    explicit DialpadNumericButton(int keyCode, const QPoint &position, const QSize &size = QSize(1, 1));
    virtual ~DialpadNumericButton();

    void setIcon(const HbIcon& icon);
    HbIcon& icon();

    void setText(const QString& text);
    QString text() const;

    void setSecondaryText(const QString& text);
    QString secondaryText() const;

    void setSecondary2ndRowText(const QString& text);
    QString secondary2ndRowText() const;

private:
    HbIcon mIcon;
    QString mText;
    QString mSecondaryText;
    QString mSecondaryTextRow2;
};

#endif // DIALPADNUMERICBUTTON_H
