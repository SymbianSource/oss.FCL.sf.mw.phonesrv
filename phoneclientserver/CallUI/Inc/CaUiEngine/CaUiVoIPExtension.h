/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  VoIP profile handler
*
*/



#ifndef CAUIVOIPEXTENSION_H
#define CAUIVOIPEXTENSION_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <PhCltTypes.h>
#include <spdefinitions.h>  //RIdArray

/**
*  Mapping for list items.
*
*  @lib CaUiEngine.lib
*  @since 3.2
*/
NONSHARABLE_CLASS( TListMappingItem )
    {
    public: //Data
        // Index of the list item.
        TInt iIndex;
        // Call type ot the list item.
        TPhCltCallType iCallType;
    };
    
// DATATYPES
typedef CArrayFixFlat <TListMappingItem> CListMappingArray;

// CLASS DECLARATION
/**
*  VoIP profile handler.
*  
*
*  @lib CaUiEngine.lib
*  @since Series60 3.0
*/
NONSHARABLE_CLASS( CCaUiVoIPExtension )  : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CCaUiVoIPExtension* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCaUiVoIPExtension();


    public: // New functions
                
        /**
        * Check has user set VoIP profiles.
        * @return TBool, ETrue if profiles exists
        */        
        TBool IsVoIPProfilesL();

        /**
        * @see CCaUiEngApi::GetVoIPServiceIdsL
        */
        void GetVoIPServiceIdsL( RIdArray& aVoipServiceIds ) const;
        
        /**
        * @see CCaUiEngApi::GetVoipServiceNameL
        */
        void GetVoipServiceNameL( TServiceId aServiceId, TDes& aServiceName ) const;

    private:

        /**
        * C++ default constructor.
        */
        CCaUiVoIPExtension();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:  // Data
    };

#endif      // CAUIVOIPEXTENSION_H   
            
// End of File
