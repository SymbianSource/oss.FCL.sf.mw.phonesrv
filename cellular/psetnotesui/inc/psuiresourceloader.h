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
* Description:  Loads resources from file. 
*
*/


#ifndef CPSUIRESOURCELOADER_H
#define CPSUIRESOURCELOADER_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class RConeResourceLoader;

// CLASS DECLARATION
/**
* A helper class for loading dll-specific resource files on demand. 
* Searches and loads the specified resource file to the CONE environment 
* (CCoeEnv) -object (accessible as iCoeEnv in CONE-related classes). 
* After calling NewL the user can access the resources directly via 
* the CONE environment. The resources in the file are accessible until 
* the CPsuiResourceLoader object is destroyed. The resource file must be 
* a well-formed UIKON resource file with NAME and RSS_SIGNATURE specified.
*
* @lib psui.lib
* @since 1.0
*/
class CPsuiResourceLoader : public CBase
    {
    public:  // Constructors & destructor

         /**
         * Symbian OS 1st phase constructor. Creates and returns a new
         * resource loader object. 
         * Loads resource from any drive. If this function is succesful
         * (doesn't leave) the resources in the file are usable through
         * CCoeEnv.
         * 
         * @return new object of this class holding a reference to the
         *                      resource file.
         *
         * @exception KErrNotFound  if the resource file is not found.
         * @exception KErrNoMemory  if out of memory.
         * @exception error         from CCoeEnv::AddResourceFileL.
         */
        IMPORT_C static CPsuiResourceLoader* NewL();

         /**
         * Same as NewL, but leaves the created object onto the cleanup stack.
         * @see NewL
         */
        IMPORT_C static CPsuiResourceLoader* NewLC();

         /* Destructor */
        ~CPsuiResourceLoader();

    protected:  // private constructor

        void ConstructL();

    private:  // member variables

        // CONE resource loader
        RConeResourceLoader* iResourceLoader;
    };

#endif  // CPSUIRESOURCELOADER_H

// End of File

