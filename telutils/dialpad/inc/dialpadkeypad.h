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
* Description: Dialpad keypad
*
*/

#ifndef DIALPADKEYPAD_H
#define DIALPADKEYPAD_H

#include <hbwidget.h>

class DialpadButton;
class DialpadButtonStyle;
class DialpadInputField;
class DialpadButton;
class QSignalMapper;
class QGraphicsGridLayout;

const int DialpadButtonCount = 13;

class DialpadKeypad : public HbWidget
{
    Q_OBJECT

public:
    explicit DialpadKeypad(
        DialpadInputField& inputField,
        QGraphicsItem* parent=0);

    ~DialpadKeypad();

public:
    void setLongPressDuration(int duration);

    void setCallButtonEnabled(bool enabled);

    void createButtonGrid();

protected slots:
    void setButtonTexts();
    void handleKeyPressed(int key);
    void handleKeyClicked(int key);
    void handleKeyReleased(int key);
    void handleLongPress();

protected:
    void showEvent(QShowEvent *event);

private:
    void postKeyEvent(QEvent::Type type, int key);
    void sendKeyEventToEditor(QEvent::Type type, int key);
    inline bool isNumericKey(int key);

private:
    DialpadInputField& mInputField;
    QGraphicsGridLayout* mGridLayout;
    DialpadButton* mButtons[DialpadButtonCount];
    DialpadButtonStyle* mNormalButtonStyle;
    DialpadButtonStyle* mCallButtonStyle;
    QSignalMapper* mKeyPressedSignalMapper;
    QSignalMapper* mKeyReleasedSignalMapper;
    QSignalMapper* mKeyClickedSignalMapper;
    QMap<int,QChar> mGeneratedChar;
    int mPressedNumericKey;
    QTimer* mLongPressTimer;
    int mLongPressDuration;
};

#endif // DIALPADKEYPAD_H
