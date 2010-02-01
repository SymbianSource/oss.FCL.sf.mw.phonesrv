/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of 
*              : CSsIconFileProvider class.
*
*/

#include "cssiconfileprovider.h"

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CSsIconFileProvider::CSsIconFileProvider
//
// -----------------------------------------------------------------------------
//
CSsIconFileProvider::CSsIconFileProvider()
    {
    }

// -----------------------------------------------------------------------------
// CSsIconFileProvider::~CSsIconFileProvider
//
// -----------------------------------------------------------------------------
//
CSsIconFileProvider::~CSsIconFileProvider()
    {
    }

// -----------------------------------------------------------------------------
// CSsIconFileProvider::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSsIconFileProvider::ConstructL( RFile& aFile )
    {
    User::LeaveIfError( iFile.Duplicate( aFile ) );
    }

// ----------------------------------------------------------------------------
// CSsIconFileProvider::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSsIconFileProvider* CSsIconFileProvider::NewL(
    RFile& aFile )
    {
    CSsIconFileProvider* self =
        new ( ELeave ) CSsIconFileProvider();
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
void CSsIconFileProvider::RetrieveIconFileHandleL(
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
void CSsIconFileProvider::Finished()
    {
    delete this;
    }

//  End of File
