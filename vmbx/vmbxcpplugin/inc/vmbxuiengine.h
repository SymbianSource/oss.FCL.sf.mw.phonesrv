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
#ifndef VMBXUIENGINE_H
#define VMBXUIENGINE_H

#include <qobject.h>
#include <mvoicemailboxobserver.h>

class CVoiceMailbox;
/*!
 *  VmbxUiEngine

 *  @lib vmbxcpplugin.lib
 *  @since S60 v5.2
 */
class VmbxUiEngine : public QObject, public MVoiceMailboxObserver
{
    Q_OBJECT
    
public:
    /*! Default Voice Mailbox setting values.*/
    enum TUiDefaultVoiceMailboxType
        {
        /*!
         *  None vmbx type.
         */
        EUiDefaultVmbxNone = 0,
        /*!
         *  Voice type.
         */
        EUiDefaultVmbxVoice,
        /*!
         *  Video type.
         */
        EUiDefaultVmbxVideo,
        /*!
         *  Voip type.
         */
        EUiDefaultVmbxPsService
        };
            
public:
    /*!
     *  Constructor.
     */
    VmbxUiEngine();

    /*!
     *  Destructor
     */
    ~VmbxUiEngine();

public:

    /*!
     * Getter method for Voice number of Voice Mailbox.
     *
     * @since S60 v5.2
     * @param aValue Retrieved number.
     */
    void GetCsVoice1Number( QString& aValue );
    /*!
     * Getter method for Voice ALS number of Voice Mailbox.
     *
     * @since S60 v5.2
     * @param aValue Retrieved number.
     */
    void GetCsVoice2Number( QString& aValue );
    
    /*!
     * Getter method for Video number of Voice Mailbox.
     *
     * @since S60 v5.2
     * @param aValue Retrieved number.
     */
    void GetCsVideo1Number( QString& aValue );  
      
    /*!
     * Getter method for default mailbox setting of Voice Mailbox.
     *
     * @since S60 v5.2
     */
    VmbxUiEngine::TUiDefaultVoiceMailboxType DefaultMailboxType();
      
signals:

    /*!
     * Signal emitted when voice mailbox data has been updated.
     *
     * @since S60 v5.2
     */
    void voiceMailboxEngineEntriesUpdated(); 

public slots:

    /*!
     * Slot for signal when number has been edited on UI.
     *
     * @since S60 v5.2
     * @param aValue New value.
     */
     void uiCsVoice1Changed( const QString& aValue );
     
private:
// from MVoiceMailboxObserver
    /*!
     * From MVoiceMailboxObserver 
     * Observer callback for number/address change notification 
     * from voice mailbox engine.
     * The UI should be notified to update the number.
     *
     * @since S60 v5.2
     * @param aVmbxEntry New voicemailbox entry.
     */
    void HandleNotifyL( const CVoiceMailboxEntry& aVmbxEntry );

private:

    /*!
     * Symbian voice mailbox engine which offers basic entry retrieval,
     * saving, querying etc. services.     
     * Own.     
     */
    CVoiceMailbox* mVmbxEngine;
};

#endif // VMBXUIENGINE_H
