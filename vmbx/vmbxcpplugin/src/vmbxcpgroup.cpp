/*
 * Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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

// System includes
#include <cpitemdatahelper.h>

// User includes
#include "vmbxuiengine.h"
#include "vmbxcpgroup.h"
#include "loggerutil.h"

/*!
    Constructor
*/
VmbxCpGroup::VmbxCpGroup(
    const QString &configFile,
    const HbDataFormModelItem *parent) :
        CpSettingFormItemData(parent),
        mUiEngine(0),
        mEditorCsVoice1(0),
        mEditorCsVoice2(0),
        mEditorCsVideo1(0)
{
    Q_UNUSED(configFile);
}

/*!
    Constructor
*/
VmbxCpGroup::VmbxCpGroup(
    HbDataFormModelItem::DataItemType type,
    const QString &label,
    const QString &configFile,
    const HbDataFormModelItem *parent,
    CpItemDataHelper &itemDataHelper) :
        CpSettingFormItemData(type, label, parent),
        mUiEngine(0),
        mEditorCsVoice1(0),
        mEditorCsVoice2(0),
        mEditorCsVideo1(0)
{ 
    _DBGLOG2("VmbxCpGroup::VmbxCpGroup label=",label)
    Q_UNUSED(configFile);
    mUiEngine = new VmbxUiEngine();
    loadingPreparation(itemDataHelper);
    _DBGLOG("VmbxCpGroup::VmbxCpGroup <")
}

/*!
    Destructor
*/
VmbxCpGroup::~VmbxCpGroup()
{
    _DBGLOG("VmbxCpGroup::~VmbxCpGroup")
    if (mUiEngine) {
        delete mUiEngine;
        mUiEngine = NULL;
    }
}

/*!
    Preparation before loading
*/
void VmbxCpGroup::loadingPreparation(CpItemDataHelper &itemDataHelper)
{   
    _DBGLOG("VmbxCpGroup::loadingPreparation >")
    
    QString newUiValue("");

    mEditorCsVoice1 = new CpSettingFormItemData(
            HbDataFormModelItem::TextItem, tr("Call mailbox") );
    _DBGLOG("VmbxCpGroup::loadingPreparation appendChild")
    itemDataHelper.addConnection( 
        mEditorCsVoice1, 
        SIGNAL(textChanged(QString)), 
        mUiEngine, SLOT(uiCsVoice1Changed(QString))
        );
    // appendChild takes ownership
    appendChild(mEditorCsVoice1);

    // Todo: add csVoice2 if ALS supported
    
    // Todo: add video only if supported
    //mEditorCsVideo1 = new CpSettingFormItemData(
    //        HbDataFormModelItem::TextItem, tr("Video mailbox") );
    //_DBGLOG("VmbxCpGroup::loadingPreparation appendChild")
    //appendChild( mEditorCsVideo1 );
    
    // TODO: get all ps addresses AND THEIR NAMES from engine and add ui controls
    //for them
    // Set UI control pointers to mPsServiceEditors
    
    updateAllMailboxesToUi();
    
    // TODO: below connect ui editing signals and allow ui control
    // eidting if vmbx allows editing
                
    // TODO: here check CS ALS editing permission and connnect signal
    // and enable editing 
        
    // TODO: here check video editing permission and connect signal
    // if ok

    // No need for Video ALS on UI, implement later if necessary.

    
    // Editing of PS Services is not allowed by default, disable ui editing
    // for them.

    _DBGLOG("VmbxCpGroup::loadingPreparation <")
}

/*!
   Update Cs voice primary 
*/
void VmbxCpGroup::updateCsVoicePrimaryToUi()
{
    _DBGLOG("VmbxCpGroup::updateCsVoicePrimaryToUi")
    if (mEditorCsVoice1 && mUiEngine) {
        QString newUiValue("");
        mUiEngine->getCsVoice1Number(newUiValue);
        mEditorCsVoice1->setContentWidgetData("text", newUiValue);
    }
}

/*!
    Update Cs voice Als
*/
void VmbxCpGroup::updateCsVoiceAlsToUi()
{
    _DBGLOG("VmbxCpGroup::updateCsVoiceAlsToUi")
    if (mEditorCsVoice2 && mUiEngine){
        QString newUiValue("");
        mUiEngine->getCsVoice1Number(newUiValue );
        _DBGLOG2("VmbxCpGroup::updateCsVoiceAlsToUi newUiValue=", newUiValue)
        mEditorCsVoice2->setContentWidgetData("text", newUiValue );
    }
}

/*!
   update Cs video primary
*/
void VmbxCpGroup::updateCsVideoPrimaryToUi()
{
    _DBGLOG("VmbxCpGroup::updateCsVideoPrimaryToUi >")
    if (mEditorCsVideo1 && mUiEngine){
        QString newUiValue("");
        mUiEngine->getCsVideo1Number(newUiValue);
        _DBGLOG2("VmbxCpGroup::updateCsVideoPrimaryToUi newUiValue=", \
            newUiValue)
        mEditorCsVideo1->setContentWidgetData("text", newUiValue);
    }
    _DBGLOG("VmbxCpGroup::updateCsVideoPrimaryToUi <")
}

/*!
   update Ps services
*/
void VmbxCpGroup::updatePsServicesToUi()
{
    _DBGLOG("VmbxCpGroup::updatePsServicesToUi >")
    // Set values from engine to ui
    // NOTICE: IT IS POSSIBLE THAT PS SERVICES WILL NOT BE DISPLAYED BY VMBX 
    // CONTROL PANEL WHEN WE GET THE NEW UI CONCEPT!!!!!!
    _DBGLOG("VmbxCpGroup::updatePsServicesToUi <")
}

/*!
    Update default mailbox
*/
void VmbxCpGroup::updateDefaultMailboxToUi()
{
    _DBGLOG("VmbxCpGroup::updateDefaultMailboxToUi >")
    // Consider should you check here if the number 
    // length of the default mailbox
    // and reset the default mailbox type selection to "none" if it is.
    // If you do so, remember to save the new value to engine.
    _DBGLOG("VmbxCpGroup::updateDefaultMailboxToUi <")
}

/*!
    Update all mailboxes
*/
void VmbxCpGroup::updateAllMailboxesToUi()
{
    _DBGLOG("VmbxCpGroup::updateAllMailboxesToUi >")
    updateCsVoicePrimaryToUi();
    //updateCsVoiceAlsToUi();
    //updateCsVideoPrimaryToUi();
    //updatePsServicesToUi();
    //updateDefaultMailboxToUi();
    _DBGLOG("VmbxCpGroup::updateAllMailboxesToUi <")
} 

/*!
    Notification slot for engine signal about 
    Voice Mailbox entries' changes
*/
void VmbxCpGroup::voiceMailboxEngineEntriesUpdated()
{
    _DBGLOG("VmbxCpGroup::voiceMailboxEngineEntriesUpdated")
    // Consider is there need to pass the actual modified data 
    // and mailbox type enum to slot (here) so only needed 
    // data can be updated.
    updateAllMailboxesToUi();
}

//End of file