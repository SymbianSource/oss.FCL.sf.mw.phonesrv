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
* Description: This module contains the implementation of CNWNotifier 
*              class member functions.
*
*/

// SYSTEM INCLUDE FILES

// INCLUDE FILES
#include "nwregistrationstatusmonitor.h" 

//  Global Variables

// CONSTANTS
_LIT( KNWNotifierThreadName,"NWNotifierThread" ); 

//  Local Functions

// -----------------------------------------------------------------------------
// DoStartL()
// 
// -----------------------------------------------------------------------------
//
LOCAL_C void DoStartL()
    {
    // Rename thread as 
    User::RenameThread( KNWNotifierThreadName );  
    
    // Create active scheduler (to run active objects)
    CActiveScheduler* threadScheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL( threadScheduler );
    CActiveScheduler::Install( threadScheduler );

    // Create an instance of status monitoring component. Start monitoring instantly.
    CNWRegistrationStatusMonitor* statusMonitor = CNWRegistrationStatusMonitor::NewL();
    
    //Starts the scheduler. Thread remains in this loop until this process is terminated.
    CActiveScheduler::Start();
    
    delete statusMonitor;
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy( threadScheduler );
    }

//  Global Functions

// -----------------------------------------------------------------------------
// E32Main()
// Provides the API for the operating system to start the executable.
// Returns the address of the function to be called.
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    // Create cleanup stack
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    
    if( !cleanup )
        {
        return KErrNoMemory;
        }
    
    // Run application code inside TRAP harness
    TRAPD( mainErr, DoStartL() );
        
    delete cleanup;
    __UHEAP_MARKEND;
    
    return mainErr;
    }

// End of File
