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
#ifndef UT_SIMHANDLER_H
#define UT_SIMHANDLER_H

#include <QObject>

class CVmbxSimHandler;

class Ut_SimHandler : public QObject
{
    Q_OBJECT
    
public:
    Ut_SimHandler();
    virtual ~Ut_SimHandler();
    
private slots:
    void testCreateAndDelete();
    //void testGetL();
    //void testSave();
    void testIsWritable();
    void testNotifyStoreEvent();        
    void testPhoneBookType();
    void testCancelAsyncRequest();
    
private:
    void createSim();
    void deleteSim();
    
private:
    CVmbxSimHandler *mSim;
};

#endif // UT_SIMHANDLER_H
