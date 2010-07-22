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

#include <QStringList>
#include <msatuiadapter.h>

#include "satappeventprovider.h"
#include "satappplaytoneprovider.h"
#include "csatuiobserver.h"
#include "tflogger.h"

// ----------------------------------------------------------------------------
// SatAppEventProvider::SatAppEventProvider
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
SatAppEventProvider::SatAppEventProvider(QObject *parent) :
    QObject(parent), mObs(NULL), mPlayTone(NULL)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::SatAppEventProvider call")
    TRAPD(err, mObs = CSatUiObserver::NewL();
                mObs->ConnectRSatSessionL();
          );
    if ( KErrNone != err ) {
        CloseSatUI();
    } else {
        mObs->SetImplementer(this);
        TFLOGSTRING("SATAPP: SatAppEventProvider::SatAppEventProvider \
            SetImplementer")
        mPlayTone = new SatAppPlayToneProvider(this);
    }

    TFLOGSTRING("SATAPP: SatAppEventProvider::SatAppEventProvider exit")
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::~SatAppEventProvider
// Sets a pointer to SATAPP: SatAppEventProvider object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
SatAppEventProvider::~SatAppEventProvider()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::~SatAppEventProvider call")
    if (mObs){
        mObs->DisconnectRSatSession();
        delete mObs;
        mObs = NULL;
    }
    if (mPlayTone) {
        delete mPlayTone;
        mPlayTone = NULL;
        TFLOGSTRING("SATAPP: SatAppEventProvider::~SatAppEventProvider delete\
            mPlayTone")
    }
    TFLOGSTRING("SATAPP: SatAppEventProvider::~SatAppEventProvider exit")
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::SetUpMenuL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::SetUpMenuL(
    const TDesC &aText,
    const MDesCArray &aMenuItems,
    const CArrayFixFlat<TSatAction> */*aMenuItemNextActions*/,
    const CFbsBitmap */*aIconBitmap*/,
    const TBool aSelfExplanatoryItems,
    const TBool aHelpIsAvailable)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::SetUpMenuL call")

    TSatUiResponse response(ESatSuccess);
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::SetUpMenuL Title=%S",
            &aText)
    }

    QStringList *menuList = new QStringList();
    TFLOGSTRING("SATAPP: SatAppEventProvider::SetUpMenuL List")

    //Add Item
    if(menuList) {
        for (int i=0 ; i<aMenuItems.MdcaCount() ; i++) {
            QString item=QString::fromUtf16(aMenuItems.MdcaPoint(i).Ptr(),
                aMenuItems.MdcaPoint(i).Length());
            menuList->append(item);
        }

        TFLOGSTRING("SATAPP: SatAppEventProvider::SetUpMenuL add item")

        emit setUpMenuEvent(
            response,
            title,
            *menuList,
            aSelfExplanatoryItems,
            aHelpIsAvailable);

        delete menuList;
    }
    TFLOGSTRING("SATAPP: SatAppEventProvider::SetUpMenuL exit")

    return response;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::SetUpMenuL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::SelectItemL(
    const TDesC &aText,
    const MDesCArray &aMenuItems,
    const CArrayFixFlat<TSatAction> */*aMenuItemNextActions*/,
    const TInt aDefaultItem,
    TUint8 &aSelection,
    const CFbsBitmap */*aIconBitmap*/,
    const TBool aSelfExplanatoryItems,
    const TBool aHelpIsAvailable)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::SelectItemL call")

    TSatUiResponse response(ESatSuccess);
    aSelection = 0;
    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::SelectItemL Title=%S",
            &aText)
    }

    QStringList *menuList = new QStringList();
    //Add Item
    if(menuList) {
        for (int i=0 ; i<aMenuItems.MdcaCount() ; i++) {
            QString item=QString::fromUtf16(aMenuItems.MdcaPoint(i).Ptr(),
                        aMenuItems.MdcaPoint(i).Length());
            menuList->append(item);
        }
        TFLOGSTRING("SATAPP: SatAppEventProvider::SelectItemL add item")

        emit selectItemEvent(
            response,
            title,
            *menuList,
            aDefaultItem,
            aSelection,
            aSelfExplanatoryItems,
            aHelpIsAvailable);

        delete menuList;
    }
    TFLOGSTRING("SATAPP: SatAppEventProvider::SelectItemL exit")

    return response;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::ProfileState
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
int SatAppEventProvider::profileState()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ProfileState call")
    int res (0);
    if (mObs) {
       res = mObs->ProfileState();
    }
    TFLOGSTRING("SATAPP: SatAppEventProvider::ProfileState exit")
    return res;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::MenuSelection
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::menuSelection(int aMenuItem, bool aHelpRequested)
{
    TFLOGSTRING2("SATAPP: SatAppEventProvider::MenuSelection call\
        aMenuItem=%d", aMenuItem)
    if(mObs && mObs->Adapter()) {
        mObs->Adapter()->MenuSelection(aMenuItem, aHelpRequested);
    }

    TFLOGSTRING("SATAPP: SatAppEventProvider::MenuSelection exit")
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::DisplayTextL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::DisplayTextL(
    const TDesC &aText,
    const TDesC &aSimApplicationName,
    CFbsBitmap */*aIconBitmapDisplayText*/,
    const TBool aSelfExplanatoryIcon,
    const TBool aSustainedText,
    const TTimeIntervalSeconds aDuration,
    const TBool aWaitUserToClear)
    {
    TFLOGSTRING("SATAPP: SatAppEventProvider::DisplayTextL call")

    TSatUiResponse response(ESatSuccess);
    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::DisplayTextL \
            Title=%S", &aText)
    }

    QString appName;
    if (aSimApplicationName.Length() > 0) {
         appName=QString::fromUtf16(aSimApplicationName.Ptr(),
                aSimApplicationName.Length());
         TFLOGSTRING2("SATAPP: SatAppEventProvider::DisplayTextL \
             appName=%S", &aSimApplicationName)
    }

    int timeInterval = aDuration.Int();
    TFLOGSTRING2("SATAPP: SatAppEventProvider::DisplayTextL: \
        timeInterval=%d", timeInterval)

    bool isSelfExplanatoryIcon = (aSelfExplanatoryIcon) ? true : false;

    emit displayTextEvent(
       response,
       title,
       appName,
       //const HbIcon &aIcon,
       isSelfExplanatoryIcon,
       aSustainedText,
       timeInterval,
       aWaitUserToClear);

    TFLOGSTRING("SATAPP: SatAppEventProvider::DisplayTextL exit")
    return response;
    }

// ----------------------------------------------------------------------------
// SatAppEventProvider::GetInputLL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::GetInputL(
    const TDesC &aText,
    const TSatCharacterSet aCharacterSet,
    TDes &aInput,
    const TInt aMinLength,
    const TInt aMaxLength,
    const TBool aHideInput,
    const TBool aGetInkey,
    const CFbsBitmap */*aIconBitmapGetInput*/,
    const TBool aSelfExplanatory,
    TUint &aDuration)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::GetInputL call")

    TSatUiResponse response(ESatSuccess);

    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::GetInputL \
            Title=%S", &aText)
     }

    QString inputText;
    if (aInput.Length() > 0) {
        inputText = QString::fromUtf16(aInput.Ptr(), aInput.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::GetInputL \
            inputText=%S", &aInput)
    }
    bool isSelfExplanatory = aSelfExplanatory;
    unsigned int duration = aDuration;
    
    if (aGetInkey) {
        //Get Inkey
         emit getInkeyEvent(
             response,
             title,
             aCharacterSet,
             inputText,
             isSelfExplanatory,
             duration);
         TFLOGSTRING("SATAPP: SatAppEventProvider::GetInputL: GetInkey")
         aInput.Copy(reinterpret_cast<const TUint16*>(inputText.utf16()), inputText.length());
     } else {
        // Get Input
        bool hideInput = aHideInput;
        int minLength = aMinLength;
        int maxLength = aMaxLength;
        emit getInputEvent(
            response,
            title,
            aCharacterSet,
            inputText,
            minLength,
            maxLength,
            hideInput,
            isSelfExplanatory,
            duration);
        int length = inputText.length();
        if (length > maxLength) {
            length = maxLength;
        }
        aInput.Copy(reinterpret_cast<const TUint16*>(inputText.utf16()), length);
        TFLOGSTRING2("SATAPP: SatAppEventProvider::GetInputL aInput=%S", &aInput)
    }
    TFLOGSTRING("SATAPP: SatAppEventProvider::GetInputL exit")
    return response;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::CallControlL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//                                         
TSatUiResponse SatAppEventProvider::CallControlL(
    const TDesC &aText,
    const TSatAlphaIdStatus aAlphaIdStatus)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::CallControlL call")
    QString text;
    if (aText.Length() > 0) {
        text=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::CallControlL \
                aText=%S", &aText)
    }
    emit callControlEvent(text, aAlphaIdStatus);
    TFLOGSTRING("SATAPP: SatAppEventProvider::CallControlL exit")
    return ESatSuccess;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::MoSmControlL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::MoSmControlL(
    const TDesC &aText,
    const TSatAlphaIdStatus aAlphaIdStatus)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::MoSmControlL call")
    QString text;
    if (aText.Length() > 0) {
        text=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::MoSmControlL \
                aText=%S", &aText)
    }
    emit moSmControlEvent(text, aAlphaIdStatus);
    TFLOGSTRING("SATAPP: SatAppEventProvider::MoSmControlL exit")
    return ESatSuccess;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::GetYesNoL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::GetYesNoL(
    const TDesC &aText,
    const TSatCharacterSet aCharacterSet,
    TChar &aInkey,
    const CFbsBitmap */*aIconBitmap*/,
    const TBool aSelfExplanatory,
    TUint &aDuration,
    const TBool aImmediateDigitResponse)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::GetYesNoL call")
    TSatUiResponse response(ESatSuccess);
    QString text;
    if (aText.Length() > 0) {
        text=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::GetYesNoL \
                aText=%S", &aText)
    }

    unsigned int inKey = static_cast < TUint >(aInkey);
    TFLOGSTRING2("SATAPP: SatAppEventProvider::GetYesNoL: inkey=%d",
     inKey)

    bool selfExplanatory = aSelfExplanatory;
    bool immediateDigitResponse = aImmediateDigitResponse;
    unsigned int duration = aDuration;

    emit getYesNoEvent(
        response,
        text,
        aCharacterSet,
        inKey,
        selfExplanatory,
        duration,
        immediateDigitResponse);

    aInkey = inKey;
    TFLOGSTRING2("SATAPP: SatAppEventProvider::GetYesNoL key=%x", inKey)
    TFLOGSTRING("SATAPP: SatAppEventProvider::GetYesNoL exit")
    return response;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::ConfirmSendL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::ConfirmSendL(
    const TDesC &aText,
    TBool &aActionAccepted,
    TInt aType)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ConfirmSendL call")
    TSatUiResponse response(ESatSuccess);
    QString text;
    if (aText.Length() > 0) {
        text=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::GetYesNoL \
                aText=%S", &aText)
    }
    bool actionAccepted = aActionAccepted;
    int type = aType;
    emit confirmSendEvent(
        response,
        text,
        actionAccepted,
        type);
    aActionAccepted = actionAccepted;
    TFLOGSTRING("SATAPP: SatAppEventProvider::ConfirmSendL exit")
    return response;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::ShowSmsWaitNoteL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::ShowSmsWaitNoteL(
    const TDesC &aText,
    const CFbsBitmap */*aIconBitmapSendSM*/,
    const TBool aSelfExplanatoryIcon)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowSmsWaitNoteL call")
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::ShowSmsWaitNoteL \
            Title=%S", &aText)
    }
    bool selfExplanatoryIcon = aSelfExplanatoryIcon;

    emit showSmsWaitNoteEvent(
        title,
        selfExplanatoryIcon);
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowSmsWaitNoteL exit")
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::ConfirmSetUpCallL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::ConfirmSetUpCallL(
    const TDesC &aText,
    const TDesC &aSimAppName,
    TBool &aActionAccepted,
    const CFbsBitmap */*aIconBitmap*/,
    const TBool /*aSelfExplanatory*/)
    {
    TFLOGSTRING("SATAPP: SatAppEventProvider::ConfirmSetUpCallL call")
    QString text;
    if (aText.Length() > 0) {
        text = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::ConfirmSetUpCallL \
            text=%S", &aText)
    }
    
    QString title;
    if (aSimAppName.Length() > 0) {
        title = QString::fromUtf16 (aSimAppName.Ptr(), aSimAppName.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::ConfirmSetUpCallL \
            Title=%S", &aSimAppName)
    }
    bool actionAccepted = aActionAccepted;

    emit showSetUpCallConfirmEvent(
        text,
        title,
        actionAccepted);
    aActionAccepted = actionAccepted;
    TFLOGSTRING2("SATAPP: SatAppEventProvider::ConfirmSetUpCallL aActionAccepted %d \
    exit", aActionAccepted)
    }

// ----------------------------------------------------------------------------
// SatAppEventProvider::ShowDtmfWaitNoteL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::ShowDtmfWaitNoteL(
    const TDesC &aText,
    const CFbsBitmap */*aIconBitmapSendSM*/,
    const TBool /*aSelfExplanatoryIcon*/)
{
    TSatUiResponse response(ESatSuccess);
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowDtmfWaitNoteL call")
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::ShowDtmfWaitNoteL \
            Title=%S", &aText)
    }
    emit showDtmfWaitNoteEvent(
        response,
        title);
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowDtmfWaitNoteL exit")
    return response;
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::clearScreen
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::ClearScreen()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ClearScreen call")
    if (mPlayTone) {
        mPlayTone->clearScreen();
    }
    emit clearScreenEvent();
    TFLOGSTRING("SATAPP: SatAppEventProvider::ClearScreen exit")
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::CloseSatUI
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::CloseSatUI()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::CloseSatUI call")
    if (mPlayTone) {
        mPlayTone->closeSatUI();
    }

    emit closeUiEvent();
    TFLOGSTRING("SATAPP: SatAppEventProvider::CloseSatUI exit")
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::StopShowWaitNote
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//    
void SatAppEventProvider::StopShowWaitNote()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::StopShowWaitNote call")
    emit stopShowWaitNoteEvent();
    TFLOGSTRING("SATAPP: SatAppEventProvider::StopShowWaitNote exit")
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::userCancelResponse
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::userCancelResponse()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::userCancelResponse call")
    mObs->Adapter()->SessionTerminated( ESessionCancel );
    TFLOGSTRING("SATAPP: SatAppEventProvider::userCancelResponse exit")
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::ShowSsWaitNoteL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::ShowSsWaitNoteL(
            const TDesC &aText,
            const CFbsBitmap* /*aIconBitmap*/,
            const TBool aSelfExplanatoryIcon )
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowSsWaitNoteL call")
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::ShowSsWaitNoteL \
            Title=%S", &aText)
    }
    bool selfExplanatoryIcon = aSelfExplanatoryIcon;

    emit showSsWaitNoteEvent(
        title,
        selfExplanatoryIcon);
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowSsWaitNoteL exit")
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::ShowWaitNoteWithoutDelayL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::ShowWaitNoteWithoutDelayL()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowWaitNoteWithoutDelayL call")
    emit showWaitNoteWithoutDelayEvent();
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowWaitNoteWithoutDelayL exit")
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::showSsErrorNoteEvent
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::ShowSsErrorNoteL()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::showSsErrorNoteEvent call")
    emit showSsErrorNoteEvent();
    TFLOGSTRING("SATAPP: SatAppEventProvider::showSsErrorNoteEvent exit")
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::ShowBIPNoteL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::ShowBIPNoteL( TInt aCommand, const TDesC &aText,
        const CFbsBitmap* /*aIconBitmap*/, const TBool /*aSelfExplanatory*/)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowBIPNoteL call")
    int cmdType = aCommand;
    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::ShowBIPNoteL \
            Title=%S", &aText)
        }
    emit showBIPNoteEvent(cmdType, title);
    TFLOGSTRING("SATAPP: SatAppEventProvider::ShowBIPNoteL exit")
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::ConfirmOpenChannelL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::ConfirmOpenChannelL(
    const TDesC &aText,
    TBool &aActionAccepted,
    const CFbsBitmap* /*aIconBitmapOpenChannel*/,
    const TBool /*aSelfExplanatory*/ )
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ConfirmOpenChannelL call")
    TSatUiResponse response( ESatSuccess );

    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::ConfirmOpenChannelL \
            Title=%S", &aText)
        }
    bool actionAccepted = aActionAccepted;
    emit showOpenChannelConfirmEvent(title, actionAccepted);
    // Show progress dialog when Openchannel confirmed
    if( actionAccepted ) {
        emit showWaitNoteWithoutDelayEvent();
    }
    aActionAccepted = actionAccepted;
    TFLOGSTRING2( "SATAPP: SatAppEventProvider::ConfirmOpenChannelL exit\
        response: %d", response)
    return response;
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::PlayStandardToneL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::PlayStandardToneL( const TDesC& aText,
    const TDesC8& aSequence,
    TTimeIntervalMicroSeconds aDuration,
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory )
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::PlayStandardToneL call")
    TSatUiResponse response = mPlayTone->PlayStandardToneL(
        aText, aSequence, aDuration, aIconBitmap, aSelfExplanatory);
    TFLOGSTRING2("SATAPP: SatAppEventProvider::PlayStandardToneL \
        response= %d exit", response)
    return response;
}

//-----------------------------------------------------------------------------
// SatAppEventProvider::PlayUserSelectedToneL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppEventProvider::PlayUserSelectedToneL(
        const TDesC &aText,
        TTimeIntervalMicroSeconds aDuration,
        TSatTone aTone,
        const CFbsBitmap* aIconBitmap,
        const TBool aSelfExplanatory )
{
    TFLOGSTRING("SatAppEventProvider::PlayUserSelectedToneL calling")
    TSatUiResponse response = mPlayTone->PlayUserSelectedToneL(
        aText, aDuration, aTone, aIconBitmap, aSelfExplanatory);
    TFLOGSTRING2( "SatAppEventProvider::PlayUserSelectedToneL response= %d \
        exit", response)
    return response;
}

//End of file
