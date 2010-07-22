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
#include <QCoreApplication>
#include <QTime> // GetInkey duration return
#include <QTimer> // timeout callback
#include <hbmainwindow.h> // softkey
#include <hbdocumentloader.h> // application xml
#include <hbaction.h> // action user response
#include <hbmessagebox.h> // DisplayText, ConfirmSend,
#include <hbdevicemessagebox.h> // CallControl, SetUpCall
#include <hblabel.h> // DisplayText, GetInput, SetUpCall
#include <hbinputdialog.h> // GetInkey
#include <hbprogressdialog.h> // SendSms wait note
#include <hblineedit.h> // For GetInput
#include <hbinputeditorinterface.h> // GetInput
#include <hbinputeditorinterface.h>
#include <hbinputstandardfilters.h>
#include <hbinputfilter.h> 
#include <dialogwaiter.h>
#include "satappview.h" // SetUpMenu, SelectItem
#include "satappuiprovider.h"
#include "tflogger.h"

const char *SATAPP_DOCML = ":/xml/satapp.docml";
const char *SATAPP_SETUPMENU_VIEW = "setupmenu_view";
const char *SATAPP_SELECTITEM_VIEW = "selectitem_view";

// ======== MEMBER FUNCTIONS ==================================================
// TODO: #ifndef __WINS__ need to be remove when orbit works well. Now the macro
// is to avoid panic on emulator. Deleting pointer before create dialog is to
// avoid memory leak in emulator.

// ----------------------------------------------------------------------------
// SatAppUiProvider
// ----------------------------------------------------------------------------
//
SatAppUiProvider::SatAppUiProvider(
    HbMainWindow &window,
    SatAppEventProvider &event,
    QObject *parent) :
    QObject(parent), mMainWindow(window), mLoader(0),
    mSetupMenuView(0), mSelectItemView(0), mDisplayPopup(0),
    mGetInkeyQuery(0),mYesNoPopup(0), mImmediateQuery(0), mGetInputQuery(0),
    mConfirmSendQuery(0), mSetUpCallQuery(0), mCallControlMsg(0),
    mConfirmBipQuery(0), mUserRsp(EUserNoResponse), mDigitalRsp(0), 
    mMinLength(0), mTimer(0), mLoop(0),mWaitNote(0)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::SatAppUiProvider call");

    bool docmlLoad = false;
    mLoader = new SatAppDocumentLoader();
    // ownership of the objects are transferred to caller
    mObjects = mLoader->load(SATAPP_DOCML,&docmlLoad);
    if (docmlLoad) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::SatAppUiProvider view found")
        mSetupMenuView = qobject_cast<SatAppView *>(mLoader->findWidget
        (SATAPP_SETUPMENU_VIEW));
        if (mSetupMenuView) {
            mMainWindow.addView(mSetupMenuView);
            mSetupMenuView->setMainWindow(mMainWindow);
            mSetupMenuView->initSetupMenu(event, *this);
            mMainWindow.setCurrentView(mSetupMenuView);
        }

        mSelectItemView = qobject_cast<SatAppView *>(mLoader->findWidget
        (SATAPP_SELECTITEM_VIEW));
        if (mSelectItemView) {
            mSelectItemView->setMainWindow(mMainWindow);
            mSelectItemView->initSelectItem(event, *this);
        }
    } else {
        TFLOGSTRING(
        "SATAPP: SatAppUiProvider::SatAppUiProvider DOCML failure!");
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::SatAppUiProvider eixt");
}

// ----------------------------------------------------------------------------
// activeView
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::activeView(const QString &view)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::activeView");
    if(view == "setupmenu_view") {
        mMainWindow.removeView(mMainWindow.currentView());
        mMainWindow.addView(mSetupMenuView);
        mMainWindow.setCurrentView(mSetupMenuView);
        TFLOGSTRING("SATAPP: SatAppUiProvider::activeView setup menu");
    } else {
        mMainWindow.removeView(mMainWindow.currentView());
        mMainWindow.addView(mSelectItemView);
        mMainWindow.setCurrentView(mSelectItemView);
        TFLOGSTRING("SATAPP: SatAppUiProvider::activeView select item");
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::activeView eixt");
}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
SatAppUiProvider::~SatAppUiProvider()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::~SatAppUiProvider call");
    stopShowWaitNote();
    delete mLoader;
    // delete all objects created from DOCML.
    while (!mObjects.isEmpty()) {
        TFLOGSTRING(
        "SATAPP: SatAppUiProvider::~SatAppUiProvider object tree");
        delete mObjects.takeFirst();
    }
    
    if (mDisplayPopup) {
        delete mDisplayPopup;
        mDisplayPopup = 0;
    }
    
    if (mGetInkeyQuery) {
        delete mGetInkeyQuery;
        mGetInkeyQuery = 0;
    }

    if (mYesNoPopup) {
        delete mYesNoPopup;
        mYesNoPopup = 0;
    }

    if (mImmediateQuery) {
        delete mImmediateQuery;
        mImmediateQuery = 0;
    }
    if (mGetInputQuery) {
        delete mGetInputQuery;
        mGetInputQuery = 0;
    }
    if (mConfirmSendQuery) {
        delete mConfirmSendQuery;
        mConfirmSendQuery = 0;
    }

    if (mSetUpCallQuery) {
        delete mSetUpCallQuery;
        mSetUpCallQuery = 0;
    }

    if (mConfirmBipQuery ) {
        delete mConfirmBipQuery ;
        mConfirmBipQuery  = 0;
    }

    if (mWaitNote) {
        delete mWaitNote;
        mWaitNote = 0;
    }

    if (mLoop && mLoop->isRunning()) {
        mLoop->exit();
        delete mLoop;
        mLoop = 0;
        TFLOGSTRING("SatAppUiProvider::~SatAppUiProvider exit loop")
    }

    if (mCallControlMsg) {
        delete mCallControlMsg;
        mCallControlMsg = 0;
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::~SatAppUiProvider exit");
}

// ----------------------------------------------------------------------------
// userPrimaryResponse
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::userPrimaryResponse()
{
    mUserRsp = EUserPrimaryResponse;
    TFLOGSTRING("SATAPP: SatAppUiProvider::userPrimaryResponse call-exit")
}

// ----------------------------------------------------------------------------
// userSecondaryResponse
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::userSecondaryResponse()
{
    mUserRsp = EUserSecondaryResponse;
    TFLOGSTRING("SATAPP: SatAppUiProvider::userSecondaryResponse call-exit")
}

// ----------------------------------------------------------------------------
// showSetUpMenuView
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showSetUpMenuView(
    TSatUiResponse &aRes,
    const QString &aText,
    const QStringList &aMenuItems,
    //const CArrayFixFlat<TSatAction> *aMenuItemNextActions,
    //const HbIcon &aIcon,
    //const CAknIconArray *aItemIconsArray,
    const bool aSelfExplanatoryItems,
    const bool aHelpIsAvailable)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSetUpMenuView call")
    stopShowWaitNote();
    activeView("setupmenu_view");

    // The text is the application name
    // should be shown on the menu area always
    if (!aText.isEmpty()) {
        TFLOGSTRING2("SATAPP: SatAppUiProvider::showSetUpMenuView: title1",
             aText.utf16())
        mSetupMenuView->setTitle(aText);
        TFLOGSTRING2("SATAPP: SatAppUiProvider::showSetUpMenuView: title2",
             aText.utf16())
        mSelectItemView->setTitle(aText);
    }

    mSetupMenuView->showSetUpMenuContent(aRes, aText, aMenuItems,
            aSelfExplanatoryItems, aHelpIsAvailable);
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSetUpMenuView exit")
}

// ----------------------------------------------------------------------------
// showSelectItemView
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showSelectItemView(
    TSatUiResponse &aRes,
    const QString &aText,
    const QStringList &aMenuItems,
    //const CArrayFixFlat<TSatAction> *aMenuItemNextActions,
    const int aDefaultItem,
    unsigned char &aSelection,
    //const HbIcon &aIcon,
    //const CAknIconArray *aItemsIconArray,
    const bool aSelfExplanatoryItems,
    const bool aHelpIsAvailable)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSelectItemView call")
    stopShowWaitNote();
    activeView("selectitem_view");
    mSelectItemView->showSelectItemContent(aRes, aText,
        aMenuItems, aDefaultItem, aSelection,
        aSelfExplanatoryItems, aHelpIsAvailable);
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSelectItemView call")
}


// ----------------------------------------------------------------------------
// showDisplayTextPopup
// ----------------------------------------------------------------------------
//
TSatAppUserResponse SatAppUiProvider::showDisplayTextPopup(
    const QString &aHeading,
    const QString &aContent,
    const int aDuration)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showDisplayTextPopup call")
    resetUserResponse();
    stopShowWaitNote();
    
    if (mDisplayPopup){
        delete mDisplayPopup;
        mDisplayPopup = NULL;
    }
    mDisplayPopup = new HbMessageBox(HbMessageBox::MessageTypeInformation);

    if (mDisplayPopup) {
        TFLOGSTRING(
        "SATAPP: SatAppUiProvider::showDisplayTextPopup note created")

        // Set the label as heading widget
        HbLabel *lHeading = new HbLabel(aHeading, mDisplayPopup);
        mDisplayPopup->setHeadingWidget(lHeading);
        mDisplayPopup->setText(aContent);
        mDisplayPopup->setIconVisible(false);
        composeDialog(mDisplayPopup, aDuration, ESatDialogDisplayText);
        TFLOGSTRING(
        "SATAPP: SatAppUiProvider::showDisplayTextPopup duration before open")
        DialogWaiter waiter;
        mDisplayPopup->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING(
        "SATAPP: SatAppUiProvider::showDisplayTextPopup duration end open")
#ifndef __WINS__
        delete mDisplayPopup;
        mDisplayPopup = 0;
#endif
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::showDisplayTextPopup exit")
    return mUserRsp;
}

// ----------------------------------------------------------------------------
// showGetInkeyQuery
// ----------------------------------------------------------------------------
//
TSatAppUserResponse SatAppUiProvider::showGetInkeyQuery(
    const QString &aHeading,
    QString &aContent,
    const TSatCharacterSet aCharacterSet,
    unsigned int &aDuration)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInkeyQuery call")

    resetUserResponse();
    stopShowWaitNote();
    
    if (mGetInkeyQuery){
        delete mGetInkeyQuery;
        mGetInkeyQuery = NULL;
    }
    mGetInkeyQuery = new HbInputDialog();
    if (mGetInkeyQuery) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInkeyQuery note created")
        // Set PromptText
        mGetInkeyQuery->setPromptText(aHeading);
        // Set ContentText
        QVariant vContent(aContent);
        mGetInkeyQuery->setValue(vContent);
        HbEditorInterface inputMode(mGetInkeyQuery->lineEdit());
        mGetInkeyQuery->lineEdit()->setFocus();
        if (ESatDigitOnly == aCharacterSet) {
            // digit mode, digits only (0 9, *, #, and +)
            inputMode.setFilter(HbPhoneNumberFilter::instance());
         } else {
            // char mode
            inputMode.setUpAsLatinAlphabetOnlyEditor();
        }
        composeDialog(mGetInkeyQuery, aDuration, ESatDialogGetInkey);
        mGetInkeyQuery->lineEdit()->setMaxLength(1);
        connect(mGetInkeyQuery->lineEdit(), SIGNAL(textChanged(QString)),
            this, SLOT(updateQueryAction(QString)));
        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInkeyQuery before pop")
        DialogWaiter waiter;
        mGetInkeyQuery->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInkeyQuery end pop")

        QString inputString = (mGetInkeyQuery->value()).toString();
        aContent = inputString;
        
#ifndef __WINS__
        delete mGetInkeyQuery;
        mGetInkeyQuery = 0;
#endif
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInkeyQuery exit")
    return mUserRsp;
}

// ----------------------------------------------------------------------------
// showGetYesNoQuery
// ----------------------------------------------------------------------------
//
int SatAppUiProvider::showGetYesNoQuery(
    const QString &aText,
    const TSatCharacterSet aCharacterSet,
    unsigned int &aInkey,
    unsigned int &aDuration,
    const bool aImmediateDigitResponse)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery call")
    resetUserResponse();
    stopShowWaitNote();
    TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetYesNoQuery duration in=%d",
            aDuration)
    if (ESatYesNo == aCharacterSet) {
        if (mYesNoPopup){
            delete mYesNoPopup;
            mYesNoPopup = 0;
       }
        mYesNoPopup = new HbMessageBox(HbMessageBox::MessageTypeInformation);

        if (mYesNoPopup) {
            TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery normal")
            // Set the label as heading widget
            HbLabel *lHeading = new HbLabel(aText, mYesNoPopup);
            mYesNoPopup->setHeadingWidget(lHeading);
            mYesNoPopup->setIconVisible(false);

            composeDialog(mYesNoPopup, aDuration, ESatDialogGetYesNo);

            TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery before open")
            DialogWaiter waiter;
            mYesNoPopup->open(&waiter, SLOT(done(HbAction *)));
            waiter.wait();
            TFLOGSTRING( "SATAPP: SatAppUiProvider::showGetYesNoQuery after open")
            mYesNoPopup->close();
        }
    } else if (aImmediateDigitResponse) {
        if (mImmediateQuery) {
            delete mImmediateQuery;
            mImmediateQuery = 0;
        }
        mImmediateQuery = new HbInputDialog();
        if (mImmediateQuery) {
            mImmediateQuery->setPromptText(aText);
            // user can choose the charactor or digital , and only cancel
            // key press, then close current dialog
            TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery immediate")
            composeDialog(mImmediateQuery, aDuration, ESatDialogGetDigitalRsp);
            bool ret = connect(mImmediateQuery->lineEdit(), 
               SIGNAL(textChanged(QString)),
               this, SLOT(updateQueryAction(QString)));
            TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetYesNoQuery:\
               connect updateQueryAction=%d", ret)
            // digit mode, digits only (0 9, *, #, and +)
            HbEditorInterface inputMode(mImmediateQuery->lineEdit());
            inputMode.setFilter(HbPhoneNumberFilter::instance());
            if (mLoop) {
                TFLOGSTRING("SatAppUiProvider::showGetYesNoQuery delete loop")
                delete mLoop;
                mLoop = 0;
            }
            if (mTimer) {
                delete mTimer;
                mTimer = 0;
                TFLOGSTRING("SatAppUiProvider::showGetYesNoQuery delete timer")
            }
            mTimer = new QTimer(this);
            mTimer->start(aDuration);
            ret = connect(mTimer, SIGNAL(timeout()), mTimer, SLOT(stop()));
            TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetYesNoQuery connect\
                mTimer stop: %d", ret)
            mLoop = new QEventLoop(this);
            ret = connect(mTimer, SIGNAL(timeout()), mLoop, SLOT(quit()));
            TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery befor pop")
            mImmediateQuery->open();
            mLoop->exec();
            TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery end pop")
            if (mTimer->isActive()) {
                TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery stop time")
                mTimer->stop();
            }
            aInkey = mDigitalRsp;
            mImmediateQuery->close();
        }
    }
    return  mUserRsp;
}

// ----------------------------------------------------------------------------
// showGetInputQuery
// ----------------------------------------------------------------------------
//
TSatAppUserResponse SatAppUiProvider::showGetInputQuery(
    const QString &heading,
    QString &content,
    const TSatCharacterSet characterSet,
    const int minLength,
    const int maxLength,
    const bool aHideInput)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInputQuery call")
    TFLOGSTRING3("SATAPP: SatAppUiProvider::showGetInputQuery \
                  min %d max %d length", minLength, maxLength)
    resetUserResponse();
    stopShowWaitNote();
    mMinLength = minLength;
    
    if (mGetInputQuery){
        delete mGetInputQuery;
        mGetInputQuery = NULL;
    }    
    mGetInputQuery = new HbInputDialog();
    mGetInputQuery->setPromptText(heading);
    QVariant vContent(content);
    mGetInputQuery->setValue(vContent);
    HbEditorInterface inputMode(mGetInputQuery->lineEdit());
    mGetInputQuery->lineEdit()->setFocus();
    if (ESatDigitOnly == characterSet) {
        // digit mode, digits only (0 9, *, #, and +)
        inputMode.setFilter(HbPhoneNumberFilter::instance());
    } else {
        // char mode
        inputMode.setUpAsLatinAlphabetOnlyEditor();
    }
    
    mGetInputQuery->lineEdit()->setMaxLength(maxLength);

    composeDialog(mGetInputQuery, KDefaultSelectionTimeoutMseconds, ESatDialogGetInput);
    // Sets the "OK"-action/button
    if ((0 == minLength || content.length() >= minLength) && 
         mGetInputQuery->actions().at(0)) {
        mGetInputQuery->actions().at(0)->setEnabled(true);
    } 

    if (aHideInput) {
        mGetInputQuery->lineEdit()->setEchoMode(HbLineEdit::Password);
        
        bool res = connect(mGetInputQuery->lineEdit(), 
            SIGNAL(contentsChanged()), this, SLOT(contentChanged()));
        TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetInputQuery \
                     connect %d",res);
    }else {
        connect(mGetInputQuery->lineEdit(), SIGNAL(textChanged(QString)),
            this, SLOT(updateQueryAction(QString)));        
    }

    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInputQuery before pop")
    DialogWaiter waiter;
    mGetInputQuery->open(&waiter, SLOT(done(HbAction *)));
    waiter.wait();
    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInputQuery end pop")

    content = (mGetInputQuery->value()).toString();
    
#ifndef __WINS__
    delete mGetInputQuery;
    mGetInputQuery = 0;
#endif

    TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetInputQuery mUserRsp =%d exit",
            mUserRsp)
    return mUserRsp;
}

// ----------------------------------------------------------------------------
// SatAppInputProvider::contentChanged
// Called when editor field is modified
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::contentChanged()
{
    if (mGetInputQuery) {
        QString content = (mGetInputQuery->value()).toString();
        updateQueryAction(content);
    }
}

// ----------------------------------------------------------------------------
// Called when editor field is modified
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::updateQueryAction(QString text)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::updateQueryAction call")
    // Get Input
    if (mGetInputQuery && mGetInputQuery->actions().at(0)) {
        if (text.length() >= mMinLength ) {
            mGetInputQuery->actions().at(0)->setEnabled(true);
        } else {
            mGetInputQuery->actions().at(0)->setEnabled(false);
        }
    }

    // Get InKey
    if (mGetInkeyQuery && mGetInkeyQuery->actions().at(0)) {
        if (1 == text.length()) {
            mGetInkeyQuery->actions().at(0)->setEnabled(true);
        } else {
            mGetInkeyQuery->actions().at(0)->setEnabled(false); 
        }
    }
    if (mImmediateQuery) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::updateQueryAction \
            digital immediate")
        immediateResponse(text);
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::updateQueryAction exit")
}

// ----------------------------------------------------------------------------
// showConfirmSendQuery
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showConfirmSendQuery(
    const QString &aText,
    bool &aActionAccepted)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmSendQuery call")
    resetUserResponse();
    
    if (mConfirmSendQuery){
        delete mConfirmSendQuery;
        mConfirmSendQuery = NULL;
    }
    mConfirmSendQuery = new HbMessageBox(HbMessageBox::MessageTypeQuestion);
    if(mConfirmSendQuery) {
        mConfirmSendQuery->setText(aText);
        composeDialog(mConfirmSendQuery, 0, ESatDialogConfirmSend);

        TFLOGSTRING("SATAPP: SatAppUiProvider::confirmSend before open")
        DialogWaiter waiter;
        mConfirmSendQuery->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING("SATAPP: SatAppUiProvider::confirmSend after open")
        
#ifndef __WINS__
        delete mConfirmSendQuery;
        mConfirmSendQuery = 0;
#endif
    }
    aActionAccepted = (EUserPrimaryResponse == mUserRsp) ? true : false;
    TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmSendQuery exit")
}

// ----------------------------------------------------------------------------
// showSmsWaitNote
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showSmsWaitNote(const QString &aText)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSmsWaitNote")
    if (mWaitNote) {
        delete mWaitNote;
        mWaitNote = 0;
    }
    mWaitNote = new HbProgressDialog(HbProgressDialog::WaitDialog);
    //remove the default cancel softkey
    if (mWaitNote){
        mWaitNote->clearActions();
        if(aText.isEmpty()) {
            mWaitNote->setText(hbTrId("txt_sat_sendingtextmessage"));
        } else {
            mWaitNote->setText(aText);
        }
        mWaitNote->show();
        // Extend showing sms wait note  timer for test
        extendNoteShowtime();        
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSmsWaitNote exit")
 }

// ----------------------------------------------------------------------------
// showDtmfWaitNote
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showDtmfWaitNote(
    TSatUiResponse &aRes,
    const QString &aText)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showDtmfWaitNote call")
    aRes = ESatSuccess;

    if (mWaitNote) {
        delete mWaitNote;
        mWaitNote = 0;
    }

    mWaitNote = new HbProgressDialog(HbProgressDialog::WaitDialog);
    //remove the default cancel softkey
    bool ret = connect(mWaitNote->actions().at(0), SIGNAL(triggered()),
                       this, SLOT(cancelResponse()));
    TFLOGSTRING2("SATAPP: SatAppUiProvider::showDtmfWaitNote \
    primaryAction=%d", ret)
    if(aText.isEmpty()) {
        mWaitNote->setText(hbTrId("txt_sat_senddtmf_wait_note"));
    } else {
        mWaitNote->setText(aText);
    }
    mWaitNote->show();
    // Extend showing sms wait note  timer for test
    extendNoteShowtime();
    TFLOGSTRING("SATAPP: SatAppUiProvider::showDtmfWaitNote exit")
 }

// ----------------------------------------------------------------------------
//stopShowWaitNote
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::stopShowWaitNote()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::stopShowWaitNote call")

    if (mWaitNote) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::mWaitNote close")
        mWaitNote->close();
        delete mWaitNote;
        mWaitNote = 0;
    }

    if (mCallControlMsg) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::mCallControlMsg close")
        mCallControlMsg->close();
        delete mCallControlMsg;
        mCallControlMsg = 0;
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::stopShowWaitNote exit")
}

// ----------------------------------------------------------------------------
// showConfirmSetUpCallQUery
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showConfirmSetUpCallQuery(
    const QString &aText,
    const QString &aSimAppName,
    bool &aActionAccepted)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmSetUpCallQUery call")
    resetUserResponse();

    QString alphaId;
    if (aText.length() == 0){
        if (aSimAppName.length() > 0){
            alphaId.append(aSimAppName);
        } else {
            alphaId.append(hbTrId("txt_sat_title"));
        }
        alphaId.append(hbTrId("txt_sat_setupcall_confirm_note"));
        TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmSetUpCallQUery length 0")
    } else {
        alphaId.append(aText);
        TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmSetUpCallQUery exit")
    }
    if (mSetUpCallQuery){
        delete mSetUpCallQuery;
        mSetUpCallQuery = NULL;
    }
    
    mSetUpCallQuery = new HbMessageBox(HbMessageBox::MessageTypeQuestion);
    if(mSetUpCallQuery) {
        mSetUpCallQuery->setText(alphaId);
        composeDialog(mSetUpCallQuery, 0, ESatDialogSetUpCall);

        TFLOGSTRING("SATAPP: SatAppSetUpCall::showSetUpCallConfirm before open")
        DialogWaiter waiter;
        mSetUpCallQuery->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING("SATAPP: SatAppSetUpCall::showSetUpCallConfirm after open")
        
#ifndef __WINS__
        delete mSetUpCallQuery;
        mSetUpCallQuery = 0;
#endif
    }
    aActionAccepted = (EUserPrimaryResponse == mUserRsp) ? true : false;
    TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmSetUpCallQUery exit")
}

// ----------------------------------------------------------------------------
// clearScreen
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::clearScreen()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen called")
    if (mDisplayPopup) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen DisplayText")
        mDisplayPopup->close();
        mDisplayPopup = NULL;
    }
    if (mGetInkeyQuery) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen GetInkey")
        mGetInkeyQuery->close();
        mGetInkeyQuery = NULL;
    }

    if (mYesNoPopup) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen mYesNoPopup")
        mYesNoPopup->close();
        delete mYesNoPopup;
        mYesNoPopup = 0;
    }

    if (mImmediateQuery) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen mImmediateQuery")
        mImmediateQuery->close();
        mImmediateQuery = NULL;
    }

    if (mGetInputQuery) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen GetInput")
        mGetInputQuery->close();
        mGetInputQuery = NULL;
    }

    if(mConfirmSendQuery) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen ConfirmSend")
        mConfirmSendQuery->close();
        mConfirmSendQuery = NULL;
    }

    if (mSetUpCallQuery) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen SetUpCall")
        mSetUpCallQuery->close();
        mSetUpCallQuery = NULL;
    }

    if (mCallControlMsg){
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen CallControlMsg")
        mCallControlMsg->close();
        mCallControlMsg = NULL;
    }

    if (mConfirmBipQuery){
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen ConfirmBip")
        mConfirmBipQuery->close();
        mConfirmBipQuery = NULL;
    }

    if (mWaitNote){
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen WaitNote")
        mWaitNote->close();
        mWaitNote = NULL;
    }
    mUserRsp = EUserClearResponse;
    TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen exit")
}

// ----------------------------------------------------------------------------
// closeUi
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::closeUi()
    {
    TFLOGSTRING("SATAPP: SatAppUiProvider::closeUi called")
    clearScreen();
    mUserRsp = EUserCloseResponse;
    QCoreApplication::instance()->quit();
    TFLOGSTRING("SATAPP: SatAppUiProvider::closeUi exit")
}

// ----------------------------------------------------------------------------
// resetUserResponse
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::resetUserResponse()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::resetUserResponse call")
    mUserRsp = EUserNoResponse;
    mDigitalRsp = Qt::Key_unknown;
    mMinLength = 0;
    TFLOGSTRING("SATAPP: SatAppUiProvider::resetUserResponse exit")
}

// ----------------------------------------------------------------------------
// composeDialog: make sure dlg has 2 buttons
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::composeDialog(
    HbDialog *dlg, 
    int aDuration,
    TSatAppDialogActionType type, 
    bool aModal/* = true*/,
    HbDialog::DismissPolicy aDismissPolicy/* = HbDialog::NoDismiss*/)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog call")

    dlg->setModal(aModal);
    dlg->setDismissPolicy(aDismissPolicy);
    if (aDuration){
        dlg->setTimeout(aDuration);
    } else {
        dlg->setTimeout(HbDialog::NoTimeout);
    }
    
    dlg->clearActions();
    HbAction *primaryAction = new HbAction(dlg);
    dlg->addAction(primaryAction);
    bool ret = connect(primaryAction, SIGNAL(triggered()),
                       this, SLOT(userPrimaryResponse()));
    TFLOGSTRING2("SATAPP: SatAppUiProvider::composeDialog \
        primaryAction=%d", ret)   
    
    HbAction *secondaryAction = new HbAction(dlg);
    dlg->addAction(secondaryAction);
    ret = connect(secondaryAction, SIGNAL(triggered()),
                  this, SLOT(userSecondaryResponse()));
    TFLOGSTRING2("SATAPP: SatAppUiProvider::composeDialog \
        secondaryAction=%d", ret)
    
    if (ESatDialogDisplayText == type){
        primaryAction->setText(hbTrId("txt_sat_general_ok"));
        secondaryAction->setText(hbTrId("txt_sat_general_back"));
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogDisplayText")
    } else if (ESatDialogGetInput == type){
        primaryAction->setText(hbTrId("txt_sat_general_ok"));
        primaryAction->setEnabled(false);
        secondaryAction->setText(hbTrId("txt_sat_general_cancel"));
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogGetInput")
    } else if (ESatDialogGetInkey == type){
        primaryAction->setText(hbTrId("txt_sat_general_ok"));
        primaryAction->setEnabled(false);
        secondaryAction->setText(hbTrId("txt_sat_general_cancel"));
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogGetInkey")
    } else if (ESatDialogGetYesNo == type){
        primaryAction->setText(hbTrId("txt_sat_general_yes"));
        secondaryAction->setText(hbTrId("txt_sat_general_no"));
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogGetYesNo")
    } else if (ESatDialogGetDigitalRsp == type){
        dlg->removeAction(primaryAction);
        secondaryAction->setText(hbTrId("txt_sat_general_cancel"));
        ret = connect(secondaryAction, SIGNAL(triggered()),
                      this, SLOT(handleImmediateCancel()));
        TFLOGSTRING2("SATAPP: SatAppUiProvider::composeDialog \
            con secondaryAction=%d", ret)
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogGetDigitalRsp")
    } else if (ESatDialogSetUpCall == type){
        primaryAction->setText(hbTrId("txt_sat_general_call"));
        secondaryAction->setText(hbTrId("txt_sat_general_cancel"));
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogSetUpCall")
    } else if (ESatDialogConfirmSend == type){
        primaryAction->setText(hbTrId("txt_sat_general_send"));
        secondaryAction->setText(hbTrId("txt_sat_general_cancel"));
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogConfirmSend")
    } else if (ESatDialogConfirmBip == type){
        primaryAction->setText(hbTrId("txt_sat_general_ok"));
        secondaryAction->setText(hbTrId("txt_sat_general_cancel"));
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogConfirmBip")
    }
    
    TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog exit")
}

// ----------------------------------------------------------------------------
// docmlLoader
// ----------------------------------------------------------------------------
//
SatAppDocumentLoader *SatAppUiProvider::docmlLoader()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::docmlLoader call-exit");
    return mLoader;
}

// ----------------------------------------------------------------------------
// createObject
// ----------------------------------------------------------------------------
//
QObject *SatAppDocumentLoader::createObject(
    const QString &type,
    const QString &name)
{
    TFLOGSTRING("SATAPP: SatAppDocumentLoader::createObject")
    if (type == SatAppView::staticMetaObject.className()) {
        QObject *object = new SatAppView();
        TFLOGSTRING2(
        "SATAPP: SatAppDocumentLoader::createObject %S", name.utf16())
        object->setObjectName(name);
        TFLOGSTRING("SATAPP: SatAppDocumentLoader::createObject sat view exit")
        return object;
        }
    TFLOGSTRING("SATAPP: SatAppDocumentLoader::createObject other exit")
    return HbDocumentLoader::createObject(type, name);
}

// ----------------------------------------------------------------------------
//extendNoteShowtime
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::extendNoteShowtime()
{
    TFLOGSTRING("SatAppUiProvider::extendNoteShowtime")
    if (mTimer) {
        delete mTimer;
        mTimer = 0;
        TFLOGSTRING("SatAppUiProvider::extendNoteShowtime delete timer")
    }
    if (mLoop) {
        delete mLoop;
        mLoop = 0;
        TFLOGSTRING("SatAppUiProvider::extendNoteShowtime delete loop")
    }
    mTimer = new QTimer(this);
    mTimer->start(KExtendTimeoutMseconds);
    bool ret = connect(mTimer, SIGNAL(timeout()), mTimer, SLOT(stop()));
    TFLOGSTRING2("SATAPP: SatAppUiProvider::extendNoteShowtime connect\
        mTimer stop: %d", ret)

    mLoop = new QEventLoop(this);
    ret = connect(mTimer, SIGNAL(timeout()), mLoop, SLOT(quit()));
    TFLOGSTRING2("SATAPP: SatAppUiProvider::extendNoteShowtime connect \
       syn: %d", ret)
    mLoop->exec(QEventLoop::AllEvents);
    TFLOGSTRING("SatAppUiProvider::extendNoteShowtime exit")
}

// ----------------------------------------------------------------------------
//cancelResponse
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::cancelResponse()
{
    TFLOGSTRING("SatAppUiProvider::cancelResponse")
    if (mWaitNote) {
        TFLOGSTRING("SatAppUiProvider::cancelResponse user cancel")
        if (mTimer) {
            mTimer->stop();
            TFLOGSTRING("SatAppUiProvider::cancelResponse time out")
        }
        if (mLoop && mLoop->isRunning()) {
            mLoop->exit();
            TFLOGSTRING("SatAppUiProvider::cancelResponse exit loop")
        }
        mWaitNote->setEnabled(false);
        mWaitNote->close();
        emit userCancelResponse();
    }
    TFLOGSTRING("SatAppUiProvider::cancelResponse exit")
}

// ----------------------------------------------------------------------------
//showSsWaitNote
// Displays a wait note to indicate SS sending.
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showSsWaitNote(const QString &aText,
        const bool aSelfExplanatoryIcon)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSsWaitNote")
    if (mWaitNote) {
        delete mWaitNote;
        mWaitNote = 0;
    }
    mWaitNote = new HbProgressDialog(HbProgressDialog::WaitDialog);
    mWaitNote->clearActions();
    if (aText.length()) {
        if (!aSelfExplanatoryIcon)  {
            TFLOGSTRING("SATAPP: SatAppUiProvider::showSsWaitNote !Icon")
            mWaitNote->setText(aText);
        } else {
            TFLOGSTRING("SATAPP: SatAppUiProvider::showSsWaitNote send")
            mWaitNote->setText(hbTrId("txt_sat_sendss_wait_note_send"));
        }    
    }  else {
        mWaitNote->setText(hbTrId("txt_sat_sendss_wait_note_request"));
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSsWaitNote show before")
    mWaitNote->show();
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSsWaitNote exit")
}

// ----------------------------------------------------------------------------
//showWaitNoteWithoutDelay
// 
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showWaitNoteWithoutDelay()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showWaitNoteWithoutDelay")
    if (mWaitNote) {
        delete mWaitNote;
        mWaitNote = 0;
    }
    mWaitNote = new HbProgressDialog(HbProgressDialog::WaitDialog);
    mWaitNote->clearActions();
    mWaitNote->setText(hbTrId("txt_sat_wait_note_without_delay"));
    mWaitNote->show();
    TFLOGSTRING("SATAPP: SatAppUiProvider::showWaitNoteWithoutDelay exit")
}

// ----------------------------------------------------------------------------
//showSsErrorNote
// 
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showSsErrorNote()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSsErrorNote")
    HbMessageBox::warning(hbTrId("txt_sat_sendss_error_note"));
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSsErrorNote exit")
}

// ----------------------------------------------------------------------------
// showConfirmOpenChannelQuery
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showConfirmOpenChannelQuery(
        const QString &aText,
        bool &aActionAccepted)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmOpenChannelQuery call")
    stopShowWaitNote();
    if (mConfirmBipQuery){
        delete mConfirmBipQuery;
        mConfirmBipQuery = NULL;
    }
    QString title = aText;
    if (!aText.length()){
        title = hbTrId("txt_sat_openchannel_confirm_note");
    }

    mConfirmBipQuery = new HbMessageBox(HbMessageBox::MessageTypeQuestion);
    if(mConfirmBipQuery) {
        // Sets the "Yes"-action/button
        mConfirmBipQuery->setText(title);    
        composeDialog(mConfirmBipQuery, 0, ESatDialogConfirmBip);

        TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmOpenChannelQuery before open")
        DialogWaiter waiter;
        mConfirmBipQuery->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmOpenChannelQuery after open")

        delete mConfirmBipQuery;
        mConfirmBipQuery = 0;
    }

    aActionAccepted = (EUserPrimaryResponse == mUserRsp) ? true : false;
    
    TFLOGSTRING("SATAPP: SatAppUiProvider::showConfirmOpenChannelQuery exit")
}

// ----------------------------------------------------------------------------
//showCallControlNote
// 
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showCallControlNote(const QString &aText)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showCallControlNote")
    if (mCallControlMsg) {
        delete mCallControlMsg;
        mCallControlMsg = 0;
    }
    mCallControlMsg = new HbDeviceMessageBox(HbMessageBox::MessageTypeInformation);
    mCallControlMsg->setText(aText);
    mCallControlMsg->show();
    // Extend showing sms wait note  timer for test
    extendNoteShowtime();
    TFLOGSTRING("SATAPP: SatAppUiProvider::showCallControlNote exit")

}

// ----------------------------------------------------------------------------
// showBIPWaitNote
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showBIPWaitNote(const QString &aText)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showBIPWaitNote call")
    if (!mWaitNote) {
        mWaitNote = new HbProgressDialog(HbProgressDialog::WaitDialog);
        mWaitNote->setText(aText);

        //remove the default cancel softkey
        bool ret = connect(mWaitNote->actions().at(0), SIGNAL(triggered()),
                           this, SLOT(cancelResponse()));
        TFLOGSTRING2("SATAPP: SatAppUiProvider::showDtmfWaitNote \
        primaryAction=%d", ret)
    
        mWaitNote->show();
    } else {
        mWaitNote->setText(aText);
        TFLOGSTRING("SATAPP: SatAppUiProvider::showBIPWaitNote Update Text")
    }    
    TFLOGSTRING("SATAPP: SatAppUiProvider::showBIPWaitNote exit")
}

// ----------------------------------------------------------------------------
//showMoSmControlNote
//
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showMoSmControlNote(const QString &aText)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showMoSmControlNote")
    HbMessageBox::information(aText);
    TFLOGSTRING("SATAPP: SatAppUiProvider::showMoSmControlNote exit")

}

// ----------------------------------------------------------------------------
// showCloseChannelWaitNote
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showCloseChannelWaitNote(const QString &aText)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showCloseChannelWaitNote call")
    if (mWaitNote) {
        delete mWaitNote;
        mWaitNote = 0;
    }   
    mWaitNote = new HbProgressDialog(HbProgressDialog::WaitDialog);
    mWaitNote->clearActions();
    mWaitNote->setText(aText);
    mWaitNote->show();
    TFLOGSTRING("SATAPP: SatAppUiProvider::showCloseChannelWaitNote exit")
}

// ----------------------------------------------------------------------------
//showSatInfoNote
//
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::showSatInfoNote(const QString &aText)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSatInfoNote")
    HbMessageBox::information(aText);
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSatInfoNote exit")
}


// ----------------------------------------------------------------------------
//handleImmediateCancel
//
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::handleImmediateCancel()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::handleImmediateCancel")
    if (mLoop && mLoop->isRunning()) {
        mLoop->quit();
    }
    TFLOGSTRING("SATAPP: SatAppUiProvider::handleImmediateCancel exit")
}

// ----------------------------------------------------------------------------
//immediateResponse
//
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::immediateResponse(QString text)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse")
    if (mLoop && mLoop->isRunning()) {
        mLoop->quit();
    }
    if ("0" == text) {
        mDigitalRsp = Qt::Key_0;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 0")
    } else if ("1" == text) {
        mDigitalRsp = Qt::Key_1;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 1")
    } else if ("2" == text) {
        mDigitalRsp = Qt::Key_2;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 2")
    } else if ("3" == text) {
        mDigitalRsp = Qt::Key_3;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 3")
    } else if ("4" == text) {
        mDigitalRsp = Qt::Key_4;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 4")
    }else if ("5" == text) {
        mDigitalRsp = Qt::Key_5;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 5")
    } else if ("6" == text) {
        mDigitalRsp = Qt::Key_6;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 6")
    } else if ("7" == text) {
        mDigitalRsp = Qt::Key_7;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 7")
    } else if ("8" == text) {
        mDigitalRsp = Qt::Key_8;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 8")
    } else if ("9" == text) {
        mDigitalRsp = Qt::Key_9;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse 9")
    } else if ("+" == text) {
        mDigitalRsp = Qt::Key_Plus;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse +")
    } else if ("*" == text) {
        mDigitalRsp = Qt::Key_Asterisk;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse *")
    } else if ("#" == text) {
        mDigitalRsp = Qt::Key_NumberSign;
        TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse #")
    }
    mUserRsp = EUserPrimaryResponse;
    TFLOGSTRING2("SATAPP: SatAppUiProvider::immediateResponse key %x",
        mDigitalRsp)

    TFLOGSTRING("SATAPP: SatAppUiProvider::immediateResponse exit")
}

// End of file
