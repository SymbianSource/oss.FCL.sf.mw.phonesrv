/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Note master class shows the actual notes.
*
*/


// INCLUDE FILES
#include <psui.rsg> 
#include <aknlists.h>               
#include <psuicontainer.h> 
#include <StringLoader.h> 
#include <exterror.h>               
#include <gsmerror.h>
#include <mmlist.h>
#include <featmgr.h>
#include <NumberGrouping.h> 
#include <cservicerequest.h>
#include <e32property.h>
#include <BTSapDomainPSKeys.h> 

#include "psuiinternalconstants.h" 
#include "psuinotemaster.h" 
#include "psuireqobserver.h" 
#include "psuiquerydialog.h" 
 
#include <centralrepository.h>
#include <NumberGroupingCRKeys.h> 
// MACROS

#ifdef _DEBUG

#define RDEBUG(X)             RDebug::Print(X);
#define RDEBUG_INT(X,Y)       RDebug::Print(X,Y);
#else           
#define RDEBUG(X)
#define RDEBUG_INT(X,Y)
#endif

//  MEMBER FUNCTIONS

// ---------------------------------------------------------------------------
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
CPsuiNoteMaster* CPsuiNoteMaster::NewL()
    {  
    CPsuiNoteMaster* self = new( ELeave ) CPsuiNoteMaster;
    CleanupStack::PushL( self );
    // Sets up TLS, must be done before FeatureManager is used.
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CPsuiNoteMaster::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPsuiNoteMaster::ConstructL()
    {
    iReqObserver = CPsuiReqObserver::NewL( *this );
    FeatureManager::InitializeLibL(); 
    
    iServiceRequest = CServiceRequest::NewL( *iReqObserver );
 
    }


// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
CPsuiNoteMaster::~CPsuiNoteMaster()
    {
    //Remove FeatureManager
    FeatureManager::UnInitializeLib();

    delete iServiceRequest;
    
    delete iReqObserver;
    
    if ( iQuery )
        {
        delete iQuery;
        iQuery = NULL;
        }
    
    if ( iPopupList )
        {
        iPopupList->CancelPopup();
        iPopupList = NULL;
        }
    
    iBarringEngine = NULL;
    iDivertEngine = NULL;
    iWaitingEngine = NULL;
    iCliEngine = NULL;
    
    }

// ---------------------------------------------------------------------------
// CPsuiNoteMaster::SetEngineContacts()
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::SetEngineContacts( MPsetCallWaiting* aWaitingEngine,
                                        MPsetCallBarring* aBarringEngine, 
                                        MPsetCallDiverting* aDivertEngine, 
                                        MPsetCli* aCliEngine )
    {
    iWaitingEngine = aWaitingEngine;
    iBarringEngine = aBarringEngine;
    iDivertEngine = aDivertEngine;
    iCliEngine = aCliEngine;
    }

// ---------------------------------------------------------------------------
// CPsuiNoteMaster::ShowNoteL()
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::ShowNoteL( TPsuiNoteTypes aType, TInt aResourceID )
    {
    if ( aResourceID < 0 )
        {
        if ( aType  == EPsuiWaitingNote )
            {
            iRequesting = ETrue;
            iServiceRequest->StartL( KNullDesC );
            }
        return; 
        }

    HBufC* string = StringLoader::LoadLC( aResourceID );    
    
    switch ( aType )
        {
        case EPsuiConfirmationNote:
            {
            iServiceRequest->ShowCompletionNoteL( 
            ETrue,                                        // aShowNote
            KErrNone,                                     // aError
            CServiceRequest::ESerqGlobalConfirmationNote, // aNoteType 
            string->Des() );                              // aText 
            break;
            }
        case EPsuiErrorNote:
            {
            CServiceRequest::TSerqNoteType errorType = CServiceRequest::ESerqGlobalErrorNote;
            if ( aResourceID == R_NO_SERVICE ||
                aResourceID == R_NOT_ALLOWED )
                {
                errorType = CServiceRequest::ESerqGlobalInformationNote;
                }
            iServiceRequest->ShowCompletionNoteL( 
            ETrue, 
            KErrNone, 
            errorType, 
            string->Des() );
            break;
            }
        case EPsuiInformationNote:
            {
            iServiceRequest->ShowCompletionNoteL( 
            ETrue, 
            KErrNone, 
            CServiceRequest::ESerqGlobalInformationNote, 
            string->Des() );
            }
        default:
            break;
        }
    CleanupStack::PopAndDestroy( string );
    string = NULL;
    }

// ---------------------------------------------------------------------------
// CPsuiNoteMaster::KillStaticNoteL()
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::KillStaticNoteL()
    {
    if ( iRequesting )
        {
        delete iServiceRequest;
        iServiceRequest = NULL;
        iServiceRequest = CServiceRequest::NewL( *iReqObserver );
        iRequesting =EFalse;
        }
     
    }

// ---------------------------------------------------------------------------
// CPsuiNoteMaster::KillRequestL()
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::KillRequestL()
    {
    if ( iWaitingEngine )
        {
        iWaitingEngine->CancelProcess();    
        }
    else if ( iBarringEngine )
        {
        iBarringEngine->CancelCurrentRequest();
        }
    else if ( iDivertEngine )
        {
        iDivertEngine->CancelCurrentRequest();
        }
    else if ( iCliEngine )
        {
        ShowCancelRequestNoteL();
        iCliEngine->CancelAll();
        }
    }
// ---------------------------------------------------------------------------
// CPsuiNoteMaster::ShowBscListL()
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::ShowBscListL( TInt aResourceHeading, 
                                   TUint8 aBsc[KPSUIMaxBscNumber] )
    {
    if ( !iWaitingEngine && 
         !iBarringEngine && 
         !iDivertEngine  &&
         !iCliEngine ) 
        {
        return;
        }

    if ( aResourceHeading < 0 )
        {
        User::Leave( KErrArgument );
        }
    
    CAknSinglePopupMenuStyleListBox* list = 
        new ( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( list );
    
    CreateListBoxL( EPsuiNoCF, list );
    
    CDesCArray* items = new ( ELeave ) CDesCArrayFlat( KPSUIMaxBscNumber );
    CleanupStack::PushL( items );
    
    CTextListBoxModel* model = list->Model();
    if ( !model )
        {
        User::Leave( KErrGeneral );
        }
    model->SetItemTextArray( items );
    model->SetOwnershipType( ELbmOwnsItemArray );
    CleanupStack::Pop( items );   // listbox model now owns this
    
    SetTitleL( aResourceHeading );
    
    TUint8 numberOfBsc = 0;
    HBufC* string = NULL;
    while ( ( aBsc[numberOfBsc] != KPSUINoBscGroup ) &&
            ( numberOfBsc < KPSUIMaxBscNumber ) )
        {        
        TUint8 bscCode = aBsc[numberOfBsc];        
        string = SetBsGroupLC( bscCode );
        
        if ( string )
            {
            items->AppendL( *string );
            CleanupStack::PopAndDestroy( string );
            }
        numberOfBsc++;
        }        
    
    iPopupList->ExecuteLD();
    iPopupList = NULL;
    CleanupStack::PopAndDestroy( list );
    }    

// ---------------------------------------------------------------------------
// Shows additional divert information.
// ---------------------------------------------------------------------------
/*****************************************************
*    Series 60 Customer / ETel
*    Series 60  ETel API
*****************************************************/ 
void CPsuiNoteMaster::ShowCfInfoL( TCFType& aCFType,
                                  const RMobilePhone::TMobilePhoneCFInfoEntryV1& aSetting )
    {
    if ( aCFType == EPsuiNoCF )
        {
        User::Leave( KErrArgument );
        }
    
    // Get base text and set phone number into it.
    // Note that two characters are reserved for LRM markers.
    TBuf<KPSUIDetailsText+2> string;    
    StringLoader::Load( string, R_TO_NUMBER );
    string.Append( KPSUIListDivider ); 

    TInt pngErr( KErrNone );
    TInt pngSupported( 0 );
    CRepository* repository = CRepository::NewL( KCRUidNumberGrouping );
    pngErr = repository->Get( KNumberGrouping, pngSupported );
    delete repository;
        
    if ( KErrNone == pngErr && pngSupported != 0 )
        {
        CPNGNumberGrouping* numberGrouping = 
            CPNGNumberGrouping::NewL( KPSUIMaxPSUIItemLength );
        CleanupStack::PushL( numberGrouping );

        //Check that number fits (with or w/o number formatting).
        if ( (aSetting.iNumber.iTelNumber.Length() + 2) <= numberGrouping->MaxDisplayLength() )
            {
            numberGrouping->Set( aSetting.iNumber.iTelNumber );
            AppendCFNumber( string, numberGrouping->FormattedNumber() );
            }
        else
            {
            AppendCFNumber( string, aSetting.iNumber.iTelNumber );
            }
        CleanupStack::PopAndDestroy( numberGrouping );
        }
    else
        {
        AppendCFNumber( string, aSetting.iNumber.iTelNumber );
        }

    //AVKON utils check whether or not conversion is needed
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( string );    
    string.Append( KPSUIListDivider );
    
    // Add delay time.
    if ( aCFType == EPsuiCFNry )  
        {
        //Header + divider.
        HBufC* delayTime = StringLoader::LoadLC( R_DELAY_TIME );
        string.Append( *delayTime );
        CleanupStack::PopAndDestroy( delayTime );
        string.Append( KPSUIListDivider );
        
        TBuf<KPSUIMaxPSUIItemLength> writeBuffer;
        HBufC* readBuffer = StringLoader::LoadLC( R_TEXT_SS_DETAIL_DELAY_SEC );
        // format time, -1 means that there is no index in the key string.
        StringLoader::Format( writeBuffer, *readBuffer, -1, aSetting.iTimeout );
        CleanupStack::PopAndDestroy( readBuffer );
        
        string.Append( writeBuffer );
        }
    
    iQuery = CPsuiQueryDialog::NewL( CAknQueryDialog::ENoTone );
    TInt command( iQuery->ExecuteLD( R_CONFIRMATION_QUERY, string ) );
    if( command == EKeyNo )
        {
        // End -key was pressed so don't relaunch previous query
        aCFType = EPsuiNoCF;            
        }
    iQuery = NULL;
    }

// ---------------------------------------------------------------------------
// Shows request cancelled note.
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::ShowCancelRequestNoteL()
    {
    if ( iRequesting )
        {
        ShowNoteL( EPsuiInformationNote, R_REQUEST_CANCELLED_NOTE );    
        }
    }

// ---------------------------------------------------------------------------
// Returns resource ID for the most common error codes.
// ---------------------------------------------------------------------------
//
TInt CPsuiNoteMaster::SetErrorNoteResourceID ( const TInt aErrorCode )
    {
    RDEBUG_INT( _L("PSUI.SetErrorNoteResourceID - code %d: "), aErrorCode )
    TInt resourceID = KErrNone;    
    switch ( aErrorCode  )
        {
        case KErrGsmSSSubscriptionViolation:
            resourceID = R_CONTACT_SERVICE_NOTE;
            break;            
        case KErrGsmSSUnknownSubscriber:
        case KErrGsmSSAbsentSubscriber:        
        case KErrGsmSSIllegalOperation:
        case KErrGsmSSIllegalSubscriber:
        case KErrGsmSSIllegalEquipment:
        case KErrGsmSSCallBarred:
        case KErrGsmSSDataMissing:
            resourceID = R_NOT_ALLOWED;
            break;
        case KErrGsmSSIncompatibility:
            resourceID = R_SERVICES_IN_CONFLICT;
            break;
        case KErrGsmSSSystemFailure:
            resourceID = R_RESULT_UNKNOWN;
            break;        
        case KErrGsmSSUnexpectedDataValue:
        case KErrGsmSSResourcesUnavailable:
            resourceID = R_REQUEST_REJECTED;
            break;
        case KErrGsmSSNegativePasswordCheck:
        case KErrGsmSSPasswordRegistrationFailure:
            resourceID = R_PASSWORD_ERROR_NOTE;
            break;
        case KErrGsmSSPasswordAttemptsViolation:
            resourceID = R_PASSWORD_BLOCKED_NOTE;
            break;
        case KErrGsmSMSNoNetworkService:
        case KErrGsmNoService:
            resourceID = R_NO_SERVICE;
            break;
        case KErrSsActivationDataLost: 
            resourceID = R_NET_INFO_LOST;
            break;
        case KErrGsmOfflineOpNotAllowed:
            if ( FeatureManager::FeatureSupported( KFeatureIdOfflineMode ) )
                {
                TInt value = EBTSapNotConnected;
                RProperty::Get(
                    KPSUidBluetoothSapConnectionState,
                    KBTSapConnectionState,
                    value );
                if ( value == EBTSapConnected )
                    {
                    resourceID = R_OFFLINE_MODE_SAP;    
                    }
                else
                    {
                    resourceID = R_OFFLINE_MODE;    
                    }     
                 }
            break;            
        default:
            resourceID = R_NOT_DONE_NOTE;
            break; 
        }
    return resourceID;
    }

// ---------------------------------------------------------------------------
// CPsuiNoteMaster::ShowBscCFListL()
// ---------------------------------------------------------------------------
/*****************************************************
*    Series 60 Customer / ETel
*    Series 60  ETel API
*****************************************************/ 
TCFType CPsuiNoteMaster::ShowBscCFListL( TInt aResourceHeading, 
                                        CMobilePhoneCFList& aList, 
                                        TCFType aCFType,
                                        TInt& aIndex )
    {
    //note master was deleted - do not create another bsc-list
    if ( !iWaitingEngine && 
         !iBarringEngine && 
         !iDivertEngine  &&
         !iCliEngine ) 
        {
        return EPsuiNoCF;
        }

    if ( aResourceHeading < 0 )
        {
        User::Leave( KErrArgument );
        }
    
    CAknSinglePopupMenuStyleListBox* list = 
        new ( ELeave ) CAknSinglePopupMenuStyleListBox;
    CleanupStack::PushL( list );
    
    CreateListBoxL( aCFType, list );
    
    CDesCArray* items = new ( ELeave ) CDesCArrayFlat( KPSUIMaxBscNumber );
    CleanupStack::PushL( items );
    
    CTextListBoxModel* model = list->Model();
    if ( !model )
        {
        User::Leave( KErrGeneral );
        }
    model->SetItemTextArray( items );
    model->SetOwnershipType( ELbmOwnsItemArray );
    CleanupStack::Pop( items );
    
    SetTitleL( aResourceHeading );
    
    TInt entries = aList.Enumerate();
    TInt numberOfBscs = 0;
    RMobilePhone::TMobilePhoneCFInfoEntryV1 cfInfo;
    HBufC* string = NULL;
    while ( entries > numberOfBscs )
        {
        cfInfo = aList.GetEntryL( numberOfBscs );
        string = SetBsGroupLC( 
            static_cast <TUint8> ( ChangeToGSM( cfInfo.iServiceGroup ) ) );
        // if no string was retrieved, ignore it.
        if ( string )
            {
            items->AppendL( *string );
            CleanupStack::PopAndDestroy( string );            
            }
        numberOfBscs++;
        }
    list->SetCurrentItemIndex( aIndex );
    
    TInt popupRet = iPopupList->ExecuteLD();
    aIndex = list->CurrentItemIndex();
    iPopupList = NULL;
    CleanupStack::PopAndDestroy( list );
    
    if ( popupRet ) // if number or details softkey pressed
        {
        return aCFType;
        }    
    return EPsuiNoCF;
    }    

// ---------------------------------------------------------------------------
// CPsuiNoteMaster::CreateListBoxL
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::CreateListBoxL( TCFType aCFType, 
                                     CAknSinglePopupMenuStyleListBox* aList )
    {
    TInt resourceId = 0;
    if ( aCFType == EPsuiNoCF )
        {
        resourceId = R_AVKON_SOFTKEYS_BACK;
        }
    else if ( aCFType == EPsuiCFOther )
        {    
        resourceId = R_PSUI_SOFTKEYS_NUMBER_BACK;
        }
    else
        {
        resourceId = R_PSUI_SOFTKEYS_DETAILS_BACK;
        }
    
    // Create popup list
    iPopupList = CAknPopupList::NewL( aList, resourceId );
    aList->ConstructL( iPopupList, CEikListBox::ELeftDownInViewRect );
    aList->CreateScrollBarFrameL( ETrue );
    aList->ScrollBarFrame()->SetScrollBarVisibilityL( 
        CEikScrollBarFrame::EOff, 
        CEikScrollBarFrame::EAuto );
    }

// ---------------------------------------------------------------------------
// CPsuiNoteMaster::SetTitleL
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::SetTitleL( const TInt& aResourceHeading )
    {
    TRAPD ( ignore, 
        HBufC* string = StringLoader::LoadLC( aResourceHeading );
        iPopupList->SetTitleL( *string );
        CleanupStack::PopAndDestroy( string );
        );
    if ( ignore )
        {
        HBufC16* emptyHeader = NULL;
        iPopupList->SetTitleL( emptyHeader->Des() );
        delete emptyHeader;
        }    
    }
// ---------------------------------------------------------------------------
// CPsuiNoteMaster::AppendCFNumber
// ---------------------------------------------------------------------------
void CPsuiNoteMaster::AppendCFNumber( TDes& aString, const TDesC& aNumber )
    {
        aString.Append( KLRMarker );
        aString.Append( aNumber );
        aString.Append( KLRMarker );
    }
// ---------------------------------------------------------------------------
// CPsuiNoteMaster::SetBsGroupLC
// ---------------------------------------------------------------------------
HBufC* CPsuiNoteMaster::SetBsGroupLC( TUint8 aBscCode )
    {
    HBufC* string = NULL;
    switch( aBscCode )
        {
        case 0:
            string = StringLoader::LoadLC( R_BS_ALPHA_0 );
            break;
        case 10:
            string = StringLoader::LoadLC( R_BS_ALPHA_10 );
            break;
        case 11:
            string = StringLoader::LoadLC( R_BS_ALPHA_11 );
            break;
        case 12:
            string = StringLoader::LoadLC( R_BS_ALPHA_12 );
            break;  
        case 13:
            string = StringLoader::LoadLC( R_BS_ALPHA_13 );
            break;
        case 16:
            string = StringLoader::LoadLC( R_BS_ALPHA_16 );
            break;
        case 19:
            string = StringLoader::LoadLC( R_BS_ALPHA_19 );
            break;
        case 20:
            if ( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
                {
                string = StringLoader::LoadLC( R_BS_ALPHA_20_VT );
                }
            else
                {
                string = StringLoader::LoadLC( R_BS_ALPHA_20 );
                }
            break;
        case 21:
            string = StringLoader::LoadLC( R_BS_ALPHA_21 );
            break;
        case 22:
            string = StringLoader::LoadLC( R_BS_ALPHA_22 );
            break;
        case 24:
            string = StringLoader::LoadLC( R_BS_ALPHA_24 );
            break;
        case 25:
            string = StringLoader::LoadLC( R_BS_ALPHA_25 );
            break;
        case 26:
            string = StringLoader::LoadLC( R_BS_ALPHA_26 );
            break;
        case 27:
            string = StringLoader::LoadLC( R_BS_ALPHA_27 );
            break;
        case 30:
            string = StringLoader::LoadLC( R_BS_ALPHA_30 );
            break;
        case 89:
            string = StringLoader::LoadLC( R_BS_ALPHA_89 );
            break;
        default:
            // all the services that are not displayed to ui
            break;
        }
    return string;
    }

// ---------------------------------------------------------
// CPsuiNoteMaster::ChangeToGSM
//
// ---------------------------------------------------------
TBasicServiceGroups CPsuiNoteMaster::ChangeToGSM( const TInt& aBsc )
    {
    switch ( aBsc )
        {               
        case RMobilePhone::EAllServices:
            return EAllTeleAndBearer;
        case RMobilePhone::EAllTele:
            return EAllTele;
        case RMobilePhone::EVoiceService:
        case RMobilePhone::ETelephony:
            return ETelephony;
        case RMobilePhone::EAllDataTele:
            return EAllDataTele;
        case RMobilePhone::EFaxService:
            return EFax;
        case RMobilePhone::EShortMessageService:
            return ESms;
        case RMobilePhone::EAllDataExSms:
            return EAllDataExSms;
        case RMobilePhone::EAllTeleExcSms:
            return EAllTeleExcSms;
        case RMobilePhone::EAllPlmnTele:
            return EAllPlmnTele;
        case RMobilePhone::EPlmnTele1:
            return EPlmnTele1;
        case RMobilePhone::EPlmnTele2:
            return EPlmnTele2;
        case RMobilePhone::EPlmnTele3:
            return EPlmnTele3;
        case RMobilePhone::EPlmnTele4:
            return EPlmnTele4;
        case RMobilePhone::EPlmnTele5:
            return EPlmnTele5;
        case RMobilePhone::EPlmnTele6:
            return EPlmnTele6;
        case RMobilePhone::EPlmnTele7:
            return EPlmnTele7;
        case RMobilePhone::EPlmnTele8:
            return EPlmnTele8;
        case RMobilePhone::EPlmnTele9:
            return EPlmnTele9;
        case RMobilePhone::EPlmnTeleA:
            return EPlmnTeleA;
        case RMobilePhone::EPlmnTeleB:
            return EPlmnTeleB;
        case RMobilePhone::EPlmnTeleC:
            return EPlmnTeleC;
        case RMobilePhone::EPlmnTeleD:
            return EPlmnTeleD;
        case RMobilePhone::EPlmnTeleE:
            return EPlmnTeleE;
        case RMobilePhone::EPlmnTeleF:
            return EPlmnTeleF;
        case RMobilePhone::EAllBearer:
            return EAllBearer;
        case RMobilePhone::EAllAsync:
            return EAllAsync;
        case RMobilePhone::EAllSync:
            return EAllSync;
        case RMobilePhone::ECircuitDataService:
        case RMobilePhone::ESyncData:
            return ESyncData;
        case RMobilePhone::EPacketDataService:
        case RMobilePhone::EAsyncData:
            return EAsyncData;
        case RMobilePhone::EPacketData:
            return EPacketData;
        case RMobilePhone::EPadAccess:
            return EPadAccess;
        case RMobilePhone::EAllPlmnBearer:
            return EAllPlmnBearer;
        case RMobilePhone::EPlmnBearerServ1:
            return EPlmnBearerServ1;
        case RMobilePhone::EPlmnBearerServ2:
            return EPlmnBearerServ2;
        case RMobilePhone::EPlmnBearerServ3:
            return EPlmnBearerServ3;
        case RMobilePhone::EPlmnBearerServ4:
            return EPlmnBearerServ4;
        case RMobilePhone::EPlmnBearerServ5:
            return EPlmnBearerServ5;
        case RMobilePhone::EPlmnBearerServ6:
            return EPlmnBearerServ6;
        case RMobilePhone::EPlmnBearerServ7:
            return EPlmnBearerServ7;
        case RMobilePhone::EPlmnBearerServ8:
            return EPlmnBearerServ8;
        case RMobilePhone::EPlmnBearerServ9:
            return EPlmnBearerServ9;
        case RMobilePhone::EPlmnBearerServA:
            return EPlmnBearerServA;
        case RMobilePhone::EPlmnBearerServB:
            return EPlmnBearerServB;
        case RMobilePhone::EPlmnBearerServC:
            return EPlmnBearerServC;
        case RMobilePhone::EPlmnBearerServD:
            return EPlmnBearerServD;
        case RMobilePhone::EPlmnBearerServE:
            return EPlmnBearerServE;
        case RMobilePhone::EPlmnBearerServF:
            return EPlmnBearerServF;
        case RMobilePhone::EAuxVoiceService:
        case RMobilePhone::EAltTele:
            return EAltTele;
        case RMobilePhone::EServiceUnspecified:
        default:
            return EUnknown;
        }
    }
    
// -----------------------------------------------------------------------------
// CPsuiNoteMaster::RequestNoteCancelledL
// Called when the Requesting note is cancelled by the user.
// -----------------------------------------------------------------------------
//
void CPsuiNoteMaster::RequestNoteCancelledL()
    {
    KillRequestL();
    iRequesting = EFalse;        
    }    
// End of file
