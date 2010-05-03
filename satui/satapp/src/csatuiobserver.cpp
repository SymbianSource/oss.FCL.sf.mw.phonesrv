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
#include    <HbGlobal>

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

    if ( KErrNone != err )
        {
        response = ESatFailure;
        aRequestedIconDisplayed = EFalse;
        }

    if ( iActionImplementer->GetEndKey() )
        {
        response = ESatSessionTerminatedByUser;
        iActionImplementer->SetEndKey( EFalse );
        }

    iActionImplementer->SetCommandPending( EFalse );

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

    if ( ( ESatYesNo == aCharacterSet ) && (aText.Length() == 0) )
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

            if ( ESatSuccess == response && input.Length() )
                {
                aInput = input[0];
                }
            }
       );

    if ( KErrNone != err )
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

    if ( KErrNone != err )
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
                (ESatSelfExplanatory == aIconListQualifier))
                {
                TFLOGSTRING("SATAPP: CSatUiObserver::SetUpMenu \
                    SelfExplanatory ")
                CleanupStack::PopAndDestroy(gulIcon); // gulIcon
                continueLoop = EFalse;
                }
            else
                {
                if (!iconItemBitmap &&
                    (ESatNotSelfExplanatory == aIconListQualifier))
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

    TInt iconCount(0);

    if (aMenuIcons)
        {
        iconCount = aMenuIcons->Count();
        }

    TFLOGSTRING2("SATAPP: CSatUiObserver::SelectItem iconCount:%d", iconCount)

    //if icons are available for item list
    if ((aMenuItems.MdcaCount() == iconCount) &&
        ((ESatSelfExplanatory == aIconListQualifier) ||
        (ESatNotSelfExplanatory == aIconListQualifier)) &&
        (iIconSupport))
        {
        TFLOGSTRING(
            "SATAPP: CSatUiObserver::SelectItem icon available for item list")

        if (ESatSelfExplanatory == aIconListQualifier)
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
                continueLoop = EFalse;
                aRequestedIconDisplayed = EFalse;
                }
            else
                {
                // when icon can't be received and is set to not self
                // explanatory, we replace it with empty icon
                if (!iconItemBitmap &&
                    (ESatNotSelfExplanatory == aIconListQualifier))
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

    if (KErrNone != err)
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
            TFLOGSTRING("Quering OpenChannel")
            TRAP(error, iActionImplementer->ConfirmOpenChannelL(
                 aText, aActionAccepted, iconBitmap, selfExplanatory));
            break;
            }

        case ESatSRefreshQuery:
            {
            TFLOGSTRING("Quering Refresh")
            //TRAP(error, iActionImplementer->ConfirmRefreshL(aActionAccepted));
            break;
            }

        case ESatSLaunchBrowserQuery:
            {
            TFLOGSTRING("Quering LaunchBrowser")
            HBufC* textHolder = NULL;

            TRAP(error,
            if (ESatAlphaIdNull != aAlphaIdStatus)
                {
                TFLOGSTRING("CSatUiObserver::ConfirmCommand AlphaId \
                    isn't null")
                if (ESatAlphaIdNotProvided == aAlphaIdStatus)
                    {
                    TFLOGSTRING("CSatUiObserver::ConfirmCommand \
                        AlphaId not provided")
                    // ToDo: replace the resouce with qt.
                    //textHolder = StringLoader::LoadLC(
                    //    R_QTN_SAT_CONF_LAUNCH_BROW,
                    //    iActionImplementer->CoeEnv());
                    }
                else
                    {
                    TFLOGSTRING("CSatUiObserver::ConfirmCommand \
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
            TFLOGSTRING( "Quering SendSm" )
            TRAP( error, iActionImplementer->ConfirmSendL( aText,
                  aActionAccepted, ESatUiConfirmSendSms ) );
            break;
            }

        case ESatSSendSsQuery:
            {
            TFLOGSTRING(" Quering SendSs")
            TRAP(error, iActionImplementer->ConfirmSendL(aText,
                  aActionAccepted, ESatUiConfirmSendSs));
            break;
            }

        case ESatSSendUssdQuery:
            {
            TFLOGSTRING(" Quering SendUssd")
            TRAP(error, iActionImplementer->ConfirmSendL(aText,
                  aActionAccepted, ESatUiConfirmSendUssd));
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
    TFLOGSTRING2("SATAPP: CSatUiObserver::Notification str=%S", &aText)
    TSatUiResponse response = ESatSuccess;
    TInt error(KErrNone);
    aRequestedIconDisplayed = EFalse;
    iActionImplementer->SetCommandPending(ETrue);

    HBufC* textHolder = NULL;
    QString resource;
    // In case where command id is SendDtmf and alphaID is not provided,
    // DTMF string to be sent is shown in dialog along with default text.
    if ((ESatSSendDtmfNotify == aCommandId) &&
         (ESatAlphaIdNotProvided == aAlphaIdStatus)) {
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification SendDtmf")
        DefaultAlphaId(aCommandId, aControlResult,resource);
        if ( resource.length()>0) 
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::Notification SendDtmf resource")
            TRAP(error, textHolder = HBufC::NewL(resource.length()));
            if (KErrNone == error) 
                {
                textHolder->Des().Copy(reinterpret_cast<const TUint16*>
                     (resource.utf16()));
                }
            }
        }
    // Get default text, if alpha not provided or it is NULL
    else if (aText.Length() == 0 && ESatAlphaIdNotNull != aAlphaIdStatus)
        {
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification alpha is null")
        DefaultAlphaId(aCommandId, aControlResult,resource);
        if ( resource.length()>0) 
            {
            TFLOGSTRING("SATAPP: CSatUiObserver::Notification alpha null resource")
            TRAP( error, textHolder = HBufC::NewL(resource.length()) );
            if (KErrNone == error) 
                {
                textHolder->Des().Copy(reinterpret_cast<const TUint16*>
                    (resource.utf16()));
                }
             }
        TFLOGSTRING("SATAPP: CSatUiObserver::Notification alpha is null out")
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
         TFLOGSTRING("SATAPP: CSatUiObserver::Notification textHolder 0")
        TRAP(error, textHolder = HBufC::NewL(0));
        }

    // No need to add textHolder into CleanupStack, since this is
    // not a leaving function

    if ((KErrNone == error) && textHolder)
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
// CSatUiObserver::DefaultAlphaId
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::DefaultAlphaId(
    const TSatSNotifyCommand aCommandId,
    const TSatControlResult aControlResult, QString &resource) const
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::DefaultAlphaId calling")
    switch (aCommandId)
        {
        case ESatSSendDataNotify: // SendData
            {
            TFLOGSTRING("SendData default")
            resource = hbTrId("txt_sat_conf_send_data_bip");
            break;
            }
        case ESatSReceiveDataNotify: // ReceiveData
            {
            TFLOGSTRING("ReceiveData default")
            resource = hbTrId("txt_sat_conf_receive_data_bip");
            break;
            }
        case ESatSCloseChannelNotify: // CloseChannel
            {
            TFLOGSTRING("CloseChannel default")
            resource = hbTrId("txt_sat_conf_close_channel_bip");
            break;
            }
        case ESatSMoSmControlNotify: // MoSmControl
            {
            if (ESatNotAllowed == aControlResult)
                {
                TFLOGSTRING(" MoSmcontrol Not allowed default")
                resource = hbTrId("txt_sat_mosm_not_allowed");
                }
            else if (ESatAllowedWithModifications == aControlResult)
                {
                TFLOGSTRING(" MoSmcontrol Modified default")
                resource = hbTrId("txt_sat_request_modified");
                }
            else
                {
                TFLOGSTRING(" MoSmcontrol No default")
                resource = hbTrId(""); // Allowed, default alpha -> no info
                }
            break;
            }
        case ESatSCallControlNotify: // CallControl
            {
            if (ESatNotAllowed == aControlResult)
                {
                TFLOGSTRING("CallControl Not allowed default")
                resource = hbTrId("txt_sat_cc_not_allowed");
                }
            else if (ESatAllowedWithModifications == aControlResult)
                {
                TFLOGSTRING("CallControl Modified default")
                resource = hbTrId("txt_sat_cc_modified");
                }
            else
                {
                TFLOGSTRING("CallControl No default")
                resource = hbTrId(""); // Allowed, default alpha -> no info
                }
            break;
            }
        case ESatSSendSmsNotify: // SendSm
            {
            TFLOGSTRING(" SendSms default")
            resource = hbTrId("txt_sat_sending_sms");
            break;
            }

        case ESatSSendDtmfNotify: // SendDtmf
            {
            TFLOGSTRING(" SendDTMF default")
            resource = hbTrId("txt_sat_note_sending_dtmf_template");
            break;
            }
        default:
            {
            TFLOGSTRING2(" Unknown command id: %i", aCommandId)
            break;
            }
        }

    TFLOGSTRING("SATAPP: CSatUiObserver::DefaultAlphaId exiting")
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
    TFLOGSTRING("SATAPP: CSatUiObserver::FetchIcon exit")
    return iconBitmap;
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::GetPopUpWindowIconSize
// Get size for the icon in popup window.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TBool CSatUiObserver::GetPopUpWindowIconSize(TSize& /*aSize*/,
    const TIconCommand /*aIconCommand*/)
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::GetPopUpWindowIconSize called")
    return ETrue;
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

// ----------------------------------------------------------------------------
// CSatUiObserver::ConnectRSatSessionL
//
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::ConnectRSatSessionL()
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::ConnectRSatSessionL called")
    //Register to Server as subsession
    //If Server is not up, this function call may take time
    iSatSession.ConnectL();
    iSat.RegisterL(iSatSession, this);
    iAdapter = iSat.Adapter();
    if (!iAdapter)
        {
        User::Leave(KErrNotFound);
        }
    TFLOGSTRING("SATAPP: CSatUiObserver::ConnectRSatSessionL exit")
    }

// ----------------------------------------------------------------------------
// CSatUiObserver::DisconnectRSatSession
// Get the profile status
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CSatUiObserver::DisconnectRSatSession()
    {
    TFLOGSTRING("SATAPP: CSatUiObserver::DisconnectRSatSession called")
    iSat.Close();
    iSatSession.Close();
    TFLOGSTRING("SATAPP: CSatUiObserver::DisconnectRSatSession exit")
    }

//  End of File
