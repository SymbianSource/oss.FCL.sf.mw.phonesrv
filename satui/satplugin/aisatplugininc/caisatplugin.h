/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Plug-in main class
*
*/


#ifndef CAISATPLUGIN_H
#define CAISATPLUGIN_H

#include <hscontentpublisher.h>
#include <aicontentmodel.h>

#include "mpluginapi.h"
#include "caisatengine.h"

class MAiContentObserver;
class MAiContentItemIterator;

/**
 *  Plug-in main class
 *
 *  @since S60 v3.2
 */
class CAiSatPlugin : public CHsContentPublisher,                     
                     public MPluginAPI
    {

public:

    /**
     * Part of the two phased constuction
     */
    static CAiSatPlugin* NewL();
    
    /**
     * Destructor
     */
    ~CAiSatPlugin();
    
    /**
     * Publishes SAT Set Up Idle Mode Text
     */
    void PublishSatL();

// from base class CHsContentPublisher
  
    /**
     * @see CHsContentPublisher
     */
    void Start( TStartReason aReason );

    /**
     * @see CHsContentPublisher
     */    
    void Stop( TStopReason aReason );
    
    /**
     * @see CHsContentPublisher
     */    
    void Resume( TResumeReason aReason );
    
    /**
     * @see CHsContentPublisher
     */    
    void Suspend( TSuspendReason aReason );
    
    /**
     * @see CHsContentPublisher
     */    
    void SubscribeL( MAiContentObserver& aObserver );
    
    /**
     * @see CHsContentPublisher
     */    
    void ConfigureL( RAiSettingsItemArray& aSettings );
    
    /**
     * @see CHsContentPublisher
     */    
    TAny* GetProperty( TProperty aProperty );
         
    /**
     * Receives a notification of the content update event
     */
    void NotifyContentUpdate();

private:

    /**
     * Constructor to use in the object creation. 
     * Initializes the necessary data.
     */    
    CAiSatPlugin();
    
    /**
     * Part of the two phased construction
     */
    void ConstructL();
    
    /**
     * Update idle area when plug in reload.
     */    
    void UpdateSatL();
           
    /**
     * Publish a icon to the idle area.
     *
     * @param aObserver The Active Idle content observer.
     * @param aIcon The icon to be published.
     * @return The error code.
     */
    TInt PublishIcon( MAiContentObserver& aObserver, CGulIcon* aIcon );
    
    /**
     * Clean setup idle mode icon.
     *
     * @param aObserver The Active Idle content observer.
     * @return The error code.
     */    
    TInt CleanIcon( MAiContentObserver& aObserver );
    
    /**
     * Publish a SetUpIdleModeText string.
     *
     * @param aObserver The Active Idle content observer.
     * @param aContent The SetUpIdleModeText string would be published.
     * @return The error code.
     */
    TInt PublishText( MAiContentObserver& aObserver,
                      const TDesC& aText );
      
    /**
     * Clean setup idle mode text.
     *
     * @param aObserver The Active Idle content observer.
     * @return The error code.
     */     
    TInt CleanText( MAiContentObserver& aObserver );

private: // data

    /**
     * Iterator for plugin content
     * Own
     */
    MAiContentItemIterator* iContent;

    /**
     * Iterator for plugin resources
     * Own
     */    
    MAiContentItemIterator* iResources;

    /**
     * Iterator for plugin events
     * Own
     */    
    MAiContentItemIterator* iEvents;

    /**
     * Plugin engine
     * Own
     */    
    CAiSatEngine* iEngine;

    /**
     * Array of content observers
     */    
    RPointerArray<MAiContentObserver> iObservers;
    
    /**
     * Whether the icon is the same with previous one.
     */      
    TBool iDupIcon;
    
    /**
     * Whether the text is the same with previous one.
     */      
    TBool iDupText;
    
    /**
     * ETrue, if data should be published in Resume()
     */
    TBool iPublishRequired;
    };

#endif // CAISATPLUGIN_H


