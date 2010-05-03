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

#ifndef SATAPPEVENTPROVIDER_H
#define SATAPPEVENTPROVIDER_H

#include <QObject>
#include <msatuiobserver.h>
#include "msatuiactionimplementer.h"

class CSatUiObserver;
class SatAppPlayToneProvider;


class SatAppEventProvider: public QObject,
                           public MSatUiActionImplementer
{
    Q_OBJECT

public:

    /**
    * Constructor
    */
    SatAppEventProvider(QObject *parent = 0);

    /**
    * Distructor
    */
    virtual ~SatAppEventProvider();

signals:
     /**
     * Notification of the SAT Display Text command.
     * @param aRes, the User's response.
     * @param aText The text string to be displayed.
     * @param aSimApplicationName  Sim Application name
     * @param aIcon the icon to show.
     * @param aRequestedIconDisplayed Informs if icon is not used.
     * @param aSustainedText Indicates is this text sustained ie. no timers
     *        used if true.
     * @param aDuration Duration for showing the text
     * @param aWaitUserToClear Indication if user is needed to clear
     *        message
     */
    void displayTextEvent(
        TSatUiResponse &aRes,
        const QString &aText,
        const QString &aSimApplicationName,
        bool &aRequestedIconDisplayed,
        const bool aSustainedText,
        const int aDuration,
        const bool aWaitUserToClear);

    /**
     * Notification of the SAT Get Input command.
     * @param aText The query text.
     * @param aCharacterSet The character range allowed.
     * @param aInput The input string.
     * @param aMaxLength The maximum length of the input allowed.
     * @param aMinLength The minimum length of the input allowed.
     * @param aHideInput A flag indicating if the input should be hidden.
     * @param aHelpIsAvailable indicates if help can be requested.
     * @param aIconId The id of icon.
     * @param aRequestedIconDisplayed Informs if icon is not used.
     * @return The response of the UI to this command.
     */
    void getInputEvent(
        TSatUiResponse &aRes,
        const QString &aTitleText,
        const TSatCharacterSet aCharacterSet,
        QString &aInputText,
        const int aMinLength,
        const int aMaxLength,
        const bool aHideInput,
        const bool aSelfExplanatory,
        unsigned int &aDuration);

    /**
     * Notification of the SAT Get Inkey command.
     * @param aText The query text.
     * @param aCharacterSet The character range allowed.
     * @param aInput The input string.
     * @param aHideInput A flag indicating if the input should be hidden.
     * @param aHelpIsAvailable indicates if help can be requested.
     * @param aIconId The id of icon.
     * @param aRequestedIconDisplayed Informs if icon is not used.
     */
    void getInkeyEvent(
        TSatUiResponse &aRes,
        const QString &aTitleText,
        const TSatCharacterSet aCharacterSet,
        QString &aInputText,
        const bool aSelfExplanatory,
        unsigned int &aDuration);

    /**
     * Notification of the SAT Get YesNo command.
     * @param aText The query text.
     * @param aCharacterSet The character range allowed.
     * @param aInput The input character.
     * @param aHelpIsAvailable indicates if help can be requested.
     * @param aIconId The id of icon.
     * @param aRequestedIconDisplayed Informs if icon is not used.
     * @param aDuration Duration for showing the dialog.
     * @param aImmediateDigitResponse Indication if Immediate digit response
     *        is needed.
     * @return The response of the UI to this command.
     */
    void getYesNoEvent(
        TSatUiResponse &aRes,
        const QString &aText,
        const TSatCharacterSet aCharacterSet,
        unsigned int &aInkey,
        //const TSatIconId &aIconId,
        const bool &aSelfExplanatory,
        unsigned int &aDuration,
        const bool aImmediateDigitResponse);
        
    /**
     * Notification of the SAT Call Control command.
     * @param aRes The response.
     * @param aText The heading.
     * @param aAlphaIdStatus The alpha ID status.
     * 
     */
    void callControlEvent(
        const QString &aText,
        const TSatAlphaIdStatus aAlphaIdStatus);

    /**
     * Notification of the SAT Call Control command.
     * @param aRes The response.
     * @param aText The heading.
     * @param aAlphaIdStatus The alpha ID status.
     *
     */
    void moSmControlEvent(
        const QString &aText,
        const TSatAlphaIdStatus aAlphaIdStatus);

    /**
    * Handles the SetUpMenu command.
    * @param aRes the out parameter. the result of the command.
    * @param aText The title text.
    * @param aMenuItems The array of menu item captions.
    * @param aMenuItemNextActions The array of menu item next action
    * indicator captions.
    * @param aIconBitmap The Context pane icon.
    * @param aItemsIconArray Array of item icons
    * @param aSelfExplanatoryItems A flag indicating if only icons are shown.
    * @param aHelpIsAvailable A flag indicating if SAT help is available.
    */
    void setUpMenuEvent(
        TSatUiResponse &aRes,
        const QString &aText,
        const QStringList &aMenuItems,
        const bool aSelfExplanatoryItems,
        const bool aHelpIsAvailable);

    /**
    * Handles the SelectItem view.
    * @param aRes the out parameter. the result of the command.
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
    void selectItemEvent(
        TSatUiResponse &aRes,
        const QString &aText,
        const QStringList &aMenuItems,
        const int aDefaultItem,
        unsigned char &aSelection,
        const bool aSelfExplanatoryItems,
        const bool aHelpIsAvailable);

    /**
    * Shows the confirmation note about the SAT Send SMS, Send SS or
    * Send USSD command.
    * @param aText The text to be displayed.
    * @param aActionAccepted Indicates whether the command was accepted.
    * @param aType Type of the request (SMS, SS, USSD).
    * @return Information of the operation result.
    */
    void confirmSendEvent(
        TSatUiResponse &aRes,
        const QString &aText,
        bool &aActionAccepted,
        int aType);

     /**
     * Shows the wait note about the Send SMS command.
     * @param aText The text to be displayed.
     * @param aIconBitmapSendSM Sending dialog icon.
     * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.
     */
    void showSmsWaitNoteEvent(
        const QString &aText,
        const bool aSelfExplanatoryIcon);
    
    /**
    * Shows the confirmation not about SetUpCall.
    * @param aText The text to be displayed.
    * @param aSimAppName The header to be displayed.
    * @param aActionAccepted Indicates whether the command was accepted.
    */
   void showSetUpCallConfirmEvent(
        const QString &aText,
        const QString &aSimAppName,
        bool &aActionAccepted);

     /**
     * Shows the wait note about the Send DTMF command.
     * @param aText The text to be displayed.
     * @param aIconBitmapSendSM Sending dialog icon.
     */
    void showDtmfWaitNoteEvent(
        TSatUiResponse &aRes,
        const QString &aText);


     /**
     * Shows the wait note about the Send Ss or Ussd command.
     * @param aText The text to be displayed.
     * @param aIconBitmapSendSM Sending dialog icon.
     * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.
     */
    void showSsWaitNoteEvent(
        const QString &aText,
        const bool aSelfExplanatoryIcon);

     /**
     * Shows the wait note about Open Channel
     * @param aText The text to be displayed.
     * @param aActionAccepted Indicates whether the command was accepted.
     */
    void showOpenChannelConfirmEvent(
        const QString &aText,
        bool &aActionAccepted);


    /*!
     Removes Display Text Dialog from the screen.
    */
    void clearScreenEvent();

    /*!
     Close the sat ui
     */
    void closeUiEvent();

    /*!
    *Close wait note
    */
    void stopShowWaitNoteEvent();

    /*!
    *Show SsWaitNote without Delay
    */  
    void showWaitNoteWithoutDelayEvent();

    /*!
    *ShowSsErrorNoteEvent
    */  
    void showSsErrorNoteEvent();
    
    /*!
    * Show BIP related Note 
    */      
    void showBIPNoteEvent(int aCommand, const QString &aText);

public slots:
    /*!
    *User cancel response, Send DTMF, Send Data, Receive Data
    */
    void userCancelResponse();

public: // from MSatUiActionImplementer and impletment by QT

    /**
    * Shows the wait note.
    */
    virtual void ShowWaitNoteL() {};

    /**
    * Shows the wait note without delay
    */
    virtual void ShowWaitNoteWithoutDelayL();

    /**
    * Removes the wait note from the screen.
    */
    void StopShowWaitNote();

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
     virtual TSatUiResponse SetUpMenuL(
         const TDesC &aText,
         const MDesCArray &aMenuItems,
         const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
         const CFbsBitmap *aIconBitmap,
         const TBool aSelfExplanatoryItems,
         const TBool aHelpIsAvailable);

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
     virtual TSatUiResponse SelectItemL(const TDesC &aText,
         const MDesCArray &aMenuItems,
         const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
         const TInt aDefaultItem,
         TUint8 &aSelection,
         const CFbsBitmap *aIconBitmap,
         const TBool aSelfExplanatoryItems,
         const TBool aHelpIsAvailable);

    /**
    * Returns CoeEnv.
    */
    virtual CCoeEnv* CoeEnv() { return 0; };

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
    virtual TSatUiResponse DisplayTextL(
        const TDesC &aText,
        const TDesC &aSimApplicationName,
        CFbsBitmap *aIconBitmapDisplayText,
        const TBool aSelfExplanatoryIcon,
        const TBool aSustainedText,
        const TTimeIntervalSeconds aDuration,
        const TBool aWaitUserToClear);

    /**
    * Removes Display Text Dialog from the screen.
    */
    virtual void ClearScreen();

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
    virtual TSatUiResponse GetYesNoL(
        const TDesC &aText,
        const TSatCharacterSet aCharacterSet,
        TChar &aInkey,
        const CFbsBitmap */*aIconBitmap*/,
        const TBool aSelfExplanatory,
        TUint &aDuration,
        const TBool aImmediateDigitResponse);

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
    virtual TSatUiResponse GetInputL(
        const TDesC &aText,
        const TSatCharacterSet aCharacterSet,
        TDes &aInput,
        const TInt aMinLength,
        const TInt aMaxLength,
        const TBool aHideInput,
        const TBool aGetInkey,
        const CFbsBitmap *aIconBitmapGetInput,
        const TBool aSelfExplanatory,
        TUint &aDuration);


    /**
    * Handles Standard Tone playing.
    * @param aText The text to be displayed.
    * @param aFileName The name of the file corresponding to the tone.
    * @param aDuration The duration of the tone to be played.
    * @param aIconBitmap The tone playing note icon
    * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
    * @return Information of the operation result.
    */
    virtual TSatUiResponse PlayStandardToneL(
        const TDesC &aText,
        const TDesC8 &aSequence,
        TTimeIntervalMicroSeconds aDuration,
        const CFbsBitmap *aIconBitmap,
        const TBool aSelfExplanatory);

    /**
    * Shows the confirmation note about the SAT Send SMS, Send SS or
    * Send USSD command.
    * @param aText The text to be displayed.
    * @param aActionAccepted Indicates whether the command was accepted.
    * @param aType Type of the request (SMS, SS, USSD).
    * @return Information of the operation result.
    */
    virtual TSatUiResponse ConfirmSendL(
        const TDesC &aText,
        TBool &aActionAccepted,
        TInt aType);

    /**
    * Shows the wait note about the Send SMS command.
    * @param aText The text to be displayed.
    * @param aIconBitmapSendSM Sending dialog icon.
    * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.
    */
    virtual void ShowSmsWaitNoteL(
        const TDesC &aText,
        const CFbsBitmap *aIconBitmapSendSM,
        const TBool aSelfExplanatoryIcon);

    /**
    * Shows the wait note about the Send SS command.
    * @param aText The text to be displayed.
    * @param aIconBitmap Sending dialog icon.
    * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.
    */
    virtual void ShowSsWaitNoteL(
        const TDesC &aText,
        const CFbsBitmap */*aIconBitmap*/,
        const TBool aSelfExplanatoryIcon);

    /**
    * Shows the confirmation note about the SAT Refresh command.
    * @param aActionAccepted Indicates whether the command was accepted.
    * @return Information of the operation result.
    */
    virtual TSatUiResponse ConfirmRefreshL(
            TBool &/*aActionAccepted*/)
        {
            return ESatSuccess;
        };

    /**
    * Notification of the SAT Call Control command.
    * @param aText The text string to be displayed.
    * @param aAlphaIdStatus Alpha Identifier status
    * @param aControlResult Call control general result
    * @return The response from the UI
    */
    virtual TSatUiResponse CallControlL(
        const TDesC &aText,
        const TSatAlphaIdStatus aAlphaIdStatus);

    /**
    * Notification of the SAT Launch browser confirmation request
    * @param aText The text to be displayed.
    * @param aActionAccepted Indicates whether the command was accepted.
    * @param aIconBitmap The confirm note icon
    * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
    */
    virtual void ConfirmLaunchBrowserL(
        const TDesC &/*aText*/,
        TBool &/*aActionAccepted*/,
        const CFbsBitmap */*aIconBitmap*/,
        const TBool /*aSelfExplanatory*/) {};

    /**
    * Notification of the SAT Mo Sm Control command.
    * @param aText The text string to be displayed.
    * @param aAlphaIdStatus Alpha Identifier status
    * @param aControlResult Mo Sm control general result
    * @return The response from the UI
    */
    virtual TSatUiResponse MoSmControlL(
        const TDesC &aText,
        const TSatAlphaIdStatus aAlphaIdStatus);

    /**
    * Shows the wait note while DTMF are being sent.
    * @param aText The text to be displayed.
    * @param aIconBitmap The confirm note icon
    * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
    * @return Information on the operation result.
    */
    virtual TSatUiResponse ShowDtmfWaitNoteL(
            const TDesC &aText,
            const CFbsBitmap *aIconBitmap,
            const TBool aSelfExplanatoryIcon);

    /**
    * Dispatch iWait to action implementer.
    * @param aWait Timer.
    */
    virtual void DispatchTimer(CActiveSchedulerWait &/*aWait*/) {};

    /**
    * Get flag which is indicating if end key is pressed.
    * @return A Boolean flag which is indicating if end key is pressed.
    *         EndKey not support, Open issue
    */
    virtual TBool GetEndKey(){return EFalse;};

    /**
    * Set a flag which is indicating if end key is pressed.
    * @param aValue Status of end key
    */
    virtual void SetEndKey(TBool /*aValue*/) {};

    /**
    * Set a flag which is indicating if proactive command is pending.
    * @param aValue Status of proactive command
    */
    virtual void SetCommandPending(TBool /*aValue*/) {};

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
        const TDesC &aText,
        TTimeIntervalMicroSeconds aDuration,
        TSatTone aTone,
        const CFbsBitmap *aIconBitmap,
        const TBool aSelfExplanatory);

    /**
    * Confirm user permission for Open Channel.
    * @param aText The text to be displayed.
    * @param aActionAccepted Indicates whether the command was accepted.
    * @param aIconBitmapOpenChannel Icon for query dialog
    * @param aSelfExplanatory A flag indicating if icon is self-explanatory
    * @return Operation result
    */
    virtual TSatUiResponse ConfirmOpenChannelL(
        const TDesC &aText,
        TBool &aActionAccepted,
        const CFbsBitmap */*aIconBitmapOpenChannel*/,
        const TBool aSelfExplanatory);

    /**
    * Shows the wait note while BIP packets are sent.
    * @param aCommand Command type.
    * @param aText The text to be displayed.
    * @param aIconBitmap Icon for query dialog
    * @param aSelfExplanatory A flag indicating if icon is self-explanatory
    */
    virtual void ShowBIPNoteL(
        TInt aCommand,
        const TDesC &aText,
        const CFbsBitmap */*aIconBitmap*/,
        const TBool /*aSelfExplanatory*/);

    /**
    * Confirm user permission for SetUpCall
    * @param aText The text to be displayed.
    * @param aSimAppName Header for th equery
    * @param aActionAccepted Indicates whether the command was accepted.
    * @param aIconBitmap Icon for query dialog
    * @param aSelfExplanatory A flag indicating if icon is self-explanatory
    */
    virtual void ConfirmSetUpCallL(
        const TDesC &aText,
        const TDesC &aSimAppName,
        TBool &aActionAccepted,
        const CFbsBitmap *aIconBitmap,
        const TBool aSelfExplanatory);

    /**
    * Show note indicating SAT is not available.
    */
    virtual void ShowNotAvailableNoteL() {};

    /**
    * Show the Ss error note.
    */
    virtual void ShowSsErrorNoteL();

    /**
    * Close SatUi process.
    */
    virtual void CloseSatUI();

public: // new method

    /**
    * Return current profile state.
    * @return current profile state.
    */
    int profileState();

     /**
     * Response to the Set Up Menu command.
     * @param aMenuItem The selected menu item.
     * @param aHelpRequested Indicates whether help was requested.
     */
    void menuSelection(int aMenuItem, bool aHelpRequested);

private: // Data

    /**
     * The pointer to the sybmian class CSatUiObserver
     * which will access Sat Client API
     * own
     */
    CSatUiObserver *mObs;

    /**
     * Own, PlayTone
     */
    SatAppPlayToneProvider *mPlayTone;

};

#endif
