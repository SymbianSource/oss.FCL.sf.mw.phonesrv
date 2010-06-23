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

#include <QLocale>
#include <QSignalMapper>

#include <hbinstance.h>
#include <hbinputkeymapfactory.h>
#include <hbinputkeymap.h>
#include <hbinpututils.h>
#include <hbinputsettingproxy.h>
#include <hbinputlanguage.h>
#include <hbapplication.h>
#include <hblineedit.h>
#include <hbinputbutton.h>

#include "dialpadkeypad.h"
#include "dialpadbutton.h"
#include "dialpadinputfield.h"

static const int DialpadRowCount = 4;
static const int DialpadColumnCount = 3;
static const QString handsetIcon("qtg_mono_call");
static const QString vmbxIcon("qtg_mono_voice_mailbox");
static const qreal DialpadKeypadBorderWidth = 0.25;

static const int DialpadKeyCodeTable[DialpadRowCount*DialpadColumnCount] =
{
    Qt::Key_1,        Qt::Key_2,      Qt::Key_3,
    Qt::Key_4,        Qt::Key_5,      Qt::Key_6,
    Qt::Key_7,        Qt::Key_8,      Qt::Key_9,
    Qt::Key_Asterisk, Qt::Key_0,      Qt::Key_NumberSign
    // Qt::Key_Yes and Qt::Key_BackSpace are handled separately
};

DialpadKeypad::DialpadKeypad(
    const HbMainWindow& mainWindow,
    DialpadInputField& inputField,
    QGraphicsItem* parent) :
    HbInputButtonGroup(parent),
    mMainWindow(mainWindow),
    mInputField(inputField)
{
    setObjectName("keypad");

    // create clicked signal mapper
    mKeyClickedSignalMapper = new QSignalMapper(this);
    connect(mKeyClickedSignalMapper,SIGNAL(mapped(int)),
            SLOT(handleKeyClicked(int)));

    // connect backspace signals
    connect(&mInputField.backspaceButton(),SIGNAL(clicked()),
            mKeyClickedSignalMapper,SLOT(map()));
    mKeyClickedSignalMapper->setMapping(&mInputField.backspaceButton(),
                                        Qt::Key_Backspace);

    // create keypad
    setGridSize(QSize(DialpadColumnCount, DialpadRowCount));
    setButtonBorderSize(DialpadKeypadBorderWidth);

    QList<HbInputButton*> buttons;

    for (int i = 0; i < DialpadRowCount * DialpadColumnCount; ++i) {
        HbInputButton *item = new HbInputButton(
            DialpadKeyCodeTable[i],
            QPoint(i % DialpadColumnCount, i / DialpadColumnCount));
        buttons.append(item);

        item->setType(HbInputButton::ButtonTypeNormal);
    }

    setButtons(buttons);

    // connect keypad signals
    QObject::connect(this, SIGNAL(buttonPressed(const QKeyEvent&)),
                     this, SLOT(sendKeyPressEvent(const QKeyEvent&)));
    QObject::connect(this, SIGNAL(buttonReleased(const QKeyEvent&)),
                     this, SLOT(sendKeyReleaseEvent(const QKeyEvent&)));
    QObject::connect(this, SIGNAL(buttonLongPressed(const QKeyEvent&)),
                     this, SLOT(sendLongPressEvent(const QKeyEvent&)));
    QObject::connect(this, SIGNAL(pressedButtonChanged(const QKeyEvent&,
                                                       const QKeyEvent&)),
                     this, SLOT(handleKeyChangeEvent(const QKeyEvent&,
                                                     const QKeyEvent&)));

    // create call button (parent layouts this)
    mCallButton = new DialpadButton(parent);
    mCallButton->setButtonType(DialpadButton::CallButton);
    mCallButton->setIcon(HbIcon(handsetIcon));
    QString buttonName;
    buttonName.setNum(Qt::Key_Yes);
    mCallButton->setObjectName(buttonName);
    connect(mCallButton,SIGNAL(clicked()),
            mKeyClickedSignalMapper,SLOT(map()));
    connect(mCallButton,SIGNAL(longPress(QPointF)),
            mKeyClickedSignalMapper,SLOT(map()));
    mKeyClickedSignalMapper->setMapping(mCallButton,
                                        Qt::Key_Yes);

    // set button texts
    setButtonTexts();
    // set button icons
    button(0)->setIcon(HbIcon(vmbxIcon),
        HbInputButton::ButtonIconIndexSecondaryFirstRow);

    // update button texts when input language is changed
    connect(HbInputSettingProxy::instance(),
            SIGNAL(globalInputLanguageChanged(HbInputLanguage)),
            this,SLOT(setButtonTexts()));
}

DialpadKeypad::~DialpadKeypad()
{
}

void DialpadKeypad::setButtonTexts()
{
    HbInputLanguage inputLanguage =
        HbInputSettingProxy::instance()->globalInputLanguage();
    const HbKeymap *keymap =
        HbKeymapFactory::instance()->keymap(inputLanguage.language());

    mGeneratedChar.clear();

    if (keymap) {
        int buttonCount = (DialpadRowCount*DialpadColumnCount);
        for (int i = 0; i < buttonCount; i++) {
            int keyCode = DialpadKeyCodeTable[i];

            if (keyCode == Qt::Key_Asterisk) {
                // asterisk is not localized
                QChar asterisk('*');
                button(i)->setText(asterisk,
                    HbInputButton::ButtonTextIndexPrimary);
                button(i)->setText("+",
                    HbInputButton::ButtonTextIndexSecondaryFirstRow);
                mGeneratedChar.insert(Qt::Key_Asterisk, asterisk);
                continue;
            }

            if (keyCode == Qt::Key_NumberSign) {
                // number sign is not localized
                QChar numberSign('#');
                button(i)->setText(numberSign,
                    HbInputButton::ButtonTextIndexPrimary);
                button(i)->setText(" ",
                    HbInputButton::ButtonTextIndexSecondaryFirstRow);
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
                button(i)->setText(numberChar,
                    HbInputButton::ButtonTextIndexPrimary);
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
                    button(i)->setText(" ",
                        HbInputButton::ButtonTextIndexSecondaryFirstRow);
                } else {
                    button(i)->setText(characters.left(numberOfCharacters),
                        HbInputButton::ButtonTextIndexSecondaryFirstRow);
                }
            }
        }
    }
}

void DialpadKeypad::handleKeyClicked(int key)
{
    // concerns only yes and backspace keys
    postKeyEvent(QEvent::KeyPress, key);
    postKeyEvent(QEvent::KeyRelease, key);
}

void DialpadKeypad::postKeyEvent(QEvent::Type type, int key)
{
    // send simulated key to application
    QKeyEvent *keyEvent = new QKeyEvent(type, key, Qt::NoModifier);
    HbApplication::postEvent(const_cast<HbMainWindow*>(&mMainWindow),keyEvent);
}

void DialpadKeypad::sendKeyEventToEditor(QEvent::Type type, int key)
{
    // send key event to editor
    QKeyEvent keyEvent(type, key, Qt::NoModifier, mGeneratedChar.value(key));
    HbApplication::sendEvent(&mInputField.editor(), &keyEvent);
}

void DialpadKeypad::sendKeyPressEvent(const QKeyEvent& event)
{
    mPressedNumericKey = event.key();
    postKeyEvent(QEvent::KeyPress, event.key());
}

void DialpadKeypad::sendKeyReleaseEvent(const QKeyEvent& event)
{
    if (mPressedNumericKey) {
        // short press, update editor here
        sendKeyEventToEditor(QEvent::KeyPress, event.key());
    }

    postKeyEvent(QEvent::KeyRelease, event.key());    
}

void DialpadKeypad::sendLongPressEvent(const QKeyEvent& event)
{
    sendKeyEventToEditor(QEvent::KeyPress, event.key());
    mPressedNumericKey = 0;
}

void DialpadKeypad::handleKeyChangeEvent(
    const QKeyEvent& releaseEvent,
    const QKeyEvent& pressEvent)
{
    Q_UNUSED(pressEvent)

    postKeyEvent(QEvent::KeyRelease, releaseEvent.key());
    cancelButtonPress();
}

void DialpadKeypad::setCallButtonEnabled(bool enabled)
{
    mCallButton->setEnabled(enabled);
}

void DialpadKeypad::resetButtons()
{
    cancelButtonPress();
    mCallButton->setDown(false);
}

DialpadButton& DialpadKeypad::callButton() const
{
    return *mCallButton;
}
