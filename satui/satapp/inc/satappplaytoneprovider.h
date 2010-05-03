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
#ifndef SATAPPPLAYTONEPROVIDER_H
#define SATAPPPLAYTONEPROVIDER_H
 
#include <QObject>
#include <MdaAudioTonePlayer.h>//PlayStandardTone
#include <MdaAudioSamplePlayer.h>//PlayUserSelectedTone
#include <QEventLoop>
#include <msatuiobserver.h>


class CMdaAudioToneUtility;
class CMdaAudioPlayerUtility;
class QTimer;
class HbMessageBox;

class SatAppPlayToneProvider: public QObject,
                           public MMdaAudioToneObserver,//PlayStandardTone
                           public MMdaAudioPlayerCallback//PlayUserSelectedTone
{
    Q_OBJECT

public:

    /**
    * Constructor
    */
    SatAppPlayToneProvider(QObject *parent = 0);

    /**
    * Distructor
    */
    virtual ~SatAppPlayToneProvider();

public: // from MSatUiActionImplementer and impletment by Symbian

    /**
    * Handles Standard Tone playing.
    * @param aText The text to be displayed.
    * @param aFileName The name of the file corresponding to the tone.
    * @param aDuration The duration of the tone to be played.
    * @param aIconBitmap The tone playing note icon
    * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
    * @return Information of the operation result.
    */
    virtual TSatUiResponse PlayStandardToneL(
        const TDesC &aText,
        const TDesC8 &aSequence,
        TTimeIntervalMicroSeconds aDuration,
        const CFbsBitmap *aIconBitmap,
        const TBool aSelfExplanatory );

    /**
    * Plays user selected tone.
    * @param aText The text to be displayed.
    * @param aDuration Play duration.
    * @param aTone Tone to be played.
    * @param aIconBitmap The tone playing note icon
    * @param aSelfExplanatory Flag indicating if icon is self-explanatory.
    * @return Information on the operation result.
    */
    virtual TSatUiResponse PlayUserSelectedToneL(
        const TDesC &aText,
        TTimeIntervalMicroSeconds aDuration,
        TSatTone aTone,
        const CFbsBitmap *aIconBitmap,
        const TBool aSelfExplanatory );

    /**
    * ClearScreen
    */
    void clearScreen();

    /**
    * CloseSatUI
    */
    void closeSatUI();

public:// from MMdaAudioToneObserver

    /**
    * From MMdaAudioToneObserver Called by the player object when
    * it has been initialised.
    * @param aError Error type.
    */
    void MatoPrepareComplete( TInt aError );

    /**
    * From MMdaAudioToneObserver Called by the player object when
    * the playback has been terminated.
    * @param aError Error type.
    */
    void MatoPlayComplete( TInt aError );

public://from MMdaAudioPlayerCallback
    /**
    * Initialisation of an audio sample has completed.
    * @param aError The status of the audio sample after initialisation.
    * @param aDuration The duration of the audio sample.
    */
    void MapcInitComplete( 
        TInt aError, 
        const TTimeIntervalMicroSeconds& /*aDuration*/ );

    /**
    * Play of an audio sample has completed.
    * @param aError The status of playback.
    */
    void MapcPlayComplete( TInt aError );

private:// for playTone

    /**
    * Get settings from current profile like volume, warning tones on/off etc.
    * @param aTone, optional specifies the type of needed tone if necessary
    * @param aToneName, optional buffer to get a soundfile name from 
    * the active profile if necessary
    */
    void GetProfileParamsL( TSatTone aTone = ESatToneNotSet,
                                             TDes *aToneName = NULL );

    /**
    * showWaitNote
    */
    void showWaitNote();

private slots:

    void stopPlayTone();

private: // Data

    /**
    * Warning And Play Tones
    */
    bool mWarningAndPlayTones;

    /**
    * Play tone volume
    */
    int mVolume;

    /**
    * Own, Media server interface to play rtf file
    */
    CMdaAudioToneUtility *mPlayer;

    /**
    * Response of Play tone
    */
    TSatUiResponse mPlayToneError;

    /*
     *  Own. PlayTone
     */
    QTimer *mTimer;

    /*
     *  Own. PlayTone 
     */
    QEventLoop *mLoop;

    /*
     *  Own. PlayTone note
     */
    HbMessageBox *mPermanentNote; 

    /*
     *  Own. Player for user selected tones.
     */ 
    CMdaAudioPlayerUtility *mAudioPlayer;

};

#endif    //SATAPPPLAYTONEPROVIDER_H
