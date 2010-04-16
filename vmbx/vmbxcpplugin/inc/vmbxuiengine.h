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
#ifndef VMBXUIENGINE_H
#define VMBXUIENGINE_H

// System includes
#include <QObject>
#include <mvoicemailboxobserver.h>

// Forward declarations
class CVoiceMailbox;
class TVoiceMailboxParams;

// Class declaration
class VmbxUiEngine : public QObject, public MVoiceMailboxObserver
{
    Q_OBJECT
            
public:
    /*!
        Constructor.
    */
    VmbxUiEngine();

    /*!
        Destructor
    */
    ~VmbxUiEngine();

public:

    /*!
        Getter method for primary voice number of Voice Mailbox.
        @param aValue Retrieved number.
    */
    void getCsVoice1Number(QString &aValue);
    /*!
        Getter method for voice ALS number of Voice Mailbox.
        @param aValue Retrieved number.
    */
    void getCsVoice2Number(QString &aValue);
    
    /*!
        Getter method for primary video number of Voice Mailbox.
        @param aValue Retrieved number.
    */
    void getCsVideo1Number(QString &aValue);
      
    // Add new Getter methods for other voice mailbox types
    
    // From MVoiceMailboxObserver
    /*!
        From MVoiceMailboxObserver
        Observer callback for number/address change notification
        from voice mailbox engine.
        The UI should be notified to update the number.
        @param aVmbxEntry New voicemailbox entry.
    */
    void HandleNotifyL(const CVoiceMailboxEntry &aVmbxEntry);

signals:

    /*!
        Signal emitted when voice mailbox data has been updated.
    */
    void voiceMailboxEngineEntriesUpdated(); 

public slots:

    /*!
        Slot for signal when number has been edited on UI.
        @param aValue New value.
    */
    void uiCsVoice1Changed( const QString &aValue );
     
private: // New Method    
    
    /*!
        Getter method for voice mailbox number.
        @param aParam Identify voice mailbox type
        @param aValue Retrieved number.
    */    
    void getNumber(const TVoiceMailboxParams &aParam, 
                   QString &aValue);
    
    /*!
        Setter method for voice mailbox number.
        @param aParam Identify voice mailbox type
        @param aValue Retrieved number.
    */
    void setNumber(const TVoiceMailboxParams &aParam, 
                   const QString &aValue);

private:

    /*!
        Symbian voice mailbox engine which offers basic entry retrieval,
        saving, querying etc. services.
        Own.
    */
    CVoiceMailbox *mVmbxEngine;
};

#endif // VMBXUIENGINE_H
