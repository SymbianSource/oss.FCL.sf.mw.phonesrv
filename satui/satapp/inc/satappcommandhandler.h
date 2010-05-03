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

#ifndef SATAPPCOMMANDHANDLER_H
#define SATAPPCOMMANDHANDLER_H

#include <QObject>
#include <msatuiobserver.h>

class SatAppUiProvider;

class SatAppCommandHandler : public QObject
    {
    Q_OBJECT    
public:
    SatAppCommandHandler(SatAppUiProvider &ui, QObject *parent = 0);
    virtual ~SatAppCommandHandler();
    
private slots:
 
    /**
    * Handles the SetUpMenu command.
    * @param aRes the out parameter. the result of the command.
    * @param aText The title text.
    * @param aMenuItems The array of menu item captions.
    * @param aMenuItemNextActions The array of menu item next action
    * indicator captions.
    * @param aIconBitmap The Context pane icon.
    * @param aItemsIconArray Array of item icons
    * @param aSelfExplanatoryItems Flag indicating if only icons are shown.
    * @param aHelpIsAvailable A flag indicating if SAT help is available.
    */
    void setUpMenu(
       TSatUiResponse &aRes,
       const QString &aText,
       const QStringList &aMenuItems,
       //const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
       //const HbIcon &aIcon,
       //const CAknIconArray* aItemIconsArray,
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
    void selectItem(
        TSatUiResponse &aRes,
        const QString &aText,
        const QStringList &aMenuItems,
        //const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
        const int aDefaultItem,
        unsigned char &aSelection,
        //const HbIcon &aIcon,
        //const CAknIconArray* aItemsIconArray,
        const bool aSelfExplanatoryItems,
        const bool aHelpIsAvailable);

    /**
    * Notification of the SAT Display Text command.
    * @param aRes, the User's response.
    * @param aText The text string to be displayed.
    * @param aSimApplicationName  Sim Application name
    * @param aIcon the icon to show.
    * @param aRequestedIconDisplayed Informs if icon is not used.
    * @param aSustainedText Indicates is this text sustained ie. no timers
    *        used if ETrue.
    * @param aDuration Duration for showing the text
    * @param aWaitUserToClear Indication if user is needed to clear
    *        message
    * @return The response of the UI to this command.
    */
    void displayText(
        TSatUiResponse &aRes,
        const QString &aText,
        const QString &aSimApplicationName,
//      const HbIcon &aIcon,
        bool &aRequestedIconDisplayed,
        const bool aSustainedText,
        const int aDuration,
        const bool aWaitUserToClear);
    
    /**
    * Notification of the SAT Get Input command.
    * @param aRes, the User's response.
    * @param aText The text string to be displayed.
    * @param aSimApplicationName  Sim Application name
    * @param aIcon the icon to show.
    * @param aRequestedIconDisplayed Informs if icon is not used.
    * @param aSustainedText Indicates is this text sustained ie. no timers
    *        used if ETrue.
    * @param aDuration Duration for showing the text
    * @param aWaitUserToClear Indication if user is needed to clear
    *        message
    * @return The response of the UI to this command.
    */
    void getInkey(
        TSatUiResponse &aRes,
        const QString &aTitleText,
        const TSatCharacterSet aCharacterSet,
        QString &aInputText,
        //const QPixmap* /*aIconBitmapGetInput*/,
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
    void getYesNo(
        TSatUiResponse &aRes,
        const QString &aText,
        const TSatCharacterSet aCharacterSet,
        unsigned int &aInkey,
        //const TSatIconId &aIconId,
        const bool &aSelfExplanatory,
        unsigned int &aDuration,
        const bool aImmediateDigitResponse);
    
    /**
    * Notification of the SAT Get Input command.
    * @param aRes, the User's response.
    * @param aText The text string to be displayed.
    * @param aSimApplicationName  Sim Application name
    * @param aIcon the icon to show.
    * @param aRequestedIconDisplayed Informs if icon is not used.
    * @param aSustainedText Indicates is this text sustained ie. no timers
    *        used if ETrue.
    * @param aDuration Duration for showing the text
    * @param aWaitUserToClear Indication if user is needed to clear
    *        message
    * @return The response of the UI to this command.
    */
    void getInput(
        TSatUiResponse &aRes,
        const QString &aTitleText,
        const TSatCharacterSet aCharacterSet,
        QString &aInputText,
        const int aMinLength,
        const int aMaxLength,
        const bool aHideInput,
        //const QPixmap* /*aIconBitmapGetInput*/,
        const bool aSelfExplanatory,
        unsigned int &aDuration);

    /**
    * Notification of the SAT Call Control command.
    * @param aText The text string to be displayed.
    * @param aAlphaIdStatus alpha ID's status
    */
    void callControl(
        const QString &aText,
        const TSatAlphaIdStatus aAlphaIdStatus);        

    /**
    * Notification of the SAT Mo Sms Control command.
    * @param aText The text string to be displayed.
    * @param aAlphaIdStatus alpha ID's status
    */
    void moSmControl(
        const QString &aText,
        const TSatAlphaIdStatus aAlphaIdStatus);
        
    /**
    * Shows the confirmation note about the SAT Send SMS, Send SS or
    * Send USSD command.
    * @param aText The text to be displayed.
    * @param aActionAccepted Indicates whether the command was accepted.
    * @param aType Type of the request (SMS, SS, USSD).
    * @return Information of the operation result.
    */
    void confirmSend(
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
    void showSmsWaitNote(
        const QString &aText,
        //const CFbsBitmap* aIconBitmapSendSM,
        const bool aSelfExplanatoryIcon
        );

    /**
    * Shows the wait note about the Send DTMF command.
    * @param aText The text to be displayed.
    * @param aIconBitmapSendDtmf Sending dialog icon.
    * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.
    */
    void showDtmfWaitNote(
        TSatUiResponse &aRes,
        const QString &aText
        );

    /*!
    Stop wait note
     */
    void stopShowWaitNote();
    
    /**
    * Shows the confirmation not about SetUpCall.
    * @param aText The text to be displayed.
    * @param aSimAppName The header to be displayed.
    * @param aActionAccepted Indicates whether the command was accepted.
    */
    void confirmSetUpCall(
        const QString &aText,
        const QString &aSimAppName,
        bool &aActionAccepted//,
        //const CFbsBitmap* aIconBitmap,
        //const TBool aSelfExplanatory
       );

    /**
    * Shows the wait note about the Send Ss or Ussd command.
    * @param aText The text to be displayed.
    * @param aIconBitmapSendSM Sending dialog icon.
    * @param aSelfExplanatoryIcon A flag indicating if only icon is shown.
    */
    void showSsWaitNote(
        const QString &aText,
        //const CFbsBitmap* aIconBitmapSendSM,
        const bool aSelfExplanatoryIcon
        );

    /**
    * Shows the wait note about BIP
    * @param aCommand command type.
    * @param aText The text to be displayed.
    */
    void showBIPWaitNote(int aCommand, const QString &aText);
    

private:
    /**
    * Not own.
    */
    SatAppUiProvider &mUi;

};

#endif /* SATAPPCOMMANDHANDLER_H */
