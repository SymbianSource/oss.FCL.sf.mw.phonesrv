/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CVmSettingsUiIconFileProvider class.
*
*/

#include "vmsettingsuiiconfileprovider.h"

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CVmSettingsUiIconFileProvider::CVmSettingsUiIconFileProvider
//
// -----------------------------------------------------------------------------
//
CVmSettingsUiIconFileProvider::CVmSettingsUiIconFileProvider()
    {
    }

// -----------------------------------------------------------------------------
// CVmSettingsUiIconFileProvider::~CVmSettingsUiIconFileProvider
//
// -----------------------------------------------------------------------------
//
CVmSettingsUiIconFileProvider::~CVmSettingsUiIconFileProvider()
    {
    iFile.Close();
    }

// -----------------------------------------------------------------------------
// CVmSettingsUiIconFileProvider::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVmSettingsUiIconFileProvider::ConstructL( RFile& aFile )
    {
    User::LeaveIfError( iFile.Duplicate( aFile ) );
    }

// ----------------------------------------------------------------------------
// CVmSettingsUiIconFileProvider::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVmSettingsUiIconFileProvider* CVmSettingsUiIconFileProvider::NewL(
    RFile& aFile )
    {
    CVmSettingsUiIconFileProvider* self =
        new ( ELeave ) CVmSettingsUiIconFileProvider();
    CleanupStack::PushL( self );
    self->ConstructL( aFile );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// From MAknIconFileProvider
//
// ---------------------------------------------------------------------------
//
void CVmSettingsUiIconFileProvider::RetrieveIconFileHandleL(
    RFile& aFile, const TIconFileType /*aType*/ )
    {
    // duplicate the file's handle
    User::LeaveIfError( aFile.Duplicate( iFile ) );
    }

// ---------------------------------------------------------------------------
// From MAknIconFileProvider
//
// ---------------------------------------------------------------------------
//
void CVmSettingsUiIconFileProvider::Finished()
    {
    delete this;
    }

//  End of File
