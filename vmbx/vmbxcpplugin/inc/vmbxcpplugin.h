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
#ifndef VMBXCPPLUGIN_H
#define VMBXCPPLUGIN_H

#include <qobject.h>
#include <cppluginplatinterface.h>
class CpItemDataHelper;

class VmbxCpPlugin : public QObject, public CpPluginPlatInterface
{
Q_OBJECT
    Q_INTERFACES(CpPluginPlatInterface)
public:
    /*!
     * constructor
     */
    VmbxCpPlugin();
    /*!
     * destructor
     */
    ~VmbxCpPlugin();
    /*!
     * from the Basic class CpPluginPlatInterface
     * see class CpPluginPlatInterface
     */
    int uid() const;

    /*!
     * 
     */
    CpSettingFormItemData *createSettingFormItemData( CpItemDataHelper &itemDataHelper ) const;
};
#endif // VMBXCPPLUGIN_H
