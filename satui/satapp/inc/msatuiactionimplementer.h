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
* Description:
*
*
*/

#ifndef MSATUIACTIONIMPLEMENTER_H
#define MSATUIACTIONIMPLEMENTER_H

//  INCLUDES
#include <msatuiobserver.h>

// CLASS DECLARATION
class CCoeEnv;
class CFbsBitmap;

/**
*  This class defines the interface between engine and UI.
*/
class MSatUiActionImplementer
    {
    public:

        /**
        * Returns CoeEnv.
        */
        virtual CCoeEnv* CoeEnv() = 0;

        /**
        * Shows the wait note.
        */
        virtual void ShowWaitNoteL() = 0;

        /**
        * Shows the wait note without delay
        */
        virtual void ShowWaitNoteWithoutDelayL() = 0;

        /**
        * Removes the wait note from the screen.
        */
        virtual void StopShowWaitNote() = 0;

        /**
        * Handles the DisplayText operation.
        * @param aText Text to be shown.
        * @param aSimApplicationName  Sim Application name
        * @param aIconBitmapDisplayText Icon for dialog
        * @param aSustainedText Indicates is this text sustained ie. no timers
        *        used if ETrue.
        * @param aDuration Duration for showing the text
        * @param aWaitUserToClear Indication if user is needed to clear
        *        message
        * @return Information of the operation result.
        */
        virtual TSatUiResponse DisplayTextL( const TDesC& aText,
            const TDesC& aSimApplicationName,
            CFbsBitmap* aIconBitmapDisplayText,
            const TBool aSelfExplanatoryIcon,
            const TBool aSustainedText,
            const TTimeIntervalSeconds aDuration,
            const TBool aWaitUserToClear ) = 0;

        /**
        * Removes Display Text Dialog from the screen.
        */
        virtual void ClearScreen() = 0;

        /**
        * Asks a Yes/No answer from the user.
        * @param aText A question to be shown.
        * @param aCharacterSet The character range allowed.
        * @param aInkey Character user feeds.
        * @param aIconBitmap Icon for query dialog.
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @param aDuration Duration for showing the dialog.
        * @param aImmediateDigitResponse Indication if Immediate digit response
        *        is needed.
        * @return Information of the operation result.
        */
        virtual TSatUiResponse GetYesNoL( const TDesC& aText,
            const TSatCharacterSet aCharacterSet,
            TChar& aInkey,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory,
            TUint& aDuration,
            const TBool aImmediateDigitResponse ) = 0;

        /**
        * Asks input from the user.
        * @param aText A question to be shown.
        * @param aCharacterSet The character range allowed.
        * @param aInput The input string.
        * @param aMinLength The minimum length of the input allowed.
        * @param aMaxLength The maximum length of the input allowed.
        * @param aHideInput A flag indicating if the input should be hidden.
        * @param aGetInkey A flag indicating if GetInkey command is used.
        * @param aIconBitmapGetInput Icon for query dialog.
        * @param aSelfExplanatory  A flag indicating if icon is self-explanatory.
        * @param aDuration Duration for showing the dialog.
        * @return Information of the operation result.
        */
        virtual TSatUiResponse GetInputL( const TDesC& aText,
            const TSatCharacterSet aCharacterSet, TDes& aInput,
            const TInt aMinLength, const TInt aMaxLength,
            const TBool aHideInput, const TBool aGetInkey,
            const CFbsBitmap* aIconBitmapGetInput,
            const TBool aSelfExplanatory,
            TUint& aDuration ) = 0;

        /**
        * Handles the SetUpMenu view.
        * @param aText The title text.
        * @param aMenuItems The array of menu item captions.
        * @param aMenuItemNextActions The array of menu item next action
        * indicator captions.
        * @param aIconBitmap The Context pane icon.
        * @param aItemsIconArray Array of item icons
        * @param aSelfExplanatoryItems Flag indicating if only icons are shown.
        * @param aHelpIsAvailable A flag indicating if SAT help is available.
        * @return Information of the operation result.
        */
        virtual TSatUiResponse SetUpMenuL( const TDesC& aText,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const CFbsBitmap* aIconBitmap,
            //const CAknIconArray* aItemIconsArray,
            const TBool aSelfExplanatoryItems,
            const TBool aHelpIsAvailable ) = 0;

       /**
        * Handles the SelectItem view.
        * @param aText The title text.
        * @param aMenuItems The array of menu items.
        * @param aMenuItemNextActions The array of next action indicators.
        * @param aDefaultItem The item in focus.
        * @param aSelection The item index selected by the user.
        * @param aIconBitmap The Context Pane icon.
        * @param aItemsIconArray Array of item icons
        * @param aSelfExplanatoryItems Flag indicating if only icons are shown.
        * @param aHelpIsAvailable A flag indicating if SAT help is available.
        * @return Information of the operation result.
        */
        virtual TSatUiResponse SelectItemL( const TDesC& aText,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TInt aDefaultItem,
            TUint8& aSelection,
            const CFbsBitmap* aIconBitmap,
            //const CAknIconArray* aItemsIconArray,
            const TBool aSelfExplanatoryItems,
            const TBool aHelpIsAvailable ) = 0;

        /**
        * Handles Standard Tone playing.
        * @param aText The text to be displayed.
        * @param aFileName The name of the file corresponding to the tone.
        * @param aDuration The duration of the tone to be played.
        * @param aIconBitmap The tone playing note icon
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @return Information of the operation result.
        */
        virtual TSatUiResponse PlayStandardToneL( const TDesC& aText,
            const TDesC8& aSequence,
            TTimeIntervalMicroSeconds aDuration,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory ) = 0;

        /**
        * Shows the confirmation note about the SAT Send SMS, Send SS or
        * Send USSD command.
        * @param aText The text to be displayed.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aType Type of the request (SMS, SS, USSD).
        * @return Information of the operation result.
        */
        virtual TSatUiResponse ConfirmSendL( const TDesC& aText,
            TBool& aActionAccepted, TInt aType ) = 0;

        /**
        * Shows the wait note about the Send SMS command.
        * @param aText The text to be displayed.
        * @param aIconBitmapSendSM Sending dialog icon.
        * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.
        */
        virtual void ShowSmsWaitNoteL( const TDesC& aText,
            const CFbsBitmap* aIconBitmapSendSM,
            const TBool aSelfExplanatoryIcon ) = 0;

        /**
        * Shows the wait note about the Send SS command.
        * @param aText The text to be displayed.
        * @param aIconBitmap Sending dialog icon.
        * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.
        */
        virtual void ShowSsWaitNoteL( const TDesC& aText,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatoryIcon ) = 0;

        /**
        * Shows the confirmation note about the SAT Refresh command.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @return Information of the operation result.
        */
        virtual TSatUiResponse ConfirmRefreshL(
            TBool& aActionAccepted) = 0;

        /**
        * Notification of the SAT Call Control command.
        * @param aText The text string to be displayed.
        * @param aAlphaIdStatus Alpha Identifier status
        * @param aControlResult Call control general result
        * @return The response from the UI
        */
        virtual TSatUiResponse CallControlL(
            const TDesC& aText,
            const TSatAlphaIdStatus aAlphaIdStatus ) = 0;

        /**
        * Notification of the SAT Launch browser confirmation request
        * @param aText The text to be displayed.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aIconBitmap The confirm note icon
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        */
        virtual void ConfirmLaunchBrowserL(
            const TDesC& aText,
            TBool& aActionAccepted,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory ) = 0;

        /**
        * Notification of the SAT Mo Sm Control command.
        * @param aText The text string to be displayed.
        * @param aAlphaIdStatus Alpha Identifier status
        * @param aControlResult Mo Sm control general result
        * @return The response from the UI
        */
        virtual TSatUiResponse MoSmControlL(
            const TDesC& aText,
            const TSatAlphaIdStatus aAlphaIdStatus ) = 0;

        /**
        * Shows the wait note while DTMF are being sent.
        * @param aText The text to be displayed.
        * @param aIconBitmap The confirm note icon
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @return Information on the operation result.
        */
        virtual TSatUiResponse ShowDtmfWaitNoteL( const TDesC& aText,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatoryIcon ) = 0;

        /** Dispatch iWait to action implementer.
        * @param aWait Timer.
        */
        virtual void DispatchTimer( CActiveSchedulerWait& aWait ) = 0;

        /**
        * Get flag which is indicating if end key is pressed.
        * @return A Boolean flag which is indicating if end key is pressed.
        */
        virtual TBool GetEndKey() = 0;

        /**
        * Set a flag which is indicating if end key is pressed.
        * @param aValue Status of end key
        */
        virtual void SetEndKey( TBool aValue ) = 0;

        /**
        * Set a flag which is indicating if proactive command is pending.
        * @param aValue Status of proactive command
        */
        virtual void SetCommandPending( TBool aValue ) = 0;

        /**
        * Plays user selected tone.
        * @param aText The text to be displayed.
        * @param aDuration Play duration.
        * @param aTone Tone to be played.
        * @param aIconBitmap The tone playing note icon
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @return Information on the operation result.
        */
        virtual TSatUiResponse PlayUserSelectedToneL(
            const TDesC& aText,
            TTimeIntervalMicroSeconds aDuration,
            TSatTone aTone,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory ) = 0;

        /**
        * Confirm user permission for Open Channel.
        * @param aText The text to be displayed.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aIconBitmapOpenChannel Icon for query dialog
        * @param aSelfExplanatory A flag indicating if icon is self-explanatory
        * @return Operation result
        */
        virtual TSatUiResponse ConfirmOpenChannelL(
            const TDesC& aText,
            TBool& aActionAccepted,
            const CFbsBitmap* aIconBitmapOpenChannel,
            const TBool aSelfExplanatory ) = 0;

        /**
        * Shows the wait note while BIP packets are sent.
        * @param aCommand Command type.
        * @param aText The text to be displayed.
        * @param aIconBitmap Icon for query dialog
        * @param aSelfExplanatory A flag indicating if icon is self-explanatory
        */
        virtual void ShowBIPNoteL(
            TInt aCommand,
            const TDesC& aText,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory ) = 0;

        /**
        * Confirm user permission for SetUpCall
        * @param aText The text to be displayed.
        * @param aSimAppName Header for th equery
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aIconBitmap Icon for query dialog
        * @param aSelfExplanatory A flag indicating if icon is self-explanatory
        */
        virtual void ConfirmSetUpCallL(
            const TDesC& aText,
            const TDesC& aSimAppName,
            TBool& aActionAccepted,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory ) = 0;

        /**
        * Show note indicating SAT is not available.
        */
        virtual void ShowNotAvailableNoteL() = 0;

        /**
        * Show the Ss error note.
        */
        virtual void ShowSsErrorNoteL() = 0;

        /**
        * Start SatUi closing process.
        */
        virtual void CloseSatUI() = 0;

    };

#endif    // MSATUIACTIONIMPLEMENTER_H

//  End of File
