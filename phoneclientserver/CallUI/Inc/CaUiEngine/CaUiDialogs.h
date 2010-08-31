/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __CAUIDIALOGS_H__
#define __CAUIDIALOGS_H__

// Includes

#include "phclttypes.h" 
#include "cauivoipextension.h" 

// Forward declarations

class CCaUiEngine;
class MCaUiReconnectQueryObserver;
class CPhCntMatcher;
class CAknGlobalListQuery;
class CCaUiGlnActiveObject;
class CCaUiReConQueryInformation;
class CAknGlobalNote;
class CCaUiNoteActiveObject;


// Contants

// The first item in list query (CCaUiQuery).
const TInt KCallUiVoiceCall = 0;

// The second item in list query (CCaUiQuery).
const TInt KCallUiVideoCall = 1;

// The third item in list query (CCaUiQuery).
const TInt KCallUiInternetCall = 2;

// Priority of the note
const TInt KNoVideoSupportNotePriority = 3500;

// Timeout for note launch.
const TInt KCaUiEngNoteTimeout = 3000000;  // 3s

NONSHARABLE_CLASS( CCaUiDialogs )
    : public CBase
    {
    
    public:
    
        /**
        * Two-phased constructor.
        * @return an instance of CCaUiDialogs.
        */
        static CCaUiDialogs* NewL( CCaUiEngine& aCaUiEngine );
    
        /**
        * Destructor.
        */
        virtual ~CCaUiDialogs();
    
        /**
        * Launch a query whether video call will be created with long 
        * press of Send key
        * 
        * @return The selection user made
        */
        TInt QueryVideoCallDefaultActionL();
        
        /**
        * Launch a list query asking what type of call the caller wants
        * to make.
        * 
        * @param aCallType The call type chosen by user in query.
        * @return ETrue if query was successful, EFalse otherwise.
        */
        TBool LaunchCreateListQueryL( TPhCltCallType& aCallType );
        
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
        * @param aAlias The alias corresponding the number. Can be empty TPtrC.
        */
        void LaunchReconConfQueryL( 
            MCaUiReconnectQueryObserver& aObserver,
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption,
            const TPtrC aAlias );
        
        /**
        * Cancel reconnect query.
        * 
        * @see CCaUiEngApi::CancelReconConfQuery().
        */
        void CancelReconConfQuery();
        
        /**
        * Cancel note and reconnect query.
        * 
        * @see CCaUiEngApi::CancelNoteAndReconConfQuery().
        */
        void CancelNoteAndReconConfQuery();
        
        /**
        * Launch note and reconnect confirmation query.
        * 
        * @see CCaUiEngApi::LaunchNoteAndReconConfQueryL().
        * @param aAlias The alias corresponding the number. Can be empty TPtrC.
        */
        void LaunchNoteAndReconConfQueryL( 
            MCaUiReconnectQueryObserver& aObserver,
            CPhCntMatcher* aPhCntMatcher,
            const TPtrC aPhoneNumber,
            const TBool aIncludeVideoCallOption,
            const TPtrC aNoteText,
            const TPtrC aAlias );
        
        /**
        * Check if reconfigure query is ongoing.
        * @since 3.2
        * @return TBool, ETrue if query is ongoing 
        *                EFalse if not ongoing.
        */
        TBool IsReconfQueryOngoing();
        
        /**
        * Set reconfigure query is ongoing.
        * @since 5.0
        */
        void SetReconfQueryOngoing( TBool aOngoing );
    
    private:
        
        /**
        * C++ default constructor.
        */
        CCaUiDialogs( CCaUiEngine& aCaUiEngine );
        
        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL();
        
        /**
        * Fill list query items
        * 
        * @since 5.0
        * @param aTextArray
        * @param aMappingList
        */
        void FillListQueryItemsL( 
            CDesCArray& aTextArray, 
            CListMappingArray& aMappingList );
        
        /**
        * Handle note expiration call back.
        * 
        * @param aAny This pointer.
        * @return KErrNone if successful, Symbian error code otherwise.
        */
        static TInt DoPerformNoteLaunchCallBackL( TAny* aAny );
        
    public: // data
        
        // tkuuva, create getter/setter
        //TBool                           iReconfQueryOngoing;
        
    private: //data
        
        // The reconnect global list query.
        CAknGlobalListQuery*            iReconListQuery;
        
        // The active object to handle reconnect query results.
        CCaUiGlnActiveObject*           iReconActObject;
        
        CCaUiEngine&                    iCaUiEngine;
        
        // Owned timer for note launch time-out.
        CPeriodic*                      iNoteTimer;
        
        // Storage for Reconnect query information.
        CCaUiReConQueryInformation*     iReConQueryInformation;
        
        // The qtn.tel.no.video.network note shown to the user.
        CAknInformationNote*            iInfoNote;
                
        // ReconfQuery ongoing flag
        TBool                           iReconfQueryOngoing;
        
    };

#endif // __CAUIDIALOGS_H__
