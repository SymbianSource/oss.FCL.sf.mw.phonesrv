/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of DialUtils.
*
*/



// INCLUDE FILES

#include    <featmgr.h>                 // Feature Manager.
#include    "DialUtilsImpl.h"           // Interface.

#include    <centralrepository.h>       // Central repository
#include    <settingsinternalcrkeys.h>  // Settings Central Repository keys.

// CONSTANTS

// Prefix change Off. See SharedDataKeys.h and SettingsInternalCRKeys.h
const TInt KDiUtPrefixChangeOff = 0;
// Prefix change On. See SharedDataKeys.h and SettingsInternalCRKeys.h
const TInt KDiUtPrefixChangeOn = 1;

// International prefix.
_LIT( KDiUtIntPrefix, "+" );
// International prefix.
_LIT( KDiUtJapanPrefix, "+81" );
// Zero prefix.
_LIT( KDiUtZeroPrefix, "0" );



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDialUtilsImpl::CDialUtilsImpl
// 
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CDialUtilsImpl::CDialUtilsImpl()
    {
    }


// -----------------------------------------------------------------------------
// CDialUtilsImpl::ConstructL
// 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDialUtilsImpl::ConstructL()
    {
    // Sets up TLS, must be done before FeatureManager is used.
    FeatureManager::InitializeLibL();

    iRepository = CRepository::NewL( KCRUidTelephonySettings );
    }


// -----------------------------------------------------------------------------
// CDialUtilsImpl::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDialUtilsImpl* CDialUtilsImpl::NewL()
    {
    CDialUtilsImpl* self = new( ELeave ) CDialUtilsImpl;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// -----------------------------------------------------------------------------
// CDialUtilsImpl::~CDialUtilsImpl
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CDialUtilsImpl::~CDialUtilsImpl()
    {
    delete iRepository;

    // Frees the TLS! Must be done after FeatureManager is used.
    FeatureManager::UnInitializeLib();
    }



// -----------------------------------------------------------------------------
// CDialUtilsImpl::CheckNumber
// 
// 
// -----------------------------------------------------------------------------
//
TInt CDialUtilsImpl::CheckNumber( HBufC& aPhoneNumber )
    {
    TInt err = KErrNone;
    TBool checkPrefix = 
        FeatureManager::FeatureSupported( KFeatureIdJapanPrefixChange );

    if ( checkPrefix )
        {
        TInt prefixMode = KDiUtPrefixChangeOff; // Default: Off.

        if ( !err )
            {
            err = iRepository->Get(
                KSettingsDialPrefixChangeMode,
                prefixMode );

            if ( !err )
                {
                // Shared data information is ok, so act according to it.
                switch ( prefixMode )
                    {
                    case KDiUtPrefixChangeOff:
                        {
                        // Nothing needs to be done, so let the phone number to
                        // be unchanged and just break.
                        break;
                        }

                    case KDiUtPrefixChangeOn:
                        {
                        TDiUtPrefixText prefixText;
                        
                        err = iRepository->Get( 
                            KSettingsDialPrefixText,
                            prefixText );

                        if ( !err )
                            {
                            err = CombineTexts( aPhoneNumber.Des(), prefixText );
                            }
                        break;
                        }

                    default:
                        // Should never happen, value is corrupted.
                        err = KErrCorrupt;
                        break;
                    }
                }
            }
        }
    return err;
    }


// -----------------------------------------------------------------------------
// CDialUtilsImpl::CombineTexts
// 
// 
// -----------------------------------------------------------------------------
//
TInt CDialUtilsImpl::CombineTexts(
    TPtr aPhoneNumber,
    const TDiUtPrefixText& aPrefixText ) const
    {
    TInt err = KErrNone;

    TInt beginning = 0; // The beginning for replacement.
    TInt replaceLength = 0; // The string length to be replaced.
    TInt location = KErrNotFound; // The location of searched string.

    // Try to find Japan prefix.
    location = aPhoneNumber.Find( KDiUtJapanPrefix );
    if ( location == beginning )
        {
        // The string was found, so make replacement. 
        // Safe because zeroPrefix < KDiUtJapanPrefix.
        replaceLength = KDiUtJapanPrefix().Length();
        aPhoneNumber.Replace( beginning, replaceLength, KDiUtZeroPrefix );
        }
    else
        {
        // Try to find international prefix.
        location = aPhoneNumber.Find( KDiUtIntPrefix );
        if ( location == beginning )
            {
            replaceLength = KDiUtIntPrefix().Length();
            // The string was found, so try to make replacement. 
            const TInt phoneNumLength = aPhoneNumber.Length();
            const TInt prefixLength = aPrefixText.Length();

            TInt stringLength = 
                ( phoneNumLength + prefixLength - replaceLength );

            if ( aPhoneNumber.MaxLength() >= stringLength )
                {
                // There is enough space to make this replace.
                aPhoneNumber.Replace( beginning, replaceLength, aPrefixText );
                }
            else
                {
                // There is no space to combine the strings, so inform it.
                err = KErrOverflow;
                }
            }
        }

    return err;
    }


// End of file
