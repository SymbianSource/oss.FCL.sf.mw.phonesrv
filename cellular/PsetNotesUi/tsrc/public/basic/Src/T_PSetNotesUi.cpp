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
* Description: Implementation of T_PSetNotesUi class.
*
*/


//  CLASS HEADER
#include "T_PSetNotesUi.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <PsuiContainer.h>

//  INTERNAL INCLUDES


// CONSTRUCTION
T_PSetNotesUi* T_PSetNotesUi::NewL()
    {
    T_PSetNotesUi* self = T_PSetNotesUi::NewLC();
    CleanupStack::Pop();

    return self;
    }

T_PSetNotesUi* T_PSetNotesUi::NewLC()
    {
    T_PSetNotesUi* self = new( ELeave ) T_PSetNotesUi();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
T_PSetNotesUi::~T_PSetNotesUi()
    {
    }

// Default constructor
T_PSetNotesUi::T_PSetNotesUi()
    {
    }

// Second phase construct
void T_PSetNotesUi::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void T_PSetNotesUi::SetupL(  )
    {
    iContainer = CPsuiContainer::NewL();
    }
    

void T_PSetNotesUi::Teardown(  )
    {
    delete iContainer;
    }
 
void T_PSetNotesUi::T_TestL()
    {
    }
    
void T_PSetNotesUi::T_Test2L()
    {
    CPsuiWaitingObs* waitingObs = iContainer->CreateCWObsL();
    delete waitingObs;
    }
    
void T_PSetNotesUi::T_Test3L()
    {
    CPsuiDivertObs* divertObs = iContainer->CreateCFObsL();
    delete divertObs;
    }    
    
void T_PSetNotesUi::T_Test4L()
    {
    CPsuiBarringObs* cbObs = iContainer->CreateCBObsL();
    delete cbObs;
    }

void T_PSetNotesUi::T_Test5L()
    {
    CPsuiCliObserver* cliObs = iContainer->CreateCliObsL();
    delete cliObs;
    }     
             
//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    T_PSetNotesUi,
    "Add test suite description here.",
    "MODULE" )

EUNIT_TEST(
    "CPsuiContainer::NewL",
    "",
    "CPsuiContainer::NewL",
    "FUNCTIONALITY",
    SetupL, T_TestL, Teardown)

EUNIT_TEST(
    "CreateCWObsL",
    "",
    "CreateCWObsL",
    "FUNCTIONALITY",
    SetupL, T_Test2L, Teardown)    

EUNIT_TEST(
    "CreateCFObsL",
    "",
    "CreateCFObsL",
    "FUNCTIONALITY",
    SetupL, T_Test3L, Teardown)  
    
EUNIT_TEST(
    "CreateCBObsL",
    "",
    "CreateCBObsL",
    "FUNCTIONALITY",
    SetupL, T_Test4L, Teardown)

EUNIT_TEST(
    "CreateCliObsL",
    "",
    "CreateCliObsL",
    "FUNCTIONALITY",
    SetupL, T_Test5L, Teardown)        
      
EUNIT_END_TEST_TABLE

//  END OF FILE
