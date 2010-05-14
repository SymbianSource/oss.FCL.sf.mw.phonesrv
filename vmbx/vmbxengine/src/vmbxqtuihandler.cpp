/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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

// qt
#include <QString>
#include <QCoreApplication>
#include <QRegExp>

#include <hbinputdialog.h>
#include <hblabel.h>
#include <hbaction.h>
#include <hbmessagebox.h>
#include <hblineedit.h>
#include <hbinputeditorinterface.h>
#include <hbinputstandardfilters.h>
#include <hbinputfilter.h> 
#include <hblistwidget.h>
#include <dialogwaiter.h>

#include <cvoicemailboxentry.h>
#include <cvoicemailbox.h>

#include "vmbxqtuihandler.h"
#include "voicemailboxdefsinternal.h"
#include "vmbxlogger.h"

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::VmbxQtUiHandler
// (Constructor).
// ----------------------------------------------------------------------------
//
VmbxQtUiHandler::VmbxQtUiHandler(QObject* parent): QObject(parent),
        iTranslator(0), iCommonTranslator(0), iQueryDialog(NULL)
{
    VMBLOGSTRING("VmbxQtUiHandler::VmbxQtUiHandler")
    init();
    VMBLOGSTRING("VmbxQtUiHandler::VmbxQtUiHandler Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::~VmbxQtUiHandler
// (Destructor).
// ----------------------------------------------------------------------------
//
VmbxQtUiHandler::~VmbxQtUiHandler()
{
    VMBLOGSTRING("VmbxQtUiHandler::~VmbxQtUiHandler")
    if ( iQueryDialog ) {
        delete iQueryDialog;
        iQueryDialog = NULL;
        VMBLOGSTRING("VmbxQtUiHandler::~VmbxQtUiHandler delete iQueryDialog")
    }
    VMBLOGSTRING("VmbxQtUiHandler::~VmbxQtUiHandler Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::Init
//
// ----------------------------------------------------------------------------
//
void  VmbxQtUiHandler::init()
{
    VMBLOGSTRING("VmbxQtUiHandler::init")
    QString lang = QLocale::system().name();
    VMBLOGSTRING2("VmbxQtUiHandler::init lang type %S",
            lang.utf16())
    QString path = "Z:/resource/qt/translations/";
    bool translatorLoaded = iTranslator.load("vmbx_"+lang, path);
    VMBLOGSTRING2("VmbxQtUiHandler::init load vmbx result %d",
            translatorLoaded)
    // Install vmbx localization
    if (translatorLoaded && qApp) {
        VMBLOGSTRING("VmbxQtUiHandler::init qApp !")
        qApp->installTranslator(&iTranslator);
    }
    // Install common localization, for select type "cancel".
    bool loaded = iCommonTranslator.load( "common_"+lang, path);
    VMBLOGSTRING2("VmbxQtUiHandler::init load common result %d",
        loaded)
    if (loaded && qApp) {
        VMBLOGSTRING("VmbxQtUiHandler::init qApp !!")
        qApp->installTranslator(&iCommonTranslator);
    }
    VMBLOGSTRING("VmbxQtUiHandler::init Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::ShowVmbxQueryDialog
// (show query dialog).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::showVmbxQueryDialog(const TVmbxType& aType,
                          QString& aNumber, int& aResult)
{
    VMBLOGSTRING("VmbxQtUiHandler::showVmbxQueryDialog")
    QString header;
    if (EVmbxVoice == aType) {
        header = hbTrId("txt_vmbx_title_voice_mailbox_number");
    } else if (EVmbxVideo == aType) {
        // type of EVmbxVideo
        header = hbTrId("txt_vmbx_title_video_mailbox_number");
    } else {
        aResult = KErrCancel;
        VMBLOGSTRING( "VmbxQtUiHandler::showVmbxQueryDialog type error" )
        return;
    }
    iQueryDialog = new HbInputDialog();
    iQueryDialog->setDismissPolicy(HbDialog::NoDismiss);
    iQueryDialog->setTimeout(HbDialog::NoTimeout);

    iQueryDialog->setModal(true);
    // Set heading content
    iQueryDialog->setPromptText(header);

    // Set HbLineEdit  Content 
    iQueryDialog->lineEdit()->setText(aNumber); // default text
    iQueryDialog->lineEdit()->setFocus(); // Enable the VKB
    iQueryDialog->lineEdit()->setMaxLength(KVmbxPhoneCharMaxLength);
    HbEditorInterface inputMode(iQueryDialog->lineEdit());
    inputMode.setFilter(HbPhoneNumberFilter::instance());
    bool ret = connect(iQueryDialog->lineEdit(), SIGNAL(textChanged(QString)),
        this, SLOT(updatePrimaryAction(QString)));    
    VMBLOGSTRING2( "VmbxQtUiHandler::showVmbxQueryDialog connect textChanged %d", ret)
    DialogWaiter waiter;
    iQueryDialog->open(&waiter, SLOT(done(HbAction *)));
    HbAction *result = waiter.wait();
    if (result == iQueryDialog->primaryAction()) {
        VMBLOGSTRING( "VmbxQtUiHandler::showVmbxQueryDialog select OK" )
        // asign OK KEY value
        aResult = KErrNone;
        aNumber = iQueryDialog->lineEdit()->text();
    } else {
        aResult = KErrCancel;
    }
    delete iQueryDialog;
    iQueryDialog = 0;
    VMBLOGSTRING2("VmbxQtUiHandler::showVmbxQueryDialog aResult = %d", aResult)
    VMBLOGSTRING("VmbxQtUiHandler::showVmbxQueryDialog Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::showDefineSelectionDialog
// (Shoe define list dialog).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::showDefineSelectionDialog(
    TVmbxType &aType, int &aResult)
{
    VMBLOGSTRING("VmbxQtUiHandler::showDefineSelectionDialog")
    HbDialog* defineListDialog = new HbDialog();
    defineListDialog->setDismissPolicy(HbDialog::NoDismiss);
    defineListDialog->setTimeout(HbDialog::NoTimeout);

    defineListDialog->setModal(true);
    // Set heading widget,"Define number:"
    defineListDialog->setHeadingWidget(new HbLabel(
            hbTrId("txt_vmbx_title_select_mailbox")));
    // Create a define list 
    HbListWidget *list = new HbListWidget();
    //Voice Mailbox
    list->addItem(hbTrId("txt_vmbx_list_voice_mailbox"));
    //Video Mailbox
    list->addItem(hbTrId("txt_vmbx_list_video_mailbox"));
    // Set content widget
    defineListDialog->setContentWidget(list);

    //  Sets the "Cancel"-action/button
    HbAction *cancelAction =  new HbAction(hbTrId(
            "txt_common_button_cancel"));
    defineListDialog->setSecondaryAction(cancelAction);

    connect(list, SIGNAL(activated(HbListWidgetItem *)), 
        defineListDialog, SLOT(close()));
    DialogWaiter waiter;
    defineListDialog->open(&waiter, SLOT(done(HbAction *)));
    HbAction *result = waiter.wait();
    if (cancelAction == result){
        aType = EVmbxNone;
        aResult = KErrCancel;
    } else {
        // Select one item
        aResult = KErrNone;
        int currentItem = list->row(list->currentItem());
        VMBLOGSTRING2("VmbxQtUiHandler::showDefineSelectionDialog:\
            currentItem %d", currentItem)
        if (0 == currentItem) {
            aType = EVmbxVoice;
        } else {
            aType = EVmbxVideo;
        }
    }
    VMBLOGSTRING2("VmbxQtUiHandler::showDefineSelectionDialog: aType%d", aType)
    VMBLOGSTRING2("VmbxQtUiHandler::showDefineSelectionDialog: aResult%d", \
        aResult)
    delete defineListDialog;
    defineListDialog = 0;
    VMBLOGSTRING("VmbxQtUiHandler::showDefineSelectionDialog Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::showCallSelectionDialog
// (show call selection dialog).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::showCallSelectionDialog(
    const QList<CVoiceMailboxEntry *> entryList,
    TVoiceMailboxParams &params, int &result )
{
    VMBLOGSTRING("VmbxQtUiHandler::showCallSelectionDialog")
    int count = entryList.count();
    VMBLOGSTRING("VmbxQtUiHandler::showCallSelectionDialog new before")
    HbDialog* callListDialog = new HbDialog();
    VMBLOGSTRING("VmbxQtUiHandler::showCallSelectionDialog new after")
    callListDialog->setDismissPolicy(HbDialog::NoDismiss);
    callListDialog->setTimeout(HbDialog::NoTimeout);
    callListDialog->setModal(true);
    // Set heading widget,"select mailbox"
    callListDialog->setHeadingWidget(new HbLabel(
               hbTrId("txt_vmbx_title_select_mailbox")));
    // Create a call list 
    HbListWidget *list = new HbListWidget();
    // Set content widget
    callListDialog->setContentWidget(list);

    QList<TVoiceMailboxParams> paramsList;
    TVoiceMailboxParams entryParams;
    for (int i = 0; i < count; i++) {
        TVmbxType vmbxType = entryList[i]->VoiceMailboxType();
        entryParams.iType = vmbxType;
        entryParams.iServiceId = entryList[i]->ServiceId();
        paramsList.append(entryParams);
        switch (vmbxType) {
            case EVmbxVoice:
                {
                list->addItem(hbTrId("txt_vmbx_list_voice_mailbox"));
                break;
                }
            case EVmbxVideo:
                {
                list->addItem(hbTrId("txt_vmbx_list_video_mailbox"));
                break;
                }
            case EVmbxVoip:
                {
                // Get name
                TPtrC vmbxName( KNullDesC );
                int voipRes = entryList[i]->GetVmbxName(vmbxName);
                VMBLOGSTRING2( "VmbxQtUiHandler::showCallSelectionDialogL\
                    : voipRes=%I", voipRes );
                if (KErrNone == voipRes) {
                    QString voipName;
                    if (vmbxName.Length() > 0) {
                        voipName=QString::fromUtf16(
                            vmbxName.Ptr(), vmbxName.Length());
                        // add voip item
                        list->addItem(voipName);
                    }
                }
                break;
                }
            default:
                break;
        }
    }
    //  Sets the "Cancel"-action/button
    HbAction *cancelAction = new HbAction(hbTrId(
        "txt_common_button_cancel_toolbar"));
    callListDialog->setSecondaryAction(cancelAction);

    connect(list, SIGNAL(activated(HbListWidgetItem *)), 
            callListDialog, SLOT(close()));
    DialogWaiter waiter;
    callListDialog->open(&waiter, SLOT(done(HbAction *)));
    HbAction *action = waiter.wait();
    if (cancelAction == action){
        params.iType = EVmbxNone;
        params.iServiceId = KVmbxServiceIdNone;
        result = KErrCancel;
    } else {
        // Select one item
        result = KErrNone;
        int currentItem = list->row(list->currentItem());
        VMBLOGSTRING2("VmbxQtUiHandler::showCallSelectionDialog:\
            currentItem %d", currentItem)
        params.iType = paramsList[currentItem].iType;
        params.iServiceId = paramsList[currentItem].iServiceId;
    }
    VMBLOGSTRING2("VmbxQtUiHandler::showCallSelectionDialog: params.iType %d",
        params.iType)
    VMBLOGSTRING2("VmbxQtUiHandler::showCallSelectionDialog: result%d",
        result)
    delete callListDialog;
    callListDialog = 0;
    VMBLOGSTRING("VmbxQtUiHandler::showCallSelectionDialog Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::showInformationNote
// (show information note).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::showInformationNote(const TVmbxNoteType aType)
{
    VMBLOGSTRING("VmbxQtUiHandler::showInformationNote")
    QString noteText;
    VMBLOGSTRING2("VmbxQtUiHandler::showInformationNote type =%d", aType)
    switch (aType) {
    case EInvalidNumber:
        // Need to comfirm with ui designer
        noteText = hbTrId("Invalid phone number.");
        break;
    case ENotAllowUserEditing:
        //User not allow to edit.
        noteText = hbTrId("Number not provisioned");
        break;
    case ESavedToPhoneMemory:
    case ESavedToSimMemory:	
        //Voice mailbox number saved to Phone memory.
        noteText = hbTrId("txt_vmbx_dpopinfo_voice_mailbox_number_saved");
        break;
    case EVideoNumberSaved:
        //Video call mailbox saved.
        noteText = hbTrId("txt_vmbx_dpopinfo_video_mailbox_number_saved");
        break;
    case EDefineVoiceNumber:
        //Define voice mailbox.
        noteText = hbTrId("txt_vmbx_dpopinfo_define_voice_mailbox_number");
        break;
    case EDefineVideoNumber:
        //Define video mailbox.
        noteText = hbTrId("txt_vmbx_dpopinfo_define_video_mailbox_number");
        break;
    default:
        VMBLOGSTRING("VmbxQtUiHandler::ShowInformationNote default")
        break;
    }
    HbMessageBox *msgBox = 0;
    if (EInvalidNumber == aType) {
        msgBox = new HbMessageBox(HbMessageBox::MessageTypeWarning);   
    } else {
        msgBox = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    }
    msgBox->setText(noteText);
    msgBox->removeAction(msgBox->primaryAction());
    DialogWaiter waiter;
    msgBox->open(&waiter, SLOT(done(HbAction *)));
    waiter.wait();
    delete msgBox;
    msgBox = 0;
    VMBLOGSTRING("VmbxQtUiHandler::showInformationNote Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::updatePrimaryAction
// (show information note).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::updatePrimaryAction(const QString &aInput)
{
    HbAction *ok = iQueryDialog->primaryAction();
    if (aInput.isEmpty() || aInput.contains(QRegExp("\\d\\d\\d"))){
        if (!ok->isEnabled()){
            ok->setEnabled(true);
            VMBLOGSTRING("VmbxQtUiHandler::updatePrimaryAction enable OK")
        }
    }else{
        if (ok->isEnabled()){
            ok->setEnabled(false);
            VMBLOGSTRING("VmbxQtUiHandler::updatePrimaryAction disable OK")  
        }
    }  
}

//End of file
