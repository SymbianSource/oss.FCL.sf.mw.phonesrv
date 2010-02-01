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
* Description:  Implementation of the CCbsUiappDocument class.
*
*
*/



// INCLUDES
#include <RCbs.h>
#include "CCbsUiappDocument.h"
#include "CCbsUiappAppUi.h"
#include "RCbsUi.h"



// ================= MEMBER FUNCTIONS =======================
//

CCbsUiappDocument::CCbsUiappDocument( CEikApplication& aApp ) : 
    CEikDocument(aApp) 
    { 
    }

void CCbsUiappDocument::ConstructL()
    {
    iServer = new (ELeave) RCbsUi;
    // Create connection to the server.
    User::LeaveIfError( iServer->Connect() );
    }

CCbsUiappDocument* CCbsUiappDocument::NewL( CEikApplication& aApp )
    {
    CCbsUiappDocument* self = new( ELeave ) CCbsUiappDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();  // self
    return self;
    }

CCbsUiappDocument::~CCbsUiappDocument()
    {
    delete iServer;
    }

CEikAppUi* CCbsUiappDocument::CreateAppUiL()
    {
    return new (ELeave) CCbsUiappAppUi( *iServer );
    }


//  End of File  
