/*
* Copyright (c) 2002,2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Shows notes related to Call Barrings operations.
*
*/


#ifndef CPSUIBARRINGOBS_H
#define CPSUIBARRINGOBS_H

// INCLUDES
#include <mpsetbarringobs.h> 
#include "psuiconstants.h" 

// FORWARD DECLARATION
class MPsetCallBarring;
class CPsuiResourceLoader;
class CPsuiContainer;

// CLASS DECLARATION

/**
*  CPsuiBarringObs class implements MPsetBarringObserver.
*  @lib psui.lib
*  @since 1.0
*/
class CPsuiBarringObs : public CBase, 
                        public MPsetBarringObserver
    {
    public: // Constructor

        /**
        * Symbian OS two phased constructor. 
        *
        * @param aContainer Reference to container object
        * @return Returns CPsuiBarringObs-pointer
        */
        IMPORT_C static CPsuiBarringObs* NewL( CPsuiContainer& aContainer );
        
    public: // Functions from base classes

        /**
        * From MPsetBarringObserver, handles barring mode changed notes.
        *
        * @param aType Type of barring program
        * @param aStatus Status of the barring program
        * @param aPlural Several barrings affected or not
        */
        void HandleBarringModeChangedL( 
            TBarringProgram aType,
            TBarringStatus aStatus, 
            TBool aPlural );

        /**
        * From MPsetBarringObserver, handles barring mode status notes.
        *
        * @param aBsc List of basic service groups
        * @param aStatus Status of the barring program
        */
        void HandleBarringModeStatusL( TUint8 aBsc[KPSUIMaxBscNumber], 
            TBarringStatus aStatus );
        
        /**
        * From MPsetBarringObserver, handles errors for call barring requests.
        *
        * @param aReason Reason for error
        */
        void HandleBarringErrorL( TInt aReason );

        /**
        * From MPsetBarringObserver, handles requesting notes.
        *
        * @param aOngoing If ETrue, there is an active request
        * @param aInterrupted If ETrue, means that notemaster should be 
        *                     immediately deleted.
        */
        void HandleCBRequestingL( TBool aOngoing, 
            TBool aInterrupted = EFalse );

        /**
        * From MPsetBarringObserver, defines reference to requesting object.
        *
        * @param aBarringEngine Reference to the Phone Settings
        */
        void SetEngineContact( MPsetCallBarring* aBarringEngine );

        /**
        * From MPsetBarringObserver, handles notes relating to call barring
        * password  change requests.
        *
        * @param aSuccess If ETrue, password change request was successful
        */
        void CbPasswordChangedL( TBool aSuccess );
    
    private: // new

        void SetContainer( CPsuiContainer& aContainer );

    private: // member variables

        // Container object
        CPsuiContainer* iContainer;
    };
#endif //CPSUIBARRINGOBS_H
