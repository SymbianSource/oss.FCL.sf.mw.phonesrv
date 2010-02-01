/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CSatUiIconHandler class.
*
*/


#include    <fbs.h>
#include    <rsatservice.h>
#include    <tsaticoninfo.h>

#include    "csatuiiconhandler.h"
#include    "tflogger.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// First part of Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSatUiIconHandler* CSatUiIconHandler::NewL()
    {
    TFLOGSTRING( "CSatUiIconHandler::NewL enter" )
    CSatUiIconHandler* self = new ( ELeave ) CSatUiIconHandler;
    TFLOGSTRING( "CSatUiIconHandler::NewL exit" )
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSatUiIconHandler::~CSatUiIconHandler()
    {
    TFLOGSTRING( "CSatUiIconHandler::~CSatUiIconHandler enter" )
    // Close RSatService interface
    iSatService.Close();
    TFLOGSTRING( "CSatUiIconHandler::~CSatUiIconHandler exit" )
    }

// ---------------------------------------------------------------------------
// Fetches first icon information, then the handle to the icon and at last
// does the duplication of the icon bitmap. Object is set to wait the
// completion of the request.
// ---------------------------------------------------------------------------
//
CFbsBitmap* CSatUiIconHandler::FetchIconL( const RSatSession& aSatSession,
                                           const TUint8 aIconId,
                                           const TInt aOptWidth,
                                           const TInt aOptHeight,
                                           TBool& aIconSupport )
    {
    TFLOGSTRING( "CSatUiIconHandler::FetchIconL enter" )
    TFLOGSTRING3( "CSatUiIconHandler::FetchIconL aOptWidth=%d aOptHeight=%d",
            aOptWidth, aOptHeight )
            
    iSatService.OpenL( aSatSession );
    CleanupClosePushL( iSatService );

    RIconEf iconEf;
    CleanupClosePushL( iconEf );
    // Order of superiority list
    RArray<TInt> selectedIcons;
    CleanupClosePushL( selectedIcons );
    CFbsBitmap* iconBitmap = NULL;
    
    // Getting the information of the icon
    TRAPD( err, iSatService.GetIconInfoL( aIconId, iconEf ); );
    
    if ( KErrNone == err )
        {
        TFLOGSTRING( "CSatUiIconHandler::FetchIconL no error" )
        // Sorting into order of superiority
        SortIconsL( iconEf, selectedIcons, aOptWidth, aOptHeight );
        
        // Reset iconCount by approved icons
        const TInt iconCount = selectedIcons.Count() <= iconEf.Count() ? 
                               selectedIcons.Count() :
                               iconEf.Count();
        TBool fetchSuccess( EFalse );
        
        TFLOGSTRING2( "CSatUiIconHandler::FetchIconL iconCount: %d", iconCount )
        // If there were icons to be fetched
        if ( 0 < iconCount )
            {
            // start going through approved icons list from the 
            // first and biggest icon
            TInt counter( 0 );
            for ( counter = 0; ( counter < iconCount ) && !fetchSuccess;
                counter++ )
                {
                TInt selectedIconCounter = selectedIcons.operator[]( counter );
                TFLOGSTRING2( "CSatUiIconHandler::\
                    FetchIconL selectedIconCounter: %d", selectedIconCounter )
                TRAPD( err, 
                       iconBitmap = iSatService.GetIconL( iconEf.operator[](
                           selectedIconCounter ) ) );
                if ( KErrNone == err  )
                    {
                    fetchSuccess = ETrue;
                    TFLOGSTRING( "CSatUiIconHandler::FetchIconL fetchSuccess!" )
                    }
                else
                    {
                    delete iconBitmap;
                    }
                }
            }
        }
        
        TFLOGSTRING2( "CSatUiIconHandler::FetchIconL icon info error=%d", err )
        if ( KErrNotSupported == err )
            {
            TFLOGSTRING( "CSatUiIconHandler::FetchIconL icons not supported" )
            aIconSupport = EFalse;
            }
    
    // selectedIcons, iconEf and iSatService not deleted but closed
    CleanupStack::PopAndDestroy( 3, &iSatService );

    TFLOGSTRING( "CSatUiIconHandler::FetchIconL exit" )
    // Icon was not found, icons are not supported or an error was received.
    return iconBitmap;
    }

// ---------------------------------------------------------------------------
// C++ default constructor cannot contain any code that might leave.
// ---------------------------------------------------------------------------
//
CSatUiIconHandler::CSatUiIconHandler()
    {
    TFLOGSTRING( "CSatUiIconHandler::CSatUiIconHandler enter - exit" )
    }

// ---------------------------------------------------------------------------
// Sorts icons into order of superiority
// ---------------------------------------------------------------------------
//
void CSatUiIconHandler::SortIconsL( const RIconEf& aIconEf,
    RArray<TInt>& aSelectedIcons, const TInt aOptWidth,
    const TInt aOptHeight ) const
    {
    TFLOGSTRING( "CSatUiIconHandler::SortIcons enter" )
    
    const TInt iconCount = aIconEf.Count();
    TFLOGSTRING2( "CSatUiIconHandler::SortIcons iconCount=%d", iconCount )
    TInt counter( 0 );
    // Search icon from available iconarray
    for ( counter = 0; counter < iconCount; counter++ )
        {
        TFLOGSTRING3( "CSatUiIconHandler::SortIcons Icon Width=%d Height=%d",
            aIconEf.operator[]( counter ).IconSize().iWidth,
            aIconEf.operator[]( counter ).IconSize().iHeight )

        const TInt selectedCount( aSelectedIcons.Count() );
        TBool replacement( EFalse );
        
        // Count the difference of the icon versus layout
        TInt newWidthDifference(
            aOptWidth - aIconEf.operator[]( counter ).IconSize().iWidth );
        TInt newHeightDifference(
            aOptHeight - aIconEf.operator[]( counter ).IconSize().iHeight );
        
        // Make the difference absolute value
        if ( newWidthDifference < 0 )
            {
            newWidthDifference = -newWidthDifference;
            }
        
        if ( newHeightDifference < 0 )
            {
            newHeightDifference = -newHeightDifference;
            }
        TInt listCounter( 0 );
        // Going through the list of icons of suitable size
        for ( listCounter = 0; ( listCounter < selectedCount ) &&
            !replacement; listCounter++ )
            {
            
            // Count the difference of the icon versus layout
            TInt oldWidthDifference( aOptWidth - aIconEf.operator[](
              aSelectedIcons.operator[]( listCounter ) ).IconSize().iWidth );
            TInt oldHeightDifference( aOptHeight - aIconEf.operator[](
              aSelectedIcons.operator[]( listCounter ) ).IconSize().iHeight );
               
            // Make the difference absolute value
            if ( oldWidthDifference < 0 )
                {
                oldWidthDifference = -oldWidthDifference;
                }
            
            if ( oldHeightDifference < 0 )
                {
                oldHeightDifference = -oldHeightDifference;
                }
            
            // New icon is better either by being closer to the layout or by
            // being same size and color instead of black and white
            if ( ( ( newWidthDifference < oldWidthDifference ) &&
                  ( newHeightDifference <= oldHeightDifference ) ) ||
                ( ( newWidthDifference <= oldWidthDifference ) &&
                  ( newHeightDifference < oldHeightDifference ) ) ||
                ( ( newWidthDifference == oldWidthDifference ) &&
                  ( newHeightDifference == oldHeightDifference ) &&
                 ( TSatIconInfo::EBasic == aIconEf.operator[]( aSelectedIcons.
                    operator[]( listCounter ) ).CodingScheme() ) &&
                 ( TSatIconInfo::EColor ==  aIconEf.operator[]( counter ).
                    CodingScheme() ) ) )
                {
                TFLOGSTRING2( "CSatUiIconHandler::SortIcons \
                             newWidthDifference: %d", newWidthDifference ) 
                TFLOGSTRING2( "CSatUiIconHandler::SortIcons \
                             oldHeightDifference: %d", oldHeightDifference )    
                // Insert new icon to correct place
                aSelectedIcons.InsertL( counter, listCounter );
                replacement = ETrue;
                }
            }
            
        TFLOGSTRING2( "CSatUiIconHandler::SortIcons listCounter: %d", \
            listCounter )    
        // Either first item or was not better than any in the list
        // New icon is added to the end of the array
        if ( !replacement )
            {
            aSelectedIcons.AppendL( counter );
            }
        }
        
    TFLOGSTRING2( "CSatUiIconHandler::SortIcons counter: %d", \
            counter )
    TFLOGSTRING( "CSatUiIconHandler::SortIcons exit" )
    }

