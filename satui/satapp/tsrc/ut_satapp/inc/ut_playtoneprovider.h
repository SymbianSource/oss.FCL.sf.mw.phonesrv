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

#ifndef UT_SATAPPPLAYTONEPROVIDER_H
#define UT_SATAPPPLAYTONEPROVIDER_H

#include <QtTest/QtTest>
#include <QObject>


// Class forwards
class SatAppPlayToneProvider;

/**
 *  Satapp unit test class.
 * 
 *  @since S60 <TB10.1>
 */
class Ut_SatAppPlayToneProvider : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppPlayToneProvider(QObject *parent = 0);

    virtual ~Ut_SatAppPlayToneProvider();

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
     * Test SatAppPlayToneProvider's constructor.
     * 
     */
    void testCreatePlayTone();

    /**
   * Test SatAppPlayToneProvider's testPlayStandardTone input param list.
   * 
   */
    void testPlayStandardTone_data();

    /**
   * Test SatAppPlayToneProvider's PlayStandardToneL.
   * 
   */
    void testPlayStandardTone();

    /**
   * Test SatAppPlayToneProvider's testPlayUserSelectedTone input param list.
   * 
   */
    void testPlayUserSelectedTone_data();

    /**
   * Test SatAppPlayToneProvider's PlayUserSelectedToneL.
   * 
   */
    void testPlayUserSelectedTone();

    /**
   * Test SatAppPlayToneProvider's closeSatUI.
   * 
   */
    void testCloseSatUI();

    /**
   * Test SatAppPlayToneProvider's clearScreen.
   * 
   */
    void testClearScreen();

    /**
   * Test SatAppPlayToneProvider's testMapcPlayComplete input param list.
   * 
   */
    void testMapcPlayComplete_data();

    /**
   * Test SatAppPlayToneProvider's MapcPlayComplete.
   * 
   */
    void testMapcPlayComplete();

    /**
   * Test SatAppPlayToneProvider's testMapcInitComplete input param list.
   * 
   */
    void testMapcInitComplete_data();

    
    /**
   * Test SatAppPlayToneProvider's MapcInitComplete.
   * 
   */
    void testMapcInitComplete();

private:
     /**
     * Own.
     */
    SatAppPlayToneProvider *mPlayTone;

};

#endif // UT_SATAPPPLAYTONEPROVIDER_H
