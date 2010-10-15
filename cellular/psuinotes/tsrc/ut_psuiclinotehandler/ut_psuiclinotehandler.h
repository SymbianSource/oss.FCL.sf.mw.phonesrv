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

#ifndef UT_PSUICLINOTEHANDLER_H
#define UT_PSUICLINOTEHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class CPsetContainer;
class PSetCliWrapper;
class PsUiCliNoteHandler;

class UT_psuiclinotehandler : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_psuiclinotehandler();
    ~UT_psuiclinotehandler();
    
private slots:
    void init();
    void cleanup();
    void t_memleak();
    
    void t_construction();
    
    void t_handleCliRequesting(); 
    void t_cliInformation();
    void t_handleCliError();  
    void t_handleCnapStatus();
 
private:
    CPsetContainer *m_psetContainer;
    PSetCliWrapper *m_mockCliWrapper;
    PsUiCliNoteHandler *m_cliHandler;
};

#endif // UT_PSUCLINOTEHANDLER_H
