/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 *
 */
 
#include <QDebug>
#include <cpitemdatahelper.h>
#include "vmbxcpplugin.h"
#include "vmbxcpgroup.h"
#include "loggerutil.h"

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
VmbxCpPlugin::VmbxCpPlugin()
{
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
VmbxCpPlugin::~VmbxCpPlugin()
{
}

// -----------------------------------------------------------------------------
// Method returns the plugin uid for control panel framework.
// -----------------------------------------------------------------------------
//
int VmbxCpPlugin::uid() const
{
    _DBGLOG2( "VmbxCpPlugin::uid=",0xEE1F4ECF )
    return 0xEE1F4ECF;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CpSettingFormItemData* VmbxCpPlugin::createSettingFormItemData( 
    CpItemDataHelper &itemDataHelper ) const
{
    _DBGLOG( "VmbxCpPlugin::createSettingFormItemData" )
    // TODO: remove the legacy arguments, these are not used!!!
    VmbxCpGroup *itemData = 
            new VmbxCpGroup(
            HbDataFormModelItem::GroupItem,
            QString("Voice Mailbox"),
            QString("vmbxcpplugin.cpcfg"),
            0,
            itemDataHelper );
    return itemData;
}
Q_EXPORT_PLUGIN2(VmbxCpPlugin, VmbxCpPlugin);
