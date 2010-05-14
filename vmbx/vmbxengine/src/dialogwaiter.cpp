/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: dialog waiter
*
*
*/

#include "dialogwaiter.h"

HbAction* DialogWaiter::wait()
{
    mLoop.exec();
    HbAction* a=mResult;
    mResult=0;
    return a;
}

void DialogWaiter::done(HbAction* result)
{
    mResult=result;
    mLoop.quit();
}

