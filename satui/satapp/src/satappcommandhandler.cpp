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

#include <QTime>
#include "satappcommandhandler.h"
#include "tflogger.h"
#include "satappcommonconstant.h"
#include "satappuiprovider.h"

// ----------------------------------------------------------------------------
// SatAppCommandHandler::DisplayText
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
SatAppCommandHandler::SatAppCommandHandler(SatAppUiProvider &ui, 
    QObject *parent) : QObject(parent), mUi(ui)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::SatAppCommandHandler call-exit")
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::~SatAppCommandHandler
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
SatAppCommandHandler::~SatAppCommandHandler()
{

}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::setUpMenu
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::setUpMenu(
    TSatUiResponse &aRes,
    const QString &aText,
    const QStringList &aMenuItems,
    //const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    //const HbIcon &aIcon,
    //const CAknIconArray* aItemIconsArray,
    const bool aSelfExplanatoryItems,
    const bool aHelpIsAvailable)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::setUpMenu call")
    mUi.showSetUpMenuView(aRes, aText, aMenuItems, 
            aSelfExplanatoryItems, aHelpIsAvailable);
    TFLOGSTRING("SATAPP: SatAppCommandHandler::setUpMenu exit")
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::selectItem
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::selectItem(
    TSatUiResponse &aRes,
    const QString &aText,
    const QStringList &aMenuItems,
    //const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const int aDefaultItem,
    unsigned char &aSelection,
    //const HbIcon &aIcon,
    //const CAknIconArray* aItemsIconArray,
    const bool aSelfExplanatoryItems,
    const bool aHelpIsAvailable)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::selectItem call")
    mUi.showSelectItemView(aRes, aText, aMenuItems, aDefaultItem, aSelection,
            aSelfExplanatoryItems, aHelpIsAvailable);
    TFLOGSTRING("SATAPP: SatAppCommandHandler::selectItem exit")
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::displayText
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::displayText(TSatUiResponse &aRes,
                       const QString &aText,
                       const QString &aSimApplicationName,
                        //      const HbIcon &aIcon,
                       bool &/*aRequestedIconDisplayed*/,
                       const bool aSustainedText,
                       const int aDuration,
                       const bool aWaitUserToClear)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::displayText call")

    aRes = ESatSuccess;
    QString heading;
    if (!aSimApplicationName.isEmpty()) {
        heading.append(aSimApplicationName);
    } else {
        heading.append(QObject::tr("SAT MESSAGE HEADER"));
    }

    // set time
    int duration(0);

    if (!aDuration) {
        TFLOGSTRING(
        "SATAPP: SatAppCommandHandler::displayText duration not given")
        duration = KDisplayTxtDefaultduration;
        if (aWaitUserToClear) {
            TFLOGSTRING(
            "SATAPP: SatAppCommandHandler::displayText wait user to clear")
            duration = KDisplayTxtUserClearTimeout;
        }
    } else {
        // if duration  defined
        duration = aDuration * 1000;
    }
    TFLOGSTRING2(
    "SATAPP: SatAppCommandHandler::displayText duration: %d", 
    duration)
    TSatAppUserResponse rsp = EUserNoResponse;
    rsp = mUi.showDisplayTextPopup(heading, aText, duration);

    // If user is not expected to clear message used response is success
    if (aWaitUserToClear && !aSustainedText) {
        // User press ok key
        if (EUserPrimaryResponse == rsp || 
            EUserClearResponse == rsp){
            aRes = ESatSuccess;
            TFLOGSTRING(
            "SATAPP: SatAppCommandHandler::DisplayText successful response")
        } else if (EUserSecondaryResponse == rsp) {
                // User press back key
                aRes = ESatBackwardModeRequestedByUser;
        } else {
            TFLOGSTRING(
            "SATAPP: SatAppCommandHandler::DisplayText no user response")
            // User did not close the text, closed by the timer or MT call
            aRes = ESatNoResponseFromUser;
        }
    }

    // request to close by the server
    if (EUserCloseResponse == rsp) {
        aRes = ESatSessionTerminatedByUser;
    }

    // to do
    //End key by user ESatSessionTerminatedByUser
    //rsp = EUserNoResponse;
    TFLOGSTRING("SATAPP: SatAppCommandHandler::displayText exit")
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::getInkey
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::getInkey(TSatUiResponse &aRes,
                                const QString &aTitleText,
                                const TSatCharacterSet aCharacterSet,
                                QString &aInputText,
                                //const QPixmap* /*aIconBitmapGetInput*/,
                                const bool /*aSelfExplanatory*/,
                                unsigned int &aDuration)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::getInkey")
    aRes = ESatSuccess;
    // the timeout expires The timer starts when the text is
    //displayed on the screen and stops when the TERMINALRESPONSE is sent.
    QTime time;
    if (aDuration) {
        TFLOGSTRING2("SATAPP: SatAppCommandHandler::getInkey\
        in aDuration: %d", aDuration)
        time.start();
    }

    QString heading;
    if (aTitleText.isEmpty()) {
        if (ESatDigitOnly == aCharacterSet) {
            TFLOGSTRING("SATAPP:SatAppCommandHandler::getInkey: ESatDigitOnly")
            heading = tr("Enternumber (1 digit 0-9) ");
        } else {
            // ESatCharSmsDefaultAlphabet or ESatCharUcs2Alphabet
            TFLOGSTRING("SatAppCommandHandler::getInkey others")
            heading = tr("Enter character ");
        }
     } else {
         heading = aTitleText;
         TFLOGSTRING("SatAppCommandHandler::getInkey: Using aTitleText")
     }
   //Duration will be implemented in GetInkey proactive command
    TSatAppUserResponse rsp = EUserNoResponse;
    rsp = mUi.showGetInkeyQuery(heading, aInputText, aCharacterSet, aDuration);

    if (aDuration) {
        aDuration = time.elapsed() / 100;
        TFLOGSTRING2("SATAPP: SatAppCommandHandler::getInkey\
                return for server aDuration: %d", aDuration)
    }
    // User press OK key
    if (EUserPrimaryResponse == rsp) {
        aRes = ESatSuccess;
        TFLOGSTRING("SATAPP: SatAppCommandHandler::getInkey successful response")
    } else if (EUserSecondaryResponse == rsp)
        {
        // User press back key
        aRes = ESatBackwardModeRequestedByUser;
    } else {
        TFLOGSTRING("SATAPP: SatAppCommandHandler::getInkey no user response")
        // User did not close the text, closed by the timer or MT call
        aRes = ESatNoResponseFromUser;
    }

    TFLOGSTRING("SATAPP: SatAppCommandHandler::getInkey exit")
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::getYesNo
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::getYesNo(
        TSatUiResponse &aRes,
        const QString &aText,
        const TSatCharacterSet aCharacterSet,
        unsigned int &aInkey,
        //const TSatIconId &aIconId,
        const bool &/*aSelfExplanatory*/,
        unsigned int &aDuration,
        const bool aImmediateDigitResponse)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::getYesNo")
    TFLOGSTRING2("SATAPP: SatAppCommandHandler::getYesNo: immediate  %d",
    aImmediateDigitResponse)

    // the timeout expires The timer starts when the text is
    //displayed on the screen and stops when the TERMINALRESPONSE is sent.
    QTime time;
    if (aDuration) {
        TFLOGSTRING2("SATAPP: SatAppUiProvider::getYesNo in aDuration: %d",
        aDuration)
        time.start();
    }

    aRes = ESatSuccess;
    int rsp = -1;
    rsp = mUi.showGetYesNoQuery(aText, aCharacterSet, 
            aDuration, aImmediateDigitResponse);
    // User press OK key
    if (EUserPrimaryResponse == rsp) {
        aRes = ESatSuccess;
        aInkey = 1;
        TFLOGSTRING("SATAPP: SatAppCommandHandler::getYesNo successful response")
    } else if (EUserSecondaryResponse == rsp)
        {
        // User press NO key
        aRes = ESatSuccess;
        aInkey = 0;
        TFLOGSTRING("SATAPP: SatAppCommandHandler::getYesNo No request by user")
    } else if (ESatYesNo != aCharacterSet && !rsp) {
        aRes = ESatSuccess;
    }
    // else if {
    //    // User press end/back key
    //    aRes = ESatBackwardModeRequestedByUser;
    //    aInkey = 1;
    else {
        TFLOGSTRING("SATAPP: SatAppCommandHandler::getYesNo no user response")
        // User did not close the text, closed by the timer or MT call
        aRes = ESatNoResponseFromUser;
        aInkey = 0;
    }
    TFLOGSTRING("SATAPP: SatAppCommandHandler::getYesNo exit")
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::getInput
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::getInput(TSatUiResponse &aRes,
                                const QString &aTitleText,
                                const TSatCharacterSet aCharacterSet,
                                QString &aInputText,
                                const int aMinLength,
                                const int aMaxLength,
                                const bool aHideInput,
                                //const QPixmap* /*aIconBitmapGetInput*/,
                                const bool /*aSelfExplanatory*/,
                                unsigned int &/*aDuration*/)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::getInput")

    aRes = ESatNoResponseFromUser;
    QString heading;

    if (aTitleText.isEmpty()) {
        if (ESatDigitOnly == aCharacterSet) {
            TFLOGSTRING("SATAPP::getInput ESatDigitOnly")
            heading = tr("Enter:\n(1 digit 0-9)");
        } else {
            // ESatCharSmsDefaultAlphabet or ESatCharUcs2Alphabet
            TFLOGSTRING("SatAppCommandHandler::getInput ESatCharSmsDefaultAlphabet \
                or ESatCharUcs2Alphabet")
               heading = tr("Enter:");
        }
    } else {
        heading.append(aTitleText);
    }

    int minLength = 0;
    int maxLength = 1;
    if (aMinLength <= KGetInputStringMaxSize && aMinLength > 0){
         minLength = aMinLength;
    } else if (aMinLength > KGetInputStringMaxSize){
        minLength = KGetInputStringMaxSize;
    }
    //lint -e{961} else block not needed, maxLength initialized.
    if (aMaxLength < minLength){
        maxLength = minLength;
    } else if (aMaxLength <= KGetInputStringMaxSize && aMaxLength > 1){
         maxLength = aMaxLength;
    } else if (aMaxLength > KGetInputStringMaxSize){
         maxLength = KGetInputStringMaxSize;
    }

    QString inputText;
    if ((aInputText.length() >= minLength)&& (aInputText.length() <= maxLength)){
        inputText = aInputText;
    }

    //Duration will be implemented in GetInkey proactive command
    TSatAppUserResponse rsp = EUserNoResponse;
    rsp = mUi.showGetInputQuery( heading, inputText,
            aCharacterSet, minLength, maxLength, aHideInput);

    if (EUserPrimaryResponse == rsp) {
        // User press OK key
        aRes = ESatSuccess;
        aInputText = inputText;
        TFLOGSTRING("SATAPP: SatAppCommandHandler::getInput successful response")
    } else if (EUserSecondaryResponse == rsp)
        {
        // User press back key
        aRes = ESatBackwardModeRequestedByUser;
        TFLOGSTRING("SATAPP: SatAppCommandHandler::getInput press back softkey response")
    } else {
        // User did not close the getinput query, closed by the timer or MT call
        aRes = ESatNoResponseFromUser;
        TFLOGSTRING("SATAPP: GetInputL::getInput no user response")
    }
    TFLOGSTRING("SATAPP: SatAppCommandHandler::getInput exit")
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::confirmSend
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::confirmSend(
    TSatUiResponse &aRes,
    const QString &/*aText*/,
    bool &aActionAccepted,
    int aType)
{
    TFLOGSTRING2("SATAPP: SatAppCommandHandler::confirmSend call, type=%d", aType)
    aRes = ESatNoResponseFromUser;
    QString title;
    switch (aType) {
        case ESatUiConfirmSendSms:
            {
            title.append(tr("Allow SIM card to send message?"));
            break;
            }
        case ESatUiConfirmSendSs:
        case ESatUiConfirmSendUssd: //the same string for SS and USSD
            //title.append(tr("xxxxx"));
            break;
        default:
           aRes = ESatFailure;
           TFLOGSTRING( "SATAPP: SatAppCommandHandler::confirmSend type unsupport" )  
           break;
    }
    if(ESatFailure != aRes) {
        mUi.showConfirmSendQuery(title, aActionAccepted);
        aRes = ESatSuccess;
        TFLOGSTRING2( "SATAPP: SatAppCommandHandler::confirmSend accept=%d",
                aActionAccepted)  
    }
    TFLOGSTRING2( "SATAPP: SatAppCommandHandler::confirmSend exit rsp=%d", aRes )
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::showSmsWaitNote
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::showSmsWaitNote(const QString &aText, 
    const bool /*aSelfExplanatoryIcon*/)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::showSmsWaitNote call")
    mUi.showSmsWaitNote(aText);
    TFLOGSTRING("SATAPP: SatAppCommandHandler::showSmsWaitNote exit")
 }

// ----------------------------------------------------------------------------
// SatAppCommandHandler::showDtmfWaitNote
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::showDtmfWaitNote(TSatUiResponse &aRes,
    const QString &aText/*,
    const bool aSelfExplanatoryIcon*/)
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::showDtmfWaitNote call")
    mUi.showDtmfWaitNote(aRes, aText);
    TFLOGSTRING("SATAPP: SatAppCommandHandler::showDtmfWaitNote exit")
 }

// ----------------------------------------------------------------------------
// SatAppCommandHandler::stopShowWaitNote
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::stopShowWaitNote()
{
    TFLOGSTRING("SATAPP: SatAppCommandHandler::stopShowWaitNote call")
    mUi.stopShowWaitNote();
    TFLOGSTRING("SATAPP: SatAppCommandHandler::stopShowWaitNote exit")
}

// ----------------------------------------------------------------------------
// SatAppCommandHandler::confirmSetUpCall
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppCommandHandler::confirmSetUpCall(
        const QString &aText,
        const QString &aSimAppName,
        bool &aActionAccepted//,
        //const CFbsBitmap* aIconBitmap,
        //const TBool aSelfExplanatory
       )
{
    TFLOGSTRING("SatAppCommandHandler::showSetUpCallConfirm")
    mUi.showConfirmSetUpCallQuery(aText, aSimAppName, aActionAccepted);
    TFLOGSTRING("SatAppCommandHandler::showSetUpCallConfirm exit")
}

//End of file
