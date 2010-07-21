/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains PhoneClient Dial Data.
*
*/



#ifndef CAUIDIALDATACONTAINER_H
#define CAUIDIALDATACONTAINER_H

//  INCLUDES
#include <e32base.h> 
#include <tphcltextphonedialdata.h> 


/**
*  Defines the class that contains PhoneClient Dial Data.
*
*  @lib CaUiEngine.lib
*  @since 2.6
*/
class CCaUiDialDataContainer 
:   public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * 
        * @return Created CCaUiDialDataContainer instance.
        */
        static CCaUiDialDataContainer* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCaUiDialDataContainer();


    public: // New functions
        
        /**
        * Returns reference to owned dial data.
        * 
        * @return Owned PhoneClient dial data.
        */
        TPhCltExtPhoneDialData& DialData();


    private:

        /**
        * C++ default constructor.
        */
        CCaUiDialDataContainer();


    private:    // Data

        // PhoneClient Dial data.
        TPhCltExtPhoneDialData  iDialData;
    };

#endif      // CAUIDIALDATACONTAINER_H  
            
// End of File
