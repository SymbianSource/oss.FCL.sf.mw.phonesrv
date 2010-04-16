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

#include <QtGui>
#include <hbinstance.h>
#include <hbinputkeymapfactory.h>
#include <hbinputkeymap.h>
#include <hbinpututils.h>
#include <hbinputsettingproxy.h>
#include <hbinputlanguage.h>
#include <hbapplication.h>
#include <hblineedit.h>

#include "dialpadkeypad.h"
#include "dialpadbuttonstyle.h"
#include "dialpadbutton.h"
#include "dialpadinputfield.h"

static const int DialpadRowCount = 5;
static const int DialpadColumnCount = 3;
static const QString handsetIcon("qtg_mono_call");
static const QString vmbxIcon("qtg_mono_voice_mailbox");

static const int DialpadButtonToKeyCodeTable[DialpadButtonCount] =
{
    Qt::Key_1,        Qt::Key_2,      Qt::Key_3,
    Qt::Key_4,        Qt::Key_5,      Qt::Key_6,
    Qt::Key_7,        Qt::Key_8,      Qt::Key_9,
    Qt::Key_Asterisk, Qt::Key_0,      Qt::Key_NumberSign,
                      Qt::Key_Yes
    // Qt::Key_BackSpace is in input field
};

DialpadKeypad::DialpadKeypad(
    const HbMainWindow& mainWindow,
    DialpadInputField& inputField,
    QGraphicsItem* parent) :
    HbWidget(parent),
    mMainWindow(mainWindow),
    mInputField(inputField),
    mLongPressDuration(0)
{
    // create signal mappers
    mKeyPressedSignalMapper = new QSignalMapper(this);
    connect(mKeyPressedSignalMapper,SIGNAL(mapped(int)),
            SLOT(handleKeyPressed(int)));
    mKeyClickedSignalMapper = new QSignalMapper(this);
    connect(mKeyClickedSignalMapper,SIGNAL(mapped(int)),
            SLOT(handleKeyClicked(int)));
    mKeyReleasedSignalMapper = new QSignalMapper(this);
    connect(mKeyReleasedSignalMapper,SIGNAL(mapped(int)),
            SLOT(handleKeyReleased(int)));

    connect(&mInputField.backspaceButton(),SIGNAL(clicked()),
            mKeyClickedSignalMapper,SLOT(map()));
    mKeyClickedSignalMapper->setMapping(&mInputField.backspaceButton(),
                                        Qt::Key_Backspace);

    // create keypad
    mNormalButtonStyle = new DialpadButtonStyle();
    mCallButtonStyle = new DialpadButtonStyle();
    mCallButtonStyle->setButtonStyle(DialpadButtonStyle::CallButtonStyle);

    for (int i = 0; i < DialpadButtonCount; i++) {
        int keyCode = DialpadButtonToKeyCodeTable[i];

        DialpadButton* button = new DialpadButton(this);
        mButtons[i] = button;

        button->setStretched(true);
        button->setFocusPolicy(Qt::NoFocus);
        button->setFlag(QGraphicsItem::ItemIsFocusable,false);

        QString buttonName;
        buttonName.setNum(keyCode);
        button->setObjectName(buttonName);

        if (keyCode==Qt::Key_Yes) {
            button->setStyle(mCallButtonStyle);
            HbIcon callIcon(handsetIcon); // todo correct icon
            button->setIcon(callIcon);
        } else {
            button->setStyle(mNormalButtonStyle);
        }

        if (keyCode==Qt::Key_1) {
            HbIcon mboxIcon(vmbxIcon);
            button->setIcon(mboxIcon);
        }

        // for Yes-key clicked() signal is enough
        if (keyCode!=Qt::Key_Yes) {
            connect(button,SIGNAL(pressed()),
                    mKeyPressedSignalMapper,SLOT(map()));
            mKeyPressedSignalMapper->setMapping(button,keyCode);

            connect(button,SIGNAL(released()),
                    mKeyReleasedSignalMapper,SLOT(map()));
            mKeyReleasedSignalMapper->setMapping(button,keyCode);
        }

        connect(button,SIGNAL(clicked()),mKeyClickedSignalMapper,SLOT(map()));
        mKeyClickedSignalMapper->setMapping(button,keyCode);
    }

    // set button texts
    setButtonTexts();
    // update button texts when input language is changed
    connect(HbInputSettingProxy::instance(),
            SIGNAL(globalInputLanguageChanged(HbInputLanguage)),
            this,SLOT(setButtonTexts()));

    createButtonGrid();

    // timer to handle long press
    mLongPressTimer = new QTimer(this);
    mLongPressTimer->setSingleShot(true);
    connect(mLongPressTimer,SIGNAL(timeout()),SLOT(handleLongPress()));
}

DialpadKeypad::~DialpadKeypad()
{
    delete mCallButtonStyle;
    delete mNormalButtonStyle;
}

void DialpadKeypad::createButtonGrid()
{
    // button grid
    mGridLayout = new QGraphicsGridLayout;

    // 12 numeric buttons
    int i=0;
    for (int row = 0; row < DialpadRowCount-1; row++) {
        for (int col = 0; col < DialpadColumnCount; col++) {
           mGridLayout->addItem(mButtons[i],row,col);
           i++;
        }
    }

    // call button take the last row
    mGridLayout->addItem(mButtons[12],4,0,1,3);
    mGridLayout->setSpacing(0);
    mGridLayout->setContentsMargins(0,0,0,0);

    setLayout(mGridLayout);
}

void DialpadKeypad::setButtonTexts()
{
    HbInputLanguage inputLanguage =
        HbInputSettingProxy::instance()->globalInputLanguage();
    const HbKeymap *keymap =
        HbKeymapFactory::instance()->keymap(inputLanguage.language());

    mGeneratedChar.clear();

    if (keymap) {
        for (int i = 0; i < DialpadButtonCount-1; i++) {
            int keyCode = DialpadButtonToKeyCodeTable[i];

            if (keyCode == Qt::Key_Asterisk) {
                // asterisk is not localized
                QChar asterisk('*');
                mButtons[i]->setText(asterisk);
                mButtons[i]->setAdditionalText("+");
                mGeneratedChar.insert(Qt::Key_Asterisk, asterisk);
                continue;
            }

            if (keyCode == Qt::Key_NumberSign) {
                // number sign is not localized
                QChar numberSign('#');
                mButtons[i]->setText(numberSign);
                mButtons[i]->setAdditionalText(" ");
                mGeneratedChar.insert(Qt::Key_NumberSign, numberSign);
                continue;
            }

            int index = i;
            if (keyCode==Qt::Key_0) {
                index = i-1;
            }

            const HbMappedKey *key =
                keymap->keyForIndex(HbKeyboardVirtual12Key, index);

            if (key) {
                QChar numberChar =
                    HbInputUtils::findFirstNumberCharacterBoundToKey(
                        key,
                        inputLanguage.language());

                // button text
                mButtons[i]->setText(numberChar);
                mGeneratedChar.insert(keyCode,numberChar);

                // additional text (letters)
                int numberOfCharacters;
                if (keyCode==Qt::Key_7 || keyCode == Qt::Key_9) {
                    numberOfCharacters = 4;
                } else if (keyCode==Qt::Key_0||keyCode==Qt::Key_1) {
                    numberOfCharacters = 0;
                } else {
                    numberOfCharacters = 3;
                }

                QString characters = key->characters(HbModifierNone);

                if (numberOfCharacters==0 && keyCode!=Qt::Key_1) {
                    mButtons[i]->setAdditionalText(" ");
                } else {
                    mButtons[i]->setAdditionalText(
                        characters.left(numberOfCharacters));
                }
            }
        }
    }
}

void DialpadKeypad::handleKeyPressed(int key)
{
    // Editor is updated on key release (clicked()) or on long press,
    // to prevent editor being updated during swipe.
    mPressedNumericKey = key;
    mLongPressTimer->start(mLongPressDuration);

    postKeyEvent(QEvent::KeyPress, key);
}

void DialpadKeypad::handleKeyClicked(int key)
{
    if (!isNumericKey(key)) {
        postKeyEvent(QEvent::KeyPress, key);
        postKeyEvent(QEvent::KeyRelease, key);
    } else if (mPressedNumericKey) {
        // update editor: generate key press event.
        sendKeyEventToEditor(QEvent::KeyPress, key);
    }
}

void DialpadKeypad::handleKeyReleased(int key)
{
    mLongPressTimer->stop();

    postKeyEvent(QEvent::KeyRelease, key);
}

void DialpadKeypad::postKeyEvent(QEvent::Type type, int key)
{
    QKeyEvent *keyEvent = new QKeyEvent(type, key, Qt::NoModifier);
    HbApplication::postEvent(const_cast<HbMainWindow*>(&mMainWindow),keyEvent);
}

void DialpadKeypad::sendKeyEventToEditor(QEvent::Type type, int key)
{
    QKeyEvent keyEvent(type, key, Qt::NoModifier, mGeneratedChar.value(key));
    HbApplication::sendEvent(&mInputField.editor(), &keyEvent);
}

void DialpadKeypad::handleLongPress()
{
    // key press
    sendKeyEventToEditor(QEvent::KeyPress, mPressedNumericKey);
    mPressedNumericKey = 0;
}

bool DialpadKeypad::isNumericKey(int key)
{
    if (key==Qt::Key_Yes || key==Qt::Key_Backspace) {
        return false;
    } else {
        return true;
    }
}

void DialpadKeypad::setLongPressDuration(int duration)
{
    mLongPressDuration = duration;
}

void DialpadKeypad::setCallButtonEnabled(bool enabled)
{
    mButtons[DialpadButtonCount-1]->setEnabled(enabled);
}

void DialpadKeypad::showEvent(QShowEvent *event)
{
    HbWidget::showEvent(event);

    if (parentWidget()->isVisible()) {
        // first show event comes before dialpad is open
        // set fixed row and column dimensions
        QSizeF effectiveSize(rect().width(),
                             rect().height());

        qreal width = effectiveSize.width() / DialpadColumnCount;
        qreal height = effectiveSize.height() / DialpadRowCount;

        for (int i=0; i < DialpadColumnCount ;i++) {
            mGridLayout->setColumnFixedWidth(i, width);
        }

        for (int i=0; i < DialpadRowCount ;i++) {
            mGridLayout->setRowFixedHeight(i, height);
        }
    }
}
