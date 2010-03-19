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
#ifndef SATAPPUIPROVIDER_H
#define SATAPPUIPROVIDER_H

#include <QObject>
#include <hbdocumentloader.h>
#include <hbdialog.h>
#include <QEventLoop>
#include <msatuiobserver.h>
#include "satappcommonconstant.h"

class HbMainWindow;
class SatAppEventProvider;  // Call back from SatServer
class SatAppView;           // SetupMenu and SelectItem
class HbMessageBox;         // DisplayText
class SatAppGetInkeyNote;   // GetInKey
class HbInputDialog;        // GetInKey
class HbDialog;             // GetInput
class HbProgressNote;       // For wait note
class QTimer;               // For SMS/DTMF

// Here we create custom document loader to be able to use own classes in XML.
class SatAppDocumentLoader : public HbDocumentLoader
{
public:
    virtual QObject *createObject(const QString &type, const QString &name);
};

class SatAppUiProvider : public QObject
{
    Q_OBJECT

public:
    /*
     * Constructor
     * @param window reference of HbMainWindow
     * @param event reference of SatAppEventProvider
     * @param parent default value 
     */
    SatAppUiProvider(HbMainWindow &window,
                     SatAppEventProvider &event,
                     QObject *parent = 0); 

    /*
     * Destructor
     */
    ~SatAppUiProvider();

    /*
     * return the document loader, not transfer the ownership.
     */
    SatAppDocumentLoader* docmlLoader();

    /*
     * Set active view.
     */
    void activeView(const QString &view);

    /*
     * Show SetUpMenuView
     * @param aRes TSatAppUserResponse the response
     * @param aText The string need to be shown in heading widget
     * @param aMenuItems The string need to be shown in list widget
     * @param aSelfExplanatoryItems
     * @param aHelpIsAvailable
     */
    void showSetUpMenuView(
            TSatUiResponse &aRes,
            const QString &aText,
            const QStringList &aMenuItems,
            //const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            //const HbIcon &aIcon,
            //const CAknIconArray* aItemIconsArray,
            const bool aSelfExplanatoryItems,
            const bool aHelpIsAvailable); 

    /*
     * Show SelectItemView
     * @param aRes TSatAppUserResponse the response
     * @param aText The string need to be shown in heading widget
     * @param aMenuItems The string need to be shown in list widget
     * @param aDefaultItem the default item
     * param aSelection The string need to be shown in list widget
     * @param aSelfExplanatoryItems
     * @param aHelpIsAvailable
     */
    void showSelectItemView(
            TSatUiResponse &aRes,
            const QString &aText,
            const QStringList &aMenuItems,
            //const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const int aDefaultItem,
            unsigned char&aSelection,
            //const HbIcon &aIcon,
            //const CAknIconArray* aItemsIconArray,
            const bool aSelfExplanatoryItems,
            const bool aHelpIsAvailable);

    /*
     * Show the Display dialog
     * @param aText The string need to be shown in content widget
     * @param aHeader The string need to be shown in heading widget
     * @param aDuration the duration of dialog shown if user do nothing
     * @return TSatAppUserResponse the response
     */
    TSatAppUserResponse showDisplayTextPopup(
            const QString &aHeading,
            const QString &aContent,
            const int aDuration); 

    /*
     * Show GetInkeyQuer
     * @param aHeader The string need to be shown in heading widget
     * @param aContent The string need to be shown in content widget
     * @param characterSet input mode is digtal or char
     * @param aDuration the duration of dialog shown if user do nothing
     * @return TSatAppUserResponse the response
     */
    TSatAppUserResponse showGetInkeyQuery(
            const QString &aHeading,
            QString &aContent,
            const TSatCharacterSet aCharacterSet,
            unsigned int &aDuration);

    /*
     * Show GetYesNoQuery
     * @param aText The string shown in content
     * @param characterSet input mode is digtal or char
     * @param aDuration the duration of dialog shown if user do nothing
     * @param aImmediateDigitResponse
     * @return int the response
     */
    int showGetYesNoQuery(
            const QString &aText,
            const TSatCharacterSet aCharacterSet,
            unsigned int &aDuration,
            const bool aImmediateDigitResponse);

    /*
     * Show GetInput Query
     * @param heading The string shown in heading widget
     * @param content The string what user enters. May contain default text
     * @param characterSet input mode is digtal or char
     * @param minLength min lenght for user input
     * @param maxLength max lenght for user input
     * @param aHideInput user input should hide or not
     */
    TSatAppUserResponse showGetInputQuery(
            const QString &heading,
            QString &content,
            const TSatCharacterSet characterSet,
            const int minLength,
            const int maxLength,
            const bool aHideInput);

    /*
     * show confirm send query
     * @param aText The string shown in heading widget
     * @param aActionAccepted 
     */
    void showConfirmSendQuery(
            const QString &aText,
            bool &aActionAccepted);

    /*
     * Show Sms wait note
     * @param aText The string shown in heading widget
     */
    void showSmsWaitNote(const QString &aText);

    /*
     * Show Dtmf wait note
     * @param aRes 
     * @param aText The string shown in heading widget
     */
    void showDtmfWaitNote(TSatUiResponse &aRes,
            const QString &aText);

    /*
     * Stop show wait note
     */
    void stopShowWaitNote();

    /*
     * Comfirm setupcall
     * @param aText 
     * @param aSimAppName 
     * @param aActionAccepted 
     */
    void showConfirmSetUpCallQuery(
            const QString &aText,
            const QString &aSimAppName,
            bool &aActionAccepted);
signals:
    /*
     * User cancel Dtmf response
     */
    void userCancelDtmfResponse();

public slots:
    /*
     * Clear Screen
     */
    void clearScreen();

    /*
     * Close Ui
     */
    void closeUi();

    /*
     * Digital response
     */
    void digitalResponse(const int aKey);

    /*
     * The response of user selected the Primary action
     */
    void userPrimaryResponse();

    /*
     * The response of user selected the secondary action
     */
    void userSecondaryResponse();

    /*
     * Update Query Action
     * @param text 
     */
    void updateQueryAction(QString text);

    /*
     * User cancel Dtmf response
     */
    void cancelDtmfResponse();

private:
    /*
     * Reset the data member mUserRsp value
     */
    void resetUserResponse();

    /*
     * Compose Dialog
     * @param type 
     * @param aModal 
     * @param aDismissPolicy 
     */
    void composeDialog(HbDialog *dlg, int aDuration,
            TSatAppDialogActionType type, bool aModal = true,
            HbDialog::DismissPolicy aDismissPolicy = HbDialog::NoDismiss);

    /*
     * Extend the note shown time
     */
    void extendNoteShowtime();

private:
    /*
     *  Not own
     */
    HbMainWindow &mMainWindow;

    /*
     *  Own,objects load from XML. Needs to be deleted once application exits
     */
    QList<QObject *> mObjects;

    /*
     *  Own. For application xml
     */
    SatAppDocumentLoader *mLoader;

    /*
     *  Own. from application xml
     */
    SatAppView *mSetupMenuView;

    /*
     *  Own. from application xml
     */
    SatAppView *mSelectItemView;

    /*
     *  Own. DisplayPopup
     */
    HbMessageBox *mDisplayPopup;

    /*
     *  Own. GetInkeyQuery
     */
    HbInputDialog *mGetInkeyQuery;

    /*
     *  Own. YesNoPopup
     */
    SatAppGetInkeyNote *mYesNoPopup;

    /*
     *  Own. GetInputQuery
     */
    HbInputDialog *mGetInputQuery;

    /*
     *  Own. Confirm SendQuery
     */
    HbMessageBox *mConfirmSendQuery;

    /*
     *  Own. SmsWaitNote
     */
    HbProgressNote *mSmsWaitNote;
    /*
     *  Own. DtmfWaitNote
     */
    HbProgressNote *mDtmfWaitNote;

    /*
     *  Own. SetUpCall Query
     */
    HbMessageBox *mSetUpCallQuery;

    /*
     *  General user response
     */
    TSatAppUserResponse mUserRsp;

    /*
     *  GetInkey immediate digital response
     */
    int mDigitalRsp;

    /*
     *  GetInput min legnth, en/dis able ok buttion
     */
    int mMinLength;

    /*
     *  Own. DTMF
     */    
    QTimer *mTimer;

    /*
     *  Own. 
     */   
    QEventLoop *mLoop;
};

#endif// SATAPPUIPROVIDER_H

// End of file
