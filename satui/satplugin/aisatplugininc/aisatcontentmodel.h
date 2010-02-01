/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Content model for SAT Plug-in.
*
*/


#ifndef AISATCONTENTMODEL_H
#define AISATCONTENTMODEL_H


#include <aicontentmodel.h>
#include "aisatpluginuids.hrh"

// SAT Operator Info Plug-in XML implementation UID
const TInt KImplUidSatPlugin = 
    AI_UID_ECOM_IMPLEMENTATION_CONTENTPUBLISHER_SATPLUGIN;
const TUid KUidSatPlugin = { KImplUidSatPlugin };

/**
 * Content Ids.
 */
enum TSatContentIds
    {
    ESatContentText,
    ESatContentIcon
    };

enum TSatEventIds
    {
    ESatLaunchSatapp = 1
    };

const TAiContentItem KSatContent[] =
    {
    { ESatContentText, L"SatIdleModeText", "text/plain" },
    { ESatContentIcon, L"SatIdleModeIcon", "image/x-s60-bitmap" }
    };

/**
 * Resource Ids.
 */
enum TSatPluginResourceIds
    {
    ESatResourceEmptyIcon,
    ESatResourceEmptyCaption
    };

/**
 * Resources that the plug-in will publish.
 */
const TAiContentItem KSatResources[] =
    { 
    { ESatResourceEmptyCaption,       L"EmptyCaption", "text/plain" },
    { ESatResourceEmptyIcon,          L"EmptyIcon", "image/*" }    
    };

/**
 * Event Ids.
 */
const TAiContentItem KSatEvents[] =
    {
    { ESatLaunchSatapp, L"LaunchSatapp", "int" }
    };

#endif //AISATCONTENTMODEL_H

