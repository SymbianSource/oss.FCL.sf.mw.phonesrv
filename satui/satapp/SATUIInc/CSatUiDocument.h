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
*    Defines CSatUiDocument class
*
*
*/


#ifndef CSATUIDOCUMENT_H
#define CSATUIDOCUMENT_H

// INCLUDES
#include <AknDoc.h>

// FORWARD DECLARATIONS
class CSatUiActionPerformer;
class CSatUiViewAppUi;

// CLASS DECLARATION

/**
*  SatUi Document class.
*/
class CSatUiDocument : public CAknDocument
    {
    public: // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CSatUiDocument(CEikApplication& aApp): CAknDocument(aApp) {};

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        virtual ~CSatUiDocument();

    private:

        /**
        * From CEikDocument Creates ViewAppUi object.
        * @return The pointer to the created object.
        */
        CEikAppUi* CreateAppUiL();

    private://Data

        //Own: Command receiver
        CSatUiActionPerformer *iActionPerformer;

        // application UI pointer
        CSatUiViewAppUi* iApplicationUi;
    };

#endif    // CSATUIDOCUMENT_H

//  End of File
