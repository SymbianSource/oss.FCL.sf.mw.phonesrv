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


// INCLUDE FILES
#include <stiftestinterface.h>
#include <stifparser.h>
#include <e32math.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>

#include "vmnumber.h"
#include "vmbxenginetest.h"

// ======== MEMBER FUNCTIONS ========

const TUid KUidVoiceMailbox = {0x101F874F};
const TUint32 KVmUIKeyOpts = 0x00000010;


const TInt KVmUIKeyOptsForSimOnly = 1;
const TInt KVmDefaultUIKeyOpts = 0;

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxEngineTest::CVmbxEngineTest( CTestModuleIf& aTestModuleIf ) :
    CScriptBase( aTestModuleIf ), iSession( NULL ), iNotifyHandler( NULL )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CVmbxEngineTest::ConstructL()
    {
    // Constructing static buffer size logger, needed with OOM testing because
    // normally logger allocates memory from heap!
    iLog = CStifLogger::NewL( KLogPath,
                              KLogFile,
                              CStifLogger::ETxt,
                              CStifLogger::EFile,
                              EFalse,
                              ETrue,
                              ETrue,
                              EFalse,
                              ETrue,
                              EFalse,
                              100 );
    iLog->Log( _L( "CVmbxEngineTest logging starts!" ) );
    }

// ---------------------------------------------------------------------------
// NewL creates a new test module
// ---------------------------------------------------------------------------
//
CVmbxEngineTest* CVmbxEngineTest::NewL( CTestModuleIf& aTestModuleIf )
    {
    CVmbxEngineTest* self = new ( ELeave ) CVmbxEngineTest( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CVmbxEngineTest::~CVmbxEngineTest()
    {
    if ( iLog )
        {
        iLog->Log( _L( "###### TEST MODULE CLOSED #######" ) );
        delete iLog;
        }
    
    delete iVmbxNumber;
    
    if ( iNotifyHandler )
        {
        iNotifyHandler->StopListening();
        delete iNotifyHandler;
        }

    delete iSession;
    delete iNumberBuf;    
    }

// ---------------------------------------------------------------------------
// From class CScriptBase.
// Runs a script line
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::RunMethodL( CStifItemParser& aItem )
    {
    static TStifFunctionInfo const KFunctions[] =
        {
        ENTRY( "Connect", CVmbxEngineTest::Connect ),
        ENTRY( "Close", CVmbxEngineTest::Close ),
        // test RVmbxNumber APIs
        ENTRY( "AllowedToChangeTheNumber", 
                CVmbxEngineTest::AllowedToChangeTheNumber ),
        ENTRY( "SelectTypeL", CVmbxEngineTest::SelectTypeL ),
        ENTRY( "IsSimMemory", CVmbxEngineTest::IsSimMemory ),
        ENTRY( "QueryVideoMbxNumber", CVmbxEngineTest::QueryVideoMbxNumber ),
        ENTRY( "QueryNumberL", CVmbxEngineTest::QueryNumberL ),
        ENTRY( "GetVideoMbxNumber", CVmbxEngineTest::GetVideoMbxNumber ),
        ENTRY( "ConfCenRepForEditNotAllowedL", 
        		CVmbxEngineTest::ConfCenRepForEditNotAllowedL ),
        ENTRY( "RevertCenRepEditNotAllowedL", 
        		CVmbxEngineTest::RevertCenRepEditNotAllowedL ),
        ENTRY( "ConfCenRepForEditAllowedL", 
                CVmbxEngineTest::ConfCenRepForEditAllowedL ),
        ENTRY( "InitGetNumFromSpecialMem", 
                CVmbxEngineTest::InitGetNumFromSpecialMem ),
        ENTRY( "TestGetNumFromSpecialMem", 
                CVmbxEngineTest::TestGetNumFromSpecialMem ),
        ENTRY( "CleanupGetNumFromSpecialMem", 
                CVmbxEngineTest::CleanupGetNumFromSpecialMem )
        };

    const TInt count = sizeof( KFunctions ) /
        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }

// ---------------------------------------------------------------------------
// Calls CVmbxEngineTest::Connect method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::Connect( CStifItemParser& /*aItem*/ )
    {
    TInt error = KErrAlreadyExists;
    if ( !iVmbxNumber )
        {
        iVmbxNumber = new RVmbxNumber();
        if ( iVmbxNumber )
            {
            iLog->Log( _L( "Opening.." ) );
            error = iVmbxNumber->Open();
            if ( KErrNone == error )
            	{
            	// save the source number temporarily 
            	// so that it will be used while some cases are reverted.
            	SaveSourceNumber();
            	}
            }
        }
    iLog->Log( _L( "Connecting done, %d" ), error );
    return error;
    }

// ---------------------------------------------------------------------------
// Calls CVmbxEngineTest::Close method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::Close( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L( "Closing.." ) );

    if ( iVmbxNumber )
        {
        iVmbxNumber->Close();
        delete iVmbxNumber;
        iVmbxNumber = NULL;
        
        if ( iNumberBuf )
            {
            delete iNumberBuf;
            iNumberBuf = NULL;
            }  
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVmbxEngineTest::AllowedToChangeTheNumber method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::AllowedToChangeTheNumber( CStifItemParser& /*aItem*/ )
    {
    TInt result( KErrNone );
    result = iVmbxNumber->AllowedToChangeTheNumber();

    iLog->Log( _L( "AllowedToChangeTheNumber return %d" ), result);
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxEngineTest::SelectTypeL method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::SelectTypeL( CStifItemParser& /*aItem*/ )
    {
    TInt result( KErrNone );
    TRAP( result, iVmbxNumber->SelectTypeL( EVmbxDefine ) );

    iLog->Log( _L( "SelectTypeL return result %d" ), result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxEngineTest::IsSimMemory method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::IsSimMemory( CStifItemParser& /*aItem*/ )
    {
    // Use allownextresult keyword in config file to control what is expected
    // return value in your test case.
    TInt result( KErrNone );
    result = iVmbxNumber->IsSimMemory();
    iLog->Log( _L( "IsSimMemory return %d" ), result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxEngineTest::QueryVideoMbxNumber method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::QueryVideoMbxNumber( CStifItemParser& aItem )
    {
    // Use allownextresult keyword in config file to control what is expected
    // return value in your test case.
    TInt result( KErrNone );
    TBuf< KVmbxMaxNumberLength > number;

    TInt queryType( KErrNone );
    aItem.GetNextInt( queryType );

    TRAP( result, iVmbxNumber->QueryVideoMbxNumberL( 
    ( TVmbxQueryType )queryType, number ) );

    iLog->Log( _L( "QueryVideoMbxNumber return %d" ), result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxEngineTest::QueryNumberL method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::QueryNumberL( CStifItemParser& aItem )
    {
    // Use allownextresult keyword in config file to control what is expected
    // return value in your test case.
    TInt result( KErrNone );
    TBuf< KVmbxMaxNumberLength > number;

    TInt queryType( KErrNone );
    aItem.GetNextInt( queryType );

    TRAP( result, iVmbxNumber->QueryNumberL( ( TVmbxQueryType )queryType, 
    number, EFalse ) );
        
    iLog->Log( _L( "QueryNumberL return %d" ), result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxEngineTest::GetVideoMbxNumber method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::GetVideoMbxNumber( CStifItemParser& /*aItem*/ )
    {
    // Use allownextresult keyword in config file to control what is expected
    // return value in your test case.
    TInt result( KErrNone );
    TBuf< KVmbxMaxNumberLength > number;

    result = iVmbxNumber->GetVideoMbxNumber( number );

    iLog->Log( _L( "GetVideoMbxNumber return %d" ), result );
    return result;
    }

// ---------------------------------------------------------------------------
// Calls CVmbxEngineTest::ConfCenRepForEditNotAllowedL method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::ConfCenRepForEditNotAllowedL( CStifItemParser& /*aItem*/ )
    {
    TInt ret( KErrNone );
    // create central repository for session and handler
    CreateCentralRepositoryL();
    
    // configurate the key for the special condition like "sim only"
    ret = iSession->Set( KVmUIKeyOpts, KVmUIKeyOptsForSimOnly );

    iLog->Log( _L( "ConfCenRepForEditNotAllowedL %d" ), ret );
    return ret;
    }

// ---------------------------------------------------------------------------
// Calls CVmbxEngineTest::RevertCenRepEditNotAllowedL method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::RevertCenRepEditNotAllowedL( CStifItemParser& /*aItem*/ )
    {
    TInt ret( KErrNone );
    // reset the key to default value
    ret = iSession->Set( KVmUIKeyOpts, KVmDefaultUIKeyOpts );
    iLog->Log( _L( "RevertCenRepEditNotAllowedL %d" ), ret );
    return ret;
    }

// ---------------------------------------------------------------------------
// Calls CVmbxEngineTest::ConfCenRepForEditAllowedL method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::ConfCenRepForEditAllowedL( CStifItemParser& /*aItem*/ )
    {
    TInt ret( KErrNone );
    // create central repository for session and handler
    CreateCentralRepositoryL();
    
    // configurate the key for the special condition like "sim only"
    ret = iSession->Set( KVmUIKeyOpts, KVmDefaultUIKeyOpts );

    iLog->Log( _L( "ConfCenRepForEditAllowedL %d" ), ret );
    return ret;
    }

// ---------------------------------------------------------------------------
// Calls CVmbxEngineTest::InitGetNumFromSpecialMem method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::InitGetNumFromSpecialMem( CStifItemParser& aItem )
	{
	TInt ret( KErrNone );
	
	// Get number from Script file
	TPtrC numberFromScript;
	aItem.GetNextString( numberFromScript );
	iLog->Log( _L( "InitGetNumFromSpecialMem saving number %S" ), 
			&numberFromScript );
	
	// the number need to be saved to the sim/phone memory 
	// to init the special case
	ret = iVmbxNumber->SaveVmbxNumber( numberFromScript, 
			EAlsActiveLineEntry );
		
	iLog->Log( _L( "InitGetNumFromSpecialMem return %d" ), ret );
	return ret;
	}

// ---------------------------------------------------------------------------
// Calls CVmbxEngineTest::TestGetNumFromSpecialMem method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::TestGetNumFromSpecialMem( CStifItemParser& aItem )
	{
	TInt ret( KErrNotFound );
    TBuf< KVmbxMaxNumberLength > number;
    // Use allownextresult keyword in config file to control what is expected
    // return value in your test case.
	
	TPtrC numberFromScript;
	aItem.GetNextString( numberFromScript );
	iLog->Log( _L( "TestGetNumFromSpecialMem saving number %S" ), 
			&numberFromScript );
    
	ret = iVmbxNumber->GetVmbxNumber( number );
	if ( KErrNone == ret && KErrNone == number.Compare( numberFromScript ) )
		{
		ret = KErrNone;
		}
	
	iLog->Log( _L( "TestGetNumFromSpecialMem number %S" ), &number );
	iLog->Log( _L( "TestGetNumFromSpecialMem return %d" ), ret );
	return ret;
	}

// ---------------------------------------------------------------------------
// Calls CVmbxEngineTest::CleanupGetNumFromSpecialMem method
// ---------------------------------------------------------------------------
//
TInt CVmbxEngineTest::CleanupGetNumFromSpecialMem( CStifItemParser& /*aItem*/ )
	{
	TInt ret( KErrNone );
	// revert the source number to sim/phone memory
	ret = iVmbxNumber->SaveVmbxNumber( *iNumberBuf, 
			EAlsActiveLineEntry );
	
	iLog->Log( _L( "CleanupGetNumFromSpecialMem return %d" ), ret );
	return ret;
	}

// ---------------------------------------------------------------------------
// Save the source number temporarily
// ---------------------------------------------------------------------------
//
void CVmbxEngineTest::SaveSourceNumber()
	{
	iNumberBuf = HBufC::New( KVmbxMaxNumberLength );
	if ( iNumberBuf )
		{
		TPtr ptr( iNumberBuf->Des() );
		iVmbxNumber->GetVmbxNumber( ptr );
		}
	}

// ---------------------------------------------------------------------------
// Create central repository for session and handler
// ---------------------------------------------------------------------------
//
void CVmbxEngineTest::CreateCentralRepositoryL()
	{
	if ( !iSession )
		{
		// create central repository
		iSession = CRepository::NewL( KUidVoiceMailbox );
		}
	
	if ( !iNotifyHandler )
		{
		// set notify handler, when the key had been changed
		// in the whole repository
		iNotifyHandler = CCenRepNotifyHandler::NewL( *this, *iSession );
		if ( iNotifyHandler )
			{
			iNotifyHandler->StartListeningL();
			}		
		}	
	}

// ======== GLOBAL FUNCTIONS ========
// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CScriptBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    return CVmbxEngineTest::NewL( aTestModuleIf );
    }
