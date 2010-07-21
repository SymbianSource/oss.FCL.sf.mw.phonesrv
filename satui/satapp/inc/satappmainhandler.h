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
#include <QVariant>

class HbMainWindow;
class SatAppServerDispatcher;
class SatAppMenuProvider;
class SatAppInputProvider;
class SatAppToneProvider;
class SatAppPopupProvider;
class SatAppConfirmProvider;

class SatAppMainHandler : public QObject
{
    Q_OBJECT

public:
    SatAppMainHandler(HbMainWindow &window, QObject *parent = 0);
    ~SatAppMainHandler();
    
private:
    void initConnections();
    
private slots:
    void updateActivity();
    void saveActivity();

private:
    void removeActivity();
    
private: // data

    SatAppServerDispatcher* mServer;
    SatAppMenuProvider* mMenu;
    SatAppInputProvider* mInput;
    SatAppToneProvider* mTone;
    SatAppPopupProvider* mPopup;
    SatAppConfirmProvider *mConfirm;
    QVariantHash mActivity;
};

#endif // SATAPPMAINHANDLER_H
