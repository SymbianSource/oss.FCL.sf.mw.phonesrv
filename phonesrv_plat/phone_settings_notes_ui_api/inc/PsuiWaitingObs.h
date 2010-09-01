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
* Description:  
*   CPsuiWaitingObs class implements the virtual methods of 
*   MPsetCallWaitingObserver. Commands are redirected to CPsuiNoteMaster,
*   which is responsible for actually showing the notes.
*
*/


#ifndef CPSUIWAITINGOBS_H
#define CPSUIWAITINGOBS_H

// INCLUDES
#include <MPsetCallWaitingObs.h>        
#include "PsuiConstants.h"              

// FORWARD DECLARATION
class CPsuiResourceLoader;
class CPsuiContainer;
class PSetCallWaiting;
class AknStaticNoteDialog;

// CLASS DECLARATION

/**
*  CPsuiWaitingObs class implements the virtual methods of 
*  MPsetCallWaitingObserver. Commands are redirected to CPsuiNoteMaster,
*  which is responsible for actually showing the notes.
*  @lib psui.lib
*  @since 1.0
*/
class CPsuiWaitingObs : public CBase, 
                        public MPsetCallWaitingObserver
    {
    public: // Constructor

        /**
        * Symbian OS two-phased constructor.
        *
        * @param aContainer Container object
        * @return CPsuiWaitingObs pointer
        */
        IMPORT_C static CPsuiWaitingObs* NewL( CPsuiContainer& aContainer );

    public: // Functions from base classes

        /**
        * From MPsetCallWaitingObserver, handles status notes
        *
        * @param aStatus Status of call waiting
        * @param aBsc List of basic service groups
        */
        void HandleCallWaitingGetStatusL( 
            const MPsetCallWaiting::TGetCallWaitingStatus aStatus, 
            TUint8 aBsc[KPSUIMaxBscNumber] );
        
        /**
        * From MPsetCallWaitingObserver, handles call waiting change notes
        *
        * @param aSetting Call waiting request type (activation/cancellation)
        * @param aResult DEPRECATED
        */
        void HandleCallWaitingChangedL( 
            const MPsetCallWaiting::TSetCallWaiting aSetting, 
            const TInt aResult );
    
        /**
        * From MPsetCallWaitingObserver, handles requesting notes.
        *
        * @param  aOngoing If ETrue, there is an active request
        * @param aInterrupted If ETrue, means that notemaster should be 
        *                     immediately deleted.
        */
        void HandleCWRequestingL( 
            TBool aOngoing, 
            TBool aInterrupted = EFalse );

        /**
        * From MPsetCallWaitingObserver, handles call waiting errors.
        *
        * @param aReason Reason for error
        */
        void HandleCWErrorL( TInt aReason );
        
        /**
        * From MPsetCallWaitingObserver, defines reference to requesting object.
        *
        * @param aEngineContact Reference to the Phone Settings
        */
        void SetEngineContact( MPsetCallWaiting& aEngineContact );

    private: //private constructor
        
        void ConstructL( CPsuiContainer& aContainer );

    private: // member variables

        // Container object
        CPsuiContainer* iContainer;
        TBool iCallWaitingDistinguishEnabled;
    };
#endif // CPSUIWAITINGOBS_H
