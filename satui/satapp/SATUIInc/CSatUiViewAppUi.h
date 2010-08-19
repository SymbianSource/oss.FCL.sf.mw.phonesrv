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
* Description:  Defines CSatUiViewAppUi class
*
*/


#ifndef CSATUIVIEWAPPUI_H
#define CSATUIVIEWAPPUI_H

// INCLUDES
#include <aknViewAppUi.h>
#include <mdaaudiotoneplayer.h>
#include <mdaaudiosampleplayer.h>
#include "MSatUiActionImplementer.h"
#include "CSatUiActionPerformer.h"

// CONSTANTS
const TUid KSetUpMenuViewId={1};
const TUid KSelectItemViewId={2};

// FORWARD DECLARATIONS
class CSatUiView;
class MSatUiAdapter;
class CAknMessageQueryDialog;
class CAknWaitDialog;
class CSatUiTextQueryDialog;
class CAknQueryDialog;
class CSatUiInactivityMonitor;
class CAknIconArray;
class CAknPopupForm; 
class CAknGlobalNote;
class CSatUiWaitDialog;

// CLASS DECLARATION

/**
* Implements the UI and methods from the MSatUiActionImplementer.
*/
class CSatUiViewAppUi : public CAknViewAppUi,
    public MSatUiActionImplementer,
    public MMdaAudioToneObserver,
    public MMdaAudioPlayerCallback
    {
    public: // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CSatUiViewAppUi();

        /**
        * Destructor.
        */
        ~CSatUiViewAppUi();

    public: // New functions

        /**
        * Sets the pointer to adapter provided by SatCli.
        * @param aAdapter SAT Client part object.
        */
        void SetAdapter( MSatUiAdapter* aAdapter );

    public: // Functions from base classes

        /**
        * From CAknViewAppUi Called when view is brought to
        * foreground or put into background
        * @param aForeground A flag to indicate is SAT background or foreground
        */
        void HandleForegroundEventL( TBool aForeground );

        /**
        * From MMdaAudioToneObserver Called by the player object when
        * it has been initialised.
        * @param aError Error type.
        */
        void MatoPrepareComplete( TInt aError );

        /**
        * From MMdaAudioToneObserver Called by the player object when
        * the playback has been terminated.
        * @param aError Error type.
        */
        void MatoPlayComplete( TInt aError );

        /**
        * From MSatUiActionImplementer Shows the wait note.
        */
        void ShowWaitNoteL();

        /**
        * From MSatUiActionImplementer Shows the wait note without delay.
        */
        void ShowWaitNoteWithoutDelayL();

        /**
        * From MSatUiActionImplementer Removes the wait note from the screen.
        */
        void StopShowWaitNote();

        /**
        * Checks that is BT SAP connection active.
        * @return BT SAP activation status.
        */
        TBool BTSAPActivatedL();

        /**
        * From MSatUiActionImplementer 
        * @return CoeEnv.
        */
        CCoeEnv* CoeEnv();

        /**
        * From MSatUiActionImplementer Displays given text on the screen.
        * @param aText Text to be shown.
        * @param aSimApplicationName  Sim Application name
        * @param aIconBitmapDisplayText Icon for dialog
        * @param aSelfExplanatoryIcon If selfexplanatory icon
        * @param aSustainedText Indicates is this text sustained ie. no timers
        *        used if ETrue.
        * @param aDuration Duration for showing the text
        * @param aWaitUserToClear Indication if user is needed to clear 
        *        message
        * @return Operation result.
        */
        TSatUiResponse DisplayTextL( const TDesC& aText,
            const TDesC& aSimApplicationName,
            CFbsBitmap* aIconBitmapDisplayText,
            const TBool aSelfExplanatoryIcon,
            const TBool aSustainedText,
            const TTimeIntervalSeconds aDuration,
            const TBool aWaitUserToClear );

        /**
        * From MSatUiActionImplementer Removes Display Text Dialog from
        * the screen.
        */
        void ClearScreen();

        /**
        * From MSatUiActionImplementer Asks a Yes/No answer from the user.
        * @param aText A question to be shown.
        * @param aCharacterSet The character range allowed.
        * @param aInkey Character user feeds.
        * @param aIconBitmap Icon for query dialog.
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @param aDuration Duration for showing the dialog.
        * @param aImmediateDigitResponse Indication if Immediate digit response
        *        is needed.
        * @return Operation result.
        */
        TSatUiResponse GetYesNoL( const TDesC& aText,
            const TSatCharacterSet aCharacterSet,
            TChar& aInkey,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory,
            TUint& aDuration,
            const TBool aImmediateDigitResponse );

        /**
        * From MSatUiActionImplementer Asks input from the user.
        * @param aText A question to be shown.
        * @param aCharacterSet The character range allowed.
        * @param aInput The input string.
        * @param aMinLength The minimum length of the input allowed.
        * @param aMaxLength The maximum length of the input allowed.
        * @param aHideInput A flag indicating if the input should be hidden.
        * @param aGetInkey A flag indicating GetInkey command.
        * @param aIconBitmapGetInput Icon for query dialog.
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @param aDuration Duration for showing the dialog.
        * @return Operation result.
        */
        TSatUiResponse GetInputL( const TDesC& aText,
            const TSatCharacterSet aCharacterSet,
            TDes& aInput,
            const TInt aMinLength,
            const TInt aMaxLength, 
            const TBool aHideInput,
            const TBool aGetInkey,
            const CFbsBitmap* aIconBitmapGetInput,
            const TBool aSelfExplanatory,
            TUint& aDuration );

        /**
        * From MSatUiActionImplementer Prepares the SetUpMenu view and
        * calls the method of the view class.
        * @param aText The title text.
        * @param aMenuItems The array of menu items.
        * @param aMenuItemNextActions The array of next action indicators.
        * @param aIconBitmap The Context Pane icon.
        * @param aItemsIconArray Array of item icons
        * @param aSelfExplanatoryItems A flag indicating if only icons are shown.
        * @param aHelpIsAvailable A flag indicating if SAT help is available.
        * @return Operation result.
        */    
        TSatUiResponse SetUpMenuL( const TDesC& aText,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const CFbsBitmap* aIconBitmap,
            const CAknIconArray* aItemsIconArray,
            const TBool aSelfExplanatoryItems,
            const TBool aHelpIsAvailable );

       /**
        * From MSatUiActionImplementer Prepares the SelectItem view and
        * calls the method of the view class.
        * @param aText The title text.
        * @param aMenuItems The array of menu items.
        * @param aMenuItemNextActions The array of next action indicators.
        * @param aDefaultItem The item in focus.
        * @param aSelection The item index selected by the user.
        * @param aIconBitmap The Context Pane icon.
        * @param aItemsIconArray Array of item icons
        * @param aSelfExplanatoryItems A flag indicating if only icons are shown.
        * @param aHelpIsAvailable A flag indicating if SAT help is available.
        * @return Operation result.
        */
        TSatUiResponse SelectItemL( const TDesC& aText,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TInt aDefaultItem,
            TUint8& aSelection,
            const CFbsBitmap* aIconBitmap,
            const CAknIconArray* aItemsIconArray,
            const TBool aSelfExplanatoryItems,
            const TBool aHelpIsAvailable );

        /**
        * From MSatUiActionImplementer Plays Standard Tone.
        * @param aText The text to be displayed.
        * @param aSequence The name of the file to be played.
        * @param aDuration The duration of the tone to be played.
        * @param aIconBitmap The tone playing note icon
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @return Operation result.
        */
        TSatUiResponse PlayStandardToneL( const TDesC& aText, 
            const TDesC8& aSequence,
            TTimeIntervalMicroSeconds aDuration,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory );

        /** 
        * From MSatUiActionImplementer Shows a query to get confirmation
        * for Send SMS, Send SS or Send USSD command.
        * @param aText The text to be displayed.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aType Type of the command (SMS, SS or USSD).
        * @return Operation result.
        */
        TSatUiResponse ConfirmSendL( const TDesC& aText,
            TBool& aActionAccepted, TInt aType );

        /** 
        * From MSatUiActionImplementer Shows a wait note indicating
        * Send SMS command.
        * @param aText The text to be displayed.
        * @param aIconBitmapSendSM Sending dialog icon.
        * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.        
        */
        void ShowSmsWaitNoteL( const TDesC& aText,
            const CFbsBitmap* aIconBitmapSendSM, 
            const TBool aSelfExplanatoryIcon );

        /** 
        * From MSatUiActionImplementer Shows a wait note indicating
        * Send SS command.
        * @param aText The text to be displayed.
        * @param aIconBitmap Sending dialog icon.
        * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.        
        */
        void ShowSsWaitNoteL( const TDesC& aText, 
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatoryIcon );

        /** 
        * From MSatUiActionImplementer Shows the query to confirm the
        * Refresh command.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @return Operation result.
        */
        TSatUiResponse ConfirmRefreshL( TBool& aActionAccepted );

        /**
        * Notification of the SAT Call Control command.
        * @param aText The text string to be displayed.
        * @param aAlphaIdStatus Alpha Identifier status
        * @param aControlResult Call control general result
        * @return The response from the UI
        */
        TSatUiResponse CallControlL( const TDesC& aText, 
            const TSatAlphaIdStatus aAlphaIdStatus );

        /** 
        * Notification of the SAT Launch browser confirmation request
        * @param aText The text to be displayed.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aIconBitmap The confirm note icon
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        */
        void ConfirmLaunchBrowserL( 
            const TDesC& aText,
            TBool& aActionAccepted,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory );

        /** 
        * Status of help support
        * @param aStatus status of help
        */
        void HelpStatus( TBool aStatus );

        /**
        * Notification of the SAT Mo Sm Control command.
        * @param aText The text string to be displayed.
        * @param aAlphaIdStatus Alpha Identifier status
        * @param aControlResult Mo Sm control general result
        * @return The response from the UI
        */
        TSatUiResponse MoSmControlL( const TDesC& aText, 
            const TSatAlphaIdStatus aAlphaIdStatus );

        /**
        * From MSatUiActionImplementer Shows the wait note while DTMF are being 
        * sent.
        * @param aText The text to be displayed.
        * @param aIconBitmap The confirm note icon
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @return Information on the operation result.
        */
        TSatUiResponse ShowDtmfWaitNoteL( const TDesC& aText, 
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatoryIcon );

        /** 
        * Dispatch iWait to action implementer.
        * @param aWait Timer.
        */
        void DispatchTimer( CActiveSchedulerWait& aWait );

        /**
        * Get a flag which is indicating if end key is pressed.
        * @return A Boolean flag which is indicating if end key is pressed.
        */
        TBool GetEndKey(); 

        /**
        * Set a flag which is indicating if end key is pressed.
        * @param aValue Status of end key
        */
        void SetEndKey( TBool aValue );

        /**
        * Set a flag which is indicating if proactive command is pending.
        * @param aValue Status of proactive command
        */
        void SetCommandPending( TBool aValue );

        /**
        * Plays user selected tone.
        * @param aText The text to be displayed.
        * @param aDuration The duration of the tone to be played.
        * @param aTone Tone type.
        * @param aIconBitmap The tone playing note icon
        * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
        * @return Operation result.
        */
        TSatUiResponse PlayUserSelectedToneL(
            const TDesC& aText,
            TTimeIntervalMicroSeconds aDuration, 
            TSatTone aTone,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory );

        /**
        * Confirm user permission for Open Channel.
        * @param aText The text to be displayed.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aIconBitmapOpenChannel Icon for query dialog
        * @param aSelfExplanatory A flag indicating if icon is self-explanatory
        * @return Operation result
        */
        TSatUiResponse ConfirmOpenChannelL(
            const TDesC& aText,
            TBool& aActionAccepted,
            const CFbsBitmap* aIconBitmapOpenChannel,
            const TBool aSelfExplanatory );

        /**
        * From MSatUiActionImplementer Shows the wait note while BIP packets 
        * are sent.
        * @param aCommand Command type.
        * @param aText The text to be displayed.
        * @param aIconBitmap Icon for query dialog
        * @param aSelfExplanatory A flag indicating if icon is self-explanatory
        */
        void ShowBIPNoteL(
            TInt aCommand,
            const TDesC& aText,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory );
        
        /** 
        * From MSatUiActionImplementer Confirm user permission for SetUpCall
        * @param aText The text to be displayed.
        * @param aSimAppName Header for th equery
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aIconBitmap Icon for query dialog
        * @param aSelfExplanatory A flag indicating if icon is self-explanatory
        */
        void ConfirmSetUpCallL( 
            const TDesC& aText, 
            const TDesC& aSimAppName,
            TBool& aActionAccepted,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory );

        /**
        * From MSatUiActionImplementer Show note indicating SAT is not 
        * available.
        */        
        void ShowNotAvailableNoteL();                    


        /**
        * From MSatUiActionImplementer Show the Ss error note.
        */
        void ShowSsErrorNoteL();      

        /**
        * Start SatUi closing process
        */
        void CloseSatUI();
        
        /**
        * Get settings from current profile like volume, warning tones on/off etc.
        * @param aTone, optional specifies the type of needed tone if necessary
        * @param aToneName, optional buffer to get a soundfile name from 
        * the active profile if necessary
        */
        void GetProfileParamsL( TSatTone aTone = ESatToneNotSet, TDes* aToneName = NULL );
        

        /**
        * Return current profile state.
        * @return current profile state.
        */
        TInt ProfileState();

    public: // Functions from base classes

        /**
        * From CAknViewAppUi Handles UI commands.
        * @param aCommand Command to be handled.
        */
        void HandleCommandL( TInt aCommand );
        
    protected:
    
       /**
        * From CEikAppUi. Takes care of commandline parameter.
        * @param aCommand Indicate the way an application is to be launched.
        * @param aDocumentName A document file name.
        * @param aTail Command line parameter.
        */
        TBool ProcessCommandParametersL(
            TApaCommand aCommand,
            TFileName& aDocumentName, 
            const TDesC8& aTail );    
    
    private:

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    private: // functions
    
        /** 
        * Creates empty menu. This is needed in SIM originated commands.
        */
        void CreateEmptyMenuL();

        /** 
        * called when user has been
        * inactive for a pre-defined period
        * @param aPtr PPointer to creato
        * @return Error value
        */
        static TInt DispatchUserInactive( TAny* aPtr );

        /** 
        * Delay callback function. Called when predefined time
        * has passed
        * @param Pointer to creator
        * @return whetwer to call function again after an interval
        */
        static TInt DelayCallback( TAny* aPtr );

        /** 
        * Delay callback function. Called when predefined time
        * has passed
        * @param Pointer to creator
        * @return whetwer to call function again after an interval
        */
        static TInt CloseCallControlNoteL( TAny* aPtr );

        /**
        * Creates a callback timer
        * @param aDelay delay after callback function is called
        * @param aCallBack Callback function
        */
        void CreateCallBackTimerL( const TInt aDelay, 
                TInt (*aFunction)(TAny* aPtr) );

        /**
        * Destroys the callback timer
        */
        void DestroyCallBackTimer( );

        /**
        * Make a mask for get input icon bitmap
        * @param aMask Mask for Get inpupt icon bitmap    
        * @param aSize Size for mask
        */
        void MakeGetInputBitmapMask( CFbsBitmap* aMask, TSize aSize ) const;

        /**
        * Handles a exit softkey.
        */
        void HandleExitCommandL();

        /**
        * From CAknViewAppUi Handles window server events..
        * @param aEvent The window server event that occurred.
        * @param aDestination The control associated with the event.
        */
        void HandleWsEventL( const TWsEvent& aEvent, 
            CCoeControl* aDestination );

        /**
        * Close all existing dialogs.
        */
        void CloseDialogs();

        /**
        * Initialisation of an audio sample has completed.
        * @param aError The status of the audio sample after initialisation.
        * @param aDuration The duration of the audio sample.
        */
        void MapcInitComplete( 
            TInt aError, 
            const TTimeIntervalMicroSeconds& /*aDuration*/ );

        /**
        * Play of an audio sample has completed.
        * @param aError The status of playback.
        */
        void MapcPlayComplete( TInt aError );

        /**
        * Remove null characters from text.
        * @param aText Text to be cleaned from NULLs 
        */
        void RemoveNullCharacters( HBufC* aText );
        
        /**
        * Make and run note dialog with an icon
        * @param aResourceId Resource id of the dialog to be created
        * @param aText Text to the dialog
        * @param aIconBitmap Pointer to the icon bitmap
        * @param aSelfExplanatory Whether text is showed or not
        * @param aIconCommand Layout of dialog for scaling
        * @param aMskCommand middle softkey command
        * @return The user response to the note
        */
        TInt RunQueryDialogL(
            TInt aResourceId,
            const TDesC& aText,
            const CFbsBitmap* aIconBitmap,
            const TBool aSelfExplanatory,
            TInt aMskCommand );

        /**
        * Check if it is allowed to change view or SAT UI is in 
        * view activation loop.
        * @param aForeground ETrue if the application is in the foreground.
        * @param aActiveView TUid of active view.
        * @return ETrue if view activation is allowed.
        */
        TBool ViewActivationAllowed( const TBool aForeground, 
                                     const TUid  aActiveView );
                                     
        /**
        * Handle received key event in immediate digit response case.
        * @param aScanCode Received key event code.
        */
        void HandleImmediateDigitResponse( TInt aScanCode );

        /**
        * Handle closing display text in high priority events
        * @param aEvent The window server event that occurred.
        */
        void PriorityVerifyAndClose( const TWsEvent& aEvent );
        
        /**
        * Stop playing PLAY TONE command.
        */
        void StopPlayToneL();
        
        /**
        * Check if foreground control is alive when HandleForegroundEventL is 
        * called.
        * @return ETrue if the foreground control is alive.
        */
        TBool CSatUiViewAppUi::ForegroundAlive() const;
        
    private: //Data

        // Own: use timer repeat
        TBool iTimerRepeatOn;

        // Own: repeat count for timer
        TInt iTimerRepeatCount;

        TBool iHelp;

        // Session Terminated
        TBool iSelectItemPending;

        // select item command pending
        TBool iSessionTerminated;

        // delay timer
        CPeriodic* iTimer;

        // Own: indicates that note was cleared after a delay
        TBool iNoteClearedAfterDelay;

        // Own: indicates that a note was cleared after a delay
        TBool iNoResponseFromUserAfterDelay;

        //Ref: SatClient abstract adapter interface
        MSatUiAdapter* iAdapter;

        //Own: SetUpMenu view
        CSatUiView* iSetUpMenuView;

        //Own: SelectItem view
        CSatUiView* iSelectItemView;

        //The command comes first time, so the view should be created
        TBool iSetUpMenuFirst;
        TBool iSelectItemFirst;

        // Own: SIM application name
        HBufC* iAppName;
        // Own: view title
        HBufC* iTitle;

        //Own: Display Text dialog and text+icon
        CAknMessageQueryDialog* iDisplayTextDialog;
        CAknPopupForm* iDisplayTextIconDialog;

        //Own: get input dialog
        CSatUiTextQueryDialog* iGetInputDialog;
        
        //Own: Wait note
        CAknWaitDialog* iWaitNote;
        
        //Own: BIP Wait note
        CSatUiWaitDialog* iBipWaitNote;
        
        //Own: Refresh note
        CAknWaitDialog* iRefreshNote;

        //Own: Media server interface to play rtf file
        CMdaAudioToneUtility *iPlayer;

        //Failure while playing tone
        TSatUiResponse iPlayToneError;

        //wait scheduler
        CActiveSchedulerWait* iWait;

        // Flag for indicating end key status
        TBool iEndKeyPressed;

        // Status of view, select item or setup menu
        TUid iActiveView;

        // Flag for indicating if command is pending
        TBool iCommandPending;

        // Own: query dialog
        CAknQueryDialog* iQueryDialog;
        
        // Own: Inform dialog
        CAknNoteDialog* iNoteDialog;

        // Own: note dialog
        CAknStaticNoteDialog* iPermanentNote;

        // Player for user selected tones.
        CMdaAudioPlayerUtility* iAudioPlayer;

        // Volume for user selected tones.
        TInt iVolume;
        
        // Note for call control. 
        CAknGlobalNote* iCcNote;
        
        // The note's ID in call control. 
        TInt iCcNoteId;

        // Flag for indicating SatUI foreground status        
        TBool iForeground;

        // Flag indicating previous SatUI foreground status
        TBool iPreviousForeground;

        // Counter for possible view activation loop
        TInt iViewLoopCounter;
        
        // Duration of tone
        TTimeIntervalMicroSeconds iDuration;

        // Used key value for immediate digit response
        TChar iKey;
        
        // Indicate if immediate digit response is requested
        TBool iImmediateDigitResponse;

        // Indication for End key case that wait note existed
        TBool iWaitNoteInUse;
        
        // Request ID for increased buffer size.
        TInt iReqWSBufferID;
        
        // Indication that warning and game tones are in use
        TBool iWarningAndPlayTones;

        // Not Own
        // Use to keep track of the dialog which is on the screen
        CCoeControl* iForegroundControl;
        
        // Indication that high priority event occured, used in DisplayText
        TBool iHighPriorityEvent;
        
        // Indication that immediate response TLV, used in DisplayText
        TBool iImmediateRsp;

    };

#endif    // CSATUIVIEWAPPUI_H

//  End of File
