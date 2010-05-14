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
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStringList>
#include <cpitemdatahelper.h>
#include <hblineedit.h>

// User includes
#include "vmbxuiengine.h"
#include "vmbxcpgroup.h"
#include "loggerutil.h"

// custom dataform type
#define CustomDataFormType \
    static_cast<HbDataFormModelItem::DataItemType>(HbDataFormModelItem::CustomItemBase)

/*!
    Constructor
*/
VmbxCpGroup::VmbxCpGroup(
    CpItemDataHelper &itemDataHelper) :
        CpSettingFormItemData(HbDataFormModelItem::GroupItem,
        hbTrId("txt_phone_subhead_call_mbx"), NULL),
        mUiEngine(NULL),
        mDefaultMailboxEditor(NULL),
        mCsVoice1Editor(NULL),
        mCsVideo1Editor(NULL)
{ 
    _DBGLOG2("VmbxCpGroup::VmbxCpGroup label=",label)
    // Localization file loading
    QTranslator translator; 
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";

    bool translatorLoaded = translator.load(path + "telephone_cp_" + lang);
    _DBGLOG("VmbxCpGroup::VmbxCpGroup translator loaded")
    if (translatorLoaded && qApp) {
        qApp->installTranslator(&translator);
        _DBGLOG("VmbxCpGroup::VmbxCpGroup translator installed")
    }

    mUiEngine = new VmbxUiEngine();
    loadingPreparation(itemDataHelper);

    // Request notify when VMBX number changed
    mUiEngine->notifyVmbxNumberChange(true);
    bool ret = connect(mUiEngine,
            SIGNAL(voiceMailboxEngineEntriesUpdated(const TVmbxType)), 
            this, SLOT(vmbxNumberChanged(const TVmbxType)));
    _DBGLOG2("VmbxCpGroup::VmbxCpGroup connect ret=", ret)
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

/*!
    Save default mailbox when user selection finished
*/ 
void VmbxCpGroup::saveDefaultMailbox(int aIndex)
{
    if (0 == aIndex){
        _DBGLOG("VmbxCpGroup::saveDefaultMailbox voice")
    }else if(1 == aIndex){
        _DBGLOG("VmbxCpGroup::saveDefaultMailbox video")
    }else{
        _DBGLOG("VmbxCpGroup::saveDefaultMailbox other")
    }
}

/*!
    Preparation before loading
*/
void VmbxCpGroup::loadingPreparation(CpItemDataHelper &itemDataHelper)
{   
    _DBGLOG("VmbxCpGroup::loadingPreparation >")
    
    // #Following code is reserved for TB10.2
    // Create default mailbox selection
    // appendChild takes ownership
    //if (mUiEngine->isVideoSupport()){
    //    mDefaultMailboxEditor = new CpSettingFormItemData(
    //            HbDataFormModelItem::ComboBoxItem, 
    //            hbTrId("Default mailbox"),
    //            this);
        //itemDataHelper.addConnection( 
        //    mDefaultMailboxEditor, SIGNAL(currentIndexChanged(int)), 
        //    mUiEngine, SLOT(saveCsVoice1Number()));
        //QStringList list;
        //list.append(hbTrId("Voice"));
        //list.append(hbTrId("Video"));
        //mDefaultMailboxEditor->setContentWidgetData("items", QVariant(list));
        //appendChild(mDefaultMailboxEditor);          
    //}

    // Create cs voice primary editor
    mCsVoice1Editor= new HbDataFormModelItem(
        CustomDataFormType,
        hbTrId("txt_phone_setlabel_default_mbx_val_voice"));
    _DBGLOG("VmbxCpGroup::loadingPreparation appendChild voice1 mailbox")

    QString voice1Number;
    mUiEngine->getCsVoice1Number(voice1Number);
    // Show voice line1 number on lineedit
    mCsVoice1Editor->setContentWidgetData(QString("text"), voice1Number);
    // Connect ui editing signals and allow ui control
    // eidting if vmbx allows editing
    if (mUiEngine->isVoiceWritable()) {
         // When click the lineedit, the query dialog would pop up.
        itemDataHelper.addConnection(mCsVoice1Editor, SIGNAL(clicked()),
            this, SLOT(queryCsVoiceNumber()));
    } else {
        // If not writable, Dim the voice editor
        mCsVoice1Editor->setEnabled(false);
    }
    // Add child
    appendChild(mCsVoice1Editor);
 
    // Create cs video primary editor
    if (mUiEngine->isVideoSupport()){
        mCsVideo1Editor= new HbDataFormModelItem(
            CustomDataFormType,
            hbTrId("txt_phone_setlabel_default_mbx_val_video"));
        _DBGLOG("VmbxCpGroup::loadingPreparation appendChild video1 mailbox")
        QString video1Number;
        mUiEngine->getCsVideo1Number(video1Number);
        // Show video line1 number on lineedit
        mCsVideo1Editor->setContentWidgetData(QString("text"), video1Number);
        // Check video editing permission and connect signal
        if (mUiEngine->isVideoWritable()) {
            itemDataHelper.addConnection(mCsVideo1Editor, SIGNAL(clicked()),
                this, SLOT(queryCsVideoNumber()));
        } else {
            // If not writable, Dim the Video editor
            mCsVideo1Editor->setEnabled(false);
        }
        appendChild( mCsVideo1Editor );
    }
    // TODO: Create cs voice als editor
    // TODO: Create cs video als editor    
    // TODO: Create all ps service editors
    
    updateAllMailboxesToUi();
                
    // TODO: here check CS ALS editing permission and connnect signal
    // and enable editing 
        
    // No need for Video ALS on UI, implement later if necessary.
    // Editing of PS Services is not allowed by default, disable ui editing
    // for them.

    _DBGLOG("VmbxCpGroup::loadingPreparation <")
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
    if (mDefaultMailboxEditor && mUiEngine) {
        // TODO: Need a new API to save default mailbox
    }
    _DBGLOG("VmbxCpGroup::updateDefaultMailboxToUi <")
}

/*!
   Update Cs voice primary 
*/
void VmbxCpGroup::updateCsVoice1ToUi()
{
    _DBGLOG("VmbxCpGroup::updateCsVoice1ToUi")
    if (mCsVoice1Editor && mUiEngine) {
        QString newUiValue("");
        mUiEngine->getCsVoice1Number(newUiValue);
        _DBGLOG2("VmbxCpGroup::updateCsVoice1ToUi\
             newUiValue=", newUiValue)
        mCsVoice1Editor->setContentWidgetData("text", newUiValue);
    }
}

/*!
   update Cs video primary
*/
void VmbxCpGroup::updateCsVideo1ToUi()
{
    _DBGLOG("VmbxCpGroup::updateCsVideo1ToUi >")
    if (mCsVideo1Editor && mUiEngine){
        QString newUiValue("");
        mUiEngine->getCsVideo1Number(newUiValue);
        _DBGLOG2("VmbxCpGroup::updateCsVideo1ToUi\
             newUiValue=", newUiValue)
        mCsVideo1Editor->setContentWidgetData("text", newUiValue);
    }
    _DBGLOG("VmbxCpGroup::updateCsVideo1ToUi <")
}

/*!
    Update all mailboxes
*/
void VmbxCpGroup::updateAllMailboxesToUi()
{
    _DBGLOG("VmbxCpGroup::updateAllMailboxesToUi >")
    updateCsVoice1ToUi();
    if (mUiEngine->isVideoSupport()) {
        updateDefaultMailboxToUi();
        updateCsVideo1ToUi();
    }
    _DBGLOG("VmbxCpGroup::updateAllMailboxesToUi <")
} 

/*!
    Query voice number
*/
void VmbxCpGroup::queryCsVoiceNumber()
{
    _DBGLOG("VmbxCpGroup::queryCsVoiceNumber >")
    mCsVoice1Editor->setEnabled(false);
    QString number;
    int result = mUiEngine->queryVoiceNumber(number);
    _DBGLOG2("VmbxCpGroup::queryCsVoiceNumber result=", result)
    if (KErrNone == result) {
        mCsVoice1Editor->setContentWidgetData(QString("text"), number);
        _DBGLOG2("VmbxCpGroup::queryCsVoiceNumber >, number=", number)
        mUiEngine->setCsVoice1Number(number);   
    }
    mCsVoice1Editor->setEnabled(true);
    _DBGLOG("VmbxCpGroup::queryCsVoiceNumber <")
}

/*!
    Query video number
*/
void VmbxCpGroup::queryCsVideoNumber()
{
    _DBGLOG("VmbxCpGroup::queryCsVideoNumber >")
    mCsVideo1Editor->setEnabled(false);
    QString number;
    int result = mUiEngine->queryVideoNumber(number);
    _DBGLOG2("VmbxCpGroup::queryCsVideoNumber  result=", result)
    if (KErrNone == result) {
        mCsVideo1Editor->setContentWidgetData(QString("text"), number);
        _DBGLOG2("VmbxCpGroup::queryCsVideoNumber >, number=", number)
        mUiEngine->setCsVideo1Number(number);
    }
    mCsVideo1Editor->setEnabled(true);
    _DBGLOG("VmbxCpGroup::queryCsVideoNumber <")
}

/*!
    Update number when vmbx number changed by OTA,OMA etc.
*/
void VmbxCpGroup::vmbxNumberChanged(const TVmbxType vmbxType)
{
    _DBGLOG("VmbxCpGroup::vmbxNumberChanged >")
    // Update cs voice number
    if (EVmbxVoice == vmbxType) {
       updateCsVoice1ToUi();
    // Update cs video number
    } else if (EVmbxVideo == vmbxType) {
       updateCsVideo1ToUi();
    }
    _DBGLOG("VmbxCpGroup::vmbxNumberChanged <")
}

//End of file
