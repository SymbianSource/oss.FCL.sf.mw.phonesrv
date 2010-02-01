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
*   Implementation of CCbsUiViewFactory.
*
*
*/


// INCLUDES
#include    "CCbsUiViewFactory.h"
#include    "CCbsUiSettingsView.h"
#include    "CCbsUiTopicListView.h"
#include    "CCbsUiTopicView.h"
#include    "CCbsUIMsgView.h"
#include    "CCbsUiTopicListMonitor.h"
#include    "CCbsUiSettingsMonitor.h"
#include    "CbsUiPanic.h"




// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiViewFactory::CCbsUiViewFactory(
    MCbs& aServer, CCbsUiTopicListMonitor& aTopicMonitor ) : 
    iTopicMonitor( aTopicMonitor ),
    iServer( aServer )
    {
    }
    
// Destructor
CCbsUiViewFactory::~CCbsUiViewFactory()
    {
    }

// ---------------------------------------------------------
// CCbsUiViewFactory::CreateTopicListViewLC
// 
// 
// ---------------------------------------------------------
//
CAknView* CCbsUiViewFactory::CreateTopicListViewLC()
    {
    CCbsUiTopicListView* view = CCbsUiTopicListView::NewL( iServer );
    CleanupStack::PushL( view );
    iTopicMonitor.AddObserverL( view );
    return view;
    }

// ---------------------------------------------------------
// CCbsUiViewFactory::CreateTopicViewLC
// 
// 
// ---------------------------------------------------------
//
CAknView* CCbsUiViewFactory::CreateTopicViewLC()
    {
    CCbsUiTopicView* view = CCbsUiTopicView::NewL( iServer );
    CleanupStack::PushL( view );
    iTopicMonitor.AddObserverL( view );
    return view;
    }

// ---------------------------------------------------------
// CCbsUiViewFactory::CreateMsgViewLC
// 
// 
// ---------------------------------------------------------
//
CAknView* CCbsUiViewFactory::CreateMsgViewLC()
    {
    CCbsUiMsgView* view = CCbsUiMsgView::NewL( iServer );
    CleanupStack::PushL( view );
    iTopicMonitor.AddObserverL( view );
    return view;
    }

// ---------------------------------------------------------
// CCbsUiViewFactory::CreateSettingsViewLC
// 
// 
// ---------------------------------------------------------
//
CAknView* CCbsUiViewFactory::CreateSettingsViewLC()
    {
    CCbsUiSettingsView* view = CCbsUiSettingsView::NewL( iServer );
    CleanupStack::PushL( view );
    view->UpdateContextPaneL();
    return view;
    }


//  End of File  
