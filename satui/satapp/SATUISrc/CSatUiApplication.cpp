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
*    Defines CSatUiApplication class
*
*
*/


// INCLUDE FILES
#include    "CSatUiDocument.h"
#include    "CSatUiApplication.h"
#include    <eikpanic.h>
#include    <eikstart.h>
#include    "tflogger.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSatUiApplication::AppDllUid
// Returns application UID.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUid CSatUiApplication::AppDllUid() const
    {
    TFLOGSTRING( "CSatUiApplication::AppDllUid() called" )
    return KUidSATUI;
    }

// ---------------------------------------------------------
// CSatUiApplication::CreateDocumentL
// Creates the document object.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CApaDocument* CSatUiApplication::CreateDocumentL()
    {
    TFLOGSTRING( "CSatUiApplication::CreateDocumentL() called" )
    CSatUiDocument* document = new ( ELeave ) CSatUiDocument( *this );
    CleanupStack::PushL( document );
    document->ConstructL();
    CleanupStack::Pop( document );
    TFLOGSTRING( "CSatUiApplication::CreateDocumentL() exit" )
    return( document );
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

LOCAL_C CApaApplication* NewApplication()
    {
    TFLOGSTRING( "NewApplication() called" )
    return new CSatUiApplication;
    }

GLDEF_C TInt E32Main()
    {
    TFLOGSTRING( "E32Main() called" )
    return EikStart::RunApplication(NewApplication);
    }

//
// ---------------------------------------------------------
// Panic
// The needed panic method.
// Returns: void
//
GLREF_C void Panic(
    TEikPanic aPanic)
    {
    _LIT(KPanicText,"satui.app");
    User::Panic(KPanicText,aPanic);
    }

//  End of File
