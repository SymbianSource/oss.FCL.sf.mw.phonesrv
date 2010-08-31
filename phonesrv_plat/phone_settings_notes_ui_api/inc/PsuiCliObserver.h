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
* Description:  CPsuiCliObserver class is responsible for handling notes 
*                related to CLIR, CLIP, COLR, COLP and CNAP.
*
*/


#ifndef CPSUICLIOBSERVER_H
#define CPSUICLIOBSERVER_H

// INCLUDES            
#include <mpsetcliobserver.h> 
#include "psuiconstants.h" 

// FORWARD DECLARATION
class CPsuiResourceLoader;
class CPsuiContainer;

// CLASS DECLARATION

/**
*  CPsuiCliObserver class is responsible for handling notes related
*  to CLIR, CLIP, COLR, COLP and CNAP.
*  @lib psui.lib
*  @since 1.0
*/
class CPsuiCliObserver : public CBase, 
                         public MPsetCliObserver 
    {
    public: // Constructor

        /**
        * Symbian OS two-phased constructor.
        *
        * @param aContainer Reference to container
        * @return Returns CPsuiCliObserver pointer
        */
        IMPORT_C static CPsuiCliObserver* NewL( CPsuiContainer& aContainer );
        
    public: 

        /**
        * From MPsetCliObserver, handles calling identity notes.
        *
        * @param aType Type of call identity information
        */
        void CliInformationL( TPsuiCli aType );

        /**
        * From MPsetCliObserver, handles requesting notes.
        *
        * @param aOngoing If ETrue, there is an active request
        * @param aInterrupted If ETrue, means that notemaster should be 
        *                     immediately deleted.
        */
        void HandleCliRequestingL( TBool aOngoing, 
            TBool aInterrupted = EFalse );

        /**
        * From MPsetCliObserver, defines reference to requesting object.
        *
        * @param aEngine Reference to the Phone Settings
        */
        void SetEngineContact( MPsetCli* aEngine );

        /**
        * From MPsetCliObserver.
        * DEPRECATED.
        */
        void HandleCliStatusL( 
            TUint8[] /*aBsc[KPSUIMaxBscNumber]*/, 
            TPsuiCli /*aMode*/ ) {}

        /**
        * This method shows CNAP status.
        * @since 1.2
        *
        * @param aStatus Status of CNAP
        *
        */
        void HandleCnapStatusL( TInt aStatus );

        /**
        * Handles errors.
        * @since 2.0
        *
        * @param aError error code.
        *
        */
        void HandleCliErrorL( TInt aError );

        /**
        * Destructor
        * @since 2.0
        */
        ~CPsuiCliObserver();
        
    private: // new

        void SetContainer( CPsuiContainer& aContainer );

    private: // member variables

        //container object
        CPsuiContainer* iContainer;
    };


#endif //CPSUICLIOBSERVER_H


    
