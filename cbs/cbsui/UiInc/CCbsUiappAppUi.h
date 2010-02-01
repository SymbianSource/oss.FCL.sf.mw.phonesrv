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
* Description: 
*   User Interface class.    
*
*
*/


#ifndef __CBSUIAPPAPPUI_H
#define __CBSUIAPPAPPUI_H

// INCLUDES
#include <aknViewAppUi.h> // CAknViewAppUi


// FORWARD DECLARATIONS
class CCbsUiTopicListMonitor;
class MCbs;

// CLASS DECLARATION

/**
* It is the application user interface. Creates all the views and
* handles their activation.
*/
class CCbsUiappAppUi : public CAknViewAppUi
    {

    public: // Constructors and destructor
               
        /**
        * C++ constructor.
        */
        CCbsUiappAppUi(MCbs& aServer);    
            
        /**
        * Destructor.
        */
        virtual ~CCbsUiappAppUi();

    public: // From CEikAppUi

        /**
        * Update context icon when parameter aType is KAknsMessageSkinChange 
        */
        void HandleResourceChangeL( TInt aType );

    private: // From CEikAppUi

        /**
        * Handles common commands.
        *
        * @param aCommand It is command to be processed.
        */
	    void HandleCommandL( TInt aCommand );

    private:

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * Checks the SIM status
        * @return TInt, value of sim state
        */
        TInt CheckSimStateL();

    private: // Data
    
        // Ref: It is the connection to the server.
        MCbs& iServer;

        // TopicListMonitor
        CCbsUiTopicListMonitor* iTopicListMonitor;

    };


#endif      // __CBSUIAPP_H

            
// End of File
