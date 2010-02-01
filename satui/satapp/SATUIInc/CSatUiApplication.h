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
*    Defines CSatUiApplication class
*    
*
*/


#ifndef CSATUIAPPLICATION_H
#define CSATUIAPPLICATION_H

#include <aknapp.h>

// CONSTANTS
const TUid KUidSATUI = { 0x101f4ce0 }; // UID of app

// CLASS DECLARATION

/**
*  SatUi Application class.
*/
class CSatUiApplication : public CAknApplication
    {
    private: // Functions from base classes

        /**
        * From CAknApplication Returns the UId of the application.
        * @return The UId of the application.
        */
        TUid AppDllUid() const;

        /**
        * From CAknApplication Creates document object.
        * @return The pointer to the created object.
        */
        CApaDocument* CreateDocumentL();
    };

#endif    // CSATUIAPPLICATION_H

//  End of File
