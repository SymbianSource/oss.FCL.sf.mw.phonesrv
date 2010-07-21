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
*   CPsuiContainer class is responsible for loading/unloading
*    the resources to be used by this dll. It is also
*    the correct way to construct observers, since
*    it gives the pointer to the resources in the
*    construction process.
*
*/


#ifndef CPSUICONTAINER_H
#define CPSUICONTAINER_H

// INCLUDES
#include <psuiwaitingobs.h> 
#include <psuidivertobs.h> 
#include <psuibarringobs.h> 
#include <psuicliobserver.h> 

// FORWARD DECLARATIONS
class CPsuiResourceLoader;

// CLASS DECLARATION

/**
*  CPsuiContainer class is responsible for loading/unloading the resources to
*  be used by this dll. It is also the correct way to construct observers, 
*  ince it gives the pointer to the resources in the construction process.
*  @lib psui.lib
*  @since 1.0
*/
class CPsuiContainer : public CBase
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor.
        *
        * @return Returns CPsuiContainer-pointer.
        */
        IMPORT_C static CPsuiContainer* NewL();
        
        /**
        * Destructor 
        */
        ~CPsuiContainer();

    public:

        /**
        * Creates Call Waiting observer.
        * 
        * @return Returns CreateCWObsL pointer.
        */
        IMPORT_C CPsuiWaitingObs* CreateCWObsL();

        /**
        * Creates Call Forwarding observer.
        * 
        * @return Returns CreateCFObsL pointer.
        */
        IMPORT_C CPsuiDivertObs* CreateCFObsL();

        /**
        * Creates Call Barring observer.
        * 
        * @return Returns CreateCBObsL pointer.
        */
        IMPORT_C CPsuiBarringObs* CreateCBObsL();

        /**
        * Creates Calling Identity observer.
        * 
        * @return Returns CreateCliObsL pointer.
        */
        IMPORT_C CPsuiCliObserver* CreateCliObsL();


    private: // private constructor

        void ConstructL();

    private: // member variables

        //reference to class' resource loader
        CPsuiResourceLoader* iAppEngineResourceLoader;
        
    };


#endif //CPSUICONTAINER_H
