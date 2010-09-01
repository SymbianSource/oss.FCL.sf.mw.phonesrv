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
* Description:  This file contains the header file of the
*              : CSsServiceHandlerVoip class.
*
*/



#ifndef CSSSERVICEHANDLERVOIP_H
#define CSSSERVICEHANDLERVOIP_H

//  INCLUDES
#include "cservicehandler.h"
#include <e32base.h>
#include <badesca.h>
#include <AknIconArray.h>
#include <gulicon.h>

// CONSTANTS
//None

// MACROS
//None

// DATA TYPES
//None.

// FUNCTION PROTOTYPES
//None

// FORWARD DECLARATIONS
class CSsConnectionHandler;
class CSsUiUtilities;
class CSsSettingsHandler;
class CSPSettingsVoIPUtils;
class CSsBsHandler;

// CLASS DECLARATION

/**
*  VoIP service handler class for service selector
*
*  @lib ServiceSelector.lib
*  @since Series60_5.0
*/
NONSHARABLE_CLASS( CSsServiceHandlerVoip )
        : public CServiceHandler
    {
    public:  // Constructors and destructor
      
        /**
        * Two-phased constructor.
        */
        static CSsServiceHandlerVoip* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CSsServiceHandlerVoip();

    public: // New functions


    protected:  // New functions
        
        /**
        * From MServiceHandler
        */     
        void ExecuteServiceSelectionL( 
            CConvergedServiceSelector::TSsResult& aResult, 
            CConvergedServiceSelector::TSsCallType aCallType,
            TUint aServiceId,
            TBool aSendKey,
            const TDesC& aString = KNullDesC );

    private:  // New functions
        
        /**
        * Gets calling service by known service id.
        * @param aResult        Selection results are returned by this param.
        * @param aServiceId     Current service id of the call.
        * @param aSendKey       Is sendkey used for call.
        * @param aString        Known number/address.
        */
        void GetServiceByServiceId(
            CConvergedServiceSelector::TSsResult& aResult, 
            TUint aServiceId,
            TBool aSendKey,
            const TDesC& aString ) const;
            
        /**
        * Gets calling service without service id.
        * Leaves on failure.
        *   -Leaves with KErrCancel if some user query canceled by
        *    the user
        * @param aResult        Selection results are returned by this param.
        */
        void GetCallingServiceL(
            CConvergedServiceSelector::TSsResult& aResult );
 
        /**
        * Fills results by parameters and shows list query for the user
        * if more that one possible service available.
        * Leaves on failure.
        *   -Leaves with KErrCancel if some user query canceled by
        *    the user.
        * @param aResult        Selection results are returned by this param.
        * @param aServices      Known service id's.
        * @param aRegistered    Services registered or not.
        */
        void SelectServiceL( 
            CConvergedServiceSelector::TSsResult& aResult, 
            RArray<TUint>& aServices,
            TBool aRegistered );
 
        /**
        * Returns reference to uiutilities class
        * @return UiUtils class.
        */
        CSsUiUtilities& UiUtilitiesL();
 
        /**
        * Returns reference to settings handler class
        * @return SettingsHandler class.
        */
        CSsSettingsHandler& SettingsHandlerL();
        
        /**
        * Returns reference to branding handler class
        * @return CSsBsHandler class.
        */
        CSsBsHandler& BrandingHandlerL();
  
        /**
        * Checks should voip call be changed as cs voice call.
        * @param aServices      Known service id.
        * @param aString    Known number/address.
        * @return ETrue if CS voice call should be made.
        */
        TBool ChangeTypeAsCsVoice( 
                TUint aServiceId,
                const TDesC& aString ) const;

        /**
        * Adds icon to the list.
        * @param aIcons On completion, contains new icon
        * @param aServiceId Used Service Id
        * @param aIconId On completion, contains icon's icon list id
        */
        void AddBrandIconL( CArrayPtr<CGulIcon>* aIcons, 
                            TUint aServiceId, 
                            TInt& aIconId );
 
        /**
        * Gets item text array and icons for the query list box
        * by service id list.
        * @param aServices Used Services.
        * @param aLbxItems On completion, contains list box item texts.
        * @param aIcons On completion, contains list icons.
        */
        void GetListBoxItemsAndIconsL(
                RArray<TUint>& aServices,
                CDesCArrayFlat* aLbxItems,
                CArrayPtr<CGulIcon>* aIcons );
        
        /**
        * Gets branded bitmap for service.
        * @param aServiceId Used Service Id.
        * @param aBrandedBitmap Branded bitmap.
        * @param aBrandedBitmapMask Bitmap mask.
        */
        void GetBrandIconL( TUint aServiceId,
                            CFbsBitmap*& aBrandedBitmap,
                            CFbsBitmap*& aBrandedBitmapMask );
        
    private:

        /**
        * C++ default constructor.
        */
        CSsServiceHandlerVoip();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        

    private:    // Data
        
        // Pointer to Connection Handler
        CSsConnectionHandler*   iConnectionHandler;
        
        // Pointer to UI Utils
        CSsUiUtilities*         iUiUtils;
        
        // Pointer to Settings handler
        CSsSettingsHandler*     iSettings;
        
        // Pointer to voip utils
        CSPSettingsVoIPUtils*   iVoipUtils;

        // Pointer to branding handler
        CSsBsHandler*           iBsHandler;
    };

#endif      // CSSSERVICEHANDLERVOIP_H
            
// End of File
