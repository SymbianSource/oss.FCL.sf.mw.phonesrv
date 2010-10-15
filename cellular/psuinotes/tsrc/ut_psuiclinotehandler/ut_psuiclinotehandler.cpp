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

#include "ut_psuiclinotehandler.h"
#include "psuiclinotehandler.h"
#include "psetcliwrapper.h"
#include "psuinotes.h"
#include "psuiutils.h"
#include "qtestmains60.h"
#include <smcmockclassincludes.h>

class CPsetContainer
{
public:
    CPsetContainer(){};
    ~CPsetContainer(){};
};

/*!
  UT_psuiclinotehandler::UT_psuiclinotehandler
 */
UT_psuiclinotehandler::UT_psuiclinotehandler() 
{
    
}

/*!
  UT_psuiclinotehandler::~UT_psuiclinotehandler
 */
UT_psuiclinotehandler::~UT_psuiclinotehandler()
{
    
}

/*!
  UT_psuiclinotehandler::init
 */
void UT_psuiclinotehandler::init()
{
    initialize();
    m_psetContainer = new CPsetContainer();
    m_mockCliWrapper = new PSetCliWrapper(*m_psetContainer,this);
    m_cliHandler = new PsUiCliNoteHandler(*m_mockCliWrapper);
}

/*!
  UT_psuiclinotehandler::cleanup
 */
void UT_psuiclinotehandler::cleanup()
{
    delete m_cliHandler;
    delete m_mockCliWrapper;   
    delete m_psetContainer;
    reset();
}

/*!
  UT_psuiclinotehandler::t_memleak
 */
void UT_psuiclinotehandler::t_memleak()
{
    
}

/*!
  UT_psuiclinotehandler::t_construction
 */
void UT_psuiclinotehandler::t_construction()
{
    EXPECT(QObject,connect).times(4);
    CPsetContainer psetContainer;
    PSetCliWrapper *mockCliWrapper = new PSetCliWrapper(psetContainer,this);
    PsUiCliNoteHandler *cliHandler = new PsUiCliNoteHandler(*mockCliWrapper);
    delete cliHandler;
    delete mockCliWrapper;
    QVERIFY(verify());
}

void UT_psuiclinotehandler::t_handleCliRequesting()
{   
    EXPECT(PsUiNotes,showGlobalProgressNote);
    bool ongoing = true;
    bool interrupted = false;
    m_cliHandler->handleCliRequesting(ongoing,interrupted);
    QVERIFY(verify());
    
    EXPECT(PsUiNotes,cancelNote).times(2);
    ongoing = false;
    interrupted = true;
    m_cliHandler->handleCliRequesting(ongoing,interrupted);
    QVERIFY(verify()); 
    
    EXPECT(PsUiNotes,cancelNote);
    ongoing = false;
    interrupted = false;
    m_cliHandler->handleCliRequesting(ongoing,interrupted);
    QVERIFY(verify()); 
}

void UT_psuiclinotehandler::t_cliInformation()
{  
    EXPECT(PsUiNotes,cancelNote);
   
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_my_caller_id_not_sent_on_calling"));
    m_cliHandler->cliInformation(ClirOn);
    QVERIFY(verify());
  
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_my_caller_id_sent_on_calling"));
    m_cliHandler->cliInformation(ClirOff);
    QVERIFY(verify());
   
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_number_of_caller_is_shown"));
    m_cliHandler->cliInformation(ClipOn);
    QVERIFY(verify());
    
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_number_of_caller_is_not_shown"));
    m_cliHandler->cliInformation(ClipOff);
    QVERIFY(verify());
  
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_my_caller_id_not_sent_on_answer"));
    m_cliHandler->cliInformation(ColrOn);
    QVERIFY(verify());
    
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_my_caller_id_sent_on_answering"));
    m_cliHandler->cliInformation(ColrOff);
    QVERIFY(verify());
    
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_answering_number_is_shown"));
    m_cliHandler->cliInformation(ColpOn);
    QVERIFY(verify());
    
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_answering_number_is_not_shown"));
    m_cliHandler->cliInformation(ColpOff);
    QVERIFY(verify());
    
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_info_result_unknown")).times(2);
    m_cliHandler->cliInformation(CliUnknown);
    m_cliHandler->cliInformation((PsCallLineIdentity)100); 
    QVERIFY(verify()); 
}

void UT_psuiclinotehandler::t_handleCliError()
{
    EXPECT(PsUiNotes,cancelNote);
    EXPECT(PsUiNotes,showGlobalErrorNote);
    int error = -1;
    m_cliHandler->handleCliError(error);
    
    QVERIFY(verify());
}

void UT_psuiclinotehandler::t_handleCnapStatus()
{  
    EXPECT(PsUiNotes,instance);
      
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_name_of_caller_is_shown")).times(2);
    m_cliHandler->handleCnapStatus(PsUiUtils::ServiceActivePermanent);
    m_cliHandler->handleCnapStatus(PsUiUtils::ServiceActiveDefaultAllowed);  
    QVERIFY(verify());
    
    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_dpopinfo_name_of_caller_is_not_shown")).times(2);
    m_cliHandler->handleCnapStatus(PsUiUtils::ServiceActiveDefaultRestricted);
    m_cliHandler->handleCnapStatus(PsUiUtils::ServiceNotProvisioned);
    QVERIFY(verify());

    EXPECT(PsUiNotes,showGlobalNotificationDialog).with(
        QString("txt_phone_info_request_not_completed"));
    m_cliHandler->handleCnapStatus(PsUiUtils::ServiceUnknown);
    QVERIFY(verify());
}

QTEST_MAIN_S60(UT_psuiclinotehandler)
