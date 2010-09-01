/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
*		CPsuiVariationProxy class is proxy class for 
*		central repository variation
*		(Call waiting/distinguish between not provisioned and not activated).
*  
*
*/


#ifndef PSUIVARIATIONPROXY_H
#define PSUIVARIATIONPROXY_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CRepository;

// CONSTANTS
const TUint KCallWaitingDistiquishNotProvisioned = 0x00000001;

// CLASS DECLARATION
/**
*  CPsuiVariationProxy class is proxy class for central repository variation
*  @lib psui.lib
*  @since 1.0
*/
NONSHARABLE_CLASS ( CPSuiVariationProxy ): public CBase 
{
public: //constructor & destructor

	/* Symbian OS 2-phase Constructor. */
	static CPSuiVariationProxy* NewL();
	
	/* Destructor */
	~CPSuiVariationProxy();
	
public:
	/**
    * Check from member variable 'iFeatures' is the requested feature active.
	*
	* @param aFeature is uid for used central repository variable.
	* @return returns TBool type ETrue/EFalse is current feature enabled or not
	*/
	
	TBool FeatureEnabled( TUint aFeature ) const;

private:
	
	/**
    * Constructor
	*
	* Open CenRep, read data into 'iFeatures' and close CenRep
	* 
	*/
	void ConstructL();
	
private:
	/* Default constructor */
	CPSuiVariationProxy();
private:
	// Saves cen rep data. Readed in ConstructL()
	TInt iFeatures;
	
};

#endif
