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
*              : CSsUiUtilities class
*
*/


//  INCLUDE FILES
#include "cssuiutilities.h"
#include "cssnotedialog.h"
#include "csslistquerydialog.h"
#include "csslogger.h"
#include <eikdialg.h>
#include <coemain.h>
#include <bautils.h>
#include <data_caging_path_literals.hrh> // For resource dir definations
#include <StringLoader.h> 
#include <AknsUtils.h> 
#include <avkon.mbg>

// CONSTANTS
// Resource file drive
_LIT(KSsResourceFileDrive, "Z:");
// Resource file name
_LIT(KSsResourceFileName, "serviceselector.rsc");
// Bitmap path definations
#define KPluginBitmapPath KDC_APP_BITMAP_DIR
_LIT( KBitmapFile, "avkon2.mif" );

// ================= MEMBER FUNCTIONS =======================================


// -----------------------------------------------------------------------------
// CSsUiUtilities::CSsUiUtilities
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSsUiUtilities::CSsUiUtilities
        (
        // None
        )
    : iResourceFileOffset( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CSsUiUtilities::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSsUiUtilities::ConstructL()
    {
    /*
     Code scanner warning "performance: Using CEikonEnv::Static"
     We have to load resources or we cannot show any
     notes nor queries. Warning ignored. 
     */
    
    // A Service selector instance can be created also when CCoeEnv is not 
    // available (e.g. from server applications). In this case, the methods 
    // needing CCoeEnv/CEikonEnv will leave with KErrNotSupported.
    iCoeEnv = CEikonEnv::Static();
    
    if ( iCoeEnv )
        {
        LoadResourceL();
        }
    }

// -----------------------------------------------------------------------------
// CSsConnectionHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSsUiUtilities* CSsUiUtilities::NewL()
    {
    CSsUiUtilities* self = new ( ELeave ) CSsUiUtilities();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CSsUiUtilities::~CSsUiUtilities()
    {
    delete iDlg;
    iDlg = NULL;
    
    if ( iResourceFileOffset )
        {
        iCoeEnv->DeleteResourceFile( iResourceFileOffset );
        iResourceFileOffset = 0;
        }
    }

// -----------------------------------------------------------------------------
// CSsUiUtilities::ShowInformationNoteL
// Shows a information note.
// -----------------------------------------------------------------------------
//
void CSsUiUtilities::ShowInformationNoteL 
        ( 
        TInt aResource
        )
    {
    if ( !iCoeEnv )
        {
        // No CCoeEnv/CEikonEnv, leave
        User::Leave( KErrNotSupported );
        }
    
    CSsNoteDialog* dlg = new(ELeave) CSsNoteDialog( &iDlg );    
    iDlg = dlg;
    
    dlg->RunDialogLD( aResource );
    }

// -----------------------------------------------------------------------------
// CSsUiUtilities::ShowListQueryL
// Shows a list query.
// -----------------------------------------------------------------------------
//
TBool CSsUiUtilities::ShowListQueryL 
        (
        TInt& aIndex,
        TInt aResource,
        MDesCArray* aItemTextArray,
        CArrayPtr<CGulIcon>* aIcons
        )
    {
    if ( !iCoeEnv )
        {
        // No CCoeEnv/CEikonEnv, leave
        User::Leave( KErrNotSupported );
        }
    
    CSsListQueryDialog* dlg = new(ELeave) CSsListQueryDialog( &iDlg, aIndex );   
    iDlg = dlg;
    
    TInt ret = dlg->ExecuteListLD( aResource, aItemTextArray, aIcons );
    
    // Return true if query not canceled ( ret = 0, if canceled )
    // and the index is on the correct range.
    return ( 0 != ret && KErrNotFound < aIndex && 
             aIndex < aItemTextArray->MdcaCount() );
    }

// -----------------------------------------------------------------------------
// CSsUiUtilities::LoadDefaultIconL
// Loads default list box icon.
// -----------------------------------------------------------------------------
//
void CSsUiUtilities::LoadDefaultIconL
        ( 
        CArrayPtr<CGulIcon>& aIcons 
        )
    {
    if ( !iCoeEnv )
        {
        // No CCoeEnv/CEikonEnv, leave
        User::Leave( KErrNotSupported );
        }
    
    CFbsBitmap* brandedBitmap = NULL;
    CFbsBitmap* brandedBitmapMask = NULL;
    
    TFindFile finder( CEikonEnv::Static()->FsSession() );
    User::LeaveIfError( finder.FindByDir( KBitmapFile,
                                 KPluginBitmapPath ) );
    TFileName bitmapFileName = finder.File();

    //default VoIP icon
    AknsUtils::CreateIconL( AknsUtils::SkinInstance(),
                             KAknsIIDQgnPropNrtypVoip,
                             brandedBitmap,
                             brandedBitmapMask,
                             bitmapFileName,
                             EMbmAvkonQgn_prop_nrtyp_voip,
                             EMbmAvkonQgn_prop_nrtyp_voip_mask );
    
    aIcons.AppendL( CGulIcon::NewL( brandedBitmap, brandedBitmapMask ) );
    }

// -----------------------------------------------------------------------------
// CSsUiUtilities::LoadResourceL
// Loads the resources needed for queries and confirmation notes
// -----------------------------------------------------------------------------
//
void CSsUiUtilities::LoadResourceL()
    {
    CSSLOGSTRING("CSSelector::LoadResources");
    TFileName fileName;
    fileName += KSsResourceFileDrive;
    fileName += KDC_RESOURCE_FILES_DIR;
    fileName += KSsResourceFileName;
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );
    iResourceFileOffset = iCoeEnv->AddResourceFileL( fileName );
    }

// ================= OTHER EXPORTED FUNCTIONS ===============================

//  End of File 
