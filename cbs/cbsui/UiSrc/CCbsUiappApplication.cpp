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
*    Implements the application class.
*
*
*/


// INCLUDES
#include <eikstart.h>
#include "CCbsUiappApplication.h"
#include "CCbsUiappDocument.h"
#include "CbsUiId.h"



// CONSTANTS
static const TUid KUidCbsUiappApp = { KUidCbsUiappDef };




// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CCbsUiappApplication::AppDllUid
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUid CCbsUiappApplication::AppDllUid() const
    {
    return KUidCbsUiappApp;
    }

// ---------------------------------------------------------
// CCbsUiappApplication::CreateDocumentL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
CApaDocument* CCbsUiappApplication::CreateDocumentL()
    {
    return CCbsUiappDocument::NewL(*this);
    }

// ================= OTHER EXPORTED FUNCTIONS ==============


LOCAL_C CApaApplication* NewApplication()
    {
    return new CCbsUiappApplication;
    }

GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }


//  End of File  
