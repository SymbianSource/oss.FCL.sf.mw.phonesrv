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
*     Implementation for CCbsUiConfirmationNote.
*
*
*/


// INCLUDES
#include <AknGlobalConfirmationQuery.h>
#include <StringLoader.h>
#include <CbsUiApp.rsg>
#include "CCbsUiConfirmationNote.h"


// MODULE DATA STRUCTURES

/**
 * Global Confirmation query observer. Doesn't do anything.
 */
class CCbsUiConfirmationNote::CGlobalConfirmationQueryObserver : 
    public CActive
    {
    public: // constructor

        inline CGlobalConfirmationQueryObserver() : CActive(EPriorityIdle) 
            {
            }

        inline ~CGlobalConfirmationQueryObserver() 
            { 
            Cancel(); 
            }

        inline void ConstructL() 
            {
            CActiveScheduler::Add(this);
            SetActive();
            }

    public: // from CActive

    	void DoCancel() 
            { 
            }

	    void RunL() 
            {
            Cancel(); 
            }
    };

// ================= MEMBER FUNCTIONS =======================

CCbsUiConfirmationNote::CCbsUiConfirmationNote()
    {
    }

CCbsUiConfirmationNote::~CCbsUiConfirmationNote()
    {
    delete iConfQueryObserver;
    delete iGlobalConfirmationQuery;
    }


// ---------------------------------------------------------
// CCbsUiConfirmationNote::ShowGlobalConfirmationNoteL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCbsUiConfirmationNote::ShowGlobalConfirmationNoteL( CCoeEnv& aCoeEnv )
    {
    // create active object
    CGlobalConfirmationQueryObserver* observer = 
        new (ELeave) CGlobalConfirmationQueryObserver();

    if (iConfQueryObserver)
        {
        delete iConfQueryObserver;
        iConfQueryObserver = NULL;
        }
    iConfQueryObserver = observer;
    iConfQueryObserver->ConstructL();

    // show critical level note
    CAknGlobalConfirmationQuery* dlg = CAknGlobalConfirmationQuery::NewL();
    if (iGlobalConfirmationQuery)
        {
        delete iGlobalConfirmationQuery;
        iGlobalConfirmationQuery = NULL;
        }
    iGlobalConfirmationQuery = dlg;

    HBufC* noteText = StringLoader::LoadLC(
        R_TEXT_NOTE_NOT_ENOUGH_MEMORY, &aCoeEnv);
    
    iGlobalConfirmationQuery->ShowConfirmationQueryL(
        iConfQueryObserver->iStatus,
        *noteText, 
        R_AVKON_SOFTKEYS_OK_EMPTY);

    CleanupStack::PopAndDestroy(noteText);
    }


//  End of File  
