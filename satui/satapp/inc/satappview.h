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
*
*/

#ifndef SATAPPVIEW_H
#define SATAPPVIEW_H

#include <QEventLoop>
#include <hbview.h>
#include <msatuiobserver.h>

class QTimer;
class HbMainWindow;
class HbAction;
class HbListWidget;
class HbListWidgetItem;
class HbLabel;
class SatAppEventProvider;
class SatAppUiProvider;

class SatAppView : public HbView
    {
    Q_OBJECT

public:
     /**
     * Constructor
     */
    SatAppView(QGraphicsItem *parent = 0);

     /**
     * Destructor
     */
    ~SatAppView();

    void setMainWindow(HbMainWindow &mainWindow);

     /**
     * Initialize setup menu view
     * Handles the SetUpMenu command.
     * @param in eventProvider The reference of SatAppEventProvider.
     * @param in uiProvider The eference of SatAppUiProvider.
     */
    void initSetupMenu(SatAppEventProvider &eventProvider, 
                       SatAppUiProvider &uiProvider);

     /**
     * Initialize select item view
     * Handles the SelectItem command.
     * @param in utils The reference of SatAppEventProvider.
     * @param in manager The reference of SatAppUiProvider.
     */
    void initSelectItem(SatAppEventProvider &eventProvider, 
                        SatAppUiProvider &uiProvider);
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
   void showSetUpMenuContent(
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
    void showSelectItemContent(
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

public slots:
    /**
     * Private slot for select item event.
     */
    void selectItemTimeOut();
        /**
     * Private slot for active item.
     */
    void menuItemSelected(HbListWidgetItem *item);

private slots:
    /**
     * Private slot for receiving backSoftkey press event.
     */
    void backButtonClicked();

private: // From HbView
    /**
     * handle key press event
     */
    void keyPressEvent(QKeyEvent *event);

private: // New method

    /**
     * Connect item with menuItemSelected.
     */
    void connectItem();

    /**
     * Disonnect item with menuItemSelected.
     */
    void disconnectItem();

private: // data
    /**
     * Support select item time out.
     */
    bool mTimeout;

    /**
     * Indicatd Select item view and Setup Menu view.
     */
    bool mSelectItem;

     /**
     * Indicate use click back softkey.
     */
    bool mClickBackSoftkey;

    /**
     * The index of list widget.
     */
    int mItemIndex;

    /**
     * Own,an event loop for select item and menu selection
     */
    QEventLoop* mLoop;

    /**
     * Own,support selection item time out
     */
    QTimer *mTimer;

    // quit for setup menu view
    /**
     * Not own. From main
     */
    HbMainWindow *mWindow;

    /**
     * Own. An Action to quit sat application
     */
    HbAction *mSoftKeyQuitAction;

    /**
     * Own. An action to back to setup menu view
     */
    HbAction *mSoftKeyBackAction;

    /**
     * Not own.
     */
    SatAppEventProvider *mEvent;

    /**
     * Not own.
     */
    SatAppUiProvider *mUi;

    /**
     * Own,from application xml,don't delete in distructor
     */
    HbListWidget *mListWidget;

    /**
     * Own,from application xml,don't delete in distructor
     */
    HbListWidget *mSelectListWidget;

    /**
     * Own,from application xml,don't delete in distructor
     */
    HbLabel *mSubTitle;
    };

#endif // SATAPPVIEW_H
