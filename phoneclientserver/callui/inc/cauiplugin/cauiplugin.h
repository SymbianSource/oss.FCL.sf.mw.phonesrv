/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides ECOM interface implementation for CallUI.
*
*/


#ifndef CAUIPLUGIN_H
#define CAUIPLUGIN_H


//  INCLUDES
#include    <aiwserviceifmenu.h>
#include    <aiwcommon.h>               // AIW dialdata.
#include    <spdefinitions.h>  //TServiceId
#include    "cauidialresultobserver.h"
#include    "caui.hrh"

// FORWARD DECLARATIONS
class CCaUiEngApi;
class MAiwNotifyCallback;
class CAiwMenuPane;



// CLASS DECLARATION

/**
*  CaUiPlugin implementation.
*
*  @since 2.6
*/
class CCaUiPlugin 
:   public CAiwServiceIfMenu,
    public MCaUiDialResultObserver 
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * 
        * @return An instance of CaUiPlugin.
        */
        static CCaUiPlugin* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCaUiPlugin();


    public: // Functions from base classes

        /**
        * @see CAiwServiceIfMenu::InitialiseL.
        */
        void InitialiseL(
            MAiwNotifyCallback& aFrameworkCallback,
            const RCriteriaArray& aInterest );

        /**
        * @see CAiwServiceIfMenu::HandleServiceCmdL.
        */
        void HandleServiceCmdL(
            const TInt& aCmdId,
            const CAiwGenericParamList& aInParamList,
            CAiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MAiwNotifyCallback* aCallback = NULL );

        /**
        * @see CAiwServiceIfMenu::InitializeMenuPaneL.
        */
        void InitializeMenuPaneL(
        CAiwMenuPane& aMenuPane,
            TInt aIndex,
            TInt aCascadeId,
            const CAiwGenericParamList& aInParamList );

        /**
        * @see CAiwServiceIfMenu::HandleMenuCmdL.
        */
        void HandleMenuCmdL(
            TInt aMenuCmdId,
            const CAiwGenericParamList& aInParamList,
            CAiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MAiwNotifyCallback* aCallback = NULL );

        /**
        * @see MCaUiDialResultObserver::HandleDialResultL.
        */
        void HandleDialResultL( const TInt aStatus );


    private:

        /**
        * C++ default constructor.
        */
        CCaUiPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Adds a new call sub menu items. If the call type is internet call 
        * and there is only one service, the service's provider ID is used 
        * in menu item.
        * @param aMenuPane Menu pane to insert commands
        * @param aIndex The index of call sub menu
        * @param aType The type of item being added
        * @param aServiceId The service ID of VoIP call item, needed when 
        *        getting the service name
        */
        void AddAiwMenuItemL(
            CAiwMenuPane& aMenuPane,
            TInt aIndex,
            EMenuItemType aType,
            TServiceId aServiceId = 0 );

    private:    // Data

        // The library loader.
        RLibrary                        iLibrary;

        // CaUiEngine.
        CCaUiEngApi*                    iCaUiEngine;

        // The AIW callback instance.
        const MAiwNotifyCallback*       iAiwNotifyCallback;

        // AIW Input parameters.
        const CAiwGenericParamList*     iInParamList;

        // AIW Output parameters, result of call is returned here.
        CAiwGenericParamList*           iOutParamList;

        // The menu command given to HandleMenuCmdL.
        TInt                            iMenuCmdId;
    };

#endif  // CAUIPLUGIN_H

// End of file
