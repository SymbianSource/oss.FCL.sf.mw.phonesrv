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

#ifndef UT_SATAPPEVENTPROVIDER_H
#define UT_SATAPPEVENTPROVIDER_H

#include <QtTest/QtTest>
#include <QObject>
#include <hbview.h>

// Class forwards
class HbMainWindow;
class HbView;
class SatAppEventProvider;

/**
 *  Satapp unit test class.
 * 
 *  @since S60 <TB10.1>
 */
class Ut_SatAppEventProvider : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppEventProvider(QObject *parent = 0);


    virtual ~Ut_SatAppEventProvider();

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
     * Test SatAppEventProvider's constructor.
     * 
     */
    void testCreateEventHandler();

    /**
     * Test SatAppEventProvider's testShowSsWaitNote param list.
     * 
     */
    void testShowSsWaitNote_data();

     /**
     * Test SatAppEventProvider's showSsWaitNote.
     * 
     */
    void testShowSsWaitNote();

     /**
     * Test SatAppEventProvider's StopShowWaitNote.
     * 
     */
    void testStopShowWaitNote();

     /**
     * Test SatAppEventProvider's ShowWaitNoteWithoutDelayL.
     * 
     */
    void testShowWaitNoteWithoutDelay();

     /**
     * Test SatAppEventProvider's ShowSsErrorNoteL.
     * 
     */
    void testShowSsErrorNote();

private:
    /**
     * Own.
     */
    SatAppEventProvider *mEventHandler;

    /**
     * Own.
     */
    HbMainWindow *mWindow;


};

#endif // UT_SATAPPEVENTPROVIDER_H
