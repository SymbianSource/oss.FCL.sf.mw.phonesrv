/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Singleton class for controlling notes. 
*
*/


#ifndef PSUINOTECONTROLLER_H
#define PSUINOTECONTROLLER_H

// INCLUDES
#include <coemain.h>
#include "psuinotemaster.h" 

// CLASS DECLARATION

class CPsuiNoteController : public CCoeStatic
    {
    public:  

        /**    
         * Returns an instance of this class. When called for the first
         * time, a new instance is created and returned.  After that,
         * calling InstanceL returns the same instance that was created
         * earlier.
         *   
         * @return A pointer to a CPsuiNoteController object    
         */    
        static CPsuiNoteController* InstanceL();   
        
        /**
        * Destructor.
        */
        virtual ~CPsuiNoteController();

        /**
        * Shows the requested note. 
        *
        * @param aType Type of note to be shown
        *    Error, confirmation, warning and waiting notes are supported.
        * @param aResourceID Content text's resource ID for the note
        */
        void ShowNoteL( TPsuiNoteTypes aType, TInt aResourceID );

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

        /*
        * Returns correct resource ID based on error code.
        * 
        *   @param aErrorCode Error code for which the resource ID is needed
        *   @return Resource ID for the string text in the error note
        */
        TInt SetErrorNoteResourceID ( const TInt aErrorCode );
        
        /**
        * Removes all notes that are being displayed by us.
        */
        void RemoveNotesL();

        /**
        * Kills waiting note when the request is completed.
        */
        void KillStaticNoteL();

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


    private:

        /**    
         * Default constructor is private because we are using the
         * singleton design pattern.
         */    
        CPsuiNoteController();  
        
        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();
                        
        
    private:
    
        // Note Master does the actual showing of notes and lists
        CPsuiNoteMaster* iNote;
    };
    
#endif // PSUINOTECONTROLLER_H

// End of File
