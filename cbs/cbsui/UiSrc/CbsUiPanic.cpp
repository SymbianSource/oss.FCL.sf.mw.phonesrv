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
*   Implementation of CbsUiPanic
*
*
*/


// INCLUDES
#include <e32base.h>
#include "CbsUiPanic.h"


// CONSTANTS
_LIT( KCbsUiName, "CbsUiApp" );



// ================= OTHER EXPORTED FUNCTIONS ==============

//
// ---------------------------------------------------------
// CbsUiPanic
// 
// 
GLDEF_C void CbsUiPanic( TCbsUiPanic aPanic )
    {
    User::Panic( KCbsUiName, aPanic );
    }


//  End of File  
