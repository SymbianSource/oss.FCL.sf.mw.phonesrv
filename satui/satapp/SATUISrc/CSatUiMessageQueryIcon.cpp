/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*     CPicture derived class. For DisplayText to show icon and text
*     
*
*/


// INCLUDE FILES
#include <fbs.h>

#include "CSatUiMessageQueryIcon.h"
#include    "tflogger.h"


// ---------------------------------------------------------
// CSatUiMessageQueryIcon::CSatUiMessageQueryIcon
// Constructor
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSatUiMessageQueryIcon::CSatUiMessageQueryIcon( TSize aSize, 
    CFbsBitmap& aBitmap ): 
    iSizeInTwips( aSize ), 
    iBitmap( &aBitmap )
    {
    }
    
// Destructor
CSatUiMessageQueryIcon::~CSatUiMessageQueryIcon()
    {
    delete iBitmap;
    iBitmap = NULL;
    }

// ---------------------------------------------------------
// CSatUiMessageQueryIcon::Draw
// Draw the picture.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiMessageQueryIcon::Draw( CGraphicsContext& aGc,
    const TPoint& aTopLeft, const TRect& aClipRect, 
    MGraphicsDeviceMap* aMap ) const
	{
    TFLOGSTRING( "CSatUiMessageQueryIcon::Draw called" )
	TRect bitmapRect=aMap->TwipsToPixels( TRect(TPoint(), iSizeInTwips ) );
	bitmapRect.Move( aTopLeft );
	aGc.Reset();
	aGc.SetClippingRect( aClipRect );
	aGc.DrawBitmap( bitmapRect, iBitmap );    
	TFLOGSTRING( "CSatUiMessageQueryIcon::Draw exit" )    
	}

// ---------------------------------------------------------
// CSatUiMessageQueryIcon::ExternalizeL
// From CPicture
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiMessageQueryIcon::ExternalizeL( RWriteStream& /*aStream*/ ) const
	{
	}

// ---------------------------------------------------------
// CSatUiMessageQueryIcon::GetOriginalSizeInTwips
// Get the picture's size in twips..
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiMessageQueryIcon::GetOriginalSizeInTwips( TSize& aSize ) const
	{
	TFLOGSTRING( "CSatUiMessageQueryIcon::GetOriginalSizeInTwips called" )
	aSize = iSizeInTwips;
	TFLOGSTRING( "CSatUiMessageQueryIcon::GetOriginalSizeInTwips exit" )
	}

