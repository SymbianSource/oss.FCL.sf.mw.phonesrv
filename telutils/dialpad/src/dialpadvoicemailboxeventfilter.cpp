/*
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
* Description: 
*
*/
#include <QTimer>
#include <QKeyEvent>
#include <hblineedit.h>
#include <hbstringutil.h>
#include "dialpadvoicemailboxeventfilter.h"
#include "dialpadsymbianwrapper.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

#ifdef Q_OS_SYMBIAN
#include <xqservicerequest.h>
#include <xqserviceutil.h>
#endif //Q_OS_SYMBIAN

const int DialpadLongKeyPressTimeOut(1000);
const QString VmbxCharacter("1");

enum err {
    DialpadErrorNone = 0,
    DialpadErrorCancel = -3
};

const int DialpadLongKeyPressButtonCount(1);
static const int DialpadLongKeyPressSupportingButtons[DialpadLongKeyPressButtonCount] =
{ Qt::Key_1 };


DialpadVoiceMailboxEventFilter::DialpadVoiceMailboxEventFilter(Dialpad* dialpad, QObject* parent) :
    QObject(parent), mDialpad(dialpad), mKeyEvent(NULL)
{
    PHONE_TRACE;
    mLongPressTimer = new QTimer(this);
    mLongPressTimer->setSingleShot(true);
    connect(mLongPressTimer,SIGNAL(timeout()),this,SLOT(handleLongKeyPress()));
    mSymbianWrapper = new DialpadSymbianWrapper(this);
}

DialpadVoiceMailboxEventFilter::~DialpadVoiceMailboxEventFilter()
{
}

bool DialpadVoiceMailboxEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    bool keyEventEaten(false);

    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    const int keyCode = keyEvent->key();
    const int eventType = event->type();

    if (eventType == QEvent::KeyPress) {
        if (checkIfSendEventAndConsumeEvent(keyCode, eventType)) {
            keyEventEaten = true;
        } else if ((isLongKeyPressSupported(keyCode)) &&
                !(mDialpad->editor().text().length() >= 1)) {
            //Check that there is only one item in dialpad, if there is more than one
            //do not handle long key press.
            mLongPressTimer->stop();
            mLongPressTimer->start(DialpadLongKeyPressTimeOut);
        }
    } else if (eventType == QEvent::KeyRelease) {
        if (checkIfSendEventAndConsumeEvent(keyCode, eventType)) {
            keyEventEaten = true;
        } else if (isLongKeyPressSupported(keyCode)){
            mLongPressTimer->stop();
        }
    }

    return keyEventEaten;
}

bool DialpadVoiceMailboxEventFilter::checkIfSendEventAndConsumeEvent(const int pressedKey, const int eventType)
{
    PHONE_TRACE4("pressedKey:", pressedKey, "eventType:", eventType);
    bool sendKeyHandled(false);
    // first check that pressed key is send key.
    if (pressedKey == Qt::Key_Yes ||
        pressedKey == Qt::Key_Enter) {
       if (eventType == QEvent::KeyPress) {
           sendKeyHandled = handleCallButtonPress();
       } else if ((eventType == QEvent::KeyRelease) &&
                  (!mDialpad->editor().text().isEmpty())) {
           sendKeyHandled = true;
       }
    }
    return sendKeyHandled;
}

bool DialpadVoiceMailboxEventFilter::isLongKeyPressSupported(const int key)
{
    PHONE_TRACE2("key:", key);
    bool longKeySupport(false);
    // check if dialpad button is pressed.
    for (int i = 0; i < DialpadLongKeyPressButtonCount; i++) {
        if (key==DialpadLongKeyPressSupportingButtons[i]) {
            longKeySupport = true;
            // Save key code for handleCallButtonPress.
            mKeyEvent = key;
        }
    }
    return longKeySupport;
}

void DialpadVoiceMailboxEventFilter::handleLongKeyPress()
{
    PHONE_TRACE;
    switch(mKeyEvent) {
    case Qt::Key_1:{
       handleMailboxOperation();
       break;
       }
   default:
       // Do nothing.
       break;
    }

    // Reset key code.
    mKeyEvent = NULL;
}

bool DialpadVoiceMailboxEventFilter::handleCallButtonPress()
{
    PHONE_TRACE;
    bool callButtonhandled(false);
    if (!mDialpad->editor().text().isEmpty()) {
#ifdef Q_OS_SYMBIAN
        // check if editor has '1' character if does then
        // get MailboxNumber.
        QString editorContent = HbStringUtil::convertDigitsTo(
            mDialpad->editor().text(), WesternDigit);
        if (VmbxCharacter==editorContent) {
            handleMailboxOperation();
            callButtonhandled = true;
        }
#endif //Q_OS_SYMBIAN
    }
    return callButtonhandled;
}

void DialpadVoiceMailboxEventFilter::handleMailboxOperation()
{
    PHONE_TRACE;
    QString mailboxNumber(NULL);
    int error = mSymbianWrapper->getMailboxNumber(mailboxNumber);
    // If here is no vmbx number and dialpad must start vmbx number definition procedures.
    if (DialpadErrorNone != error) {
        mDialpad->closeDialpad();
        // If define mailbox query was interupted than reopen dialpad.
        error = mSymbianWrapper->defineMailboxNumber(mailboxNumber);
        if (DialpadErrorCancel == error) {
            mDialpad->openDialpad();
        }
    }
    // Valid vmbx number found or defined and there vmbx didnt
    // return error values then create a call.
    if ((DialpadErrorNone == error) &&
        (mailboxNumber.length() != 0)) {
        createCall(mailboxNumber);
        clearEditor();
        mDialpad->openDialpad();
    }
}

void DialpadVoiceMailboxEventFilter::clearEditor()
{
    PHONE_TRACE;
    // Erase data from dialpad editor.
    mDialpad->editor().setText(NULL);
}

void DialpadVoiceMailboxEventFilter::createCall(const QString &phoneNumber)
{
    PHONE_TRACE2("phoneNumber:", phoneNumber);
#ifdef Q_OS_SYMBIAN
    XQServiceRequest snd("com.nokia.symbian.ICallDial","dial(QString)", false);
    snd << phoneNumber;
    QVariant retValue;
    snd.send(retValue);
#endif // Q_OS_SYMBIAN
}
