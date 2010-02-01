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
*   The application class.
*
*
*/


#ifndef __CBSUIAPPAPPLICATION_H
#define __CBSUIAPPAPPLICATION_H

//  INCLUDES
#include <aknapp.h>


// CLASS DECLARATION


/**
*  The Application class
*/
class CCbsUiappApplication : public CAknApplication
    {
    private: // Functions from base classes

        /**
        * From CApaApplication
        */
        CApaDocument* CreateDocumentL();

        /**
        * From CApaApplication.
        */
        TUid AppDllUid() const;
    };


#endif      // __CBSUIAPPAPPLICATION_H
            
// End of File
