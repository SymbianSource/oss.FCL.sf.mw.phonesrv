/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of RVmbxNumber class.
*
*/


// INCLUDE FILES

#include <vmnumber.h>
#include <e32std.h>
#include <avkon.hrh>
#include <bautils.h>
#include <eikenv.h>
#include <commonphoneparser.h>
#include <aknnotewrappers.h>
#include <StringLoader.h>
#include <data_caging_path_literals.hrh>
#include <PhCltUtils.h>
#include <vmbx.rsg>
// includes for phonebook search
#include <cphonecntfactory.h>
#include <cphcntsingleitemfetch.h>
#include <featmgr.h>
#include <AknIconArray.h>
#include <AknsUtils.h>

#ifdef RD_STARTUP_CHANGE
    #include <startupdomainpskeys.h>//for KPSSimStatus
#endif

#include <AknQueryDialog.h>

/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
#include <mpbutil.h>
#include <utf.h> //CnvUtfConverter
#include "voicemailboxprivatecrkeys.h"
#include "voicemailboxdomaincrkeys.h"
#include <PSVariables.h>
#include <e32property.h>
#include "vmdialog.h"
#include "vmlist.h"
#include "vmblogger.h"
#include <vm.mbg>
#include "vmspshandler.h"
#include "vmbshandler.h"

// CONSTANTS
const TInt KVmFlagOpened                 = 0x00000001;
const TInt KVmFlagPhoneOpened            = 0x00000002;
const TInt KVmFlagSimReadSupport         = 0x00000004;
const TInt KVmFlagPhoneModuleLoaded      = 0x00000008;
const TInt KVmFlagNotifyRequested        = 0x00000010;
const TInt KVmFlagNotifyOnActiveLineOnly = 0x00000020;
const TInt KVmFlagPhonebookBeingCached   = 0x00000040;
const TInt KVmFlagSimWriteSupport        = 0x00000080;
const TInt KVmFlagSimVmbxNumSupport      = 0x00000100;
// Flag of external API saving number to phone
const TInt KForceSavedToPhoneMem         = 0x00000400;

//flag of CS defined number
const TInt KVmFlagCSNumberDefined        = 0x00000001;
//flag of Vedio defined number
const TInt KVmFlagVideoNumberDefined     = 0x00000002;
//flag of VoIP defined number
const TInt KVmFlagVoIpNumberDefined      = 0x00000004;
// phone application uid
const TInt KPhoneApplicationUid          = 0x100058B3;

_LIT (KVmbxResourceFileDrive, "Z:");
// The file directory is in literal KDC_RESOURCE_FILES_DIR
// (in data data_caging_path_literals.hrh)
_LIT (KVmbxResourceFileName, "Vmbx.rsc");

// Phonebook entry name
_LIT( KVmbxPhoneBookEntryName, "Vmbx" );  // If changed, remember to update
// the size of KVmbxPhoneBookEntryNameLength in VmNumber.h
const TInt KVmbxPhonebookRetriesIfInUse = 20;
const TInt KVmbxPhonebookRetryDelay     = 100000;  // 0.1s

const TInt KVmbxPhonebookBufferSize     = 150;  // Estimated max size 128

_LIT( KVmbxOnePlus, "1" );

// Format of the IAP selection list query.
_LIT( KQueryItemFormat, "%d\t%S" );
//for the mbm file
_LIT( KVmMbmDrive, "Z:" );
_LIT( KVmLibMbmFile, "vm.mbm" );

const TInt KVmNumOverwrite = 1;

const TInt KVmNamStoreMDNId = RMobileNamStore::EMobileDirectoryNumber;
// MIN is not included in mmetel std params.  Third extended param
const TInt KVmNamStoreMINId = RMobileNamStore::ENumStandardNamParameters + 3;
const TInt KVmPhNumMinLength = 3;
const TInt KVmLbxItemsArraySize = 3;
const TInt KVmOriginalNumLength = 256;
const TInt KVmPhoneNumDigitsMinLength = 2;
const TInt KVmPhoneNumDigitsMaxLength = 40;
const TInt KVmbxOnlyDefinedOneNumber = 1;
const TInt KMailBoxLimitForPrompt = 1;

_LIT (KVmbxIllegalSimCharacter, "w");
_LIT (KTestNumber, "12345");

const TInt KVmExtFileSize = 20;
const TInt KVmMaxStoreSize = 48;

const TInt KDefaultServiceId = 0; // default ServerId
const TInt KCSorVideoServiceId = 1; // CS or Video ServerId
const TInt KDefaultDefinedNumberCount = 0; // default defined number count

// When the define VM_USE_TSY is set, SIM phonebook will be used and
// methods won't work if a compatible TSY/DOS environment is not present.

/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
// &
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/

#ifndef __WINS__  // SIM phonebook stalls in WINS
#define VM_USE_TSY
#endif
// ================= MEMBER FUNCTIONS ==========================================



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RVmbxNumber::RVmbxNumber
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RVmbxNumber::RVmbxNumber() : iNotifyCallBack( NULL ),
                                iUserAlsObserver( NULL ),
                                iPhoneBookInfoPckg( iPhoneBookInfo ),
                                iPhoneVoicemailInfoPckg( iPhoneVoicemailInfo ),
                                iRSatSession(),
                                iRSatClient( *this ),
                                iNotifyHandler( NULL ),
                                iSession( NULL ),
                                iResourceFileOffset( 0 ),
                                iFlags( KForceSavedToPhoneMem ), 
                                iQuery( NULL ),
                                iClosedPtr( NULL ),
                                iCoeEnv( NULL ),
                                iVmNumOpts( EVmNumOptsBlank ),
                                iVmUiOpts( 0 ), 
                                iSynchronize( EFalse ),
                                iType( EVmbxNone ),
                                iVMSimQueryDialog( NULL ),
                                iVideoQuery( NULL ),
                                iTypeSelectionQuery( NULL ),
                                iAlphaStringFound( EFalse ),
                                iAlphaStringFromSIM( NULL ),
                                iNumberFound( EFalse ),
                                iUSimFirstRoundTest( ETrue ),
                                iCntFetch( NULL ),
                                iVmSpsHandler( NULL ),
                                iVmBsHandler( NULL ),
                                iFeatMgrInitialized( EFalse ),
                                iSimCardFound( EFalse )              
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::RVmbxNumber =>" );

    // Default sets to Phone memory, this can be changed in RVmbxNUmber::Open()
    iMemoryLocation = EVmbxPhoneMemory;

    //Create repository and notify handler.
    TRAPD( error,
           FeatureManager::InitializeLibL();
           iFeatMgrInitialized = ETrue;
           VMBLOGSTRING( "VMBX: RVmbxNumber::RVmbxNumber: FeatureManager initialized" );

           iSession = CRepository::NewL( KCRUidVoiceMailbox );
           VMBLOGSTRING( "VMBX: RVmbxNumber::RVmbxNumber: CRepository created" );

           if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
               {
               iVmSpsHandler = CVmSPSHandler::NewL();
               VMBLOGSTRING( "VMBX: RVmbxNumber::RVmbxNumber: CVmSPSHandler created" );
               }

           iVmBsHandler = CVmBSHandler::NewL();
           VMBLOGSTRING( "VMBX: RVmbxNumber::RVmbxNumber: CVmBSHandler created" );
           );

    if ( error != KErrNone )
        {
        VMBLOGSTRING2( "VMBX: RVmbxNumber::RVmbxNumber : \
        Initialization error %I", error );
        _LIT(KVmbxConstructorPanicType,"RVmbxNumber::RVmbxNumber");
        User::Panic( KVmbxConstructorPanicType, error );
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::RVmbxNumber <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::~RVmbxNumber
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C RVmbxNumber::~RVmbxNumber()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::~RVmbxNumber =>" );

    if ( iNotifyHandler )
        {
        iNotifyHandler->StopListening();
        delete iNotifyHandler;
        }

    delete iSession;
    delete iAlphaStringFromSIM;

    delete iVmSpsHandler;
    delete iVmBsHandler;

    if ( iFeatMgrInitialized )
        {
        FeatureManager::UnInitializeLib();
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::~RVmbxNumber <=" );
    }



// -----------------------------------------------------------------------------
// RVmbxNumber::Open
// Opens a new ETel connection
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
EXPORT_C TInt RVmbxNumber::Open()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::Open =>" );

    TInt ret( KErrGeneral );
    // initialize the CenRep number in the bankup store
    ret = BackUpSimNumber( KEmptyVoiceMailNumber, KVmbxTmpNumberLinePrimary );
    VMBLOGSTRING2( "RVmbxNumber::Open(): Clear KVmbxTmpNumberLinePrimary result = %d",
    ret );
    ret = BackUpSimNumber( KEmptyVoiceMailNumber, KVmbxTmpNumberLineAuxiliary );
    VMBLOGSTRING2( "RVmbxNumber::Open(): Clear KVmbxTmpNumberLineAuxiliary result = %d",
    ret );
    VMBLOGSTRING( "RVmbxNumber::Open(): Clear Tmp numbers from CenRep" );
    if ( iFlags & KVmFlagOpened )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::Open: Opened already" );
        ret = KErrNone;
        }
    else
        {
        ret = ConnectETelServer();
        if ( !ret )
            {
            // try to load ETel TSY module
            ret = iTelServer.LoadPhoneModule( KMmTsyModuleName );
            if ( !ret )
                {
                iFlags |= KVmFlagPhoneModuleLoaded;

                RTelServer::TPhoneInfo info;
                ret = iTelServer.GetPhoneInfo( 0, info );
                if ( !ret )
                    {
                    ret = iPhone.Open( iTelServer, info.iName );
                    if ( !ret )
                        {
                        iFlags |= KVmFlagPhoneOpened;
                        }
                    }
                }
            }

        if ( !ret )
            {
            ret = Open( iPhone );
            }
        else
            {
            Close();
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::Open <=" );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::Open
// Opens a new ETel connection, when there is existing ETel connection already
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
EXPORT_C TInt RVmbxNumber::Open( RMobilePhone& aPhone )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::Open =>" );
    TInt result( KErrNone );
    TInt alsline( EAlsLine1 );

    if ( iFlags & KVmFlagOpened )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::Open: Opened already" );
        return KErrNone;
        }
    iPhone = aPhone;

    result = iSession->Get( KVmUsesSimMemory, iMemoryLocation );

    if ( KErrNone != result )
        {
        if ( KErrNotFound == result )
            {
            iMemoryLocation = EVmbxPhoneMemory;
            }
        else
            {
            Close();
            VMBLOGSTRING( "VMBX: RVmbxNumber::Open: Memory location error" );
            return result;
            }
        }

    VMBLOGSTRING2( "iMemoryLocation = %d", iMemoryLocation );

    // Connect to system agent
    RProperty property;
    TInt psErr( KErrNone );

#ifdef RD_STARTUP_CHANGE
    psErr = property.Attach( KPSUidStartup, KPSSimStatus );
#else
    psErr = property.Attach( KUidSystemCategory, KPSUidSimCStatusValue );
#endif
    if ( KErrNone != psErr )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::Open: SIM status property error" );
        return psErr;
        }

   result = iSsSettings.Open( NULL );

    if ( KErrNone != result )
        {
        Close();
        VMBLOGSTRING( "VMBX: RVmbxNumber::Open: SS setting opening error" );
        return result;
        }

    // ignore error
    iSsSettings.Register( ESSSettingsAls, *this );

    // get the current ALS line
    if ( KErrNone != GetAlsLine( alsline ) )
        {
        alsline = EAlsLine1;  // if problems, assume primary line
        }

#ifdef VM_USE_TSY

    TInt nValue;
    psErr = property.Get( nValue );
    TBool simStatus = EFalse;

    // Sim aceess test on startup
    iStartUpTest = ETrue;
#ifdef RD_STARTUP_CHANGE

    if ( ( FeatureManager::FeatureSupported( KFeatureIdSimCard ) )
        && ( ESimNotPresent != nValue ) )
        {
        simStatus = ETrue;
        }

#else //RD_STARTUP_CHANGE

    if ( ( FeatureManager::FeatureSupported( KFeatureIdSimCard ) )
        && ( EPSCSimRemoved != nValue ) )
        {
        simStatus = ETrue;
        }
#endif // RD_STARTUP_CHANGE

    VMBLOGSTRING2( "simStatus = %d", simStatus );
    if ( simStatus )//Is simCard supproted and it is not removed
        {
        iSimCardFound = ETrue;

        // Get identifiers from MBI-file
        if ( EAlsLine1 == alsline )
            {
            result = MailboxNumbersIdentifiers();
            VMBLOGSTRING2( "Identifier result = %d", result );
            if ( KErrPathNotFound == result )
                {
                // try open vmbx-phonebook next
                iPhoneBookType = EVMBXPhoneBook;
                iMbdnPhonebookOk = EFalse;
                VMBLOGSTRING( "no mbdn file is found, use vmbx phonebook" );
                }
            else
                {
                iPhoneBookType = EMBDNPhoneBook;
                iMbdnPhonebookOk = ETrue;
                // if mbdn file can be found, set the entry index to EAlsLine1
                iPhoneVoicemailInfo.iVoice = EAlsLine1;

                // try to open mbdn-type phonebook
                result = iPhoneBook.Open( iPhone, KETelIccMbdnPhoneBook );
                VMBLOGSTRING2( "Mbdn phonebook opening result = %I ", result );
                if ( KErrNone == result )
                    {
                    // Get phonebook info
                    result = PhoneBookInfo();
                    VMBLOGSTRING2( "Mbdn phonebook opening again \
                    result = %I ", result );
                    }
                // close mbdn phonebook when error in opening or GetInfo 
                // to test vmbx-phonebook
                if ( KErrNone != result )
                    {
                    VMBLOGSTRING( "Close MBDN phonebook" );
                    iPhoneBook.Close();
                    iMbdnPhonebookOk = EFalse;
                    // try open vmbx-phonebook next 
                    iPhoneBookType = EVMBXPhoneBook;
                    }
                // close mbdn phonebook when no number found 
                // to test vmbx-phonebook
                else if ( ( KErrNone == result ) && iNoNumberFound )
                    {
                    VMBLOGSTRING( "mbdn close, number not found" );
                    iMbdnPhonebookOk = ETrue;
                    iPhoneBook.Close();
                    // try open vmbx-phonebook next 
                    iPhoneBookType = EVMBXPhoneBook;
                    }
                }
            }

        if ( EAlsLine2 == alsline || EVMBXPhoneBook == iPhoneBookType )
            {
            // the mbdn number is not used when line2 is active
            // the number is only got from vmbx-phonebook
            iMbdnPhonebookOk = EFalse;
            // try to open vmbx-type phonebook
            result = iPhoneBook.Open( iPhone, KETelIccVoiceMailBox );
            VMBLOGSTRING2( "Vmbx phonebook opening result = %I ", result );
            if ( KErrNone == result )
                {
                // check that this phonebook supports reading
                result = PhoneBookInfo();
                VMBLOGSTRING( "VmbxPhoneBook opened" );
                }            	
            }
        VMBLOGSTRING3( "VMBX: RVmbxNumber::Open: \
        iMbdnPhonebookOk: %I, iNoNumberFound: %I", iMbdnPhonebookOk, iNoNumberFound );
        // reopen mbdn-phonebook when vmbx-phonebook has no number
        if ( iMbdnPhonebookOk && iNoNumberFound )
           {
           VMBLOGSTRING( "reopen mbdn" );
           iPhoneBook.Close();
           // try open vmbx-phonebook next
           iPhoneBookType = EMBDNPhoneBook;
           // try to open mbdn-type phonebook
           result = iPhoneBook.Open( iPhone, KETelIccMbdnPhoneBook );
           VMBLOGSTRING2( "Mbdn phonebook opening result = %I ", result );
           if ( KErrNone == result )
               {
               // Get phonebook info
               result = PhoneBookInfo();
               }
           // close phonebook when error in opening or GetInfo
           if ( KErrNone != result )
               {
               VMBLOGSTRING( "Close MBDN phonebook" );
               iPhoneBook.Close();
               }
           }

        // set readonly state if aquired
        if ( IsReadOnlySIM() )
            {
            iFlags &= ~KVmFlagSimWriteSupport;
            VMBLOGSTRING( "VMBX: RVmbxNumber::Open: ReadOnly set" );
            }

        if ( KErrNone != result && KErrInUse != result )
            {
            // Fatal error
            Close();
            property.Close();
            VMBLOGSTRING( "VMBX: RVmbxNumber::Open: Phonebook info error" );

            return result;
            }

        // If no SIM support for vmbx numbers, we don't need
        //ETel for anything. -> close all ETel resources
        if ( !( iFlags &
                (KVmFlagPhonebookBeingCached | KVmFlagSimReadSupport ) ) )
            {
            CloseETel();
            }
        }
    // Sim access rights are tested only in start up
    iStartUpTest = EFalse;
#endif  // VM_USE_TSY

    property.Close();

    if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
        {
        result = iSession->Get( KVmNumOpts,iVmNumOpts );

        if ( KErrNone != result )
            {
            if ( KErrNotFound == result )
                {
                iVmNumOpts = EVmNumOptsBlank;
                }
            else
                {
                Close();
                VMBLOGSTRING( "VMBX: RVmbxNumber::Open: NumOpt error" );
                return result;
                }
            }
        }
    result = iSession->Get( KVmUIOpts, iVmUiOpts );

    if ( KErrNone != result )
        {
        if ( KErrNotFound == result )
            {
            iVmUiOpts = 0;
            }
        else
            {
            Close();
            VMBLOGSTRING( "VMBX: RVmbxNumber::Open: NumOpt error" );
            return result;
            }
        }

    iCoeEnv = CEikonEnv::Static();

    // load the resources for queries and notes
    if ( iCoeEnv )
        {
        __ASSERT_DEBUG( iCoeEnv,
                    User::Panic( KVmbxPanicCategory, EVmbxNoCoeEnv ) );

        // load the resources for queries and notes
        TInt tmpStatus;
        TRAP( tmpStatus, LoadResourceL() );
        result = tmpStatus;

        if ( result )
            {
            Close();
            }
        else
            {
            iFlags |= KVmFlagOpened;
            }
        }

    HandleNumberStores( alsline );

    VMBLOGSTRING( "VMBX: RVmbxNumber::Open <=" );
    return result;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::ConnectETelServer
// Connects to ETel server
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
TInt RVmbxNumber::ConnectETelServer()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::ConnectETelServer =>" );
    const TInt KRetryCount = 7;
    const TInt KRetryTimeout = 100000;
    TInt status( KErrNone );

    for ( TInt i( 0 ); i < KRetryCount; i++ )
        {
        status = iTelServer.Connect();
        if ( !status )
            {
            break;
            }
        User::After( KRetryTimeout );
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::ConnectETelServer <=" );
    return status;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetVmbxNumber
// Fetches the specified vmbx number
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
EXPORT_C TInt RVmbxNumber::GetVmbxNumber( TDes& aNumber,
                                                    TVmbxNumberEntry aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmbxNumber =>" );
    __ASSERT_DEBUG( iFlags & ( KVmFlagOpened ),
                    User::Panic(KVmbxPanicCategory, EVmbxNotConnected) );
    __ASSERT_DEBUG( aNumber.MaxLength() >= KVmbxMaxNumberLength,
                    User::Panic(KVmbxPanicCategory, EVmbxDescriptorTooSmall) );

    TInt lineNumber( aEntry );

    if ( aEntry == EAlsActiveLineEntry )
        {
        // get the current ALS line
        if ( GetAlsLine( lineNumber ) != KErrNone )
            {
            lineNumber = EAlsLine1;  // if problems, assume primary line
            }
        }
    TInt ret( KErrNotFound );
    TBuf< KVmbxMaxNumberLength > number;
    TBuf< KVmbxMaxNumberLength > line2AlsNumber;

    //VMN callback#
    if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) &&
         ( iVmUiOpts & KVmNumOverwrite ) )
        {
        ret = GetVmNumFromVMN( number );
        if ( ret == KErrNone )
            {
            aNumber.Copy( number );
            VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmbxNumber OK"  );
            return ret;
            }
        else if ( ret != KErrNotFound )
            {
            VMBLOGSTRING2( "VMBX: RVmbxNumber:: \
            GetVmbxNumber error = %I", ret );
            return ret;
            }
        }

    TBool isFromSD = EFalse;
    TBool isFromSim = EFalse;

    // Get saved number
    switch ( iVmNumOpts )
        {
        case EVmNumOptsBlank:
            {
            // On ALS1 read number from sim, if sim is active
            // On ALS2 always read number from sim when vmbx phone memory in use
            if ( IsSimMemory() && ( ( EAlsLine1 == lineNumber )
                           || ( EAlsLine2 == lineNumber ) ) )
                {
                ret = GetVmNumFromSIM( number, lineNumber );
                isFromSim = ETrue;
                // If there is no voice mailbox number available on SIM,
                // a note(qtn_...) should be shown,
                // when pressing '1' + 'send' key or long pressing '1' key
                // to try to call voice mailbox by phone application.
                if ( ( !UserEditNumber( lineNumber ) 
                    || !IsPhoneMemForcedEdit( lineNumber ) ) &&
                    ( ( KErrNotFound == ret ) || ( KErrNotSupported == ret ) )
                    && !AllowedToChangeTheNumber() )
                    {
                    // get current active process
                    RProcess curProcess;
                    TInt curProcessId( curProcess.SecureId().iId );
                    VMBLOGSTRING2( "VMBX: RVmbxNumber:: Get cur process id: \
                    curProcessId = %I", curProcessId );

                    if ( KPhoneApplicationUid == curProcessId )
                        {
                        TBool thisClosed( EFalse );
                        iClosedPtr = &thisClosed;

                        // This check is done because phone client calls  
                        // GetVmbxNumber and then SelectTypeL, 
                        // which causes note qtn_... to be shown twice
                        if ( IsNoNumberNoteAllowed() )
                            {
                            // show the information note to user
                            TRAP_IGNORE( 
                            ShowInformationNoteL( R_VOICE_NO_VMBX_NUMBER_NOTE ) )
                            }

                        iClosedPtr = NULL;
                        if ( thisClosed )
                            {
                            //End Key Pressed
                            VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmbxNumber: \
                            End key pressed" );
                            return KErrCancel;
                            }
                        VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmbxNumber \
                        No voice mailbox number available on SIM" );
                        }
                    }
                if ( KErrNone == ret )
                    {
                    ResetUserEditNumber( lineNumber );
                    }
                VMBLOGSTRING2( "GetVmNumFromSIM = %I", ret );
                }
            else
                {
                ret = KErrNotFound;
                }
            break;
            }
        case EVmNumOptsDedicatedVMN:
            {
            ret = GetVmNumFromVMN( number );
            break;
            }
        case EVmNumOptsMIN:
        case EVmNumOptsOnePlusMIN:
            {
            ret = GetVmNumFromNamStore( number, EVmbxMIN );
            break;
            }
        case EVmNumOptsMDN:
        case EVmNumOptsOnePlusMDN:
            {
            ret = GetVmNumFromNamStore( number );
            break;
            }
        case EVmNumOptsMDNCustom:
        case EVmNumOptsOnePlusMDNCustom:
            {
            ret = GetVmNumFromSD( number, lineNumber );
            if ( ret != KErrNone )
                {
                ret = GetVmNumFromNamStore( number );
                }
            break;
            }
        case EVmNumOptsSharedData:
            {
            ret = GetVmNumFromSD( number, lineNumber );
            isFromSD = ETrue;
            break;
            }
        default:
            //
            break;
        }

        // On ALS2, if number exists on SD, then use it
        // if ALS2 number found in SIM, but not in phone memory,
        // reset flag for automatic SIM number use.
        // Number found from SIM is saved to SD later

    if ( lineNumber == EAlsLine2 )
        {
         TInt alsReturnValue = GetVmNumFromSD( line2AlsNumber, lineNumber );
         if ( ( alsReturnValue == KErrNone ) && ( line2AlsNumber.Length() ) )
            {
            VMBLOGSTRING( "GetVmbxNumber::ALS2 number found from CenRep"  );
            number.Copy( line2AlsNumber );
            isFromSD = ETrue;
            }
         else
            {
            if ( ( KErrNone == ret ) && isFromSim )
                {
                VMBLOGSTRING( "[VMBX] GetVmbxNumber::ALS 2 number found from SIM"  );
                ResetUserEditNumber( lineNumber );
                }
            else
                {
                number.Zero();
                }
            }
        }

    // No number retrieved so try to read SD for number
    // (Phone memory in use)
    if ( ret != KErrNone )
        {
        switch( iVmNumOpts )
            {
            case EVmNumOptsBlank:
            case EVmNumOptsDedicatedVMN:
            case EVmNumOptsMIN:
            case EVmNumOptsOnePlusMIN:
            case EVmNumOptsMDN:
            case EVmNumOptsOnePlusMDN:
                {
                if ( !IsSimMemory() )
                    {
                    ret = GetVmNumFromSD( number, lineNumber );
                    VMBLOGSTRING2( "GetVmNumFromSD = %I", ret );
                    isFromSD = ETrue;
                    }
                else
                    {
                    aNumber.Copy( number );
                    VMBLOGSTRING2( "number = %S", &number );
                    }
                break;
                }
            default:
                break;
            }
        }

    if ( ret == KErrNone )
        {
        if ( number.Length() )
            {
             VMBLOGSTRING( "Number length > 0" );
            //Synchronize the number in storage with the retrieved number
            if ( iVmNumOpts != EVmNumOptsMDNCustom &&
                iVmNumOpts != EVmNumOptsOnePlusMDNCustom &&
                !isFromSD )
                {
                iSynchronize = ETrue;

                if ( ( iFlags & KVmFlagSimWriteSupport ) ||
                       ( lineNumber == EAlsLine2 ) )
                    {
                    TInt error( KErrNone );
                    // If number is not fetched from SIM and SIM memory active
                    // on primary line, number saved to SIM.
                    if ( UsesSimMemory() )
                        {
                        if ( !isFromSim )
                            {
                            error = SaveVmbxNumberToSim( number, aEntry );
                            VMBLOGSTRING2( "SaveVmbxNumberToSim = %I", error );
                            }
                        }
                    else
                        {
                        error = SaveVmbxNumberToPhone( number, aEntry );
                        VMBLOGSTRING2( "SaveVmbxNumberToPhone = %I", error );
                        }
                    }
                else
                    {
                    iFlags &= (~KForceSavedToPhoneMem);
                    SaveVmbxNumber( number, aEntry );
                    }
                }

            if ( iVmNumOpts == EVmNumOptsOnePlusMIN ||
                                          iVmNumOpts == EVmNumOptsOnePlusMDN )
                {
                if ( KVmbxMaxNumberLength < number.Length() + 1 )
                    {
                    VMBLOGSTRING( "GetVmbxNumber::Number too long" );
                    return KErrTooBig;
                    }
                // Add 1 plus
                number.Insert( 0, KVmbxOnePlus );
                }

            aNumber.Copy( number );
            }
        else
            {
            VMBLOGSTRING( "GetVmbxNumber::No number found." );
            ret = KErrNotFound;
            }
        }

    VMBLOGSTRING2( "VMBX: RVmbxNumber::GetVmbxNumber error = %I", ret );
    if ( ret != KErrNone )
        {
        return KErrNotFound;
        }

    if ( IsEmergencyNumberL( aNumber ) )
        {
        aNumber.Zero();
        ret = KErrNotFound;
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::QueryNumberL
// Check wether the voice mailbox number can be changed
// -----------------------------------------------------------------------------
//
EXPORT_C TBool RVmbxNumber::QueryNumberL( TVmbxQueryType aQueryType,
                                          TDes& aNumber,
                                          TBool aShowQuery)
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: =>" );
    __ASSERT_DEBUG( aNumber.MaxLength() >= KVmbxMaxNumberLength,
                   User::Panic( KVmbxPanicCategory, EVmbxDescriptorTooSmall ) );
    __ASSERT_DEBUG( aNumber.Length() <= KVmbxMaxNumberLength,
                        User::Panic( KVmbxPanicCategory, EVmbxNumberTooLong ) );

    VMBLOGSTRING2( "aQueryType = %d", aQueryType );
    VMBLOGSTRING2( "iPhoneBookType = %d", iPhoneBookType );

    TBool result( EFalse );
    if ( EVmQueryProcess == iQueryingState )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: \
        running forever case" );
        VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: <=" );
        return result;
        }

    if ( !AllowedToChangeTheNumber() )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: \
        Number not allowed to change" );
        VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: <=" );
        User::Leave( KErrAccessDenied );
        }

    // Vmbx type is set for FetchNumberFromPhonebookL
    iType = EVmbx;

    __ASSERT_DEBUG( aNumber.MaxLength() >= KVmbxMaxNumberLength,
                   User::Panic( KVmbxPanicCategory, EVmbxDescriptorTooSmall ) );
    __ASSERT_DEBUG( aNumber.Length() <= KVmbxMaxNumberLength,
                        User::Panic( KVmbxPanicCategory, EVmbxNumberTooLong ) );

    // Close() sets thisClosed to ETrue
    TBool thisClosed( EFalse );

    TBuf<KVmbxMaxNumberLength> vmbxNumber;

    TInt alsline( EAlsLine1 );
    // get the current ALS line
    if ( GetAlsLine( alsline ) != KErrNone )
        {
        // if problems, assume primary line
        alsline = EAlsLine1;
        }
    VMBLOGSTRING2( "VMBX: RVmbxNumber::QueryNumberL: ALSline = %d",
        alsline );

    if ( iFlags & KVmFlagSimReadSupport )
        {
        VMBLOGSTRING( "RVmbxNumber::QueryNumberL: SIM Read support" )
        TBool querySim( EFalse );

        if ( alsline == EAlsLine1 )
            {
            // Do sim query when not defined query
            // or 6f17 Sim memory not active and query wanted
            if ( ( aQueryType == EVmbxNotDefinedQuery ) ||
                 ( !IsSimMemory() && aShowQuery  &&
                   ( iMemoryLocation == EVmbxPhoneMemory ) ) )
                {
                VMBLOGSTRING( "RVmbxNumber::QueryNumberL: Query on line1" );
                querySim = ETrue;
                }
            }
        else //EAlsLine2
            {
            // Allways and only when vmbx phonebook in use
            if  ( iPhoneBookType == EVMBXPhoneBook )
                {
                VMBLOGSTRING( "RVmbxNumber::QueryNumberL: Query on line2" );
                querySim = ETrue;
                }
            }

        if ( querySim )
            {
            iClosedPtr = &thisClosed;
            VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: Q SIM number" );

            TBool isSimNumber = QuerySimNumberL( vmbxNumber );

            VMBLOGSTRING2( "VMBX: RVmbxNumber::QueryNumberL: SIM number %S",
            &vmbxNumber );

            iClosedPtr = NULL;

            if ( thisClosed )
                {
                //End Key Pressed
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: \
                End key pressed" );
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: <=" );
                return EFalse;
                }

            if ( isSimNumber )
                {
                // save to phone memory when ALS2 in use
                // or read-only sim on ALS1
                if ( ( alsline == EAlsLine2 ) ||
                     ( ( alsline == EAlsLine1 ) &&
                       !( iFlags & KVmFlagSimWriteSupport ) ) )
                    {
                    VMBLOGSTRING( "Save number To Phone" );
                    SaveVmbxNumberToPhone( vmbxNumber, EAlsActiveLineEntry );
                    // confirmation note is not needed if number is taken
                    // from SIM automatically
                    if ( UserEditNumber( alsline ) || 
                            IsPhoneMemForcedEdit( alsline ) )
                        {
                        ResetUserEditNumber( alsline );
                        ShowConfirmationNoteL( R_SIM_NUMBER_NOTE );
                        }
                    }
                else
                    {
                    // confirmation note is not needed if number is taken
                    // from SIM automatically
                    if ( UserEditNumber( alsline ) || 
                            IsPhoneMemForcedEdit( alsline ) )
                        {
                        ShowConfirmationNoteL( R_SIM_NUMBER_NOTE );
                        }
                    }
                aNumber.Copy( vmbxNumber );
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL Number from \
                    Sim accepted" );
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: <=" );
                return ETrue;
                }
            }
        }

    TBuf<KVmOriginalNumLength> originalNumber;
    originalNumber.Copy( aNumber );

    iQueryingState = EVmQueryProcess;
    //QueryNumberL dialog
    FOREVER
        {
        vmbxNumber.Copy( aNumber );

        // get the correct query prompt from resource file
        HBufC* prompt = StringLoader::LoadLC( ( aQueryType ==
                                    EVmbxNotDefinedQuery
                                    ? R_DEFINE_NUMBER_PROMPT
                                    : R_CHANGE_NUMBER_PROMPT ),
                                                 iCoeEnv );

        VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL string loaded" );
        iClosedPtr = &thisClosed;

        // Convert output to accommodate the current locale
        AknTextUtils::LanguageSpecificNumberConversion( vmbxNumber );

        TInt ret;  // return value of the query

        // ETrue to open the query again after closing single fetch dialog
        TBool searchUsed;

        // enable search if number length is 0
        TBool searchEnabled( vmbxNumber.Length() ? EFalse : ETrue );

        do
            {
            // show query with the correct prompt
            iQuery = CVmTextQueryDialog::NewL( vmbxNumber,
                                                *prompt, searchEnabled );

            VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL ExecuteLD" );
            ret = iQuery->ExecuteLD( R_VMBX_NUMBER_QUERY );
            VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL ExecuteLD done" );

            iQuery = NULL;
            searchUsed = EFalse;

            if ( searchEnabled && ret && !vmbxNumber.Length() )
                {
                searchUsed = ETrue;
                // Show phonebook's single fetch dialog
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL Fetch from Pb" );
                TRAPD( error, FetchNumberFromPhonebook2L( vmbxNumber ) );
                VMBLOGSTRING2( "VMBX: RVmbxNumber::QueryNumberL Fetch from Pb done \
                error = %d", error );

                // If application is killed via FSW, contact selection query
                // returns KLeaveExit. This should propagate to "root"
                // application where it reaches the active scheduler loop 
                // and thus allows proper application exit when the loop exits.
                // Therefore propagate the error to caller and do not ignore it.
                if ( KLeaveExit == error )
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL KLeaveExit" );
                    User::Leave( error );
                    }

                // user has pressed End Key
                if ( thisClosed )
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: \
                    End key pressed" );
                    CleanupStack::PopAndDestroy( prompt );

                    iQueryingState = EVmQueryNormal;
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: <=" );
                    return EFalse;
                    }

                if ( error )   //  Back pressed in Contact UI
                    {
                    // Now the phone book fetch dialog was closed,
                    // get out of the loop
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL Fetch done" );
                    ret = 0;
                    break;
                    }
                }
            } while ( searchUsed );

        iClosedPtr = NULL;

        CleanupStack::PopAndDestroy( prompt );

        if ( ret != KErrNone )
            {
            if ( !vmbxNumber.Length() )
                {
                iFlags &= (~KForceSavedToPhoneMem);
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL has number" );
                // Resets the flag that indicates that the user has edited VMBX number
                ResetUserEditNumber( alsline );
                TInt status =
                    SaveVmbxNumber( vmbxNumber, EAlsActiveLineEntry );
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL number save" );

                if ( status == KErrNone )
                    {
                    // If number does not exist,
                    // number from SIM is used if it's available
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL save ok" );
                    if ( UseSimNumIfAvailable( vmbxNumber, alsline )
                        && vmbxNumber.Length() )
                        {
                        // If number is taken from SIM,
                        // show appropriate confirmation note
                        ShowConfirmationNoteL( R_SIM_NUMBER_NOTE );
                        }
                    aNumber.Copy( vmbxNumber );
                    if ( !vmbxNumber.Length() )
                        {
                        // show "not defined" note
                        ShowConfirmationNoteL( R_NOTE_EMPTY_NUMBER );
                        }
                    result = ETrue;
                    break;
                    }
                else if ( status == KErrCancel )
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL save \
                        cancel" );

                    aNumber.Copy( vmbxNumber );
                    continue;
                    }
                else
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: \
                    Number saving error" );
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: <=" );
                    User::LeaveIfError( status );
                    }
                }
            else
                {
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL no number" );
                SetUserEditNumber( alsline );
                }

            // Convert back to western digits
            ConvertToWesternDigits( vmbxNumber );

            // check the given number here (emergency numbers not allowed)
            if ( IsValidPhoneNumber( vmbxNumber )
                        && !IsEmergencyNumberL( vmbxNumber ) )
                {
                iFlags &= ~KForceSavedToPhoneMem;
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: save num 2" );
                TInt status = SaveVmbxNumber( vmbxNumber, EAlsActiveLineEntry );

                if ( status == KErrNone )
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: save \
                        num 2 ok" );
                    aNumber.Copy( vmbxNumber );
                    if ( UsesSimMemory() )
                        {
                        ShowConfirmationNoteL(
                                            R_SAVED_NOTE_SIM_MEMORY );
                        }
                    else
                        {
                        ShowConfirmationNoteL(
                                           R_SAVED_NOTE_PHONE_MEMORY );
                        }
                    result = ETrue;
                    break;
                    }
                else if ( status == KErrCancel )
                    {// The query number dialog was cancelled, so continue
                    aNumber.Copy( vmbxNumber );
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: save \
                        num 2 cancel" );
                    continue;
                    }
                else if ( status == KErrArgument )
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: save \
                        num 2 err arg" );
                    result = EFalse;
                    }
                else
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: \
                    Invalid number" );
                    result = EFalse;
                    }
                }

            if ( !result )
                {
                // show "invalid number" note and wait
                // for a short timeout
                VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: invalid \
                    number show note" );

                prompt = StringLoader::LoadLC( R_INVALID_NUMBER_NOTE,
                                                     iCoeEnv );

                iClosedPtr = &thisClosed;

                CAknInformationNote* note =
                                new( ELeave ) CAknInformationNote( ETrue );
                note->ExecuteLD( *prompt );

                iClosedPtr = NULL;

                CleanupStack::PopAndDestroy( prompt );

                if ( !thisClosed )
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: invalid \
                    number query again" );
                    continue;  // the number was invalid -> query again.
                    }
                }
            }

            // retrieve the original number in the query
            // as user has cancelled the query...
            aNumber.Copy( originalNumber );
            break;  // user canceled the query
        }
    iQueryingState = EVmQueryNormal;
    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryNumberL: <=" );
    return result;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::QuerySimNumberL
// Routine to query and save SIM number from user
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::QuerySimNumberL( TDes& aNumber, TVmbxNumberEntry aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::QuerySimNumberL: =>" );
    TInt lineNumber( aEntry );

    if ( aEntry == EAlsActiveLineEntry )
        {
        // get the current ALS line
        if ( GetAlsLine( lineNumber ) != KErrNone )
            {
            lineNumber = EAlsLine1;  // if problems, assume primary line
            }
        }
    TInt ret( KErrNotFound );

    ret = GetVmNumFromSIM( aNumber, lineNumber );

    if ( ret )
        {
        return EFalse;
        }

    VMBLOGSTRING2( "iMemoryLocation = %d", iMemoryLocation );

    TBool doSimQuery( EFalse );
    if ( ( lineNumber == EAlsLine2 ) && ( iPhoneBookType == EVMBXPhoneBook ) )
        {
        VMBLOGSTRING( "Sim query allowed on line2" );
        doSimQuery = ETrue;
        }
    else if ( ( iMemoryLocation == EVmbxPhoneMemory ) &&
              ( 0 != aNumber.Length() ) )
        {
        VMBLOGSTRING( "Sim query allowed on line1" );
        doSimQuery = ETrue;
        }

    if ( doSimQuery )
        {
        TInt retVal = EVmbxSimNumNotAccepted;
        // If number not defined, query is not needed as number
        // found in SIM is used automatically
        if ( VmbxNumDefInPhoneMemory( lineNumber ) )
            {
            // If number found in SIM is the same as the number stored in phone memory,
            // skip query
            if ( !NumStoredInPhone( aNumber, lineNumber ) )
                {
                HBufC* label = StringLoader::LoadLC( R_SAVED_SIM_NUMBER_INTO_USE,
                                                                    aNumber );
                iVMSimQueryDialog = CVMSimQueryDilaog::NewL();
                iVMSimQueryDialog->SetVMSimPromptL( *label );
                CleanupStack::PopAndDestroy( label );
                //coverity static analysis tool generates a warning finding here
                //ignore that
                //coverity[deref_ptr_in_call]
                retVal= iVMSimQueryDialog->ExecuteLD(
                                              R_VMBX_TAKE_SIM_NUMBER_INTO_USE );
                //coverity static analysis tool generates a warning finding here
                //ignore that
                //coverity[check_after_deref] 
                if ( iVMSimQueryDialog == NULL )
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QuerySimNumberL: \
                    No number inserted" );
                    return EFalse;
                    }
                iVMSimQueryDialog = NULL;
                if ( retVal )
                    {
                    SetUserEditNumber( lineNumber );
                    }
                }
            }
        else
            {
            // IF VMBX number not defined and number found in SIM,
            // number from SIM is used
            retVal = EVmbxSimNumAccepted;
            ResetUserEditNumber( lineNumber );
            }

        // sim number accepted on line 1
        if ( retVal && ( lineNumber == EAlsLine1 ) )
            {
            VMBLOGSTRING( "sim number accepted on line 1" );
            // No write support on sim, use phone memory
            if ( !( iFlags & KVmFlagSimWriteSupport ) )
                {
                iSession->Set( KVmUsesSimMemory, EVmbxPhoneMemory );
                iMemoryLocation = EVmbxPhoneMemory;
                }
            else
                {
                iSession->Set( KVmUsesSimMemory, EVmbxSimMemory );
                iMemoryLocation = EVmbxSimMemory;
                }
            VMBLOGSTRING( "return ETrue" );
            return ETrue;
            }
        // sim number accepted on line 2
        else if ( retVal && ( lineNumber == EAlsLine2 ) )
            {
            VMBLOGSTRING( "Sim number accepted on line2" );
            return ETrue;
            }
        // user selection "No"
        else
            {
            VMBLOGSTRING( "VMBX: RVmbxNumber::QuerySimNumberL: \
            No number inserted" );
            return EFalse;
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::QuerySimNumberL: <=" );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::ConvertToWesternDigits
// Converts a phone number to western format
// -----------------------------------------------------------------------------
//
void RVmbxNumber::ConvertToWesternDigits( TDes& aDes ) const
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::ConvertToWesternDigits: =>" );
    AknTextUtils::ConvertDigitsTo( aDes, EDigitTypeWestern );
    VMBLOGSTRING( "VMBX: RVmbxNumber::ConvertToWesternDigits: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SaveVmbxNumber
// Saves vmbx number to phone or sim memory
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
EXPORT_C TInt RVmbxNumber::SaveVmbxNumber( const TDesC& aNumber,
                                                    TVmbxNumberEntry aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumber: =>" );
    TInt status( KErrNone );
    TInt alsline( EAlsLine1 );
    // get the current ALS line
    if ( GetAlsLine( alsline ) != KErrNone )
        {
        alsline = EAlsLine1;  // if problems, assume primary line
        }

    if ( EAlsLine1 == alsline)
        {
        // Ask place to save if sim available for saving
        if ( iFlags & KVmFlagSimWriteSupport )
            {
            TInt index( 0 );
            TInt ret( 0 );

            CAknListQueryDialog* dlg( NULL );
            TRAP_IGNORE( dlg = new( ELeave ) CAknListQueryDialog( &index ) );

            if ( dlg )
                {
                TRAP_IGNORE( ret = dlg->ExecuteLD( R_VMBX_MEMORY_LIST_QUERY ) );
                dlg = NULL;
                }

            if ( ret )
                {
                // user selected to save number to Sim
                if ( EVmbxSimMemory == index )
                    {
                    iMemoryLocation = EVmbxSimMemory;
                    iSession->Set( KVmUsesSimMemory, EVmbxSimMemory );

                    status = SaveVmbxNumberToSim( aNumber, aEntry );
                    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumber: \
                    Number saved to Sim" );
                    }
                // user selected to save number to Phone memory
                else if ( EVmbxPhoneMemory == index )
                    {
                    iMemoryLocation = EVmbxPhoneMemory;
                    iSession->Set( KVmUsesSimMemory, EVmbxPhoneMemory );
                    if ( iFlags & KForceSavedToPhoneMem )
                        {
                        SetPhoneMemForcedEdit( alsline );
                        }

                    status = SaveVmbxNumberToPhone( aNumber, aEntry );
                    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumber: \
                    Number saved to phone memory" );
                    }
                }
            else// The user has selected Cancel.. so no changes, return KErrCancel
                {
                VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumber: canceled" );
                status = KErrCancel;
                }
            }
        // Line1 in use and no sim available, so use Phone memory
        else
            {
            // Sim write not support and external clients invoke the function
            // set phone memory forced edit
            if ( iFlags & KForceSavedToPhoneMem )
                {
                SetPhoneMemForcedEdit( alsline );
                }
            iMemoryLocation = EVmbxPhoneMemory;
            iSession->Set( KVmUsesSimMemory, EVmbxPhoneMemory );
            status = SaveVmbxNumberToPhone( aNumber, EAlsActiveLineEntry );
            VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumber: \
            Number saved to phone memory" );
            }
        }
    // EAlsLine2
    else
        {
        // Sim write not support and external clients invoke the function
        // set phone memory forced edit
        if ( ( iFlags & KForceSavedToPhoneMem ) &&
                ! ( iFlags & KVmFlagSimWriteSupport ) )
            {
            SetPhoneMemForcedEdit( alsline );
            }
        status = SaveVmbxNumberToPhone( aNumber, EAlsActiveLineEntry );
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumber: <=" );
    return status;
    }


// -----------------------------------------------------------------------------
// RVmbxNumber::SaveVmbxNumberToSim
// Saves vmbx number to sim memory
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
TInt RVmbxNumber::SaveVmbxNumberToSim( const TDesC& aNumber,
                                                     TVmbxNumberEntry aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumberToSim: =>" );

    // AllowedToChangeTheNumber() check not done in this method 
    // in order to always allow saving.
    TInt alsline( EAlsLine1 );
    // get the current ALS line
    if ( GetAlsLine( alsline ) != KErrNone )
        {
        alsline = EAlsLine1;  // if problems, assume primary line
        }
    // ALS line2 not supported when saving to SIM
    if ( alsline == EAlsLine2 )
        {
        VMBLOGSTRING( "Sim usage for ALS Line2 not supported, \
         return KErrArgument " );
        return KErrArgument;
        }

    // number containing "w"-character not allowed to save to sim
    if ( aNumber.Find( KVmbxIllegalSimCharacter ) != KErrNotFound )
        {
        VMBLOGSTRING( "w-char not allowed, return KErrArgument " );
        return KErrArgument;
        }

    __ASSERT_DEBUG( iFlags & KVmFlagOpened,
                         User::Panic( KVmbxPanicCategory, EVmbxNotConnected ) );
    __ASSERT_DEBUG( aNumber.Length() <= KVmbxMaxNumberLength,
                        User::Panic( KVmbxPanicCategory, EVmbxNumberTooLong ) );

    TInt lineNumber( aEntry );
    TInt error( KErrNone );

    if ( aEntry == EAlsActiveLineEntry )
        {
        // get the active ALS line
        lineNumber = alsline;
        }

#ifdef VM_USE_TSY
        // Update maxLength info
        // First read number from sim.
        // Then get info of the max size.
        // Finally determine actual max size using retrieved info.
        // Note! GetInfo reads data from cache, which is updated when write
        // attempt is done. So if Ext-files are taken into use by some
        // other application while Vmbx is opened, then correct info
        // cannot fetch by GetInfo. (This appears when trying to save
        // long number twice and first one fails and second is saved
        // successfully.)
        TBuf<KVmbxMaxNumberLength> numberOnSim;
        // return value ignored here, because no meaning for size determination
        GetVmNumFromSIM( numberOnSim, lineNumber );

        TInt ret = DoPhonebookOperation( EVmbxPhonebookGetInfo );

        if ( ret != KErrNone )
            {
            VMBLOGSTRING2( "DoPhonebookOperation getinfo retval = %I", ret );
            return ret;
            }
        else
            {
            CalculateActualMaxLengthOnSim( numberOnSim.Length() );
            }

    // store in SIM phonebook
    if ( iVmNumOpts == EVmNumOptsBlank )
        {
        if ( aNumber.Length() <= iPhoneBookInfo.iMaxNumLength )
            {
            VMBLOGSTRING( "SaveVmbxNumberToSim::Num length <= maxNumLength" );
            if ( iFlags & KVmFlagPhonebookBeingCached )
                {
                error = DoPhonebookOperation( EVmbxPhonebookGetInfo );
                }

            if ( !error && ( iFlags & KVmFlagSimWriteSupport ) )
                {
                TVmbxEntry entry;
                entry.iIndex = lineNumber;

                if ( aNumber.Length() )
                    {
                    entry.iTelNumber.Copy( aNumber );
                    entry.iText.Copy( KVmbxPhoneBookEntryName );
                    entry.iText.AppendNum( lineNumber );
                    error = DoPhonebookOperation( EVmbxPhonebookWrite,
                                                                     &entry );
                    VMBLOGSTRING3( "VMBX: RVmbxNumber::SaveVmbxNumberToSim: \
                     Write SIM entry %I, status: %I", lineNumber, error );
                    }
                else
                    {
                    error = DoPhonebookOperation( EVmbxPhonebookDelete,
                                                                     &entry );
                    VMBLOGSTRING3( "VMBX: RVmbxNumber::SaveVmbxNumberToSim: \
                    Delete SIM entry %I, status: %I", lineNumber, error );
                    }
                }
            }
        else
            {
            error = KErrArgument;
            }
        }
    else
        {
        error = KErrNotFound;
        }
#else // <= VM_USE_TSY
    error = KErrNotFound;
#endif  // VM_USE_TSY
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumberToSim: <=" );
    return error; //to be removed
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SaveVmbxNumberToPhone()
// Saves vmbx number to phone memory
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
TInt RVmbxNumber::SaveVmbxNumberToPhone( const TDesC& aNumber,
                                                     TVmbxNumberEntry aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumberToPhone: =>" );

    __ASSERT_DEBUG( iFlags & KVmFlagOpened,
                         User::Panic( KVmbxPanicCategory, EVmbxNotConnected ) );
    __ASSERT_DEBUG( aNumber.Length() <= KVmbxMaxNumberLength,
                        User::Panic( KVmbxPanicCategory, EVmbxNumberTooLong ) );

    // AllowedToChangeTheNumber() check not done in this method 
    // in order to always allow saving.
    TInt lineNumber( aEntry );
    TInt error( KErrNone );
    VMBLOGSTRING2( "VMBX: RVmbxNumber::SaveVmbxNumberToPhone \
    the number is from phone or SIM aNumber = %S", &aNumber );

    if ( aEntry == EAlsActiveLineEntry )
        {
        // get the current ALS line
        if ( GetAlsLine( lineNumber ) != KErrNone )
            {
            lineNumber = EAlsLine1;  // if problems, assume primary line
            }
        }

    TUint32 keyword = lineNumber ==
            EAlsLine1Entry ? KVmbxNumberLinePrimary: KVmbxNumberLineAuxiliary;
    error = iSession->Set( keyword, aNumber );

    if ( !error )
        {
        // Client will generate the event itself on request.
        if ( iFlags & KVmFlagNotifyRequested )
            {
            HandleNotifyString( keyword, aNumber );
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxNumberToPhone: <=" );
    return error;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::NotifyVmbxNumberChangeL
// Issues a notify request on a vmbx number change
// -----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::NotifyVmbxNumberChangeL(
                   MVmbxNotifyHandler* aHandler, TBool aNotifyOnActiveLineOnly )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyVmbxNumberChangeL: =>" );
    __ASSERT_DEBUG( iFlags & ( KVmFlagOpened ),
                        User::Panic( KVmbxPanicCategory, EVmbxNotConnected ) );

    iNotifyCallBack = aHandler;
    SubscribeSATNotificationsL(); // subscribe SAT notifications

    // only issue a new notify request if there isn't one already
    if ( !( iFlags & KVmFlagNotifyRequested ) )
        {
        iNotifyHandler = CCenRepNotifyHandler::NewL( *this, *iSession );
        iNotifyHandler->StartListeningL();

        iFlags |= KVmFlagNotifyRequested;
        }

    if ( aNotifyOnActiveLineOnly )
        {
        iFlags |= KVmFlagNotifyOnActiveLineOnly;
        }
    else
        {
        iFlags &= ( ~KVmFlagNotifyOnActiveLineOnly );
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyVmbxNumberChangeL: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SubscribeSATNotificationsL
// Subscribe SAT notifications
// -----------------------------------------------------------------------------
void RVmbxNumber::SubscribeSATNotificationsL()
    {
    if ( iPhoneBookType == EMBDNPhoneBook )
        {
        iRSatSession.ConnectL();
        iRSatClient.OpenL( iRSatSession );
        iObservedFileList.Append( KMbdnEf );
        iObservedFileList.Append( KMbiEf );
        iRSatClient.NotifyFileChangeL( iObservedFileList );
        }
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::NotifyVmbxNumberChangeCancel
// Cancels a notify request on a vmbx number change.
// -----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::NotifyVmbxNumberChangeCancel()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyVmbxNumberChangeCancel: =>" );
    __ASSERT_DEBUG( iFlags & KVmFlagOpened,
                     User::Panic( KVmbxPanicCategory, EVmbxNotConnected ) );

    iNotifyHandler->StopListening();

    iFlags &= ( ~KVmFlagNotifyRequested );
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyVmbxNumberChangeCancel: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::NotifyAlsLineChangeL
// Issues a notify request on active ALS line change
// -----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::NotifyAlsLineChangeL( MVmbxAlsObserver* aObserver )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyAlsLineChangeL: =>" );
    __ASSERT_DEBUG( iFlags & KVmFlagOpened,
             User::Panic( KVmbxPanicCategory, EVmbxNotConnected ) );
    iUserAlsObserver = aObserver;
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyAlsLineChangeL: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::NotifyAlsLineChangeCancel
// Cancels a notify request on ALS line change
// -----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::NotifyAlsLineChangeCancel()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyAlsLineChangeCancel: =>" );
    __ASSERT_DEBUG( iFlags & KVmFlagOpened,
                     User::Panic(KVmbxPanicCategory, EVmbxNotConnected) );
    iUserAlsObserver = NULL;
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyAlsLineChangeCancel: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::Close
// Closes the resources allocated in Open
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
EXPORT_C void RVmbxNumber::Close()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::Close: =>" );
    if ( iCntFetch )
        {
        delete iCntFetch;
        iCntFetch = NULL;
        }

    iRSatClient.Cancel();
    iRSatClient.Close();
    iRSatSession.Close();
    if ( iClosedPtr )
        {
        *iClosedPtr = ETrue;
        iClosedPtr = NULL;
        }

    if ( iVMSimQueryDialog )
        {
        TRAP_IGNORE( iVMSimQueryDialog->VMSimDismissDialogL() );
        iVMSimQueryDialog = NULL;
        }

    if ( iQuery )
        {
        delete iQuery;
        iQuery = NULL;
        }

    if ( iVideoQuery )
        {
        delete iVideoQuery;
        iVideoQuery = NULL;
        }
    // Type selection query
    if ( iTypeSelectionQuery )
        {
        delete iTypeSelectionQuery;
        iTypeSelectionQuery = NULL;
        }

    iVmbxServiceSelection.Close();
    if ( iResourceFileOffset )
        {
        iCoeEnv->DeleteResourceFile( iResourceFileOffset );
        iResourceFileOffset = 0;
        }

    CloseETel();

    iUserAlsObserver = NULL;

    // these automatically cancel all observations as well

    iSsSettings.Close();

    iFlags = 0;
    VMBLOGSTRING( "VMBX: RVmbxNumber::Close: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::LoadResourceL
// Loads the resources needed for queries and confirmation notes
// -----------------------------------------------------------------------------
//
void RVmbxNumber::LoadResourceL()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::LoadResourceL: =>" );
    TFileName fileName;
    fileName += KVmbxResourceFileDrive;
    fileName += KDC_RESOURCE_FILES_DIR;
    fileName += KVmbxResourceFileName;
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );
    iResourceFileOffset = iCoeEnv->AddResourceFileL( fileName );
    VMBLOGSTRING( "VMBX: RVmbxNumber::LoadResourceL: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::AllowedToChangeTheNumber
// Check wether the voice mailbox number can be changed
// -----------------------------------------------------------------------------
//
EXPORT_C TBool RVmbxNumber::AllowedToChangeTheNumber()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::AllowedToChangeTheNumber: =>" );
    TBool ret( ETrue ); 

    if ( EUiNmbrChangeNotAllowed & iVmUiOpts )
        {
        ret = EFalse;
        }
    VMBLOGSTRING2( "RVmbxNumber::AllowedToChangeTheNumber \
    iVmUiOpts = %M", iVmUiOpts );
    VMBLOGSTRING( "VMBX: RVmbxNumber::AllowedToChangeTheNumber: <=" );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::HandleNotifyString
// To notify the client about changes for string value keys
// -----------------------------------------------------------------------------
//
void RVmbxNumber::HandleNotifyString( TUint32 aKey, const TDesC16& aValue )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNotifyString: =>" );
    TInt alsline( EAlsLine1 );

    // get the current ALS line
    if ( GetAlsLine( alsline ) != KErrNone )
        {
        alsline = EAlsLine1;  // if problems, assume primary line
        }
    if ( iNotifyCallBack )
        {
        TInt notifyLine( 0 );

        if ( aKey == KVmbxNumberLinePrimary )
            {
            notifyLine = EAlsLine1Entry;
            }
        else if ( aKey == KVmbxNumberLineAuxiliary )
            {
            notifyLine = EAlsLine2Entry;
            }

        if ( ( iFlags & KVmFlagNotifyOnActiveLineOnly ) && alsline != notifyLine )
            {
            VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNotifyString: \
            Line not notifyline" );
            return;
            }

        if ( notifyLine == EAlsLine1Entry || notifyLine == EAlsLine2Entry )
            {
            TRAPD( err, iNotifyCallBack->HandleNotifyL( notifyLine, aValue ) );
            if ( err != KErrNone )
                {
                VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNotifyString: \
                Notify handle error" );
                _LIT( KVmbxPanicType,"iNotifyCallBack->HandleNotifyL" );
                User::Panic( KVmbxPanicType, err );
                }
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNotifyString: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::HandleNotifyGeneric
// To notify the client about changes in keys when the whole repository
// is listened for.
// -----------------------------------------------------------------------------
//
void RVmbxNumber::HandleNotifyGeneric( TUint32 aKey )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNotifyGeneric: =>" );
    if ( aKey == NCentralRepositoryConstants::KInvalidNotificationId )
        {

        // We have to check which keys have changed.
        }
    else
        {

        //Check the key value of the particular key being changed and
        //do further processing.
        }

    if ( iQuery )
        {
        // if the vmbx number is saved,notify to dismiss the query.
        iQuery->DismissQueryDlg();
        }
    if ( iVideoQuery )
        {
        // if the video number is saved,notify to dismiss the query.
        iVideoQuery->DismissQueryDlg();
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNotifyGeneric: <=" );
    }


// -----------------------------------------------------------------------------
// RVmbxNumber::PhoneSettingChanged
// To notify changes in SS settings in phone
// -----------------------------------------------------------------------------
//
void RVmbxNumber::PhoneSettingChanged( TSSSettingsSetting aSetting,
                                                             TInt aNewValue )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhoneSettingChanged: =>" );
    TInt alsline( EAlsLine1 );
    if ( aSetting == ESSSettingsAls )
        {
        alsline = ( aNewValue ==
                ESSSettingsAlsAlternate ? EAlsLine2Entry : EAlsLine1Entry );

        if ( iUserAlsObserver )
            {
            TRAP_IGNORE( iUserAlsObserver->HandleAlsChangeL(
                                                    ( TVmbxAlsLine ) alsline ) );
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhoneSettingChanged: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetAlsLine
// Fetches ALS line from SS settings
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::GetAlsLine( TInt& aLineNumber ) const
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetAlsLine: =>" );
    aLineNumber = EAlsLine1;

    TInt alsline( ESSSettingsAlsPrimary );

    TInt ret = CONST_CAST( RVmbxNumber*, this )->iSsSettings.Get(
                                                        ESSSettingsAls, alsline );

    if ( alsline == ESSSettingsAlsAlternate )
        {
        aLineNumber = EAlsLine2;
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetAlsLine: <=" );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::ShowConfirmationNoteL
// Shows confirmation note to user
// -----------------------------------------------------------------------------
//
void RVmbxNumber::ShowConfirmationNoteL( TInt aResourceId ) const
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::ShowConfirmationNoteL: =>" );
    HBufC* prompt = StringLoader::LoadLC( aResourceId, iCoeEnv );

    CAknConfirmationNote* note = new( ELeave ) CAknConfirmationNote( ETrue );

    note->ExecuteLD( *prompt );

    CleanupStack::PopAndDestroy( prompt );
    VMBLOGSTRING( "VMBX: RVmbxNumber::ShowConfirmationNoteL: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::IsValidPhoneNumber
// Validates phone number
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::IsValidPhoneNumber( const TDesC& aNumber ) const
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsValidPhoneNumber: =>" );
    TBool isValid( EFalse );

    if ( aNumber.Length() >= KVmPhNumMinLength )
        {
        TInt index( 0 );
        TInt digits( 0 );

        // Skip the possible '*'s and '#'s in the beginning.
        while ( index < aNumber.Length() &&
                ( aNumber[ index ] == '#' || aNumber[ index ] == '*' ||
                    aNumber[ index ] == ' ') )
            {
            index++;
            }

        // Skip the possible '+'.
        if ( index < aNumber.Length() && aNumber[ index ] == '+' )
            {
            index++;
            }

        // Count the digits.
        // For this let's use another index so that the following
        // operation will start from the beginning of the actual
        // vmbx number
        TInt loopindex = index;

        while ( loopindex < aNumber.Length() &&
                ( aNumber[ loopindex ] >= '0' && aNumber[ loopindex ] <= '9'
                    || aNumber[ loopindex ] == ' ') )
            {
            if ( aNumber[ loopindex ] != ' ' )
                {
                digits++;
                }
            loopindex++;
            }

        // Check that there are only allowed characters in the remaining string.
        while ( index < aNumber.Length() )
            {
            TText t = aNumber[ index ];

            if ( (t < '0' && t > '9') || t == ' ' )
                {
                break;
                }
            index++;
            }

        // If only legal characters were encountered, index is now same as
        // the length of the string.

        // phone number part must have 2...41 digits
        // '+' is only allowed as the first character in the string
        if ( digits >= KVmPhoneNumDigitsMinLength &&
                digits <= KVmPhoneNumDigitsMaxLength &&
                    index == aNumber.Length())
            {
            isValid = ETrue;
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsValidPhoneNumber: <=" );
    return isValid;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::IsEmergencyNumberL
// Checks if a number is an emergency number.
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::IsEmergencyNumberL( const TDesC& aNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsEmergencyNumberL: =>" );
    TBool isEmergency = EFalse;

    // Emergency Call API does not strip invalid characters like Call UI does, so it is done manually here.
    TPhCltTelephoneNumber number( aNumber.Left( KPhCltTelephoneNumberLength ) );
    PhCltUtils::RemoveInvalidChars( number );

    CPhCltEmergencyCall* emergencyCall = CPhCltEmergencyCall::NewL( this );

    TInt result = emergencyCall->IsEmergencyPhoneNumber( number, isEmergency );
    delete emergencyCall;
    if ( result != KErrNone )
        {
        return EFalse;
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsEmergencyNumberL: <=" );
    return isEmergency;
    }

// ---------------------------------------------------------
//  RVmbxNumber::HandleEmergencyDialL()
// ---------------------------------------------------------
//
void RVmbxNumber::HandleEmergencyDialL( const TInt /*aStatus*/ )
    {
    // No action
    }


// -----------------------------------------------------------------------------
// RVmbxNumber::PhoneBookInfo
// Fetches information about the SIM phonebook for vmbx numbers
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
TInt RVmbxNumber::PhoneBookInfo()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhoneBookInfo: =>" );
    TRequestStatus status( KErrNone );
    TInt alsline( EAlsLine1 );

    // get the current ALS line
    if ( GetAlsLine( alsline ) != KErrNone )
        {
        alsline = EAlsLine1;  // if problems, assume primary line
        }

    // This may return KErrInUse if phonebook is being cached
    iPhoneBook.GetInfo( status, iPhoneBookInfoPckg );
    User::WaitForRequest( status );  // Wait for asynchronous call to finish

    VMBLOGSTRING2( "VMBX: RVmbxNumber::PhoneBookInfo: \
    SIM Phonebook info read, status: %I", status.Int() );
    VMBLOGSTRING2( "VMBX: RVmbxNumber::PhoneBookInfo: \
    SIM Phonebook write access = %I",
    iPhoneBookInfo.iCaps &  RMobilePhoneBookStore::KCapsWriteAccess ? 1 : 0);
    iFlags &= ~KVmFlagPhonebookBeingCached;
    iFlags &= ~KVmFlagSimVmbxNumSupport;

    VMBLOGSTRING2( "MaxNum length from sim: %I", iPhoneBookInfo.iMaxNumLength );
    if ( status == KErrInUse || status == KErrNotReady )
        {
        iFlags |= KVmFlagPhonebookBeingCached;
        VMBLOGSTRING( "Phonebook Being Cached" );
        }
    else if ( status == KErrNone )
        {
        //If read access, only then write access possible
        if ( iPhoneBookInfo.iCaps & RMobilePhoneBookStore::KCapsReadAccess )
            {
            VMBLOGSTRING( "ReadAccess ok" );
            iFlags |= KVmFlagSimReadSupport;

            if ( iPhoneBookInfo.iCaps & RMobilePhoneBookStore::KCapsWriteAccess )
                {
                VMBLOGSTRING( "WriteAccess ok" );
                iFlags |= KVmFlagSimWriteSupport;
                iFlags |= KVmFlagSimVmbxNumSupport;
                }
            // Sim access rights are tested only in start up
            if ( iStartUpTest )
                {
                TestAndSetSimAccessFlags();
                }
            }
        //As there is no sim support set the memory to phone memory
        else
            {
            // No write capability and line1 in use
            if ( ( iMemoryLocation != EVmbxPhoneMemory ) &&
                ( alsline == EAlsLine1 ) )
                {
                iMemoryLocation = EVmbxPhoneMemory;
                iSession->Set( KVmUsesSimMemory, EVmbxPhoneMemory );
                }
            iFlags &= ~KVmFlagSimReadSupport;
            }
        }
    // no caps fetched
    else
        {
        if ( ( iMemoryLocation != EVmbxPhoneMemory ) &&
            ( alsline == EAlsLine1 ) )
                {//as there is no sim support set the memory to phone memory
                iMemoryLocation = EVmbxPhoneMemory;
                iSession->Set( KVmUsesSimMemory, EVmbxPhoneMemory );
                }
        iFlags &= ~KVmFlagSimReadSupport;
        VMBLOGSTRING( "USim not supported" );
        status = KErrNone;
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhoneBookInfo: <=" );
    return status.Int();
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::IsReadOnlySIM
// Checks if writing to sim is prohibited
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::IsReadOnlySIM()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsReadOnlySIM. =>" );

    TBool readOnlySIM = EFalse;
    iSession->Get( KReadOnlySim, readOnlySIM );
    VMBLOGSTRING2( "VMBX: RVmbxNumber::IsReadOnlySIM: \
        read only flag = %I", readOnlySIM );
    return readOnlySIM;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::DoPhonebookOperation
// Does a vmbx phonebook operation
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::DoPhonebookOperation( TVmbxPhonebookOperation aOperation,
                                                TVmbxEntry* aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::DoPhonebookOperation: =>" );
    TInt result( KErrNone );
    TInt retryCounter( KVmbxPhonebookRetriesIfInUse );

    FOREVER
        {
        retryCounter--;

        switch ( aOperation )
            {
            case EVmbxPhonebookGetInfo:
                {
                result = PhoneBookInfo();
                break;
                }
            case EVmbxPhonebookRead:
                {
                TRAPD( readErr, PhonebookReadL( *aEntry ) );
                if ( readErr != KErrNone && readErr != KErrNotFound )
                    {
                    result = KErrNotSupported;
                    }
                break;
                }
            case EVmbxPhonebookWrite:
                {
                result = PhonebookWrite( *aEntry );
                break;
                }
            case EVmbxPhonebookDelete:
                {
                result = PhonebookDelete( *aEntry );
                break;
                }
            default:
                {
                __ASSERT_DEBUG( 0, User::Panic( KVmbxPanicCategory,
                                                 EVmbxUnhandledOperation) );
                break;
                }
            }

        if ( result != KErrInUse || retryCounter <= 0 )
            {
            break;
            }

        iFlags |= KVmFlagPhonebookBeingCached;
        User::After( KVmbxPhonebookRetryDelay );
        }
    // set readonly state if aquired
    if ( IsReadOnlySIM() )
        {
        iFlags &= ~KVmFlagSimWriteSupport;
        VMBLOGSTRING( "VMBX: RVmbxNumber::DoPhonebookOperation: ReadOnly set" );
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::DoPhonebookOperation: <=" );
    return result;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::PhonebookReadL
// Reads an entry from the ICC-based phonebook
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
void RVmbxNumber::PhonebookReadL( TVmbxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookReadL: =>" );
    TRequestStatus status( KErrNone );
    TInt numEntries( 1 );
    TBuf8<KVmbxPhonebookBufferSize> pbData;
    pbData.FillZ();
    pbData.Zero();
    TInt result( KErrNone );

    // Reset variables
    iNumberFound = EFalse;
    if ( iAlphaStringFound )
        {
        delete iAlphaStringFromSIM;
        iAlphaStringFromSIM = NULL;
        }
    iAlphaStringFound = EFalse;
    
    TInt lineNumber( EAlsLine1 );
    if ( KErrNone != GetAlsLine( lineNumber ) )
    	{
        lineNumber = EAlsLine1;
    	}

    if ( ( iPhoneBookType == EMBDNPhoneBook ) && ( EAlsLine1 == lineNumber ) )
        {
        VMBLOGSTRING( "start MBDN PhoneBook read" );
        VMBLOGSTRING2( "VMBX: RVmbxNumber::PhonebookReadL: \
        iPhoneVoicemailInfo.iVoice = %I", iPhoneVoicemailInfo.iVoice );
        iPhoneBook.Read( status, iPhoneVoicemailInfo.iVoice,
                                                        numEntries, pbData );
        }
    else
        {
        // Record#1 in sim is for line1 number and Record#2 in sim is for
        // line2 number so line is used to fetch
        VMBLOGSTRING( "start VMBX PhoneBook read" );
        iPhoneBook.Read( status, aEntry.iIndex, numEntries, pbData );
        }

    User::WaitForRequest( status );  // Wait for asynchronous call to finish
    VMBLOGSTRING2( "PhoneBook read status = %I", status.Int() );

    if ( status == KErrNone   || status == KErrNotFound )
        {
        if ( pbData.Length() )
            {
            // -> Search Tags "ETagPBNumber" and "ETagPBText"
            // and read (decode) them
            TUint8 tagValue(0);
            CPhoneBookBuffer::TPhBkTagType dataType(
                                           CPhoneBookBuffer::EPhBkTypeNoData );

            CPhoneBookBuffer* pbkBuffer = NULL;
            TRAPD( err, pbkBuffer = new(ELeave) CPhoneBookBuffer() );
            if ( err != KErrNone )
                {
                VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookReadL: \
                Phonebook creation error" );
                User::Leave( err );
                }

            pbkBuffer->Set( &pbData );
            pbkBuffer->StartRead();

            // Read first "new-entry-tag"
            result = pbkBuffer->GetTagAndType( tagValue, dataType );

            if ( result != KErrNone || tagValue != RMobilePhoneBookStore::ETagPBNewEntry)
                {
                delete pbkBuffer;
                VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookReadL: \
                Unknown error" );
                User::Leave( KErrUnknown );  // Something wrong in TLV
                }

            // search number and text fields from TLV entry
            result = pbkBuffer->GetTagAndType( tagValue, dataType );

            while ( result == KErrNone && pbkBuffer->RemainingReadLength() > 0 )
                {
                VMBLOGSTRING2("VMBX: RVmbxNumber::PhonebookReadL(): tagValue: %d", tagValue )
                VMBLOGSTRING2("VMBX: RVmbxNumber::PhonebookReadL(): dataType: %d", dataType )

                // Check for text field
                if ( tagValue == RMobilePhoneBookStore::ETagPBText )
                    {
                    // Alpha string field found from TLV entry,
                    // assuming 16bit data
                    TPtrC16 alphaPtrC;
                    result = pbkBuffer->GetValue( alphaPtrC );
                    if ( result == KErrNone )
                        {
                        iAlphaStringFromSIM = alphaPtrC.AllocL();
                        iAlphaStringFound = ETrue;
                        VMBLOGSTRING2("VMBX: RVmbxNumber::PhonebookReadL(): iAlphaStringFromSIM: %S", iAlphaStringFromSIM )
                        }
                    }
                 // Check for number field
                 else if ( tagValue == RMobilePhoneBookStore::ETagPBNumber )
                    {
                    // Number field found from TLV entry, assuming 16bit data
                    TPtrC16 numberPtrC;
                    result = pbkBuffer->GetValue( numberPtrC );
                    iNumberFound = ETrue;
                    VMBLOGSTRING("VMBX: RVmbxNumber::PhonebookReadL(): ETagPBNumber found.");

                    if ( result == KErrNone )
                        {
                        // Error code is returned if number not fit to
                        // local buffer.
                        if ( numberPtrC.Length() > aEntry.iTelNumber.MaxLength() )
                            {
                            delete pbkBuffer;
                            VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookReadL: \
                            Overflow error" );
                            User::Leave( KErrOverflow );
                            }
                        else
                            {
                            // copy number to local buffer
                            aEntry.iTelNumber.Copy( numberPtrC );
                            VMBLOGSTRING2("VMBX: RVmbxNumber::PhonebookReadL(): ETagPBNumber found. Number: %S", &aEntry.iTelNumber );
                            }
                        }
                    }
                 else
                    {
                    // skip field
                    pbkBuffer->SkipValue( dataType );
                    }
                // Read next field type
                result = pbkBuffer->GetTagAndType( tagValue, dataType );
                }

             // Number and Alpha fields not found from TLV entry
             if ( !iAlphaStringFound && !iNumberFound )
                {
                delete pbkBuffer;
                VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookReadL: \
                Not found error" );
                User::Leave( KErrNotFound );
                }
            delete pbkBuffer;
            }
        else
            {
            VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookReadL: Not found error" );
            iNumberFromSim.FillZ();
            iNumberFromSim.Zero();
            User::Leave( KErrNotFound );
            }
        }
    else
        {
        // sim read-only and number fetched by GetPhonebookInfo
        // so return number
        if ( iNumberFromSim.Length() && !( iFlags & KVmFlagSimWriteSupport ) )
            {
            aEntry.iTelNumber.Copy( iNumberFromSim );
            iMemoryLocation = EVmbxPhoneMemory;
            iSession->Set( KVmUsesSimMemory, EVmbxPhoneMemory );
            VMBLOGSTRING2( "Number value = %S", &iNumberFromSim );
            VMBLOGSTRING2( "iMemoryLocation = %d", iMemoryLocation );
            VMBLOGSTRING( "Startuptest ongoing, read only sim, number fetched" );
            }
        else
            {
            User::Leave( status.Int() );
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookReadL: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::PhonebookWrite
// Writes an entry into ICC-based phonebook
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
TInt RVmbxNumber::PhonebookWrite( TVmbxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookWrite: =>" );
    TRequestStatus status( KErrNone );
    TBuf8<KVmbxPhonebookBufferSize> pbData;
    pbData.FillZ();

    CPhoneBookBuffer* pbkBuffer = NULL;
    TRAPD( err, pbkBuffer = new(ELeave) CPhoneBookBuffer() );
    if (err != KErrNone)
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookWrite: \
        Phonebook creation error" );
        return err;
        }

    pbkBuffer->Set( &pbData );

    // New entry
    status = pbkBuffer->AddNewEntryTag();

    // Add index
    // Type of index is TUint16 in Multimode ETel and TInt in old ETel.
    status = pbkBuffer->PutTagAndValue( RMobilePhoneBookStore::ETagPBAdnIndex,
                                                      (TUint16)aEntry.iIndex );

    // Add name if it existed on SIM card
    // Type of ETagPBText is TDes16
    if ( iAlphaStringFound )
        {
        status = pbkBuffer->PutTagAndValue( RMobilePhoneBookStore::ETagPBText,
                                                        *iAlphaStringFromSIM );
        }

    // Add number
    // Type of ETagPBNumber is TDes16
    status = pbkBuffer->PutTagAndValue( RMobilePhoneBookStore::ETagPBNumber,
                                                           aEntry.iTelNumber );
    if ( iPhoneBookType == EMBDNPhoneBook )
        {
        TInt index = iPhoneVoicemailInfo.iVoice;
        VMBLOGSTRING2( "VMBX: RVmbxNumber::PhonebookWrite: \
        iPhoneVoicemailInfo.iVoice index = %I", index );
        iPhoneBook.Write( status, pbData, index );
        VMBLOGSTRING( "Mbdn writing" );
        }
    else
        {
        iPhoneBook.Write( status, pbData, aEntry.iIndex );
        VMBLOGSTRING( "6f17 writing" );
        }
    User::WaitForRequest( status );  // Wait for asynchronous call to finish
    VMBLOGSTRING2( "PhonebookWrite: status=%I",  status.Int() );

    delete pbkBuffer;
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookWrite: <=" );
    return status.Int();
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::PhonebookDelete
// Removes an entry from ICC-based phonebook
// -----------------------------------------------------------------------------
//
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
TInt RVmbxNumber::PhonebookDelete( TVmbxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookDelete: =>" );
    TInt lineNumber( EAlsLine1 );
    if ( KErrNone != GetAlsLine( lineNumber ) )
        {
        lineNumber = EAlsLine1;
        }
    
    TRequestStatus status( KErrNone );
    if ( ( iPhoneBookType == EMBDNPhoneBook ) && ( EAlsLine1 == lineNumber ) )
        {
        TInt index = iPhoneVoicemailInfo.iVoice;
        iPhoneBook.Delete( status, index );
        }
    else
        {
        iPhoneBook.Delete( status, aEntry.iIndex );
        }
    User::WaitForRequest( status );  // Wait for asynchronous call to finish
    VMBLOGSTRING( "VMBX: RVmbxNumber::PhonebookDelete: <=" );
    return status.Int();
    }


// -----------------------------------------------------------------------------
// RVmbxNumber::FetchNumberFromPhonebook2L
// Create and show Phonebook's Single Fetch Dialog
// -----------------------------------------------------------------------------
//
void RVmbxNumber::FetchNumberFromPhonebook2L( TDes& aPhoneNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::FetchNumberFromPhonebook2L: =>" );

    CPhCntFactory* contactFactory = CreateCntFactoryL();
    CleanupStack::PushL( contactFactory );
    iCntFetch = NULL;
    iCntFetch = contactFactory->CreateSingleItemFetchL();
    CPhCntSelection* contactSelect = NULL;

    TRAPD( error, contactSelect = iCntFetch->SelectPhoneNumberLD() );
    // Task swapper close activated
    if ( error != KErrNone )
        {
        contactSelect = NULL;
        iCntFetch = NULL;
        User::Leave( error ); //close application
        }

    // This check is needed because contactselect is NULL in case of user
    // presses back in contact fetch dialog. It will NOT leave.
    if ( contactSelect )
        {
        if ( contactSelect->Number().Length() )
            {
            aPhoneNumber = contactSelect->Number().Left( KVmbxMaxNumberLength );
            }
        delete contactSelect;
        contactSelect = NULL;
        }

    iCntFetch = NULL;
    CleanupStack::PopAndDestroy( contactFactory );

    //for removing special chars from number fetched from phone book
    TText ch = 0;
    TInt len = aPhoneNumber.Length();
    for ( TInt cnt = 0; cnt < len; cnt++ )
        {
        ch = aPhoneNumber[cnt];
        switch ( ch )
            {
            case '(':
            case ')':
            case '-':
            case ' ':
                {
                aPhoneNumber.Delete( cnt, 1 );
                cnt--;
                len = aPhoneNumber.Length();
                break;
                }
            default:
                  break;
            }
        }


    VMBLOGSTRING( "VMBX: RVmbxNumber::FetchNumberFromPhonebook2L: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetVmNumFromSIM
// Retrieves voice mailbox number from SIM/R-UIM
// -----------------------------------------------------------------------------
//
#ifdef VM_USE_TSY
TInt RVmbxNumber::GetVmNumFromSIM( TDes& aNumber, TInt aLineNumber )
#else
TInt RVmbxNumber::GetVmNumFromSIM( TDes& /*aNumber*/, TInt /*aLineNumber*/ )
#endif
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromSIM: =>" );
    TInt ret( KErrNotFound );

#ifdef VM_USE_TSY

    if ( iFlags & KVmFlagPhonebookBeingCached )
        {
        ret = DoPhonebookOperation( EVmbxPhonebookGetInfo );

        if ( ret != KErrNone )
            {
            VMBLOGSTRING2( "DoPhonebookOperation getinfo retval = %I", ret );
            return ret;
            }
        }

    // Now we have SIM support information updated
    if ( iFlags & KVmFlagSimReadSupport )
        {
        TVmbxEntry entry;
        entry.iIndex = aLineNumber;  // 1 or 2
        ret = DoPhonebookOperation( EVmbxPhonebookRead, &entry );
        VMBLOGSTRING3( "VMBX: RVmbxNumber::GetVmNumFromSIM: \
        Read SIM entry %I, status: %I", aLineNumber, ret );

        if ( ret == KErrNone )
            {
            if ( !entry.iTelNumber.Length() )
                {
                ret = KErrNotFound;
                }
            else
                {
                aNumber.Copy( entry.iTelNumber );
                }
            }
        }
    else
        {
        ret = KErrNotSupported;
        }

    if ( IsEmergencyNumberL( aNumber ) )
        {
        aNumber.Zero();
        ret = KErrNotFound;
        }

        VMBLOGSTRING2( "Number = %S", &aNumber );
#endif  // VM_USE_TSY
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromSIM: <=" );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetVmNumFromVMN
// Retrieves voice mailbox number from VMN summary info
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::GetVmNumFromVMN( TDes& aNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromVMN: =>" );
    TBuf<KVmbxMaxNumberLength> number;
    TInt ret( KErrNotFound );
    TInt value = 0;

    ret = iSession->Get( KVmSummaryInfoIsSet, value );

    if ( !ret && value == 1 )
        {
        ret = iSession->Get( KVmCallbackNumber, number );
        if ( !ret )
            {
            if ( number.Length() )
                {
                aNumber.Copy( number );
                }
            else
                {
                ret = KErrNotFound;
                }
            }
        }

    if ( IsEmergencyNumberL( aNumber ) )
        {
        aNumber.Zero();
        ret = KErrNotFound;
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromVMN: <=" );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetVmNumFromNamStore
// Retrieves voice mailbox number from NAM
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::GetVmNumFromNamStore( TDes& aNumber, TInt aNumType )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromNamStore: =>" );
    TRequestStatus ret( KErrNotFound );
    RMobileNamStore namStore;

    ret = namStore.Open( iPhone );
    if ( ret != KErrNone )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromNamStore: \
        NameStore error %I, ret.Int()" );
        return ret.Int();
        }

    RMobileNamStore::TMobileNamStoreInfoV1 namInfo;
    RMobileNamStore::TMobileNamStoreInfoV1Pckg namInfoPckg( namInfo );

    namStore.GetInfo( ret, namInfoPckg );
    User::WaitForRequest( ret );
    if ( ret == KErrNone )
        {
        RMobileNamStore::TMobileNamEntryV1 namEntry;
        RMobileNamStore::TMobileNamEntryV1Pckg namEntryPckg( namEntry );

        namEntry.iNamId = namInfo.iActiveNam;

        switch ( aNumType )
            {
            case EVmbxMDN:
                {
                namEntry.iParamIdentifier = KVmNamStoreMDNId;
                break;
                }
            case EVmbxMIN:
                {
                namEntry.iParamIdentifier = KVmNamStoreMINId;
                break;
                }
            default:
                break;
            }

        namStore.Read( ret, namEntryPckg );
        User::WaitForRequest( ret );
        if ( ret == KErrNone )
            {
            ret = CnvUtfConverter::ConvertToUnicodeFromUtf8(
                aNumber,            // Unicode
                namEntry.iData );   // Utf8
            }
        }

    if ( IsEmergencyNumberL( aNumber ) )
        {
        aNumber.Zero();
        ret = KErrNotFound;
        }

    namStore.Close();
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromNamStore: <=" );
    return ret.Int();
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetVmNumFromSD
// Retrieves voice mailbox number from Shared Data
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::GetVmNumFromSD( TDes& aNumber, TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromSD: =>" );
    TBuf< KVmbxMaxNumberLength > number;
    TInt ret( KErrNotFound );

    TUint32 keyword = aLineNumber ==
                EAlsLine1Entry?KVmbxNumberLinePrimary: KVmbxNumberLineAuxiliary;
    ret = iSession->Get( keyword, number );

    if ( !ret )
        {
        if ( number.Length() )
            {
            aNumber.Copy( number );
            }
        else
            {
            ret = KErrNotFound;
            }
        }
    if ( IsEmergencyNumberL( aNumber ) )
        {
        aNumber.Zero();
        ret = KErrNotFound;
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmNumFromSD: <=" );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::CloseETel
// Closes all ETel resources
// -----------------------------------------------------------------------------
//
void RVmbxNumber::CloseETel()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::CloseETel: =>" );
    iPhoneBook.Close();

    if ( iFlags & KVmFlagPhoneOpened )
        {
        iPhone.Close();
        }

    if ( iFlags & KVmFlagPhoneModuleLoaded )
        {
        iTelServer.UnloadPhoneModule( KMmTsyModuleName );
        }
    iTelServer.Close();

    // Clear corresponding flags
    iFlags &= ~KVmFlagSimReadSupport;
    iFlags &= ~KVmFlagSimWriteSupport;
    iFlags &= ~KVmFlagPhoneModuleLoaded;
    iFlags &= ~KVmFlagPhoneOpened;
    iFlags &= ~KVmFlagPhonebookBeingCached;
    VMBLOGSTRING( "VMBX: RVmbxNumber::CloseETel: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::AppendStrToArrayL
// Appends string from resourse to descriptor array
// -----------------------------------------------------------------------------
//
void RVmbxNumber::AppendStrToArrayL( TInt aResourceId, CDesCArray& aArray )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::AppendStrToArrayL: =>" );
    HBufC* tmp = StringLoader::LoadLC( aResourceId, iCoeEnv );
    aArray.AppendL( *tmp );
    CleanupStack::PopAndDestroy( tmp );
    VMBLOGSTRING( "VMBX: RVmbxNumber::AppendStrToArrayL: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SetHeaderFromResourceL
// Set query dialog header text from resource string
// -----------------------------------------------------------------------------
//
void RVmbxNumber::SetHeaderFromResourceL( TInt aResourceId,
                                                        CAknQueryDialog& aDlg )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SetHeaderFromResourceL: =>" );
    HBufC* headingText = StringLoader::LoadLC( aResourceId, iCoeEnv );
    aDlg.SetHeaderTextL( *headingText );
    CleanupStack::PopAndDestroy( headingText );
    VMBLOGSTRING( "VMBX: RVmbxNumber::SetHeaderFromResourceL: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::QueryAddressL
// Fetches Voip address (returns ETrue when success)
// -----------------------------------------------------------------------------
//
EXPORT_C TBool RVmbxNumber::QueryAddressL( TVmbxQueryType /*aQueryType*/,
                                           TDes& /*aNumber*/ )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryAddressL: =>" );
    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryAddressL: METHOD NOT SUPPORTED ANYMORE" );
    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryAddressL: <=" );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SelectTypeL
// Select which type of Vmbx
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::SelectTypeL( TBool aTitle )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: =>" );
    if ( !AllowedToChangeTheNumber() && EVmbxDefine == aTitle )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: \
        Not Allow to define voice mail number" );
        VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: <=" );
        return EVmbxNone;
        }

    if ( VideoSupported() )
        {
        TInt index( 0 );

        RArray<TInt> vmbxTypeArray;
        CleanupClosePushL( vmbxTypeArray );

        iTypeSelectionQuery = CVmListQueryDialog::NewL( &index );
        iTypeSelectionQuery->PrepareLC( R_MAILBOX_LISTQUERY );

        VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: Not only Voice" );

        // Select which type voice mailbox number to call, CS or Video
        if ( EVmbxCall == aTitle )
            {
            // Set prompt
            VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: aTitle = call" );

            SetHeaderFromResourceL( R_CALLTO_LBX_PROMPT, *iTypeSelectionQuery );

            // Fill items to array
            CDesCArrayFlat* lbxItems =
                        new (ELeave) CDesCArrayFlat( KVmLbxItemsArraySize );
            CleanupStack::PushL( lbxItems );

            TBuf<KVmbxMaxNumberLength> vmbxNumber;

            // Check CS number
            if ( KErrNone == GetVmbxNumber( vmbxNumber ) )
                {
                AppendStrToArrayL( R_CALLTO_LBX_CS_MAILBOX_ITEM, *lbxItems );
                User::LeaveIfError( vmbxTypeArray.Append( EVmbx ) );
                iType = EVmbx;
                VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: EVmbx" );
                }
            // Check Video number
            if ( VideoSupported() &&
                 KErrNone == GetVideoMbxNumber( vmbxNumber ) )
                {
                AppendStrToArrayL( R_CALLTO_LBX_VIDEO_MAILBOX_ITEM,
                                                               *lbxItems );
                User::LeaveIfError( vmbxTypeArray.Append( EVmbxVideo ) );
                iType = EVmbxVideo;
                VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: EVmbxVideo" );
                }

            iTypeSelectionQuery->SetItemTextArray( lbxItems );
            // vest ownership to lbx
            iTypeSelectionQuery->SetOwnershipType( ELbmOwnsItemArray );
            CleanupStack::Pop( lbxItems );
            }
        // Select which type of VMBX number to define, CS or Video
        else
            {
            // Set prompt
            VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: aTitle = define" );

            SetHeaderFromResourceL( R_DEFINE_VIDEO_OR_VOICE_LBX_PROMPT,
                                    *iTypeSelectionQuery );

            // Fill items to array
            CDesCArrayFlat* lbxItems =
                        new (ELeave) CDesCArrayFlat( KVmLbxItemsArraySize );
            CleanupStack::PushL( lbxItems );

            AppendStrToArrayL( R_DEFINE_LBX_CS_MAILBOX_ITEM, *lbxItems );
            User::LeaveIfError( vmbxTypeArray.Append( EVmbx ) );

            if ( VideoSupported() )
                {
                AppendStrToArrayL( R_DEFINE_LBX_VIDEO_MAILBOX_ITEM,
                                                                *lbxItems );
                User::LeaveIfError( vmbxTypeArray.Append( EVmbxVideo ) );
                }

            iTypeSelectionQuery->SetItemTextArray( lbxItems );
            // vest ownership to lbx
            iTypeSelectionQuery->SetOwnershipType( ELbmOwnsItemArray );
            CleanupStack::Pop( lbxItems );
            }

        VMBLOGSTRING2( "VMBX: RVmbxNumber::SelectTypeL array count %d",
            vmbxTypeArray.Count());

        if ( ( EVmbxCall == aTitle ) &&
            ( KVmbxOnlyDefinedOneNumber == vmbxTypeArray.Count() ) )
            {
            // call directly
            VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: Only one" );
            CleanupStack::PopAndDestroy( iTypeSelectionQuery );
            }
        else
            {
            if ( iTypeSelectionQuery->RunLD() )
                {
                iType = vmbxTypeArray[ index ];
                }
            else
                {
                iType = EVmbxNone;
                }
             }

        iTypeSelectionQuery = NULL;
        //close and destroy vmbxTypeArray
        CleanupStack::PopAndDestroy( &vmbxTypeArray );
        VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: <=" );
        return iType;
        }
    // Only supported CS
    else
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: <=" );
        return EVmbx;
        }
    }


// -----------------------------------------------------------------------------
// RVmbxNumber::SaveVmbxAddressL
// Save voice mailbox address to RCSC
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::SaveVmbxAddressL( TDes& /*aAddress*/ )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVmbxAddressL(TDes& aAddress): Method deprecated" );
    return KErrNotSupported;
    }


// ----------------------------------------------------------------------------
// RVmbxNumber::IsVoIPProfilesFound
// Check if VoIP profiles is found.
// ----------------------------------------------------------------------------
//
EXPORT_C TBool RVmbxNumber::IsVoIPProfilesFound()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsVoIPProfilesFound" );

    return iVmSpsHandler->IsVoIPProfilesFound();
    }


EXPORT_C TBool RVmbxNumber::IsIpVoiceMailboxServices()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsIpVoiceMailboxServices: =>" );

    TBool ret( EFalse );

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        RArray<TUint> profileIds;

        GetServiceIds( profileIds );

        TInt count = profileIds.Count();

        profileIds.Close();

        if ( count > 0 )
            {
            ret = ETrue;
            }
        else
            {
            ret = EFalse;
            }
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::IsIpVoiceMailboxServices: <=" );
    return ret;
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::IsSimMemory
// Returns if the memory location for vmbx is sim memory
// ----------------------------------------------------------------------------
//
EXPORT_C TBool RVmbxNumber::IsSimMemory()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsSimMemory: =>" );
    TBool ret( EFalse );
    TInt alsLine( EAlsLine1 );
    // get the current ALS line
    if ( GetAlsLine( alsLine ) != KErrNone )
        {
        alsLine = EAlsLine1;  // if problems, assume primary line
        }
    if ( iSimCardFound )
        {
        // if number automatically fetched from read only SIM
        // or ALS2 is active
        if ( ( !( iFlags & KVmFlagSimWriteSupport ) ) || EAlsLine2 == alsLine  )
            {
            ret = ( !UserEditNumber( alsLine ) && 
                        !IsPhoneMemForcedEdit( alsLine ) );
            }
        // if number stored in writable SIM
        else
            {
            ret = UsesSimMemory();
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsSimMemory: <=" );
    VMBLOGSTRING3( "VMBX: RVmbxNumber::IsSimMemory: sim memory is used \
    iSimCardFound: %I, ret: %I", iSimCardFound, ret );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetVideoMbxNumber
// Fetches the specified video mailbox number
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::GetVideoMbxNumber( TDes& aNumber,
                                                      TVmbxNumberEntry aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVideoMbxNumber: =>" );
    TInt ret = VideoSupported();

    if ( ret )
        {
        TInt lineNumber( aEntry );
        TBuf< KVmbxMaxNumberLength > number;
        if ( EAlsActiveLineEntry == aEntry )
            {
            // get the current ALS line
            if ( KErrNone != GetAlsLine( lineNumber ) )
                {
                lineNumber = EAlsLine1;  // if problems, assume primary line
                }
            }

        TUint32 keyword = lineNumber ==
         EAlsLine1Entry ? KVideoMbxNumberLinePrimary : KVideoMbxNumberLineAuxiliary;
        ret = iSession->Get( keyword, number );

        if ( KErrNone == ret )
            {
            if ( number.Length() )
                {
                aNumber.Copy( number );
                }
            else
                {
                ret = KErrNotFound;
                }
            }

        if ( IsEmergencyNumberL( aNumber ) )
            {
            aNumber.Zero();
            ret = KErrNotFound;
            }
        }
    else
        {
        ret = KErrNotFound;
        }
    VMBLOGSTRING2( "VMBX: RVmbxNumber::GetVideoMbxNumber:<=\
                                Video supported = %d", ret );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::QueryVideoMbxNumberL
// Queries a video mailbox number
// -----------------------------------------------------------------------------
//
EXPORT_C TBool RVmbxNumber::QueryVideoMbxNumberL( TVmbxQueryType aQueryType,
                                                                 TDes& aNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryVideoMbxNumberL: =>" );
    TBool result( EFalse );

    __ASSERT_DEBUG( aNumber.MaxLength() >= KVmbxMaxNumberLength,
                    User::Panic(KVmbxPanicCategory, EVmbxDescriptorTooSmall) );
    __ASSERT_DEBUG( aNumber.Length() <= KVmbxMaxNumberLength,
                         User::Panic(KVmbxPanicCategory, EVmbxNumberTooLong) );

    if ( !AllowedToChangeTheNumber() )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::QueryVideoMbxNumberL: \
        Video number not allowed to change" );
        VMBLOGSTRING( "VMBX: RVmbxNumber::QueryVideoMbxNumberL: <=" );
        User::Leave( KErrAccessDenied );
        }

    TBuf< KVmbxMaxNumberLength > number;
    TBool thisClosed( EFalse );
    // Close() sets thisClosed to ETrue

    FOREVER
        {
        number.Copy( aNumber );

        // get the correct query prompt from resource file
        HBufC* prompt = StringLoader::LoadLC(
                    ( aQueryType == EVmbxNotDefinedQuery ?
                            R_VIDEO_DEFINE_NUMBER_PROMPT :
                                R_VIDEO_CHANGE_NUMBER_PROMPT),
                                        iCoeEnv );

        iClosedPtr = &thisClosed;

        // Convert output to accommodate the current locale
        AknTextUtils::LanguageSpecificNumberConversion( number );

        TInt ret;  // return value of the query
        TBool searchUsed;  // ETrue to open the query again after
                           //closing single fetch dialog

        // enable search if number length is 0
        TBool searchEnabled( number.Length() ? EFalse : ETrue );

        do
            {
            // show query with the correct prompt
            iVideoQuery = CVmTextQueryDialog::NewL( number,
                                                *prompt, searchEnabled );

            ret = iVideoQuery->ExecuteLD( R_VMBX_NUMBER_QUERY );

            iVideoQuery = NULL;
            searchUsed = EFalse;

            if ( searchEnabled && ret && !number.Length() )
                {
                searchUsed = ETrue;
                // Show phonebook's single fetch dialog
                TRAPD( error, FetchNumberFromPhonebook2L( number ) );

                // user has pressed End Key
                if ( thisClosed )
                    {
                    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryVideoMbxNumberL: \
                    End key pressed" );
                    CleanupStack::PopAndDestroy( prompt );
                    return EFalse;
                    }

                if ( error ) //  Back pressed in Contact UI
                    {
                    // Now the phone book fetch dialog
                    // was closed, get out of the loop
                    ret = EFalse;
                    break;
                    }
                }
            } while( searchUsed );

        iClosedPtr = NULL;

        CleanupStack::PopAndDestroy( prompt );

        if ( ret )
            {
            if ( !number.Length() )
                {
                User::LeaveIfError( SaveVideoMbxNumber( number,
                                                    EAlsActiveLineEntry ) );
                aNumber.Copy( number );

                // show "not defined" note
                ShowConfirmationNoteL( R_VIDEO_NOTE_EMPTY_NUMBER );
                result = ETrue;
                break;
                }

            // Convert back to western digits
            ConvertToWesternDigits( number );

            // check the given number here (emergency numbers not allowed)
            if ( IsValidPhoneNumber( number ) && !IsEmergencyNumberL( number ) )
                {
                TInt status = SaveVideoMbxNumber( number, EAlsActiveLineEntry );

                if ( status == KErrNone )
                    {
                    aNumber.Copy( number );
                    ShowConfirmationNoteL( R_VIDEO_SAVED_NOTE );
                    result = ETrue;
                    break;
                    }
                }

            if ( !result )
                {
                // show "invalid number" note and wait for a short timeout
                prompt = StringLoader::LoadLC( R_INVALID_NUMBER_NOTE, iCoeEnv );

                iClosedPtr = &thisClosed;

                CAknInformationNote* note =
                                     new( ELeave ) CAknInformationNote( ETrue );
                note->ExecuteLD( *prompt );

                iClosedPtr = NULL;

                CleanupStack::PopAndDestroy( prompt );

                if ( !thisClosed )
                    {
                    continue;  // the number was invalid -> query again.
                    }
                }
            }
        break;  // user canceled the query
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::QueryVideoMbxNumberL: <=" );
    return result;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SaveVideoMbxNumber
// Saves the vmbx number. (in phone file system)
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::SaveVideoMbxNumber( const TDesC& aNumber,
                                                     TVmbxNumberEntry aEntry )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVideoMbxNumber: =>" );
    TInt error( KErrNone );
    __ASSERT_DEBUG( iFlags & KVmFlagOpened,
                User::Panic(KVmbxPanicCategory, EVmbxNotConnected) );
    __ASSERT_DEBUG( aNumber.Length() <= KVmbxMaxNumberLength,
                User::Panic(KVmbxPanicCategory, EVmbxNumberTooLong) );

    TInt lineNumber( aEntry );
    if ( aEntry == EAlsActiveLineEntry )
        {
    // get the current ALS line
        if ( GetAlsLine( lineNumber ) != KErrNone )
            {
            lineNumber = EAlsLine1;  // if problems, assume primary line
            }
        }
    TUint32 keyword = lineNumber ==
      EAlsLine1Entry ? KVideoMbxNumberLinePrimary: KVideoMbxNumberLineAuxiliary;

    error = iSession->Set( keyword, aNumber );
    if ( !error )
        {
         // Client will generate the event itself on request.
        if ( iFlags & KVmFlagNotifyRequested )
            {
            HandleNotifyString( keyword, aNumber );
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::SaveVideoMbxNumber: <=" );
    return error;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::IsUsimSupport
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  Series 60 Customer / MSatRefreshOserver
//  Series 60 MSatRefreshObserver API
// -----------------------------------------------------------------------------
TBool RVmbxNumber::AllowRefresh(
            TSatRefreshType aType, const TSatRefreshFiles& aFiles )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::AllowRefresh: =>" );
    TBool allowRefresh( ETrue );
    if ( aType == EFileChangeNotification )
        {
        const TInt elFile1( aFiles.Locate( KMbdnEf ) );
        const TInt elFile2( aFiles.Locate( KMbiEf) );

        if ( ( KErrNotFound == elFile1) &&
             ( KErrNotFound == elFile2 ) )
            {
            allowRefresh = EFalse;
            }

        }
    else
        {
        allowRefresh = EFalse;
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::AllowRefresh: <=" );
    return allowRefresh;
    }

// -----------------------------------------------------------------------------
//  Series 60 Customer / MSatRefreshOserver
//  Series 60 MSatRefreshObserver API
// -----------------------------------------------------------------------------
 void RVmbxNumber::Refresh( TSatRefreshType aType,
                                               const TSatRefreshFiles& aFiles )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::Refresh: =>" );
    if ( aType == EFileChangeNotification )
        {
        const TInt elFile1( aFiles.Locate( KMbdnEf ) );
        const TInt elFile2( aFiles.Locate( KMbiEf ) );

        if ( ( KErrNotFound != elFile1) || ( KErrNotFound != elFile2 ) )
            {
            //do phone book getInfo
            TInt error( KErrNone );
            error = DoPhonebookOperation( EVmbxPhonebookGetInfo );
            if ( !error )
                {
                TVmbxEntry entry;
                entry.iIndex = 1;
                TInt ret = DoPhonebookOperation( EVmbxPhonebookRead, &entry );

                // Notify SAT Server that refresh initiated file read is done
                iRSatClient.RefreshEFRead( EFalse );

                if ( ( iNotifyCallBack  ) && ( entry.iTelNumber.Length() > 0 )
                    && ( iMemoryLocation == EVmbxSimMemory ) )
                    {
                    TInt notifyLine( 0 );
                    TRAPD( err, iNotifyCallBack->HandleNotifyL( notifyLine,
                                                        entry.iTelNumber ) );
                    if ( err )
                        {
                        _LIT(KVmbxPanicType,"iNotifyCallBack->HandleNotifyL");
                        User::Panic( KVmbxPanicType, err );
                        }
                    }
                }
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::Refresh: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::MailboxNumbersIdentifiers
// Gets identifiers of the records in sim from MBI-file in sim
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::MailboxNumbersIdentifiers()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::MailboxNumbersIdentifiers: =>" );
    TRequestStatus status;

    // Get identifiers
    iPhone.GetMailboxNumbers( status, iPhoneVoicemailInfoPckg );

    // Wait for asynchronous call to finish
    User::WaitForRequest( status );
    VMBLOGSTRING2( "Identifier read status = %I", status.Int() );
    VMBLOGSTRING2( "iVoice value: %d", iPhoneVoicemailInfo.iVoice );
    VMBLOGSTRING( "VMBX: RVmbxNumber::MailboxNumbersIdentifiers: <=" );
    return status.Int();
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::MailboxNumbersIdentifiers
// Gets identifiers of the records in sim from MBI-file in sim
// -----------------------------------------------------------------------------
//
void RVmbxNumber::TestAndSetSimAccessFlags()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::TestAndSetSimAccessFlags: =>" );
    // Caps are not valid for read-only Sim, so check if writing
    // to sim fails.
    // Assume that a number exist
    iNoNumberFound = EFalse;
    TBool isTestNumberUsed = EFalse;
    // read sim data first
    TVmbxEntry entry;
    entry.iIndex = EAlsLine1;

    TInt readError = DoPhonebookOperation( EVmbxPhonebookRead, &entry );

    // KErrNotFound is returned when number length is zero, then
    // testnumber is used to test write support
    if ( readError == KErrNone || readError == KErrNotFound )
        {
        // save number for later use
        if ( entry.iTelNumber.Length() )
            {
            iNumberFromSim.Copy( entry.iTelNumber );
            }
        // number not found, test with testnumber
        else if ( entry.iTelNumber.Length() == 0 )
            {
            // no number found
            iNoNumberFound = ETrue;
            VMBLOGSTRING( "Use test number for testing write support" );
            entry.iTelNumber.Copy( KTestNumber ) ;
            isTestNumberUsed = ETrue;
            }

        VMBLOGSTRING( "Try writing" );

        TInt writeError = DoPhonebookOperation( EVmbxPhonebookWrite, &entry );
        VMBLOGSTRING2( "Write: %I", writeError );

        // write error, set write flag to not supported
        // and phone memory in use
        if ( writeError )
            {
            VMBLOGSTRING( "WriteAccess not ok" );
            iFlags &= ~KVmFlagSimWriteSupport;
            }
        // tested with and sim has write access so delete test number from sim
        else if ( isTestNumberUsed && writeError == KErrNone )
            {
            VMBLOGSTRING( "Remove test number from sim" );
            DoPhonebookOperation( EVmbxPhonebookDelete, &entry );
            }
        }
    // read error, set read and write flag to not supported
    // and phone memory in use
    else
        {
        VMBLOGSTRING( "ReadAccess not ok" );
        iFlags &= ~KVmFlagSimReadSupport;

        VMBLOGSTRING( "WriteAccess not ok" );
        iFlags &= ~KVmFlagSimWriteSupport;
        // The first USim phonebook type is tested,
        // keep memorylocation unchanged
        if ( iUSimFirstRoundTest )
            {
            VMBLOGSTRING2( "iMemoryLocation = %d", iMemoryLocation );
            iUSimFirstRoundTest = EFalse;
            }
        else
            {
            iMemoryLocation = EVmbxPhoneMemory;
            iSession->Set( KVmUsesSimMemory, EVmbxPhoneMemory );
            VMBLOGSTRING2( "iMemoryLocation = %d", iMemoryLocation );
            }
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::TestAndSetSimAccessFlags: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::MailboxNumbersIdentifiers
// Gets identifiers of the records in sim from MBI-file in sim
// -----------------------------------------------------------------------------
//
void RVmbxNumber::CalculateActualMaxLengthOnSim( TInt aSimNumberLength )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::CalculateActualMaxLength: =>" );
    VMBLOGSTRING2("iPhoneBookInfo.iMaxNumLength = %I", iPhoneBookInfo.iMaxNumLength );
    VMBLOGSTRING2("aSimNumberLength = %I", aSimNumberLength );

    // If max length is less than 20 or -1 returned, then
    // max length must be detemined from number length that is
    // saved on sim.
    if ( iPhoneBookInfo.iMaxNumLength == KVmExtFileSize ||
            iPhoneBookInfo.iMaxNumLength == -1 )
        {
        // number on sim is less than 21
        if ( aSimNumberLength <= KVmExtFileSize )
            {
            iPhoneBookInfo.iMaxNumLength = KVmExtFileSize;
            }
        // number on sim is more than 20 but less than 40
        else if ( KVmExtFileSize < aSimNumberLength &&
                        aSimNumberLength <= KVmPhoneNumDigitsMaxLength  )
            {
            iPhoneBookInfo.iMaxNumLength = KVmPhoneNumDigitsMaxLength;
            }
        // number on sim is more than 40
        else
            {
            iPhoneBookInfo.iMaxNumLength = KVmMaxStoreSize;
            }
        }
    // If max length is 40, and number length on sim is more
    // than 20 but less than 41, then 48 is available on sim.
    else if ( iPhoneBookInfo.iMaxNumLength == KVmPhoneNumDigitsMaxLength &&
              KVmExtFileSize < aSimNumberLength &&
              aSimNumberLength <= KVmPhoneNumDigitsMaxLength )
        {
        iPhoneBookInfo.iMaxNumLength = KVmMaxStoreSize;
        }
    VMBLOGSTRING2("iPhoneBookInfo.iMaxNumLength = %I", iPhoneBookInfo.iMaxNumLength );

    VMBLOGSTRING( "VMBX: RVmbxNumber::CalculateActualMaxLength: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::VideoSupported
// Returns video support state
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::VideoSupported()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::VideoSupported: =>" );

    TBool supported( EFalse );

    if ( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        TInt result = iSession->Get( KVideoMbxSupport, supported );
        if ( result != KErrNone )
            {
            supported = EFalse;
            }
        }

    VMBLOGSTRING2("VMBX: RVmbxNumber::VideoSupported: supported = %d", supported );
    return supported;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetVmbxAddressL
// Retrieves voice mailbox address from RCSC
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::GetVmbxAddressL( TDes& /*aAddress*/ )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmbxAddressL(TDes& aAddress): Method deprecated" );
    VMBLOGSTRING( "Use GetVmbxAddressL(TDes& aAddress, TUint aServiceId) instead" );

    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::GetVmbxAddress
// Retrieves voice mailbox address from SPS
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::GetVmbxAddress( TDes& aAddress, TUint aServiceId )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmbxAddress: =>" );
    TInt ret( KErrNotSupported );

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        TVmbxServiceInfo serviceInfo;
        serviceInfo.iServiceId = aServiceId;
        ret = GetServiceInfo( serviceInfo );
        VMBLOGSTRING2( "VMBX: RVmbxNumber::GetVmbxAddress: error: %d", ret );
        if ( KErrNone == ret )
            {
            if ( serviceInfo.iAddress.Length() > aAddress.MaxLength() )
                {
                ret = KErrOverflow;
                }
            else
                {
                aAddress.Copy( serviceInfo.iAddress );
                }
            }
        VMBLOGSTRING2( "VMBX: RVmbxNumber::GetVmbxAddress: error: %d", ret );
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::GetVmbxAddress: <=" );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SelectTypeL
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::SelectTypeL( TBool aTitle, TUint& aServiceId )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: =>" );

    if ( !AllowedToChangeTheNumber() && EVmbxDefine == aTitle )
       {
       VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: \
       Not Allow to define voice mail number" );
       VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: <=" );
       return EVmbxNone;
       }

    if ( EVmbxCall != aTitle )
        {
        User::Leave( KErrNotSupported );
        }
    // the sum of defined numbers
    TInt definedNumber( 0 );
    TInt definedFlag( 0 );
    CheckDefinedNumberCountL(definedNumber, definedFlag, aServiceId );

    // Pop up selection dialog only if there are more than 2 defined numbers
    if ( KMailBoxLimitForPrompt < definedNumber )
        {
        TInt index( 0 );
        // Clear the old service information
        iVmbxServiceSelection.Reset();

        // Create selection list query and show it.
        iTypeSelectionQuery = new (ELeave) CAknListQueryDialog( &index );
        iTypeSelectionQuery->PrepareLC( R_VOIP_REL2_2_CALL_TO );

        // Create descriptor array for connection networks, connection networks
        // icons and load connection network images to memory..
        CDesCArrayFlat* lbxItems =
            new (ELeave) CDesCArrayFlat( KVmLbxItemsArraySize );
        CleanupStack::PushL( lbxItems );
        CArrayPtr<CGulIcon>* icons =
            new (ELeave) CAknIconArray( KVmLbxItemsArraySize );
        CleanupStack::PushL( icons );

        ConstructDefaultIconsL( icons );

        // Create final list box item data.
        TBuf<512> queryItem( KNullDesC );
        if( definedFlag & KVmFlagCSNumberDefined )
            {
            HBufC* csName = StringLoader::LoadLC( R_CALLTO_LBX_CS_MAILBOX_ITEM,
                                                                       iCoeEnv );
            queryItem.Format( KQueryItemFormat, 0, csName );
            CleanupStack::PopAndDestroy( csName );

            // Append text to the item array.
            lbxItems->AppendL( queryItem );
            // CS's hardcoded service value is 1
            User::LeaveIfError( iVmbxServiceSelection.Append(
                TVmbxServiceSelection( EVmbx, 1 ) ) );
            }

        if( definedFlag & KVmFlagVideoNumberDefined )
            {
            HBufC* videoName =
                StringLoader::LoadLC( R_CALLTO_LBX_VIDEO_MAILBOX_ITEM,
                                        iCoeEnv );
            // Create final list box item data.
            queryItem.Format( KQueryItemFormat, 1, videoName );
            CleanupStack::PopAndDestroy( videoName );

            // Append text to the item array.
            lbxItems->AppendL( queryItem );
            // Video's hardcoded service value is 1
            User::LeaveIfError( iVmbxServiceSelection.Append(
                TVmbxServiceSelection( EVmbxVideo, 1 ) ) );
            }
        if( definedFlag & KVmFlagVoIpNumberDefined )
            {
             // Get all Service Ids
            RArray<TUint> profileIds;
            CleanupClosePushL( profileIds );
            GetServiceIds( profileIds );

            for ( TInt i( 0 ); i < profileIds.Count(); i++ )
                {
                TVmbxServiceInfo serviceInfo;
                serviceInfo.iServiceId = profileIds[ i ];
                GetServiceInfo( serviceInfo );

                HBufC* voipName = HBufC::NewLC( serviceInfo.iName.Length() );
                voipName->Des().Copy( serviceInfo.iName );

                TInt iconId( KErrNotFound );
                TRAPD( error, AddBrandIconL( icons, profileIds[ i ], iconId ) );

                // If branded icon is not available, use hardcoded value 2 which is
                // default VoIP icons value
                if ( KErrNone != error || KErrNotFound == iconId )
                    {
                    iconId = 2;
                    }

                // Create final list box item data.
                queryItem.Format( KQueryItemFormat, iconId, voipName );
                CleanupStack::PopAndDestroy( voipName );

                //Append text to the item array.
                lbxItems->AppendL( queryItem );
                User::LeaveIfError( iVmbxServiceSelection.Append(
                    TVmbxServiceSelection( EVmbxIP, serviceInfo.iServiceId ) ) );
                }

            CleanupStack::PopAndDestroy( &profileIds );
            }

        iTypeSelectionQuery->SetIconArrayL( icons );
        iTypeSelectionQuery->SetItemTextArray( lbxItems );
        iTypeSelectionQuery->SetOwnershipType( ELbmOwnsItemArray );

        CleanupStack::Pop( icons );
        CleanupStack::Pop( lbxItems );

        if ( iTypeSelectionQuery->RunLD() )
            {
            TVmbxServiceSelection serviceType = iVmbxServiceSelection[ index ];
            iType = serviceType.iServiceType;
            aServiceId  = serviceType.iServiceId;
            VMBLOGSTRING2( "VMBX: RVmbxNumber::VoIP vmbx with service id %d selected", aServiceId );
            }
        else
            {
            iType = EVmbxNone;
            aServiceId = 0;
            }
        iTypeSelectionQuery = NULL;
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::SelectTypeL: <=" );
    return iType;
    }
// -----------------------------------------------------------------------------
// RVmbxNumber::CheckDefinedNumberCountL
//
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::CheckDefinedNumberCountL( TInt& aDefinedNumber,
                                      TInt& aDefinedFlag, TUint& aServiceId )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::CheckDefinedNumberCountL: =>" );
    iType = EVmbxNone;
    aServiceId = KDefaultServiceId;
     // the sum of defined numbers
    TInt definedNumber( KDefaultDefinedNumberCount );
    TInt definedServiceTypes( 0 );
    TInt definedFlag( 0 );

    TBuf<KVmbxMaxNumberLength> tmpNumber(KNullDesC);

    // First check what mailboxes are defined.
    // Skip querying if only one  mailbox with number/address defined.
    // 1. Get voice number
    if ( KErrNone == GetVmbxNumber( tmpNumber ) )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::CheckDefinedNumberCountL: CS vmbx number defined");
        definedNumber++;
        definedServiceTypes++;
        iType = EVmbx;
        aServiceId = KCSorVideoServiceId;
        definedFlag |= KVmFlagCSNumberDefined;
        }

    // 2. Get video number
    if ( VideoSupported() && KErrNone == GetVideoMbxNumber( tmpNumber ) )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::CheckDefinedNumberCountL: Video mbx number defined");
        definedNumber++;
        definedServiceTypes++;
        if( KMailBoxLimitForPrompt == definedNumber)
            {
            iType = EVmbxVideo;
            }
        aServiceId = KCSorVideoServiceId;
        definedFlag |= KVmFlagVideoNumberDefined;
        }

    // 3. Get count of all Service Ids of VoIP
    RArray<TUint> profileIds;
    CleanupClosePushL( profileIds );

    if ( KErrNone == GetServiceIds( profileIds ) )
        {
        TInt idCount( profileIds.Count() );
        VMBLOGSTRING2( "VMBX: RVmbxNumber::CheckDefinedNumberCountL: %d IP vmbx services defined", idCount );

        if ( idCount )
            {
            definedNumber += idCount;
            definedServiceTypes++;
            // If only VoIP and only one service support Vmbx,
            // then use it directly without prompting.
            if ( KMailBoxLimitForPrompt == definedNumber )
                {
                iType = EVmbxIP;
                aServiceId = profileIds[ 0 ];
                }
            definedFlag |= KVmFlagVoIpNumberDefined;
            }
        }

    CleanupStack::PopAndDestroy( &profileIds );
    aDefinedNumber = definedNumber;
    aDefinedFlag = definedFlag;
    VMBLOGSTRING2( "VMBX: RVmbxNumber::CheckDefinedNumberCountL: definedNumber = %d",
                definedNumber );
    VMBLOGSTRING( "VMBX: RVmbxNumber::CheckDefinedNumberCountL: <=" );

    if ( 1 < definedServiceTypes )
        {
        iType = EVmbxNone;
        }
    return iType;
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::GetServiceIds
//
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::GetServiceIds( RArray<TUint>& aProfileIds )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetServiceIds: =>" );
    TInt ret( KErrNotFound );
    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        TRAP( ret, iVmSpsHandler->GetServiceIdsL( aProfileIds ) );
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetServiceIds: <=" );
    return ret;
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::GetServiceInfo
//
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RVmbxNumber::GetServiceInfo( TVmbxServiceInfo& aServiceInfo )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetServiceInfo: =>" );
    TInt ret( KErrNotFound );
    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        ret = iVmSpsHandler->GetServiceInfo( aServiceInfo );
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetServiceInfo: <=" );
    return ret;
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::NotifyServiceChange
//
// ----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::NotifyServiceChange(
    MServiceNotifyHandler* aHandler )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyServiceChange: =>" );

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        iVmSpsHandler->NotifyServiceChange( aHandler );
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyServiceChange: <=" );
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::NotifyServiceChangeCancel
//
// ----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::NotifyServiceChangeCancel()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyServiceChangeCancel: =>" );

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        iVmSpsHandler->NotifyServiceChangeCancel();
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::NotifyServiceChangeCancel: <=" );
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::BrandIdL
//
// ----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::BrandIdL( TInt aServiceId, TDes8& aBrandId )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::BrandIdL: =>" );

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        iVmSpsHandler->BrandIdL( aServiceId, aBrandId );
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::BrandIdL: <=" );
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::GetBrandedIconL
//
// ----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::GetBrandedIconL(
    const TDesC8& aBrandingId,
    CFbsBitmap*& aBrandedBitmap,
    CFbsBitmap*& aBrandedBitmapMask )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::GetBrandedIconL: =>" );

    iVmBsHandler->GetBrandedIconL(
        aBrandingId, aBrandedBitmap, aBrandedBitmapMask );

    VMBLOGSTRING( "VMBX: RVmbxNumber::GetBrandedIconL: <=" );
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::ConstructDefaultIconsL
//
// ----------------------------------------------------------------------------
//
EXPORT_C void RVmbxNumber::ConstructDefaultIconsL(
    CArrayPtr<CGulIcon>* aIcons )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::ConstructDefaultIconsL: =>" );

    CFbsBitmap* bitmap;
    CFbsBitmap* mask;

    //Get the mbm file path
    TFileName mbmfile( KVmMbmDrive );
    mbmfile.Append( KDC_APP_BITMAP_DIR );
    mbmfile.Append( KVmLibMbmFile );

    //default CS icon
    AknIconUtils::CreateIconL( bitmap, mask, mbmfile,
                               EMbmVmQgn_prop_nrtyp_mobile,
                               EMbmVmQgn_prop_nrtyp_mobile_mask );

    aIcons->AppendL( CGulIcon::NewL( bitmap, mask ) );

    //default Video icon
    AknIconUtils::CreateIconL( bitmap, mask, mbmfile,
                               EMbmVmQgn_prop_nrtyp_video,
                               EMbmVmQgn_prop_nrtyp_video_mask );

    aIcons->AppendL( CGulIcon::NewL( bitmap, mask ) );

    //default VoIP icon
    AknIconUtils::CreateIconL( bitmap, mask, mbmfile,
                               EMbmVmQgn_prop_nrtyp_voip,
                               EMbmVmQgn_prop_nrtyp_voip_mask );

    aIcons->AppendL( CGulIcon::NewL( bitmap, mask ) );

    VMBLOGSTRING( "VMBX: RVmbxNumber::ConstructDefaultIconsL: <=" );
    }
// -----------------------------------------------------------------------------
// RVmbxNumber::AddBrandIconL
//
// -----------------------------------------------------------------------------
//
void RVmbxNumber::AddBrandIconL(
    CArrayPtr<CGulIcon>* aIcons,
    TUint aServiceId,
    TInt& aIconId )
    {
    // Set branding icon to context pane if available.
    TBuf8<KVmSettingsUiBrandingIdLength> brandId( KNullDesC8 );

    BrandIdL( aServiceId, brandId );

    CFbsBitmap* brandedBitmap = NULL;
    CFbsBitmap* brandedBitmapMask = NULL;

    // Get branded bitmap
    TRAPD( err, GetBrandedIconL( brandId, brandedBitmap, brandedBitmapMask ) );

    if ( KErrNone == err )
        {
        // The count of list icons(before adding branding icon to the list)
        // must be the branded icon list id
        aIconId = aIcons->Count();
        // Create new icon and add it to the icon list
        aIcons->AppendL( CGulIcon::NewL( brandedBitmap, brandedBitmapMask ) );
        }
    else
        {
        delete brandedBitmap;
        delete brandedBitmapMask;
        }
    }


// -----------------------------------------------------------------------------
// RVmbxNumber::VmbxNumDefInPhoneMemory
// Checks is VMBX number defined in phone memory
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::VmbxNumDefInPhoneMemory( TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::VmbxNumDefInPhoneMemory: =>" );
    TBool numDefined( EFalse );
    TBuf< KVmbxMaxNumberLength > number;
    TUint32 lineNumber = aLineNumber ==
        EAlsLine1 ? KVmbxNumberLinePrimary: KVmbxNumberLineAuxiliary;

    if ( KErrNone == iSession->Get( lineNumber, number ) )
        {
        if ( aLineNumber == EAlsLine1 )
            {
            if (  iMemoryLocation == EVmbxPhoneMemory  &&
                 number.Length()  )
                {
                numDefined = ETrue;
                }
            }
        else if ( aLineNumber == EAlsLine2 && number.Length() )
            {
            numDefined = ETrue;
            }
        }

    VMBLOGSTRING2("VMBX: RVmbxNumber::VmbxNumDefInPhoneMemory: numDefined = %d",
                numDefined );
    return numDefined;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::NumStoredInPhone
// Checks is the given number stored in phone memory
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::NumStoredInPhone( const TDesC& aNumber, TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::NumStoredInPhone: =>" );
    TBool numStoredInPhone( EFalse );
    TBuf< KVmbxMaxNumberLength > number;
    TUint32 lineNumber = aLineNumber ==
        EAlsLine1 ? KVmbxNumberLinePrimary : KVmbxNumberLineAuxiliary;

    if (  KErrNone == iSession->Get( lineNumber, number ) )
        {
        if ( !( number.Compare( aNumber ) ) )
            {
            numStoredInPhone = ETrue;
            }
        }

    VMBLOGSTRING2( "VMBX: RVmbxNumber::NumStoredInPhone: numStoredInPhone = %d",
            numStoredInPhone );
    return numStoredInPhone;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::BackUpSimNumber
// Saves VMBX number to the backup store
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::BackUpSimNumber( const TDesC& aNumber, TInt aLineNumber  )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::BackUpSimNumber: =>" );
    TUint32 lineNumber = aLineNumber == EAlsLine1 ?
            KVmbxTmpNumberLinePrimary : KVmbxTmpNumberLineAuxiliary;

    VMBLOGSTRING( "VMBX: RVmbxNumber::BackUpSimNumber: <=" );
    return ( iSession->Set( lineNumber, aNumber ));
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::FetchSimNumberBackUp
// Fetches VMBX number from backup store
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::FetchSimNumberBackUp( TDes& aNumber, TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::FetchSimNumberBackUp: =>" );
    TUint32 lineNumber = aLineNumber == EAlsLine1 ?
                KVmbxTmpNumberLinePrimary : KVmbxTmpNumberLineAuxiliary;

    VMBLOGSTRING( "VMBX: RVmbxNumber::FetchSimNumberBackUp: <=" );
    return ( iSession->Get( lineNumber, aNumber ));
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::ClearVMBXNumberFromPhone
// Clear VMBX number is from SIM from the phone memory
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::ClearVMBXNumberFromPhone( TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::ClearVMBXNumberFromPhone: =>" );

    TInt error( KErrNone );

    if ( aLineNumber == EAlsLine1 )
        {
        error = SaveVmbxNumberToPhone( KEmptyVoiceMailNumber, EAlsLine1Entry );
        if ( KErrNone == error &&
           ( KErrNone == iSession->Set( KVmUsesSimMemory, EVmbxSimMemory ) ) )
            {
            iMemoryLocation = EVmbxSimMemory;
            VMBLOGSTRING(
              "VMBX: RVmbxNumber::ClearVMBXNumberFromPhone: Line 1" );
            }
        }
    else if ( aLineNumber == EAlsLine2 )
        {
        error = SaveVmbxNumberToPhone( KEmptyVoiceMailNumber, EAlsLine2Entry );
        if ( KErrNone == error )
            {
            VMBLOGSTRING(
              "VMBX: RVmbxNumber::ClearVMBXNumberFromPhone: Line 2");
            }
        }
    else
        {
        error = KErrArgument;
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::ClearVMBXNumberFromPhone: <=" );
    return error;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::UserEditNumber
// Indicates if user has edited VMBX number
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::UserEditNumber( TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::UserEditNumber: =>" );
    TBool nbrEdited( EFalse );

    TUint32 lineNumber = aLineNumber == EAlsLine1 ?
            KUserEditedNumber : KUserEditedNumberAuxiliary;

    iSession->Get( lineNumber, nbrEdited );

    VMBLOGSTRING2( "VMBX: RVmbxNumber::UserEditNumber: result = %d",
            nbrEdited );
    return EVmbxUserEdit == nbrEdited;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SetUserEditNumber
// Sets the flag indicating that the user has edited VMBX number
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::SetUserEditNumber( TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SetUserEditNumber: =>" );

    TUint32 lineNumber = aLineNumber == EAlsLine1 ?
            KUserEditedNumber : KUserEditedNumberAuxiliary;

    VMBLOGSTRING( "VMBX: RVmbxNumber::SetUserEditNumber: <=" );
    return ( iSession->Set( lineNumber, EVmbxUserEdit ) );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::ResetUserEditNumber
// Resets the flag that indicates that the user has edited VMBX number
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::ResetUserEditNumber( TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::ResetUserEditNumber: =>" );

    TUint32 lineNumber = aLineNumber == EAlsLine1 ?
                KUserEditedNumber : KUserEditedNumberAuxiliary;

    VMBLOGSTRING( "VMBX: RVmbxNumber::ResetUserEditNumber: <=" );
    return ( iSession->Set( lineNumber, EVmbxNotUserEdit ) );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::UseSimNumIfAvailable
// Fetches VMBX number stored in SIM if number is available
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::UseSimNumIfAvailable( TDes& aNumber, TInt aLineNumber )
    {
    TBuf< KVmbxMaxNumberLength > number;
    TBool ret( EFalse );
    TInt error( KErrNone );
    TVmbxNumberEntry numberEntry = aLineNumber == EAlsLine1 ?
            EAlsLine1Entry : EAlsLine2Entry;

    VMBLOGSTRING( "VMBX: RVmbxNumber::UseSimNumIfAvailable: =>" );

    // If readonly SIM which supports VMBX number
    // number is fetched from the backup store
    if ( ( !( iFlags & KVmFlagSimWriteSupport ) && ( iFlags & KVmFlagSimVmbxNumSupport ) )
             && ( ( aLineNumber == EAlsLine1 && EVmbxPhoneMemory == iMemoryLocation ) ||
                    aLineNumber == EAlsLine2 ) )
        {
        error = FetchSimNumberBackUp( number, aLineNumber );
        if ( KErrNone == error )
            {
            aNumber.Copy( number );
            SaveVmbxNumberToPhone( aNumber, numberEntry );
            VMBLOGSTRING(
              "VMBX: RVmbxNumber::UseSimNumIfAvailable: number from backup store" );
            ret = ETrue;
            }
        }
    // If writable SIM which supports VMBX number
    // number is fetched from SIM
    else if ( iFlags & KVmFlagSimWriteSupport &&
            ( ( aLineNumber == EAlsLine1 && EVmbxPhoneMemory == iMemoryLocation ) ||
                    aLineNumber == EAlsLine2 ) )
        {
        error = GetVmNumFromSIM( number, aLineNumber );
        if ( KErrNone == error )
            {
            aNumber.Copy( number );
            if ( EAlsLine1 == aLineNumber )
                {
                iMemoryLocation = EVmbxSimMemory;
                iSession->Set( KVmUsesSimMemory, EVmbxSimMemory );
                }
            VMBLOGSTRING(
              "VMBX: RVmbxNumber::UseSimNumIfAvailable: number from SIM" );
            ret = ETrue;
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::UseSimNumIfAvailable: <=" );
    return ret;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::HandleNumberStores
// Handles the updating of a number stores in phone memory
// -----------------------------------------------------------------------------
//
TInt RVmbxNumber::HandleNumberStores( TInt aLineNumber )
    {
    TInt error( KErrNone );
    TBuf< KVmbxMaxNumberLength > line2number;
    
    TInt inactiveLineNumber( EAlsLine2 );

    VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNumberStores: =>" );

    if ( iNumberFromSim.Length() )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNumberStores: Sim number found" );
        // If number found in read only SIM
        // backup is made
        if ( !( iFlags & KVmFlagSimWriteSupport ) )
            {
            if ( aLineNumber == EAlsLine2 )
                {
                error = GetVmNumFromSIM( line2number, EAlsLine2Entry );
                if ( KErrNone == error )
                    {
                    error = BackUpSimNumber( line2number, aLineNumber );
                    VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNumberStores: Back up ALS 2 Sim number " );
                    }
                }
            else
                {
                error = BackUpSimNumber( iNumberFromSim, aLineNumber );
                VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNumberStores: Back up ALS 1 Sim number" );
                }
            }
        // If number found in SIM, old stored number is erased if
        // it's not defined by the user or updated via external clients
        if ( !UserEditNumber( aLineNumber ) && 
                !IsPhoneMemForcedEdit( aLineNumber ) )
            {
            ClearVMBXNumberFromPhone( aLineNumber );
            // if the number was not edited in inactive line,
            // and do clearing opreration. 
            // The old stored number from SIM is erased.
            // otherwise, the edited number was 
            // defined as the inactive line number.
            // the number should be kept
            if ( !IsInactiveLineEdited( inactiveLineNumber ) )
            	{
                ClearVMBXNumberFromPhone( inactiveLineNumber );
            	}
            
            VMBLOGSTRING( "[VMBX]: RVmbxNumber::HandleNumberStores: Clear old VMBX number" );
            }
        }
    else
        {
        // Changing or defining the number from UI (user) not allowed
        if ( !UserEditNumber( aLineNumber ) 
                && !IsPhoneMemForcedEdit( aLineNumber ) )
            {
            ClearVMBXNumberFromPhone( aLineNumber );
            if ( !IsInactiveLineEdited( inactiveLineNumber ) )
                {
                ClearVMBXNumberFromPhone( inactiveLineNumber );
                }
            VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNumberStores: Clear old VMBX number" );
            }
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::HandleNumberStores: <=" );
    return error;
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::UsesSimMemory
// Returns if the SIM memory is used on ALS 1
// ----------------------------------------------------------------------------
//
TBool RVmbxNumber::UsesSimMemory()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::UsesSimMemory: =>" );
    TBool ret( EFalse );
    TInt alsline( EAlsLine1 );
    // get the current ALS line
    if ( GetAlsLine( alsline ) != KErrNone )
        {
        alsline = EAlsLine1;  // if problems, assume primary line
        }

    iSession->Get( KVmUsesSimMemory, iMemoryLocation );
    if ( ( iMemoryLocation == EVmbxSimMemory ) && ( alsline == EAlsLine1 ) )
        {
        VMBLOGSTRING( "VMBX: RVmbxNumber::UsesSimMemory: SimMemory active" );
        ret = ETrue;
        }
    VMBLOGSTRING( "VMBX: RVmbxNumber::UsesSimMemory: <=" );
    return ret;
    }

// ----------------------------------------------------------------------------
// RVmbxNumber::IsPhoneMemForcedEdit
// Returns if VMBX number stored in phone memory via external clients
// ----------------------------------------------------------------------------
// 
TBool RVmbxNumber::IsPhoneMemForcedEdit( const TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber:: IsPhoneMemForcedEdit: =>" );
    TInt nbrEdited( NULL );
    
    TUint32 lineNumber = aLineNumber == EAlsLine1 ?
            KUserEditedNumber : KUserEditedNumberAuxiliary;
    
    iSession->Get( lineNumber, nbrEdited );
    
    VMBLOGSTRING2( "VMBX: RVmbxNumber:: IsPhoneMemForcedEdit: value = %d",
            nbrEdited ); 
    return EvmbxPhoneMemForcedEdit == nbrEdited;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::SetPhoneMemForcedEdit
// Sets the flag indicating that external clients have edited VMBX number
// -----------------------------------------------------------------------------    
//
TInt RVmbxNumber::SetPhoneMemForcedEdit( TInt aLineNumber )
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::SetPhoneMemForcedEdit: =>" );

    TUint32 lineNumber = aLineNumber == EAlsLine1 ?
            KUserEditedNumber : KUserEditedNumberAuxiliary;

    VMBLOGSTRING( "VMBX: RVmbxNumber::SetPhoneMemForcedEdit: <=" );
    return iSession->Set( lineNumber, EvmbxPhoneMemForcedEdit );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::ShowInformationNoteL
// Shows information note to user
// -----------------------------------------------------------------------------
//
void RVmbxNumber::ShowInformationNoteL( const TInt aResourceId ) const
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::ShowInformationNoteL: =>" );
    HBufC* prompt = StringLoader::LoadLC( aResourceId, iCoeEnv );

    CAknInformationNote* note = new ( ELeave ) CAknInformationNote ( ETrue );
    note->ExecuteLD( *prompt );

    CleanupStack::PopAndDestroy( prompt );
    VMBLOGSTRING( "VMBX: RVmbxNumber::ShowInformationNoteL: <=" );
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::IsNoNumberNoteAllowed
// check whether the note should be shown
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::IsNoNumberNoteAllowed()
    {
    VMBLOGSTRING( "VMBX: RVmbxNumber::IsNoNumberNoteAllowed: =>" );
    TBool result( EFalse );

    TInt videoRet( KErrNotFound );
    TBool isVideoSupport( VideoSupported() );
    TBool isVoipSupport( IsIpVoiceMailboxServices() );
    TBool psAddressFound( EFalse );

    if ( isVideoSupport )
        {
        HBufC* tmpNumber = HBufC::New( KVmbxMaxNumberLength );
        if ( tmpNumber )
            {
            TPtr tmpPrt( tmpNumber->Des() );
            videoRet = GetVideoMbxNumber( tmpPrt );
            VMBLOGSTRING2( "VMBX: RVmbxNumber::IsNoNumberNoteAllowed \
            get videonum result=%d", videoRet );
            }
        delete tmpNumber;
        tmpNumber = NULL;
        }

    if ( isVoipSupport )
        {
        RArray<TUint> profileIds;
        GetServiceIds( profileIds );

        TVmbxServiceInfo serviceInfo;
        serviceInfo.iAddress.Zero();
        for ( TInt i = 0; i < profileIds.Count() 
        && !psAddressFound; i++ )
            {
            serviceInfo.iServiceId = profileIds[ i ];
            GetServiceInfo( serviceInfo );

            VMBLOGSTRING2( "VMBX: RVmbxNumber::IsNoNumberNoteAllowed \
            PS address length=%d", serviceInfo.iAddress.Length() );
            if ( serviceInfo.iAddress.Length() )
                {
                psAddressFound = ETrue;
                }
            }

        profileIds.Close();
        }

    // This check is done because phone client calls GetVmbxNumber 
    // and then SelectTypeL, which causes note qtn_... to be shown twice
    // so the note can be shown in below several condition:
    // the voice mailbox number is not defined, 
    // when video feature and 
    // voip feature are not supported. 
    // the video number is not defined, when video feature
    // is supported but voip feature is not supported.
    // the voip address is not defined, when voip feature
    // is supported but video feature is not supported.
    // both the video number and the voip address are not
    // defined, when both video feature and 
    // voip feature are supported.

    if ( ( !isVideoSupport && !isVoipSupport )
        || ( ( isVideoSupport && KErrNotFound == videoRet )
        && !isVoipSupport )
        || ( ( isVoipSupport && !psAddressFound
        && !isVideoSupport ) )
        || ( isVideoSupport && isVoipSupport
        && KErrNotFound == videoRet &&
        !psAddressFound ) )
        {
        result = ETrue;
        }

    VMBLOGSTRING( "VMBX: RVmbxNumber::IsNoNumberNoteAllowed: <=" );
    return result;
    }

// -----------------------------------------------------------------------------
// RVmbxNumber::IsInactiveLineEdited
// Check whether the number had been edited in inactive line
// -----------------------------------------------------------------------------
//
TBool RVmbxNumber::IsInactiveLineEdited( TInt& aInactiveLineNumber )
	{
	VMBLOGSTRING( "VMBX: RVmbxNumber::IsInactiveLineEdited: =>" );
	TBool result( EFalse );
	
	TInt activeLineNumber( EAlsLine1 );
	TInt inactiveLineNumber( aInactiveLineNumber );
	
	// get the current ALS line
	if ( KErrNone != GetAlsLine( activeLineNumber ) )
	    {
	    activeLineNumber = EAlsLine1;  
	    }
	else
		{
	    // get the inactive line
	    if ( EAlsLine1 == activeLineNumber )
	    	{
	        inactiveLineNumber = EAlsLine2;
	    	}
	    else
	    	{
	        inactiveLineNumber = EAlsLine1;
	    	}
	    // check whether the number had been edited in inactive line before
	    if ( UserEditNumber( inactiveLineNumber ) 
	    		|| IsPhoneMemForcedEdit( inactiveLineNumber ) )
	    	{
	        result = ETrue;
	    	}
		}
	// get the inactive line number, make it be the output argument
	aInactiveLineNumber = inactiveLineNumber;
	
	VMBLOGSTRING2( "VMBX: RVmbxNumber::IsInactiveLineEdited: result = %d",
	result );
	return result;
	}

//  End of File
