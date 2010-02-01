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
* Description:  Interface for sending messages.
*
*/



// INCLUDE FILES
#include "CPhCltUssdImp.h"
#include "CPhCltUssdRequestHandler.h"
#include "PhCltClientServer.h"
#include <PhCltTypes.h>

#include <etelmm.h>
#include <f32file.h>
#include <charconv.h>
#include <gsmuelem.h>
//
#include <coemain.h>
#include <avkon.hrh>
#include <avkon.rsg> 

#include <PhoneClient.rsg>
#include <exterror.h>

#include <gsmerror.h>
#include <etelsat.h>

#include "MPhCltUssdNoteControllerCallBack.h"
#include "CPhCltUssdNoteController.h"

// CONSTANTS

const TUint8 KPhCltUssdDcsDefaultAlphabet = 0x0f; // 00001111
                       
const TUint8 KPhCltUssdDcsAlphabetDefaultPrecededLanguage = 0x10; // 00010000 
const TUint8 KPhCltUssdDcsAlphabetDefaultPrecededLanguageSkipChars3 = 3;
const TUint8 KPhCltUssdDcsAlphabetDefaultPrecededLanguageSkipChars2 = 2;
const TUint8 KPhCltUssdDcsAlphabetDefaultPrecededLanguageStartBit = 5;

const TUint8 KPhCltUssdDcsAlphabetUCS2PrecededLanguage = 0x11;   // 00010001
const TUint8 KPhCltUssdDcsAlphabetUCS2PrecededLanguageSkipChars = 2;

const TUint8 KPhCltUssdDcsGeneralInformationMask = 0xc0;         // 11000000
const TUint8 KPhCltUssdDcsGeneralInformation = 0x40;             // 01xxxxxx
const TUint8 KPhCltUssdDcsGeneralInformationCompressed = 0x20;   // xx1xxxxx
const TUint8 KPhCltUssdDcsGeneralInformationSimMask = 0x13;      // 00010011
const TUint8 KPhCltUssdDcsGeneralInformationSim = 0x12;          // xxx1xx10
const TUint8 KPhCltUssdDcsGeneralInformationAlphabetMask = 0x0c; // 00001100
const TUint8 KPhCltUssdDcsGeneralInformationAlphabet8Bit = 0x04; // xxxx01xx 
const TUint8 KPhCltUssdDcsGeneralInformationAlphabetUCS2 = 0x08; // xxxx10xx 

const TUint8 KPhCltUssdDcsMessageHandlingAlphabetMask = 0xf4;    // 11110100
const TUint8 KPhCltUssdDcsMessageHandlingAlphabet8Bit = 0xf4;    // 1111x1xx 

const TUint8 KPhCltUssdCarriageReturn = 0x0d;
const TUint8 KPhCltUssdDcsNotSet  = 0x00; // not set


// ============================= LOCAL FUNCTIONS ===============================

// CLASS DECLARATION

#ifndef TPHCLTUSSDALPHABETPACKER_H
#define TPHCLTUSSDALPHABETPACKER_H

/**
* Class for packing the Ussd string.
*/
class TPhCltUssdAlphabetPacker
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        *
        * @param aAlphabet The used Data Coding Scheme.
        * @param aIsBinary Is the data binary.
        * @param aStartBit The start bit of the conversion.
        */
        TPhCltUssdAlphabetPacker(
            TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
            TBool aIsBinary,
            TInt aStartBit );

    public:  // New functions

        /**
        * Pack the given string to the desired coding scheme.
        *
        * @param aOut The output string of packing
        * @param aIn The string to be packed.
        * @return The number of octets used.
        */
        TInt PackL(TDes8& aOut,const TDesC8& aIn);

        /**
        * Convert and pack the given string to the desired coding scheme.
        *
        * @param aCharacterSetConverter The character converter.
        * @param aFs Reference to file system.
        * @param aOut The output string of packing
        * @param aIn The string to be packed.
        * @param aConvertedNumUDUnits The number of converted units.
        * @return The number of octets used.
        */
        TInt ConvertAndPackL(
            CCnvCharacterSetConverter& aCharacterSetConverter,
            RFs& aFs,
            TDes8& aOut,
            const TDesC& aIn,
            TInt& aConvertedNumUDUnits );

        /**
        * Return information of how many octets are required when
        * the aNumUDUnits number of characters is packed.
        *
        * @param aNumUDUnits The number of characters to be packed.
        * @return The number of octets required.
        */
        TInt PackedOctetsRequiredL( TInt aNumUDUnits ) const;
        
    private:

        // Private helper methods
        TInt ElementSizeInBitsL() const;
        
    private:    // Data

        // The Data Coding Scheme of the conversion.
        TSmsDataCodingScheme::TSmsAlphabet iAlphabet;

        // Information is the given descriptor binary.
        TBool iIsBinary;

        // The start bit of the conversion.
        TInt iStartBit;
    };

#endif // TPHCLTUSSDALPHABETPACKER_H


// -----------------------------------------------------------------------------
// TPhCltUssdAlphabetPacker::TPhCltUssdAlphabetPacker
//
// -----------------------------------------------------------------------------
//
TPhCltUssdAlphabetPacker::TPhCltUssdAlphabetPacker(
    TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
    TBool aIsBinary,
    TInt aStartBit)
    : iAlphabet( aAlphabet ),
      iIsBinary( aIsBinary ),
      iStartBit( aStartBit )
    {
    }


// -----------------------------------------------------------------------------
// TPhCltUssdAlphabetPacker::PackL
//
// -----------------------------------------------------------------------------
//
TInt TPhCltUssdAlphabetPacker::PackL( TDes8& aOut,const TDesC8& aIn )
    {
    // Ensure we've got the right length
    TInt packedOctetsRequired = PackedOctetsRequiredL( aIn.Length() );
    if ( packedOctetsRequired > ( aOut.MaxLength() - aOut.Length() ) )
        {
        User::Leave( KErrOverflow );
        }

    // Do the conversion
    TInt elementSizeInBits = ElementSizeInBitsL();
    if ( elementSizeInBits == 8 ) // 8 bit data
        {
        // Straight copy here
        aOut.Append( aIn );
        }
    else if ( elementSizeInBits == 7 ) // Need packing to 7-bit
        {
        // Get raw pointers and do packing
        TUint8* outPtr = ( TUint8* )aOut.Ptr() + aOut.Length();
        const TUint8* inPtr = aIn.Ptr();
        
        outPtr[0] = 0;
        for ( TInt i = 0; i < aIn.Length() ; i++ )
            {
            TUint8 to = inPtr[i];
            *outPtr |= ( to << iStartBit );
            if ( iStartBit )
                {
                outPtr++;
                *outPtr = ( TUint8 ) ( to >> ( 8 - iStartBit ) );
                }
            iStartBit = ( iStartBit + 7 )%8; // roll 0,1,2,3,4,5,6,7,0,1,2,...
            
            if ( i == aIn.Length() - 1 ) // if this is the last time
                {
                if ( ( to == KPhCltUssdCarriageReturn && iStartBit == 0 ) || 
                     iStartBit == 1 )
                    {
                    //We have to add one CR more
                    *outPtr |= ( KPhCltUssdCarriageReturn << iStartBit );
                    if ( iStartBit )
                        {
                        outPtr++;
                        *outPtr = ( TUint8 ) (
                            KPhCltUssdCarriageReturn >> ( 8 - iStartBit ) );
                        }
                    else
                        {
                        packedOctetsRequired++;
                        }
                    iStartBit = ( iStartBit + 7 )%8;
                    }
                }
            }
        // Increment the length for the packed data
        aOut.SetLength( aOut.Length() + packedOctetsRequired );
        }
    else
        {
        User::Invariant();
        }
    // Return number of bytes used
    return packedOctetsRequired;
    }


// -----------------------------------------------------------------------------
// TPhCltUssdAlphabetPacker::ConvertAndPackL
//
// -----------------------------------------------------------------------------
//
TInt TPhCltUssdAlphabetPacker::ConvertAndPackL(
    CCnvCharacterSetConverter& aCharacterSetConverter,
    RFs& aFs,
    TDes8& aOut,
    const TDesC& aIn,
    TInt& aConvertedNumUDUnits )
    {
    
    // Do the conversion
    CSmsAlphabetConverter* converter = 
        CSmsAlphabetConverter::NewLC( 
            aCharacterSetConverter, aFs, iAlphabet, iIsBinary );

    TPtrC8 convertedPtr = converter->ConvertFromNativeL( aIn );
    aConvertedNumUDUnits = convertedPtr.Length();

    // Do the packing
    TInt octetsUsed = PackL( aOut,convertedPtr );

    // Cleanup and return
    CleanupStack::PopAndDestroy(); // converter
    return octetsUsed;
    }


// -----------------------------------------------------------------------------
// TPhCltUssdAlphabetPacker::PackedOctetsRequiredL
//
// -----------------------------------------------------------------------------
//
TInt TPhCltUssdAlphabetPacker::PackedOctetsRequiredL( TInt aNumUDUnits ) const

    {  
    TInt octetsRequired = 0;
    TInt elementSizeInBits = ElementSizeInBitsL();
    if ( elementSizeInBits == 8 )
        {
        octetsRequired=aNumUDUnits;
        }
    else
        {
        octetsRequired =
            ( iStartBit + aNumUDUnits*elementSizeInBits + 7 )/8;    // Rounds up
        }
    return octetsRequired;
    }


// -----------------------------------------------------------------------------
// TPhCltUssdAlphabetPacker::ElementSizeInBitsL
//
// -----------------------------------------------------------------------------
//
TInt TPhCltUssdAlphabetPacker::ElementSizeInBitsL() const
    {   
    if ( iIsBinary )
        return 8;
    switch ( iAlphabet )
        {
        case TSmsDataCodingScheme::ESmsAlphabet7Bit:
            {
            return 7;
            }
        case TSmsDataCodingScheme::ESmsAlphabet8Bit:
        case TSmsDataCodingScheme::ESmsAlphabetUCS2:
            {
            return 8;
            }
        default:
            {
            User::Leave( KErrGsmSMSDataCodingSchemeNotSupported );
            return 8;
            }
        }
    }




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltUssdImp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCltUssdImp* CPhCltUssdImp::NewL( TBool aShowNotes )
    {
    CPhCltUssdImp* self = new( ELeave ) CPhCltUssdImp;
    CleanupStack::PushL( self );
    self->ConstructL( aShowNotes );
    CleanupStack::Pop(); // self
    return self;
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhCltUssdImp::ConstructL( TBool aShowNotes )
    {
    // The note controller is needed only if the notes are shown.
    iNoteController = NULL;
    
    if ( aShowNotes )
        {
        iNoteController = CPhCltUssdNoteController::NewL( 
            *this, 
            CActive::EPriorityStandard );
        }

    iDCS = KPhCltUssdDcsDefaultAlphabet;
    iWait = new( ELeave ) CActiveSchedulerWait;

    // The one that send the request forward.
    iRequestHandler = CPhCltUssdRequestHandler::NewL( 
        *this, 
        CActive::EPriorityStandard );
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::CPhCltUssdImp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltUssdImp::CPhCltUssdImp()
    {
    } 


// -----------------------------------------------------------------------------
// CPhCltUssdImp::~CPhCltUssdImp
// Destructor.
// -----------------------------------------------------------------------------
//
CPhCltUssdImp::~CPhCltUssdImp()
    {
    if ( iWait )
        {
        if ( iWait->IsStarted() )
            {
            iWait->AsyncStop();
            }
        delete iWait;
        iWait = NULL;
        }

    delete iRequestHandler;
    iRequestHandler = NULL;
    
    delete iNoteController;
    iNoteController = NULL;      
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::HandleSendEventL
// 
// Called when the Send request is completed. aError might be
// positive -> the Send is completed due received message.
// -----------------------------------------------------------------------------
//
void CPhCltUssdImp::HandleSendEventL( const TInt aError )
    {
    *iSendError = Min( aError , KErrNone );

    // iNoteController is allocated only if notes are shown.
    if ( iNoteController )
        {
        // Send request is completed, now destroy the wait note
        iNoteController->DestroyGlobalWaitNote();

        // Show a different note depenging on the error code.
        if ( aError <= 0 )
            {
            switch ( aError )
                {
                // All Ok.
                case KErrNone:
                    iNoteController->ShowGlobalConfirmationNoteL( R_TEXT_DONE );
                    break;

                // Operation cancelled.
                case KErrCancel:
                    iNoteController->ShowGlobalInformationNoteL( R_TEXT_UNCONFIRMED );
                    break;

                // Ongoing Ussd session or the string is barred due SS request
                // or Fixed Dialling feature.
                case KErrInUse:
                case KErrAccessDenied:
                case KErrGsmSSCallBarred:
                    iNoteController->ShowGlobalInformationNoteL( R_TEXT_NOT_ALLOWED );
                    break;

                // No network coverage.
                case KErrGsmSMSNoNetworkService:
                    iNoteController->ShowGlobalInformationNoteL( R_TEXT_NO_SERVICE );
                    break;

                // Offline mode.
                case KErrGsmOfflineOpNotAllowed:
                    iNoteController->ShowGlobalInformationNoteL( R_TEXT_OFFLINE );
                    break;
                    
                case KErrSatControl:
                    break; 

                // Unknown error.
                default:
                    iNoteController->ShowGlobalInformationNoteL( R_TEXT_NOT_DONE );
                    break;
                }
            }
        }

    // Let the original active object (one that did the send request) run again.
    if ( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::SendUssd
// 
// 16-bit buffer is packed into 7-bit stream and sent.
// Default alphabet is used.
// -----------------------------------------------------------------------------
//
TInt CPhCltUssdImp::SendUssd( const TDesC& aMsgData )
    {
    __ASSERT_ALWAYS( aMsgData.Length() <= KPhCltUssdMax8BitCharacters,
        User::Invariant() );

    TRAPD( err ,
        {
        HBufC8* buffer8 = HBufC8::NewLC( KPhCltUssdMax7BitCharacterOctets );
        
        TPtr8 ptr8 = buffer8->Des();
        EncodeL( aMsgData , ptr8 );
        
        User::LeaveIfError( SendUssd( ptr8 ) );
        
        CleanupStack::PopAndDestroy(); // ptr8
        });

    return err;
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::SendUssd
// 
// Packed buffer is send to the network with default alphabet.
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/

TInt CPhCltUssdImp::SendUssd( const TDesC8& aMsgData )
    {
    __ASSERT_ALWAYS( aMsgData.Length() <= KPhCltUssdMax7BitCharacterOctets,
        User::Invariant() );
 
    return SendUssd( aMsgData , iDCS );
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::SendUssd
// 
// Packed buffer is sent to network with given alphabet.
// -----------------------------------------------------------------------------
//
TInt CPhCltUssdImp::SendUssd( 
    const TDesC8& aMsgData, 
    const TUint8 iSendDcs )
    {
    __ASSERT_ALWAYS( aMsgData.Length() <= KPhCltUssdMax7BitCharacterOctets,
        User::Invariant() );

	RMobileUssdMessaging::TMobileUssdAttributesV1 attribute;

    attribute.iFlags = 
        RMobileUssdMessaging::KUssdDataFormat + 
        RMobileUssdMessaging::KUssdMessageDcs;
    if ( iDCS2  == KPhCltDcs7Bit )
        {
        attribute.iFormat = RMobileUssdMessaging::EFormatPackedString;
        }
    else
        {
        attribute.iFormat = RMobileUssdMessaging::EFormatUnspecified;
        }
        
    if ( iSendDcs == KPhCltUssdDcsNotSet  ) // 0x00
		{
		attribute.iDcs = KPhCltUssdDcsDefaultAlphabet;
 		}
 	else
 		{
 		attribute.iDcs = iSendDcs;
 		} 
    
    RMobileUssdMessaging::TMobileUssdAttributesV1Pckg 
        attributePckg( attribute );


    if ( iWait->IsStarted() )
        {
        return KErrInUse;
        }

    TInt error = KErrNone;
    iSendError = &error;
  
  	iRequestHandler->SendUssd( aMsgData , attributePckg );
    // iNoteController is allocated only if notes are shown.
    if ( iNoteController )
        {
        TRAP_IGNORE( iNoteController->ShowGlobalWaitNoteL( 
        	R_TEXT_SENDING, 
            R_AVKON_SOFTKEYS_QUIT ) );
        }

    // Set this active object to wait the completion of the send request.
    iWait->Start();

    // If not deleted:
    if ( iWait )
        {
        iSendError = NULL;
        }
    return error;
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::SendUssdCancel
// 
// Cancels the out-standing request.
// -----------------------------------------------------------------------------
//
void CPhCltUssdImp::SendUssdCancel()
    {
    iRequestHandler->SendUssdCancel();
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::StartUssdEditor
// 
// Request Ussd App to start
// ---------------------------------------------------------------------------
//
TInt CPhCltUssdImp::StartUssdEditor() const
    {
    return iRequestHandler->UssdClient().StartUssdEditor();
    }

// -----------------------------------------------------------------------------
// CPhCltUssdImp::AppStarting
// 
// UI informs that it is been created
// -----------------------------------------------------------------------------
//
TInt CPhCltUssdImp::AppStarting()
    {
    return iRequestHandler->UssdClient().AppStarting();
    }
    
// -----------------------------------------------------------------------------
// CPhCltUssdImp::AppTerminating
// 
// UI informs that it is terminating
// -----------------------------------------------------------------------------
//    
TInt CPhCltUssdImp::AppTerminating( 
      TPhCltUssdAppExitReason aExitReason )
    {
    return iRequestHandler->UssdClient().AppTerminating( aExitReason );
    }
    
// -----------------------------------------------------------------------------
// CPhCltUssdImp::AppToForeground
// 
// UI informs that it is brougth to foreground
// -----------------------------------------------------------------------------
//    
TBool CPhCltUssdImp::AppToForeground()
    {
    return iRequestHandler->UssdClient().AppToForeground();
    }
    
// -----------------------------------------------------------------------------
// CPhCltUssdImp::AppToBackground
// 
// UI informs that it is gone background
// -----------------------------------------------------------------------------
//    
TInt CPhCltUssdImp::AppToBackground()
    {
    return iRequestHandler->UssdClient().AppToBackground();
    }

// -----------------------------------------------------------------------------
// CPhCltUssdImp::StartSAT
// 
// 
// -----------------------------------------------------------------------------
//    
void  CPhCltUssdImp::StartSAT( 
            TRequestStatus& aStatus, 
            TDes& aReceiveMessage, 
            TPckg< TUint >& aShowNotesAndDcs )
    {
    iRequestHandler->UssdClient().StartSAT( aStatus, aReceiveMessage, aShowNotesAndDcs );
    }
 
// -----------------------------------------------------------------------------
// CPhCltUssdImp::StopSAT
// 
// Cancels SAT session
// -----------------------------------------------------------------------------
//   
 void CPhCltUssdImp::StopSAT()
    {
    iRequestHandler->UssdClient().StopSAT();
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::GlobalNoteDismissedL
//
// Dialog is cancelled by the right softkey.
// -----------------------------------------------------------------------------
//
void CPhCltUssdImp::GlobalWaitNoteDismissedL( TInt aButtonId )
    {
    if ( aButtonId == EAknSoftkeyClose || 
         aButtonId == EAknSoftkeyCancel ||
         aButtonId == EAknSoftkeyQuit )
        {   
        SendUssdCancel();
        }
    }


// -----------------------------------------------------------------------------
// CPhCltUssdImp::EncodeL
//
// Converts a given Uniocde string into 7-bit buffer.
// -----------------------------------------------------------------------------
//
void CPhCltUssdImp::EncodeL( const TDesC& aSrc, TDes8& aDes )
    {
    //
     aDes.Zero();
    TSmsDataCodingScheme::TSmsAlphabet alphabet = 
        TSmsDataCodingScheme::ESmsAlphabet7Bit; // default
        
    CCnvCharacterSetConverter* charConv = CCnvCharacterSetConverter::NewLC();
    
    TUint cutChars = 0;
    TUint startBit = 0;

    if ( iDCS == KPhCltUssdDcsAlphabetDefaultPrecededLanguage )
        {
        alphabet = TSmsDataCodingScheme::TSmsDataCodingScheme::ESmsAlphabet7Bit;
        FindFirstCarriageReturnL( aSrc , cutChars , startBit );
        
        }
    else if ( iDCS == KPhCltUssdDcsAlphabetUCS2PrecededLanguage )
        {
        alphabet = TSmsDataCodingScheme::ESmsAlphabetUCS2;
        cutChars = KPhCltUssdDcsAlphabetUCS2PrecededLanguageSkipChars;
        }
    else if ( (iDCS & KPhCltUssdDcsGeneralInformationMask)  == 
            KPhCltUssdDcsGeneralInformation ) // General data coding information
        {
        //Is text compressed?
        if ( iDCS & KPhCltUssdDcsGeneralInformationCompressed )
            {
            User::Leave( KErrNotSupported );
            }
        // Is SIM specific message
        else if ( ( iDCS & KPhCltUssdDcsGeneralInformationSimMask ) 
                   == KPhCltUssdDcsGeneralInformationSim )
            {
            User::Leave( KErrAbort ); // Do not show any messages
            }
        // 8 bit data?
        else if ( ( iDCS & KPhCltUssdDcsGeneralInformationAlphabetMask ) ==  
                   KPhCltUssdDcsGeneralInformationAlphabet8Bit )
            {
            alphabet = TSmsDataCodingScheme::ESmsAlphabet8Bit;
            }
        // UCS2 bit data?
        else if ( ( iDCS & KPhCltUssdDcsGeneralInformationAlphabetMask ) == 
                   KPhCltUssdDcsGeneralInformationAlphabetUCS2 )
            {
            alphabet = TSmsDataCodingScheme::ESmsAlphabetUCS2;
            }
        }
    // Data coding/message handling
    else if ( ( iDCS & KPhCltUssdDcsMessageHandlingAlphabetMask ) 
               == KPhCltUssdDcsMessageHandlingAlphabet8Bit )
        {
        alphabet = TSmsDataCodingScheme::ESmsAlphabet8Bit;
        }
    
    TPhCltUssdAlphabetPacker* packer = 
        new ( ELeave ) TPhCltUssdAlphabetPacker( 
            alphabet, EFalse, startBit );
        CleanupStack::PushL( packer );
        
   
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    TInt numberOfElem = 0;
   // packer->ConvertAndPackL( 
    //    *charConv , fs , aDes , aSrc , numberOfElem );
        
        
    if ( alphabet == TSmsDataCodingScheme::ESmsAlphabet7Bit )
        {
        //TInt numberOfElem = (( aSrc.Length() - cutChars ) * 8 - startBit ) / 7;
            
         packer->ConvertAndPackL( 
        *charConv , fs , aDes , aSrc.Right( aSrc.Length() - cutChars ), numberOfElem );

        // DCS was 7-bit data.
        iDCS2 = KPhCltDcs7Bit; 
        }
    else // ESmsAlphabet8Bit || ESmsAlphabetUCS2
        {
        CSmsAlphabetConverter* converter =
            CSmsAlphabetConverter::NewLC(
                *charConv ,fs, alphabet, EFalse );

        aDes = converter->ConvertFromNativeL(
            aSrc.Right( aSrc.Length() - cutChars ) );
        CleanupStack::PopAndDestroy(); //converter

        if( alphabet == TSmsDataCodingScheme::ESmsAlphabet8Bit )
            {
            // DCS was 8-bit data.
            iDCS2 = KPhCltDcs8Bit;
            }
        else if( alphabet == TSmsDataCodingScheme::ESmsAlphabetUCS2 )
            {
             //DCS was UCS2 data.
            iDCS2 = KPhCltDcsUcs2;
            }
        // If DCS not 8-bit or UCS2, then EPhCltDcsUnknown is returned.
        }
    
    CleanupStack::PopAndDestroy(3); // fs, packer, charConv
    }

// -----------------------------------------------------------------------------
// CPhCltUssdImp::SetDCS()
//
// Set data coding scheme 
// -----------------------------------------------------------------------------
//
void CPhCltUssdImp::SetDCS( TUint8 aDCS )
    {
    iDCS = aDCS;
    }

// -----------------------------------------------------------------------------
// CPhCltUssdImp::FindFirstCarriageReturnL
// -----------------------------------------------------------------------------
//
void CPhCltUssdImp::FindFirstCarriageReturnL( 
    const TDesC& aBuffer ,
    TUint& aSkipChars , 
    TUint& aStartBit )
    {
    if ( aBuffer.Length() < 3 )
        {
        User::Leave( KErrUnderflow );
        }

    aSkipChars = 0;
    aStartBit = 0;

    // Try out two different cases:
    // 1. Find CR from thrid byte
    // 2. Find CR from second byte, starting from bit 6

    // 1.:
    if ( aBuffer[2] == KPhCltUssdCarriageReturn )
        {
        aSkipChars = KPhCltUssdDcsAlphabetDefaultPrecededLanguageSkipChars3;
        aStartBit = 0;
        return;
        }

    // 2.:
    // First put the pieces together and then compare
    // Take last 2 bits from the second byte:
    TUint result1 = aBuffer[1];
    result1 = result1 >> 6;

    // Take first 5 bits from the third byte:
    TUint result2 = aBuffer[2];
    result2 = result2 & 0x1f; // bits 0-4.
    result2 = result2 << 2; // move to bits 2-6.

    TUint result = result1 + result2; // 0000 00xx + 0xxx xx00
    if ( result == KPhCltUssdCarriageReturn )
        {
        aSkipChars = KPhCltUssdDcsAlphabetDefaultPrecededLanguageSkipChars2;
        aStartBit = KPhCltUssdDcsAlphabetDefaultPrecededLanguageStartBit;
        return;
        }

    // Is was not case 1. or 2. so we are not supporting sort of string.
    User::Leave( KErrNotSupported );
    }

// End of File
