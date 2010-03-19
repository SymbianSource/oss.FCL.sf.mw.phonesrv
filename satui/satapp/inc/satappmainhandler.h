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
* Description:
*
*
*/

#ifndef SATAPPMAINHANDLER_H
#define SATAPPMAINHANDLER_H

#include <QObject>

class HbMainWindow;
class SatAppUiProvider;
class SatAppEventProvider;
class SatAppCommandHandler;

class SatAppMainHandler : public QObject
{
    Q_OBJECT

public:
    SatAppMainHandler(HbMainWindow &window, QObject *parent = 0);
    ~SatAppMainHandler();
    
private:
    void initConnections();

private: // data
    SatAppEventProvider *mEvent;
    SatAppUiProvider *mUi;
    SatAppCommandHandler *mCommand; 
};

#endif // SATAPPMAINHANDLER_H
