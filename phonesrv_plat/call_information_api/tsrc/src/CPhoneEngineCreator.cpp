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
* Description:
*
*/
#include "CPhoneEngineCreator.h"

#include <TelephonyAudioRoutingManager.h>
#include <activeidle2domainpskeys.h>
#include <CPEPhoneModelIf.h>
#include "CPEMessageWaiter.h"


CPhoneEngineCreator* CPhoneEngineCreator::NewL()
    {
    CPhoneEngineCreator* self = new( ELeave )CPhoneEngineCreator();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
CPhoneEngineCreator::~CPhoneEngineCreator()
    {
    delete iStubAudioFactory;
    
    }
    
CPhoneEngineCreator::CPhoneEngineCreator() 
    {
    }
    
void CPhoneEngineCreator::ConstructL()
    {
    iStubAudioFactory = CStubAudioFactory::NewL();
    }
    
MPEPhoneModel* CPhoneEngineCreator::CreatePhoneEngineL(
    CPEMessageWaiter& aMsgWaiter,
    TBool aReturnImmedialtely )
    {
    CPEPhoneModelIF* pEif = 
        CPEPhoneModelIF::CreateL( aMsgWaiter, *iStubAudioFactory );
    
    if ( !aReturnImmedialtely )
        {
        aMsgWaiter.WaitForMsg(
                MEngineMonitor::EPEMessagePEConstructionReady,
                40 );
        }    
    
    // No error checking here because this will succeed only once when this is called. The RProperty::Set will
    // fail, if this call is not successfull at all.
    RProperty::Define( 
        KPSUidAiInformation,
        KActiveIdleState,
        RProperty::EInt );
                            
    User::LeaveIfError( RProperty::Set( KPSUidAiInformation, KActiveIdleState, EPSAiForeground ) ); 
    
    return pEif;
    }

CStubAudioFactory* CPhoneEngineCreator::AudioFactory() const
    {
    return iStubAudioFactory;
    }

