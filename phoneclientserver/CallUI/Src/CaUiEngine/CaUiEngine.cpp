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
* Description:  Implementation of CallUIEngine.
*
*/


// INCLUDE FILES
#include    "cauiengine.h" 
#include    "cauiquery.h" // CCaUiQuery. 
#include    <mcauireconnectqueryobserver.h>  // MCaUiReconnectQueryObserver.
#include    <callui.rsg> // Resources. 
#include    <cphcltdialer.h> // CPhcltDialer. 
#include    <cphonecntfactory.h>
#include    <cphcntsingleitemfetch.h>
#include    <aiwinternaldialdata.h>
#include    <aiwservicehandler.h> 
#include    <cphcltextphonedialdata.h> // CPhCltExtPhoneDialData 

#include    "callui.loc"                // Localized strings.

#include    <stringloader.h> // String Loader. 
#include    <aknnotewrappers.h>         // CAknInformationNote.
#include    <akngloballistquery.h> // CAknGloballistQuery. 
#include    <featmgr.h>                 // FeatureManager.

#include    <aknglobalconfirmationquery.h> // Confirmation query.

#include    <phcltutils.h> // Character removal. 
#include    <aiwcommon.h> // AIW dialdata. 

#include    <networkhandlingdomainpskeys.h> 
#include    <e32property.h>             // Pub&Sub functionality.

#include    "cauivoipextension.h" // VoIP/SCCP profile store 

#include    <centralrepository.h>
#include    <settingsinternalcrkeys.h> 
#include     "cauilogger.h" // Call Ui Logger 
#include    <aknmessagequerydialog.h> 
#include    <aknglobalnote.h>
#include    <mphcntstoreloader.h>
#include    <mvpbkcontactstore.h> 

#include "cauidialogs.h" 

// CONSTANTS

// GSM Network.
const TInt KCaUiEngGsmNetwork = ENWNetworkModeGsm;
// WCDMA Network.
const TInt KCaUiEngWcdmaNetwork = ENWNetworkModeWcdma;


_LIT( KCallTypeUriParam, "call-type" );
_LIT( KCallTypeVideo, "video" );
_LIT( KSemiColon, ";" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaUiEngine::CCaUiEngine
// 
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCaUiEngine::CCaUiEngine()
:   iResourceLoader( *CCoeEnv::Static() )
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
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::ConstructL() Begin");
    // Sets up TLS, must be done before FeatureManager is used.
    FeatureManager::InitializeLibL();

    if ( CCoeEnv::Static() )
        {
        // Open CallUI resource file.
        TFileName* name = new ( ELeave ) TFileName( KCallUIResFile );
        CleanupStack::PushL( name );
        User::LeaveIfError( iResourceLoader.Open( *name ) );
        CleanupStack::PopAndDestroy( name );
        }

    // Get info is VoIP supported
    iVoIPProfileHandler = CCaUiVoIPExtension::NewL();
    iLongPressSet = ECaUiLongPressKeyNotSet;

    iServiceSelector = CConvergedServiceSelector::NewL();

    iDialogs = CCaUiDialogs::NewL( *this );
    
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::ConstructL() Complete");
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
    delete iServiceSelector;
    
    // Delete VoIP extension if VoIP is supported
    if ( iVoIPProfileHandler )
        {
        delete iVoIPProfileHandler;
        }
    
    delete iEmergencyCall;      
    delete iContactSelector;
    delete iFactory;
    delete iVTSettingQuery;

    // Clear dial components.
    ClearDialContents();

    if ( CCoeEnv::Static() )
        {
        // Close resource loader.
        iResourceLoader.Close();
        }
    // Delete the menubuffer.
    delete iBuf;
    iBuf = NULL;
  
    delete iVTString;
    iVTString = NULL;

    delete iContactStoreLoader;
    delete iDialogs;
    
    // Frees the TLS! Must be done after FeatureManager is used.
    FeatureManager::UnInitializeLib();
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
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::DialL() Begin");
    iDialogs->SetReconfQueryOngoing( EFalse );
    // If iPhCltDialer and iDialData exist, 
    // destroy them and create new ones. This clears the dial data.
    ClearDialContents();
    iPhCltDialer = CPhCltExtPhoneBase::NewL( this );
    iObserver = &aObserver;
    iDialData = CPhCltExtPhoneDialData::NewL();
   
    CAiwInternalDialData* aiwDialData = CAiwInternalDialData::NewLC( aDialData );
    
    TPhCltTelephoneNumber  aiwTelNumber( aiwDialData->PhoneNumber() );
    HBufC* uriParams = FindAndRipURIParametersL( aiwTelNumber );
    CleanupStack::PushL( uriParams );
    aiwDialData->SetPhoneNumberL( aiwTelNumber );
    

    // ConvertDialDataL overwrites, if given in aDialData
    SetDialDataCallType( aCallType );
    
    // Convert AIW dial data to PhoneClient format.
    ConvertDialDataL( *aiwDialData  );
    
    // Check if there has been a long press of Send key
    if ( iDialData->InitiateCall() )
        {
        HandleLongPressOfSendKeyL();
        }
    
    iDialData->SetSendKeyPressed( 
        IsSendKeyPressed( aCallType, aiwDialData->CallType() ) );
    

    // By default call is made, unless user cancels the procedure.
    TBool createCall = ETrue;

    TPhCltCallType callType = iDialData->CallType();
    
    if ( aCallType == ECaUiCallTypeVoice )
        {
        iDialData->SetCallType( EPhCltVoice );
        }
    // If not forced video call is wanted, ask user to confirm it.
    // ECaUiCallTypeUnknown indicates that the call is not made from options menu.
    else if ( iDialData->CallType() == EPhCltVideo 
                            && aCallType == ECaUiCallTypeUnknown )
        {
        // Get preferred call-type from URI params, it will be passed to
        // list query and selected by default
        callType = CallTypeFromUriParams( *uriParams );
        // If query is canceled, then call is not made.
        createCall = iDialogs->LaunchCreateListQueryL( callType );
        // Set the call type, video must be forced
        if ( EPhCltVideo == callType )
            {
            iDialData->SetCallType( EPhCltForcedVideo );
            }
        else
            {
            iDialData->SetCallType( callType );
            }
        }
    else if ( aCallType == ECaUiCallTypeInternet )
        {
        iDialData->SetCallType( EPhCltCallVoIP );
        }
    else if ( aCallType == ECaUiCallTypeVideo )
        {
        // (Forced) video call must be created.
        iDialData->SetCallType( EPhCltForcedVideo );
        }
        
    CleanupStack::PopAndDestroy( uriParams );
    
    
    
    // If user did not Cancel query or query was not shown, 
    // perform the dial operation.
    if ( createCall )
        {
        if ( iDialData->ContactLink() != KNullDesC8() 
            && !aiwDialData->PhoneNumber().Length() )
            {

            TBool storeLoaded 
                = LaunchContactStoreLoadingL( iDialData->ContactLink() );
            if ( storeLoaded )
                {
                // Launch address select.
                // aCallType describes which selector is used. ( VoIP/PhoneNumber )
                LaunchAddressSelectL( iDialData->ContactLink(), aCallType );
                }
            }
        else 
            {
            PhoneClientDialL( NULL );
            }
        }
    else
        {
        HandleDialL( KErrCancel );
        }
    
    CleanupStack::PopAndDestroy( aiwDialData );
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::DialL() End");
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::LaunchNoNetSupNoteL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::LaunchNoNetSupNoteL()
    {

    iDialogs->LaunchNoNetSupNoteL();

    }



// -----------------------------------------------------------------------------
// CCaUiEngine::LaunchReconConfQueryL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::LaunchReconConfQueryL( 
    MCaUiReconnectQueryObserver& aObserver,
    CPhCntMatcher* aPhCntMatcher,
    const TPtrC aPhoneNumber,
    const TBool aIncludeVideoCallOption )
    {
    // Empty pointer.
    TPtrC ptrc;

    iDialogs->LaunchReconConfQueryL(
        aObserver,
        aPhCntMatcher,
        aPhoneNumber,
        aIncludeVideoCallOption,
        ptrc );
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::CancelReconConfQuery
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::CancelReconConfQuery()
    {

    iDialogs->CancelReconConfQuery();

    }


// -----------------------------------------------------------------------------
// CCaUiEngine::LaunchNoteAndReconConfQueryL
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::LaunchNoteAndReconConfQueryL( 
    MCaUiReconnectQueryObserver& aObserver,
    CPhCntMatcher* aPhCntMatcher,
    const TPtrC aPhoneNumber,
    const TBool aIncludeVideoCallOption,
    const TPtrC aNoteText )
    {
    // Empty pointer.
    const TPtrC ptrc;

    iDialogs->LaunchNoteAndReconConfQueryL( 
        aObserver,
        aPhCntMatcher,
        aPhoneNumber,
        aIncludeVideoCallOption,
        aNoteText,
        ptrc );

    }


// -----------------------------------------------------------------------------
// CCaUiEngine::CancelNoteAndReconConfQuery
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::CancelNoteAndReconConfQuery()
    {

    iDialogs->CancelNoteAndReconConfQuery();

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
    TBool addressSelected( EFalse );
    TCaUiCallType callType( ECaUiCallTypeVoice );

    if ( aVoIPAddressSelect )
        {
        callType = ECaUiCallTypeInternet;
        }
    
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
        MVPbkContactStore* /*aStore*/, TInt aErrorCode )
    {
    TInt result( aErrorCode );
    
    if ( KErrNone == result )
        {
        TCaUiCallType caUiCallType( ECaUiCallTypeUnknown );
        ConvertPhCltCallType( iDialData->CallType(), caUiCallType );
        
        TRAP( result, 
            LaunchAddressSelectL( iDialData->ContactLink(), caUiCallType ) );
        }
    
    if ( KErrNone != result )
        {
        TRAP_IGNORE( HandleDialL( aErrorCode ) );
        }
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
    if ( !iDialogs->IsReconfQueryOngoing() )
        {
        ClearDialContents();
        }
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::OptionSelected
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::OptionSelected( TCaUiReconType aReconType )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::OptionSelected() Begin");
    if ( aReconType == ECallUiReconTypeVoice ||
         aReconType == ECallUiReconTypeVideo )
        {
        // Call is wanted to be made. Do it only if Dial data exists.
        if ( iDialData )
            {
          
            iDialogs->SetReconfQueryOngoing( EFalse );

            if( aReconType == ECallUiReconTypeVoice )
                {
                // Make sure that call type is correct, i.e. voice call.
                iDialData->SetCallType( EPhCltVoice );
                }
            else
                {
                // Make sure that call type is correct, i.e. video call.
                iDialData->SetCallType( EPhCltVideo );
                }

            TRAPD( err, iPhCltDialer->DialL( *iDialData ) );
            if( err )
                {
                CAUILOGSTRING2("CALLUI: >>>CCaUiEngine DialL error = %d", err );
                }
            }
        }
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::OptionSelected() End");
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::ConvertDialDataL
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
    else if ( aiwCallType == CAiwDialData::EAIWForcedCS )
        {
        phCltCallType = EPhCltVoice; 
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
    
    // Set Long keypress activity. ETrue if client wants to initate 
    // voice/video call.
    iDialData->SetInitiateCall( aAiwDialData.InitiateCall() );

    if ( aAiwDialData.ServiceId() )
        {
        iDialData->SetServiceId( aAiwDialData.ServiceId() );
        }
    // Remove invalid chars.
    if( iDialData->CallType() != EPhCltCallVoIP )

        {
        TPhCltTelephoneNumber telnum;
        telnum.Zero();
        telnum = iDialData->TelephoneNumber();
        PhCltUtils::RemoveInvalidChars( telnum );

        iDialData->SetTelephoneNumber( telnum );
        }
    iDialData->SetUUIL( aAiwDialData.UUI() );
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
// CCaUiEngine::IsVoIPProfiles()
// 
// 
// -----------------------------------------------------------------------------
//
TBool CCaUiEngine::IsVoIPProfiles()
    {
    TBool isProfiles = EFalse;

    TRAP_IGNORE( isProfiles = iVoIPProfileHandler->IsVoIPProfilesL() );
      
    return isProfiles; 
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::GetVoIPServiceIdsL()
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::GetVoIPServiceIdsL( RIdArray& aVoipServiceIds ) const
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        iVoIPProfileHandler->GetVoIPServiceIdsL( aVoipServiceIds );
        }
    }


// -----------------------------------------------------------------------------
// CCaUiEngine::GetVoipServiceNameL()
// 
// 
// -----------------------------------------------------------------------------
//
void CCaUiEngine::GetVoipServiceNameL( TServiceId aServiceId, TDes& aServiceName ) const
    {
    iVoIPProfileHandler->GetVoipServiceNameL( aServiceId, aServiceName );
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
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::LaunchAddressSelectL() start");
    if ( !iFactory )
        {
        iFactory = CreateCntFactoryL();
        }
    if ( !iContactSelector ) 
        {
        iContactSelector = iFactory->CreateContactDataSelectionL();
        }
    
    CPhCntContactDataSelection::TCallType callType;
   
    // if aCallType is ECaUiCallTypeUnknown call is not intitated from Call ui menu
    // so have to use iDialDataCallType 
    if ( aCallType != ECaUiCallTypeUnknown )  
        {
        ConvertCaUiCallType( aCallType, callType );
        }
    else
        {
        ConvertDialDataCallType( callType );
        }
        
    iContactSelector->Cancel();
    iContactSelector->SelectPhoneNumberForCallL( aContactLink, 
                                                 callType, *this );
 
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::LaunchAddressSelectL() end");
    }

    
//------------------------------------------------------------------------------
// CCaUiEngine::FindAndRipURIParametersL()
// 
// 
// -----------------------------------------------------------------------------
//
HBufC* CCaUiEngine::FindAndRipURIParametersL( TDes& aNumber ) const
    {
    HBufC* buf = NULL;    
    TInt offset = aNumber.Locate( TChar( KSemiColon()[0] ) );
    if( offset > 0 ) // Separator is no allowed to be a first char
        {
        __ASSERT_ALWAYS( offset <= aNumber.Length(), 
            User::Leave( KErrGeneral ) );
        buf = aNumber.Mid( offset ).AllocL();         
        aNumber.SetLength( offset );           
        }
    else
        {
        buf = HBufC::NewL( 0 );
        }
    return buf;    
    }
   
//------------------------------------------------------------------------------
// CBrowserTelService::CallTypeFromUriParams()
// 
// 
// -----------------------------------------------------------------------------
//  
TPhCltCallType CCaUiEngine::CallTypeFromUriParams( 
                                           const TDesC& aUriParams ) const
    {
    TPhCltCallType type( EPhCltVoice ); // Default type is voice call
    
    TPtrC value( ParamValue( KCallTypeUriParam, aUriParams ) );
    if( value.CompareF( KCallTypeVideo ) == 0 ) // URIs are case-insensitive.
        {
        type = EPhCltVideo;
        }
    // Otherwise return default type
    return type;
    }
    
//------------------------------------------------------------------------------
// CBrowserTelService::ParamValue()
// 
// 
// -----------------------------------------------------------------------------
//     
TPtrC CCaUiEngine::ParamValue( const TDesC& aName, const TDesC& aUri ) const
    {  
    TInt startIndex = 0;
    TInt offset = 0;
    TPtrC pairTmp( KNullDesC()  );
    TPtrC ret( KNullDesC() );
    TBool found = EFalse;

    if( aName.Length() > 0 )
        {
        while( startIndex < aUri.Length() && !found )
            {
            pairTmp.Set( FindNextParamValuePair( aUri, startIndex ) );
            offset = pairTmp.FindF( aName ); // URIs are case-insensitive.
            if( offset >= 0 && 
              ( offset + aName.Length() + 1 < pairTmp.Length() ) )
                {
                ret.Set( pairTmp.Mid( offset + aName.Length() + 1 ) );
                found = ETrue;
                }
            }
        }
        
    return ret;    
    }    
   
//------------------------------------------------------------------------------
// CCaUiEngine::FindNextParamValuePair()
// 
// 
// -----------------------------------------------------------------------------
//   
TPtrC CCaUiEngine::FindNextParamValuePair( const TDesC& aUri, 
                                           TInt& aStartIndex ) const
    {  
    TInt offset = 0;
    TPtrC tmp( KNullDesC() );
    TPtrC ret( KNullDesC() );

    tmp.Set( aUri.Mid( aStartIndex ) );
    // Search start mark for new paramter.
    offset = tmp.Find( KSemiColon );
    if( offset >= 0 )
        {
        if( aStartIndex + offset + KSemiColon().Length() < aUri.Length() ) 
            {
            // Move start index
            aStartIndex += offset + KSemiColon().Length();
        
            // Store descriptor after start mark
            tmp.Set( aUri.Mid( aStartIndex ) );
            
            // Search end mark (semi-colon)
            offset = tmp.Find( KSemiColon );
            if ( offset >= 0 )
                {
                // If end mark was found store the param/value pair
                ret.Set( tmp.Left( offset ) );
                }
            else
                { 
                // If end mark is not found, 
                // the rest of descriptor belong to this parameter
                ret.Set( tmp );
                }      
            }
        }
    
    // Move start offset based on findings    
    if( ret.Length() )
        {
        aStartIndex += ret.Length();
        }
    else
        {
        aStartIndex = aUri.Length();
        }        

    return ret;
    }    

// ---------------------------------------------------------
//  CCaUiEngine::HandleFirstLongPressOfSendKeyL()
// ---------------------------------------------------------
//
TBool CCaUiEngine::HandleFirstLongPressOfSendKeyL()
    {
    TBool set( EFalse );    
    
    TInt result = iDialogs->QueryVideoCallDefaultActionL();
       
    // Check user decision and set return value accordingly     
    switch ( result )
        {
        case EAknSoftkeyYes: // from avkon.hrh
            set = ETrue;
            break;
        case EAknSoftkeyNo:            
            set = EFalse;
            break;                                                           
        default:        
            set = EFalse;
            break;
        }    
    return set;
    }

// ---------------------------------------------------------
//  CCaUiEngine::HandleLongPressOfSendKeyL()
// ---------------------------------------------------------
//    
void CCaUiEngine::HandleLongPressOfSendKeyL()
    {
        
    // Check if the setting is already ON -> make a video call
    if ( iLongPressSet == ECaUiLongPressKeyInUse )
        {
        // ON -> initiate a video call 
        // (Forced) video call must be created
        iDialData->SetCallType( EPhCltForcedVideo );
        }
    // Check if the setting is already OFF -> make a voice call
    else if ( iLongPressSet == ECaUiLongPressKeyNotInUse )
        {
        // OFF -> initiate a voice call
        // Voice call must be created
        iDialData->SetCallType( EPhCltVoice );    
        }
    // If the setting has never been set, check what the user wants to do
    else if ( iLongPressSet == ECaUiLongPressKeyNotSet )
        {
           // Ask from the user how long press of Send key should be handled
        TBool set = HandleFirstLongPressOfSendKeyL();
    
        if ( set )
            {
            // User answered YES -> initiate a video call
            // (Forced) video call must be created
            iLongPressSet = ECaUiLongPressKeyInUse;
            iDialData->SetCallType( EPhCltForcedVideo );
            }
        else
            {
            // User answered NO -> initiate a voice call
            // Voice call must be created
            iLongPressSet = ECaUiLongPressKeyNotInUse; 
            iDialData->SetCallType( EPhCltVoice );
            }              
        }        
    }

// ---------------------------------------------------------
//  CCaUiEngine::SelectionDone()
// ---------------------------------------------------------
//     
void CCaUiEngine::SelectionDone( CPhCntSelectedData* aContactData, 
                                  TInt aErrorCode )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::SelectionDone() Start");
    CAUILOGSTRING2("CALLUI: >>>SelectionDone() ECode = %d", aErrorCode );
    if ( aErrorCode == KErrNone )
        {
        TPhCltTelephoneNumber phoneNumber;
        HBufC8* fieldLink = NULL;
        
        if ( iDialData->TelephoneNumber() == KNullDesC )
            {
            phoneNumber = aContactData->Data();

            // Do not remove illegal chars if call type is VOIP and
            // call is made to a voip number
            if ( !( EPhCltCallVoIP == iDialData->CallType() &&
                    aContactData->NumberType()==MPhCntMatch::EVoipNumber ) )
                {                    
                PhCltUtils::RemoveInvalidChars( phoneNumber );
                }                                    
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
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::SelectionDone() End");
    }
// ---------------------------------------------------------
//  CCaUiEngine::PhoneClientDialL()
// ---------------------------------------------------------
// 
void CCaUiEngine::PhoneClientDialL( const TDesC8* aFieldLink )
    {
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::DialL()::PhoneClient dial Start");
    
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
        error = KErrNone;
        iEmergencyCall->DialEmergencyCallL( iDialData->TelephoneNumber() );
        }
    
    if ( createCall )
        {
        CConvergedServiceSelector::TSsResult results;
        
        error = iServiceSelector->GetCallingServiceByCallType( 
                        results,
                        ServiceSelectorCallType(),
                        iDialData->ServiceId(),
                        iDialData->SendKeyPressed(),
                        iDialData->TelephoneNumber() );
            
        if ( KErrNone == error )
            {
            SetSelectorResults( results );
            }
        else
            {
            createCall = EFalse;
            }
        }
    
    // Here note launch + query if this is video call and we
    // are not in 3G network.
   
    if ( createCall )
        {
        if ( ( iDialData->CallType() == EPhCltVideo ) ||
             ( iDialData->CallType() == EPhCltForcedVideo ) )
            {
            if ( !IsVideoCallAllowedL() )
                {
                createCall = EFalse;
                error = KErrCancel;
                }
            }
        }
    if ( createCall )
        {
        CAUILOGSTRING("CALLUI: >>>CCaUiEngine::DialL(): Call PhoneClientdial");
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
    CAUILOGSTRING("CALLUI: >>>CCaUiEngine::DialL()::PhoneClient dial End");
    }
    
// ---------------------------------------------------------
//  CCaUiEngine::SetDialDataCallType()
// ---------------------------------------------------------
// 
void CCaUiEngine::SetDialDataCallType( const TCaUiCallType aCallType )
    {
    if ( aCallType == ECaUiCallTypeVoice )
        {
        // Voice call must be created.
        iDialData->SetCallType( EPhCltVoice );
        }
    else if ( aCallType == ECaUiCallTypeVideo )
        {
        // (Forced) video call must be created.
        iDialData->SetCallType( EPhCltForcedVideo );
        }
    else if ( aCallType == ECaUiCallTypeInternet )
        {
        // VoIP call must be created.
        iDialData->SetCallType( EPhCltCallVoIP );
        }
    else
        {
        // else Voice call is ok.
        iDialData->SetCallType( EPhCltVoice );
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

// ---------------------------------------------------------
//  CCaUiEngine::IsVideoCallAllowedL()
// ---------------------------------------------------------
// 
TBool CCaUiEngine::IsVideoCallAllowedL()
    {
    TInt err = KErrNone;
    TInt createCall( ETrue );
    TInt networkMode = KCaUiEngGsmNetwork; // Default: GSM.               
    err = RProperty::Get( KPSUidNetworkInfo, 
                          KNWTelephonyNetworkMode, 
                          networkMode );     
   
    if ( err )
        {
        // Information is not ok, so assume that
        // we are in GSM network.
        networkMode = KCaUiEngGsmNetwork;
        }
    // check network status
    TInt nwStatus = ENWStatusRegistrationUnknown;
    RProperty::Get( KPSUidNetworkInfo, KNWRegistrationStatus, nwStatus );

    // If we are not in WCDMA network, then it is not allowed 
    // to create a video call.
    if ( networkMode != KCaUiEngWcdmaNetwork )
        {     
        TInt automaticredial = 0;
        
        GetCenRepValueL( KCRUidTelephonySettings, KSettingsAutomaticRedial, automaticredial );        
        
        // if no network, fall back to voice call for proper error handling
        if ( (FeatureManager::FeatureSupported( KFeatureIdAutoRedialForVideoCall ) && automaticredial)
              || nwStatus == ENWStatusRegistrationUnknown
              || nwStatus == ENWStatusNotRegisteredNoService
              || nwStatus == ENWStatusNotRegisteredEmergencyOnly
              || nwStatus == ENWStatusNotRegisteredSearching )
            {
            // fallback to voice when no network support to videocall
            // handled in Phone.
            }
        else
            {
            createCall = EFalse;
            // Video call can not be done.
            // => Launch Note + Reconnect Query.

            // Empty pointer.
            TPtrC ptrc;

            iDialogs->LaunchNoteAndReconConfQueryL( 
                *this,
                NULL,
                iDialData->TelephoneNumber(),
                EFalse,
                ptrc,
                iDialData->Name() );
            }
        }
    return createCall;
    }
// ---------------------------------------------------------
//  CCaUiEngine::HandleEmergencyDialL()
// ---------------------------------------------------------
//    
void CCaUiEngine::HandleEmergencyDialL( const TInt aStatus )
    {
    if ( iObserver )
        {
        iObserver->HandleDialResultL( aStatus );
        }
    }
// ---------------------------------------------------------
//  CCaUiEngine::GetCenRepValue()
// ---------------------------------------------------------
// 
void CCaUiEngine::GetCenRepValueL( 
        const TUid& aUid, 
        const TUint aId,
        TInt& aValue ) const
    {
    CRepository* repository = CRepository::NewL( aUid );
    TInt err = repository->Get( aId, aValue );
     
    delete repository;
    repository = NULL;
     
    if ( err != KErrNotFound ) // KErrNotFound acceptable.
        {
        User::LeaveIfError( err );
        }
    }

// ---------------------------------------------------------
//  CCaUiEngine::ConvertCaUiCallType( )
// ---------------------------------------------------------
//    
void CCaUiEngine::ConvertCaUiCallType( const TCaUiCallType aCallType,
                    CPhCntContactDataSelection::TCallType& aResultCallType  )
    {
    switch ( aCallType )
        {
        case ECaUiCallTypeVoice:
            {
            aResultCallType = CPhCntContactDataSelection::ECallPhoneNumber;
            break;
            }
        case ECaUiCallTypeVideo:
            {
            aResultCallType = CPhCntContactDataSelection::ECallVideoNumber;
            break;
            }
        case ECaUiCallTypeInternet:
            {
            aResultCallType = CPhCntContactDataSelection::ECallVoip;
            break;
            }
        default:
            {
            aResultCallType = CPhCntContactDataSelection::ECallPhoneNumber;
            break;
            }
        }
    }
    
// ---------------------------------------------------------
//  CCaUiEngine::ConvertDialDataCallType()
// ---------------------------------------------------------
//    
void CCaUiEngine::ConvertDialDataCallType( CPhCntContactDataSelection::TCallType& aResultCallType )
    {
    switch ( iDialData->CallType() )
        {
        case EPhCltVoice:
            {
            aResultCallType = CPhCntContactDataSelection::ECallPhoneNumber;
            break;
            }
        case EPhCltVideo:
        case EPhCltForcedVideo:
            {
            aResultCallType = CPhCntContactDataSelection::ECallVideoNumber;
            break;
            }
        case EPhCltCallVoIP:
            {
            aResultCallType = CPhCntContactDataSelection::ECallVoip;
            break;
            }
        default:
            {
            aResultCallType = CPhCntContactDataSelection::ECallPhoneNumber;
            break;
            }
        }
    }


// ---------------------------------------------------------
//  CCaUiEngine::ConvertPhCltCallType
// ---------------------------------------------------------
//    
void CCaUiEngine::ConvertPhCltCallType( const TPhCltCallType aCallType,
        TCaUiCallType& aResultCallType ) const
    {
    switch ( aCallType )
        {
        case EPhCltVoice:
            {
            aResultCallType = ECaUiCallTypeVoice;
            break;
            }
        case EPhCltVideo:
        case EPhCltForcedVideo:
            {
            aResultCallType = ECaUiCallTypeVideo;
            break;
            }
        case EPhCltCallVoIP:
            {
            aResultCallType = ECaUiCallTypeInternet;
            break;
            }
        default:
            {
            break;
            }
        }
    }


// ---------------------------------------------------------
//  CCaUiEngine::ServiceSelectorCallType()
// ---------------------------------------------------------
//    
CConvergedServiceSelector::TSsCallType CCaUiEngine::ServiceSelectorCallType() const
    {
    CConvergedServiceSelector::TSsCallType ret;
    
    switch ( iDialData->CallType() )
        {
        case EPhCltVoice:
            {
            ret = CConvergedServiceSelector::ESsVoiceCall;
            break;
            }
        case EPhCltVideo:
        case EPhCltForcedVideo:
            {
            ret = CConvergedServiceSelector::ESsVideoCall;
            break;
            }
        case EPhCltCallVoIP:
            {
            ret = CConvergedServiceSelector::ESsVoipCall;
            break;
            }
        default:
            {
            ret = CConvergedServiceSelector::ESsVoiceCall;
            break;
            }
        }
    
    return ret;
    }

// ---------------------------------------------------------
//  CCaUiEngine::SetSelectorResults()
// ---------------------------------------------------------
//    
void CCaUiEngine::SetSelectorResults( 
        CConvergedServiceSelector::TSsResult& aResults )
    {
    switch ( aResults.iCallType )
        {
        case CConvergedServiceSelector::ESsVoipCall:
            {
            iDialData->SetCallType( EPhCltCallVoIP );
            break;
            }
        case CConvergedServiceSelector::ESsVideoCall:
            {
            iDialData->SetCallType( EPhCltForcedVideo );
            break;
            }
        case CConvergedServiceSelector::ESsVoiceCall:
        default:
            {
            iDialData->SetCallType( EPhCltVoice );
            break;
            }
        }
    
    iDialData->SetServiceId( aResults.iServiceId );
    }


// ---------------------------------------------------------
//  CCaUiEngine::LaunchContactStoreLoadingL
// ---------------------------------------------------------
//  
TBool CCaUiEngine::LaunchContactStoreLoadingL( const TDesC8& aContactLink )
    {
    if ( !iFactory )
        {
        iFactory = CreateCntFactoryL();
        }
    
    if ( !iContactStoreLoader )
        {
        iContactStoreLoader = iFactory->CreateContactStoreLoaderL();
        }
    
    TBool isStoreLoaded( 
        iContactStoreLoader->IsContactStoreLoaded( aContactLink ) );
    if ( !isStoreLoaded )
        {
        iContactStoreLoader->LoadContactStoreL( aContactLink, *this );        
        }
    
    return isStoreLoaded;
    }

// ---------------------------------------------------------
//  CCaUiEngine::IsSendKeyPressed
// ---------------------------------------------------------
//
TBool CCaUiEngine::IsSendKeyPressed( TCaUiCallType aCallType,
        CAiwDialData::TCallType aAiwCallType ) const
    {
    TBool isSendKeyPressed( EFalse );
    if ( CAiwDialData::EAIWForcedCS != aAiwCallType )
        {
        if ( ECaUiCallTypeUnknown == aCallType )
            {
            isSendKeyPressed = ETrue;
            }
        }
    
    return isSendKeyPressed;
    }

// End of file
