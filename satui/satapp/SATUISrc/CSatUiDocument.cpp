/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*    Defines CSatUiDocument class
*
*
*/


// INCLUDE
#include    <apgwgnam.h>
#include    <aknnotedialog.h>
#include    <aknstaticnotedialog.h>
#include    "CSatUiViewAppUi.h"
#include    "CSatUiActionPerformer.h"
#include    "CSatUiDocument.h"
#include    "tflogger.h"

// ================= MEMBER FUNCTIONS =======================

// Symbian OS default constructor can leave.
void CSatUiDocument::ConstructL()
    {
    TFLOGSTRING( "CSatUiDocument::ConstructL() called" )
    iActionPerformer = CSatUiActionPerformer::NewL();
    TFLOGSTRING( "CSatUiDocument::ConstructL() exit" )
    }

// Destructor.
CSatUiDocument::~CSatUiDocument()
    {
    TFLOGSTRING( "CSatUiDocument::~CSatUiDocument called" )
    delete iActionPerformer;
    TFLOGSTRING( "CSatUiDocument::~CSatUiDocument exit" )
    }

// ---------------------------------------------------------
// CSatUiDocument::CreateAppUiL
// Constructs App Ui object.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CEikAppUi* CSatUiDocument::CreateAppUiL()
    {
    TFLOGSTRING( "CSatUiDocument::CreateAppUiL called" )
    iApplicationUi = new (ELeave) CSatUiViewAppUi;
    // Set adapter into SetUpMenu view.
    iApplicationUi->SetAdapter( iActionPerformer->Adapter() ); 

    iActionPerformer->SetImplementer( iApplicationUi );
    TFLOGSTRING( "CSatUiDocument::CreateAppUiL exit" )
    return iApplicationUi;
    }

//  End of File
