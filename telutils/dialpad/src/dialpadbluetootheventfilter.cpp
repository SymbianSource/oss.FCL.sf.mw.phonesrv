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
#include "dialpadbluetootheventfilter.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

#ifdef Q_OS_SYMBIAN
#include <xqservicerequest.h>
#endif //Q_OS_SYMBIAN

const int DialpadLongKeyPressTimeOut(1000);
const QString BluetoothCharacter("*");

DialpadBluetoothEventFilter::DialpadBluetoothEventFilter(Dialpad* dialpad, QObject* parent) :
    QObject(parent), mDialpad(dialpad)
{
    PHONE_TRACE;
    mLongPressTimer = new QTimer(this);
    mLongPressTimer->setSingleShot(true);
    connect(mLongPressTimer,SIGNAL(timeout()), this, SLOT(toggleBluetooth()));
}

DialpadBluetoothEventFilter::~DialpadBluetoothEventFilter()
{
}

bool DialpadBluetoothEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    const int keyCode = keyEvent->key();
    const int eventType = event->type();

    if (eventType == QEvent::KeyPress && keyCode == Qt::Key_Asterisk) {
        if (!(mDialpad->editor().text().length() >= 1)) {
            //Check that there is only one item in dialpad, if there is more than one
            //do not handle long key press.
            mLongPressTimer->stop();
            mLongPressTimer->start(DialpadLongKeyPressTimeOut);
        }
    } else if (eventType == QEvent::KeyRelease) {
            mLongPressTimer->stop();
    }

    // Don't consume the key
    return false;
}

void DialpadBluetoothEventFilter::toggleBluetooth()
{
    PHONE_TRACE;
    mDialpad->editor().setText(QString(""));
#ifdef Q_OS_SYMBIAN
    XQServiceRequest snd("com.nokia.services.btservices.ToggleBluetooth","toggleBluetooth()", false);
    QVariant retValue;
    snd.send(retValue);
#endif // Q_OS_SYMBIAN
}
