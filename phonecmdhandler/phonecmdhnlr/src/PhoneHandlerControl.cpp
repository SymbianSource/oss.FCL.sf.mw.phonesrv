/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Receives call handling related key presses from accessories and 
*				 executes them. 
*
*/



// INCLUDE FILES
#include "PhoneHandlerControl.h"
#include "PhoneHandlerService.h"
#include "PhoneHandlerAnswerCall.h"
#include "PhoneHandlerEndCall.h"
#include "PhoneHandlerDialCall.h"
#include "PhoneHandlerVoiceDial.h"
#include "PhoneHandlerRedial.h"
#include "PhoneHandlerMultipartyCall.h"
#include "PhoneHandlerDTMF.h"
#include "PhoneHandlerActive.h"
#include "PhoneHandlerResponse.h"
#include "PhoneHandlerCallState.h"
#include "PhoneHandlerDebug.h"
#include <remconinterfaceselector.h>
#include <RemConCallHandlingTarget.h>
#include <CPbkContactEngine.h>
#include <ctsydomainpskeys.h>
#include <connect/sbdefs.h>
#include <coreapplicationuisdomainpskeys.h>
#include "PhoneHandlerCallArray.h"
#include <callinformation.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES
const TInt KMultipartyCallMinParam = 1;
const TInt KMultipartyCallMaxParam = 2;

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::CPhoneHandlerControl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerControl::CPhoneHandlerControl()
	: iPrevState( EPSCTsyCallStateNone )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::ConstructL( CRemConInterfaceSelector* aIfSelector )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::ConstructL() start" );
		
    if( !aIfSelector )
    	{
    	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::ConstructL() Create connection to RemCon." );
	    	
    	// Target connection to RemCon FW hasn't been done.
    	iInterfaceSelector = CRemConInterfaceSelector::NewL();
	   	iTarget = CRemConCallHandlingTarget::NewL( *iInterfaceSelector, *this );
		iInterfaceSelector->OpenTargetL();
		}
	else
		{
		COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::ConstructL() Don't create connection to RemCon." );
	    		
		// Connection to RemCon FW as target has already been done in a process.
		iTarget = CRemConCallHandlingTarget::NewL( *aIfSelector, *this );
		}
    	
	iResponse = CPhoneHandlerResponse::NewL( *this );
	iCallStateObserver = CPhoneHandlerCallState::NewL( *this );
    iCallArray = CPhoneHandlerCallArray::NewL();
    
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::ConstructL() end" );
	}

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerControl* CPhoneHandlerControl::NewL( 
									CRemConInterfaceSelector* aIfSelector )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NewL() start" );
    
    CPhoneHandlerControl* self = new ( ELeave ) CPhoneHandlerControl();
    
    CleanupStack::PushL( self );
    self->ConstructL( aIfSelector );
    CleanupStack::Pop( self );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NewL() end" );
    
    return self;
    }

// Destructor
CPhoneHandlerControl::~CPhoneHandlerControl()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::~CPhoneHandlerControl() start" );
    
    if( iInterfaceSelector )
    	{
    	delete iInterfaceSelector;
       	}
    
    if( iResponse )
    	{
    	iResponse->Delete();
       	}
       	
    if( iCallStateObserver )
    	{
    	delete iCallStateObserver;
    	}
    
    delete iCallArray;
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::~CPhoneHandlerControl() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::TelephoneNumber
// Provides phone number for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TPhCltTelephoneNumber& 
    CPhoneHandlerControl::TelephoneNumber() const
    {
    return iTelNumber;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::VoiceDialStatus
// Provides voice dial status for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TBool& 
    CPhoneHandlerControl::VoiceDialStatus() const
    {
    return iActivate;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::ChldCommand
// Provides CHLD command for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TInt& 
    CPhoneHandlerControl::ChldCommand() const
    {
    return iChldCommand;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::ChldCallNumber
// Provides CHLD call number for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TInt& 
    CPhoneHandlerControl::ChldCallNumber() const
    {
    return iChldCallNumber;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::Tone
// Provides DTMF tone for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TChar& 
    CPhoneHandlerControl::Tone() const
    {
    return iChar;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::CommandInitiator
// Provides API that service uses to send reponse to command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRemConCallHandlingTarget& 
    CPhoneHandlerControl::CommandInitiator() const
    {
    return *iTarget;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::NotifyCallState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::NotifyCallStateL( const MCall* aCall )
    {
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState() aCall.CallIndex:%d, aCall.CallState:%d",
				aCall->CallIndex(), aCall->CallState() );
    
    TCallInfo* call = iCallArray->FindCall( aCall->CallIndex() );
   
    if ( call )
    	{
    	// If the call is in idle or disconnecting state it can be removed from the array
    	if ( CCPCall::EStateIdle == aCall->CallState() 
    			|| CCPCall::EStateDisconnecting == aCall->CallState() )
    		{
    		COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState(): remove call" );
        	iCallArray->Remove( call );
    		}
    	else
    		{
    		COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState(): update existing call" );
    		iCallArray->Remove( call );
    		iCallArray->AddL( aCall );
    		}
    	}
    else
    	{
    	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState(): new call" );
    	iCallArray->AddL( aCall );
    	}
	COM_TRACE_3( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState() connected:%d existing:%d total:%d", 
			iCallArray->ConnectedCallCount(), iCallArray->ExistingCallCount(), iCallArray->CallCount() );
	}

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::AnswerCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::AnswerCall()
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::AnswerCall() start" );
	
	StartProcessing( ERemConExtAnswerCall );
	
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::AnswerCall() end" );
    }
	
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::EndCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::EndCall()
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::EndCall() start" );
    
    StartProcessing( ERemConExtEndCall );
    	    
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::EndCall() end" );
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::AnswerEndCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::AnswerEndCall()
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::AnswerEndCall() start" );
    
    StartProcessing( ERemConExtAnswerEnd );
    	   
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::AnswerEndCall() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::VoiceDial
// Dummy implementation. Operation implemented in SIND 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::VoiceDial( const TBool /*aActivate*/ )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::VoiceDial()" );
 
    iResponse->SetResponse( ERemConExtVoiceDial, KErrNone );
    iResponse->Process();

    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::VoiceDial() end" );
    }
 
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::LastNumberRedial
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::LastNumberRedial( )
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::LastNumberRedial() start" );
	
	if ( IsAutoLockOn() && !IsBTAccessoryCmd() )
        {
        iResponse->SetResponse( ERemConExtLastNumberRedial, KErrAccessDenied );
        iResponse->Process();
        return;
        }

	StartProcessing( ERemConExtLastNumberRedial );
	
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::LastNumberRedial() end" );
	}

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::DialCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::DialCall( const TDesC8& aTelNumber )
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::DialCall() start" );
    
	if ( IsAutoLockOn() && !IsBTAccessoryCmd() )
	        {
 	        iResponse->SetResponse( ERemConExtDialCall, KErrAccessDenied );
	        iResponse->Process();
	        return;
	        }
 	
    // Check aTelNumber range    
   	if( KPhCltTelephoneNumberLength < aTelNumber.Length() )
   		{
  		COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::DialCall() number value=%d too long", aTelNumber.Size() );
  		iResponse->SetResponse( ERemConExtDialCall, KErrArgument );
  		iResponse->Process();
  		return; 
  		}
  	
  	iTelNumber.Copy( aTelNumber );
  	      	  	    
    StartProcessing( ERemConExtDialCall );
    
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::DialCall() end" );
	}

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::MultipartyCalling
// Method supports multiparty call handling according to BT handsfree profile 
// (HFP) 1.5. BT HFP 1.5 defines contents of aData parameter as follows: 0, 1, 
// 1<idx>, 2, 2<idx>, 3 and 4, where: 
//
// 0 = Releases all held calls or sets User Determined User Busy (UDUB) for 
// a waiting call. 
// 1 = Releases all active calls (if any exist) and accepts the other 
// (held or waiting) call. 
// 1<idx> = Releases specified active call only (<idx> = digit from 1 to 9. 
// Command is e.g. "11" ). 
// 2 = Places all active calls (if any exist) on hold and accepts the other 
// (held or waiting) call. 
// 2<idx> = Request private consultation mode with specified call (<idx> = 
// from 1 to 9. Command is e.g. "21" ). 
// (Place all calls on hold EXCEPT the call indicated by <idx>.) 
// 3 = Adds a held call to the conversation. 
// 4 = Connects the two calls and disconnects the subscriber from both calls 
// (Explicit Call Transfer). Support for this value and its associated 
// functionality is optional for the HF. 
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::MultipartyCalling( const TDesC8& aData )
	{
	TBool error( EFalse );
	iChldCommand = 0;
	iChldCallNumber = 0;
	TBuf8< KRemConExtParamMaxLength > buf( aData );
	buf.Trim();
	TInt length = buf.Length();
	
	// Check invalid number of characters
	if ( length < KMultipartyCallMinParam || length > KMultipartyCallMaxParam )
	    {
	    error = ETrue;
	    }
	else
	    {
	    TLex8 param;
	    // Set command	
		param.Assign( buf.Mid( 0, 1 ) );
		if( param.Val( iChldCommand ) != KErrNone )
			{
			error = ETrue;
			}
		else if ( length == KMultipartyCallMaxParam )
	    	{
			// Set call number
			param.Assign( buf.Mid(1) );
			if( param.Val( iChldCallNumber ) != KErrNone )
				{
				error = ETrue;
				}
	    	}
	    }

	if( error )
		{
		// Invalid command 
		iResponse->SetResponse( ERemConExt3WaysCalling, KErrArgument );
  		iResponse->Process();
  		return;	
		}
		
	COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerControl::MultipartyCalling() iChldCommand=%d, iChldCallNumber=%d", iChldCommand, iChldCallNumber );
		
	StartProcessing( ERemConExt3WaysCalling );
	
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::MultipartyCalling() end" );
	}
	
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::GenerateDTMF
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::GenerateDTMF( const TChar aChar )
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::GenerateDTMF() start" );
	
	iChar = aChar;
	
	StartProcessing( ERemConExtGenerateDTMF );
	
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::GenerateDTMF() end" );
	}
	
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::SpeedDial
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::SpeedDial( const TInt aIndex )
	{
	COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::SpeedDial() aIndex=%d", aIndex );

	iIndex = aIndex;
	
	StartProcessing( ERemConExtSpeedDial );
	
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::SpeedDial() end" );
	}

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::StartProcessing
// Creates an object that executes call handling command received from 
// accessory.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::StartProcessing(	
	const TRemConExtCallHandlingApiOperationId aOperation )
	{
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::StartProcessing() start" );
    		
	// Create a service
	MPhoneHandlerService* service = NULL;
	TInt error( KErrNone );
	TRAP( error, service = CreateServiceL( aOperation ) ); 
		
	COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() returned %d", error );
		
	if( error )
		{
		COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::StartProcessing(): service wasn't created succesfully." );
		
		// An error happened. Delete service, if it exists.
		if( service )
			{
			service->Delete();
			service = NULL;
			}
		
		iResponse->SetResponse( aOperation, error );
  		iResponse->Process();
  		return;
		}
	else
		{
		// start service
		service->Process();	
		iSwitchCall = EFalse;
		}
			
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::StartProcessing() end" );
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::CreateServiceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MPhoneHandlerService* CPhoneHandlerControl::
	CreateServiceL( const TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() start" );
      
    MPhoneHandlerService* pService = NULL;

    switch( aOperation )
        {
        case ERemConExtAnswerCall:
		    {
		    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtAnswerCall command" );
		    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - iCallArray->ExistingCallCount() is %d", iCallArray->ExistingCallCount() );

			/*    	    
    	    if( iActiveCalls > 0 )
    	    	{
    	    	// Multiparty call
    	    	iSwitchCall = ETrue;
    	    	pService = CPhoneHandlerMultipartyCall::NewL( *this );
    	    	}
    	    else
    	    	{
    	    	// non-multiparty call
    	    	pService = CPhoneHandlerAnswerCall::NewL( *this );
    	    	}
    	    */
    	    pService = CPhoneHandlerAnswerCall::NewL( *this );

            break;	
		    }
		    
        case ERemConExtEndCall:
		    {
		    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtEndCall command" );
    	    
		    pService = CPhoneHandlerEndCall::NewL( *this );
		    
		    break;	
		    }
		    
        case ERemConExtAnswerEnd:
        	{
        	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtAnswerEnd command" );
    	       
        	if( iCallArray->ExistingCallCount() )
          		{
          		COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - call(s) exist" );
          		// If only one call exists and it's state is CCPCall::EStateQueued,
          		// it is actually in ringing state and should be answered.
          		if ( NULL == iCallArray->CallByState( CCPCall::EStateRinging ) 
						&& ( iCallArray->ExistingCallCount() == 1 
							&& NULL == iCallArray->CallByState( CCPCall::EStateQueued ) ) )
						{
						COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - no ringing calls" );
						pService = CPhoneHandlerEndCall::NewL( *this, 
														   aOperation );
						}
				// Ringing call(s) exist
				else
					{
					if( iCallArray->ExistingCallCount() > 1 )
						{
						COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - several ringing calls" );
						// multicall case : End call that has existed longer.
						// (From UI viewpoint ringing call is in waiting state.)
						pService = CPhoneHandlerEndCall::NewL( *this, 
													   aOperation );
						}
					else
						{
						COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - single ringing call" );
						pService = CPhoneHandlerAnswerCall::NewL( 
										*this, 
										aOperation );
						}
					}
		    	}
		    else
		    	{
		    	// Send / end button was pressed when there were not 
		    	// any calls active. Response has to be sent back to accessory
		    	// at least for following reasons:
		    	
		    	// 1. RemCon FW releases message related memory only when response
		    	// is sent back to accessory.
		    	
		    	// 2. BT accessory key press producing ERemConExtAnswerEnd 
		    	// operation has some other meaning than answer/end call 
		    	// and it's processed by proper client. However, there 
		    	// might be a situation where proper client isn't active 
		    	// and can't process command. In any case RemCon/BT accessory
		    	// requires response to command. That's why PhoneCmdHandler 
		    	// sends reponse to command. 
								
				iResponse->SetResponse( aOperation, KErrNone );
				pService = iResponse;
				}
        	break;
        	} 
		    
		case ERemConExtDialCall:
		    {
		    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtDialCall command" );
    	    		    
		   	pService = CPhoneHandlerDialCall::NewL( *this );
            break;	
		    }
		    
		case ERemConExtVoiceDial:
        	{
		    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtVoiceDial command" );
    	    		    
		    pService = CPhoneHandlerVoiceDial::NewL( *this );
            break;	
		    }
		    
        case ERemConExtLastNumberRedial:
        	{
		    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtLastNumberRedial command" );
    	    		    
		    pService = CPhoneHandlerLastNumberRedial::NewL( *this );
            break;	
		    }
		    
		case ERemConExt3WaysCalling:
        	{
		    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExt3WaysCalling command" );
    	    		    
		    pService = CPhoneHandlerMultipartyCall::NewL( *this );
            break;	
		    }
		    
		case ERemConExtGenerateDTMF:
        	{
		    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtGenerateDTMF command" );
    	    		    
		    pService = CPhoneHandlerDTMF::NewL( *this );
            break;	
		    }
		    
        case ERemConExtSpeedDial:
        	{
        	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtSpeedDial command" );
    	            	
        	InitializeSpeedDialL();
        	pService = CPhoneHandlerDialCall::NewL( *this, 
        											aOperation );
            break;	
		    } 
				    
        default:
        	{
        	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() Unspecified state" );
			break;
	       	}
        };
        
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() end, %d", pService );
    return pService;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::InitializeSpeedDialL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::InitializeSpeedDialL()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::InitializeSpeedDialL() start" );
    
    // first check contack engine is not in busy
  TInt cntEngState( 0 );
	TInt err = iProperty.Get( KUidSystemCategory, conn::KUidBackupRestoreKey, cntEngState ); 
  
	if( err == KErrNotFound || cntEngState == 0 || 
			cntEngState & conn::KBURPartTypeMask == conn::EBURNormal || 
			cntEngState & conn::KBackupIncTypeMask == conn::ENoBackup )
		{
	    // Get phone number from phonebook by index	
		CPbkContactEngine* ptr = CPbkContactEngine::NewL();
    CleanupStack::PushL( ptr );       
		TRACE_ASSERT( ptr != NULL );
		ptr->GetSpeedDialFieldL( iIndex, iTelNumber );
		COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::InitializeSpeedDialL() speed dial number is %S", &iTelNumber );
		CleanupStack::PopAndDestroy( ptr );
		}
	else
		{
		// contact engine is in busy
		COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::InitializeSpeedDialL() contact engine is in backup/restore" );
		User::Leave( KErrInUse );
		}
		
	COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::InitializeSpeedDialL() end" );
	}

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::SwitchCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPhoneHandlerControl::SwitchCall()
    {
    return iSwitchCall;
	}

// ---------------------------------------------------------
// CPhoneHandlerControl::IsAutoLockOn
// ---------------------------------------------------------
//
TBool CPhoneHandlerControl::IsAutoLockOn() const
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::IsAutoLockOn() start " );
    TInt err( KErrNone );
    TInt value( EAutolockStatusUninitialized );

    err = RProperty::Get( KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, value );
    if ( err != KErrNone )
        {
        value = err;
        }
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::IsAutoLockOn() autolock value = %d", value );
    
    return value > EAutolockOff ? ETrue : EFalse;
      
    }
// ---------------------------------------------------------
// CPhoneHandlerControl::IsBTAccessoryCmd
// ---------------------------------------------------------
//
TBool CPhoneHandlerControl::IsBTAccessoryCmd() const
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::IsBTAccessoryCmd() start " );
    TBool retval( EFalse );
    
    TRemConExtCmdSource source;             
    iTarget->GetCommandSourceInfo( source );
    if ( source == ERemConExtCmdSourceBluetooth )
        {       
        retval = ETrue;
        }
    
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::IsBTAccessoryCmd() GetCommandSourceInfo = %d", source );
       
    return retval;
    }

 

 
// ======================== OTHER EXPORTED FUNCTIONS ===========================

// End of File
