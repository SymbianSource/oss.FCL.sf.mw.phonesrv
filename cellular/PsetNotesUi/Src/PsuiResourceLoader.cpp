/*
* Copyright (c) 2002-2004,2007 Nokia Corporation and/or its subsidiary(-ies). 
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


// INCLUDE FILES
#include <coemain.h>
#include <ConeResLoader.h>

#include "PsuiResourceLoader.h"

// CONSTANTS
_LIT( KPsuiResourceFileNameAndPath, "Z:\\resource\\Psui.rsc" );

// MEMBER FUNCTIONS 
//-----------------------------------------------------------------------------
// Symbian OS two-phased constructor.
//-----------------------------------------------------------------------------
EXPORT_C CPsuiResourceLoader* CPsuiResourceLoader::NewL()
    {
    CPsuiResourceLoader* self = CPsuiResourceLoader::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

//-----------------------------------------------------------------------------
// Symbian OS two-phased constructor.
//-----------------------------------------------------------------------------
EXPORT_C CPsuiResourceLoader* CPsuiResourceLoader::NewLC()
    {
    CPsuiResourceLoader* self = new( ELeave ) CPsuiResourceLoader;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

//-----------------------------------------------------------------------------
// Destructor.
//-----------------------------------------------------------------------------
CPsuiResourceLoader::~CPsuiResourceLoader()
    {
    if ( iResourceLoader )
        {
        iResourceLoader->Close();
        }
    delete iResourceLoader;
    iResourceLoader = NULL;
    }

// ----------------------------------------------------------------------------
// 
// Creates the resource loader and connects it with environment.
// 
// ----------------------------------------------------------------------------
//
void CPsuiResourceLoader::ConstructL()
    {
    iResourceLoader = new ( ELeave ) RConeResourceLoader( *CCoeEnv::Static() ); 
    TFileName fileName ( KPsuiResourceFileNameAndPath ); 
    iResourceLoader->OpenL( fileName );
    }
//  End of File  

