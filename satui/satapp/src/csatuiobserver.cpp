/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class handles the communication between server and
*                user interface
*
*/


// INCLUDE FILES
#include    <e32test.h>
#include    <msatuiadapter.h>
#include    <ProfileEngineSDKCRKeys.h>
#include    <centralrepository.h>
#include    <settingsinternalcrkeys.h>
#include    <gulicon.h> //for itemsicon
#include    <fbs.h>

#include    "csatuiobserver.h"
#include    "msatuiactionimplementer.h"
#include    "csatuiiconhandler.h"
#include    "tflogger.h"

// CONSTANTS
const TInt KSatActiveProfileOffline = 5; // value from ProfileEngine

// PlayTone durations
const TInt64 KSatDur170ms  = 170000;  // 170ms
const TInt64 KSatDur200ms  = 200000;  // 200ms
const TInt64 KSatDur250ms  = 250000;  // 250ms
const TInt64 KSatDur1200ms = 1200000; // 1200ms

// play tone tones
_LIT8(KNetToneSeqNetBusy, "\x00\x11\x05\x4E\x30\x40\x30\x07\x0B");

_LIT8(KNetToneSeqNetCallWaiting, "\x00\x11\x05\x4E\x14\x40\x3C\x4E\x14\
\x40\x64\x40\x64\x40\x64\x4E\x14\x40\x3C\x4E\x14\x40\x64\x40\x64\x40\x64\
\x07\x0B");

_LIT8(KNetToneSeqNetCongestion, "\x00\x11\x05\x4E\x14\x40\x14\x07\x0B");

_LIT8(KNetToneSeqNetSpecialInformation, "\x00\x11\x05\x4F\x21\x50\
\x21\x51\x21\x40\x64\x07\x0B");

_LIT8(KNetToneSeqDial,"\x00\x11\x05\x4E\xFE\x4E\xFE\x4E\xFE\x4E\xFE\x4E\xFE\
\x4E\xFE\x4E\xFE\x4E\xFE\x07\x0B");

_LIT8(KNetToneSeqRingGoing, "\x00\x11\x05\x4E\x64\x05\x04\x40\x64\06\x07\x0B");

// General Beep is set for forever duration. Instead of AVKON definition 170ms.
_LIT8(KGeneralBeep, "\x00\x11\x0A\x76\x00\x0B");
_LIT8(KErrorTone, "\x00\x11\x0A\x6C\x19\x0B");
_LIT8(KRadioPathNotAvailable, "\x00\x11\x0A\x05\x03\x4E\x14\x40\x14\x06\x0B");
_LIT8(KRadioPathAck, "\x00\x11\x0A\x02\xFC\x0A\x80\x4E\x14\x0A\x7f\x0B");

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CSatUiObserver::CSatUiObserver
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CSatUiObserver::CSatUiObserver():
    iRefresh(EFalse),
    iIconSupport(ETrue),
    iClearScreenEvent(EFalse)
    {
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::ConstructL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::ConstructL()
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::ConstructL called")

    //Register to Server as subsession
    //If Server is not up, this function call may take time
    iSatSession.ConnectL();

    iSat.RegisterL(iSatSession, this);
    iAdapter = iSat.Adapter();
    if (!iAdapter)
        {
        User::Leave(KErrNotFound);
        }

    // create icon handler
    //iIconHandler = CSatUiIconHandler::NewL();

    //Item icons
    //iItemIcons = new (ELeave) CAknIconArray(1);

    TFLOGSTRING("SATAPP: CSatUiObserver::ConstructL exit")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::NewL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CSatUiObserver* CSatUiObserver::NewL()
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::NewL() called")
    CSatUiObserver* self = new (ELeave) CSatUiObserver;

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TFLOGSTRING("SATAPP: CSatUiObserver::NewL() exit")
    return self;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::~CSatUiObserver
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CSatUiObserver::~CSatUiObserver()
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::~CSatUiObserver() called")

    iSat.Close();
    iSatSession.Close();

//    if (iItemIcons)
//        {
//        iItemIcons->ResetAndDestroy();
//        delete iItemIcons;
//        }

    // delete icon handler
//    delete iIconHandler;
//    iIconHandler = NULL;

    iActionImplementer = NULL;
    iAdapter = NULL;

    TFLOGSTRING("SATAPP: CSatUiObserver::~CSatUiObserver() exit")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::SetImplementer
// Sets a pointer to CSatUiViewAppUi object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::SetImplementer(
    MSatUiActionImplementer* aImplementer)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::SetImplementer calling")
    iActionImplementer = aImplementer;
    TFLOGSTRING("SATAPP: CSatUiObserver::SetImplementer exiting")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::Adapter
// Returns a pointer to MSatUiAdapter provided by SatCli.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
MSatUiAdapter* CSatUiObserver::Adapter()
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::Adapter calling-exiting")
    return iAdapter;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::DisplayTextL
// Checks length of the text and calls method from
// the CSatUiViewAppUi class if the length is OK.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse CSatUiObserver::DisplayTextL(
    const TDesC& aText,
    const TDesC& aSimApplicationName,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed,
    const TBool aSustainedText,
    const TTimeIntervalSeconds aDuration,
    const TBool aWaitUserToClear)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::DisplayText called")
    TSatUiResponse response(ESatFailure);
    aRequestedIconDisplayed = ETrue;
    TBool selfExplanatoryIcon(EFalse);

    TInt textLength(aText.Length());

    if ((!textLength) || (textLength > RSat::KTextToDisplayMaxSize))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::DisplayText no text")
        aRequestedIconDisplayed = EFalse;
        return response;
        }

    CFbsBitmap* iconBitmapDisplayText = NULL;
    iActionImplementer->SetCommandPending(ETrue);

    //if icon is available for command
    if (((ESatSelfExplanatory == aIconId.iIconQualifier) ||
        (ESatNotSelfExplanatory == aIconId.iIconQualifier)) &&
        (iIconSupport))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::DisplayText have icon")
        iconBitmapDisplayText = FetchIcon(aIconId.iIdentifier,
                                           EIconDisplayText);

        if (ESatSelfExplanatory == aIconId.iIconQualifier)
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::DisplayText self \
                explanatory icon")
            selfExplanatoryIcon = ETrue;
            }
        }

    //icon not found not shown
    if (!iconBitmapDisplayText)
        {
        aRequestedIconDisplayed = EFalse;
        }

    iActionImplementer->DispatchTimer(iWait);

    TFLOGSTRING2("CSatUiObserver::DisplayTextL aDuration: %d", aDuration.Int())
    TRAPD(err,
        response = iActionImplementer->DisplayTextL(aText, aSimApplicationName,
            iconBitmapDisplayText, selfExplanatoryIcon,
            aSustainedText, aDuration, aWaitUserToClear);
       );

    if (err != KErrNone)
        {
        response = ESatFailure;
        aRequestedIconDisplayed = EFalse;
        }

    if (iActionImplementer->GetEndKey())
        {
        response = ESatSessionTerminatedByUser;
        iActionImplementer->SetEndKey(EFalse);
        }

    iActionImplementer->SetCommandPending(EFalse);

    TFLOGSTRING2("SATAPP: CSatUiObserver::DisplayText exit, return: %d", \
        response)
    return response;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::GetInkeyL
// Checks type of the input wanted and calls right method
// from the CSatUiViewAppUi class.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse CSatUiObserver::GetInkeyL(
    const TDesC& aText,
    const TSatCharacterSet aCharacterSet,
    TChar& aInput,
    const TBool /*aHelpIsAvailable*/,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed,
    TUint& aDuration,
    const TBool aImmediateDigitResponse)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::GetInkey called")

    TSatUiResponse response(ESatSuccess);
    aRequestedIconDisplayed = EFalse;
    TBuf<1> input;

    if (aInput.IsAssigned())
        {
        input.Fill(aInput, 1);
        }

    if ((aCharacterSet == ESatYesNo) && (aText.Length() == 0))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::GetInkey return ESatFailure")
        return ESatFailure;
        }

    CFbsBitmap* iconBitmapGetInkey = NULL;
    iActionImplementer->SetCommandPending(ETrue);

    // If icon is available for command
    if (((ESatSelfExplanatory == aIconId.iIconQualifier) ||
        (ESatNotSelfExplanatory == aIconId.iIconQualifier)) &&
        (iIconSupport))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::GetInkey have icon")
        TIconCommand iconCommand (EIconGetInkey);

        if (ESatYesNo == aCharacterSet)
            {
            iconCommand = EIconGetYesNo;
            }

        iconBitmapGetInkey = FetchIcon(aIconId.iIdentifier,
                iconCommand);
        GetScalableBitmap(iconBitmapGetInkey, iconCommand,
            aRequestedIconDisplayed);
        }

    TBool selfExplanatory(EFalse);

    // Icon is self-explanatory
    if (ESatSelfExplanatory == aIconId.iIconQualifier)
        {
        selfExplanatory = ETrue;
        }

    iActionImplementer->DispatchTimer(iWait);

    TRAPD(err,

        if (ESatYesNo == aCharacterSet || aImmediateDigitResponse)
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::GetInkey digit response")
            response = iActionImplementer->GetYesNoL(aText, aCharacterSet,
                aInput, iconBitmapGetInkey, selfExplanatory, aDuration,
                aImmediateDigitResponse);
            }
        else //ESatCharSmsDefaultAlphabet, ESatCharUcs2Alphabet, ESatDigitOnly
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::GetInkey other conditions")
            response = iActionImplementer->GetInputL(
                aText, aCharacterSet, input, 1, 1, EFalse, ETrue,
                iconBitmapGetInkey, selfExplanatory, aDuration);

            if (ESatSuccess == response)
                {
                if (input.Length())
                    {
                    aInput = input[0];
                    }
                }
            }
       );

    if (err != KErrNone)
        {
        response = ESatFailure;
        }

    if (iActionImplementer->GetEndKey())
        {
        response = ESatSessionTerminatedByUser;
        iActionImplementer->SetEndKey(EFalse);
        }
    iActionImplementer->SetCommandPending(EFalse);

    TFLOGSTRING2("SATAPP: CSatUiObserver::GetInkey exit, return: %d", \
        response)
    return response;
    }

// --------------------------------------------------------
// CSatUiObserver::GetInputL
// Checks type of the input wanted and whether it should be
// hidden and calls right method from the CSatUiViewAppUi class.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse CSatUiObserver::GetInputL(
    const TDesC& aText,
    const TSatCharacterSet aCharacterSet,
    TDes& aInput,
    const TInt aMinLength,
    const TInt aMaxLength,
    const TBool aHideInput,
    const TBool /*aHelpIsAvailable*/,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::GetInput called")
    TSatUiResponse response(ESatSuccess);
    aRequestedIconDisplayed = EFalse;

    if (((aCharacterSet == ESatCharSmsDefaultAlphabet) ||
         (aCharacterSet == ESatCharUcs2Alphabet)) &&
         (aHideInput))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::GetInput return ESatFailure")
        return ESatFailure;
        }

    CFbsBitmap* iconBitmapGetInput = NULL;
    iActionImplementer->SetCommandPending(ETrue);

    // If icon is available for command
    if (((ESatSelfExplanatory == aIconId.iIconQualifier) ||
        (ESatNotSelfExplanatory == aIconId.iIconQualifier)) &&
        (iIconSupport))
        {
        iconBitmapGetInput = FetchIcon(aIconId.iIdentifier, EIconGetInput);
        GetScalableBitmap(iconBitmapGetInput, EIconGetInput,
            aRequestedIconDisplayed);
        }

    TBool selfExplanatory(EFalse);

    // Icon is self-explanatory
    if (ESatSelfExplanatory == aIconId.iIconQualifier)
        {
        selfExplanatory = ETrue;
        }

    iActionImplementer->DispatchTimer(iWait);

    TUint duration (0);
    TRAPD(err,
        response = iActionImplementer->GetInputL(
            aText, aCharacterSet, aInput, aMinLength, aMaxLength,
            aHideInput, EFalse, iconBitmapGetInput, selfExplanatory,
            duration);
       );

    if (err != KErrNone)
        {
        TFLOGSTRING2("SATAPP: CSatUiObserver::GetInput err:%d", err)
        response = ESatFailure;
        aRequestedIconDisplayed = EFalse;
        }

    if (iActionImplementer->GetEndKey())
        {
        response = ESatSessionTerminatedByUser;
        iActionImplementer->SetEndKey(EFalse);
        }
    iActionImplementer->SetCommandPending(EFalse);

    TFLOGSTRING2("SATAPP: CSatUiObserver::GetInput response:%d", response)
    TFLOGSTRING2("SATAPP: CSatUiObserver::GetInput aRequestedIconDisplayed:%d",
        aRequestedIconDisplayed)
    TFLOGSTRING("SATAPP: CSatUiObserver::GetInput exit")
    return response;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::SetUpMenuL
// Calls the method from the CSatUiViewAppUi class to show
// a SetUpMenu view.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse CSatUiObserver::SetUpMenuL(
    const TDesC& aText,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const TBool aHelpIsAvailable,
    const TSatIconId& aIconId,
    const CArrayFixFlat<TInt>* aMenuIcons,
    const enum TSatIconQualifier aIconListQualifier,
    const enum TSatSelectionPreference /*aSelectionPreference*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::SetUpMenu called")
    TSatUiResponse response(ESatSuccess);
    TBool selfExplanatoryItems(EFalse);
    CFbsBitmap* iconTitleBitmap = NULL;
    iActionImplementer->SetCommandPending(ETrue);

    TFLOGSTRING2("SATAPP: CSatUiObserver::SetUpMenu item count: %d",
     aMenuItems.MdcaCount())

    if (!aMenuItems.MdcaCount() ||
        (KSatActiveProfileOffline == ProfileState()))
        {
        iActionImplementer->SetCommandPending(EFalse);
        iActionImplementer->ShowNotAvailableNoteL();
        iActionImplementer->CloseSatUI();
        return response;
        }

    //if contextpane icon available for command
    TFLOGSTRING2("SATAPP: CSatUiObserver::SetUpMenu icon qua: %d",
     aIconId.iIconQualifier)
    if (((ESatSelfExplanatory == aIconId.iIconQualifier) ||
        (ESatNotSelfExplanatory == aIconId.iIconQualifier)) &&
         (iIconSupport))
        {
        iconTitleBitmap = FetchIcon(aIconId.iIdentifier,
                                     EIconSetUpMenuContext);
        GetScalableBitmap(iconTitleBitmap, EIconSetUpMenuContext);
        }

//    iItemIcons->ResetAndDestroy();
    TInt iconCount(0);

    if (aMenuIcons)
        {
        iconCount = aMenuIcons->Count();
        TFLOGSTRING2("SATAPP: CSatUiObserver::SetUpMenu icon count: %d", iconCount)
        }


    //if icons are available for item list
    if ((aMenuItems.MdcaCount() == iconCount) &&
        ((aIconListQualifier == ESatSelfExplanatory) ||
        (aIconListQualifier == ESatNotSelfExplanatory)) &&
        (iIconSupport))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::SetUpMenu have icon")
        if (aIconListQualifier == ESatSelfExplanatory)
            {
            selfExplanatoryItems = ETrue;
            }

        TBool continueLoop(ETrue);
        TInt count(0);
        for (count = 0; (count < iconCount) && continueLoop; count++)
            {
            CGulIcon* gulIcon = CGulIcon::NewLC();
            TUint8 itemIconId(STATIC_CAST(TUint8,
                aMenuIcons->operator[](count)));
            CFbsBitmap* iconItemBitmap = NULL;
            iconItemBitmap = FetchIcon(itemIconId, EIconSetUpMenuItems);

            if (!iconItemBitmap &&
                (aIconListQualifier == ESatSelfExplanatory))
                {
                TFLOGSTRING("SATAPP: CSatUiObserver::SetUpMenu \
                    SelfExplanatory ")
                CleanupStack::PopAndDestroy(gulIcon); // gulIcon
//                iItemIcons->ResetAndDestroy();
                continueLoop = EFalse;
                }
            else
                {
                if (!iconItemBitmap &&
                    (aIconListQualifier == ESatNotSelfExplanatory))
                    {
                    TFLOGSTRING("SATAPP: CSatUiObserver::SetUpMenu \
                        not SelfExplanatory ")
                    iconItemBitmap = new(ELeave)CFbsBitmap();
                    }
                else
                    {
                    TFLOGSTRING("SATAPP: CSatUiObserver::SetUpMenu \
                        other conditions ")
                    GetScalableBitmap(iconItemBitmap, EIconSetUpMenuItems);
                    }

                gulIcon->SetBitmap(iconItemBitmap);
//                iItemIcons->AppendL(gulIcon);
                CleanupStack::Pop(gulIcon);
                }
            }
        TFLOGSTRING2("SATAPP: CSatUiObserver::SetUpMenu count: %d", count)
        }

    TRAPD(err,
        response = iActionImplementer->SetUpMenuL(
            aText, aMenuItems, aMenuItemNextActions, iconTitleBitmap,
            /*iItemIcons,*/ selfExplanatoryItems, aHelpIsAvailable);
       );

    TFLOGSTRING2("SATAPP: CSatUiObserver::SetUpMenu err: %d", err)

    if (KErrNone != err)
        {
        response = ESatFailure;
        }

    if (iActionImplementer->GetEndKey())
        {
        response = ESatSessionTerminatedByUser;
        iActionImplementer->SetEndKey(EFalse);
        }

    iActionImplementer->SetCommandPending(EFalse);
    TFLOGSTRING2("SATAPP: CSatUiObserver::SetUpMenu exit, \
        return:%i", response)
    return response;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::SelectItemL
// Calls the method from the CSatUiViewAppUi class to show
// a SelectItem view.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse CSatUiObserver::SelectItemL(
    const TDesC& aText,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const TInt aDefaultItem,
    TUint8& aSelection,
    const TBool aHelpIsAvailable,
    const TSatIconId& aIconId,
    const CArrayFixFlat<TInt>* aMenuIcons,
    const enum TSatIconQualifier aIconListQualifier,
    TBool& aRequestedIconDisplayed,
    const enum TSatSelectionPreference /*aSelectionPreference*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::SelectItem called")
    TSatUiResponse response(ESatSuccess);
    aRequestedIconDisplayed = ETrue;
    TBool selfExplanatoryItems(EFalse);

    CFbsBitmap* iconTitleBitmap = NULL;
    iActionImplementer->SetCommandPending(ETrue);

    //if contextpane icon available for command
    if (((ESatSelfExplanatory == aIconId.iIconQualifier) ||
        (ESatNotSelfExplanatory == aIconId.iIconQualifier)) &&
        (iIconSupport))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::SelectItem icon available")
        iconTitleBitmap = FetchIcon(aIconId.iIdentifier,
                                     EIconSetUpMenuContext);
        GetScalableBitmap(iconTitleBitmap, EIconSetUpMenuContext,
            aRequestedIconDisplayed);
        }

    //iItemIcons->ResetAndDestroy();

    TInt iconCount(0);

    if (aMenuIcons)
        {
        iconCount = aMenuIcons->Count();
        }

    TFLOGSTRING2("SATAPP: CSatUiObserver::SelectItem iconCount:%d", iconCount)

    //if icons are available for item list
    if ((aMenuItems.MdcaCount() == iconCount) &&
        ((aIconListQualifier == ESatSelfExplanatory) ||
        (aIconListQualifier == ESatNotSelfExplanatory)) &&
        (iIconSupport))
        {
        TFLOGSTRING(
            "SATAPP: CSatUiObserver::SelectItem icon available for item list")

        if (aIconListQualifier == ESatSelfExplanatory)
            {
            selfExplanatoryItems = ETrue;
            }

        TBool continueLoop(ETrue);
        TInt count(0);
        for (count = 0; (count < iconCount) && continueLoop; count++)
            {
            CGulIcon* gulIcon = CGulIcon::NewLC();
            TUint8 itemIconId = STATIC_CAST(TUint8,
                aMenuIcons->operator[](count));
            CFbsBitmap* iconItemBitmap = NULL;
            iconItemBitmap = FetchIcon(itemIconId, EIconSetUpMenuItems);

            // when icon can't be received and is set to self explanatory, we
            // iqnore all icons and show only text
            if (!iconItemBitmap &&
                (aIconListQualifier == ESatSelfExplanatory))
                {
                TFLOGSTRING("SATAPP: CSatUiObserver::SelectItem \
                    SelfExplanatory ")
                CleanupStack::PopAndDestroy(gulIcon); // gulIcon
                //iItemIcons->ResetAndDestroy();
                continueLoop = EFalse;
                aRequestedIconDisplayed = EFalse;
                }
            else
                {
                // when icon can't be received and is set to not self
                // explanatory, we replace it with empty icon
                if (!iconItemBitmap &&
                    (aIconListQualifier == ESatNotSelfExplanatory))
                    {
                    TFLOGSTRING("SATAPP: CSatUiObserver::SelectItem \
                        not SelfExplanatory ")
                    iconItemBitmap = new(ELeave)CFbsBitmap();
                    aRequestedIconDisplayed = EFalse;
                    }
                // when icon was able to receive, we scale it
                // if every prior icon is correctly received
                else if (aRequestedIconDisplayed)
                    {
                    TFLOGSTRING("SATAPP: CSatUiObserver::SelectItem \
                        receive icon ")
                    GetScalableBitmap(iconItemBitmap, EIconSetUpMenuItems,
                        aRequestedIconDisplayed);
                    }
                // if aRequestIconDisplayed is false, at least one prior icon
                // fetch had problems and thus we must report that by not
                // updating aRequestedIconDisplayed
                else
                    {
                    TFLOGSTRING("SATAPP: CSatUiObserver::SelectItem \
                        aRequestIconDisplayed false ")
                    GetScalableBitmap(iconItemBitmap, EIconSetUpMenuItems);
                    }

                gulIcon->SetBitmap(iconItemBitmap);
                //iItemIcons->AppendL(gulIcon);
                CleanupStack::Pop(gulIcon);
                }
            }
        TFLOGSTRING2("SATAPP: CSatUiObserver::SelectItem count: %d", count)
        }

    TRAPD(err,
        response = iActionImplementer->SelectItemL(aText, aMenuItems,
            aMenuItemNextActions, aDefaultItem, aSelection, iconTitleBitmap,
            /*iItemIcons,*/ selfExplanatoryItems, aHelpIsAvailable);
       );

    if (err != KErrNone)
        {
        response = ESatFailure;
        aRequestedIconDisplayed = EFalse;
        }

    if (iActionImplementer->GetEndKey())
        {
        response = ESatSessionTerminatedByUser;
        iActionImplementer->SetEndKey(EFalse);
        }

    iActionImplementer->SetCommandPending(EFalse);

    TFLOGSTRING2("SATAPP: CSatUiObserver::SelectItem aSelection:%d", aSelection)
    TFLOGSTRING2("SATAPP: CSatUiObserver::SelectItem response:%d", response)
    TFLOGSTRING("SATAPP: CSatUiObserver::SelectItem exit")
    return response;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::PlayTone
// Checks type of the tone wanted and calls right method
// from the CSatUiViewAppUi class.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse CSatUiObserver::PlayTone(
    const TDesC& aText,
    const TSatTone aTone,
    const TTimeIntervalMicroSeconds aDuration,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone called")
    TFLOGSTRING2("SATAPP: CSatUiObserver::PlayTone tone is %i", aTone)

    aRequestedIconDisplayed = EFalse;

    iSequence.Zero();
    TTimeIntervalMicroSeconds duration(aDuration.Int64());
    iActionImplementer->SetCommandPending(ETrue);
    // This is used to determine zero length duration.
    const TTimeIntervalMicroSeconds zeroDuration(static_cast<TInt64>(0));

    CFbsBitmap* iconBitmap = NULL;
    // If icon is available for command
    if (((ESatSelfExplanatory == aIconId.iIconQualifier) ||
         (ESatNotSelfExplanatory == aIconId.iIconQualifier)) &&
        (iIconSupport))
        {
        iconBitmap = FetchIcon(aIconId.iIdentifier, EIconPlayTone);
        GetScalableBitmap(iconBitmap, EIconPlayTone,
            aRequestedIconDisplayed);
        }

    TBool selfExplanatory(EFalse);

    // Icon is self-explanatory
    if (ESatSelfExplanatory == aIconId.iIconQualifier)
        {
        selfExplanatory = ETrue;
        }

    // Check if the current Profile is Meeting
    TInt errCR(KErrNone);
    TInt profileId(0);

    CRepository* repository = NULL;
    TSatTone tone = aTone;

    // Which UID to monitor.
    TRAP(errCR, repository = CRepository::NewL(KCRUidProfileEngine));

    if (!errCR && repository)
        {
        errCR = repository->StartTransaction(
            CRepository::EReadWriteTransaction);
        errCR = repository->Get(KProEngActiveProfile, profileId);
        }

    delete repository;

    TFLOGSTRING2("SATAPP: CSatUiObserver::PlayTone(): err = %d", errCR)

    TFLOGSTRING2("SCSatUiObserver::PlayTone() profileId: %d",
        profileId)

    switch (tone)
        {
        case ESatGeneralBeep:
        case ESatPositiveTone:
        case ESatToneNotSet:
            if (zeroDuration == duration)
                {
                duration = KSatDur170ms; // 170ms
                }
            iSequence.Copy(KGeneralBeep);
            break;
        case ESatNegativeTone:
            if (zeroDuration == duration)
                {
                duration = KSatDur250ms; // 250ms
                }
            iSequence.Copy(KErrorTone);
            break;
        case ESatRadioPathNotAvailableCallDropped:
            if (zeroDuration == duration)
                {
                duration = KSatDur1200ms; // 1200ms
                }
            iSequence.Copy(KRadioPathNotAvailable);
            break;
        case ESatRadioPathAcknowledge:
            if (zeroDuration == duration)
                {
                duration = KSatDur200ms; // 200ms
                }
            iSequence.Copy(KRadioPathAck);
            break;
        case ESatDialTone:
            {
            if (zeroDuration == duration)
                {
                TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone() ESatDialTone")
                return ESatCmdDataNotUnderstood;
                }
            else
                {
                iSequence.Copy(KNetToneSeqDial);
                }
            }
            break;
        case ESatCalledSubscriberBusy:
            {
            if (zeroDuration == duration)
                {
                return ESatCmdDataNotUnderstood;
                }
            else
                {
                iSequence.Copy(KNetToneSeqNetBusy);
                }
            }
            break;
        case ESatCongestion:
            {
            if (zeroDuration == duration)
                {
                return ESatCmdDataNotUnderstood;
                }
            else
                {
                iSequence.Copy(KNetToneSeqNetCongestion);
                }
            }
            break;
        case ESatErrorSpecialInfo:
            {
            if (zeroDuration == duration)
                {
                return ESatCmdDataNotUnderstood;
                }
            else
                {
                iSequence.Copy(KNetToneSeqNetSpecialInformation);
                }
            }
            break;
        case ESatCallWaitingTone:
            {
            if (zeroDuration == duration)
                {
                return ESatCmdDataNotUnderstood;
                }
            else
                {
                iSequence.Copy(KNetToneSeqNetCallWaiting);
                }
            }
            break;
        case ESatRingingTone:
            {
            if (zeroDuration == duration)
                {
                return ESatCmdDataNotUnderstood;
                }
            else
                {
                iSequence.Copy(KNetToneSeqRingGoing);
                }
            }
            break;
        case ESatUserSelectedToneIncomingSpeech:
        case ESatUserSelectedToneIncomingSms:
            {
            // These are handled later.
            }
            break;
        default:
            TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone Default")
            return ESatCmdDataNotUnderstood;
        }

    TSatUiResponse response(ESatFailure);

    iActionImplementer->DispatchTimer(iWait);

    // ESatSClearScreenEvent doesn't occur,clear displaytext on screen.
    if (EFalse == iClearScreenEvent)
        {
        // Clear displaytext on the screen.
        TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone clear screen")
        iActionImplementer->ClearScreen();
        }

    if (0 != iSequence.Length())
        {
        TFLOGSTRING2("SATAPP: CSatUiObserver::PlayTone duration microseconds %d",
            aDuration.Int64())
        TRAPD(err,
            response = iActionImplementer->PlayStandardToneL(
                aText,
                iSequence,
                duration,
                iconBitmap,
                selfExplanatory);
           );

        if (KErrNone != err)
            {
            response = ESatFailure;
            TFLOGSTRING2("SATAPP: CSatUiObserver::PlayTone error %i", err)
            }
        TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone StandardTone exit")
        }
    else
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone length of sequence \
            is zero")

            TRAPD(err,
            response = iActionImplementer->PlayUserSelectedToneL(
                aText,
                aDuration.Int64(),
                tone,
                iconBitmap,
                selfExplanatory);
           );

            if (KErrNone != err)
                {
                response = ESatFailure;
                TFLOGSTRING2("SATAPP: CSatUiObserver::PlayTone error %i", err)
                }

        TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone UserSelectedTone exit")
        }

    if (iActionImplementer->GetEndKey())
        {
        response = ESatSessionTerminatedByUser;
        iActionImplementer->SetEndKey(EFalse);
        }

    iActionImplementer->SetCommandPending(EFalse);

    TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone iClearScreenEvent false")
    iClearScreenEvent = EFalse;

    TFLOGSTRING2("SATAPP: CSatUiObserver::PlayTone response: %d", response)
    TFLOGSTRING("SATAPP: CSatUiObserver::PlayTone exit")
    return response;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::ConfirmCommand
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::ConfirmCommand(
    const TSatSQueryCommand aCommandId,
    const TSatAlphaIdStatus aAlphaIdStatus,
    const TDesC& aText,
    const TDesC& aAdditionalText,
    TBool& aActionAccepted,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed,
    TBool& aTerminatedByUser)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::ConfirmCommand calling")
    aRequestedIconDisplayed = EFalse;
    TInt error(KErrNone);
    TBool selfExplanatory(EFalse);

    // Icon is self-explanatory
    if (ESatSelfExplanatory == aIconId.iIconQualifier)
        {
        selfExplanatory = ETrue;
        }

    aActionAccepted = ETrue;

    iActionImplementer->SetCommandPending(ETrue);

    iActionImplementer->StopShowWaitNote();

    CFbsBitmap* iconBitmap = NULL;
    // If icon is available for command
    if ((((ESatSelfExplanatory == aIconId.iIconQualifier) ||
        (ESatNotSelfExplanatory == aIconId.iIconQualifier)) &&
        ((ESatOpenChannelQuery == aCommandId) ||
        (ESatSLaunchBrowserQuery == aCommandId) ||
        (ESatSSetUpCallQuery == aCommandId))) &&
        (iIconSupport))
        {
        iconBitmap = FetchIcon(aIconId.iIdentifier,
            EIconConfirmCommand);
        GetScalableBitmap(iconBitmap, EIconConfirmCommand,
            aRequestedIconDisplayed);
        }
    TFLOGSTRING2("SATAPP: CSatUiObserver::ConfirmCommand aCommandId: %d",
                  aCommandId)
    // Check quering command
    switch (aCommandId)
        {
        case ESatOpenChannelQuery:
            {
            TFLOGSTRING(" Quering OpenChannel")
            //TRAP(error, iActionImplementer->ConfirmOpenChannelL(
            //    aText, aActionAccepted, iconBitmap, selfExplanatory));
            break;
            }

        case ESatSRefreshQuery:
            {
            TFLOGSTRING(" Quering Refresh")
            //TRAP(error, iActionImplementer->ConfirmRefreshL(aActionAccepted));
            break;
            }

        case ESatSLaunchBrowserQuery:
            {
            TFLOGSTRING(" Quering LaunchBrowser")
            HBufC* textHolder = NULL;

            TRAP(error,
            if (ESatAlphaIdNull != aAlphaIdStatus)
                {
                TFLOGSTRING(" CSatUiObserver::ConfirmCommand AlphaId \
                    isn't null")
                if (ESatAlphaIdNotProvided == aAlphaIdStatus)
                    {
                    TFLOGSTRING(" CSatUiObserver::ConfirmCommand \
                        AlphaId not provided")
                    // ToDo: replace the resouce with qt.
                    //textHolder = StringLoader::LoadLC(
                    //    R_QTN_SAT_CONF_LAUNCH_BROW,
                    //    iActionImplementer->CoeEnv());
                    }
                else
                    {
                    TFLOGSTRING(" CSatUiObserver::ConfirmCommand \
                        AlphaId provided")
                    textHolder = HBufC::NewLC(aText.Length());
                    TPtr ptr = textHolder->Des();
                    ptr.Append(aText);
                    }
                
                //TRAP(error, iActionImplementer->ConfirmLaunchBrowserL(*textHolder,
                //    aActionAccepted, iconBitmap, selfExplanatory));
 
                // Clean textHolder
                CleanupStack::PopAndDestroy(textHolder);
                }

           ); // TRAP
            break;
            }

        case ESatSSendSmQuery:
            {
            TFLOGSTRING( " Quering SendSm" )
            TRAP( error, iActionImplementer->ConfirmSendL( aText,
                  aActionAccepted, ESatUiConfirmSendSms ) );
            break;
            }

        case ESatSSendSsQuery:
            {
            TFLOGSTRING(" Quering SendSs")
            //TRAP(error, iActionImplementer->ConfirmSendL(aText,
            //      aActionAccepted, ESatUiConfirmSendSs));
            break;
            }

        case ESatSSendUssdQuery:
            {
            TFLOGSTRING(" Quering SendUssd")
            //TRAP(error, iActionImplementer->ConfirmSendL(aText,
            //      aActionAccepted, ESatUiConfirmSendUssd));
            break;
            }

        case ESatSSetUpCallQuery:
            {
            TFLOGSTRING(" Quering SetUpCall")
            TRAP(error, iActionImplementer->ConfirmSetUpCallL(
                aText, aAdditionalText, aActionAccepted, iconBitmap,
                selfExplanatory));
            break;
            }

        default:
            {
            TFLOGSTRING(" Unknown quering command")
            aActionAccepted = EFalse;
            }
        }

    if (KErrNone != error)
        {
        TFLOGSTRING2("SATAPP: CSatUiObserver::ConfirmCommand error:%d", error)
        aActionAccepted = EFalse;
        }
    else if (iActionImplementer->GetEndKey())
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::ConfirmCommand press endkey")
        aTerminatedByUser = ETrue;
        aActionAccepted = EFalse;
        iActionImplementer->SetEndKey(EFalse);
        }
    else
        {
        // Meaningless else
        }

    iActionImplementer->SetCommandPending(EFalse);
    TFLOGSTRING("SATAPP: CSatUiObserver::ConfirmCommand exiting")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::Notification
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse CSatUiObserver::Notification(
    const TSatSNotifyCommand aCommandId,
    const TSatAlphaIdStatus aAlphaIdStatus,
    const TDesC& aText,
    const TSatIconId& aIconId,
    TBool& aRequestedIconDisplayed,
    const TSatControlResult aControlResult)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::Notification calling")
    TSatUiResponse response = ESatSuccess;
    TInt error(KErrNone);
    aRequestedIconDisplayed = EFalse;
    iActionImplementer->SetCommandPending(ETrue);

    HBufC* textHolder = NULL;

    // In case where command id is SendDtmf and alphaID is not provided,
    // DTMF string to be sent is shown in dialog along with default text.
    if ((ESatSSendDtmfNotify == aCommandId) &&
         (ESatAlphaIdNotProvided == aAlphaIdStatus))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification SendDtmf")
        TRAP(error,
            TInt resource = DefaultAlphaIdL(aCommandId, aControlResult);

            // Todo:
            // Now that resource is defined, get default text
            //textHolder = StringLoader::LoadL(
            //    resource, aText)
           );
        }
    // Get default text, if alpha not provided or it is NULL
    else if (aText.Length() == 0 && ESatAlphaIdNotNull != aAlphaIdStatus)
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification alpha is null")
        TRAP(error,
            TInt resource = DefaultAlphaIdL(aCommandId, aControlResult);
            // Todo:
            // Now that resource is defined, get default text
            //textHolder = StringLoader::LoadL(
            //    resource, iActionImplementer->CoeEnv())
           );
        }
    // else use given text
    else
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification alpha is provided")
        TRAP(error,
            textHolder = HBufC::NewL(aText.Length());
            TPtr ptr = textHolder->Des();
            ptr.Copy(aText);
           );
        }

    if (!textHolder)
        {
        TRAP(error, textHolder = HBufC::NewL(0));
        }

    // No need to add textHolder into CleanupStack, since this is
    // not a leaving function

    if ((KErrNone == error || KErrArgument == error) && textHolder)
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification none error ")
        CFbsBitmap* iconBitmap = NULL;
        TBool selfExplanatoryIcon(EFalse);
        // Fetch icon
        // Icons not supported for sending and receiving data
        if ((((ESatSelfExplanatory == aIconId.iIconQualifier) ||
            (ESatNotSelfExplanatory == aIconId.iIconQualifier)) &&
            ((ESatSSendDataNotify != aCommandId) &&
            (ESatSReceiveDataNotify != aCommandId))) &&
            (iIconSupport))
            {
            iconBitmap = FetchIcon(aIconId.iIdentifier,
                EIconNotification);
            //GetScalableBitmap(iconBitmap, EIconNotification,
            //    aRequestedIconDisplayed);
            }

        if (ESatSelfExplanatory == aIconId.iIconQualifier)
            {
            selfExplanatoryIcon = ETrue;
            }

        // Check notifying command
        switch (aCommandId)
            {
            case ESatSSendDataNotify:
                {
                TFLOGSTRING(" Notifying SendData")
                TRAP(error,
                    iActionImplementer->ShowBIPNoteL(
                        ESendDataIdentier, *textHolder, iconBitmap,
                        selfExplanatoryIcon)
                   );
                break;
                }
            case ESatSReceiveDataNotify:
                {
                TFLOGSTRING(" Notifying ReceiveData")
                TRAP(error,
                    iActionImplementer->ShowBIPNoteL(
                        EReceiveDataIdentifier, *textHolder, iconBitmap,
                        selfExplanatoryIcon)
                   );
                break;
                }
            case ESatSCloseChannelNotify:
                {
                TFLOGSTRING(" Notifying CloseChannel")
                iActionImplementer->StopShowWaitNote();
                TRAP(error,
                    iActionImplementer->ShowBIPNoteL(
                        ECloseChannelIdentifier, *textHolder, iconBitmap,
                        selfExplanatoryIcon)
                   );
                break;
                }
            case ESatSMoSmControlNotify:
                {
                TFLOGSTRING(" Notifying MoSmControl")
                TRAP(error,
                    iActionImplementer->MoSmControlL(
                        *textHolder, aAlphaIdStatus)
                   );
                iActionImplementer->SetCommandPending(EFalse);
                break;
                }
            case ESatSCallControlNotify:
                {
                TFLOGSTRING(" Notifying CallControl")
                iActionImplementer->DispatchTimer(iWait);
                TRAP(error,
                    iActionImplementer->CallControlL(
                        *textHolder, aAlphaIdStatus)
                   );
                iActionImplementer->SetCommandPending(EFalse);
                break;
                }
            case ESatSSendUssdNotify:   // fall through
            case ESatSSendSsNotify:
                {
                TFLOGSTRING(" Notifying SendSs / SendUssd")
                iActionImplementer->StopShowWaitNote();
                TRAP(error,
                    if ((ESatAlphaIdNotNull == aAlphaIdStatus) ||
                        (ESatAlphaIdNotProvided == aAlphaIdStatus))
                        {
                        iActionImplementer->ShowSsWaitNoteL(*textHolder,
                            iconBitmap, selfExplanatoryIcon);
                        }
                    else
                        {
                        iActionImplementer->ShowWaitNoteWithoutDelayL();
                        }
                   );
                break;
                }
            case ESatSSendDtmfNotify:
                {
                TFLOGSTRING(" Notifying SendDtmf")
                iActionImplementer->StopShowWaitNote();
                TRAP(error, response = iActionImplementer->ShowDtmfWaitNoteL(
                    *textHolder, iconBitmap, selfExplanatoryIcon));
                break;
                }
            case ESatSSendSmsNotify:
                {
                TFLOGSTRING(" Notifying SendSms")
                iActionImplementer->StopShowWaitNote();

                if (KErrNone == error)
                    {
                    // Show Sms wait note
                    TRAP(error,
                        iActionImplementer->ShowSmsWaitNoteL(*textHolder,
                            iconBitmap, selfExplanatoryIcon));
                    }

                break;
                }
            default:
                {
                TFLOGSTRING(" Unkown notification")
                response = ESatFailure;
                break;
                }
            }
        }

    delete textHolder;
    textHolder = NULL;

    if (KErrNone != error)
        {
        TFLOGSTRING2("SATAPP: CSatUiObserver::Notification error:%d", error)
        response = ESatFailure;
        }
    else if (iActionImplementer->GetEndKey())
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification terminated by user")
        response = ESatSessionTerminatedByUser;
        iActionImplementer->SetEndKey(EFalse);
        }
    else
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification success")
        response = ESatSuccess;
        }


    TFLOGSTRING2("SATAPP: CSatUiObserver::Notification exiting, return: %d", \
        response)
    return response;
    }

// ------------------------------------------------------------------------------------------------
// CSatUiObserver::EventNotification
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------------
//
void CSatUiObserver::EventNotification(
    const TSatSEvent aEventId,
    const TSatSEventStatus /*aEventStatus*/,
    const TInt /*aError*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::EventNotification calling")

    iActionImplementer->SetCommandPending(EFalse);
    switch (aEventId)
        {
        case ESatSSmEndEvent:
            {
            TFLOGSTRING(" SmsEnd event")
            iActionImplementer->StopShowWaitNote();
            break;
            }
        case ESatSClearScreenEvent:
            {
            TFLOGSTRING(" ClearScreen event")
            iClearScreenEvent = ETrue;
            iActionImplementer->ClearScreen();
            break;
            }
        case ESatSsEndEvent:
            {
            TFLOGSTRING(" Ss end event")
            iActionImplementer->StopShowWaitNote();
            break;
            }
        case ESatSsErrorEvent:
            {
            TFLOGSTRING(" Notifying Ss error")
            // If error occurred (and Alpha ID provided), notify user
            TRAPD(err, iActionImplementer->ShowSsErrorNoteL())

            if (err)
                {
                TFLOGSTRING(" Error Note could not be created!")
                }
            break;
            }
        case ESatSDtmfEndEvent:
            {
            TFLOGSTRING(" DTMF End event")
            iActionImplementer->StopShowWaitNote();
            break;
            }
        case ESatSCloseSatUiAppEvent:
            {
            TFLOGSTRING(" Close UI event")
            // Handle this similar way when End key is used
            if (KSatActiveProfileOffline == ProfileState())
                {
                iActionImplementer->SetEndKey(ETrue);
                TRAP_IGNORE(iActionImplementer->ShowNotAvailableNoteL())
                }
            iActionImplementer->CloseSatUI();
            break;
            }
        default:
            {
            TFLOGSTRING2(" Unknown event occured: %i", aEventId)
            break;
            }
        }

    TFLOGSTRING("SATAPP: CSatUiObserver::EventNotification exiting")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::DefaultAlphaIdL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CSatUiObserver::DefaultAlphaIdL(
    const TSatSNotifyCommand aCommandId,
    const TSatControlResult aControlResult) const
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::DefaultAlphaIdL calling")
    TInt resource(0);

    switch (aCommandId)
        {
        case ESatSSendDataNotify: // SendData
            {
            TFLOGSTRING(" SendData default")
            //resource = R_QTN_SAT_CONF_SEND_DATA_BIP;
            break;
            }

        case ESatSReceiveDataNotify: // ReceiveData
            {
            TFLOGSTRING(" ReceiveData default")
            //resource = R_QTN_SAT_CONF_RECEIVE_DATA_BIP;
            break;
            }

        case ESatSCloseChannelNotify: // CloseChannel
            {
            TFLOGSTRING(" CloseChannel default")
            //resource = R_QTN_SAT_CONF_CLOSE_CHANNEL_BIP;
            break;
            }

        case ESatSMoSmControlNotify: // MoSmControl
            {
            if (ESatNotAllowed == aControlResult)
                {
                TFLOGSTRING(" MoSmcontrol Not allowed default")
                //resource = R_QTN_SAT_MOSM_NOT_ALLOWED;
                }
            else if (ESatAllowedWithModifications == aControlResult)
                {
                TFLOGSTRING(" MoSmcontrol Modified default")
                //resource = R_QTN_SAT_MOSM_MODIFIED;
                }
            else
                {
                TFLOGSTRING(" MoSmcontrol No default")
                resource = 0; // Allowed, default alpha -> no info
                }
            break;
            }

        case ESatSCallControlNotify: // CallControl
            {
            if (ESatNotAllowed == aControlResult)
                {
                TFLOGSTRING(" CallControl Not allowed default")
                //resource = R_QTN_SAT_CC_NOT_ALLOWED;
                }
            else if (ESatAllowedWithModifications == aControlResult)
                {
                TFLOGSTRING(" CallControl Modified default")
                //resource = R_QTN_SAT_CC_MODIFIED;
                }
            else
                {
                TFLOGSTRING(" CallControl No default")
                resource = 0; // Allowed, default alpha -> no info
                }
            break;
            }

        case ESatSSendSmsNotify: // SendSm
            {
            TFLOGSTRING(" SendSms default")
            //resource = R_QTN_SAT_SENDING_SMS;
            break;
            }

        case ESatSSendDtmfNotify: // SendDtmf
            {
            TFLOGSTRING(" SendDTMF default")
            //resource = R_QTN_SAT_NOTE_SENDING_DTMF_TEMPLATE;
            break;
            }

        default:
            {
            TFLOGSTRING2(" Unknown command id: %i", aCommandId)
            User::Leave(KErrArgument);
            }
        }

    TFLOGSTRING2("SATAPP: CSatUiObserver::DefaultAlphaIdL exiting, resource: \
        %d", resource)
    return resource;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::FetchIcon
// Fetch the icon information.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CFbsBitmap* CSatUiObserver::FetchIcon(const TUint8 /*aIconId*/,
    const enum TIconCommand /*aIconCommand*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::FetchIcon called")

    CFbsBitmap* iconBitmap = NULL;
/*
    TSize layoutSize(0, 0);

    // Set icon max size according the command
    if (GetPopUpWindowIconSize(layoutSize, aIconCommand))
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::FetchIcon max size")
        // Comment out icon related code, add qt icon support future
        TRAPD(err, iconBitmap = iIconHandler->FetchIconL(iSatSession,
            aIconId, layoutSize.iWidth, layoutSize.iHeight, iIconSupport));

        if (err != KErrNone)
            {
            #if defined _DEBUG
            _LIT(KFetchIcon, "SATAPP: CSatUiObserver::FetchIcon");
            User::Panic(KFetchIcon, err);
            #endif
            }
        }
    else
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::FetchIcon not supported")
        }
*/
    TFLOGSTRING("SATAPP: CSatUiObserver::FetchIcon exit")
    return iconBitmap;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::GetPopUpWindowIconSize
// Get size for the icon in popup window.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TBool CSatUiObserver::GetPopUpWindowIconSize(TSize& aSize,
    const TIconCommand /*aIconCommand*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::GetPopUpWindowIconSize called")

    TRect rect(TSize(0, 0));
    TBool supported(ETrue);
    // Comment out this code for no icon support in current qt satapp
/*    TAknLayoutRect opLogoLayout;

    TFLOGSTRING2("SATAPP: CSatUiObserver::GetPopUpWindowIconSize, \
        aIconCommand: %i", aIconCommand)
    // depending on the command, get correct layout and
    // Calculate rectangle based on LAF specification.
    switch (aIconCommand)
        {
        case EIconSetUpMenuContext:
            {
            opLogoLayout.LayoutRect(rect,
                AknLayoutScalable_Avkon::context_pane_g1().LayoutLine());
            break;
            }
        case EIconSetUpMenuItems:
            {
            opLogoLayout.LayoutRect(rect,
                AknLayout::List_pane_elements__single_graphic__Line_1());
            break;
            }
        case EIconDisplayText:
            {
            // layout borrowed from video
            opLogoLayout.LayoutRect(rect, AknLayoutScalable_Avkon::
                popup_query_sat_info_window(0).LayoutLine());
            break;
            }
        case EIconPlayTone:
            {
            opLogoLayout.LayoutRect(rect, AknLayoutScalable_Avkon::
                popup_note_window_g1(0).LayoutLine());
            break;
            }
        case EIconConfirmCommand:   // fall through
        case EIconGetInput:         // fall through
        case EIconGetYesNo:         // fall through
        case EIconGetInkey:
            {
            opLogoLayout.LayoutRect(rect, AknLayout::Icon(0));
            break;
            }
        case EIconNotification:
            {
            opLogoLayout.LayoutRect(rect, AknLayout::
                Wait_or_progress_note_pop_up_window_elements_Line_1());
            break;
            }
        default:
            {
            TFLOGSTRING(
                "SATAPP: CSatUiObserver::GetPopUpWindowIconSize not supported")
            supported = EFalse;
            break;
            }
        }

    aSize = opLogoLayout.Rect().Size();

    // Layout for title icon gives incorrect width
    // but since it is square height can be set to width
    if (EIconSetUpMenuContext == aIconCommand)
        {
        aSize.iWidth = aSize.iHeight;
        }
*/
    TFLOGSTRING3("SATAPP: CSatUiObserver::GetPopUpWindowIconSize %dx%d exit",
        aSize.iWidth, aSize.iHeight)
    return supported;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::GetScalableBitmap
// Creates scalable bitmap
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::GetScalableBitmapL(
    CFbsBitmap*& /*aBitMapToConvert*/,
    const TIconCommand  /*aIconCommand*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap called")
/*
    TSize layoutSize(0, 0);
    CFbsBitmap* bitmap = NULL;

    // If scalable icon is supported in current layout then
    // makes the scalable icon.
    if (GetPopUpWindowIconSize(layoutSize, aIconCommand))
        {
        bitmap = new (ELeave) CFbsBitmap;
        CleanupStack::PushL(bitmap);
        // CAknIcon takes ownership of bitmaps.
        CFbsBitmap* dupMain = new (ELeave) CFbsBitmap;
        CleanupStack::PushL(dupMain);
        TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap duplicate original")
        User::LeaveIfError(
            dupMain->Duplicate(aBitMapToConvert->Handle()));

        //CAknIcon* bitmapIcon = CAknIcon::NewL();
        // Mask is not set because skins are not used.
        // Ownership is transferred.
        bitmapIcon->SetBitmap(dupMain);
        CleanupStack::Pop(dupMain);
        CleanupStack::PushL(bitmapIcon);
        CAknIcon* scalableIcon = AknIconUtils::CreateIconL(bitmapIcon);
        CleanupStack::Pop(bitmapIcon);
        CleanupStack::PushL(scalableIcon);

        // fetch the size of icon
        TSize iconSize = dupMain->SizeInPixels();

        // At first we assume that width is scaled to layout maximum and
        // thus height is set so that the ratio of the image remains correct

        TInt newHeight =
            (layoutSize.iWidth * iconSize.iHeight) / iconSize.iWidth;

        // If the new height is larger than the height of the layout
        // we scale height to maximum and set the width so that the ratio of
        // the image remains correct
        if (newHeight > layoutSize.iHeight)
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap \
                larger than layout height")
            layoutSize.iWidth =
                (layoutSize.iHeight * iconSize.iWidth) / iconSize.iHeight;
            }
        // If the new height is smaller or the same as the height of the
        // layout, the image is scaled according to it
        else
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap \
                smaller than layout height")
            layoutSize.iHeight = newHeight;
            }

        TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap resize scaled icon")
        AknIconUtils::SetSize(scalableIcon->Bitmap(), layoutSize);

        TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap duplicate scaled")
        User::LeaveIfError(bitmap->Duplicate(
            scalableIcon->Bitmap()->Handle()));

        CleanupStack::PopAndDestroy(scalableIcon);

        // Uses scaled icon if scalable bitmap is supported.
        TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap Show Scaled")
        delete aBitMapToConvert;
        aBitMapToConvert = bitmap;
        CleanupStack::Pop(bitmap);
        }
*/
    TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap exit")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::GetScalableBitmap
// Handles scalable bitmap
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::GetScalableBitmap(
    CFbsBitmap*& /*aBitMapToConvert*/,
    const TIconCommand  /*aIconCommand*/,
    TBool& /*aRequestedIconDisplayed*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap called")
/*
    if (aBitMapToConvert)
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap bitmap \
            to convert")
        // Scale icon
        TRAPD(err, GetScalableBitmapL(aBitMapToConvert, aIconCommand););

        if (KErrNoMemory == err)
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap memory \
                low")
            // Memory low, command is done without icon
            delete aBitMapToConvert;
            aBitMapToConvert = NULL;
            aRequestedIconDisplayed = EFalse;
            }
        else
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap have \
                memory")
            aRequestedIconDisplayed = ETrue;
            }
        }
    else
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap  no bitmap")
        // Icon not received
        aRequestedIconDisplayed = EFalse;
        }
*/
    TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap exit")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::GetScalableBitmap
// Handles scalable bitmap
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::GetScalableBitmap(
    CFbsBitmap*& /*aBitMapToConvert*/,
    const TIconCommand /*aIconCommand*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap called")
/*
    if (aBitMapToConvert)
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap bitmap \
            to convert")
        // Scale icon
        TRAPD(err, GetScalableBitmapL(aBitMapToConvert, aIconCommand););

        if (KErrNoMemory == err)
            {
             TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap memory \
                 low")
            // Memory low, command is done without icon
            delete aBitMapToConvert;
            aBitMapToConvert = NULL;
            }
        }
*/
    TFLOGSTRING("SATAPP: CSatUiObserver::GetScalableBitmap exit")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::ProfileState
// Get the profile status
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CSatUiObserver::ProfileState()
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::ProfileState called")
    TInt profileId(0);
    CRepository* cr (NULL);

    TRAPD(err, cr = CRepository::NewL(KCRUidProfileEngine));
    if ((KErrNone == err) && cr)
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::ProfileState no err")
        // Get the ID of the currently active profile:
        cr->Get(KProEngActiveProfile, profileId);
        delete cr;
        }
    else
        {
        TFLOGSTRING2("SATAPP: CSatUiObserver::ProfileState CR failed: %i", err)
        }

    TFLOGSTRING2("SATAPP: CSatUiObserver::ProfileState exit value: %d",
        profileId)

    return profileId;
    }

//  End of File
