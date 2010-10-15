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

#include <hbdevicenotificationdialog.h>
#include <psetcliwrapper.h>
#include "psuinotes.h"
#include "psuiclinotehandler.h"
#include "psuiutils.h"
#include "psuilogging.h"

/*!
  PsUiCliNoteHandler::PsUiCliNoteHandler
 */
PsUiCliNoteHandler::PsUiCliNoteHandler(PSetCliWrapper& cliWrapper) :
    m_cliWrapper(cliWrapper)
{
    DPRINT << ": IN";

    QObject::connect(
        &m_cliWrapper, 
        SIGNAL(cliInformation( const PsCallLineIdentity& )),
        this, 
        SLOT(cliInformation( const PsCallLineIdentity& )));
    QObject::connect(
        &m_cliWrapper, 
        SIGNAL(handleCliRequesting( bool, bool )),
        this, 
        SLOT(handleCliRequesting( bool, bool )));
    QObject::connect(
        &m_cliWrapper, 
        SIGNAL(handleCliError( int )),
        this, 
        SLOT(handleCliError( int )));
    
    QObject::connect(
        &m_cliWrapper, 
        SIGNAL(handleCnapStatus( int )),
        this, 
        SLOT(handleCnapStatus( int )));

    DPRINT << ": OUT";
}

/*!
  PsUiCliNoteHandler::~PsUiCliNoteHandler
 */
PsUiCliNoteHandler::~PsUiCliNoteHandler()
{
    DPRINT << ": IN";
    DPRINT << ": OUT";
}

/*!
  PsUiCliNoteHandler::handleCliInformation
 */
void PsUiCliNoteHandler::cliInformation( 
    const PsCallLineIdentity& type)
{
    DPRINT << ": IN";
    PsUiNotes::instance()->cancelNote(m_activeNoteId);

    switch (type) {
        case ClirOn:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_my_caller_id_not_sent_on_calling"));
            break;
        case ClirOff:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_my_caller_id_sent_on_calling"));
            break;
        case ClipOn:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_number_of_caller_is_shown"));
            break;
        case ClipOff:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_number_of_caller_is_not_shown"));
            break;
        case ColrOn:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_my_caller_id_not_sent_on_answer"));
            break;
        case ColrOff:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_my_caller_id_sent_on_answering"));
            break;
        case ColpOn:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_answering_number_is_shown"));
            break;
        case ColpOff:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_answering_number_is_not_shown"));
            break;
        case CliUnknown:
        default:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_info_result_unknown"));
            break;
    }
   
    DPRINT << ": OUT";
}

/*!
  PsUiCliNoteHandler::handleCliRequesting
 */
void PsUiCliNoteHandler::handleCliRequesting(bool ongoing,bool interrupted)
{
    DPRINT << ": IN";
    DPRINT << "ongoing:" << ongoing << ", interrupted:" << interrupted;
    
    if (interrupted) {
        PsUiNotes::instance()->cancelNote(m_activeProgressNoteId);
        PsUiNotes::instance()->cancelNote(m_activeNoteId);
    } else {
        if (ongoing) {
          PsUiNotes::instance()->showGlobalProgressNote(
              m_activeProgressNoteId, hbTrId("txt_common_info_requesting"));
      } else {
          PsUiNotes::instance()->cancelNote(m_activeProgressNoteId);
      }
    }            
    DPRINT << ": OUT";
}

/*!
PsUiCliNoteHandler::handleCnapStatus
 */
void PsUiCliNoteHandler::handleCnapStatus(int status)
{
    DPRINT << ": IN";   
    switch (status) {
        case PsUiUtils::ServiceActivePermanent:
        case PsUiUtils::ServiceActiveDefaultAllowed:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_name_of_caller_is_shown"));
            break;
        case PsUiUtils::ServiceActiveDefaultRestricted:
        case PsUiUtils::ServiceNotProvisioned:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_dpopinfo_name_of_caller_is_not_shown"));
            break;
        case PsUiUtils::ServiceUnknown:
        default:
            PsUiNotes::instance()->showGlobalNotificationDialog(
                hbTrId("txt_phone_info_request_not_completed"));
            break;
    }
    DPRINT << ": OUT";
}

/*!
  PsUiCliNoteHandler::handleCliError
 */
void PsUiCliNoteHandler::handleCliError(int error)
{
    DPRINT << ": IN";
    DPRINT << "errorCode:" << error;
    PsUiNotes::instance()->cancelNote(m_activeNoteId);
    PsUiNotes::instance()->showGlobalErrorNote(m_activeNoteId, error);

    DPRINT << ": OUT";
}
