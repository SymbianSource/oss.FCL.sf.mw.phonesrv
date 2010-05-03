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

#include "satappplaytoneprovider.h"
#include "satappcommonconstant.h"
#include "tflogger.h"

//const
// Maximum length for sound file. 
const TInt KMaxSoundFileLength = 256;
// Used when dividing some values. Used only for preventing the use of magic 
// numbers
// Audio sample is repeated indefinitely.
const TInt KSoundPlayerRepeatForever = KMdaRepeatForever;

// ----------------------------------------------------------------------------
// SatAppPlayToneProvider::SatAppPlayToneProvider
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
SatAppPlayToneProvider::SatAppPlayToneProvider(QObject *parent) :
    QObject(parent), mWarningAndPlayTones(false),mVolume(0),
    mPlayer(0),mPlayToneError(ESatSuccess), mTimer(0),mLoop(0),
    mPermanentNote(0), mAudioPlayer(0)
{
    TFLOGSTRING("SATAPP: SatAppPlayToneProvider::SatAppPlayToneProvider call")
    TFLOGSTRING("SATAPP: SatAppPlayToneProvider::SatAppPlayToneProvider exit")
}

// ----------------------------------------------------------------------------
// SatAppPlayToneProvider::~SatAppPlayToneProvider
// Sets a pointer to CSatUiViewAppUi object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
SatAppPlayToneProvider::~SatAppPlayToneProvider()
{
    TFLOGSTRING("SATAPP: SatAppPlayToneProvider::~SatAppPlayToneProvider call")
    stopPlayTone();
    if (mPermanentNote) {
        TFLOGSTRING("SATAPP: SatAppPlayToneProvider::~SatAppPlayToneProvider note 0")
        delete mPermanentNote;
        mPermanentNote = 0;
    }
    if (mTimer) {
        TFLOGSTRING("SATAPP: SatAppPlayToneProvider::~SatAppPlayToneProvider timer 0")
        delete mTimer;
        mTimer = 0;
    }
    if (mLoop) {
        TFLOGSTRING("SatAppPlayToneProvider::~SatAppPlayToneProvider loop 0")
        delete mLoop;
        mLoop = 0;
    }
    TFLOGSTRING("SATAPP: SatAppPlayToneProvider::~SatAppPlayToneProvider exit")
}

//-----------------------------------------------------------------------------
// SatAppPlayToneProvider::PlayStandardToneL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppPlayToneProvider::PlayStandardToneL( const TDesC& aText,
    const TDesC8& aSequence,
    TTimeIntervalMicroSeconds aDuration,
    const CFbsBitmap* /*aIconBitmap*/,
    const TBool aSelfExplanatory )
{
    TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayStandardToneL call")
    if (mTimer) {
        if (mTimer->isActive()) {
            mTimer->stop();
        }
        delete mTimer;
        mTimer = 0;
        TFLOGSTRING("SatAppPlayToneProvider::PlayStandardToneL delete timer")
    }
    if (mLoop) {
        TFLOGSTRING("SatAppPlayToneProvider::PlayStandardToneL delete loop")
        if (mLoop->isRunning()) {
            mLoop->exit();
        }
        delete mLoop;
        mLoop = 0;
    }

    TSatUiResponse response(ESatSuccess);
    unsigned int duration(0);
    if (aDuration > 0) {
        duration = aDuration.Int64() / 1000;
    }
    TFLOGSTRING2("SATAPP: SatAppPlayToneProvider::PlayStandardToneL \
     duration microseconds %d", duration)
    QString text;
    mLoop = new QEventLoop(this);
    if (aText.Length() > 0) {
        text = QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppPlayToneProvider::PlayStandardToneL \
            text=%S", &aText)
        mPermanentNote = new HbMessageBox(HbMessageBox::MessageTypeInformation);
        bool ret = connect(mPermanentNote, SIGNAL(aboutToClose()),
                this, SLOT(stopPlayTone()));
        TFLOGSTRING2("SATAPP: SatAppPlayToneProvider::PlayStandardToneL connect \
           note close to timer stop: %d", ret)

        bool selfExplanatory = aSelfExplanatory;
        if (!selfExplanatory) {
            mPermanentNote->setText(text);
            if (duration > 0 ) {
                mPermanentNote->setTimeout(duration);
            }
            mPermanentNote->setDismissPolicy(HbPopup::TapAnywhere);
            TFLOGSTRING("SatAppPlayToneProvider::PlayStandardToneL show before")
            mPermanentNote->show();
            TFLOGSTRING("SatAppPlayToneProvider::PlayStandardToneL show after")
        }
    }

    if (mPlayer) {
        delete mPlayer;
        mPlayer = 0;
    }
    mPlayer = CMdaAudioToneUtility::NewL(*this);
    TInt volume(mPlayer->MaxVolume());
    mPlayer->SetVolume(volume);
    mPlayer->PrepareToPlayDesSequence(aSequence);

    if (duration > 0) {
        mTimer = new QTimer(this);
        bool ret = connect(mTimer, SIGNAL(timeout()),
                this, SLOT(stopPlayTone()));
        TFLOGSTRING2("SATAPP: SatAppPlayToneProvider::PlayStandardToneL \
                connect mTimer stop: %d", ret)
        mTimer->start(duration);
    }

    if (mLoop) {
        TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayStandardToneL loop")
        mLoop->exec(QEventLoop::AllEvents);
        TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayStandardToneL quit loop")
        delete mLoop;
        mLoop = 0;
    }
    if (mPermanentNote) {
        TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayStandardToneL note 0")
        delete mPermanentNote;
        mPermanentNote = 0;
    }
    if (mTimer) {
        TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayStandardToneL timer 0")
        delete mTimer;
        mTimer = 0;
    }


    TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayStandardToneL before play cancel")
    mPlayer->CancelPlay();
    delete mPlayer;
    mPlayer = NULL;

    //get warning and game tones setting from active profile
    GetProfileParamsL();
    if((EFalse == mWarningAndPlayTones) && 
          (ESatSuccess == mPlayToneError)) {
        mPlayToneError = ESatSuccessToneNotPlayed;
    }
    response = mPlayToneError;
    TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayStandardToneL exit")
    return response;
}


// ---------------------------------------------------------
// SatAppCommandHandler::GetProfileParamsL
// Get and store settings from current profile 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void SatAppPlayToneProvider::GetProfileParamsL( TSatTone aTone /*= ESatToneNotSet*/,
                                         TDes* aToneName /*= NULL*/ )
{
    TFLOGSTRING( "SatAppCommandHandler::GetProfileParamsL calling" )
    MProfile* profile = NULL;
    MProfileEngine* engine = NULL;
    engine = CreateProfileEngineL();
    CleanupReleasePushL( *engine );

    profile = engine->ActiveProfileLC();

    const MProfileTones& tones = profile->ProfileTones();

    // Takes a current warning and gametones setting.
    const TProfileToneSettings& ts = tones.ToneSettings();
    mWarningAndPlayTones = ts.iWarningAndGameTones;
    mVolume = ts.iRingingVolume;
    if ( ( ESatUserSelectedToneIncomingSms == aTone ) && ( aToneName ) )
        {
        TFLOGSTRING("SatAppCommandHandler::GetProfileParamsL message tone")
        aToneName->Copy( tones.MessageAlertTone() );
    } else if( aToneName ) {
        TFLOGSTRING("SatAppCommandHandler::GetProfileParamsL ring tone")
        aToneName->Copy( tones.RingingTone1() );
    }

    CleanupStack::PopAndDestroy(2); // engine, profile
    TFLOGSTRING("SatAppCommandHandler::GetProfileParamsL exits")
}

// ---------------------------------------------------------
// SatAppPlayToneProvider::MatoPrepareComplete
// Indicates success or failure.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void SatAppPlayToneProvider::MatoPrepareComplete(TInt aError)
{
    TFLOGSTRING2("SatAppPlayToneProvider::MatoPrepareComplete called\
        aError = %d", aError)
    if (KErrNone == aError && mPlayer) {
        mPlayToneError = ESatSuccess;
        TTimeIntervalMicroSeconds zero(static_cast<TInt64>( 0 ));
        mPlayer->SetPriority(KAudioPriorityLowLevel,
               STATIC_CAST(TMdaPriorityPreference, KAudioPrefConfirmation));
        mPlayer->SetRepeats( KMdaAudioToneRepeatForever, zero );
        mPlayer->Play();
    } else {
        mPlayToneError = ESatFailure;
    }
    TFLOGSTRING( "SatAppPlayToneProvider::MatoPrepareComplete exit" )
}

//-----------------------------------------------------------------------------
// SatAppPlayToneProvider::MatoPlayComplete
// Indicates success or failure.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppPlayToneProvider::MatoPlayComplete( TInt aError )
{
    TFLOGSTRING("SatAppPlayToneProvider::MatoPlayComplete calling")
    if (KErrNone == aError && mPlayer) {
        TFLOGSTRING("SatAppPlayToneProvider::MatoPlayComplete play")
        mPlayer->Play();
    } else {
        TFLOGSTRING( "SatAppPlayToneProvider::MatoPlayComplete stop playing")
        // Stops playing if error.
        stopPlayTone();
    }
    TFLOGSTRING2("SatAppPlayToneProvider::MatoPlayComplete exit error %d", aError)
}

//-----------------------------------------------------------------------------
// SatAppPlayToneProvider::PlayUserSelectedToneL
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TSatUiResponse SatAppPlayToneProvider::PlayUserSelectedToneL(
        const TDesC &aText,
        TTimeIntervalMicroSeconds aDuration,
        TSatTone aTone,
        const CFbsBitmap* /*aIconBitmap*/,
        const TBool aSelfExplanatory )
{
    TFLOGSTRING("SatAppPlayToneProvider::PlayUserSelectedToneL calling")
    if (mTimer) {
        delete mTimer;
        mTimer = 0;
        TFLOGSTRING("SatAppPlayToneProvider::PlayUserSelectedToneL delete timer")
    }
    if (mLoop) {
        delete mLoop;
        mLoop = 0;
        TFLOGSTRING("SatAppPlayToneProvider::PlayUserSelectedToneL delete loop")
    }
    unsigned int duration(0);
    if (aDuration > 0) {
        duration = aDuration.Int64() / KPlayToneSymbianConvertQtTime;
        TFLOGSTRING2("SatAppPlayToneProvider::PlayUserSelectedToneL duration %d",
                duration)
    }

    QString text;
    if (aText.Length() > 0) {
        text = QString::fromUtf16(aText.Ptr(), aText.Length());
        TFLOGSTRING2("SATAPP: SatAppPlayToneProvider::PlayUserSelectedToneL \
            text=%S", &aText)
        mPermanentNote = new HbMessageBox(HbMessageBox::MessageTypeInformation);
        bool selfExplanatory = aSelfExplanatory;
        if (!selfExplanatory) {
            mPermanentNote->setText(text);
            if (duration) {
                mPermanentNote->setTimeout(duration);
            }
            mPermanentNote->setDismissPolicy(HbPopup::TapAnywhere);
            TFLOGSTRING("SatAppPlayToneProvider::PlayUserSelectedToneL show before")
            mPermanentNote->show();
            TFLOGSTRING("SatAppPlayToneProvider::PlayUserSelectedToneL show after")
        }
    }

    // If several messages/calls arrive during a small amount of time and if the
    // message received or incoming call tone is already being played we do not 
    // restart it.
    if (mAudioPlayer) {
        TFLOGSTRING( "SatAppPlayToneProvider::\
            PlayUserSelectedToneL Error - already played" )
        return ESatFailure;
    }

    TSatUiResponse response(ESatSuccess);
    // This defines name for the current message alert or incoming call tone. 
    TBuf<KMaxSoundFileLength> soundFileName;
    GetProfileParamsL( aTone, &soundFileName );
    // This defines the behaviour to be adopted by an
    // audio client if a higher priority client takes over the device.
    TInt mdaPriority( KAudioPrefIncomingCall );
    // This is used to resolve conflicts when more than one client tries to 
    // access the same sound output device simultaneously.
    TInt audioPriority( KAudioPriorityPhoneCall );

    TFLOGSTRING2( "SATAPP: SatAppPlayToneProvider::PlayUserSelectedToneL\
        Volume is %d ", mVolume )

    // Creates the audio player.
    mAudioPlayer = CMdaAudioPlayerUtility::NewFilePlayerL( 
        soundFileName,
        *this,
        audioPriority,
        ( TMdaPriorityPreference ) mdaPriority );

    if (aDuration > 0) {
        TFLOGSTRING( "SATAPP: SatAppPlayToneProvider::PlayUserSelectedToneL\
         duration not 0" )
        mAudioPlayer->SetRepeats( KSoundPlayerRepeatForever, 
                                  TTimeIntervalMicroSeconds( 
                                  static_cast<TInt64>( 0 ) ) );
        mTimer = new QTimer(this);
        mTimer->start(duration);
        mLoop = new QEventLoop(this);
        bool ret = connect(mTimer, SIGNAL(timeout()), this, SLOT(stopPlayTone()));
        TFLOGSTRING2("SATAPP: SatAppPlayToneProvider::PlayUserSelectedToneL\
                connect mTimer stop: %d", ret)

        if (mPermanentNote) {
            ret = connect(mPermanentNote, SIGNAL(aboutToClose()),
                    this, SLOT(stopPlayTone()));
            TFLOGSTRING2("SATAPP: SatAppPlayToneProvider::PlayUserSelectedToneL \
               connect note close to timer stop: %d", ret)
        }
        if (mLoop) {
            mLoop->exec(QEventLoop::AllEvents);
            TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayUserSelectedToneL quit loop")
            delete mLoop;
            mLoop = 0;
        }
        if (mPermanentNote) {
            TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayUserSelectedToneL note 0")
            delete mPermanentNote;
            mPermanentNote = 0;
        }
        if (mTimer) {
            TFLOGSTRING("SATAPP: SatAppPlayToneProvider::PlayUserSelectedToneL timer 0")
            delete mTimer;
            mTimer = 0;
        }
        // Sample is played in forever loop for the duration.
        // After duration call back timers are destroyed       
        // If player exists, stop playing and delete player. MapcPlayComplete
        // is not called because CallBackTimer stops the playing.
        if (mAudioPlayer) {
            mAudioPlayer->Stop();
            delete mAudioPlayer;
            mAudioPlayer = 0;
            TFLOGSTRING( "SatAppPlayToneProvider::PlayUserSelectedToneL\
                mAudioPlayer deleted" )
        }
    } else {
        // If duration is zero then tone is played only once.
        // Playing duration is same as duration of the sample.
        mAudioPlayer->SetRepeats(
            0, TTimeIntervalMicroSeconds( static_cast<TInt64>( 0 ) ) );
        TFLOGSTRING( "SatAppPlayToneProvider::PlayUserSelectedToneL SetRepeats 0" )
    }

    TFLOGSTRING( "SatAppPlayToneProvider::PlayUserSelectedToneL exit" )
    if( EFalse == mWarningAndPlayTones ) {
        response = ESatSuccessToneNotPlayed;
    }

    TFLOGSTRING( "SatAppPlayToneProvider::PlayUserSelectedToneL exit" )
    return response;
}

//-----------------------------------------------------------------------------
// SatAppPlayToneProvider::MapcInitComplete
// Plays the tone.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
void SatAppPlayToneProvider::MapcInitComplete( TInt aError, 
    const TTimeIntervalMicroSeconds& /*aDuration*/ )
{
    TFLOGSTRING( "SatAppPlayToneProvider::MapcInitComplete called" )
    // Audio player is initialised, so if there is no error, we can start 
    // playing the tone.
    if (KErrNone == aError && mAudioPlayer) {
        const TInt volume( Max(0, Min(mVolume, 
                           mAudioPlayer->MaxVolume())));
        TFLOGSTRING2("CSatUiViewAppUi::\
            MapcInitComplete SetVolume %d", volume )
        // Set volume according Settings. 
        mAudioPlayer->SetVolume( volume );
        mAudioPlayer->Play();
        TFLOGSTRING( "SatAppPlayToneProvider::MapcInitComplete Play" )
    } else {
        // Otherwise, we delete the initialised audio player.
        if ( mAudioPlayer ) {
            TFLOGSTRING( "SatAppPlayToneProvider::MapcInitComplete mAudioPlayer \
                          true" )
            delete mAudioPlayer;
            mAudioPlayer = 0;
     }

        // Remove also the note for play tone
        // If there was a duration for play tone, stop waiting
        stopPlayTone();

        TFLOGSTRING2( "SatAppPlayToneProvider::MapcInitComplete Error %d", aError )
    }
    TFLOGSTRING( "SatAppPlayToneProvider::MapcInitComplete exit" )
}

//-----------------------------------------------------------------------------
// SatAppPlayToneProvider::MapcPlayComplete
// Deletes audio player after playing is completed.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
void SatAppPlayToneProvider::MapcPlayComplete( TInt /*aError*/ )
{
    TFLOGSTRING( "SatAppPlayToneProvider::MapcPlayComplete called" )

    // When playing is completed, deleting the audio player.
    if ( mAudioPlayer ) {
        TFLOGSTRING("SatAppPlayToneProvider::MapcPlayComplete delete mAudioPlayer")
        delete mAudioPlayer;
        mAudioPlayer = 0;
    }
    // Remove note after play has completed.
    stopPlayTone();
    TFLOGSTRING("SatAppPlayToneProvider::MapcPlayComplete exit")
}

// ----------------------------------------------------------------------------
// SatAppPlayToneProvider::clearScreen
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppPlayToneProvider::clearScreen()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::ClearScreen call")
    stopPlayTone();
    TFLOGSTRING("SATAPP: SatAppEventProvider::ClearScreen exit")
}

// ----------------------------------------------------------------------------
// SatAppPlayToneProvider::clearScreen
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppPlayToneProvider::closeSatUI()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::closeSatUI call")
    clearScreen();
    TFLOGSTRING("SATAPP: SatAppEventProvider::closeSatUI exit")
}

// ----------------------------------------------------------------------------
// SatAppPlayToneProvider::stopPlayTone
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void SatAppPlayToneProvider::stopPlayTone()
{
    TFLOGSTRING("SATAPP: SatAppEventProvider::stopPlayTone call")
     if (mPermanentNote) {
        TFLOGSTRING("SatAppPlayToneProvider::stopPlayTone delete mPermanentNote")
        disconnect(mPermanentNote,SIGNAL(aboutToClose()),
                this, SLOT(stopPlayTone())); 
        mPermanentNote->close();
    }
    if (mTimer) {
        TFLOGSTRING("SatAppPlayToneProvider::stopPlayTone delete timer")
        disconnect(mTimer,SIGNAL(timeout()), this, SLOT(stopPlayTone()));
        if (mTimer->isActive()) {
            mTimer->stop();
        }
    }
    if (mLoop && mLoop->isRunning()) {
        TFLOGSTRING("SatAppPlayToneProvider::stopPlayTone exit loop")
        mLoop->exit();
    }
    TFLOGSTRING("SATAPP: SatAppEventProvider::stopPlayTone exit")
}

 //End of file
