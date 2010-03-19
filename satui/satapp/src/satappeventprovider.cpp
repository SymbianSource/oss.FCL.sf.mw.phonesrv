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
#include "csatuiobserver.h"
#include "tflogger.h"

// ----------------------------------------------------------------------------
// SatAppEventProvider::SatAppEventProvider
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
SatAppEventProvider::SatAppEventProvider(QObject *parent) :
    QObject(parent), mObs(NULL)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::SatAppEventProvider call")
    TRAPD(err, mObs = CSatUiObserver::NewL())
    TFLOGSTRING2("SATAPP: SatAppEventProvider::SatAppEventProvider \
        new CSatUiObserver err=%d", err)
    
    if (KErrNone != err) {
        CloseSatUI();
    } else {
        mObs->SetImplementer(this);
        TFLOGSTRING("SATAPP: SatAppEventProvider::SatAppEventProvider")
    }  
    TFLOGSTRING("SATAPP: SatAppEventProvider::SatAppEventProvider exit")
}

// ----------------------------------------------------------------------------
// SatAppEventProvider::~SatAppEventProvider
// Sets a pointer to CSatUiViewAppUi object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
SatAppEventProvider::~SatAppEventProvider()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::~SatAppEventProvider call")
    delete mObs;
    mObs = NULL;
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
    //const CAknIconArray */*aItemIconsArray*/,
    const TBool aSelfExplanatoryItems,
    const TBool aHelpIsAvailable)
    {
    TFLOGSTRING("SATAPP: SatAppEventProvider::SetUpMenuL call")

    TSatUiResponse response(ESatSuccess);
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::SetUpMenuL Title=%s",
            title.utf16())
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
            //const CArrayFixFlat<TSatAction> *aMenuItemNextActions,
            //const HbIcon &aIcon,
            //const CArrayFixFlat<TInt> *aMenuIcons,
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
    //const CAknIconArray */*aItemsIconArray*/,
    const TBool aSelfExplanatoryItems,
    const TBool aHelpIsAvailable)
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::SelectItemL call")

    TSatUiResponse response(ESatSuccess);
    aSelection = 0;
    QString title;
    if (aText.Length() > 0) {
        title=QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::SelectItemL Title=%s",
            title.utf16())
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
            //const CArrayFixFlat<TSatAction> *aMenuItemNextActions,
            //const HbIcon &aIcon,
            //const CArrayFixFlat<TInt> *aMenuIcons,
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
            Title=%s", title.utf16())
        }

    QString appName;
    if (aSimApplicationName.Length() > 0) {
         appName=QString::fromUtf16(aSimApplicationName.Ptr(),
                aSimApplicationName.Length());
         TFLOGSTRING2("SATAPP: SatAppEventProvider::DisplayTextL \
             appName=%s", appName.utf16())
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
            Title=%s", title.utf16())
        }

    QString inputText;
    if (aInput.Length() > 0) {
        inputText = QString::fromUtf16(aInput.Ptr(), aInput.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::GetInputL \
            inputText=%s", inputText.utf16())
        }
    bool isSelfExplanatory = aSelfExplanatory;
    unsigned int duration = aDuration;
    //QPixmap *iconBitmapGetInput = QPixmap::fromSymbianCFbsBitmap(CFbsBitmap *aIconBitmapGetInput);
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
            //iconBitmapGetInput,
            isSelfExplanatory,
            duration);
        int length = inputText.length();
        if (length > maxLength) {
            length = maxLength;
        }
        TFLOGSTRING("SATAPP: SatAppEventProvider::GetInputL: GetInput")
        TFLOGSTRING2( "SATAPP: SatAppEventProvider::GetInputL:GetInput \
        text=%s", inputText.utf16() )
        aInput.Copy(reinterpret_cast<const TUint16*>(inputText.utf16()), length);
        TFLOGSTRING2("SATAPP: SatAppEventProvider::GetInputL aInput=%S", &aInput)
    }
    TFLOGSTRING("SATAPP: SatAppEventProvider::GetInputL exit")
    return response;
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
                aText=%s", text.utf16())
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
        //const TSatIconId &aIconId,
        selfExplanatory,
        duration,
        immediateDigitResponse);

    aInkey = inKey;
    TFLOGSTRING2("SATAPP: SatAppEventProvider::GetYesNoL key=%d", inKey)
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
                aText=%s", text.utf16())
    }
    bool actionAccepted = aActionAccepted;
    int type = aType;
    emit confirmSendEvent(
        response,
        text,
        actionAccepted,
        type);
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
            Title=%s", title.utf16())
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
            text=%s", text.utf16())
    }
    
    QString title;
    if (aText.Length() > 0) {
        title = QString::fromUtf16 (aSimAppName.Ptr(), aSimAppName.Length());
        TFLOGSTRING2("SATAPP: SatAppEventProvider::ConfirmSetUpCallL \
            Title=%s", title.utf16())
    }
    bool actionAccepted = aActionAccepted;

    emit showSetUpCallConfirmEvent(
        text,
        title,
        actionAccepted);
    TFLOGSTRING("SATAPP: SatAppEventProvider::ConfirmSetUpCallL exit")    
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
            Title=%s", title.utf16())
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
// SatAppEventProvider::userCancelDtmfResponse
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppEventProvider::userCancelDtmfResponse()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::userCancelDtmfResponse call")
    mObs->Adapter()->SessionTerminated( ESessionCancel );
    TFLOGSTRING("SATAPP: SatAppEventProvider::userCancelDtmfResponse exit")
}

 //End of file
