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
* Description:  Implementation of the Mt_vmbxEngine class
*
*/


#ifndef _MT_VMBXENGINE_H
#define _MT_VMBXENGINE_H

#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QSignalSpy>
#include <QMetaType>
#include <QObject>


// Class forwards
class CVoiceMailboxEntry;
class CVoiceMailbox;
class HbMainWindow;


/**
 *  Mt_vmbxEngine module test class.
 * 
 *  Tests Mt_vmbxEngine interface.
 *  See cpp file for more information. 
 *
 *  @lib mt_vmbxngine.lib
 *  @since S60 <TB10.1>
 */
class Mt_vmbxEngine : public QObject
{
    Q_OBJECT

public:
    Mt_vmbxEngine();

    virtual ~Mt_vmbxEngine();

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
    
    // Called before each testfunction is executed. Currently not needed
    //void init();

     /**
     * Called after every testfunction.   Currently not needed
     * 
     */
    //void cleanup();

	/**
	* Test first case,Create testCreateVmbxEntry
	* 
	*/
	void testCreateVmbxEntry();
	
	 /**
	 * Test second case,Create CVoiceMailbox
	 * 
	 */
	void testCreateVmbxMailbox();
	
	/**
	* Test middle case,vmbx type
	* 
	*/
	void testVmbxType();
   
	/**
	* Test middle case,ServiceId
	* 
	*/
	void testServiceId();
 
	/**
	* Test middle case, AlsLine Type
	* 
	*/
	void testAlsLineType();
	
	/**
	* Test middle case, VmbxNumber
	* 
	*/
	void testVmbxNumber();
	
	/**
	* Test middle case, reset
	* 
	*/
	void testReset();

     /**
     * Test middle case,CheckConfiguration
     * 
     */
    void testCheckConfiguration();

    /**
    * Test middle case,CreateWindow
    * 
    * provide window for UI
    */
    void testCreateWindow();
 
    /**
    * Test middle case,regsit observer for vmbx number changed
    * 
    */
   void testNotifyVmbxNumberChange();

   /**
   * Test middle case,SaveProvisionedEntry
   * 
   */
   void testSaveProvisionedEntry();

     /**
     * Test middle case,query VmbxMailbox
     * 
     */
    void testQueryVmbxMailbox();
    
    /**
    * Test middle case,cancel observer for vmbx number changed
    * 
    */
   void testNotifyVmbxNumberChangeCancel();

   /**
   * Test middle case,delete HbMainWindow
   * 
   */  
   void testDeleteWindow();

     /**
     * Test middle case,delete CVoiceMailbox
     * 
     */
    void testDeleteVmbxMailbox();
    
    /**
    * Test last case,delete CVoiceMailboxEntry
    * 
    */
   void testDeleteVmbxEntry();

private:  //data

    /**
     * Own.
     */
	CVoiceMailboxEntry* mVmbxEntry;

    /**
     * Own.
     */
    CVoiceMailbox* mVmbxEngine;
 
    /**
     * Own.
     */
    HbMainWindow* mWindow;
};

#endif // _MT_VMBXENGINE_H
