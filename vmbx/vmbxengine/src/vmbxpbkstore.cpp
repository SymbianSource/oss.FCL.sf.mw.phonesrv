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
* Description:  Implementation of the CVmbxPbkStore class
*
*/


// INCLUDE FILES
#include <commonphoneparser.h>
#include <mpbutil.h>
#include <cvoicemailboxentry.h>

#include "vmbxutilities.h"
#include "vmbxetelconnection.h"
#include "vmbxlogger.h"

#include "vmbxpbkstore.h"

//CONSTANTS
// Amount of retries to be performed.
const TInt KVmbxPhonebookBufferSize( 150 );

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxPbkStore::CVmbxPbkStore
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxPbkStore::CVmbxPbkStore(): CActive(EPriorityStandard ),
                                iAsynType( EVmbxSimDefault )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::CVmbxPbkStore =>" );
    CActiveScheduler::Add( this );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::CVmbxPbkStore <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::~CVmbxPbkStore
// destructor
// ---------------------------------------------------------------------------
//
CVmbxPbkStore:: ~CVmbxPbkStore()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::~CVmbxPbkStore =>" );
    Cancel();
    iPhoneBook.Close();
    delete iAlphaStringFromSIM;
    delete iWait;
    delete iETelConnection;
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::~CVmbxPbkStore <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::NewL
// two phase constructor
// ---------------------------------------------------------------------------
//
CVmbxPbkStore* CVmbxPbkStore::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::NewL =>" );
    CVmbxPbkStore* self = new( ELeave ) CVmbxPbkStore();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::ConstructL
// two phase constructor
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ConstructL =>" );
    // Open tel server and phone
    iETelConnection = CVmbxETelConnection::NewL();

    iWait = new( ELeave ) CActiveSchedulerWait; 
    TVmbxAlsLineType alsLine = VmbxUtilities::AlsLine();

    // Supported ALS line
    if ( EVmbxAlsLineDefault != alsLine )
        {
        // open 6f17 file
        User::LeaveIfError( OpenVmbxPhonebook() );
        }
    // Not supported ALS line
    else
        {
        // USIM exist
        if ( SimFileExistsAndReadAccess() )
            {
            // Open 6fc7 file
            User::LeaveIfError( OpenMbdnPhonebook() );
            }
        // USIM not exist, open 6f17 file
        else
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::ConstructL: no mbdn file\
                then open 6f17 file" );
            User::LeaveIfError( OpenVmbxPhonebook() );
            }
        }
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::GetVmbxInfo
// Activates phonebook info query
// ---------------------------------------------------------------------------
TInt CVmbxPbkStore::GetVmbxInfo( 
        RMobilePhoneBookStore::TMobilePhoneBookInfoV1& aInfo )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetVmbxInfo =>" );
    TInt result( KErrInUse );
    RMobilePhoneBookStore::TMobilePhoneBookInfoV1Pckg
                                            InfoPckg( aInfo );
    // get Info from phonebook
    if( !IsActive() && !iWait->IsStarted() )
        {
        iPhoneBook.GetInfo( iStatus, InfoPckg );
        iAsynType = EVmbxSimGetInfo;
        SetActive();
        iWait->Start();

        result = iStatus.Int();
        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::PhonebookInfo: \
                SIM Phonebook info read, status: %I", result );
        }

    VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetVmbxInfo <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::IsWritable
// Sim write support
// ---------------------------------------------------------------------------
//
TBool CVmbxPbkStore::IsWritable()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::IsWritable =>" );
    TBool result( EFalse );
    RMobilePhoneBookStore::TMobilePhoneBookInfoV1 info;
    TInt temp = GetVmbxInfo( info );
    if( KErrNone == temp )
        {
        result = ( info.iCaps &
                RMobilePhoneBookStore::KCapsWriteAccess ? 1 : 0 );
        
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore:: IsWritable: info.iCaps %I",
                     info.iCaps );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore:: IsWritable result %I <=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::PhoneBookType
// PhoneBookType
// ---------------------------------------------------------------------------
//
TVmbxSimPhonebookType CVmbxPbkStore::PhoneBookType()
    {
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::PhoneBookType type %I", 
                         iPhoneBookType );
    return iPhoneBookType;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::Write
// Write to SIM
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::Write( const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::Write =>" );
    TInt result( KErrInUse );
    TBuf8<KVmbxPhonebookBufferSize> pbData;

    CPhoneBookBuffer* pbkBuffer = new CPhoneBookBuffer();
    
    if ( !pbkBuffer )
        {
        VMBLOGSTRING( "VMBX: CVmbxPbkStore::Write: \
        Phonebook creation error" );
        result = KErrNoMemory;
        }
    else
        {
        pbkBuffer->Set( &pbData );
        TInt activeAlsLine = aEntry.VmbxAlsLineType();
        // New entry
        result = pbkBuffer->AddNewEntryTag();
        if ( KErrNone == result )
            {
            // Add index
            // Type of index is TUint16 in Multimode ETel and TInt in old ETel.
            result = pbkBuffer->PutTagAndValue( 
            RMobilePhoneBookStore::ETagPBAdnIndex, (TUint16)activeAlsLine );
            }
        // Add name if it existed on SIM card
        // Type of ETagPBText is TDes16
        if ( KErrNone == result && iAlphaStringFromSIM )
            {
            result = pbkBuffer->PutTagAndValue( 
            RMobilePhoneBookStore::ETagPBText, *iAlphaStringFromSIM );
            // Add number,Type of ETagPBNumber is TDes16
            if ( KErrNone == result )
                {
                TPtrC vmbxNumber( KNullDesC );
                result = aEntry.GetVmbxNumber( vmbxNumber );
                if ( KErrNone == result )
                    {
                    result = pbkBuffer->PutTagAndValue( 
                    RMobilePhoneBookStore::ETagPBNumber, vmbxNumber );
                    }
                }
            }
        if ( KErrNone == result )
            {
            if ( iPhoneBookType == EMBDNPhoneBook )
                {
                RMobilePhone::TMobilePhoneVoicemailIdsV3 mbdnInfo;
                result = GetMbdnInfo( EVmbxAlsLine1, mbdnInfo );

                if ( KErrNone == result )
                    {
                    TInt index = mbdnInfo.iVoice;
                    if( !IsActive() && !iWait->IsStarted() )
                        {
                        // write vmbx number to 6fc7 file
                        iPhoneBook.Write( iStatus, pbData, index );
                        // Wait for asynchronous call to finish
                        iAsynType = EVmbxSimEntryWrite;
                        SetActive();
                        iWait->Start();
                        result = iStatus.Int();
                        }
                    }
                VMBLOGSTRING( "Mbdn writing" );
                }
            else
                {
                if( !IsActive() && !iWait->IsStarted() )
                    {
                    // write vmbx number to 6f17 file
                    iPhoneBook.Write( iStatus, pbData, activeAlsLine );
                    iAsynType = EVmbxSimEntryWrite;
                    // Wait for asynchronous call to finish
                    SetActive();
                    iWait->Start();
                    result = iStatus.Int();
                    }
                VMBLOGSTRING( "CVmbxPbkStore::Write 6f17 writing" );
                }
            }
        }
    delete pbkBuffer;
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::Write: result=%I<=",  result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::PhonebookStore
// Return Phonebook
// ---------------------------------------------------------------------------
RMobilePhoneBookStore& CVmbxPbkStore::PhonebookStore()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::PhonebookStore <=>" );
    return iPhoneBook;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::GetMbdnInfo
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::GetMbdnInfo( const TVmbxAlsLineType aAlsLine,
            RMobilePhone::TMobilePhoneVoicemailIdsV3& aInfo )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetMbdnInfo =>" );
    TInt result( KErrInUse );
    // Get identifiers
    if ( !IsActive() && !iWait->IsStarted() )
        {
        VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetMbdnInfo: GetMailboxNumbers" );
        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo: iStatus %I",
                                                             iStatus.Int() );
        RMobilePhone::TMobilePhoneVoicemailIdsV3Pckg infoPckg( aInfo );
        iETelConnection->Phone().GetMailboxNumbers( iStatus, infoPckg );
        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo :iStatus %I",
                                                             iStatus.Int() );
        iAsynType = EVmbxSimMbdnInfo;
        // Wait for asynchronous call to finish
        SetActive();
        iWait->Start();

        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo: iVoice value %I",
                                                             aInfo.iVoice );
        if ( iStatus.Int() == KErrNotFound )
            {
            result = KErrNone;
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetMbdnInfo: KErrNotFound,\
             but igorned and change to KErrNone" );
            }
        else
            {
            result = iStatus.Int();
            }

        if ( EVmbxAlsLineDefault == aAlsLine && KErrNone == result )
            {
            aInfo.iVoice =  EVmbxAlsLine1;
            }
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo: result %I<=",
         result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::OpenMbdnPhonebook
// opening mbdn-type phonebook
// ---------------------------------------------------------------------------
//   
TInt CVmbxPbkStore::OpenMbdnPhonebook()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::OpenMbdnPhonebook =>" );
    // try to open mbdn-type phonebook
    TInt result = iPhoneBook.Open( iETelConnection->Phone(),
                                         KETelIccMbdnPhoneBook );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::OpenMbdnPhonebook :\
        MBDN opening result = %I", result );
    if ( KErrNone == result )
        {
        iPhoneBookType = EMBDNPhoneBook;
        }
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::OpenMbdnPhonebook <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::OpenVmbxPhonebook
// opening vmbx-type phonebook
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::OpenVmbxPhonebook()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::OpenVmbxPhonebook =>" );
    // try to open vmbx-type phonebook
    TInt result = iPhoneBook.Open( iETelConnection->Phone(),
                                                 KETelIccVoiceMailBox );
    if ( KErrNone == result )
        {
        iPhoneBookType = EVMBXPhoneBook;
        }
    VMBLOGSTRING2( "Vmbx phonebook opening result = %I ", result );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::OpenVmbxPhonebook <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::GetL
// Fetches mailbox number from Sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::GetL( CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetL: iPhoneBookType %d=>",
                         iPhoneBookType );

    if ( EMBDNPhoneBook == iPhoneBookType )
        {
        TRAPD( err, SimReadL( aEntry ) );

        TPtrC vmbxNumber( KNullDesC );
        if ( KErrNone == err )
            {
            err = aEntry.GetVmbxNumber( vmbxNumber );
            }

        // 6fc7 file empty
        if ( ( KErrNone != err ) || ( !vmbxNumber.Length() ) )
            {
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetL: \
            no vmbx number in 6fc7 file then read from 6f17 file err%I", err );
            // close 6fc7 and open 6f17 file
            iPhoneBook.Close();
            User::LeaveIfError( OpenVmbxPhonebook() );
            // read vmbx number from 6f17 file
            SimReadL( aEntry );
            } 
        }
    else
        {
        SimReadL( aEntry );
        }

    VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::SimReadL
// read vmbx number from sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::SimReadL( CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::SimReadL =>" );

    TInt numEntries( 1 );
    TBuf8<KVmbxPhonebookBufferSize> pbData;

    delete iAlphaStringFromSIM;
    iAlphaStringFromSIM = NULL;
    TInt result( KErrInUse );
    if ( iPhoneBookType == EMBDNPhoneBook )
        {
        RMobilePhone::TMobilePhoneVoicemailIdsV3 mbdnInfo;
        result = GetMbdnInfo( EVmbxAlsLine1, mbdnInfo );
        if ( KErrNone == result )
            {
            VMBLOGSTRING( "start MBDN PhoneBook read" );
            if( !IsActive() && !iWait->IsStarted() )
                {
                // read vmbx number from 6fc7 file
                iPhoneBook.Read( 
                iStatus, mbdnInfo.iVoice, numEntries, pbData );
                iAsynType = EVmbxSimEntryRead;
                // Wait for asynchronous call to finish
                SetActive();
                iWait->Start();
                }         
            }
        }
    else
        {
        // Record#1 in sim is for line1 number and Record#2 in sim is for
        // line2 number so line is used to fetch
        VMBLOGSTRING( "start VMBX PhoneBook read" );
        TInt activeAlsLine = aEntry.VmbxAlsLineType();
         if( !IsActive() && !iWait->IsStarted() )
            {
            result = KErrNone;
            // read vmbx number from 6f17 file
            iPhoneBook.Read( iStatus, activeAlsLine, numEntries, pbData );
            iAsynType = EVmbxSimEntryRead;
            // Wait for asynchronous call to finish
            SetActive();
            iWait->Start();
            }
        }

    User::LeaveIfError( result );
    // asynchronous call finished
    VMBLOGSTRING2( "PhoneBook read iStatus = %I", iStatus.Int() );

    if( iStatus.Int() == KErrNone )
        {
        ParseDataL( aEntry, pbData );
        }
    else
        {
        User::Leave( iStatus.Int() );
        }

    VMBLOGSTRING( "VMBX: CVmbxPbkStore::SimReadL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::ParseDataL
// read vmbx number from sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::ParseDataL( CVoiceMailboxEntry& aEntry, TDes8& aPbData )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ParseDataL =>" );
    TInt result( KErrNotFound );
    delete iAlphaStringFromSIM;
    iAlphaStringFromSIM = NULL;

    VMBLOGSTRING2( "CVmbxPbkStore::ParseDataL Lengh = %d", aPbData.Length() );

    if ( aPbData.Length() )
        {
        // -> Search Tags "ETagPBNumber" and "ETagPBText"
        // and read (decode) them
        // create buffer
        CPhoneBookBuffer* pbkBuffer = new( ELeave ) CPhoneBookBuffer();
        CleanupStack::PushL( pbkBuffer );

        // start read
        pbkBuffer->Set( &aPbData );
        pbkBuffer->StartRead();

        result = ReadNewEntryTag( pbkBuffer );
        // Read first "new-entry-tag"
        if ( KErrNone == result )
            {
            ReadPbkDataL( pbkBuffer, aEntry );
            }
        CleanupStack::PopAndDestroy( pbkBuffer );
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ParseDataL: result %I", result );
    User::LeaveIfError( result );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ParseDataL <=" );
    }


// ---------------------------------------------------------------------------
// CVmbxPbkStore::ReadNewEntryTag
// Reads "new-entry-tag" from phonebook data from Sim
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::ReadNewEntryTag( CPhoneBookBuffer* aPbkBuffer )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadNewEntryTag =>" );
    TInt result( KErrNone );
    TUint8 tagValue( 0 );
    CPhoneBookBuffer::TPhBkTagType dataType(
                               CPhoneBookBuffer::EPhBkTypeNoData );

    result = aPbkBuffer->GetTagAndType( tagValue, dataType );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ReadNewEntryTag result = %d",
     result );

    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ReadNewEntryTag tagValue = %X",
     tagValue );
    if ( tagValue != RMobilePhoneBookStore::ETagPBNewEntry )
        {
        VMBLOGSTRING( "VMBX: CVmbxPbkStore::ParseDataL: Unknown result" );
        result = KErrArgument;  // Something wrong in TLV
        }
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadNewEntryTag <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::ReadPbkData
// Reads tags from buffer retrieved from sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::ReadPbkDataL( CPhoneBookBuffer* aPbkBuffer,
                                         CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadPbkDataL =>" );

    TInt result( KErrNone );
    TUint8 tagValue( 0 );
    CPhoneBookBuffer::TPhBkTagType dataType(
                                          CPhoneBookBuffer::EPhBkTypeNoData );

    result = aPbkBuffer->GetTagAndType( tagValue, dataType );

    // loop through data to find a number and an possible alpha string
    while ( result == KErrNone && aPbkBuffer->RemainingReadLength() > 0 )
        {
        VMBLOGSTRING2("VMBX: CVmbxPbkStore::ReadPbkDataL: tagValue: %d",
         tagValue );
        VMBLOGSTRING2("VMBX: CVmbxPbkStore::ReadPbkDataL: dataType: %d",
         dataType );

        // Check for text field
        if ( tagValue == RMobilePhoneBookStore::ETagPBText )
            {
            // Alpha string field found from TLV entry,
            // assuming 16bit data
            TPtrC16 alphaPtrC;
            result = aPbkBuffer->GetValue( alphaPtrC );
            if ( KErrNone == result )
                {
                iAlphaStringFromSIM = alphaPtrC.AllocL();
                VMBLOGSTRING2("VMBX: CVmbxPbkStore::ReadPbkDataL: \
                iAlphaStringFromSIM: %S", iAlphaStringFromSIM );
                }
            }

         // Check for number field
         else if ( tagValue == RMobilePhoneBookStore::ETagPBNumber )
            {
            // Number field found from TLV entry, assuming 16bit data
            TPtrC16 numberPtrC;
            result = aPbkBuffer->GetValue( numberPtrC );

            VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadPbkDataL:\
                                            ETagPBNumber found" );

            if ( KErrNone == result )
                {
                // Error code is returned if number not fit to
                // local buffer.
                if ( numberPtrC.Length() > KVmbxMaxNumberLength )
                    {
                    result = KErrOverflow;
                    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadPbkDataL: \
                                            Overflow error" );
                    }
                else
                    {
                    result = aEntry.SetVmbxNumber( numberPtrC );
                    VMBLOGSTRING("VMBX: CVmbxPbkStore::ReadPbkDataL: \
                                    ETagPBNumber found Number" );
                    break;
                    }
                }
            }
         else
            {
            // skip field
            aPbkBuffer->SkipValue( dataType );
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadPbkDataL: SkipValue" );
            }
        // read next if no errors
        if ( KErrNone == result )
            {
            // Read next field type
            result = aPbkBuffer->GetTagAndType( tagValue, dataType );
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ReadPbkDataL: \
                                    next GetTagAndType = %I", result );
            }
        }

    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ReadPbkDataL result=%I", result );
    User::LeaveIfError( result );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadPbkDataL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::IsUsimSupport
// Checks if USim card in the phone
// ---------------------------------------------------------------------------
TBool CVmbxPbkStore::SimFileExistsAndReadAccess()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::SimFileExistsAndReadAccess =>" );
    TBool result( EFalse );

    TUint32 capability;
    TInt err = iETelConnection->Phone().GetIccAccessCaps( capability );
    VMBLOGSTRING2( "VMBX: CVmbxSimHandler::SimFileExistsAndReadAccess:\
        Err = %I", err);

    if ( err == KErrNone &&
        // Phone has a USIM and it currently supports USIM access by clients.
        // the current sim card is 3G
         capability & RMobilePhone::KCapsUSimAccessSupported )
        {
        result = ETrue;
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::SimFileExistsAndReadAccess \
        return = %d <=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::RunL
// read vmbx number from sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::RunL()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::RunL =>" );
    if ( iWait->IsStarted() )
        {
        // stop blocking
        iWait->AsyncStop();
        VMBLOGSTRING( "VMBX: CVmbxSimHandler::RunL: AsyncStop" );
        }
    iAsynType = EVmbxSimDefault;
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::RunL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::DoCancel
// read vmbx number from sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::DoCancel()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel =>" );
    if ( iWait->IsStarted() )
        {
        // stop blocking
        iWait->AsyncStop();
        VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel: AsyncStop" );
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::DoCancel: iAsynType %I",iAsynType );
    switch ( iAsynType )    
        {
        // only for USIM
        case EVmbxSimMbdnInfo:
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel : \
                                        EVmbxSimMbdnInfo" );
            iETelConnection->Phone().CancelAsyncRequest( 
                                              EMobilePhoneGetMailboxNumbers );
            break;
            }
        case EVmbxSimGetInfo:
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel : \
                                              EVmbxSimGetInfo" );
            iETelConnection->Phone().CancelAsyncRequest( 
                                               EMobilePhoneStoreGetInfo );
            break;
            }            
        case EVmbxSimEntryRead:
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel : EVmbxSimEntryRead" );
            iETelConnection->Phone().CancelAsyncRequest( 
                                               EMobilePhoneStoreRead );
            break;
            }                                        
        case EVmbxSimEntryWrite:
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel : EVmbxSimEntryWrite" );
            iETelConnection->Phone().CancelAsyncRequest( 
                                               EMobilePhoneStoreWrite );
            break;
            }                                        
        default:
            break;   
        }
    
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::RunError
// read vmbx number from sim
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::RunError(TInt aError)
    {
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::RunError: %I", aError );
    return KErrNone;
    }

//End of file
