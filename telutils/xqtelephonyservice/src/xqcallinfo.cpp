/*!
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
* Description: Provides information about ongoing calls.
*
*/

#include "xqcallinfoimpl.h"
#include <QDebug>
#include <xqcallinfo.h>


XQCallInfo *XQCallInfo::create()
{
    qDebug()<<"XQCallInfo::create()";
    return new XQCallInfoImpl();
}

void XQCallInfo::sendCallInfoChangedSignal()
{
    qDebug()<<"XQCallInfo::sendCallInfoChangedSignal()";
    emit callInformationChanged ();
}

XQCallInfo::XQCallInfo()
{
}
