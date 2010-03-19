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
    QObject */*parent*/):mEvent(0), mUi(0), mCommand(0)
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


void SatAppMainHandler::initConnections()
{
    TFLOGSTRING("SATAPP: SatAppMainHandler::initConnections call")
    bool ret = false; 
    // For SetUpMenu
    ret = connect(mEvent, SIGNAL(setUpMenuEvent(TSatUiResponse &,
        QString, QStringList, /*const HbIcon& ,*/ bool, bool)),
        mCommand, SLOT(setUpMenu(TSatUiResponse &,
        QString, QStringList, /*const HbIcon& ,*/ bool, bool)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        SetUpMenu=%d", ret)
    
    // For SelectItem
    ret = connect(mEvent, SIGNAL(selectItemEvent(TSatUiResponse&,
        QString, QStringList, /*const HbIcon& ,*/
        int, unsigned char&, bool, bool)),
        mCommand, SLOT(selectItem(TSatUiResponse&,
        QString,QStringList, /*const HbIcon& ,*/
        int, unsigned char&, bool, bool)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        SelectItem=%d", ret)
    
    // for display text
    ret = connect(mEvent, SIGNAL(displayTextEvent(TSatUiResponse &,
        QString, QString, /*const HbIcon & ,*/ bool &, bool, int, bool)),
        mCommand, SLOT(displayText(TSatUiResponse&,
        QString, QString, /*const HbIcon & ,*/ bool &, bool, int, bool)),
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
        //const QPixmap* aIconBitmapGetInput,
        bool,unsigned int &)),
        mCommand, SLOT(getInkey(TSatUiResponse &, QString,
        TSatCharacterSet, QString &,
        //const QPixmap* aIconBitmapGetInput,
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
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: GetInput=%d", ret)
    
    // For send confirm
    ret = connect(mEvent, SIGNAL(confirmSendEvent(TSatUiResponse &, QString,
        bool &, int)),
        mCommand, SLOT(confirmSend(TSatUiResponse&, QString, bool &, int)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppSendSms::initConnections: confirmSend=%d", ret)

    ret = connect(mEvent, SIGNAL(showSmsWaitNoteEvent(QString,
            /*const CFbsBitmap* aIconBitmapSendSM,*/ bool)),
            mCommand, SLOT(showSmsWaitNote(QString,
            /*const CFbsBitmap* aIconBitmapSendSM,*/bool)),
            Qt::DirectConnection);

    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: \
        showSmsWaitNote=%d", ret)
    //For Send DTMF
    ret = connect(mEvent, SIGNAL(showDtmfWaitNoteEvent(
        TSatUiResponse &, QString/*,
        const CFbsBitmap* aIconBitmapSendDTMF, bool*/)),
        mCommand, SLOT(showDtmfWaitNote(TSatUiResponse &, QString
                /*const CFbsBitmap* aIconBitmapSendDTMF,bool*/)),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: showDtmfsWaitNote=%d", ret)
    //For DTMF cancel
    ret = connect(mUi, SIGNAL(userCancelDtmfResponse()),
                mEvent, SLOT(userCancelDtmfResponse()),
        Qt::DirectConnection);
    TFLOGSTRING2("SATAPP: SatAppMainHandler::initConnections: userCancelDtmfResponse=%d", ret)
    
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
    
    TFLOGSTRING("SATAPP: SatAppMainHandler::initConnections exit")
}

//End of file
