/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Implementaion of CCbsUiTopicListMonitor.
*
*
*/


// INCLUDES
#include "MCbs.h"
#include "CCbsUiTopicListMonitor.h"
#include "MCbsUiTopicMonitorObserver.h"
#include "CbsUiPanic.h"
#include "CbsUiConstants.h"



// CONSTANTS
const TInt KNumberOfObservers = 3;




// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiTopicListMonitor::CCbsUiTopicListMonitor( MCbs& aServer )
    : CActive(EPriorityStandard), iContinue(ETrue), iServer(aServer)
    {
    }

// Symbian OS default constructor can leave
void CCbsUiTopicListMonitor::ConstructL( )
    {    
    CActiveScheduler::Add(this);
    IssueRequest();

    iObservers = new (ELeave) TCbsUiMonitorObservers(KNumberOfObservers);
    }


// Two-phased constructor.
CCbsUiTopicListMonitor* CCbsUiTopicListMonitor::NewL( MCbs& aServer )
    {
    CCbsUiTopicListMonitor* self = 
        new (ELeave) CCbsUiTopicListMonitor(aServer);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

    
// Destructor
CCbsUiTopicListMonitor::~CCbsUiTopicListMonitor()
    {
    delete iObservers;
    Cancel();
    }

// ---------------------------------------------------------
// CCbsUiTopicListMonitor::IssueRequest()
// 
// 
// ---------------------------------------------------------
void CCbsUiTopicListMonitor::IssueRequest()
    {
    if ( iContinue )
        {
        SetActive();

        iServer.NotifyOnTopicListEvent( 
            iStatus, ECbsTopicAllEvents, iEvent, iTopicNumber );
        }
    }


// ---------------------------------------------------------
// CCbsUiTopicListMonitor::RunL
// 
// 
// ---------------------------------------------------------
void CCbsUiTopicListMonitor::RunL()  

    {
    if ( iObservers->Count() > 0 )
        {
        for ( TInt i(0); i < iObservers->Count(); i++ )
            {
            iObservers->At(i)->TopicListChangedL(iEvent, iTopicNumber);
            }
        }

    __ASSERT_DEBUG( 
        (iEvent & ECbsTopicAllEvents), CbsUiPanic(EErrInvalidEvent) );

    if ( iContinue )
        {
        IssueRequest();
        }
    }


// ---------------------------------------------------------
// CCbsUiTopicListMonitor::DoCancel
// 
// 
// ---------------------------------------------------------
void CCbsUiTopicListMonitor::DoCancel()
    {
    iContinue = EFalse;

    iServer.NotifyOnTopicListEventCancel();
    }

// ---------------------------------------------------------
// CCbsUiTopicListMonitor::AddObserverL
// 
// 
// ---------------------------------------------------------
void CCbsUiTopicListMonitor::AddObserverL(
    MCbsUiTopicMonitorObserver* aObserver )
    {
    __ASSERT_DEBUG( iObservers, CbsUiPanic(EErrNullPointer) );
    __ASSERT_DEBUG( aObserver, CbsUiPanic(EErrNullPointer) );

    iObservers->AppendL(aObserver);
    }


//  End of File  
