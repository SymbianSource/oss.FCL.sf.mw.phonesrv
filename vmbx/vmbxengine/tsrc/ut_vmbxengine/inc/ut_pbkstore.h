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
#ifndef UT_PBKSTORE_H
#define UT_PBKSTORE_H

#include <QObject>

class CVmbxPbkStore;

class Ut_PbkStore : public QObject
{
    Q_OBJECT
    
public:
    Ut_PbkStore();
    virtual ~Ut_PbkStore();
    
private slots:
    void testCreateAndDelete();
    void testIsWritable();
    void testPhoneBookType();
    // Can not handle AO in Current UT desing
    //void testWrite(); 
    void testPhonebookStore();
    //void testGetL();
    
private:
    void createPbk();
    void deletePbk();
    
private:
    CVmbxPbkStore *mPbk;
};

#endif // UT_PBKSTORE_H
