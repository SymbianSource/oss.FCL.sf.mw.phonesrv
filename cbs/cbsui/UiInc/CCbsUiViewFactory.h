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
*    Works as a mediator to different views
*
*
*/


#ifndef CCBS_UI_VIEW_FACTORY_H
#define CCBS_UI_VIEW_FACTORY_H

//  INCLUDES
#include <e32base.h>
#include <CbsCommon.h>


// FORWARD DECLARATIONS
class MCbs;
class CCbsUiTopicListView;
class CCbsUiTopicView;
class CCbsUiMsgView;
class CCbsUiTopicListMonitor;
class CCbsUiSettingsView;
class CAknView;
class CCbsUiTopicListMonitor;

// CLASS DECLARATION


/**
*  For creating views in cbsui.
*  
*/
class CCbsUiViewFactory : public CBase
    {
    public:  // Constructors and destructor

        /**
        * C++ constructor.
        */
        CCbsUiViewFactory(
            MCbs& aServer, CCbsUiTopicListMonitor& aTopicMonitor );        
       
        /**
        * Destructor.
        */
        virtual ~CCbsUiViewFactory();

    public: // New functions
        
        /**
        * Returns pointer to topic list view and keeps a reference
        * to that object.
        * 
        * @return created view
        */
        CAknView* CreateTopicListViewLC();

        /**
        * Returns pointer to topioc view and keeps a reference
        * to that object.
        *
        * @return created view
        */
        CAknView* CreateTopicViewLC();

        /**
        * Returns pointer to msg view and keeps a reference
        * to that object.
        *
        * @return created view
        */
        CAknView* CreateMsgViewLC();

        /**
        * Returns pointer to settings view and keeps a reference
        * to that object.
        *
        * @return created view
        */
        CAknView* CreateSettingsViewLC();

    private:    // Data

        // Ref:
        CCbsUiTopicListMonitor& iTopicMonitor;

        // Ref:
        MCbs& iServer;
    };

#endif      // CCBS_UI_VIEW_FACTORY_H
            
// End of File
