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
#include "dialpadkeysequenceeventfilter.h"
#include "qtphonesrvlog.h"

DialpadKeyHandler::DialpadKeyHandler(
    Dialpad *dialPad, HbMainWindow& mainWindow, QObject *parent) 
    : 
    QObject(parent),
	mMainWindow(mainWindow),
	mVmbxFilter(0),
	mBtFilter(0),
	mKeySequenceFilter(0)
{
    PHONE_TRACE;
    
    mVmbxFilter.reset(new DialpadVoiceMailboxEventFilter(dialPad));
    mBtFilter.reset(new DialpadBluetoothEventFilter(dialPad));
    mKeySequenceFilter.reset(new DialpadKeySequenceEventFilter(dialPad));
    
    // Stack different event filters
    mMainWindow.installEventFilter(mVmbxFilter.data());
    mMainWindow.installEventFilter(mBtFilter.data());
    mMainWindow.installEventFilter(mKeySequenceFilter.data());
}

DialpadKeyHandler::~DialpadKeyHandler()
{
}
