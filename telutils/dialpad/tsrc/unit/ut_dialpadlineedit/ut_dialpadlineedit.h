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
*/

#ifndef UT_DIALPADLINEEDIT_H
#define UT_DIALPADLINEEDIT_H

#include <QtTest/QtTest>
#include <mockservice.h>

class TestDialpadLineEdit;

class UT_dialpadlineedit : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_dialpadlineedit();
    ~UT_dialpadlineedit();
    
private slots:

    void init();
    void cleanup();

    void t_focusOutEvent();
    void t_focusInEvent();
    
private:
    TestDialpadLineEdit *m_classUt;
};

#endif  // UT_DIALPADLINEEDIT_H
