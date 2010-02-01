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
* Description:  For packing VMBXEngine test component.
*
*/

#ifndef CVMBXENGINETEST_H
#define CVMBXENGINETEST_H

#define STIFTEST

#include <stiflogger.h>
#include <testscripterinternal.h>
#include <stiftestmodule.h>

class RVmbxNumber;

class CRepository;
class MCenRepNotifyHandlerCallback;
class CCenRepNotifyHandler;

// Logging path
_LIT( KLogPath, "\\logs\\testframework\\vmbxenginetest\\" );
// Log file
_LIT( KLogFile, "vmbxenginetest.txt" );


/**
*  Test class for VMBX Engine
*
*  @lib vmbxenginetest.dll
*  @since S60 5.1
*/
class CVmbxEngineTest : public CScriptBase, public MCenRepNotifyHandlerCallback
    {
public:

    /**
    * Two-phased constructor.
    */
    static CVmbxEngineTest* NewL( CTestModuleIf& aTestModuleIf );

    /**
    * Destructor.
    */
    virtual ~CVmbxEngineTest();

     /**
     * From CScriptBase Runs a script line.
     *
     * @since S60 5.1
     * @param aItem Script line containing method name and parameters
     * @return Symbian OS error code
     */
    TInt RunMethodL( CStifItemParser& aItem );

private:    // Methods for engine

    /**
    * C++ default constructor.
    */
    CVmbxEngineTest( CTestModuleIf& aTestModuleIf );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

    /**
    * Calls RVmbxNumber::Connect
    *
    * @since S60 5.1
    * @param aItem, Script line containing method name and parameters
    * @return TInt, KErrNone if OK, other values if a failure happens
    */
    TInt Connect( CStifItemParser& aItem );

    /**
    * Calls RVmbxNumber::Close
    *
    * @since S60 5.1
    * @param aItem, Script line containing method name and parameters
    * @return TInt, KErrNone if OK, other values if a failure happens
    */
    TInt Close( CStifItemParser& aItem );
    
    /**
    * Calls RVmbxNumber::AllowedToChangeTheNumber
    *
    * @since S60 5.1
    * @param aItem, Script line containing method name and parameters
    * @return TInt, KErrNone if OK, other values if a failure happens
    */
    TInt AllowedToChangeTheNumber( CStifItemParser& aItem );

    /**
    * Calls RVmbxNumber::SelectTypeL
    *
    * @since S60 5.1
    * @param aItem, Script line containing method name and parameters
    * @return TInt, KErrNone if OK, other values if a failure happens
    */
    TInt SelectTypeL( CStifItemParser& aItem );

    /**
    * Calls RVmbxNumber::IsSimMemory
    *
    * @since S60 5.1
    * @param aItem, Script line containing method name and parameters
    * @return TInt, KErrNone if OK, other values if a failure happens
    */
    TInt IsSimMemory( CStifItemParser& aItem );
    
    /**
    * Calls RVmbxNumber::QueryVideoMbxNumber
    *
    * @since S60 5.1
    * @param aItem, Script line containing method name and parameters
    * @return TInt, KErrNone if OK, other values if a failure happens
    */
    TInt QueryVideoMbxNumber( CStifItemParser& aItem );

    /**
     * Calls RVmbxNumber::QueryNumberL
     * @since S60 5.1
     * @param aItem, Script line containing method name and parameters
     * @return TInt, KErrNone if OK, other values if a failure happens
     */
    TInt QueryNumberL( CStifItemParser& aItem );
    
    /**
     * Calls RVmbxNumber::GetVideoMbxNumber
     * @since S60 5.1
     * @param aItem, Script line containing method name and parameters
     * @return TInt, KErrNone if OK, other values if a failure happens
     */
    TInt GetVideoMbxNumber( CStifItemParser& aItem );

private:
    /**
     * Calls ConfCenRepForEditNotAllowedL
     * @since S60 5.1
     * @param aItem, Script line containing method name and parameters
     * @return TInt, KErrNone if OK, other values if a failure happens
     */
    TInt ConfCenRepForEditNotAllowedL( CStifItemParser& aItem );

    /**
     * Calls RevertCenRepEditNotAllowedL
     * @since S60 5.1
     * @param aItem, Script line containing method name and parameters
     * @return TInt, KErrNone if OK, other values if a failure happens
     */
    TInt RevertCenRepEditNotAllowedL( CStifItemParser& aItem );
    
    /**
     * Calls ConfCenRepForEditAllowedL
     * @since S60 5.1
     * @param aItem, Script line containing method name and parameters
     * @return TInt, KErrNone if OK, other values if a failure happens
     */
    TInt ConfCenRepForEditAllowedL( CStifItemParser& aItem );
    
    /**
     * Calls InitGetNumFromSpecialMem
     * @since S60 5.1
     * @param aItem, Script line containing method name and parameters
     * @return TInt, KErrNone if OK, other values if a failure happens
     */
    TInt InitGetNumFromSpecialMem( CStifItemParser& aItem ); 
    
    /**
     * Calls TestGetNumFromSpecialMem
     * @since S60 5.1
     * @param aItem, Script line containing method name and parameters
     * @return TInt, KErrNone if OK, other values if a failure happens
     */
    TInt TestGetNumFromSpecialMem( CStifItemParser& aItem ); 
    
    /**
     * Calls CleanupGetNumFromSpecialMem
     * @since S60 5.1
     * @param aItem, Script line containing method name and parameters
     * @return TInt, KErrNone if OK, other values if a failure happens
     */
    TInt CleanupGetNumFromSpecialMem( CStifItemParser& aItem );
    
    /**
     * Save the source number temporarily
     * @since S60 5.1
     */
    void SaveSourceNumber();
    
    /**
     * Create central repository for session and handler
     * @since S60 5.1
     */
    void CreateCentralRepositoryL();

private:    // Data

    /**
    * Pointer to logger
    * Own.
    */
    CStifLogger * iLog;

    /**
    * Pointer to engine
    * Own.
    */
    RVmbxNumber *iVmbxNumber;  

    /**
     * Pointer to repository
     * Own.
     */
    CRepository* iSession;
    
    /**
     * Pointer to cenreqnotifyhandler
     * Own.
     */
    CCenRepNotifyHandler* iNotifyHandler;
    
    /**
     * Pointer to HBufC for mailbox number
     * Own.
     */
    HBufC* iNumberBuf;
    };

#endif      // CVMBXEngineTestTEST_H
