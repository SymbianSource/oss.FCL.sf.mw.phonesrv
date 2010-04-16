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
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include <voicemailboxdefs.h>

// User includes
#include "vmbxuiengine.h"
#include "loggerutil.h"

/*!
    Constructor
*/
VmbxUiEngine::VmbxUiEngine() :
    mVmbxEngine(NULL)
{ 
    _DBGLOG("VmbxUiEngine::VmbxUiEngine >")
    QT_TRAP_THROWING(mVmbxEngine = CVoiceMailbox::NewL());
    _DBGLOG("VmbxUiEngine::VmbxUiEngine <")
}

/*!
    Destructor
*/
VmbxUiEngine::~VmbxUiEngine()
{
    _DBGLOG( "VmbxUiEngine::~VmbxUiEngine >")
    delete mVmbxEngine;
    mVmbxEngine = NULL;
    _DBGLOG( "VmbxUiEngine::~VmbxUiEngine <")
}

/*!
    Getter method for primary voice number of Voice Mailbox.
*/
void VmbxUiEngine::getCsVoice1Number(QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getCsVoice1Number >")

    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    getNumber(vmbxParams, aValue);
    _DBGLOG2("VmbxUiEngine::getCsVoice1Number < , string=", aValue)
}

/*!
    Getter method for voice ALS number of Voice Mailbox.
*/
void VmbxUiEngine::getCsVoice2Number(QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getCsVoice2Number >")
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine2;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    getNumber(vmbxParams, aValue);
    _DBGLOG2("VmbxUiEngine::getCsVoice2Number < , string=", aValue)
}

/*!
    Getter method for primary video number of Voice Mailbox.
*/
void VmbxUiEngine::getCsVideo1Number(QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getCsVideo1Number >")
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVideo;
    getNumber(vmbxParams, aValue);
    _DBGLOG2("VmbxUiEngine::getCsVideo1Number < , string=", aValue)
}

/*!
    Callback from voice mailbox engine when number has been updated
    This method notifies the UI to refresh its data.
*/
void VmbxUiEngine::uiCsVoice1Changed(const QString& aValue)
{
    _DBGLOG2("VmbxUiEngine::uiCsVoice1Changed >, value=",aValue)
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    setNumber(vmbxParams, aValue);
    _DBGLOG("VmbxUiEngine::uiCsVoice1Changed <");
}

/*!
    Callback from voice mailbox engine when number has been updated
    This method notifies the UI to refresh its data.
*/
void VmbxUiEngine::HandleNotifyL(const CVoiceMailboxEntry &aVmbxEntry)
{
    _DBGLOG( "VmbxUiEngine::HandleNotifyL")
    Q_UNUSED(aVmbxEntry);
    // Consider is there need to pass the actual modified data 
    // and mailbox type enum to slot.
    //
    // Issue the notification request to vmbxengine, that is missing
    // at the moment !
    emit voiceMailboxEngineEntriesUpdated();
}

/*!
    Private getter method for voice mailbox number.
*/  
void VmbxUiEngine::getNumber(const TVoiceMailboxParams &aParam, 
                             QString &aValue)
{
    _DBGLOG("VmbxUiEngine::getNumber >")
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TInt result = mVmbxEngine->GetStoredEntry( aParam, vmbxEntry );
    _DBGLOG2("VmbxUiEngine::getNumber, GetStoredEntry result=", result);
    if ( KErrNone == result && vmbxEntry ){ 
        TPtrC entryNumber( KNullDesC );
        result = vmbxEntry->GetVmbxNumber( entryNumber );
        _DBGLOG3("VmbxUiEngine::getNumber, GetVmbxNumber result=", 
            result,
            " but ignore code and allow returing of an empty string to UI");
        aValue = QString::fromUtf16 (entryNumber.Ptr(), entryNumber.Length());
    }
    delete vmbxEntry;
    vmbxEntry = NULL;
    _DBGLOG2("VmbxUiEngine::getNumber < , string=", aValue)    
}

/*!
    Setter method for voice mailbox number.
*/
void VmbxUiEngine::setNumber(const TVoiceMailboxParams &aParam, 
                             const QString &aValue)
{
    _DBGLOG("VmbxUiEngine::setNumber >")
        
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TInt result = mVmbxEngine->GetStoredEntry(aParam, vmbxEntry);    
    _DBGLOG2("VmbxUiEngine::setNumber, GetStoredEntry result=", result);
    if ( KErrNone != result ){
        QT_TRAP_THROWING(vmbxEntry = CVoiceMailboxEntry::NewL());
        vmbxEntry->SetServiceId(aParam.iServiceId);
        vmbxEntry->SetVoiceMailboxType(aParam.iType);
        vmbxEntry->SetVmbxAlsLineType(aParam.iLineType);
        // Service name for cs is basically the one in service table,
        // but in this case it's not used for anything by vmbx.
        // So let's not set anything as name...fix if problems arise
    }
    _DBGLOG( "VmbxUiEngine::setNumber: cast to TPtrC")
    TPtrC newNumber( 
        reinterpret_cast<const TUint16*>( aValue.utf16() ),
        aValue.length() );
    result = vmbxEntry->SetVmbxNumber( newNumber );
    _DBGLOG2("VmbxUiEngine::setNumber, SetVmbxNumber result=", result);

    if ( KErrNone == result ){
        result = mVmbxEngine->SaveEntry( *vmbxEntry );
        _DBGLOG2("VmbxUiEngine::setNumber, SaveEntry result=", result);
    } 
    delete vmbxEntry;
    // TODO: handle error here
    _DBGLOG("VmbxUiEngine::setNumber <");
}

//End of file
