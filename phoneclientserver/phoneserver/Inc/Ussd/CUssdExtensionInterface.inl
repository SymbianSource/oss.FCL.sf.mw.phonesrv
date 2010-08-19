/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface's (abstract base class's) static factory method implementation.
*
*/

#include <e32def.h>
#include <ecom\ecom.h>

// ---------------------------------------------------------------------------
// CUssdExtensionInterface::NewL
// Interface's (abstract base class's) static factory method implementation.
// Asks ECOM plugin framework to instantiate appropriate concret plugin
// implementation.
// ---------------------------------------------------------------------------
//
inline CUssdExtensionInterface* CUssdExtensionInterface::NewL()
    {
    TEComResolverParams resolutionParameters;
    resolutionParameters.SetGenericMatch(ETrue);
    TAny* interface = REComSession::CreateImplementationL (
            KUssdExtensionInterfaceUid, _FOFF (CUssdExtensionInterface, iDtor_ID_Key), resolutionParameters, KRomOnlyResolverUid);

    return reinterpret_cast <CUssdExtensionInterface*> (interface);
    }

// ---------------------------------------------------------------------------
// CUssdExtensionInterface::NewL
// Interface's (abstract base class's) static factory method implementation.
// Asks ECOM plugin framework to instantiate appropriate concret plugin
// implementation.
// ---------------------------------------------------------------------------
//
inline CUssdExtensionInterface* CUssdExtensionInterface::NewL(const TDesC8& aOperationName)
    {
    TEComResolverParams resolverParams;
    resolverParams.SetDataType (aOperationName);
    resolverParams.SetWildcardMatch (ETrue);

    TAny* interface = REComSession::CreateImplementationL (KUssdExtensionInterfaceUid,
        _FOFF (CUssdExtensionInterface, iDtor_ID_Key), resolverParams, KRomOnlyResolverUid);

    return reinterpret_cast <CUssdExtensionInterface*> (interface);
    }

// ---------------------------------------------------------------------------
// CUssdExtensionInterface::~CUssdExtensionInterface
// Interface's (abstract base class's) destructor
// ---------------------------------------------------------------------------
//
inline CUssdExtensionInterface::~CUssdExtensionInterface()
    {
    REComSession::DestroyedImplementation (iDtor_ID_Key);
    }


