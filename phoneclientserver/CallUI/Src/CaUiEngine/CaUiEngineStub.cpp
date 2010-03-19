/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CallUIEngine, stub version.
*
*/



// INCLUDE FILES
#include    "cauiengine.h" 
#include    "cauidialogs.h" 

#include    <callui.rsg> // resources. 
#include    <cphcltdialer.h> // cphcltdialer. 
#include    <cphonecntfactory.h> 
#include    <aiwinternaldialdata.h> 
#include    <aiwservicehandler.h> 
#include    <cphcltextphonedialdata.h> // cphcltextphonedialdata 

#include    "callui.loc" // localized strings. 

#include    <stringloader.h> // string loader. 
#include    <featmgr.h> // featuremanager. 

#include    <phcltutils.h> // character removal. 
#include    <aiwcommon.h> // aiw dialdata. 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaUiEngine::CCaUiEngine
// 
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCaUiEngine::CCaUiEngine()
:   iResourceLoader( *CCoeEnv::Static() )
#ifndef RD_PHONE_NG
    ,iResourceLoaderPhoneApE( *CCoeEnv::Static() )
#endif    
    {
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::ConstructL
// 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaUiEngine::ConstructL()
    {
    // Sets up TLS, must be done before FeatureManager is used.
    FeatureManager::InitializeLibL();

    if( CCoeEnv::Static() )
        {
        // Open CallUI resource file.
        TFileName* name = new ( ELeave ) TFileName( KCallUIResFile );
        CleanupStack::PushL( name );
        User::LeaveIfError( iResourceLoader.Open( *name ) );
        CleanupStack::PopAndDestroy( name );
        }
    }

// -----------------------------------------------------------------------------
// CCaUiEngine::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaUiEngine* CCaUiEngine::NewL()
    {
    CCaUiEngine* self = new( ELeave ) CCaUiEngine;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::~CCaUEngine
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CCaUiEngine::~CCaUiEngine()
    {
    delete iContactSelector;
    delete iFactory;

    // Clear dial components.
    ClearDialContents();

    if( CCoeEnv::Static() )
        {
        // Close resource loader.
        iResourceLoader.Close();
        
#ifndef RD_PHONE_NG
        // Release PhoneAppEngine resources.
        iResourceLoaderPhoneApE.Close();
#endif
        }
    
    // Delete the menubuffer.
    delete iBuf;
    iBuf = NULL;

    // Frees the TLS! Must be done after FeatureManager is used.
    FeatureManager::UnInitializeLib();
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::GetResources
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::GetResources(
    const TCaUiEngResource aResource,
    TFileName& aResFile,
    TInt& aResId )
    {
    aResFile = KCallUIResFile;
    if ( aResource == ECaUiResOnlyInternet )
        {
        aResId = 0;
        }
    else
        {
        // Return pure voice menu item.
        aResId = R_CALLUI_CALL_MENU;
        }
    }

// -----------------------------------------------------------------------------
// CCaUiEngine::DialL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::DialL(
    MCaUiDialResultObserver& aObserver,
    const TDesC8& aDialData,
    const TCaUiCallType aCallType )
    {
    TInt length = aDialData.Length();
   
    // If iPhCltDialer and iDialDataContainer exist, 
    // destroy them and create new ones. This clears the dial data.
    ClearDialContents();
    iPhCltDialer = CPhCltExtPhoneBase::NewL( this );
    iObserver = &aObserver;
    
    iDialData = CPhCltExtPhoneDialData::NewL();
    CAiwInternalDialData* aiwDialData = CAiwInternalDialData::NewLC( aDialData );

    // Convert AIW dial data to PhoneClient format.
   
    ConvertDialDataL( *aiwDialData );

    iDialData->SetCallType( EPhCltVoice );
    
    //a call type is still unknown if the dialing is made via phonebook
    //send key (doesn't use HandleMenuCmdL, check the call type in 
    //aiwDialData to make sure if unknown is really unknown
    //only do if call type unknown
    TCaUiCallType callType = aCallType;
    if ( callType == ECaUiCallTypeUnknown )
        {
        CAiwDialData::TCallType aiwCalltype = aiwDialData->CallType();
        
        switch ( aiwCalltype )
           {
           case CAiwDialData::EAIWVoice:
                callType = ECaUiCallTypeVoice;
                break;
            case CAiwDialData::EAIWVideo:
            case CAiwDialData::EAIWForcedVideo:
                callType = ECaUiCallTypeVideo;
                break;
            case CAiwDialData::EAIWVoiP:
                callType = ECaUiCallTypeInternet;
                break;
            default:
                //already unknown
                break;
            }
        }
        
    //aiwDialData not needed anymore, remove from stack
    CleanupStack::PopAndDestroy( aiwDialData );

    // If Contact link is provided fetch field link from Phone Book.
    if ( iDialData->ContactLink() != KNullDesC8() )
        {
        // Launch address select.
        // aCallType describes which selector is used. ( VoIP/PhoneNumber )
        LaunchAddressSelectL( 
                        iDialData->ContactLink(), 
                        callType );
        }
    

    else 
        {
        iPhCltDialer->DialL( *iDialData );
        }
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::LaunchNoNetSupNoteL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::LaunchNoNetSupNoteL()
    {
    User::Leave( KErrNotSupported );
    }



// -----------------------------------------------------------------------------
// CCaUiEngine::LaunchReconConfQueryL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::LaunchReconConfQueryL( 
    MCaUiReconnectQueryObserver& /*aObserver*/,
    CPhCntMatcher* /*aPhCntMatcher*/,
    const TPtrC /*aPhoneNumber*/,
    const TBool /*aIncludeVideoCallOption*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::CancelReconConfQuery
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::CancelReconConfQuery()
    {
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::LaunchNoteAndReconConfQueryL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::LaunchNoteAndReconConfQueryL( 
    MCaUiReconnectQueryObserver& /*aObserver*/,
    CPhCntMatcher* /*aPhCntMatcher*/,
    const TPtrC /*aPhoneNumber*/,
    const TBool /*aIncludeVideoCallOption*/,
    const TPtrC /*aNoteText*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::CancelNoteAndReconConfQuery
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::CancelNoteAndReconConfQuery()
    {
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::LaunchAddressSelectL
// 
// 
// -----------------------------------------------------------------------------
//
TBool CCaUiEngine::LaunchAddressSelectL( 
    TDes& aTelNum, 
    const TInt aContactId,
    const TBool aVoIPAddressSelect )
    {
    if ( aVoIPAddressSelect )
        {
        User::Leave( KErrNotSupported );
        }

    TBool addressSelected( EFalse );
    TCaUiCallType callType( ECaUiCallTypeVoice );
    
    addressSelected = LaunchAddressSelectL( aTelNum, aContactId, callType );

    return addressSelected;
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::CancelAddressSelect
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::CancelAddressSelect()
    {
    }

// -----------------------------------------------------------------------------
// From MPhCntStoreLoaderObserver.
// CCaUiEngine::ContactStoreLoadingCompleted
// -----------------------------------------------------------------------------
//
void CCaUiEngine::ContactStoreLoadingCompleted( 
        MVPbkContactStore* /*aStore*/, TInt /*aErrorCode*/ )
    {
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::HandleDialL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::HandleDialL( const TInt aStatus )
    {
    if ( iObserver )
        {
        iObserver->HandleDialResultL( aStatus );
        }
    
    ClearDialContents();
    }
    
// -----------------------------------------------------------------------------
// From base class MCaUiReconnectQueryObserver
// -----------------------------------------------------------------------------
//   
void CCaUiEngine::OptionSelected( 
    MCaUiReconnectQueryObserver::TCaUiReconType /*aReconType*/ )
    {
    }

// -----------------------------------------------------------------------------
// CCaUiEngine::ConvertDialData
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::ConvertDialDataL(
    const CAiwInternalDialData& aAiwDialData )
    {
    // Telephony number.
    iDialData->SetTelephoneNumber( aAiwDialData.PhoneNumber().Left( 
        iDialData->TelephoneNumber().MaxLength() ) );

    // Call type.
    CAiwDialData::TCallType aiwCallType = aAiwDialData.CallType();
    TPhCltCallType phCltCallType = EPhCltVoice;

    // After this the call type should be correct one.
    if ( aiwCallType == CAiwDialData::EAIWVideo )
        {
        phCltCallType = EPhCltVideo;
        iDialData->SetCallType( phCltCallType );
        }
    else if ( aiwCallType == CAiwDialData::EAIWForcedVideo )
        {
        phCltCallType = EPhCltForcedVideo;
        iDialData->SetCallType( phCltCallType );
        }
    else if ( aiwCallType == CAiwDialData::EAIWVoiP )
        {
        phCltCallType = EPhCltCallVoIP;
        iDialData->SetCallType( phCltCallType );
        }

     // Name.
    iDialData->SetNameL( aAiwDialData.Name().Left( 
        iDialData->Name().MaxLength() ) );

    // Contact link.
    iDialData->SetContactLinkL( aAiwDialData.ContactLink() );

    // Window group.
    iDialData->SetWindowGroup( aAiwDialData.WindowGroup() );

    // Redial.
    iDialData->SetRedial( aAiwDialData.Redial() );
    
    // Redial maximum duration.
    iDialData->SetRedialMaximumDuration( aAiwDialData.RedialMaximumDuration() );

    // Show number.
    iDialData->SetShowNumber( aAiwDialData.ShowNumber() );

    // Match.
    iDialData->SetAllowMatch( aAiwDialData.AllowMatch() );

    // End other calls.
    iDialData->SetEndOtherCalls( aAiwDialData.EndOtherCalls() );

    // Subaddress.
    iDialData->SetSubAddressL( aAiwDialData.SubAddress().Left( 
        iDialData->SubAddress().MaxLength() ) );

    // SAT call.
    iDialData->SetSATCall( aAiwDialData.SATCall() );

    // Bearer.
    iDialData->SetBearerL( aAiwDialData.Bearer().Left( 
        iDialData->Bearer().MaxLength() ) );

    TPhCltTelephoneNumber telnum;
    telnum.Zero();
    telnum = iDialData->TelephoneNumber();
    PhCltUtils::RemoveInvalidChars( telnum );
    iDialData->SetTelephoneNumber( telnum );

    }

// -----------------------------------------------------------------------------
// CCaUiEngine::ClearDialContents
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::ClearDialContents()
    {
    if ( iPhCltDialer )
        {
        delete iPhCltDialer;
        iPhCltDialer = NULL;
        }

    if ( iDialData )
        {
        delete iDialData;
        iDialData = NULL;
        }

    // Not owned, so just set to NULL.
    iObserver = NULL;
    }

// -----------------------------------------------------------------------------
// CCaUiEngine::LaunchAddressSelectL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::LaunchAddressSelectL( 
    const TDesC8& aContactLink,
    const TCaUiCallType aCallType )
    {
    if ( aCallType != ECaUiCallTypeVoice )
        {
        HandleDialL( KErrNotSupported );
        }
    
    else
        {
        if ( !iFactory )
            {
            iFactory = CreateCntFactoryL();
            }
        if ( !iContactSelector ) 
            {
            iContactSelector = iFactory->CreateContactDataSelectionL();
            }
            
        CPhCntContactDataSelection::TCallType callType;
        
        if ( aCallType == ECaUiCallTypeVoice )
            {
            callType = CPhCntContactDataSelection::ECallPhoneNumber;
            }
        else if ( aCallType == ECaUiCallTypeVideo )
            {
            callType = CPhCntContactDataSelection::ECallVideoNumber;
            }
        else if ( aCallType == ECaUiCallTypeInternet )
            {
            callType = CPhCntContactDataSelection::ECallVoip;
            }
        else
            {
            callType = CPhCntContactDataSelection::ECallPhoneNumber;
            }
            
        iContactSelector->Cancel();
        iContactSelector->SelectPhoneNumberForCallL( aContactLink, callType, *this );
 
        }
     
    }

// ---------------------------------------------------------
//  CCaUiEngine::HandleEmergencyDialL()
// ---------------------------------------------------------
//    
void CCaUiEngine::HandleEmergencyDialL( const TInt /*aStatus*/ )
    {
    // Do nothing
    }

// ---------------------------------------------------------
//  CCaUiEngine::SelectionDone()
// ---------------------------------------------------------
//     
void CCaUiEngine::SelectionDone( CPhCntSelectedData* aContactData, 
                                  TInt aErrorCode )
    {
    if ( aErrorCode == KErrNone )
        {
        TPhCltTelephoneNumber phoneNumber;
        HBufC8* fieldLink = NULL;
        
        if ( iDialData->TelephoneNumber() == KNullDesC )
            {
            phoneNumber = aContactData->Data();
            PhCltUtils::RemoveInvalidChars( phoneNumber );
            iDialData->SetTelephoneNumber( phoneNumber );
            }
        if ( aContactData->FieldLink().Length() > 0 )
            {
            fieldLink = aContactData->FieldLink().Alloc();
            }
        
        TRAP( aErrorCode, PhoneClientDialL( fieldLink ) );
      
        delete fieldLink;
        fieldLink = NULL;
        }
        
    if ( aErrorCode != KErrNone )
        {
        TRAP_IGNORE( HandleDialL( aErrorCode ) );
        }            
    }
    
// ---------------------------------------------------------
//  CCaUiEngine::PhoneClientDialL()
// ---------------------------------------------------------
// 
void CCaUiEngine::PhoneClientDialL( const TDesC8* aFieldLink )
    {
    
    TBool createCall( ETrue );
    TInt error( KErrNone );
    
    if ( aFieldLink )
        {
        iDialData->SetContactLinkL( *aFieldLink );
        }
    
    // Check if number is emergency number and if it is 
    // then call emeregency call
    if ( !iEmergencyCall )
        {
        iEmergencyCall = CPhCltEmergencyCall::NewL( this );
        }
    if ( IsEmergencyNumber() )
        {
        createCall = EFalse;
        if ( aFieldLink )
            {
            error = KErrNone;
            iEmergencyCall->DialEmergencyCallL( iDialData->TelephoneNumber() );
            }
        else
            {
            error = KErrNotSupported;
            }
        }
    
    // Here note launch + query if this is video call and we
    // are not in 3G network.
   
    if ( createCall )
        {
        if ( ( iDialData->CallType() == EPhCltVideo ) ||
             ( iDialData->CallType() == EPhCltForcedVideo ) ||
             ( iDialData->CallType() == EPhCltCallVoIP ) )
            {
                createCall = EFalse;
                error = KErrNotSupported;
            }
            
        }
    if ( createCall )
        {
        iPhCltDialer->DialL( *iDialData );
        }
    else 
        {        
        //User cancelled. Need to inform client.
        if ( error != KErrNone ) // Emergency call does not need to inform client. 
            {
            HandleDialL( error );
            }
         }
    }

// ---------------------------------------------------------
//  CCaUiEngine::IsEmergencyNumber()
// ---------------------------------------------------------
//
TBool CCaUiEngine::IsEmergencyNumber()
    {
    TBool isEmergencyNumber( EFalse );
    
    // get phonenumber from iDialData and heck number using phoneclient
    // emergencycall api
    TInt result = iEmergencyCall->IsEmergencyPhoneNumber(
        iDialData->TelephoneNumber(), 
        isEmergencyNumber );
   
    return isEmergencyNumber;
    }

// -----------------------------------------------------------------------------
// CCaUiEngine::IsVoIPProfiles()
// 
// 
// -----------------------------------------------------------------------------
//
TBool CCaUiEngine::IsVoIPProfiles()
    {  
    return EFalse; 
    }

// End of file
