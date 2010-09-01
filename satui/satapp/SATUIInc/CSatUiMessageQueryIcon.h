/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef CSATUIMESSAGEQUERYICON_H
#define CSATUIMESSAGEQUERYICON_H

//  INCLUDES
#include <gdi.h>

// FORWARD DECLARATIONS
class TSize;
class CFbsBitmap;

// CLASS DECLARATION

class CSatUiMessageQueryIcon :public CPicture
    {

    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
		* @param aSize Size of the picture in twips.
		* @param aBitmap Bitmap
        */
        CSatUiMessageQueryIcon( TSize aSize, CFbsBitmap& aBitmap );

        /**
        * Destructor.
        */
	    virtual ~CSatUiMessageQueryIcon();

	public: // From CPicture

		/**
        * Draw the picture.
        * @param aGc Graphic context.
        * @param aTopLeft Co-ordinates of left corner.
        * @param aClipRect Clipping rectangle.
        * @param aMap Device map.
        */
		virtual void Draw( CGraphicsContext& aGc,
						   const TPoint& aTopLeft,
						   const TRect& aClipRect,
						   MGraphicsDeviceMap* aMap ) const;
		/**
        * From CPicture.
        * A derived class must implement this function.
        */
		virtual void ExternalizeL( RWriteStream& aStream ) const;
		
        /**
        * Get the picture's size in twips.
		* @param aSize Size.
        */
		virtual void GetOriginalSizeInTwips( TSize& aSize ) const;

	protected:	// Data

		TSize iSizeInTwips;
		CFbsBitmap* iBitmap;
    };

#endif
            
