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
#ifndef VMBXCPGROUP_H
#define VMBXCPGROUP_H

// System includes
#include <cpsettingformitemdata.h>

// Forward declarations
class VmbxUiEngine;
class CpItemDataHelper;

// Class declaration
class VmbxCpGroup : public CpSettingFormItemData
{
    Q_OBJECT
public:
    
    /*!
        Constructor
    */
    VmbxCpGroup( 
        HbDataFormModelItem::DataItemType type,
        const QString &label,
        const QString &configFile,
        const HbDataFormModelItem *parent,
        CpItemDataHelper &itemDataHelper);

    /*!
        Constructor
    */
     explicit VmbxCpGroup(
        const QString &configFile = QString(),
        const HbDataFormModelItem *parent = 0);

    /*!
        Destructor
    */
    ~VmbxCpGroup();

private slots:

    /*!
        Notification slot for engine signal about 
        Voice Mailbox entries' changes
    */
    void voiceMailboxEngineEntriesUpdated();

private:

    /*!
       Update Cs voice primary 
    */
    void updateCsVoicePrimaryToUi();

    /*!
        Update Cs voice Als
    */
    void updateCsVoiceAlsToUi();

    /*!
       update Cs video primary
    */
    void updateCsVideoPrimaryToUi();
    
    /*! 
       NOTICE: IT IS POSSIBLE THAT PS SERVICES WILL NOT BE DISPLAYED 
       BY VMBX CONTROL PANEL WHEN WE GET THE NEW UI CONCEPT!!!!!! 
       In that case this is not needed
    */

    /*!
       update Ps services
    */
    void updatePsServicesToUi();

    /*!
        Update default mailbox
    */
    void updateDefaultMailboxToUi();
    
    /*!
        Update all mailboxes
    */
    void updateAllMailboxesToUi();
    
private:

    /*!
        Preparation before loading
    */
    void loadingPreparation(CpItemDataHelper &itemDataHelper);


private:
    
    // Own.
    VmbxUiEngine *mUiEngine;

    // Not own.
    CpSettingFormItemData *mEditorCsVoice1;
    
    // Not own.
    CpSettingFormItemData *mEditorCsVoice2;
    
    // Not own.
    CpSettingFormItemData *mEditorCsVideo1;

    // Contained pointers not own.
    QList<CpSettingFormItemData *> mPsServiceEditors;
};

#endif // VMBXCPGROUP_H
