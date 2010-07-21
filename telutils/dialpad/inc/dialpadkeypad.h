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

#include <QTextLayout>
#include <QColor>
#include <hbinputbuttongroup.h>

class DialpadInputField;
class DialpadButton;
class DialpadNumericButton;
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
    void cancelButtonPress();

protected:
    void postKeyEvent(QEvent::Type type, int key);
    void sendKeyEventToEditor(QEvent::Type type, int key);

    inline DialpadNumericButton* button(int i) const;
    void updateButtonLabels();
    void updateColorArray();
    void updateIconColor();
    void updateTextLayouts(const QSizeF &size);
    void resolveTextContent(QList<QString> &content);
    void createTextLayouts(const QSizeF &size,
                           const QList<QString> &content);
    void layoutTextLines(const QSizeF &size,
                         QTextLayout &textLayout,
                         int state,
                         int type);

protected:
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);
    void setGeometry(const QRectF &rect);
    void changeEvent(QEvent *event);

private:
    enum ButtonState {
        Normal = 0,
        Pressed,
        StateCount
    };

    enum TextType {
        PrimaryText = 0,
        SecondaryText,
        TextTypeCount
    };

private:
    const HbMainWindow& mMainWindow;
    DialpadInputField& mInputField;
    QSignalMapper* mKeyClickedSignalMapper;
    QMap<int,QChar> mGeneratedChar;
    int mPressedNumericKey;
    DialpadButton* mCallButton;
    QList<QColor> mColors;
    QList<QTextLayout*> mTextLayouts;
    qreal mUnit;
    qreal mMaxPrimaryLineWidth;
};

#endif // DIALPADKEYPAD_H
