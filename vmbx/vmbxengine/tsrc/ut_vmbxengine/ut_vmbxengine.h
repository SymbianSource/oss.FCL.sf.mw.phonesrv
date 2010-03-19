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
* Description:  Implementation of the Ut_vmbxEngine class
*
*/

#ifndef _UT_VMBXENGINE_H
#define _UT_VMBXENGINE_H

#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QSignalSpy>
#include <QMetaType>
#include <QObject>


// Class forwards
class CVoiceMailbox;
class CVmbxUiHandler;
class VmbxQtUiHandler;
class HbMainWindow;


/**
 *  Ut_vmbxEngine unit test class.
 * 
 *  Tests vmbxEngine cpp files public APIS.
 *  See cpp file for more information. 
 *
 *  @lib ut_vmbxengine.lib
 *  @since S60 <TB10.1>
 */
class Ut_vmbxEngine : public QObject
{
    Q_OBJECT

public:
    Ut_vmbxEngine();
    virtual ~Ut_vmbxEngine();

    //void testResult(const bool aResult);

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
    * Test middle case,CreateWindow
    * 
    * provide window for UI
    */
    void testCreateWindow();

    /**
    * Test middle case,showVmbxQueryDialog
    * 
    */   
    void testshowVmbxQueryDialog();

     /**
     * Test middle case,showDefineSelectionDialog
     * 
     */
    void testshowDefineSelectionDialog();

     /**
     * Test first case,Create CVoiceMailbox
     * 
     */
    void testCreateVmbxMailbox();

     /**
     * Test middle case,showSaveEmptyNote
     * 
     */
    void testshowSaveEmptyNote();

    /**
    * Test middle case,delete HbMainWindow
    * 
    */  
    void testDeleteWindow();
      /**
     * Test last case,delete CVoiceMailbox
     * 
     */
    void testDeleteVmbxMailbox();

private:  //data

    /**
     * Own.
     */
    CVoiceMailbox* mVmbxEngine;

    /**
     * Own.
     */
    CVmbxUiHandler *mUiHandler;
  
    /**
     * Own.
     */
    VmbxQtUiHandler *mQtUiHandler;
    
    /**
     * Own.
     */
    HbMainWindow* mWindow;
};

#endif // _UT_VMBXENGINE_H
