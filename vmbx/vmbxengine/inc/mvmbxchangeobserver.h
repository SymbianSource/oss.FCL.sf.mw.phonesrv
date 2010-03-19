/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Notifies when vmbx interrelated modules are changed
*  Interface   : Internal, MVmbxChangeObserver
*
*/


#ifndef M_VMBXCHANGEOBSERVER_H
#define M_VMBXCHANGEOBSERVER_H

// INCLUDE
#include "voicemailboxdefsinternal.h"

// FORWORD

// CLASS DECLARATION

/**
*  Notifier for vmbx number/address changes
*
*  @lib vmbxengine.lib
*
*  @since S60 v5.2
*
**/
NONSHARABLE_CLASS ( MVmbxChangeObserver )
    {

public:

   /**
    * Observer callback function which is called when
    *sat refresh
    *
    * @since S60 5.2
    **/
    virtual void SatRefreshL() = 0;

   /**
    * Observer callback function which is called when
    *CenRep Key value changed
    *
    * @param aId id related with the key of CenRep
    * @since S60 5.2
    **/
    virtual void CenRepChangedL( TVmbxCenRepKey aId ) = 0;

   /**
    * Observer callback function which is called when
    *sim store changed
    *
    * @since S60 5.2
    **/
    virtual void SimStoreChangedL() = 0;
    };

#endif // M_VMBXCHANGEOBSERVER_H
