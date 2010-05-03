/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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


// PlayStandardTone
#include    <MProfile.h>
#include    <MProfileEngine.h>
#include    <MProfileTones.h>
#include    <TProfileToneSettings.h>
#include    <audiopreference.h>//KAudioPriorityLowLevel

#include    <Mda/Common/Resource.h>//KMdaRepeatForever
#include    <QTimer> // timeout callback
#include    <hbmessagebox.h>//playtone note
#include    <mdaaudiotoneplayer.h>// for CMdaAudioToneUtility&CMdaAudioPlayerUtility
#include    <e32std.h>//TTimeIntervalMicroSeconds

#include "dummyplaytoneprovider.h"

//#include "tflogger.h"


// ----------------------------------------------------------------------------
// DummyPlayToneProvider::DummyPlayToneProvider
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
DummyPlayToneProvider::DummyPlayToneProvider(QObject *parent) :
    QObject(parent)
{
    qDebug("SATAPP: DummyPlayToneProvider::DummyPlayToneProvider >");
    qDebug("SATAPP: DummyPlayToneProvider::DummyPlayToneProvider <");
}

// ----------------------------------------------------------------------------
// DummyPlayToneProvider::~DummyPlayToneProvider
// Sets a pointer to CSatUiViewAppUi object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
DummyPlayToneProvider::~DummyPlayToneProvider()
{
    qDebug("SATAPP: DummyPlayToneProvider::~DummyPlayToneProvider >");
    qDebug("SATAPP: DummyPlayToneProvider::~DummyPlayToneProvider <");
}

//-----------------------------------------------------------------------------
// DummyPlayToneProvider::PlayStandardToneL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyPlayToneProvider::PlayStandardToneL( const TDesC& /*aText*/,
    const TDesC8& /*aSequence*/,
    TTimeIntervalMicroSeconds /*aDuration*/,
    const CFbsBitmap* /*aIconBitmap*/,
    const TBool /*aSelfExplanatory*/ )
{
    qDebug("SATAPP: DummyPlayToneProvider::PlayStandardToneL >");
    qDebug("SATAPP: DummyPlayToneProvider::PlayStandardToneL <");
    return ESatSuccess;
}


// ---------------------------------------------------------
// SatAppCommandHandler::GetProfileParamsL
// Get and store settings from current profile 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void DummyPlayToneProvider::GetProfileParamsL( TSatTone 
                  /*aTone = ESatToneNotSet*/, TDes* /*aToneName = NULL*/ )
{
    qDebug( "SatAppCommandHandler::GetProfileParamsL >");
    qDebug("SatAppCommandHandler::GetProfileParamsL <");
}

// ---------------------------------------------------------
// DummyPlayToneProvider::MatoPrepareComplete
// Indicates success or failure.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void DummyPlayToneProvider::MatoPrepareComplete(TInt /*aError*/)
{
    qDebug("DummyPlayToneProvider::MatoPrepareComplete >");
    qDebug( "DummyPlayToneProvider::MatoPrepareComplete <");
}

//-----------------------------------------------------------------------------
// DummyPlayToneProvider::MatoPlayComplete
// Indicates success or failure.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyPlayToneProvider::MatoPlayComplete( TInt /*aError*/ )
{
    qDebug("DummyPlayToneProvider::MatoPlayComplete >");
    qDebug("DummyPlayToneProvider::MatoPlayComplete <");
}

//-----------------------------------------------------------------------------
// DummyPlayToneProvider::PlayUserSelectedToneL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse DummyPlayToneProvider::PlayUserSelectedToneL(
        const TDesC &/*aText*/,
        TTimeIntervalMicroSeconds /*aDuration*/,
        TSatTone /*aTone*/,
        const CFbsBitmap* /*aIconBitmap*/,
        const TBool /*aSelfExplanatory*/ )
{
    qDebug("DummyPlayToneProvider::PlayUserSelectedToneL >");
    qDebug( "DummyPlayToneProvider::PlayUserSelectedToneL <");
    return ESatSuccess;
}

//-----------------------------------------------------------------------------
// DummyPlayToneProvider::MapcInitComplete
// Plays the tone.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
void DummyPlayToneProvider::MapcInitComplete( TInt /*aError*/, 
    const TTimeIntervalMicroSeconds& /*aDuration*/ )
{
    qDebug( "DummyPlayToneProvider::MapcInitComplete >" );
    qDebug( "DummyPlayToneProvider::MapcInitComplete <" );
}

//-----------------------------------------------------------------------------
// DummyPlayToneProvider::MapcPlayComplete
// Deletes audio player after playing is completed.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
void DummyPlayToneProvider::MapcPlayComplete( TInt /*aError*/ )
{
    qDebug( "DummyPlayToneProvider::MapcPlayComplete >");
    qDebug("DummyPlayToneProvider::MapcPlayComplete <");
}

// ----------------------------------------------------------------------------
// DummyPlayToneProvider::clearScreen
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyPlayToneProvider::clearScreen()
{
    qDebug("SATAPP: SatAppEventProvider::clearScreen >");
    qDebug("SATAPP: DummyPlayToneProvider::clearScreen <");
}

// ----------------------------------------------------------------------------
// DummyPlayToneProvider::clearScreen
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void DummyPlayToneProvider::closeSatUI()
{
    qDebug("SATAPP: DummyPlayToneProvider::closeSatUI >");
    clearScreen();
    qDebug("SATAPP: DummyPlayToneProvider::closeSatUI <");
}

 //End of file
