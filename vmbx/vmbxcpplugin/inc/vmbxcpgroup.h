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
#include <voicemailboxdefs.h>


// Forward declarations
class VmbxUiEngine;
class CpItemDataHelper;
class HbDataFormModelItem;

// Class declaration
class VmbxCpGroup : public CpSettingFormItemData
{
    Q_OBJECT

public:
    
    /*!
        Constructor
    */
    VmbxCpGroup( 
        CpItemDataHelper &itemDataHelper);

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

    /*!
        Save default Mailbox
    */
    void saveDefaultMailbox(int aIndex);

    /*!
        Query cs voice number when user click line edit
    */
    void queryCsVoiceNumber();

    /*!
        Query cs video number when user click line edit
    */
    void queryCsVideoNumber();

    /*!
        Update number when vmbx number changed by OTA,OMA etc.
    */
    void vmbxNumberChanged(const TVmbxType vmbxType);

private:
    
    /*!
        Preparation before loading
    */
    void loadingPreparation(CpItemDataHelper &itemDataHelper);    
    
    /*!
        Update default mailbox
    */
    void updateDefaultMailboxToUi();

    /*!
       Update Cs voice primary 
    */
    void updateCsVoice1ToUi();

    /*!
       update Cs video primary
    */
    void updateCsVideo1ToUi();
    
    /*!
        Update all mailboxes
    */
    void updateAllMailboxesToUi();

private:
    
    // Own.
    VmbxUiEngine *mUiEngine;

    // Not own.
    CpSettingFormItemData *mDefaultMailboxEditor;    

    // Not own.
    HbDataFormModelItem *mCsVoice1Editor;

    // Not own.
    HbDataFormModelItem *mCsVideo1Editor;
};

#endif // VMBXCPGROUP_H
