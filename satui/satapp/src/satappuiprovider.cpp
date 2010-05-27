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
#include "satappgetinkeynote.h" // GetYesNo immediate digit response
#include "satappuiprovider.h"
#include "tflogger.h"

const char *SATAPP_DOCML = ":/xml/satapp.docml";
const char *SATAPP_SETUPMENU_VIEW = "setupmenu_view";
const char *SATAPP_SELECTITEM_VIEW = "selectitem_view";

// ======== MEMBER FUNCTIONS ==================================================

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
    mGetInkeyQuery(0), mYesNoPopup(0), mGetInputQuery(0),
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
    // Create a note
    mDisplayPopup = new HbMessageBox(HbMessageBox::MessageTypeInformation);

    if (mDisplayPopup) {
        TFLOGSTRING(
        "SATAPP: SatAppUiProvider::showDisplayTextPopup note created")

        // Set the label as heading widget
        HbLabel *heading = new HbLabel(aHeading, mDisplayPopup);
        mDisplayPopup->setHeadingWidget(heading);
        mDisplayPopup->setText(aContent);
        composeDialog(mDisplayPopup, aDuration, ESatDialogDisplayText);

        TFLOGSTRING(
        "SATAPP: SatAppUiProvider::showDisplayTextPopup duration before open")
        DialogWaiter waiter;
        mDisplayPopup->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING(
        "SATAPP: SatAppUiProvider::showDisplayTextPopup duration end open")

        delete mDisplayPopup;
        mDisplayPopup = 0;
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
    // Create a query
    mGetInkeyQuery = new HbInputDialog();
    if (mGetInkeyQuery) {
        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInkeyQuery note created")
        // Set PromptText
        mGetInkeyQuery->setPromptText(aHeading);
        // Set ContentText
        QVariant vContent(aContent);
        mGetInkeyQuery->setValue(vContent);
        HbEditorInterface inputMode(mGetInkeyQuery->lineEdit());
        if (ESatDigitOnly == aCharacterSet) {
            // digit mode, digits only (0 9, *, #, and +)
            inputMode.setFilter(HbPhoneNumberFilter::instance());
         } else {
            // char mode
            inputMode.setUpAsLatinAlphabetOnlyEditor();
        }

        unsigned int duration = KDefaultSelectionTimeoutMseconds;
        if (aDuration) {
            duration = aDuration * KSymbianTimeConvertQtTime;
        }
        composeDialog(mGetInkeyQuery, duration, ESatDialogGetInkey);
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
        delete mGetInkeyQuery;
        mGetInkeyQuery = 0;
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
    unsigned int &aDuration,
    const bool aImmediateDigitResponse)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery call")
    resetUserResponse();
    stopShowWaitNote();
    TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetYesNoQuery duration in=%d",
            aDuration)
    unsigned int duration = KDefaultSelectionTimeoutMseconds;
    if (aDuration) {
        duration = aDuration * KSymbianTimeConvertQtTime;
    }
    TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetYesNoQuery duration=%d",
            duration)
    if (ESatYesNo == aCharacterSet) {
        mYesNoPopup = new SatAppGetInkeyNote(aText);
        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery normal")
        composeDialog(mYesNoPopup, duration, ESatDialogGetYesNo);
    } else if (aImmediateDigitResponse){
        // user can choose the charactor or digital , and only cancel
        // key press, then close current dialog
        mYesNoPopup = new SatAppGetInkeyNote(aText);
        composeDialog(mYesNoPopup, duration, ESatDialogGetDigitalRsp);

        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery immediate")
        bool ret = connect(mYesNoPopup, SIGNAL(digitalKeyPressed(int)),
                 this, SLOT(digitalResponse(int)),
                 Qt::DirectConnection);
        TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetYesNoQuery:\
        digitalKeyPressed=%d", ret)
    }
    if (mYesNoPopup) {
        TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetYesNoQuery duration=%d",
                aDuration)
        QTime time;
        time.start();

        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery befor pop")
        DialogWaiter waiter;
        mYesNoPopup->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery end pop")
        aDuration = time.elapsed() / KSymbianTimeConvertQtTime;
        TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetYesNoQuery duration out=%d",
                aDuration)
        delete mYesNoPopup;
        mYesNoPopup = 0;
    }
    if (ESatYesNo == aCharacterSet) {
        mDigitalRsp = mUserRsp;
        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetYesNoQuery:yes/no rsp")
    }
    return  mDigitalRsp;
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
    // Create a query
    mGetInputQuery = new HbInputDialog();
    // Set PromptText
    mGetInputQuery->setPromptText(heading);
    // Set ContentText
    QVariant vContent(content);
    mGetInputQuery->setValue(vContent);
    HbEditorInterface inputMode(mGetInputQuery->lineEdit());
    if (ESatDigitOnly == characterSet) {
        // digit mode, digits only (0 9, *, #, and +)
        inputMode.setFilter(HbPhoneNumberFilter::instance());
    } else {
        // char mode
        inputMode.setUpAsLatinAlphabetOnlyEditor();
    }
    connect(mGetInputQuery->lineEdit(), SIGNAL(textChanged(QString)),
        this, SLOT(updateQueryAction(QString)));
    mGetInputQuery->lineEdit()->setMaxLength(maxLength);

    composeDialog(mGetInputQuery, KDefaultSelectionTimeoutMseconds, ESatDialogGetInput);
    // Sets the "OK"-action/button
    if (0 == minLength || content.length() >= minLength) {
        HbAction* okAction = new HbAction(hbTrId("txt_sat_general_ok"), 
            mGetInputQuery);
        bool ret = connect(okAction, SIGNAL(triggered()),
                            this, SLOT(userPrimaryResponse()));
        TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetInputQuery \
        connect okAction: %d", ret)
        mGetInputQuery->setPrimaryAction(okAction);
    } else {
        // Set default primary action is 0
        mGetInputQuery->setPrimaryAction(0);
    }

    if (aHideInput) {
        mGetInputQuery->lineEdit()->setEchoMode(HbLineEdit::Password);
        TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInputQuery hide")
    }

    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInputQuery before pop")
    DialogWaiter waiter;
    mGetInputQuery->open(&waiter, SLOT(done(HbAction *)));
    waiter.wait();
    TFLOGSTRING("SATAPP: SatAppUiProvider::showGetInputQuery end pop")

    content = (mGetInputQuery->value()).toString();

    delete mGetInputQuery;
    mGetInputQuery = 0;

    TFLOGSTRING2("SATAPP: SatAppUiProvider::showGetInputQuery mUserRsp =%d exit",
            mUserRsp)
    return mUserRsp;
}

// ----------------------------------------------------------------------------
// Called when editor field is modified
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::updateQueryAction(QString text)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::updateQueryAction call")
    // Get Input
    if (mGetInputQuery) {
        if (text.length() >= mMinLength) {
            if (!mGetInputQuery->primaryAction()) {
                HbAction *okAction = new HbAction(hbTrId("txt_sat_general_ok"), 
                    mGetInputQuery);
                bool ret = connect(
                    okAction, SIGNAL(triggered()),
                    this, SLOT(userPrimaryResponse()));
              TFLOGSTRING2("SATAPP: SatAppUiProvider::updateQueryAction \
              get input connect okAction: %d", ret)
              mGetInputQuery->setPrimaryAction(okAction);
            }
            TFLOGSTRING("SATAPP: SatAppUiProvider::updateQueryAction \
                get input  OK")
        } else {
            if (mGetInputQuery->primaryAction()) {
                mGetInputQuery->removeAction(mGetInputQuery->primaryAction());
            }
            TFLOGSTRING("SATAPP: SatAppUiProvider::updateQueryAction \
                get input no OK Action")
        }
    }
    // Get InKey
    if (mGetInkeyQuery) {
        if (!mGetInkeyQuery->primaryAction() && (text.length() == 1)) {
            HbAction *okAction = new HbAction(hbTrId("txt_sat_general_ok"), 
                mGetInkeyQuery);
            bool ret = connect(okAction, SIGNAL(triggered()),
                this, SLOT(userPrimaryResponse()));
            TFLOGSTRING2("SATAPP: SatAppUiProvider::updateQueryAction \
                get inkey  connect okAction: %d", ret)
            mGetInkeyQuery->setPrimaryAction(okAction);
        TFLOGSTRING("SATAPP: SatAppUiProvider::updateQueryAction \
            get inkey OK")
    } else {
        if (mGetInkeyQuery->primaryAction()) {
            mGetInkeyQuery->removeAction(mGetInkeyQuery->primaryAction());
        }
        TFLOGSTRING("SATAPP: SatAppUiProvider::updateQueryAction \
            get inkey not OK Action")
        }
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
    mConfirmSendQuery = new HbMessageBox();
    if(mConfirmSendQuery) {
        mConfirmSendQuery->setText(aText);
        composeDialog(mConfirmSendQuery, 0, ESatDialogConfirmSend);

        TFLOGSTRING("SATAPP: SatAppUiProvider::confirmSend before open")
        DialogWaiter waiter;
        mConfirmSendQuery->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING("SATAPP: SatAppUiProvider::confirmSend after open")

        delete mConfirmSendQuery;
        mConfirmSendQuery = 0;
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
    mWaitNote->removeAction(mWaitNote->primaryAction());
    if(aText.isEmpty()) {
        mWaitNote->setText(hbTrId("txt_sat_sendingtextmessage"));
    } else {
        mWaitNote->setText(aText);
    }
    mWaitNote->show();
    // Extend showing sms wait note  timer for test
    extendNoteShowtime();
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
    bool ret = connect(mWaitNote->primaryAction(), SIGNAL(triggered()),
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

    mSetUpCallQuery = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    if(mSetUpCallQuery) {
        mSetUpCallQuery->setText(alphaId);
        composeDialog(mSetUpCallQuery, 0, ESatDialogSetUpCall);

        TFLOGSTRING("SATAPP: SatAppSetUpCall::showSetUpCallConfirm before open")
        DialogWaiter waiter;
        mSetUpCallQuery->open(&waiter, SLOT(done(HbAction *)));
        waiter.wait();
        TFLOGSTRING("SATAPP: SatAppSetUpCall::showSetUpCallConfirm after open")

        delete mSetUpCallQuery;
        mSetUpCallQuery = 0;
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
        mDisplayPopup->close();
        mDisplayPopup = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen DisplayText")
    }
    if (mGetInkeyQuery) {
        mGetInkeyQuery->close();
        mGetInkeyQuery = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen GetInkey")
    }
    if (mYesNoPopup) {
        mYesNoPopup->close();
        mYesNoPopup = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen GetYesNo")
    }
    if (mGetInputQuery) {
        mGetInputQuery->close();
        mGetInputQuery = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen GetInput")
    }
    if(mConfirmSendQuery) {
        mConfirmSendQuery->close();
        mConfirmSendQuery = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen ConfirmSend")
    }
    if (mSetUpCallQuery) {
        mSetUpCallQuery->close();
        mSetUpCallQuery = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen SetUpCall")
    }
    if (mCallControlMsg){
        mCallControlMsg->close();
        mCallControlMsg = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen CallControlMsg")
    }
    if (mConfirmBipQuery){
        mConfirmBipQuery->close();
        mConfirmBipQuery = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen ConfirmBip")
    }
    if (mWaitNote){
        mWaitNote->close();
        mWaitNote = NULL;
        TFLOGSTRING("SATAPP: SatAppUiProvider::clearScreen WaitNote")
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
// digitalResponse
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::digitalResponse(const int aKey)
{
    TFLOGSTRING("SATAPP: SatAppGetInkey::digitalResponse call")
    mDigitalRsp = aKey;
    if (mYesNoPopup){
        TFLOGSTRING("SATAPP: SatAppUiProvider::digitalResponse Close")
        mYesNoPopup->close();
        }
    TFLOGSTRING("SATAPP: SatAppUiProvider::digitalResponse exit")
}

// ----------------------------------------------------------------------------
// resetUserResponse
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::resetUserResponse()
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::resetUserResponse call")
    mUserRsp = EUserNoResponse;
    mDigitalRsp = 0;
    mMinLength = 0;
    TFLOGSTRING("SATAPP: SatAppUiProvider::resetUserResponse exit")
}

// ----------------------------------------------------------------------------
// composeDialog
// ----------------------------------------------------------------------------
//
void SatAppUiProvider::composeDialog(
    HbDialog *dlg, 
    int aDuration,
    TSatAppDialogActionType type,
    bool aModal,
    HbDialog::DismissPolicy aDismissPolicy)
{
    TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog call")

    dlg->setModal(aModal);
    dlg->setDismissPolicy(aDismissPolicy);
    if (aDuration){
        dlg->setTimeout(aDuration);
    } else {
        dlg->setTimeout(HbDialog::NoTimeout);
    }

    HbAction *primaryAction = 0;
    HbAction *secondaryAction = 0;

    if (ESatDialogDisplayText == type){
        primaryAction = new HbAction(hbTrId("txt_sat_general_ok"), dlg);
        secondaryAction = new HbAction(hbTrId("txt_sat_general_back"), dlg);
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogDisplayText")
    } else if (ESatDialogGetInput == type){
        secondaryAction = new HbAction(hbTrId("txt_sat_general_cancel"), dlg);
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogGetInput")
    } else if (ESatDialogGetInkey == type){
        secondaryAction = new HbAction(hbTrId("txt_sat_general_cancel"), dlg);
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogGetInkey")
    } else if (ESatDialogGetYesNo == type){
        primaryAction = new HbAction(hbTrId("txt_sat_general_yes"), dlg);
        secondaryAction = new HbAction(hbTrId("txt_sat_general_no"), dlg);
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogGetYesNo")
    } else if (ESatDialogGetDigitalRsp == type){
        secondaryAction = new HbAction(hbTrId("txt_sat_general_cancel"), dlg);
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogGetDigitalRsp")
    } else if (ESatDialogSetUpCall == type){
        primaryAction = new HbAction(hbTrId("txt_sat_general_call"), dlg);
        secondaryAction = new HbAction(hbTrId("txt_sat_general_cancel"), dlg);
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogSetUpCall")
    } else if (ESatDialogConfirmSend == type){
        primaryAction = new HbAction(hbTrId("txt_sat_general_send"), dlg);
        secondaryAction = new HbAction(hbTrId("txt_sat_general_cancel"), dlg);
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogConfirmSend")
    }else if (ESatDialogConfirmBip == type){
        primaryAction = new HbAction(hbTrId("txt_sat_general_ok"), dlg);
        secondaryAction = new HbAction(hbTrId("txt_sat_general_cancel"), dlg);
        TFLOGSTRING("SATAPP: SatAppUiProvider::composeDialog \
            ESatDialogConfirmBip")
    }

    if (primaryAction) {
        bool ret = connect(
            primaryAction, SIGNAL(triggered()),
            this, SLOT(userPrimaryResponse()));
        TFLOGSTRING2("SATAPP: SatAppUiProvider::composeDialog \
            primaryAction=%d", ret)
    }
    dlg->setPrimaryAction(primaryAction);

    if (secondaryAction) {
        bool ret = connect(
            secondaryAction, SIGNAL(triggered()),
            this, SLOT(userSecondaryResponse()));
        TFLOGSTRING2("SATAPP: SatAppUiProvider::composeDialog \
            secondaryAction=%d", ret)
    }
    dlg->setSecondaryAction(secondaryAction);

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
    mWaitNote->removeAction(mWaitNote->primaryAction());
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
    mWaitNote->removeAction(mWaitNote->primaryAction());
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
    HbMessageBox *msgBox = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    msgBox->setText(hbTrId("txt_sat_sendss_error_note"));
    DialogWaiter waiter;
    msgBox->open(&waiter, SLOT(done(HbAction *)));
    waiter.wait();
    delete msgBox;
    msgBox = NULL;
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
    QString title = aText;
    if (!aText.length()){
        title = hbTrId("txt_sat_openchannel_confirm_note");
    }

    mConfirmBipQuery = new HbMessageBox();
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
        bool ret = connect(mWaitNote->primaryAction(), SIGNAL(triggered()),
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
    HbMessageBox *msgBox = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    msgBox->setText(aText);
    msgBox->setTimeout(KMoSmControlTimeOut);
    DialogWaiter waiter;
    msgBox->open(&waiter, SLOT(done(HbAction *)));
    waiter.wait();
    delete msgBox;
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
    mWaitNote->removeAction(mWaitNote->primaryAction());
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
    HbMessageBox *msgBox = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    msgBox->setText(aText);
    DialogWaiter waiter;
    msgBox->open(&waiter, SLOT(done(HbAction *)));
    waiter.wait();
    delete msgBox;
    msgBox = 0;
    TFLOGSTRING("SATAPP: SatAppUiProvider::showSatInfoNote exit")
}

// End of file
