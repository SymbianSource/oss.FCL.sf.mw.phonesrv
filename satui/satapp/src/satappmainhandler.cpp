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
*
*/

#include "satappmainhandler.h"
#include "satappuiprovider.h"
#include "satappeventprovider.h"
#include "satappcommandhandler.h"
#include "satappcommonconstant.h"
#include "tflogger.h"

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// SatAppMainHandler::SatAppMainHandler
// (Construtor).
// ----------------------------------------------------------------------------
//
SatAppMainHandler::SatAppMainHandler(HbMainWindow &window, 
    QObject */*parent*/):mEvent(NULL), mUi(NULL), mCommand(NULL)
{
    TFLOGSTRING("SATAPP: SatAppMainHandler::SatAppMainHandler call")

    mEvent = new SatAppEventProvider(this);
    mUi = new SatAppUiProvider(window, *mEvent, this);
    mCommand = new SatAppCommandHandler(*mUi, this);

    initConnections();
    TFLOGSTRING("SATAPP: SatAppMainHandler::SatAppMainHandler exit")
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::~SatAppMainHandler
// (Destructor).
// ----------------------------------------------------------------------------
//
SatAppMainHandler::~SatAppMainHandler()
{
    TFLOGSTRING("SATAPP: SatAppMainHandler::~SatAppMainHandler call-exit")
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::initConnections
// 
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::initConnections()
{
    TFLOGSTRING("SATAPP: SatAppMainHandler::initConnections call")
    // For SetUpMenu
    bool ret = connect(mEvent, SIGNAL(setUpMenuEvent(TSatUiResponse &,
        QString, QStringList, bool, bool)),
        mCommand, SLOT(setUpMenu(TSatUiResponse &,
        QString, QStringList, bool, bool)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        SetUpMenu=%d", ret)
    
    // For SelectItem
    ret = connect(mEvent, SIGNAL(selectItemEvent(TSatUiResponse&,
        QString, QStringList, 
        int, unsigned char&, bool, bool)),
        mCommand, SLOT(selectItem(TSatUiResponse&,
        QString,QStringList, 
        int, unsigned char&, bool, bool)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        SelectItem=%d", ret)
    
    // for display text
    ret = connect(mEvent, SIGNAL(displayTextEvent(TSatUiResponse &,
        QString, QString, bool &, bool, int, bool)),
        mCommand, SLOT(displayText(TSatUiResponse&,
        QString, QString, bool &, bool, int, bool)),
        Qt::DirectConnection);
    
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        DisplayText=%d", ret)
    
    ret = connect(mEvent, SIGNAL(clearScreenEvent()),
        mUi, SLOT(clearScreen()),
        Qt::DirectConnection);
    
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        ClearScreen=%d", ret)
    
    ret = connect(mEvent, SIGNAL(closeUiEvent()),
        mUi, SLOT(closeUi()),
        Qt::DirectConnection);
    
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        CloseUi=%d", ret)

    // For GetInkey
    ret = connect(mEvent, SIGNAL(getInkeyEvent(TSatUiResponse &,
        QString, TSatCharacterSet, QString &,
        bool,unsigned int &)),
        mCommand, SLOT(getInkey(TSatUiResponse &, QString,
        TSatCharacterSet, QString &,
        bool, unsigned int &)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        GetInkey=%d", ret)

    ret = connect(mEvent, SIGNAL(getYesNoEvent(TSatUiResponse &,
        QString, TSatCharacterSet, unsigned int &, //TSatIconId,
        bool, unsigned int &, bool)),
        mCommand, SLOT(getYesNo(TSatUiResponse &, QString,
        TSatCharacterSet, unsigned int &, //TSatIconId,
        bool, unsigned int &, bool)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        GetYesNo=%d", ret)
    
    // For GetInput
    ret = connect(mEvent, SIGNAL(getInputEvent(TSatUiResponse &, QString,
        TSatCharacterSet, QString &, int, int, bool, bool, unsigned int &)),
        mCommand, SLOT(getInput(TSatUiResponse &, QString, TSatCharacterSet,
        QString &, int, int, bool, bool, unsigned int &)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        GetInput=%d", ret)
    
    // For CallControl
    ret = connect(mEvent, SIGNAL(callControlEvent(QString,
        TSatAlphaIdStatus)),
        mCommand, SLOT(callControl(QString, TSatAlphaIdStatus)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        CallControl=%d", ret)

    // For MoSmControl
    ret = connect(mEvent, SIGNAL(moSmControlEvent(QString,
        TSatAlphaIdStatus)),
        mCommand, SLOT(moSmControl(QString, TSatAlphaIdStatus)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        MoSmControl=%d", ret)

    // For send confirm
    ret = connect(mEvent, SIGNAL(showSmsWaitNoteEvent(QString,
        bool)),
        mCommand, SLOT(showSmsWaitNote(QString,
        bool)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        showSmsWaitNote=%d", ret)
        
    //For Send DTMF
    ret = connect(mEvent, SIGNAL(showDtmfWaitNoteEvent(
        TSatUiResponse &, QString)),
        mCommand, SLOT(showDtmfWaitNote(
        TSatUiResponse &, QString)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        showDtmfsWaitNote=%d", ret)
    
    //For DTMF, BIP cancel
    ret = connect(mUi, SIGNAL(userCancelResponse()),
                mEvent, SLOT(userCancelResponse()),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        userCancelResponse=%d", ret)
    
    //Stop wait note
    ret = connect(mEvent, SIGNAL(stopShowWaitNoteEvent()),
        mCommand, SLOT(stopShowWaitNote()),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        stopShowWaitNote=%d", ret)

    // For SetUpCall
    ret = connect(mEvent, SIGNAL(showSetUpCallConfirmEvent(
        QString, QString, bool &)),
        mCommand, SLOT(confirmSetUpCall(
        QString, QString, bool &)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        showSetUpCallConfirm=%d", ret)
        
    ret = connect(mEvent, SIGNAL(showSsWaitNoteEvent(QString,
        bool)),
        mCommand, SLOT(showSsWaitNote(QString,
        bool)),
        Qt::DirectConnection);
    
    // For Send SS or USSD
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        showSsWaitNote=%d", ret)

    //For showWaitNoteWithoutDelay
    ret = connect(mEvent, SIGNAL(showWaitNoteWithoutDelayEvent()),
        mUi, SLOT(showWaitNoteWithoutDelay()),
        Qt::DirectConnection);
     TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        showWaitNoteWithoutDelayEvent=%d", ret)

    //For showSsErrorNote
    ret = connect(mEvent, SIGNAL(showSsErrorNoteEvent()),
        mUi, SLOT(showSsErrorNote()),
        Qt::DirectConnection);
     TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        showSsErrorNoteEvent=%d", ret)

    // For Open Channel
    ret = connect(mEvent, SIGNAL(showOpenChannelConfirmEvent(
        QString, bool &)),
        mUi, SLOT(showConfirmOpenChannelQuery(
        QString, bool &)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        showOpenChannelConfirm=%d", ret)

    // For BIP wait note
    ret = connect(mEvent, SIGNAL(showBIPNoteEvent(
        int, QString)),
        mCommand, SLOT(showBIPWaitNote(
        int, QString)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
         showBIPNote=%d", ret)

    TFLOGSTRING("SATAPP: SatAppMainHandler::initConnections exit")
}

//End of file
