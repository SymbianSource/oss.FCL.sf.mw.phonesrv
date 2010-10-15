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
#include <btxqserviceapi.h>
#include "dialpadbluetootheventfilter.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

#ifdef Q_OS_SYMBIAN
#include <xqappmgr.h>
#endif //Q_OS_SYMBIAN

const int DialpadLongKeyPressTimeOut(1000);
const QString BluetoothCharacter("*");

DialpadBluetoothEventFilter::DialpadBluetoothEventFilter(Dialpad* dialpad, QObject* parent) :
    QObject(parent), mDialpad(dialpad), mRequest(0)
{
    PHONE_TRACE;
    mLongPressTimer = new QTimer(this);
    mLongPressTimer->setSingleShot(true);
    connect(mLongPressTimer,SIGNAL(timeout()), this, SLOT(toggleBluetooth()));
}

DialpadBluetoothEventFilter::~DialpadBluetoothEventFilter()
{
    delete mRequest;
    mRequest = 0;
}

bool DialpadBluetoothEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
            
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Asterisk && 
            !(mDialpad->editor().text().length() >= 1)) {
            //Check that there is only one item in dialpad, if there is more than one
            //do not handle long key press.
            mLongPressTimer->stop();
            mLongPressTimer->start(DialpadLongKeyPressTimeOut);
        }
    } else if (event->type() == QEvent::KeyRelease) {
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
    if(mRequest == NULL) {
        XQApplicationManager appManager;
        mRequest = appManager.create(BluetoothServiceName, BluetoothInterfaceTogglePower,
                                                           BluetoothTogglePower, false);
    }
    
    if (mRequest == NULL) {
        return;
    } else {
        connect(mRequest, SIGNAL(requestOk(const QVariant&)), this, SLOT(requestOk(const QVariant&)));
        connect(mRequest, SIGNAL(requestError(int, const QString&)), this, SLOT(requestError(int, const QString&)));
    }

    mRequest->setSynchronous(false);
    mRequest->send();
#endif // Q_OS_SYMBIAN
}

void DialpadBluetoothEventFilter::requestOk(const QVariant& value)
{
    PHONE_TRACE;
    Q_UNUSED(value);
    delete mRequest;
    mRequest = 0;
}

void DialpadBluetoothEventFilter::requestError(int errorCode, const QString& errorMessage)
{
    Q_UNUSED(errorCode);
    Q_UNUSED(errorMessage);
    PHONE_DEBUG2("DialpadBluetoothEventFilter::requestError - errorCode: ", errorCode);
    PHONE_DEBUG2("DialpadBluetoothEventFilter::requestError - errorMessage: ", errorMessage);
    delete mRequest;
    mRequest = 0;
}

