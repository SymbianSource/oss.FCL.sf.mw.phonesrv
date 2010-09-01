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
* Description:  API for CallUiEngine usage.
*
*/


#ifndef CAUIENG_H
#define CAUIENG_H


//  INCLUDES
#include <e32base.h>
#include <spdefinitions.h>  //RIdArray
#include "CaUI.hrh"


// FORWARD DECLARATIONS
class MCaUiReconnectQueryObserver;
class MDesC16Array;
class MCaUiDialResultObserver;
class CPhCntMatcher;


// CLASS DECLARATION

/**
*  The API for CallUIEngine usage.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
class CCaUiEngApi 
:   public CBase
    {
    public: // Data types

        /**
        * Enumerate all supported resource types.
        *
        * ECaUiResVoice         - Pure voice call resources.
        * ECaUiResVoiceAndVideo - Voice and video call resources.
        * ECaUiResVoiceInternetAndVideo - All Internet, voice and video call resources.
        * ECaUiResOnlyInternet  - Only Internet call resources.
        */
        enum TCaUiEngResource 
            {
            ECaUiResVoice                   = 0,
            ECaUiResVoiceAndVideo           = 1,
            ECaUiResVoiceInternetAndVideo   = 2,
            ECaUiResOnlyInternet            = 3
            };


        /**
        * Enumerate all supported localized text.
        *
        * ECallMenuItemText      - Menu item text for call.
        * EVoiceCallMenuItemText - Menu item text for voice call.
        * EVideoCallMenuItemText - Menu item text for video call.
        * EInternetCallMenuItemText - Menu item text for internet call.
        */
        enum TCaUiEngText 
            {
            ECallMenuItemText          = 0,
            EVoiceCallMenuItemText     = 1,
            EVideoCallMenuItemText     = 2,
            EInternetCallMenuItemText  = 3
            };

    public: // New functions

        /**
        * Make the dial operation.
        * 
        * @param aObserver The observer to handle dial result.
        * @param aDialData The TCallUIDialDataV1 dial data information in
        *                  packaged format.
        * @param aCallType If not equal to ECaUiCallTypeUnknown, then 
        *                  overwrites the call type in aDialData.
        */
        virtual void DialL(
            MCaUiDialResultObserver& aObserver,
            const TDesC8& aDialData,
            const TCaUiCallType aCallType ) = 0;

        /**
        * Launch the qtn.tel.no.video.network information note.
        * 
        */
        virtual void LaunchNoNetSupNoteL() = 0;


        /**
        * Launch the reconnect confirmation query shown to user.
        * 
        * @param aObserver The observer to handle query result.
        * @param aPhCntMatcher The Phone Contact Matcher. 
        *        Ownership not transferred. If NULL, then matcher is created 
        *        inside CaUIEngine.
        * @param aPhoneNumber Pointer to the number where dial was tried.
        * @param aIncludeVideoCallOption If set to ETrue, video call option is
        *        included to query, otherwise it is left out.
        */
        virtual void LaunchReconConfQueryL( 
            MCaUiReconnectQueryObserver& aObserver,
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption ) = 0;


        /**
        * Cancel reconnect query launched with LaunchReconConfQueryL.
        * 
        */
        virtual void CancelReconConfQuery() = 0;


        /**
        * Launch note and reconnect confirmation query in sequence.
        * 
        * @param aObserver The observer to handle query result.
        * @param aPhCntMatcher The Phone Contact Matcher. 
        *        Ownership not transferred. If NULL, then matcher is created 
        *        inside CaUIEngine.
        * @param aPhoneNumber Pointer to the number where dial was tried.
        * @param aIncludeVideoCallOption If set to ETrue, video call option is
        *        included to query, otherwise it is left out.
        * @param aNoteText The text shown in information note. If empty, then 
        *        qtn.tel.no.video.network text is shown.
        */
        virtual void LaunchNoteAndReconConfQueryL( 
            MCaUiReconnectQueryObserver& aObserver,
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption,
            const TPtrC aNoteText ) = 0;

        /**
        * Cancel note and reconnection query launched with 
        * LaunchNoteAndReconConfQueryL.
        * 
        */
        virtual void CancelNoteAndReconConfQuery() = 0;

        /**
        * Launch address selection. If contact contains default number or 
        * only one number, number will be returned directly. 
        * Otherwise address selection list displayed.
        * @since 3.0
        * 
        * @param aTelNum Contains selected number after execution.
        * @param aContactId The contact id from PhoneBook.
        * @param aVoIPAddressSelect If ETrue VoIP address select is used. 
        * @return TBool, ETrue if number selected, 
        *     EFalse otherwice (E.g. list canceled).
        */
        virtual TBool LaunchAddressSelectL( 
            TDes& aTelNum, 
            const TInt aContactId,
            const TBool aVoIPAddressSelect ) = 0;

        /**
        * Cancel address selection launched with LaunchAddressSelectL.
        * 
        */
        virtual void CancelAddressSelect() = 0;
        
        /**
        * Get VoIP services
        * @param aVoipServiceIds On return, contains the list of VoIP services
        */
        virtual void GetVoIPServiceIdsL( RIdArray& aVoipServiceIds  ) const = 0;
        
        /**
        * Get the service provider name
        * @param aServiceId The service, whose name is requested
        * @param aServiceName On return, contains the service name
        */
        virtual void GetVoipServiceNameL( TServiceId aServiceId, TDes& aServiceName ) const = 0;
    };




/**
*  Interface for CaUiEngine creation.
*
*  @lib 
*  @since 2.6
*/
class CCaUiEngFactory : public CBase
    {
    public:

        /**
        * Creates CallUIEngine.
        * CCaUiEngFactory instance is deleted even in leave case.
        * 
        * @return CCaUiEngApi implementation. NULL if not supported.
        *         Ownership transferred.
        */
        virtual CCaUiEngApi* CCaUiEngApiLD() = 0;

    };

/**
* Only exported function. This is in index 1 in the lookup table.
* Call this function to create CCaUiEngFactory instance.
* 
* IMPORT_C CCaUiEngFactory* CreateCaUiEngFactoryL();
*/ 




#endif      // CAUIENG_H 

// End of File
