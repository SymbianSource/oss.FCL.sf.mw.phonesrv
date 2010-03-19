/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the Ut_vmbxCpPlugin class
*
*/


#ifndef _UT_VMBXCPPLUGIN_H
#define _UT_VMBXCPPLUGIN_H

#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QSignalSpy>
#include <QMetaType>
#include <QObject>


// Class forwards
class VmbxCpGroup;
class VmbxUiEngine;
class HbMainWindow;


/**
 *  Mt_vmbxEngine module test class.
 * 
 *  Tests Ut_vmbxCpPlugin interface.
 *  See cpp file for more information. 
 *
 *  @lib Ut_vmbxCpPlugin.lib
 *  @since S60 <TB10.1>
 */
class Ut_vmbxCpPlugin : public QObject
{
    Q_OBJECT

public:
    Ut_vmbxCpPlugin();

    virtual ~Ut_vmbxCpPlugin();

private slots: 

     /**
     * Called before the first testfunction is executed.
     * 
     */
    void initTestCase();

     /**
     * Called after the last testfunction has been executed.
     * 
     */
    void cleanupTestCase();

    /**
    * Test first case,GetCsVoice1Number
    * 
    */
    void testGetCsVoice1Number();
    
     /**
     * Test second case, GetCsVoice2Number
     * 
     */
    void testGetCsVoice2Number();
    
    /**
    * Test middle case,GetCsVideo1Number
    * 
    */
    void testGetCsVideo1Number();
     
    /**
    * Test middle case,DefaultMailboxType
    * 
    */
    void testDefaultMailboxType();

private:  //data

    /**
     * Own.
     */
    VmbxCpGroup* mCpGroup;

    /**
     * Own.
     */
    VmbxUiEngine* mUiEngine;
 
    /**
     * Own.
     */
    HbMainWindow* mWindow;
};

#endif // _UT_VMBXCPPLUGIN_H
