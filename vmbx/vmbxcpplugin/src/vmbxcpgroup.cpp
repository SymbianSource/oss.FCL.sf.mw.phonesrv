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
 
#include <hbsettingformdataitem.h> // TODO: remember to replace this during API switch
#include <cpitemdatahelper.h>
#include "vmbxuiengine.h"
#include "vmbxcpgroup.h"
#include "loggerutil.h"

// TODO: To get signals from ui control in QtControlPanelwk38 package you have 
// to fix the mistyped connect
// in qtcontrolpanel\qtcontrolpanel\cpframework\src\cpsettingformitemdata.cpp:
// "lineEditTextChanged" is mistyped as "lineEditTextChangge"

// -----------------------------------------------------------------------------
// Comment me!
// -----------------------------------------------------------------------------
//
VmbxCpGroup::VmbxCpGroup(
    const QString &/*configFile*/,
    const HbDataFormModelItem *parent) :
        CpSettingFormItemData( parent ),
        mEditorCsVoice1( 0 ) // TODO: null member pointers as it is good practice
{
}

// -----------------------------------------------------------------------------
// Comment me!
// -----------------------------------------------------------------------------
//
VmbxCpGroup::VmbxCpGroup(
    HbDataFormModelItem::DataItemType type,
    const QString &label,
    const QString &/*configFile*/,
    const HbDataFormModelItem *parent,
    CpItemDataHelper &itemDataHelper ) :
        CpSettingFormItemData( type, label, parent ),
        mEditorCsVoice1( 0 ),// TODO: null member pointers as it is good practice
        mEditorCsVoice2( 0 ),
        mEditorCsVideo1( 0 )
{ 
    _DBGLOG2("VmbxCpGroup::VmbxCpGroup label=",label)
    mUiEngine = new VmbxUiEngine();
    beforeLoadingConfigPlugins( itemDataHelper );
    _DBGLOG("VmbxCpGroup::VmbxCpGroup <")
}

// -----------------------------------------------------------------------------
// Comment me!
// -----------------------------------------------------------------------------
//
// Todo: rename this method
void VmbxCpGroup::beforeLoadingConfigPlugins( CpItemDataHelper &itemDataHelper ) 
{   
    _DBGLOG("VmbxCpGroup::beforeLoadingConfigPlugins >")
        
    // HbSettingFormDataItem::appendChild takes ownership
    
    QString newUiValue("");

    mEditorCsVoice1 = new CpSettingFormItemData(
            HbDataFormModelItem::TextItem, tr("Call mailbox") );
    _DBGLOG("VmbxCpGroup::beforeLoadingConfigPlugins appendChild")
    itemDataHelper.addConnection( 
        mEditorCsVoice1, 
        SIGNAL(textChanged(QString)), 
        mUiEngine, SLOT(uiCsVoice1Changed(QString) ) 
        );
    appendChild( mEditorCsVoice1 );

    // Todo: add csVoice2 if ALS supported
    
    // Todo: add video only if supported
    //mEditorCsVideo1 = new CpSettingFormItemData(
    //        HbDataFormModelItem::TextItem, tr("Video mailbox") );
    //_DBGLOG("VmbxCpGroup::beforeLoadingConfigPlugins appendChild")
    //appendChild( mEditorCsVideo1 );
    
    // TODO: get all ps addresses AND THEIR NAMES from engine and add ui controls for them
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

    _DBGLOG("VmbxCpGroup::beforeLoadingConfigPlugins <")
}

// -----------------------------------------------------------------------------
// Comment me!
// -----------------------------------------------------------------------------
//
VmbxCpGroup::~VmbxCpGroup()
{
    _DBGLOG("VmbxCpGroup::~VmbxCpGroup")
    if ( mUiEngine ) {
        delete mUiEngine;
    }
    // TODO: check cleanup, this is draft project version
}

// -----------------------------------------------------------------------------
// Comment me!
// -----------------------------------------------------------------------------
//
void VmbxCpGroup::updateCsVoicePrimaryToUi()
{
    _DBGLOG("VmbxCpGroup::updateCsVoicePrimaryToUi")
    if ( mEditorCsVoice1 &&mUiEngine) {
        QString newUiValue("");
        mUiEngine->GetCsVoice1Number( newUiValue );
        mEditorCsVoice1->setContentWidgetData( "text", newUiValue );
    }
}

// -----------------------------------------------------------------------------
// Comment me! 
// -----------------------------------------------------------------------------
//
void VmbxCpGroup::updateCsVoiceAlsToUi()
{
    _DBGLOG("VmbxCpGroup::updateCsVoiceAlsToUi")
    if ( mEditorCsVoice2 && mUiEngine )
        {
        QString newUiValue("");
        mUiEngine->GetCsVoice1Number( newUiValue );
        _DBGLOG2( "VmbxCpGroup::updateCsVoiceAlsToUi newUiValue=", newUiValue)
        mEditorCsVoice2->setContentWidgetData( "text", newUiValue );
        }
}

// -----------------------------------------------------------------------------
// Comment me! 
// -----------------------------------------------------------------------------
//
void VmbxCpGroup::updateCsVideoPrimaryToUi()
{
    _DBGLOG("VmbxCpGroup::updateCsVoiceAlsToUi")
    if ( mEditorCsVideo1 && mUiEngine )
        {
        QString newUiValue("");
        mUiEngine->GetCsVideo1Number( newUiValue );
        _DBGLOG2( "VmbxCpGroup::updateCsVideoPrimaryToUi newUiValue=", newUiValue)
        mEditorCsVideo1->setContentWidgetData( "text", newUiValue );
        }
}

// -----------------------------------------------------------------------------
// Comment me! 
// -----------------------------------------------------------------------------
//
void VmbxCpGroup::updatePsServicesToUi()
{
    _DBGLOG("VmbxCpGroup::updatePsServicesToUi EMPTY METHOD WAITING FOR IMPLEMENTING")
    /* 
        TODO: Set values from engine to ui
        NOTICE: IT IS POSSIBLE THAT PS SERVICES WILL NOT BE DISPLAYED BY VMBX 
        CONTROL PANEL WHEN WE GET THE NEW UI CONCEPT!!!!!!
    */
}

// -----------------------------------------------------------------------------
// Comment me! 
// -----------------------------------------------------------------------------
//
void VmbxCpGroup::updateDefaultMailboxToUi()
{
    _DBGLOG("VmbxCpGroup::updateDefaultMailboxToUi EMPTY METHOD WAITING FOR IMPLEMENTING")
    // TODO: implement me!
    // Consider should you check here if the number length of the default mailbox
    // and reset the default mailbox type selection to "none" if it is.
    // If you do so, remember to save the new value to engine. 
}

// -----------------------------------------------------------------------------
// Comment me! 
// -----------------------------------------------------------------------------
//
void VmbxCpGroup::updateAllMailboxesToUi()
{
    _DBGLOG("VmbxCpGroup::updateAllMailboxesToUi")
    updateCsVoicePrimaryToUi();
    updateCsVoiceAlsToUi();
    //updateCsVideoPrimaryToUi();
    updatePsServicesToUi();
    updateDefaultMailboxToUi();
} 

// -----------------------------------------------------------------------------
// Notification slot for engine signal about Voice Mailbox entries' changes
// -----------------------------------------------------------------------------
//
void VmbxCpGroup::voiceMailboxEngineEntriesUpdated()
{
    _DBGLOG("VmbxCpGroup::voiceMailboxEngineEntriesUpdated")
    /* TODO:  consider is there need to pass the actual modified data 
    and mailbox type enum to slot (here) so only needed data can be updated.
    */
    updateAllMailboxesToUi();
}
