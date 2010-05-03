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
#include "dummyeventprovider.h"
#include "tflogger.h"
#include "csatuiobserver.h"
#include "dummyplaytoneprovider.h"

// ----------------------------------------------------------------------------
// DummyEventProvider::DummyEventProvider
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
DummyEventProvider::DummyEventProvider(QObject *parent) :
    QObject(parent), mObs(NULL)
{
    qDebug("SATAPP: DummyEventProvider::DummyEventProvider call");
    TRAPD(err, mObs = CSatUiObserver::NewL());
    
    if (KErrNone != err) {
        CloseSatUI();
    } else {
        mObs->SetImplementer(this);
        TFLOGSTRING("SATAPP: DummyEventProvider::DummyEventProvider \
            SetImplementer")
        mPlayTone = new DummyPlayToneProvider(this);
    }  
    qDebug("SATAPP: DummyEventProvider::DummyEventProvider exit");
}

// ----------------------------------------------------------------------------
// DummyEventProvider::~DummyEventProvider
// Sets a pointer to CSatUiViewAppUi object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
DummyEventProvider::~DummyEventProvider()
{
    qDebug("SATAPP: DummyEventProvider::~DummyEventProvider call");
    delete mObs;
    mObs = NULL;
    if (mPlayTone) {
        delete mPlayTone;
        mPlayTone = 0;
        TFLOGSTRING("SATAPP: DummyEventProvider::~DummyEventProvider delete\
            mPlayTone")
    }
    qDebug("SATAPP: DummyEventProvider::~DummyEventProvider exit");
}

// ----------------------------------------------------------------------------
// DummyEventProvider::SetUpMenuL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::SetUpMenuL(
    const TDesC &aText,
    const MDesCArray &aMenuItems,
    const CArrayFixFlat<TSatAction> */*aMenuItemNextActions*/,
    const CFbsBitmap */*aIconBitmap*/,
    //const CAknIconArray */*aItemIconsArray*/,
    const TBool aSelfExplanatoryItems,
    const TBool aHelpIsAvailable)
    {
    qDebug("SATAPP: DummyEventProvider::SetUpMenuL call");

    TSatUiResponse response(ESatSuccess);
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
        }

    QStringList *menuList = new QStringList();
    qDebug("SATAPP: DummyEventProvider::SetUpMenuL List");

    //Add Item
    if(menuList) {
        for (int i=0 ; i<aMenuItems.MdcaCount() ; i++) {
            QString item=QString::fromUtf16(aMenuItems.MdcaPoint(i).Ptr(),
                aMenuItems.MdcaPoint(i).Length());
            menuList->append(item);
            }

        qDebug("SATAPP: DummyEventProvider::SetUpMenuL add item");

        emit setUpMenuEvent(
            response,
            title,
            *menuList,
            //const CArrayFixFlat<TSatAction> *aMenuItemNextActions,
            //const HbIcon &aIcon,
            //const CArrayFixFlat<TInt> *aMenuIcons,
            aSelfExplanatoryItems,
            aHelpIsAvailable);

        delete menuList;
    }
    qDebug("SATAPP: DummyEventProvider::SetUpMenuL exit");

    return response;
    }

// ----------------------------------------------------------------------------
// DummyEventProvider::SetUpMenuL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::SelectItemL(
    const TDesC &aText,
    const MDesCArray &aMenuItems,
    const CArrayFixFlat<TSatAction> */*aMenuItemNextActions*/,
    const TInt aDefaultItem,
    TUint8 &aSelection,
    const CFbsBitmap */*aIconBitmap*/,
    //const CAknIconArray */*aItemsIconArray*/,
    const TBool aSelfExplanatoryItems,
    const TBool aHelpIsAvailable)
{
    qDebug("SATAPP: DummyEventProvider::SelectItemL call");

    TSatUiResponse response(ESatSuccess);
    aSelection = 0;
    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        }

    QStringList *menuList = new QStringList();
    //Add Item
    if(menuList) {
        for (int i=0 ; i<aMenuItems.MdcaCount() ; i++) {
            QString item=QString::fromUtf16(aMenuItems.MdcaPoint(i).Ptr(),
                        aMenuItems.MdcaPoint(i).Length());
            menuList->append(item);
            }
        qDebug("SATAPP: DummyEventProvider::SelectItemL add item");

        emit selectItemEvent(
            response,
            title,
            *menuList,
            aDefaultItem,
            aSelection,
            //const CArrayFixFlat<TSatAction> *aMenuItemNextActions,
            //const HbIcon &aIcon,
            //const CArrayFixFlat<TInt> *aMenuIcons,
            aSelfExplanatoryItems,
            aHelpIsAvailable);

        delete menuList;
    }
    qDebug("SATAPP: DummyEventProvider::SelectItemL exit");

    return response;
}

// ----------------------------------------------------------------------------
// DummyEventProvider::ProfileState
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
int DummyEventProvider::profileState()
{
    qDebug("SATAPP: DummyEventProvider::ProfileState call");
    int res (0);
    if (mObs) {
       res = mObs->ProfileState();
    }
    qDebug("SATAPP: DummyEventProvider::ProfileState exit");
    return res;
}

// ----------------------------------------------------------------------------
// DummyEventProvider::MenuSelection
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::menuSelection(int aMenuItem, bool aHelpRequested)
    {
    if(mObs && mObs->Adapter()) {
        mObs->Adapter()->MenuSelection(aMenuItem, aHelpRequested);
        }

    qDebug("SATAPP: DummyEventProvider::MenuSelection exit");
    }

// ----------------------------------------------------------------------------
// DummyEventProvider::DisplayTextL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::DisplayTextL(
    const TDesC &aText,
    const TDesC &aSimApplicationName,
    CFbsBitmap */*aIconBitmapDisplayText*/,
    const TBool aSelfExplanatoryIcon,
    const TBool aSustainedText,
    const TTimeIntervalSeconds aDuration,
    const TBool aWaitUserToClear)
    {
    qDebug("SATAPP: DummyEventProvider::DisplayTextL call");

    TSatUiResponse response(ESatFailure);
    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
    } else {
         qDebug("SATAPP: DummyEventProvider::DisplayTextL null text");
         response = ESatFailure;
    }

    QString appName;
    if (aSimApplicationName.Length() > 0) {
         appName=QString::fromUtf16(aSimApplicationName.Ptr(),
                aSimApplicationName.Length());
    } else {
        // test leave
        qDebug("SATAPP: DummyEventProvider::DisplayTextL leave exit");
        User::Leave(KErrNotFound);
    }

    int timeInterval = aDuration.Int();

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

    qDebug("SATAPP: DummyEventProvider::DisplayTextL exit");
    return response;
    }

// ----------------------------------------------------------------------------
// DummyEventProvider::GetInputLL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::GetInputL(
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
    qDebug("SATAPP: DummyEventProvider::GetInputL call");

    TSatUiResponse response(ESatSuccess);

    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: DummyEventProvider::GetInputL \
            Title=%S", &aText)
        }

    QString inputText;
    if (aInput.Length() > 0) {
        inputText = QString::fromUtf16(aInput.Ptr(), aInput.Length());
    } else {
        // test leave
        TFLOGSTRING("SATAPP: DummyEventProvider::GetInkey test leave")
        User::Leave(KErrNotFound);
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
             //iconBitmapGetInput,
             isSelfExplanatory,
             duration);
         qDebug("SATAPP: DummyEventProvider::GetInputL: GetInkey");
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
            //iconBitmapGetInput,
            isSelfExplanatory,
            duration);
        int length = inputText.length();
        if (length > maxLength) {
            length = maxLength;
        }
        qDebug("SATAPP: DummyEventProvider::GetInputL: GetInput");
        aInput.Copy(reinterpret_cast<const TUint16*>(inputText.utf16()), length);
    }
    qDebug("SATAPP: DummyEventProvider::GetInputL exit");
    return response;
    }

    // ----------------------------------------------------------------------------
// DummyEventProvider::GetYesNoL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::GetYesNoL(
    const TDesC &aText,
    const TSatCharacterSet aCharacterSet,
    TChar &aInkey,
    const CFbsBitmap */*aIconBitmap*/,
    const TBool aSelfExplanatory,
    TUint &aDuration,
    const TBool aImmediateDigitResponse)
{
    qDebug("SATAPP: DummyEventProvider::GetYesNoL call");
    TSatUiResponse response(ESatSuccess);
    QString text;
    if (aText.Length() > 0) {
        text=QString::fromUtf16(aText.Ptr(), aText.Length());
    }

    unsigned int inKey = static_cast < TUint >(aInkey);

    bool selfExplanatory = aSelfExplanatory;
    bool immediateDigitResponse = aImmediateDigitResponse;
    unsigned int duration = aDuration;

    emit getYesNoEvent(
        response,
        text,
        aCharacterSet,
        inKey,
        //const TSatIconId &aIconId,
        selfExplanatory,
        duration,
        immediateDigitResponse);

    aInkey = inKey;
    qDebug("SATAPP: DummyEventProvider::GetYesNoL exit");
    return response;
}

// ----------------------------------------------------------------------------
// DummyEventProvider::ConfirmSendL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::ConfirmSendL(
    const TDesC &aText,
    TBool &aActionAccepted,
    TInt aType)
{
    qDebug("SATAPP: DummyEventProvider::ConfirmSendL call");
    TSatUiResponse response(ESatSuccess);
    QString text;
    if (aText.Length() > 0) {
        text=QString::fromUtf16(aText.Ptr(), aText.Length());
    }
    bool actionAccepted = aActionAccepted;
    int type = aType;
    emit confirmSendEvent(
        response,
        text,
        actionAccepted,
        type);
    qDebug("SATAPP: DummyEventProvider::ConfirmSendL exit");
    return response;
}

// ----------------------------------------------------------------------------
// DummyEventProvider::ShowSmsWaitNoteL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::ShowSmsWaitNoteL(
    const TDesC &aText,
    const CFbsBitmap */*aIconBitmapSendSM*/,
    const TBool aSelfExplanatoryIcon)
{
    qDebug("SATAPP: DummyEventProvider::ShowSmsWaitNoteL call");
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
    }
    bool selfExplanatoryIcon = aSelfExplanatoryIcon;

    emit showSmsWaitNoteEvent(
        title,
        selfExplanatoryIcon);
    qDebug("SATAPP: DummyEventProvider::ShowSmsWaitNoteL exit");
}

// ----------------------------------------------------------------------------
// DummyEventProvider::ConfirmSetUpCallL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::ConfirmSetUpCallL(
    const TDesC &aText,
    const TDesC &aSimAppName,
    TBool &aActionAccepted,
    const CFbsBitmap */*aIconBitmap*/,
    const TBool /*aSelfExplanatory*/)
    {
    qDebug("SATAPP: DummyEventProvider::ConfirmSetUpCallL call");
    QString text;
    if (aText.Length() > 0) {
        text = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: DummyEventProvider::ConfirmSetUpCallL \
            text=%S", text.utf16())
    }
    
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aSimAppName.Ptr(), aSimAppName.Length());
        TFLOGSTRING2("SATAPP: DummyEventProvider::ConfirmSetUpCallL \
            Title=%S", title.utf16())
    }
    bool actionAccepted = aActionAccepted;

    emit showSetUpCallConfirmEvent(
        text,
        title,
        actionAccepted);
    qDebug("SATAPP: DummyEventProvider::ConfirmSetUpCallL exit");
    }

// ----------------------------------------------------------------------------
// DummyEventProvider::ShowDtmfWaitNoteL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::ShowDtmfWaitNoteL(
    const TDesC &aText,
    const CFbsBitmap */*aIconBitmapSendSM*/,
    const TBool /*aSelfExplanatoryIcon*/)
{
    TSatUiResponse response(ESatSuccess);
    qDebug("SATAPP: DummyEventProvider::ShowDtmfWaitNoteL call");
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: DummyEventProvider::ShowDtmfWaitNoteL \
            Title=%S", title.utf16())
    }
    emit showDtmfWaitNoteEvent(
        response,
        title);
    qDebug("SATAPP: DummyEventProvider::ShowDtmfWaitNoteL exit");
    return response;
}

// ----------------------------------------------------------------------------
// DummyEventProvider::clearScreen
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::ClearScreen()
{
    qDebug("SATAPP: DummyEventProvider::ClearScreen call");
    if (mPlayTone) {
        mPlayTone->clearScreen();
    }
    emit clearScreenEvent();
    qDebug("SATAPP: DummyEventProvider::ClearScreen exit");
}

// ----------------------------------------------------------------------------
// DummyEventProvider::CloseSatUI
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::CloseSatUI()
{
    qDebug("SATAPP: DummyEventProvider::CloseSatUI call");
    if (mPlayTone) {
        mPlayTone->closeSatUI();
    }
    emit closeUiEvent();
    qDebug("SATAPP: DummyEventProvider::CloseSatUI exit");
}

//-----------------------------------------------------------------------------
// DummyEventProvider::StopShowWaitNote
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//    
void DummyEventProvider::StopShowWaitNote()
{
    qDebug("SATAPP: DummyEventProvider::StopShowWaitNote call");
    emit stopShowWaitNoteEvent();
    qDebug("SATAPP: DummyEventProvider::StopShowWaitNote exit");
}

//-----------------------------------------------------------------------------
// DummyEventProvider::userCancelDtmfResponse
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::userCancelResponse()
{
    qDebug("SATAPP: DummyEventProvider::userCancelDtmfResponse call");
    mObs->Adapter()->SessionTerminated( ESessionCancel );
    qDebug("SATAPP: DummyEventProvider::userCancelDtmfResponse exit");
}

//-----------------------------------------------------------------------------
// DummyEventProvider::ShowSsWaitNoteL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::ShowSsWaitNoteL(
            const TDesC &aText,
            const CFbsBitmap* /*aIconBitmap*/,
            const TBool aSelfExplanatoryIcon )
{
    TFLOGSTRING("SATAPP: DummyEventProvider::ShowSsWaitNoteL call")
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: DummyEventProvider::ShowSsWaitNoteL \
            Title=%s", title.utf16())
    }
    bool selfExplanatoryIcon = aSelfExplanatoryIcon;

    emit showSsWaitNoteEvent(
        title,
        selfExplanatoryIcon);
    TFLOGSTRING("SATAPP: DummyEventProvider::ShowSsWaitNoteL exit")
}

//-----------------------------------------------------------------------------
// DummyEventProvider::ShowWaitNoteWithoutDelayL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::ShowWaitNoteWithoutDelayL()
{
    TFLOGSTRING("SATAPP: DummyEventProvider::ShowWaitNoteWithoutDelayL call")
    emit showWaitNoteWithoutDelayEvent();
    TFLOGSTRING("SATAPP: DummyEventProvider::ShowWaitNoteWithoutDelayL exit")
}

//-----------------------------------------------------------------------------
// DummyEventProvider::showSsErrorNoteEvent
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::ShowSsErrorNoteL()
{
    TFLOGSTRING("SATAPP: DummyEventProvider::showSsErrorNoteEvent call")
    emit showSsErrorNoteEvent();
    TFLOGSTRING("SATAPP: DummyEventProvider::showSsErrorNoteEvent exit")
}

//-----------------------------------------------------------------------------
// DummyEventProvider::ShowBIPNoteL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyEventProvider::ShowBIPNoteL( TInt aCommand, const TDesC &aText,
        const CFbsBitmap* /*aIconBitmap*/, const TBool /*aSelfExplanatory*/)
{
    TFLOGSTRING("SATAPP: DummyEventProvider::ShowBIPNoteL call")
    int cmdType = aCommand;
    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: DummyEventProvider::ShowBIPNoteL \
            Title=%S", title.utf16())
        }
    emit showBIPNoteEvent(cmdType, title);
    TFLOGSTRING("SATAPP: DummyEventProvider::ShowBIPNoteL exit")
}

//-----------------------------------------------------------------------------
// DummyEventProvider::ConfirmOpenChannelL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::ConfirmOpenChannelL(
    const TDesC &aText,
    TBool &aActionAccepted,
    const CFbsBitmap* /*aIconBitmapOpenChannel*/,
    const TBool /*aSelfExplanatory*/ )
{
    TFLOGSTRING("SATAPP: DummyEventProvider::ConfirmOpenChannelL call")
        
    TSatUiResponse response( ESatSuccess );

    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: DummyEventProvider::ConfirmOpenChannelL \
            Title=%s", title.utf16())
        }
     
    bool actionAccepted = aActionAccepted;
    
    emit showOpenChannelConfirmEvent(title, actionAccepted);
    
    TFLOGSTRING2( "SATAPP: DummyEventProvider::ConfirmOpenChannelL exit\
        response: %d", response)
    return response;
}

//-----------------------------------------------------------------------------
// DummyEventProvider::PlayStandardToneL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::PlayStandardToneL( const TDesC& aText,
    const TDesC8& aSequence,
    TTimeIntervalMicroSeconds aDuration,
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory )
{
    TFLOGSTRING("SATAPP: DummyEventProvider::PlayStandardToneL call")
    TSatUiResponse response = mPlayTone->PlayStandardToneL(
        aText, aSequence, aDuration, aIconBitmap, aSelfExplanatory);
    TFLOGSTRING2("SATAPP: DummyEventProvider::PlayStandardToneL \
        response= %d exit", response)
    return response;
}

//-----------------------------------------------------------------------------
// DummyEventProvider::PlayUserSelectedToneL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyEventProvider::PlayUserSelectedToneL(
        const TDesC &aText,
        TTimeIntervalMicroSeconds aDuration,
        TSatTone aTone,
        const CFbsBitmap* aIconBitmap,
        const TBool aSelfExplanatory )
{
    TFLOGSTRING("DummyEventProvider::PlayUserSelectedToneL calling")
    if (100 == aTone) {
        TFLOGSTRING("DummyEventProvider::PlayUserSelectedToneL test failure")
        return ESatFailure;
    }
    TSatUiResponse response = mPlayTone->PlayUserSelectedToneL(
        aText, aDuration, aTone, aIconBitmap, aSelfExplanatory);
    TFLOGSTRING2( "DummyEventProvider::PlayUserSelectedToneL response= %d \
        exit", response)
    return response;
}
 //End of file
