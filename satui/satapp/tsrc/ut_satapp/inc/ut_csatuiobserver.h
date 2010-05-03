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
* Description: Unit test CSatUiObserver class
*
*/

#ifndef UT_CSATUIOBSERVER_H
#define UT_CSATUIOBSERVER_H

#include <QtTest/QtTest>
#include <QObject>

// Class forwards
class CSatUiObserver;
class DummyEventProvider;

/**
 *  CSatUiObserver unit test class.
 * 
 *  @since S60 <TB10.1>
 */
class Ut_CSatUiObserver : public QObject
{
    Q_OBJECT

public:
     /**
     * Constructor.
     */
    Ut_CSatUiObserver(QObject *parent = 0);
     /**
     * Destructor.
     */
    virtual ~Ut_CSatUiObserver();

private slots:

     /**
     * Called before the first testfunction is executed.
     * Handles the SetUpMenu command.
     */
    void initTestCase();

     /**
     * Called after the last testfunction has been executed.
     * 
     */
    void cleanupTestCase();

     /**
     * Test CSatUiObserver's ConnectRSatSessionL.
     * 
     */
    void testConnectRSatSession();

     /**
     * Test CSatUiObserver's Adapter.
     * 
     */
    void testAdapter();

     /**
     * Test CSatUiObserver's DisconnectRSatSession.
     * 
     */
    void testDisconnectRSatSession();

     /**
     * Test CSatUiObserver's ProfileState.
     * 
     */
    void testProfileState();

    /**
     * Test CSatUiObserver's SetImplementer.
     * 
     */
    void testSetImplementer();

    /**
     * Test CSatUiObserver's testDisplayText's input param list.
     * 
     */
    void testDisplayText_data();

     /**
     * Test CSatUiObserver's DisplayTextL.
     * 
     */
    void testDisplayText();

    /**
     * Test CSatUiObserver's testSetUpMenu's input param list.
     * 
     */
    void testSetUpMenu_data();

     /**
     * Test CSatUiObserver's SetUpMenuL.
     * 
     */
    void testSetUpMenu();

     /**
     * Test CSatUiObserver's testGetInkey's input param list.
     * 
     */
    void testGetInkey_data();

     /**
     * Test CSatUiObserver's GetInkeyL.
     * 
     */
    void testGetInkey();

     /**
     * Test CSatUiObserver's testGetInput's input param list.
     * 
     */
    void testGetInput_data();

     /**
     * Test CSatUiObserver's GetInputL.
     * 
     */
    void testGetInput();

     /**
     * Test CSatUiObserver's testConfirmCommand's input param list.
     * 
     */
    void testConfirmCommand_data();

     /**
     * Test CSatUiObserver's ConfirmCommand.
     * 
     */
    void testConfirmCommand();
    
    
    /**
    * Test CSatUiObserver's Notification input param list.
    * 
    */
   void testNotification_data();

    /**
    * Test CSatUiObserver's Notification.
    * 
    */
   void testNotification();


   /**
   * Test CSatUiObserver's EventNotification input param list.
   * 
   */
    void testEventNotification_data();

   /**
   * Test CSatUiObserver's Notification.
   * 
   */
    void testEventNotification();

  /**
   * Test CSatUiObserver's testSetUpMenu's input param list.
   * 
   */
    void testSelectItem_data();

   /**
   * Test CSatUiObserver's SetUpMenuL.
   * 
   */
    void testSelectItem();

  /**
   * Test CSatUiObserver's testPlayTone input param list.
   * 
   */
    void testPlayTone_data();

   /**
   * Test CSatUiObserver's testPlayTone.
   * 
   */
    void testPlayTone();

private:

     /**
     * Own.
     * 
     */
    CSatUiObserver *mSatObserver;

    /**
     * Own.
     * 
     */
    DummyEventProvider *mImplementer;

};

#endif // UT_CSATUIOBSERVER_H
