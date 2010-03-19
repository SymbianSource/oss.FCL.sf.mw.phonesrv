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
*
*/

// qt
#include <QtGui/QtGui>
#include <QString>
#include <QObject>

#include <hbdialog.h>
#include <hblabel.h>
#include <hbaction.h>
#include <hbtextedit.h>
#include <hbmessagebox.h>
#include <hbvalidator.h>
#include <hblineedit.h>
#include <hbinputeditorinterface.h>
#include <hblistwidget.h>

#include "vmbxqtuihandler.h"
#include "voicemailboxdefsinternal.h"
#include "vmbxlogger.h"

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::VmbxQtUiHandler
// (Constructor).
// ----------------------------------------------------------------------------
//
VmbxQtUiHandler::VmbxQtUiHandler(QObject* parent): QObject(parent)
{
    VMBLOGSTRING("VmbxQtUiHandler::VmbxQtUiHandler")
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
    VMBLOGSTRING("VmbxQtUiHandler::~VmbxQtUiHandler Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::ShowVmbxQueryDialog
// (show query dialog).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::showVmbxQueryDialog(const TVmbxType& aType,
                          const TVmbxQueryMode& aMode,
                          QString& aNumber, int& aResult)
{
    VMBLOGSTRING("VmbxQtUiHandler::showVmbxQueryDialog")

    HbDialog *displayPopup = new HbDialog();
    displayPopup->setDismissPolicy(HbDialog ::NoDismiss);
    displayPopup->setTimeout(HbDialog ::NoTimeout);

    displayPopup->setModal(true);
    QString header;
    if (EVmbxVoice == aType) {
        if (EVmbxDefineMode == aMode) {
            header = tr("Define voice mailbox:");
        } else {
            header = tr("Change voice mailbox:");
        }
    } else {
        // type of EVmbxVideo
        if (EVmbxDefineMode == aMode) {
            header = tr("Define video mailbox:");
        } else {
            header = tr("Change video mailbox:");
        }
    }
    // Set the label as heading widget
    displayPopup->setHeadingWidget(new HbLabel(header));
    // Set the HbLineEdit as Content widget
    // Define parameters for text box
    HbLineEdit *lineEdit = new HbLineEdit();
    lineEdit->setText( aNumber ); // default text
    //lineEdit->setMaximumRows( 3 ); // just a magic number
    HbEditorInterface inputMode(lineEdit); // Enables VKB
    inputMode.setInputMode( HbInputModeNumeric );
    // Limit charachter set
    HbValidator *val = new HbValidator;
    QRegExp r;
    r.setPattern("[0123456789*#+]{0,255}"); // define what characters can be entered
    val->setMasterValidator(new QRegExpValidator(r,0));
    lineEdit->setValidator( val );

    displayPopup->setContentWidget( lineEdit ); // ownership moved

    HbAction* okAction = new HbAction(tr("Ok"));
    // Sets the "OK"-action/button
    displayPopup->setPrimaryAction(okAction);
    bool ret = connect(okAction, SIGNAL(triggered()),
        displayPopup, SLOT(close()));
    VMBLOGSTRING2( "VmbxQtUiHandler::showVmbxQueryDialog:connect ok %d",
        ret )
    //  Sets the "Back"-action/button
    HbAction* cancelAction =  new HbAction(tr("Cancel"));
    displayPopup->setSecondaryAction(cancelAction);
    ret = connect(okAction, SIGNAL(triggered()),
        displayPopup, SLOT(close()));
    VMBLOGSTRING2( "VmbxQtUiHandler::showVmbxQueryDialog:connect cancel %d",
        ret )
    VMBLOGSTRING( "VmbxQtUiHandler::ShowVmbxQueryDialog to show" )
    HbAction* result = displayPopup->exec();
    if( okAction == result ) {
        VMBLOGSTRING( "VmbxQtUiHandler::showVmbxQueryDialog select OK" )
        // asign OK KEY value
        aResult = KErrNone;
        aNumber = lineEdit->text();
        VMBLOGSTRING2( "VmbxQtUiHandler::showVmbxQueryDialog number = %s",
            aNumber.utf16() )
    } else {
        aResult = KErrCancel;
    }
    delete val;
    val = 0;
    delete displayPopup;
    displayPopup = 0;
    VMBLOGSTRING2("VmbxQtUiHandler::showVmbxQueryDialog aResult = %d", aResult)
    VMBLOGSTRING("VmbxQtUiHandler::showVmbxQueryDialog Exit")
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::showDefineSelectionDialog
// (Shoe define list dialog).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::showDefineSelectionDialog(
    TVmbxType& aType, int& aResult)
{
    VMBLOGSTRING("VmbxQtUiHandler::showDefineSelectionDialog")
    HbDialog* defineListDialog = new HbDialog();
    defineListDialog->setDismissPolicy(HbDialog::NoDismiss);
    defineListDialog->setTimeout(HbDialog::NoTimeout);

    defineListDialog->setModal(true);
    // Set heading widget
    defineListDialog->setHeadingWidget(new HbLabel(tr("Define number:")));
    // Create a define list 
    HbListWidget *list = new HbListWidget();
    list->addItem(tr("Voice Mailbox"));
    list->addItem(tr("Video Mailbox"));
    // Set content widget
    defineListDialog->setContentWidget(list);

    HbAction* okAction = new HbAction(tr("Ok"));
    // Sets the "OK"-action/button
    defineListDialog->setPrimaryAction(okAction);
    //  Sets the "Back"-action/button
    HbAction* cancelAction =  new HbAction(tr("Cancel"));
    defineListDialog->setSecondaryAction(cancelAction);

    connect(list, SIGNAL(activated(HbListWidgetItem*)), 
        defineListDialog, SLOT(close()));
    HbAction* result = defineListDialog->exec();

    if (cancelAction == result){
        aType = EVmbxNone;
        aResult = KErrCancel;
    } else {
    // OK or single click items
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
// VmbxQtUiHandler::ShowInformationNote
// (show information note).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::showInformationNote(int aNoteType)
{
    VMBLOGSTRING("VmbxQtUiHandler::showInformationNote")
    QString noteText;
    QString iconName;
    VMBLOGSTRING2("VmbxQtUiHandler::~showInformationNote type =%d", aNoteType)
    switch (aNoteType) {
    case EInvalidNumber:
        noteText = tr("Invalid phone number.");
        iconName = "note_error";
        break;
    case ESavedToPhoneMemory:
        noteText = tr("Voice mailbox number saved to Phone memory.");
        iconName = "note_info";
        break;
    case ESavedToSimMemory:
        noteText = tr("Voice mailbox number saved to Sim card.");
        iconName = "note_info";
        break;
    case EVideoNumberSaved:
        noteText = tr("Video call mailbox saved.");
        iconName = "note_info";
        break;
    default:
        VMBLOGSTRING("VmbxQtUiHandler::ShowInformationNote default")
        break;
    }

    HbMessageBox *msgBox = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    msgBox->setText(noteText);
    msgBox->setIcon(HbIcon(iconName));
    msgBox->exec();
    delete msgBox;
    VMBLOGSTRING("VmbxQtUiHandler::~ShowInformationNote Exit")
    
}

// ----------------------------------------------------------------------------
// VmbxQtUiHandler::ShowSaveEmptyNote
// (show information note).
// ----------------------------------------------------------------------------
//
void VmbxQtUiHandler::showSaveEmptyNote(const TVmbxType& aType)
{
    VMBLOGSTRING("VmbxQtUiHandler::~showSaveEmptyNote")
    QString noteText;
    VMBLOGSTRING2("VmbxQtUiHandler::~showSaveEmptyNote type = %d", aType)
    switch (aType) {
    case EVmbxVoice:
        noteText = tr("Voice mailbox number not defined.");
        break;
    case EVmbxVideo:
        noteText = tr("Video mailbox number not defined.");
        break;
    default:
        VMBLOGSTRING("VmbxQtUiHandler::ShowSaveEmptyNote default")
        break;
    }
    HbMessageBox *msgBox = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    msgBox->setText(noteText);
    msgBox->setIcon(HbIcon("note_error"));
    msgBox->exec();
    delete msgBox;
    VMBLOGSTRING("VmbxQtUiHandler::~ShowSaveEmptyNote Exit")
}

//End of file
