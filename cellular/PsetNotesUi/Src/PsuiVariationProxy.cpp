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
* Description: Implementation of CPSuiVariationProxy class.
*
*
*/


#include "PsuiVariationProxy.h" 
#include <centralrepository.h>
#include <settingsinternalcrkeys.h> 

CPSuiVariationProxy::CPSuiVariationProxy()
    {   
    }
    
CPSuiVariationProxy::~CPSuiVariationProxy()
    {       
    }
    
CPSuiVariationProxy* CPSuiVariationProxy::NewL()
    {
    CPSuiVariationProxy* self = new ( ELeave ) CPSuiVariationProxy();  
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );         
    return self;
    }
    
void CPSuiVariationProxy::ConstructL()
    {
    TInt setting = 0;
    CRepository* centralRepository = CRepository::NewLC( KCRUidPhoneSettings );
    centralRepository->Get( KPSetCallWaiting, setting );
    if( setting )
        {
        iFeatures |= KCallWaitingDistiquishNotProvisioned;  
        }
    CleanupStack::PopAndDestroy( centralRepository );
    }
    
TBool CPSuiVariationProxy::FeatureEnabled( TUint aFeature ) const
    {
    return aFeature&iFeatures;
    }
