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
#ifndef VMBXCPGROUP_H
#define VMBXCPGROUP_H

#include <cpsettingformitemdata.h>

class VmbxUiEngine;
class CpItemDataHelper;
/*!
 *  vmbx control panel plugin
 *
 *  @lib vmbxcpplugin.lib
 *  @since S60 v5.2
 */

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO: update VmbxCpGroup to class VmbxCpGroup : public CpSettingFormItemData 
// when DataForm bug is fixed (QTControlPanel wk39-41?)
// API will change but it should be just a few minutes of work
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
class VmbxCpGroup : public CpSettingFormItemData
{
    Q_OBJECT
public:
    
    /*!
     *  Constructor
     */
    VmbxCpGroup( 
        HbDataFormModelItem::DataItemType type,
        const QString &label,
        const QString &configFile,
        const HbDataFormModelItem *parent,
        CpItemDataHelper &itemDataHelper);

    /*!
     *
     */
     explicit VmbxCpGroup(
        const QString &configFile = QString(),
        const HbDataFormModelItem *parent = 0);

    /*!
     * Destructor
     */
    ~VmbxCpGroup();

private slots:

    /*!
     *
     */
    void voiceMailboxEngineEntriesUpdated();

private:
// TODO: comment everything

    /*!
     *
     */
    void updateCsVoicePrimaryToUi();

    /*!
     *
     */
    void updateCsVoiceAlsToUi();

    /*!
     *
     */
    void updateCsVideoPrimaryToUi();
    /* NOTICE: IT IS POSSIBLE THAT PS SERVICES WILL NOT BE DISPLAYED BY VMBX CONTROL PANEL
        WHEN WE GET THE NEW UI CONCEPT!!!!!! In that case this is not needed
        */

    /*!
     *
     */
    void updatePsServicesToUi();

    /*!
     *
     */
    void updateAllMailboxesToUi();

    /*!
     *
     */
    void updateDefaultMailboxToUi();

private:
	
// From CpCategoryObject

    /*!
     *
     */
    //loadConfigPlugins();

    /*!
     *
     */
    void beforeLoadingConfigPlugins( CpItemDataHelper &itemDataHelper );

    /*!
     *
     */
    //void afterLoadingConfigPlugins();

private:

    /*!
     * 
     * Own.
     */
    VmbxUiEngine* mUiEngine;

    // UI controls

    /*!
     * 
     * Not own.
     */
    CpSettingFormItemData* mEditorCsVoice1;

    /*!
     * 
     * Not own.
     */
    CpSettingFormItemData* mEditorCsVoice2;

    /*!
     * 
     * Not own.
     */
    CpSettingFormItemData* mEditorCsVideo1;

    /*!
     * 
     * Contained pointers not own.
     */
    QList<CpSettingFormItemData*> mPsServiceEditors;
};

#endif // VMBXCPGROUP_H
