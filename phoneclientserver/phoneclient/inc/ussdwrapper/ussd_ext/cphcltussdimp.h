/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for sending messages.
*
*/


#ifndef CPHCLTUSSDIMP_H
#define CPHCLTUSSDIMP_H

//  INCLUDES
#include <e32base.h>
#include "mphcltussdrequesthandler.h" 
#include "mphcltussdnotecontrollercallback.h" 
#include "rphcltussd.h"

#include "rphcltserver.h" 
#include "cphcltussdint.h" 


// FORWARD DECLARATIONS
class CPhCltUssdRequestHandler;
class CPhCltUssdNoteController;


// CLASS DECLARATION

/**
*  Interface for sending messages.
*
*  @lib phoneclient.lib
*  @since 1.0
*/
class CPhCltUssdImp:
    public CPhCltUssdInt,
    public MPhCltUssdRequestHandler, 
    public MPhCltUssdNoteControllerCallBack
    
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        *
        * @param aShowNotes If ETrue, notes are shown.
        * @return Pointer to created CPhCltUssdImp instance.
        */
        static CPhCltUssdImp* NewL( TBool aShowNotes = ETrue );

        /**
        * Destructor.
        */
        virtual ~CPhCltUssdImp();
        
     
     
     public: // New functions

        /**
        * Send Ussd string to network with default DCS.
        *
        * @param aMsgData Unicode string to be sent.
        * @return Error code.
        */
        virtual TInt SendUssd( const TDesC& aMsgData  );

        /**
        * Send Ussd buffer to network with default DCS.
        *
        * @param aMsgData 7-bit buffer to be sent.
        * @return Error code.
        */
        virtual TInt SendUssd( const TDesC8& aMsgData );

        /**
        * Send Ussd buffer to network with given DCS.
        *
        * @param aMsgData 7-bit buffer to be sent.
        * @param iSendDcs Specifies DCS and the send type.
        * @return Error code.
        */
        virtual TInt SendUssd( 
            const TDesC8& aMsgData, 
            const TUint8 iSendDcs );

        /**
        * Cancels outstanding Send request.
        */
        virtual void SendUssdCancel(); 
        
        /**
        * Starts editor. Creates own window server session.
        *
        * @return Error code.
        */
        TInt StartUssdEditor() const;
        
        /**
        * UI informs that it is been created
        * 
        * @return Error code.
        */
        virtual TInt AppStarting(); 
        
        /**
        * UI informs that it is terminating.
        *
        * @param aExitReason The reason why the application was terminated.
        * @return Error code.
        */
        virtual TInt AppTerminating( 
            TPhCltUssdAppExitReason aExitReason );
            
        /* UI informs that it is brougth to foreground.
        *
        * @return Should the editor be emptied?
        */
        virtual TBool AppToForeground();
        
        /**
        * UI informs that it is gone background.
        *
        * @return Error code.
        */
        virtual TInt AppToBackground();
        
        
        /**
        * Starts SAT session.
        *
        * @param aStatus Completed status.
        * @param aReceiveMessage Received message.
        * @param aShowNotesAndDcs Contains the information of note showing,
        *                         ( Phone Client -> Phone Server ) and
        *                         The used dcs in received message
        *                         ( Phone Server -> Phone Client ).
        */
        void StartSAT( 
            TRequestStatus& aStatus, 
            TDes& aReceiveMessage, 
            TPckg< TUint >& aShowNotesAndDcs );
        
        /**
        * Cancels SAT session.
        */ 
        void StopSAT();
        
        /**
        * Set data coding scheme
        * 
        * @param aDCS data coding scheme value
        */
        void SetDCS( TUint8 aDCS );
        
        
         
     protected:  // From base classes
        

        // Called when send completes.
        void HandleSendEventL( const TInt aError );

        // Hidden when the user cancel the wait dialog.
        void GlobalWaitNoteHidden();
        
        

    private: // New functions

        // Constructor. 
         CPhCltUssdImp();

        // Symbian 2nd phase constructor.
        void ConstructL( TBool aShowNotes );

        // Encodes the message.
        void EncodeL( const TDesC& aSrc, TDes8& aDes );
        
        // Finds CR character        
        void FindFirstCarriageReturnL( 
        const TDesC& aBuffer ,
        TUint& aSkipChars , 
        TUint& aStartBit );
        
    private:    // Data

        // The handler of the request.
        CPhCltUssdRequestHandler* iRequestHandler;

        // Wait scheduler used in this class. 
        CActiveSchedulerWait* iWait;

        // TInt to sending error store place.
        TInt iSendError;
        
        // Data coding scheme value
        TUint8 iDCS;
        
        // Data coding scheme value for ussd sending
        TUint8 iDCS2;
        
        //RPhCltUssd class instance
        RPhCltUssd  iCltUssd;
        
        //RPhCltServer class instance creates phonesession
        RPhCltServer iServer;
        
        //CPhCltUssdNoteController class instance
        CPhCltUssdNoteController* iNoteController;
        
    };

#endif // CPHCLTUSSDIMP_H

// End of File
