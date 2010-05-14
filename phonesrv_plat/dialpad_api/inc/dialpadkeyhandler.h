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
* Description: Dialpad key handler.
*
*/

#ifndef DIALPADKEYHANDLER_H
#define DIALPADKEYHANDLER_H

#include <QObject>

class Dialpad;
class DialpadVoiceMailboxEventFilter;
class DialpadBluetoothEventFilter;
class DialpadKeySequenceEventFilter;
class HbMainWindow;

#ifdef BUILD_DIALPADKEYHANDLER
#define DIALPADKEYHANDLER_EXPORT Q_DECL_EXPORT
#else
#define DIALPADKEYHANDLER_EXPORT Q_DECL_IMPORT
#endif
/*!
    DialpadKeyHandler
    Class provides key handling for dialpad component.

    @code
    Dialpad *dialpad = new Dialpad();
    DialpadKeyHandler *keyhandler = new DialpadKeyHandler(dialpad, this);
    @endcode
    
*/
class DIALPADKEYHANDLER_EXPORT DialpadKeyHandler : public QObject
{
    Q_OBJECT

public:
    explicit DialpadKeyHandler(Dialpad *dialPad, HbMainWindow& mainWindow, QObject *parent = 0);
    virtual ~DialpadKeyHandler();

private:
    HbMainWindow& mMainWindow;
    QScopedPointer<DialpadVoiceMailboxEventFilter> mVmbxFilter;
    QScopedPointer<DialpadBluetoothEventFilter> mBtFilter;
    QScopedPointer<DialpadKeySequenceEventFilter> mKeySequenceFilter;
};

#endif // DIALPADKEYHANDLER_H
