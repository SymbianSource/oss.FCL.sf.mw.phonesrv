/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file for the CAiSatEngine class
*
*/


#ifndef CAISATENGINE_H
#define CAISATENGINE_H

// INCLUDE FILES

#include <e32property.h>
#include <SATInternalPSKeys.h>
#include <rsatsession.h>          // For idle mode text and icon  
#include <rsatservice.h>          // For idle mode text and icon
#include <tsaticoninfo.h>

class CAiSatNotifier;
class TAIPluginInitData;
class MPluginAPI;

/**
 *  CAiSatEngine
 *
 *  @since S60 v3.2
 */
class CAiSatEngine : public CBase
    {

public:  // Constructors and destructor

    /**
     * Part of the two phased constuction
     *
     * @param MPluginAPI& aPlugin a ref to a object implementing MPluginAPI
     *       (the object ordering a notification of the content status change)
     * @return Pointer to the created CAiSatEngine object
     */
    static CAiSatEngine* NewL( MPluginAPI& aPlugin );

    /**
     * Destructor
     */
    ~CAiSatEngine();

public: // New functions

    /**
     * Offers the Idle Mode text
     *
     * @param aString Idle mode text for publishing
     */
    void ContentText( TPtrC& aString );
 
    /**
     * Offers the Idle Mode icon
     *
     * @param aGulIcon icon for publishing
     * 
     */    
    void ContentIconL( CGulIcon*& aGulIcon );
    
    /**
     * Formulates response to SatIcon API.
     */
    void HandleNotifyL();
 
    /**
     * Get idle text, icon id, icon qulifier and load icon as bitmap.
     *
     * @param aDupIcon If the icon is the same as previous one.
     * @param aDupText If the text is the same as previous one.
     */    
    void PrepareIdleModeDataL( TBool& aDupIcon, TBool& aDupText );  
            
private: // private construction method

    /**
     * Constructor to use in the object creation. 
     * Initializes the necessary data.
     *
     * @param MPluginAPI& aPlugin a ref to a object implementing MPluginAPI
     *      (the object ordering a notification of the content status change)
     */
    CAiSatEngine( MPluginAPI& aPlugin);

    /**
     * Part of the two phased construction
     */
    void ConstructL();     

private: // New functions

    /**
     * Loads the Idle Mode icon from SatIcon.
     *
     * @param aIconId The icon record id.
     * @param aIconQualifier the icon qualifier.
     * @return The Idle Mode icon id in TInt, the caller need to delete
     *         the pointer.
     */
    CFbsBitmap* LoadIconL( TUint8 aIconId, 
        RSatService::TSatIconQualifier aIconQualifier );  
        
    /**
     * Information of the content availability.
     *
     * @return ETrue if there is content to show, EFalse if not.
     */
    TBool HasContent();    
    
    /**
     * Sends idle information to satIconInfo
     *
     * @param const RSatService::TSATIdleResult& aResponse
     */
    void SendSatResponseL(
        const RSatService::TSATIdleResult& aResponse ) const;                                      
           
private: // Data

    /**
     * Pointer to the Display Text proactive command handler
     * Own 
     */
    CAiSatNotifier* iNotifier;

    /**
     * ref to the class implementing the MPluginAPI interface
     */    
    MPluginAPI& iPlugin;
   
    /**
     * SIM icon buffer
     * Own
     */     
    CFbsBitmap* iIcon;
    
    /**
     * Icon id in previous icon loading (to check if icon is changed)
     */    
    TUint8 iPreviousIconId;
    
    /**
     * SatClient session. 
     */    
    RSatSession iSatSession;
    
    /**
     * SatClient service.
     */    
    RSatService iSatService;
    };

#endif // CAISATENGINE_H
