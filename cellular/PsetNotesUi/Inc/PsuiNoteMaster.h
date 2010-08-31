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
* Description:  
*   CPsuiNoteMaster class is responsible for showing notes. It has also capa-
*   bilities to show bsc list that is common for different features.
*   Pending supplementary service request can be cancelled by the user.
*
*/


#ifndef CPSUINOTEMASTER_H
#define CPSUINOTEMASTER_H

// INCLUDES
#include <aknnotewrappers.h>
#include <bldvariant.hrh>
#include "psuiconstants.h" 
#include "psuiinternalconstants.h" 
#include "psetcalldiverting.h" 
 
// FORWARD DECLARATIONS
class MPsetCallWaiting;
class MPsetCallBarring;
class MPsetCli;
class CAknWaitDialog;
class CAknGlobalNote;
class CAknPopupList;
class CAknSinglePopupMenuStyleListBox;
class CServiceRequest;
class CPsuiReqObserver;
class CPsuiQueryDialog;

// CLASS DECLARATION

/**
*  CPsuiNoteMaster class is responsible for showing notes. It has also capa-
*  bilities to show bsc list that is common for different features.
*  Pending supplementary service request can be cancelled by the user.
*  @lib psui.lib
*  @since 1.0
*/
class CPsuiNoteMaster : public CBase
    {
    public: // Constructors and destructor
                
        /**
        * Symbian OS two-phased constructor.
        * 
        * @return Returns CPsuiNoteMaster-pointer.
        */
        static CPsuiNoteMaster* NewL();
        
        /**
        * Destructor 
        */
        ~CPsuiNoteMaster();
    
    private:
        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();
    
    
    public: //new
        
        /**
        * Sets the connection to engine. 
        * This is needed to provide user the possibility to cancel
        * pending requests. All the pointers are set with same
        * command, if in the future the amount of parameters comes
        * too large, redesigning might be worth doing. 
        *
        * @param aWaitingEngine Reference to Phone Settings' object
        * @param aBarringEngine Reference to Phone Settings' object
        * @param aDivertEngine Reference to Phone Settings' object
        * @param aCliEngine Reference to Phone Settings' object
        */
        void SetEngineContacts( MPsetCallWaiting* aWaitingEngine,
            MPsetCallBarring* aBarringEngine, 
            MPsetCallDiverting* aDivertEngine,
            MPsetCli* aCliEngine);
        
        /**
        * Shows the requested note. 
        *
        * @param aType Type of note to be shown
        *    Error, confirmation, warning and waiting notes are supported.
        * @param aResourceID Content text's resource ID for the note
        */
        void ShowNoteL( TPsuiNoteTypes aType, TInt aResourceID );
        
        /**
        * Kills waiting note when the request is completed.
        */
        void KillStaticNoteL();
        
        /**    
        * Shows the basic service list assosiated with supplementary service
        * activation or status check.
        * 
        * @param aResourceHeading Heading for the list
        * @param aBsc List of basic service groups
        */
        void ShowBscListL( 
            TInt aResourceHeading, 
            TUint8 aBsc[KPSUIMaxBscNumber] );
        
        /* 
        * Same as above, except intended for CF basic service lists.
        *
        * @param aResourceHeading Heading for the list
        * @param aList List of basic service groups and their statuses 
        *              and settings
        * @param aCFType Type of call forwarding
        * @param aIndex Index of list item user has selected
        * @return user action ID, i.e. what kind of list to show next
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/ 
        TCFType ShowBscCFListL( 
            TInt aResourceHeading, 
            CMobilePhoneCFList& aList, 
            TCFType aCFType,
            TInt& aIndex );
        
        /**
        * Shows detailed info about the selected basic service group.
        *
        *   @param  aCFType Type of Call Forwarding, defines softkeys
        *   @param  aSetting List of basic service groups and their statuses
        *                    and settings
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/ 
        void ShowCfInfoL( 
            TCFType& aCFType,
            const RMobilePhone::TMobilePhoneCFInfoEntryV1& aSetting );
        
        /**
        * Shows request cancelled note.
        */
        void ShowCancelRequestNoteL();
        
        /*
        * Returns correct resource ID based on error code.
        * 
        *   @param aErrorCode Error code for which the resource ID is needed
        *   @return Resource ID for the string text in the error note
        */
        TInt SetErrorNoteResourceID ( const TInt aErrorCode );
                    
        /**
        * Called when service request
        * is cancelled by the user.
        */
        void RequestNoteCancelledL();
        
    protected:
    
    
    private: //new
        
        void AppendCFNumber( TDes& aString, const TDesC& aNumber);

        void CreateListBoxL( 
            TCFType aCFType, CAknSinglePopupMenuStyleListBox* aList );

        void KillRequestL();

        void SetTitleL( const TInt& aResourceHeading );        

        HBufC* SetBsGroupLC( TUint8 aBscCode );                       

        TBasicServiceGroups ChangeToGSM( const TInt& aBsc );
        
    private: // Member variables
        
        // General Service request
        CServiceRequest*        iServiceRequest;
        // Call Barrings engine
        MPsetCallBarring*       iBarringEngine; 
        // Reference to Call Waiting engine
        MPsetCallWaiting*       iWaitingEngine;
        // Reference to Call Diverts engine
        MPsetCallDiverting*     iDivertEngine;
        // Reference to Calling Identity engine
        MPsetCli*               iCliEngine;
        // There is an active request
        TBool                   iRequesting;
        // Basic Service Groups list
        CAknPopupList*          iPopupList;
        // Request note cancel observer
        CPsuiReqObserver*       iReqObserver;
        // Diverts additional information query
        CPsuiQueryDialog*        iQuery;

    };       
#endif    //CPSUINOTEMASTER_H
