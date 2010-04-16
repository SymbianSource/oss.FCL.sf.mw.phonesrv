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
* Description: Custom button
*
*/

#include <hbmainwindow.h>
#include "dialpadkeyhandler.h"
#include "dialpadvoicemailboxeventfilter.h"
#include "dialpadbluetootheventfilter.h"
#include "qtphonesrvlog.h"

DialpadKeyHandler::DialpadKeyHandler(Dialpad *dialPad, HbMainWindow& mainWindow, QObject *parent) : QObject(parent),  mMainWindow(mainWindow)
{
    PHONE_TRACE;
    mVmbxFilter = new DialpadVoiceMailboxEventFilter(dialPad, this);
    Q_ASSERT(mVmbxFilter != NULL);
    mBtFilter = new DialpadBluetoothEventFilter(dialPad, this);
    Q_ASSERT(mBtFilter != NULL);

    // Stack different event filters
    mMainWindow.installEventFilter(mVmbxFilter);
    mMainWindow.installEventFilter(mBtFilter);
}

DialpadKeyHandler::~DialpadKeyHandler()
{
}
