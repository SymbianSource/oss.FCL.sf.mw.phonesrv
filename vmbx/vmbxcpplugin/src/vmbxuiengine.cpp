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
 
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include <QtGlobal>
#include <QVariant>
#include "vmbxuiengine.h"
#include "loggerutil.h"


// -----------------------------------------------------------------------------
// Comment me!
// -----------------------------------------------------------------------------
//
VmbxUiEngine::VmbxUiEngine() :
    mVmbxEngine( 0 )
{ 
    _DBGLOG("VmbxUiEngine::VmbxUiEngine>")

    // TODO: test that leave vs thrown exception really works
    //mVmbxEngine = QT_TRAP_THROWING( CVoiceMailbox::NewL() ); this available in qt 4.6  
    // TODO: replace all this trapping&throwing using QT_TRAP_THROWING when qt 4.6
    TRAPD( error, mVmbxEngine = CVoiceMailbox::NewL(); );
    if ( error || !mVmbxEngine ) {
        QT_THROW( 0 );
    }
    _DBGLOG("VmbxUiEngine::VmbxUiEngine <")
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
VmbxUiEngine::~VmbxUiEngine()
{
    _DBGLOG( "VmbxUiEngine::~VmbxUiEngine >")
    delete mVmbxEngine;
    // TODO: check cleanup, this is a first draft!
    _DBGLOG( "VmbxUiEngine::~VmbxUiEngine <")
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void VmbxUiEngine::GetCsVoice1Number( QString& aValue )
{
    _DBGLOG("VmbxUiEngine::GetCsVoice1Number >")

    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    TInt result = mVmbxEngine->GetStoredEntry( vmbxParams, vmbxEntry );
    _DBGLOG2("VmbxUiEngine::GetCsVoice1Number, GetStoredEntry result=", result);
    if ( KErrNone == result && vmbxEntry )
        { 
        TPtrC entryNumber( KNullDesC );
        result = vmbxEntry->GetVmbxNumber( entryNumber );
        _DBGLOG3("VmbxUiEngine::GetCsVoice1Number, GetVmbxNumber result=", 
            result,
            " but ignore code and allow returing of an empty string to UI");
        aValue = QString( (QChar*)entryNumber.Ptr(), entryNumber.Length() );
        }

    _DBGLOG2("VmbxUiEngine::GetCsVoice1Number < , string=", aValue)
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void VmbxUiEngine::GetCsVoice2Number( QString& aValue )
{
    _DBGLOG("VmbxUiEngine::GetCsVoice2Number >")
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine2;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    TInt result = mVmbxEngine->GetStoredEntry( vmbxParams, vmbxEntry );
    _DBGLOG2("VmbxUiEngine::GetCsVoice2Number, GetStoredEntry result=", result);
    if ( KErrNone == result && vmbxEntry )
        { 
        TPtrC entryNumber( KNullDesC );
        result = vmbxEntry->GetVmbxNumber( entryNumber );
        _DBGLOG3("VmbxUiEngine::GetCsVoice2Number, GetVmbxNumber result=", 
            result,
            " but ignore code and allow returing of an empty string to UI");
        aValue = QString( (QChar*)entryNumber.Ptr(),entryNumber.Length() );
        }
    _DBGLOG2("VmbxUiEngine::GetCsVoice2Number < , string=", aValue)
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void VmbxUiEngine::GetCsVideo1Number( QString& aValue )
{
    _DBGLOG("VmbxUiEngine::GetCsVideo1Number >")
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVideo;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVideo;
    TInt result = mVmbxEngine->GetStoredEntry( vmbxParams, vmbxEntry );
    _DBGLOG2("VmbxUiEngine::GetCsVideo1Number, GetStoredEntry result=", result);
    if ( KErrNone == result && vmbxEntry )
        { 
        TPtrC entryNumber( KNullDesC );
        result = vmbxEntry->GetVmbxNumber( entryNumber );
        _DBGLOG3("VmbxUiEngine::GetCsVideo1Number, GetVmbxNumber result=", 
            result,
            " but ignore code and allow returing of an empty string to UI");
        aValue = QString( (QChar*)entryNumber.Ptr(),entryNumber.Length() );
        }
    _DBGLOG2("VmbxUiEngine::GetCsVideo1Number < , string=", aValue)
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
VmbxUiEngine::TUiDefaultVoiceMailboxType VmbxUiEngine::DefaultMailboxType()
{
    _DBGLOG( "VmbxUiEngine::DefaultMailboxType")
    TUiDefaultVoiceMailboxType result = EUiDefaultVmbxNone;
    // TODO: get type from engine IF AND WHEN THIS FEATURE IS SUPPORTED
    _DBGLOG2("VmbxUiEngine::GetCsVideo1Number < , string=", result)
    return result;
}

// -----------------------------------------------------------------------------
// Callback from voice mailbox engine when number has been updated
// This method notifies the UI to refresh its data. 
// -----------------------------------------------------------------------------
//
void VmbxUiEngine::HandleNotifyL( const CVoiceMailboxEntry& /*aVmbxEntry*/ )
{
    _DBGLOG( "VmbxUiEngine::HandleNotifyL")
    /* TODO:  consider is there need to pass the actual modified data 
    and mailbox type enum to slot.
    
    TODO: issue the notification request to vmbxengine, that is missing
    at the moment !
    */
    emit voiceMailboxEngineEntriesUpdated();
}

// -----------------------------------------------------------------------------
// Callback from voice mailbox engine when number has been updated
// This method notifies the UI to refresh its data. 
// -----------------------------------------------------------------------------
//
void VmbxUiEngine::uiCsVoice1Changed( const QString& aValue )
{
    _DBGLOG2("VmbxUiEngine::uiCsVoice1Changed, value=",aValue)
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    vmbxParams.iType = EVmbxVoice;
    vmbxParams.iLineType = EVmbxAlsLine1;
    vmbxParams.iServiceId = KVmbxServiceVoice;
    TInt result = mVmbxEngine->GetStoredEntry( vmbxParams, vmbxEntry );
    _DBGLOG2("VmbxUiEngine::uiCsVoice1Changed, GetStoredEntry result=", result);
    if ( KErrNone != result )
        {
        // TODO: test that leave vs thrown exception really works
        //vmbxEntry = QT_TRAP_THROWING( CVoiceMailboxEntry::NewL() ); this available in qt 4.6  
        // TODO: replace all this trapping using QT_TRAP_THROWING when qt 4.6
        TRAP_IGNORE( vmbxEntry = CVoiceMailboxEntry::NewL() );
        vmbxEntry->SetServiceId( KVmbxServiceVoice );
        vmbxEntry->SetVoiceMailboxType( EVmbxVoice );
        vmbxEntry->SetVmbxAlsLineType( EVmbxAlsLine1 );
        // Service name for cs is basically the one in service table,
        // but in this case it's not used for anything by vmbx.
        // So let's not set anything as name...fix if problems arise
        }
    _DBGLOG( "VmbxUiEngine::uiCsVoice1Changed: cast to TPtrC")
    TPtrC newNumber( 
        reinterpret_cast<const TUint16*>( aValue.utf16() ),
        aValue.length() );
    result = vmbxEntry->SetVmbxNumber( newNumber );
    _DBGLOG2("VmbxUiEngine::uiCsVoice1Changed, SetVmbxNumber result=", result);

    if ( KErrNone == result )
        {
        result = mVmbxEngine->SaveEntry( *vmbxEntry );
        } 
    delete vmbxEntry;
    // TODO: handle error here
    _DBGLOG2("VmbxUiEngine::uiCsVoice1Changed <, SaveEntry result=", result);
}
