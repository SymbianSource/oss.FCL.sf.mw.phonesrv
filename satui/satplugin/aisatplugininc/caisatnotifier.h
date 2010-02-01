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
* Description:  The handler for the SAT Plug-in
*                Set Up Idle Mode Text proactive command.
*
*/


#ifndef CAISATNOTIFIER_H
#define CAISATNOTIFIER_H

//  INCLUDES
#include "mpluginapi.h" // Needed in cpp and header

class RSatSession;
class RSatService;

// CLASS DECLARATION

/**
 *  This is the handler for the Set Up Idle Mode Text proactive command.
 *  This active objects is registered with ETel SAT Api to receive notifications
 *  about the incoming Display Text commands.
 *
 *  @since S60 v3.2
 *
 */
class CAiSatNotifier : public CActive
    {
public: // Constructors and destructor

    /**
     * Part of the two phased constuction
     *
     * @param MPluginAPI& aPlugin a ref to a object implementing MPluginAPI
     *        (the object ordering a notification of the content status change)
     * @param RSatService& aSatService a ref to RSatService
     * @return Pointer to the created CAiSatNotifier object
     */
    static CAiSatNotifier* NewL( MPluginAPI& aPlugin,
                                 RSatService& aSatService );

    /**
     * Destructor
     */
    virtual ~CAiSatNotifier();

    /**
     * Starts listening for the Sat command.
     * @return Error code.
     */
    TInt Start();
    
    /**
     * Whether the P&S has content or not.
     * @return whether has content or not.
     */    
    TBool HasContent();    

    /**
     * Get Idle mode data to member: string, icon qualifier, icon id.
     * 
     * @param aDupIcon Whether the icon is the same with previous one
     * @param aDupText Whether the icon is the same with previous one
     */    
    void GetIdleModeDataL( TBool& aDupIcon, TBool& aDupText );

    /**
     * Get idle text string
     *
     * @param aString Idle text string.
     */
    void GetIdleTextString( TPtrC& aString  );

    /**
     * Idle icon record id
     *
     * @return Icon record id
     */
    TUint8 IdleIconId();

    /**
     * Idle icon qualifier
     *
     * @return Icon qualifier
     */
    RSatService::TSatIconQualifier IdleIconQualifier();

protected: // From base class CActive

    /**
     * From CActive
     * Handles the request completion.
     */
    void RunL();

    /**
     * From CActive
     * Implements the cancel protocol.
     */
    void DoCancel();

private: // Constructors
    
    /**
     * Constructor to use in the object creation. 
     * Initializes the necessary data.
     *
     * @param aPlugin Ref to a object implementing MPluginAPI
     * (the object ordering a notification of the content status change)
     * @param aSatService Ref to a RSatService
     */
    CAiSatNotifier( MPluginAPI& aPlugin,
                    RSatService& aSatService );

    /**
     * To continue listening to the Sat command.
     */
    void Activate();

private: // Data

    /**
     * Idle mode text.
     * Own
     */
    HBufC* iString;  
    
    /**
     * Idle mode icon id.
     */
	TUint8 iRecordId;

	/**
	 * Idle mode icon qualifier.
	 */
	RSatService::TSatIconQualifier iIconQualifier;
	
	/**
	 * CAiSatPlugin reference.
	 */
    MPluginAPI& iPlugin;

    /**
     * RSatService reference from CAiSatEngine.
     */
    RSatService& iSatService;

	/**
	 * Identify the first start up.
	 */    
    TBool iFirstStartup;
    
    };

#endif // CAISATNOTIFIER_H

