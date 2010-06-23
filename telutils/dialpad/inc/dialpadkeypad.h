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

#include <hbinputbuttongroup.h>

class DialpadInputField;
class DialpadButton;
class QSignalMapper;
class QGraphicsGridLayout;

class DialpadKeypad : public HbInputButtonGroup
{
    Q_OBJECT

public:
    explicit DialpadKeypad(
        const HbMainWindow& mainWindow,
        DialpadInputField& inputField,
        QGraphicsItem* parent=0);

    ~DialpadKeypad();

public:
    void setCallButtonEnabled(bool enabled);

    void resetButtons();

    DialpadButton& callButton() const;

protected slots:
    void setButtonTexts();

    void handleKeyClicked(int key);
    void sendKeyPressEvent(const QKeyEvent& event);
    void sendKeyReleaseEvent(const QKeyEvent& event);
    void sendLongPressEvent(const QKeyEvent& event);
    void handleKeyChangeEvent(const QKeyEvent& releaseEvent,
                              const QKeyEvent& pressEvent);

private:
    void postKeyEvent(QEvent::Type type, int key);
    void sendKeyEventToEditor(QEvent::Type type, int key);

private:
    const HbMainWindow& mMainWindow;
    DialpadInputField& mInputField;
    QSignalMapper* mKeyClickedSignalMapper;
    QMap<int,QChar> mGeneratedChar;
    int mPressedNumericKey;
    DialpadButton* mCallButton;
};

#endif // DIALPADKEYPAD_H
