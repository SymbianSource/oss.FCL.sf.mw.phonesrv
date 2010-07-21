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

#include <HbApplication>
#include <HbActivityManager>
#include "satappmainhandler.h"
#include "satappserverdispatcher.h"
#include "satappmenuprovider.h"
#include "satappinputprovider.h"
#include "satapptoneprovider.h"
#include "satapppopupprovider.h"
#include "satappconstant.h"
#include "satappconfirmprovider.h"

// Activity ID for Sat Application
const char *SATAPP_ACTIVITY_ID = "SIMServicesList";

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// SatAppMainHandler::SatAppMainHandler
// (Construtor).
// ----------------------------------------------------------------------------
//
SatAppMainHandler::SatAppMainHandler(HbMainWindow &window, 
    QObject */*parent*/)
{
    qDebug("SATAPP: SatAppMainHandler::SatAppMainHandler >");
    
    mServer = new SatAppServerDispatcher(this);
    mMenu = new SatAppMenuProvider(&window, this);
    mInput = new SatAppInputProvider(this);
    mTone = new SatAppToneProvider(this);
    mPopup = new SatAppPopupProvider(this);
    mConfirm = new SatAppConfirmProvider(this);
    initConnections();
    removeActivity();
    qDebug("SATAPP: SatAppMainHandler::SatAppMainHandler <");
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::~SatAppMainHandler
// (Destructor).
// ----------------------------------------------------------------------------
//
SatAppMainHandler::~SatAppMainHandler()
{
    qDebug("SATAPP: SatAppMainHandler::~SatAppMainHandler");
}

// ----------------------------------------------------------------------------
// Local override for connect-function
// this method ENSURES that the connection is succesful.
// ----------------------------------------------------------------------------
//
void doConnect(
    const QObject *sender,
    const char *signal,
    const QObject *receiver,
    const char *member)
{
    bool ret = QObject::connect(sender, signal,
                receiver, member, Qt::DirectConnection);
    Q_ASSERT_X(ret, "doConnect: connection failed for: ", signal);
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::initConnections
// 
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::initConnections()
{
    qDebug("SATAPP: SatAppMainHandler::initConnections >");

    // --------------------------------------
    // MenuProvider
    // --------------------------------------

    // SetupMenu command from server
    doConnect(
        mServer, SIGNAL( setUpMenu( SatAppAction &) ),
        mMenu, SLOT( setUpMenu( SatAppAction &) ) );
    
    // SelectItem command from server
    doConnect(
        mServer, SIGNAL( selectItem( SatAppAction &) ),
        mMenu, SLOT( selectItem( SatAppAction &) ) );

    // --------------------------------------
    // InputProvider
    // --------------------------------------

    // GetInkey command    
    doConnect(
        mServer, SIGNAL( getInkey( SatAppAction &) ),
        mInput, SLOT( getInkey( SatAppAction &) ) );
    // GetInput command
    doConnect(
        mServer, SIGNAL( getInput( SatAppAction &) ),
        mInput, SLOT( getInput( SatAppAction &) ) );

    // clearScreen in InputProvider
    doConnect(
        mServer, SIGNAL( clearScreen() ),
        mInput, SLOT( resetState() ));
 
    // --------------------------------------
    // Play tone
    // --------------------------------------
    // Play tone 
    doConnect(
        mServer, SIGNAL( playTone( SatAppAction &) ),
        mTone, SLOT( playTone( SatAppAction &) ) );

    // clearScreen in tone provider
    doConnect(
        mServer, SIGNAL( clearScreen() ),
        mTone, SLOT( clearScreen() ) );

    // --------------------------------------
    // Show Popups
    // --------------------------------------

    // display text
    doConnect(
        mServer, SIGNAL(displayText( SatAppAction & )),
        mPopup, SLOT(displayText( SatAppAction & )));

    // show notification
    doConnect(
        mServer, SIGNAL( notification( SatAppAction & ) ),
        mPopup, SLOT( notification( SatAppAction & ) ) );

    // hide wait note
    doConnect(
        mServer, SIGNAL( stopShowWaitNote() ),
        mPopup, SLOT( stopShowWaitNote() ) );

    // clearScreen in popup note provider
    doConnect(
        mServer, SIGNAL(clearScreen()),
        mPopup, SLOT(clearScreen()));
    
    // clearScreen in popup note provider
    doConnect(
        mServer, SIGNAL(showSsErrorNote()),
        mPopup, SLOT(showSsErrorNote()));    

    // --------------------------------------
    // Show Confirm
    // --------------------------------------

    // show confirmCommand
    doConnect(
        mServer, SIGNAL( confirmCommand( SatAppAction & ) ),
        mConfirm, SLOT( confirmCommand( SatAppAction & ) ) );

    // clearScreen
    doConnect(
        mServer, SIGNAL( clearScreen() ),
        mConfirm, SLOT( clearScreen() ) );

    // Task switcher
    doConnect(
        mServer, SIGNAL( setUpMenu( SatAppAction & ) ),
        this, SLOT( updateActivity() ) );

    doConnect(
        qApp, SIGNAL( aboutToQuit() ),
        this, SLOT( saveActivity() ) );

    qDebug("SATAPP: SatAppMainHandler::initConnections <");
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::updateActivity
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::updateActivity()
{
    qDebug("SATAPP: SatAppMainHandler::updateActivity >");
    mActivity.insert("screenshot", mMenu->takeScreenShot());
    qDebug("SATAPP: SatAppMainHandler::updateActivity <");
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::saveActivity
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::saveActivity()
{
    qDebug("SATAPP: SatAppMenuProvider::saveActivity >");

    // Add the activity to the activity manager
    const bool ok = qobject_cast<HbApplication*>(qApp)->activityManager()->
        addActivity(SATAPP_ACTIVITY_ID, QVariant(), mActivity);
    
    qDebug("SATAPP: SatAppMenuProvider::saveActivity < %d", ok);
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::removeActivity
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::removeActivity()
{
    qDebug("SATAPP: SatAppMenuProvider::removeActivity >");
        
    QList<QVariantHash> activityList = 
        qobject_cast<HbApplication*>(qApp)->activityManager()->activities();
    qDebug("SATAPP: SatAppMenuProvider::removeActivity count=%d",
        activityList.count());
    foreach (QVariantHash activity, activityList){
        if (activity.keys().contains(SATAPP_ACTIVITY_ID)){
            mActivity = activity;
            qDebug("SATAPP: SatAppMenuProvider::removeActivity store");
            break;
        }
    }    

    const bool ok = qobject_cast<HbApplication*>(qApp)->activityManager()->
        removeActivity(SATAPP_ACTIVITY_ID);
    
    qDebug("SATAPP: SatAppMenuProvider::removeActivity < %d", ok);
}

//End of file
