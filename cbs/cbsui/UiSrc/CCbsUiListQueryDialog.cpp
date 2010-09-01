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
*     Implementation of CCbsUiListQueryDialog
*
*
*/


// INCLUDES
#include <eikclb.h>	// CEikColumnListBox
#include <eikaufty.h>	// MEikAppUiFactory
#include <aknlists.h>	// CAknSingleGraphicStyleListBox
#include <gulicon.h>	// CGulIcon
#include <eikclbd.h> 	// CColumnListBoxData
#include <aknnavide.h> 	// CAknNavigationDecorator
#include <akntitle.h> 	// CAknTitlePane
#include <barsread.h> 	// TResourceReader
#include <AknIconArray.h>
#include <StringLoader.h>
#include "CCbsUiListQueryDialog.h"
#include "CbsUiPanic.h"
#include "MCbs.h"






// ================= MEMBER FUNCTIONS =======================

// C++ constructor can NOT contain any code, that
// might leave.
//
CCbsUiListQueryDialog::CCbsUiListQueryDialog(
    CArrayFix<TInt>& aIndexArray ) : 
    CAknListQueryDialog( &aIndexArray ),
    iIndexArray( aIndexArray )    
    {
    }

// ---------------------------------------------------------
// CCbsUiListQueryDialog::PostLayoutDynInitL
// 
// 
// ---------------------------------------------------------
//
void CCbsUiListQueryDialog::PostLayoutDynInitL()
    {
    CAknListQueryDialog::PostLayoutDynInitL();
    ListBox()->SetSelectionIndexesL( &iIndexArray );
    }

// ---------------------------------------------------------
// CCbsUiListQueryDialog::OkToExitL
// 
// 
// ---------------------------------------------------------
//
TBool CCbsUiListQueryDialog::OkToExitL( TInt aKeyCode )
    {    
    if ( aKeyCode == EEikBidOk )
        {        
        iIndexArray.Reset();
        }
                
    return CAknListQueryDialog::OkToExitL( aKeyCode );
    }



//  End of File  
