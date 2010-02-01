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

#include <aicontentpublisher.h>
#include <aipropertyextension.h>
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
class CAiSatPlugin : public CAiContentPublisher,
                     public MAiPropertyExtension,
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

// from base class CAiContentPublisher
  
    /**
     * From CAiContentPublisher
     * The method is called by the framework to request the plug-in free all
     * memory and CPU resources and close all its open files, e.g. the plug-in 
     * should unload its engines due backup operation. The method transits the 
     * plug-in to "Idle" state.
     *
     * @param aReason Reason for state change, see TAiTransitionChange.
     */
    void Stop( TAiTransitionReason aReason );

    /**
     * From CAiContentPublisher
     * The method is called by the framework to instruct plug-in that it is
     * allowed to consume CPU resources, e.g plug-in is able to run timers,
     * perform asynchronous operations, etc. The method transits the plug-in
     * to "Alive" state.
     *
     * @param aReason Reason for state change, see TAiTransitionChange.
     */
    void Resume( TAiTransitionReason aReason );

    /**
     * From CAiContentPublisher
     * The method is called by the framework to instruct plug-in that it is
     * not allowed to consume CPU resources, e.g plug-in MUST stop each
     * timers, cancel outstanding asynchronous operations, etc. The method
     * transits the plug-in to "Suspendend" state.
     *
     * @param aReason Reason for state change, see TAiTransitionChange.
     */
    void Suspend( TAiTransitionReason aReason );

    /**
     * From CAiContentPublisher
     * Adds the content observer / subscriber to plug-in. The plug-in MUST
     * maintain a registry of subscribers and send notification to all them
     * whenever the plug-in changes state or new content available.
     *
     * @param aObserver Content observer to register.
     */
    void SubscribeL( MAiContentObserver& aObserver );
    
    /**
     * From CAiContentPublisher
     * Configures the plug-in.
     * Plug-ins take ownership of the settings array, so it must either
     * store it in a member or free it. Framework has put the array in cleanup
     * stack so the plugin shouldn't do that.
     * If this leaves, the plug-in will be destroyed by AI FW.
     * Plug-in must support LaunchByValue-event even if normal shortcuts don't
     * work. The only allowed serious enough leave is KErrNotFound from CenRep.
     *
     * @param aSettings Setting items defined in the UI definition.
     */
    void ConfigureL( RAiSettingsItemArray& aSettings );

    /**
     * From CAiContentPublisher
     * Returns interface extension. In Series 60 3.1 only event & property
     * extensions are supported. See MAiEventExtension & MAiPropertyExtension
     * interfaces.
     *
     * @param  aUid UID of the extension interface to access.
     * @return The extension interface. Actual type depends on the passed aUid 
     *         argument.
     */
    TAny* Extension( TUid aUid );  

// from base class MAiPropertyExtension

    /**
     * From MAiPropertyExtension.
     * Read property of publisher plug-in.
     *
     * @param aProperty Identification of property.
     * @return Pointer to property value.
     */
    TAny* GetPropertyL( TInt aProperty );

    /**
     * From MAiPropertyExtension.
     * Write property value.
     *
     * @param aProperty Identification of property.
     * @param aValue Contains pointer to property value.
     */
    void SetPropertyL( TInt aProperty, TAny* aValue );
     
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
     * Information about the content publisher (this plug-in)
     */    
    TAiPublisherInfo iInfo;
    
    /**
     * Whether the icon is the same with previous one.
     */      
    TBool iDupIcon;
    
    /**
     * Whether the text is the same with previous one.
     */      
    TBool iDupText;
    };

#endif // CAISATPLUGIN_H


