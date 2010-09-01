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
* Description:  Provides engine implementation for CallUI.
*
*/


#ifndef CAUIENGINE_H
#define CAUIENGINE_H

//  INCLUDES
#include    <e32std.h>
#include    <ConeResLoader.h>           // RConeResourceLoader.
#include    <mphcltextphoneobserver.h>  // MPhCltExtPhoneObserver.
#include    "cauieng.h"
#include    "cauidialresultobserver.h"
#include    "CaUiQueryObserver.h"
#include    <PhCltTypes.h>
#include    <MPhCltEmergencyCallObserver.h>
#include    <cphcntcontactdataselection.h>  // MPhCntSelectionObserver
#include    <CPhCltEmergencyCall.h>     // CPhCltEmergencyCall
#include    <mcauireconnectqueryobserver.h>
#include    <cconvergedserviceselector.h>
#include    <mphcntstoreloaderobserver.h>
#include    <aiwdialdata.h>

// FORWARD DECLARATIONS
class CPhCltExtPhoneBase;
class CAknGlobalListQuery;
class CAknGlobalConfirmationQuery;
class CAknGlobalNote;
class CCaUiReConQueryInformation;
class CAiwInternalDialData;
class CPhCltExtPhoneDialData;
class CPhCntSingleItemFetch;
class CPhCntFactory;
class CCaUiVoIPExtension;
class MPhCntStoreLoader;
class MVPbkContactStore;

class CCaUiDialogs;

// CONSTANTS

// Owned and used resource file.
_LIT( KCallUIResFile,         "z:\\resource\\CallUI.rsc" );


    /**   
    * Indicates state of long press key
    * Default value: 0
    * Default value for iMode variant: 1    
    */
enum TCaUiLongPressKey
    {
    ECaUiLongPressKeyNotSet      = 0, // Long press key is not set
    ECaUiLongPressKeyNotInUse    = 1, // Long press key is not in use
    ECaUiLongPressKeyInUse       = 2, // Long press key is in use
    };
   
// CLASS DECLARATION

/**
*  CaUIEngine implementation.
*  
*  @lib CaUiEngine.lib
*  @since 2.6
*/
NONSHARABLE_CLASS( CCaUiEngine )
:   public CCaUiEngApi,
    public MPhCltExtPhoneObserver,
    public MCaUiReconnectQueryObserver,
    public MPhCntSelectionObserver,
    public MPhCltEmergencyCallObserver,
    public MPhCntStoreLoaderObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @return an instance of CallUI.
        */
        static CCaUiEngine* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCaUiEngine();


    public:  // New functions

        /**
        * Make the dial operation.
        * 
        * @see CCaUiEngApi::DialL().
        */
        void DialL(
            MCaUiDialResultObserver& aObserver,
            const TDesC8& aDialData,
            const TCaUiCallType aCallType );

        /**
        * Launch the qtn.tel.no.video.network information note.
        * 
        * @see CCaUiEngApi::LaunchNoNetSupNoteL().
        */
        void LaunchNoNetSupNoteL();


        /**
        * Launch reconnect confirmation query.
        * 
        * @see CCaUiEngApi::LaunchReconConfQueryL().
        */
        void LaunchReconConfQueryL( 
            MCaUiReconnectQueryObserver& aObserver,
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption );

        /**
        * Cancel reconnect query.
        * 
        * @see CCaUiEngApi::CancelReconConfQuery().
        */
        void CancelReconConfQuery();


        /**
        * Launch note and reconnect confirmation query.
        * 
        * @see CCaUiEngApi::LaunchNoteAndReconConfQueryL().
        */
        void LaunchNoteAndReconConfQueryL( 
            MCaUiReconnectQueryObserver& aObserver,
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption,
            const TPtrC aNoteText );

        /**
        * Cancel note and reconnect query.
        * 
        * @see CCaUiEngApi::CancelNoteAndReconConfQuery().
        */
        void CancelNoteAndReconConfQuery();

        /**
        * Launch address selection.
        * @since 3.0
        * 
        * @see CCaUiEngApi::LaunchAddressSelectL(). 
        */
        TBool LaunchAddressSelectL(
            TDes& aTelNum, 
            const TInt aContactId,
            const TBool aVoIPAddressSelect );

        /**
        * Cancel address select launched with LaunchAddressSelectL.
        * @since 3.0
        * 
        * @see CCaUiEngApi::CancelAddressSelect().
        */
        void CancelAddressSelect();

    public: // from MPhCntStoreLoaderObserver
        
        /**
         * From MPhCntStoreLoaderObserver;
         * @since S60 v9.1
         * @see MPhCntStoreLoaderObserver.
         */
        void ContactStoreLoadingCompleted( 
            MVPbkContactStore* aStore, TInt aErrorCode );
    
    private: // Functions from base classes

        /**
        * Handle dial request completion.
        * 
        * @see MPhCltExtPhoneObserver::HandleDialL().
        */
        void HandleDialL( const TInt aStatus );

        /**
        * Called when CCaUiReconnectQuery has got a key press.
        * 
        * @see MCaUiReconnectQueryObserver::OptionSelectedL().
        */
        void OptionSelected( MCaUiReconnectQueryObserver::TCaUiReconType aReconType );

    
    private:

        /**
        * C++ default constructor.
        */
        CCaUiEngine();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Convert the AIW dialdata to PhoneClient format.
        * 
        * @param aAiwDialData AIW DialData format.
        */
       void ConvertDialDataL(
            const CAiwInternalDialData& aAiwDialData );
   

        /**
        * Clear the Dial related contents if they exist.
        */
        void ClearDialContents();



    public:

        /**
        * Check is there VoIP profiles in phone.
        * 
        * @since 3.0
        * @return ETrue if found, EFalse otherwise.
        */
        TBool IsVoIPProfiles();
        
        /**
        * @see CCaUiEngApi::GetVoIPServiceIdsL
        */
        void GetVoIPServiceIdsL( RIdArray& aVoipServiceIds ) const;
        
        /**
        * @see CCaUiEngApi::GetVoipServiceNameL
        */
        void GetVoipServiceNameL( TServiceId aServiceId, TDes& aServiceName ) const;

    private:

        /**
        * Launch address selection. If contact contains default number or 
        * only one number, number will be returned directly. 
        * Otherwise address selection list displayed.
        * @since 3.0
        * 
        * @param aTelNum Contains selected number after execution.
        * @param aContactId The contact id from PhoneBook.
        * @param aCallType If calltype is ECaUiCallTypeInternet VoIP address
        *     select is used. 
        * @param aRemoveInvalidChars Removes invalid chars from selected number
        *                            if set ETrue.
        * @param aFocusedField Focused field from PhoneBook
        * @return TBool, ETrue if number selected, EFalse otherwice 
        *     (E.g. list canceled)
        */
        void LaunchAddressSelectL( 
            const TDesC8& aContactLink,
            const TCaUiCallType aCallType
            );


        /**
        * Looks for tel URI parameters from a specified phone number.
        * @param aNumber The phone number that might contain URI parameters
        * @return HBufC containing URI parameters, if parameters couldn't 
        * be found the HBufC with zero length is returned
        * NOTE: The parameters will be ripped off from the given phone 
        * number (aNumber) if found.
         */
        HBufC* FindAndRipURIParametersL( TDes& aNumber ) const;
        
        /**
         * Looks call-type parameter from, URI params and converts the
         * possible found hint as a return value.
         * @param aUriParams The descriptor containing URI part of the tel URI.
         * Parameters can be extracted from URI by using 
         * FindAndRipURIParametersL method.
         * @return The hinted call type defined in URI parameters. 
         * If call-type parameter couldn't be found the EPhCltVoice is returned.
         */        
        TPhCltCallType CallTypeFromUriParams( const TDesC& aUriParams ) const;
        
        /**
         * Utility method to search value of named parameter from tel URI.
         * @param aName The name of parameter what is searched
         * @param aUri The URI from from which parameter is searched
         * @return The descriptor containing the valud of named parameter.
         * If named parameter couldn't be found, KNullDesC is returned
         */        
        TPtrC ParamValue( const TDesC& aName, const TDesC& aUri ) const;
        
        /**
         * Utility method to loop through all parameters in Tel URI.
         * @param aUri Tel URI from which the parameters are searched.
         * @param aStartIndex Offset from which searching is started.
         * NOTE: method modifies aStartIndex parameter, always when 
         * it returns. So 
         * @return The descriptor containing the next parameter name=value pair.
         */        
        TPtrC FindNextParamValuePair( const TDesC& aUri, 
                                      TInt& aStartIndex ) const;
     
        /**
        * Handle first long press of send key
        * @since 3.2
        * Leaves if cenrep data can't be read.
        * @return User decision about the VT setting
        */
        TBool HandleFirstLongPressOfSendKeyL();
        
        /**
        * General handling of long press of send key
        * @since 3.2
        * Leaves if cenrep data can't be read.        
        */
        void HandleLongPressOfSendKeyL();
        
        /**
        *  from MPhCntSelectionObserver class 
        * @since 3.2
        * Leaves if aContactData data can't be read.        
        */
        void SelectionDone( CPhCntSelectedData* aContactData, TInt aErrorCode );
        
        /**
        * Makes phoneclient dial request 
        * @since 3.2
        * @param fieldLink   link to phonenumber in contacts. If NULL call is 
        * not created from Phonebook.        
        */
        void PhoneClientDialL( const TDesC8* fieldLink );
        
        /**
        * Sets call type to phoneclient dial data struct.
        * @since 3.2
        * @param aCallType Call type to set in dialdata struct.        
        */
        void SetDialDataCallType( const TCaUiCallType aCallType ); 
        
        /**
        * Checks if number is emergency number using
        * Emergency call API
        * @since 3.2
        * returns ETrue if number is emergency number. Default EFalse.        
        */
        TBool IsEmergencyNumber();
        
        /**
        * Handling of emergency dial result. 
        * @since 3.2
        * @see MPhCltEmergencyCallObserver::HandleEmergencyDialL().        
        */
        void HandleEmergencyDialL( const TInt aStatus ); 

        /**
        * Checks if Video Call can be made
        * @since 3.2
        * Leaves if cenrep data can't be read.
        * returns ETrue if call can be made.
        */
        TBool IsVideoCallAllowedL();
        
        /**
        * Gets Central repository value
        * @since 3.2
        * @param aUid Central repository uid.
        * @param aId, Central repository key Id.
        * @param aValue  Cen Rep key value. 
        * Leaves if cenrep data can't be read.
        */
        void GetCenRepValueL( const TUid& aUid, 
                             const TUint aId,
                             TInt& aValue ) const;
        
        /**
        * Converts given TCaUiCallType to CPhCntContactDataSelection::TCallType 
        * @since 3.2
        * @param aCallType CallType to convert
        * @param aResultCallType converted type
        * 
        */
        void ConvertCaUiCallType( const TCaUiCallType aCallType,
                    CPhCntContactDataSelection::TCallType& aResultCallType  );
        
         /**
        * Converts CPhCltExtPhoneDialData callType 
        * to CPhCntContactDataSelection::TCallType 
        * @since 3.2
        * @param aResultCallType converted type
        * 
        */            
        void ConvertDialDataCallType( 
                    CPhCntContactDataSelection::TCallType& aResultCallType );

      /**
       * Converts TPhCltCallType callType to TCaUiCallType. 
       * @since S60 v9.1
       * @param aCallType           Source type
       * @param aResultCallType     Converted type
       */
        void ConvertPhCltCallType( const TPhCltCallType aCallType,
            TCaUiCallType& aResultCallType ) const;
        
        /**
        * Returns Service Selectors calltype by dial data calltype.
        * @since 5.0
        * @param Selector calltype.
        */            
        CConvergedServiceSelector::TSsCallType ServiceSelectorCallType() const;
       
        /**
        * Converts CCConvergedServiceSelector results 
        * to CPhCltExtPhoneDialData (calltype and service id) 
        * @since 5.0
        * @param aResults Service selectors results.
        */            
        void SetSelectorResults( 
                  CConvergedServiceSelector::TSsResult& aResults );

        /**
        * Starts to load specified contact store if not yet loaded. 
        * If loading is needed, MPhCntStoreLoaderObserver::ContactStoreLoaded
        * is called when store is ready for use.
        *  
        * @since    S60 v9.1
        * @param    aContactLink   Contact link specifying store to load.
        * @return   ETrue if store is already loaded.
        */
        TBool LaunchContactStoreLoadingL( const TDesC8& aContactLink );

        /**
        * Resolves with a help of call type values whether call is initiated
        * with a send command or through the explicit menu option. 
        * 
        * @since    S60 v9.1
        * @param    aCallType       Call UI call type.
        * @param    aAiwCallType    AIW call type.
        * @return   ETrue if send key pressed, EFalse otherwise.
        */
        TBool IsSendKeyPressed( TCaUiCallType aCallType,
            CAiwDialData::TCallType aAiwCallType ) const;
        
    private:    // Data

        // Owned resource loader.
        RConeResourceLoader             iResourceLoader;

        // Buffer for returned localized texts.
        HBufC*                          iBuf;

        // PhoneClient for call creation.
        CPhCltExtPhoneBase*             iPhCltDialer;

        // Handler class for PhoneClient Dial data.
        CPhCltExtPhoneDialData*         iDialData;
        // Emergency call API
        CPhCltEmergencyCall*            iEmergencyCall;
        
        // The observer to handle CallUI dial results.
        MCaUiDialResultObserver*        iObserver;
        
        // VoIP extension.        
        CCaUiVoIPExtension*             iVoIPProfileHandler;

        // Long press of Send key setting state, NOT SET/ON/OFF
        TInt iLongPressSet;
        
        // The qtn.tel.xxx note shown to the user.
        CAknGlobalConfirmationQuery*    iVTSettingQuery;

        // Long key press note buffer        
        HBufC* iVTString;
        // Contact factory for PhoneCntFinder connection
        CPhCntFactory* iFactory;
        
        // Contact selector for fetching contact information from phonebook
        CPhCntContactDataSelection* iContactSelector;
        
        // Service selector for calling service (service id and type)
        CConvergedServiceSelector*      iServiceSelector;

        /**
         * Contact store loader.
         * Own.
         */
        MPhCntStoreLoader* iContactStoreLoader;

        // Dialogs class. Own
        CCaUiDialogs* iDialogs;

    };

#endif  // CAUIENGINE_H

// End of file
