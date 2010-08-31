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
* Description:  Shows notes and lists for Divert supplementary service
*
*/


#ifndef CPSUIDIVERTOBS_H
#define CPSUIDIVERTOBS_H

// INCLUDES
#include <mpsetdivertobs.h> 
#include "psuiconstants.h" 

// FORWARD DECLARATION
class MPsetCallDiverting;
class CPsuiResourceLoader;
class CPsuiContainer;
class CMobilePhoneCFList;

// CLASS DECLARATION

/**
*  CPsuiDivertObs class implements the MPsetDivertObserver observer.
*  @lib psui.lib
*  @since 1.0
*/
class CPsuiDivertObs :  public CBase, 
public MPsetDivertObserver
    {
    public: // Constructor
        
        /**
        * Symbian OS two-phased constructor.
        *
        * @param aContainer Reference to container
        * @return Returns pointer to this-object.
        */
        IMPORT_C static CPsuiDivertObs* NewL( CPsuiContainer& aContainer );
                
    public: // Functions from base classes
        
        /**
        * From MPsetDivertObserver, handles diverting changed.
        *
        * @param aSetting Call forwarding settings
        * @param aPlural Indicates that operation affected several basic 
        *                service groups
        */
        void HandleDivertingChangedL( 
            const TCallDivertSetting& aSetting, TBool aPlural );
        
        /**
        * From MPsetDivertObserver, handles diverting status.
        *
        * @param aSetting Call forwarding settings
        * @param aPlural - DEPRECATED
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/ 
        void HandleDivertingStatusL( 
            CMobilePhoneCFList& aSetting, 
            TBool aPlural );
        
        /**
        * From MPsetDivertObserver, handles diverting error.
        *
        * @param aReason Reason for error
        */
        void HandleDivertingErrorL( TInt aReason );
        
        /**
        * From MPsetDivertObserver, handles requesting notes.
        *
        * @param aTrue If ETrue, there is an active request
        * @param aInterrupted If ETrue, means that notemaster should be 
        *                     immediately deleted
        */
        void HandleCFRequestingL( TBool aTrue, TBool aInterrupted = EFalse );
        
        /**
        * From MPsetDivertObserver, defines reference to requesting object.
        *
        * @param aDivertEngine Reference to the Phone Settings
        */
        void SetEngineContact( MPsetCallDiverting* aDivertEngine );
        
    private: // new
        
        void SetContainer( CPsuiContainer& aContainer );
        
        
    private: // member variables
        
        // Container object
        CPsuiContainer* iContainer;
    };
#endif //CPSUIDIVERTOBS_H
