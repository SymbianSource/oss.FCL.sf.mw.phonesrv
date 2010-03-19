/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CVmBSHandler class.
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

#include "vmsettingsuiiconfileprovider.h"
#include "vmbshandler.h"
#include "vmbxlogger.h"


// CONSTANTS used in VoIP branding, remove/update when branding is handled
// Branding server & data constants.
//const TInt KVmSettingsUiBrandingIconSize  = 21;         // icon size

//_LIT8( KVmSettingsUiBSApplicationId, "xsp" );                 // application id
//_LIT8( KVmSettingsUiBSIconId, "service_active_image_small" ); // icon id


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmBSHandler::CVmBSHandler
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CVmBSHandler::CVmBSHandler()
    {
    VMBLOGSTRING( "VMBX: CVmBSHandler::CVmBSHandler <=>" );
    }

// ---------------------------------------------------------------------------
// CVmBSHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmBSHandler::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmBSHandler::ConstructL <=>" );
    }

// ---------------------------------------------------------------------------
// CVmBSHandler::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmBSHandler* CVmBSHandler::NewL( )
    {
    VMBLOGSTRING( "VMBX: CVmBSHandler::NewL =>" );
    CVmBSHandler* self = CVmBSHandler::NewLC();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmBSHandler::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmBSHandler::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmBSHandler* CVmBSHandler::NewLC()
    {
    VMBLOGSTRING( "VMBX: CVmBSHandler::NewLC =>" );
    CVmBSHandler* self = new (ELeave) CVmBSHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    VMBLOGSTRING( "VMBX: CVmBSHandler::NewLC =>" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmBSHandler::NewLC
// Destructor
// ---------------------------------------------------------------------------
//
CVmBSHandler::~CVmBSHandler()
    {
    VMBLOGSTRING( "VMBX: CVmBSHandler::~CVmBSHandler <=>" );
    }

// Commented out because branding is not supported yet.
// ---------------------------------------------------------------------------
// CVmBSHandler::GetBrandedIconL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
// void CVmBSHandler::GetBrandedIconL(
//     const TDesC8& aBrandingId,
//     CFbsBitmap*& aBrandedBitmap,
//     CFbsBitmap*& aBrandedBitmapMask ) const
//     {
//     VMBLOGSTRING( "VMBX: CVmBSHandler::GetBrandedIconL =>" );
//     // Create branding server factory and access to branding data.
//     CBSFactory* factory = CBSFactory::NewL(
//         aBrandingId, KVmSettingsUiBSApplicationId );
//     CleanupStack::PushL( factory );
//
//     MBSAccess* access = factory->CreateAccessL(
//         aBrandingId, ELangInternationalEnglish );
//     CleanupClosePushL( *access );
//
//     // Get structure which contains bitmap information.
//     MBSElement* element = access->GetStructureL( KVmSettingsUiBSIconId );
//     CleanupClosePushL( *element );
//     const CBSBitmap& bitmap = element->BitmapDataL();
//
//     // Open file which contains branded bitmaps.
//     RFile file;
//     CleanupClosePushL( file );
//     access->GetFileL( bitmap.BitmapFileId(), file );
//
//     // Try to load a skinned bitmap.
//     if ( CCoeEnv::Static() )
//         {
//         TAknsItemID itemId;
//         itemId.Set( bitmap.SkinIdMajor(), bitmap.SkinIdMinor() );
//         MAknsSkinInstance* skin = AknsUtils::SkinInstance();
//         TRAP_IGNORE( ( aBrandedBitmap =
//             AknsUtils::CreateBitmapL( skin, itemId ) ) );
//         }
//
//     // If skinned bitmaps was not found, try to load bitmaps manually.
//     if ( !aBrandedBitmap )
//         {
//         CVmSettingsUiIconFileProvider* ifp =
//             CVmSettingsUiIconFileProvider::NewL( file );
//
//         AknIconUtils::CreateIconL(
//             aBrandedBitmap,
//             aBrandedBitmapMask,
//             *ifp,
//             bitmap.BitmapId(),
//             bitmap.BitmapMaskId() );
//          }
//
//     // Set sizes.
//     const TInt iconSize( KVmSettingsUiBrandingIconSize );
//     AknIconUtils::SetSize( aBrandedBitmap, TSize( iconSize, iconSize ) );
//     AknIconUtils::SetSize( aBrandedBitmapMask, TSize( iconSize, iconSize ) );
//
//     // Destroy some items.
//     CleanupStack::PopAndDestroy( &file );
//     CleanupStack::PopAndDestroy( element );
//     CleanupStack::PopAndDestroy( access );
//     CleanupStack::PopAndDestroy( factory );
//     VMBLOGSTRING( "VMBX: CVmBSHandler::GetBrandedIconL <=" );
//     }

//  End of File
