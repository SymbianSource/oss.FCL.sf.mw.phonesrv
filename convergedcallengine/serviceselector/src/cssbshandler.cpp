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
*              : CSsBsHandler class.
*
*/

// INCLUDE FILES
#include <mbsaccess.h>
#include <cbsfactory.h>
#include <mbselement.h>
#include <cbsbitmap.h>
#include <coemain.h>
#include <AknsItemID.h> 
#include <AknsSkinInstance.h> 
#include <AknsUtils.h> 

#include "cssiconfileprovider.h"
#include "cssbshandler.h"
#include "csslogger.h"

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES
// None

// CONSTANTS
// Branding server & data constants.
const TInt KSsBrandingIconSize  = 21;         // icon size

_LIT8( KSsBSApplicationId, "xsp" );                 // application id
_LIT8( KSsUiBSIconId, "default_brand_image" ); // icon id

// MACROS
// None

// LOCAL CONSTANTS AND MACROS
// None

// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ============================= LOCAL FUNCTIONS =============================

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSsBsHandler::CSsBsHandler
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CSsBsHandler::CSsBsHandler()
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CSsBsHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CSsBsHandler::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CSsBsHandler::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSsBsHandler* CSsBsHandler::NewL( )
    {
    CSsBsHandler* self = CSsBsHandler::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSsBsHandler::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSsBsHandler* CSsBsHandler::NewLC()
    {
    CSsBsHandler* self = new (ELeave) CSsBsHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CSsBsHandler::~CSsBsHandler()
    {
    }

// ---------------------------------------------------------------------------
// CSsBsHandler::GetBrandedIconL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CSsBsHandler::GetBrandedIconL(
    const TDesC8& aBrandingId,
    CFbsBitmap*& aBrandedBitmap,
    CFbsBitmap*& aBrandedBitmapMask ) const
    {
    CSSLOGSTRING( "CSSelector::CreateBrandedBitmapL: =>" );

    // Create branding server factory and access to branding data.
    CBSFactory* factory = CBSFactory::NewL(
        aBrandingId, KSsBSApplicationId );
    CleanupStack::PushL( factory );

    MBSAccess* access = factory->CreateAccessL(
        aBrandingId, ELangInternationalEnglish );
    CleanupClosePushL( *access );

    // Get structure which contains bitmap information.
    MBSElement* element = access->GetStructureL( KSsUiBSIconId );
    CleanupClosePushL( *element );
    const CBSBitmap& bitmap = element->BitmapDataL();

    // Open file which contains branded bitmaps.
    RFile file;
    CleanupClosePushL( file );
    access->GetFileL( bitmap.BitmapFileId(), file );

    // Try to load a skinned bitmap.
    if ( CCoeEnv::Static() )
        {
        TAknsItemID itemId;
        itemId.Set( bitmap.SkinIdMajor(), bitmap.SkinIdMinor() );
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TRAP_IGNORE( ( aBrandedBitmap =
            AknsUtils::CreateBitmapL( skin, itemId ) ) );
        }

    // If skinned bitmaps was not found, try to load bitmaps manually.
    if ( !aBrandedBitmap )
        {
        CSsIconFileProvider* fileProvider =
            CSsIconFileProvider::NewL( file );

        AknIconUtils::CreateIconL(
            aBrandedBitmap,
            aBrandedBitmapMask,
            *fileProvider,
            bitmap.BitmapId(),
            bitmap.BitmapMaskId() );
         }

    // Set sizes.
    const TInt iconSize( KSsBrandingIconSize );
    AknIconUtils::SetSize( aBrandedBitmap, TSize( iconSize, iconSize ) );
    AknIconUtils::SetSize( aBrandedBitmapMask, TSize( iconSize, iconSize ) );

    // Destroy some items.
    CleanupStack::PopAndDestroy( &file );
    CleanupStack::PopAndDestroy( element );
    CleanupStack::PopAndDestroy( access );
    CleanupStack::PopAndDestroy( factory );

    CSSLOGSTRING( "CSSelector::CreateBrandedBitmapL: <=" );
    }

// ========================== OTHER EXPORTED FUNCTIONS =======================

//  End of File
