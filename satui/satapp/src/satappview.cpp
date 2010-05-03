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
#include <QTimer>
#include <hbaction.h>
#include <hbmainwindow.h>
#include <hbview.h>
#include <hblistwidget.h>
#include <hblistwidgetitem.h>
#include <hblabel.h>
#include <hbmenu.h>

#include "satappuiprovider.h"
#include "satappeventprovider.h"
#include "satappview.h"
#include "tflogger.h"
#include "satappcommonconstant.h"

const char *SATAPP_MENUITEM = "s:ListWidget";
const char *SATAPP_SUBTITLE = "t:label";
const char *SATAPP_SELECTITEM = "t:ListWidget";


// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// SatAppView::SatAppView
// (Constructor).
// ----------------------------------------------------------------------------
//
SatAppView::SatAppView(QGraphicsItem *parent) :
    HbView(parent),
    mTimeout(false),
    mSelectItem(false),
    mClickBackSoftkey(false),
    mItemIndex(0),
    mLoop(0),
    mTimer(0),
    mWindow(0),
    mSoftKeyQuitAction(0),
    mSoftKeyBackAction(0),
    mEvent(0),
    mUi(0),
    mListWidget(0),
    mSelectListWidget(0),
    mSubTitle(0)
{
    TFLOGSTRING("SATAPP: SatAppView::SatAppView call-exit")
}

// ----------------------------------------------------------------------------
// SatAppView::~SatAppView
// (Disconstructor).
// ----------------------------------------------------------------------------
//
SatAppView::~SatAppView()
{
    TFLOGSTRING("SATAPP: SatAppView::~SatAppView")
    // The objects are outside the object tree, Delete them manually
    if (mTimer) {
        delete mTimer;
        mTimer = 0;
        TFLOGSTRING("SATAPP: SatAppView::~SatAppView delete subtitle")
    }

    if (mLoop) {
        if (mLoop->isRunning()) {
            mLoop->quit();
        }
        delete mLoop;
        mLoop = 0;
        TFLOGSTRING("SATAPP: SatAppView::~SatAppView delete subtitle")
    }
    TFLOGSTRING("SATAPP: SatAppView::~SatAppView Exit")
}

// ----------------------------------------------------------------------------
// SatAppView::initSetupMenu
// (Init the setup menu view).
// ----------------------------------------------------------------------------
//
void SatAppView::initSetupMenu(
    SatAppEventProvider &event,
    SatAppUiProvider &ui)
{
    TFLOGSTRING( "SATAPP: SatAppView::initSetupMenu call" )
    mEvent = &event;
    mUi = &ui;

    // Listwidget
    mListWidget = qobject_cast<HbListWidget *>
        ( mUi->docmlLoader()->findWidget(SATAPP_MENUITEM ));
    if (mListWidget && mWindow) {
        mSoftKeyQuitAction = new HbAction(Hb::QuitAction,this);
        HbAction *menuAction = menu()->addAction("Exit");
        bool ret = connect(menuAction, SIGNAL(triggered()),
                       mSoftKeyQuitAction, SIGNAL(triggered()));
        TFLOGSTRING2("SATAPP: SatAppView::initSetupMenu: \
            menu connected %d", ret)
        ret = connect(mSoftKeyQuitAction, SIGNAL(triggered()),
                       mWindow, SLOT(close()));
        TFLOGSTRING2("SATAPP: SatAppView::initSetupMenu: \
            quit connected %d", ret)
    }

    TFLOGSTRING("SATAPP: SatAppView::initSetupMenu exit")
}


// ----------------------------------------------------------------------------
// SatAppView::initSelectItem
// (Init the select item view).
// ----------------------------------------------------------------------------
//
void SatAppView::initSelectItem(
    SatAppEventProvider &event,
    SatAppUiProvider &ui)
{
    TFLOGSTRING( "SATAPP: SatAppView::initSelectItem call")
    mEvent = &event;
    mUi = &ui;
    // Subtitle
    mSubTitle = qobject_cast<HbLabel *>
        ( mUi->docmlLoader()->findWidget(SATAPP_SUBTITLE) );

    mSelectListWidget = qobject_cast<HbListWidget *>
        ( mUi->docmlLoader()->findWidget(SATAPP_SELECTITEM ));

    if (mSelectListWidget && mWindow) {
        mSoftKeyBackAction = new HbAction(Hb::BackAction,this);
        HbAction *menuBack = menu()->addAction("Back");
        bool ret = connect(menuBack, SIGNAL(triggered()),
                       mSoftKeyBackAction, SIGNAL(triggered()));
        TFLOGSTRING2("SATAPP: SatAppView::initSelectItem: \
            menu connected %d", ret)
        HbAction *menuQuit = menu()->addAction("Exit");
        ret = connect(menuQuit, SIGNAL(triggered()),
                      mWindow, SLOT(close()));
        TFLOGSTRING2("SATAPP: SatAppView::initSelectItem: \
            close connected %d", ret)
        ret = connect(mSoftKeyBackAction, SIGNAL(triggered()),
                      this, SLOT(backButtonClicked()));
        TFLOGSTRING2("SATAPP: SatAppView::initSelectItem: \
            back connected %d", ret)
    }
    TFLOGSTRING("SATAPP: SatAppView::initSelectItem exit")
}

// ----------------------------------------------------------------------------
// SatAppView::SatAppView
// (Set up menu).
// ----------------------------------------------------------------------------
//
void SatAppView::showSetUpMenuContent(
    TSatUiResponse& aRes,
    const QString& aText,
    const QStringList& aMenuItems,
    //const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    //const HbIcon& aIcon,
    //const CAknIconArray* aItemIconsArray,
    const bool /*aSelfExplanatoryItems*/,
    const bool /*aHelpIsAvailable*/)
{
    TFLOGSTRING("SATAPP: SatAppView::SetUpMenu")
    // Disconnect select item
    disconnectItem();
    aRes = ESatSuccess;
    mSelectItem = false;

    // The text is the application name
    // should be shown on the menu area always
    if (!aText.isEmpty()) {
        TFLOGSTRING( "SATAPP: SatAppView::SetUpMenu: title" )
        setTitle(aText);
    }
    if (mListWidget) {
        TFLOGSTRING("SATAPP: SatAppView::SetUpMenu mListWidget")
        mListWidget->clear();
        for(int i = 0; i < aMenuItems.count(); i++ ) {
            mListWidget->addItem(aMenuItems.at( i ));
        }
        // connect setup menu item
        connectItem();
        setNavigationAction(mSoftKeyQuitAction);
    }
    TFLOGSTRING("SATAPP: SatAppView::SetUpMenu exit")
}


// ----------------------------------------------------------------------------
// SatAppView::SelectItem
// (Selected item).
// ----------------------------------------------------------------------------
//
void SatAppView::showSelectItemContent(
    TSatUiResponse& aRes,
    const QString& aText,
    const QStringList& aMenuItems,
    //const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const int aDefaultItem,
    unsigned char& aSelection,
    //const HbIcon& aIcon,
    //const CAknIconArray* aItemsIconArray,
    const bool /*aSelfExplanatoryItems*/,
    const bool /*aHelpIsAvailable*/)
{
    TFLOGSTRING("SATAPP: SatAppView::selectItem")
    // Disconnect setup menu item
    disconnectItem();
    aRes =  ESatSuccess;
    mSelectItem = true;
    mClickBackSoftkey = false;
    if (mWindow){
        mWindow->setCurrentViewIndex(aDefaultItem);
    }
    
    // Set sub title
    if (!aText.isEmpty()) {
        mSubTitle->setPlainText(aText);
    } else {
        TFLOGSTRING("SATAPP: txt_sat_selectitem_title")
        mSubTitle->setPlainText(hbTrId("txt_sat_selectitem_title"));
    }
    // Set List widget
    if (mSelectListWidget) {
        TFLOGSTRING("SATAPP: SatAppView::selectItem mSelectListWidget")
        mSelectListWidget->clear();
        for(int i = 0; i < aMenuItems.count(); i++ ) {
            mSelectListWidget->addItem(aMenuItems.at( i ));
        }
    }
    // Set Back key
    //setSoftkeyBack();
    if (mSoftKeyBackAction) {
       setNavigationAction(mSoftKeyBackAction);
       TFLOGSTRING("SATAPP: SatAppView::selectItem set softkey back")
    }
    // connect selectitem 
    connectItem();

    mTimer = new QTimer();
    if (mTimer) {
        bool ret = connect(mTimer, SIGNAL(timeout()),
                            this, SLOT(selectItemTimeOut()));
        TFLOGSTRING2("SATAPP: SatAppView::selectItem timer %d", ret)
        mTimer->start( KDefaultSelectionTimeoutMseconds );
        mLoop = new QEventLoop();
        TFLOGSTRING("SATAPP: SatAppView::selectItem loop exec before")
        mLoop->exec(QEventLoop::AllEvents);
        TFLOGSTRING("SATAPP: SatAppView::selectItem loop exec after")
        delete mTimer;
        mTimer = 0;
        delete mLoop;
        mLoop = 0;
    }

    if (mTimeout) {
        aRes = ESatNoResponseFromUser;
        TFLOGSTRING("SATAPP: SatAppView::selectItem time out")
    } else if (mClickBackSoftkey) {
        TFLOGSTRING("SATAPP: SatAppView::selectItem, \
        user click the back softkey")
        aRes = ESatBackwardModeRequestedByUser;
    } else {
        TFLOGSTRING2("SATAPP: SatAppView::selectItem selected %d",
                      mItemIndex)
        //For Demo. We will not return user select item to SIM.
        aSelection = mItemIndex;
    }
   // disconnet select item
    disconnectItem();
    TFLOGSTRING2("SATAPP: SatAppView::selectItem aRes:%d", aRes)
    mTimeout = false;
    TFLOGSTRING("SATAPP: SatAppView::selectItem exit")
}

// ----------------------------------------------------------------------------
// SatAppView::selectItemTimeOut
// (Time Out of select item).
// ----------------------------------------------------------------------------
//
void SatAppView::selectItemTimeOut()
{
    TFLOGSTRING("SATAPP: SatAppView::selectItemTimeOut")
    if (mSelectItem && mLoop && mLoop->isRunning()) {
        TFLOGSTRING("SATAPP: SatAppView::selectItemTimeOut time out")
        mTimeout = true;
        mLoop->exit();
    }
    TFLOGSTRING("SATAPP: SatAppView::selectItemTimeOut exit")
}

// ----------------------------------------------------------------------------
// SatAppView::keyPressEvent
// (keyPressEvent).
// ----------------------------------------------------------------------------
//
void SatAppView::keyPressEvent(QKeyEvent *event)
{
    TFLOGSTRING("SATAPP: SatAppView::keyPressEvent")
    // Need updating, End key support is still missing
    switch (event->key())
        {
        case Qt::Key_Hangup:
            {
            TFLOGSTRING("SATAPP: SatAppView::keyPressEvent end key")
            }
            break;
        default:
            {
            TFLOGSTRING2("SATAPP: SatAppView::keyPressEvent key %d", 
            event->key())
            HbView::keyPressEvent(event);
            }
        }
    TFLOGSTRING("SATAPP: SatAppView::keyPressEvent exit")
}

// ----------------------------------------------------------------------------
// SatAppView::backButtonClicked
// (Back action).
// ----------------------------------------------------------------------------
//
void SatAppView::backButtonClicked()
{
    TFLOGSTRING("SATAPP: SatAppView::backButtonClicked")
    if (mSelectItem) {
        mClickBackSoftkey = true;
        TFLOGSTRING("SATAPP: SatAppView::backButtonClicked back selected")
        if (mLoop) {
            TFLOGSTRING("SATAPP: SatAppView::backButtonClicked quit loop")
            if (mLoop->isRunning()) {
                mLoop->exit();
            }
        }
    }
    TFLOGSTRING("SATAPP: SatAppView::backButtonClicked exit")
}

// ----------------------------------------------------------------------------
// SatAppView::setMainWindow
// (Set window for view).
// ----------------------------------------------------------------------------
//
void SatAppView::setMainWindow(HbMainWindow &mainWindow)
{
    TFLOGSTRING("SATAPP: SatAppView::setMainWindow")
    TFLOGSTRING2("SATAPP: SatAppView::setMainWindow \
            &mainWindow=0x%x", &mainWindow)
    mWindow = &mainWindow;
    TFLOGSTRING("SATAPP: SatAppView::setMainWindow exit")
}

// ----------------------------------------------------------------------------
// SatAppView::menuItemSelected
// (Menu item selected).
// ----------------------------------------------------------------------------
//
void SatAppView::menuItemSelected(HbListWidgetItem *item)
{
    TFLOGSTRING("SATAPP: SatAppView::menuItemSelected")
    if(mSelectItem && mSelectListWidget) {
        mItemIndex = mSelectListWidget->row(item);
        if (mLoop && mLoop->isRunning()) {
            TFLOGSTRING( "SATAPP: SatAppView::menuItemSelected sel item")
            mLoop->quit();
            TFLOGSTRING( "SATAPP: SatAppView::menuItemSelected exit loop")
        }
    } 
    if (!mSelectItem && mListWidget) {
        mItemIndex = mListWidget->row(item);
        TFLOGSTRING( "SATAPP: SatAppView::menuItemSelected setup menu")
        if (mEvent) {
            mEvent->menuSelection( mItemIndex, false );
            TFLOGSTRING( "SATAPP: SatAppView::menuItemSelected setup menu")
        }
        // disconnet the list widget, when server returns response, 
        // the connect again
        disconnectItem();
    }
    TFLOGSTRING("SATAPP: SatAppView::menuItemSelected exit")
}

// ----------------------------------------------------------------------------
// SatAppView::connectItem
// (Connect listwidget item).
// ----------------------------------------------------------------------------
//
void SatAppView::connectItem()
{
    TFLOGSTRING("SATAPP: SatAppView::connectItem")
    if (!mSelectItem && mListWidget) {
        const bool result = connect(
            mListWidget, SIGNAL(activated(HbListWidgetItem *)), 
            this, SLOT(menuItemSelected(HbListWidgetItem *)));
        TFLOGSTRING2( 
        "SATAPP: SatAppView::connectItem exit: setup menu item result=%d", 
        result)
    }

    if (mSelectItem && mSelectListWidget) {
        const bool result = connect(
            mSelectListWidget, SIGNAL(activated(HbListWidgetItem *)), 
            this, SLOT(menuItemSelected(HbListWidgetItem *)));
        TFLOGSTRING2( 
        "SATAPP: SatAppView::connectItem exit: select item result=%d", 
        result)
    }
    TFLOGSTRING("SATAPP: SatAppView::connectItem exit")
}

// ----------------------------------------------------------------------------
// SatAppView::disconnectItem
// (Disconnect listwidget item).
// ----------------------------------------------------------------------------
//
void SatAppView::disconnectItem()
{
    TFLOGSTRING("SATAPP: SatAppView::disconnectItem")
    // setup menu view
    if (!mSelectItem && mListWidget) {
        disconnect(mListWidget, SIGNAL( activated(HbListWidgetItem *)),
              this, SLOT( menuItemSelected( HbListWidgetItem *)));
        TFLOGSTRING("SATAPP: SatAppView::disconnectItem: setup menu item ")
    }
    // select item view
    if (mSelectItem && mSelectListWidget) {
        disconnect(mSelectListWidget, SIGNAL( activated(HbListWidgetItem *)),
              this, SLOT( menuItemSelected( HbListWidgetItem *)));
        TFLOGSTRING("SATAPP: SatAppView::disconnectItem: select item ")
    }
    TFLOGSTRING("SATAPP: SatAppView::disconnectItem exit")
}

// End of file
