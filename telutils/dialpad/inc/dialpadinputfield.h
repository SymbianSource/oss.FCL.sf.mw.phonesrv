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
* Description: Input field
*
*/

#ifndef DIALPADINPUTFIELD_H
#define DIALPADINPUTFIELD_H

#include <hbwidget.h>

class HbLineEdit;
class DialpadButtonStyle;
class DialpadButton;

class DialpadInputField : public HbWidget
{
    Q_OBJECT

public:
    DialpadInputField(QGraphicsItem* parent=0);
    ~DialpadInputField();

    HbLineEdit& editor() const;

    DialpadButton& backspaceButton() const;

protected:
    QSizeF sizeHint(Qt::SizeHint which,
                    const QSizeF & constraint = QSizeF()) const;

private slots:
    void onEditorContentChanged();

private:
    HbLineEdit* mNumberEditor;
    DialpadButton* mBackspace;
    int mHeight;
};

#endif // DIALPADINPUTFIELD_H
