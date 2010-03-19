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
* Description: Access class to VoIp engine implementation
*  Interface   : Private, CVmbxVoIpEngine
*
*/


#ifndef C_VMBXVOIPENGINE_H
#define C_VMBXVOIPENGINE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CVmbxEngineBase;
//class CGulIcon;
class CVmSPSHandler;
class CVmBSHandler;
class TVmbxServiceInfo;
//class CFbsBitmap;

// CLASS DECLARATION

/**
*  Implements functionality specifig to voip mailbox entry
*
*  @lib vmbxengine.lib
*  @since S60 v5.2
*/
NONSHARABLE_CLASS( CVmbxVoIpEngine ) : public CVmbxEngineBase
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     * @param aProvider Reference to Ui Utilities
     */
    static CVmbxVoIpEngine* NewL( MVmbxResourceProvider& aProvider );


    /**
     * Destructor.
     */
    virtual ~CVmbxVoIpEngine();

public: // from base class CVmbxEntryBase

    /**
     *
     * @since S60 v5.2
     * @see CVmbxEntryBase::GetL
     * @param aEntry Reference to store handler
     */
    void GetL( CVoiceMailboxEntry*& aEntry );

    /**
     *
     * @since S60 v5.2
     * @see CVmbxEntryBase::SaveL
     * @param aEntry Reference to CVoiceMailboxEntry
     */
    void SaveL( const CVoiceMailboxEntry& aEntry );

    /**
     * To save mailbox Provisioned entry value
     *
     * @since S60 v5.2
     * @param aEntry Provisioned Entry data
     */
    void SaveProvisionedEntryL( const CVoiceMailboxEntry& aEntry );

    /**
     * see CVmbxEntryBase::CheckConfiguration
     *
     * @since S60 v5.2
     */   
    TBool CheckConfiguration( const TVoiceMailboxParams& aParams,
                                 const TInt aFlags );

    /**
     * Displays number query dialog
     *
     * @since S60 v5.2
     * @param aEntry CVoiceMailboxEntry
     */
    void QueryDefineNumberL( CVoiceMailboxEntry& aEntry );

    /**
     * Displays number query dialog
     *
     * @since S60 v5.2
     * @param aEntry Entry data
     */
    void QueryChangeNumberL( CVoiceMailboxEntry& aEntry );

private: // New function

    /**
     * C++ default constructor.
     *
     * @since S60 v5.2
     * @param aProvider Reference to Ui Utilities
     */
    CVmbxVoIpEngine( MVmbxResourceProvider& aProvider );

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @since S60 v5.2
    */
    void ConstructL();

    /**
     * Check from SPSettings if there are VoIP profiles
     * @since S60 3.2.3
     * @param None
     * @return ETrue if VoIP profiles were found, otherwise EFalse
     */
    TBool IsVoIPProfilesFound();

    /**
     * Checks the Voice Mailbox how many VoIP service Ids
     *
     * @since S60 v5.2
     * @param aProfileIds
     * @return KErrNone if get service Ids successfully.
     */
    void GetServiceIdsL( RIdArray& aProfileIds );

    /**
     * Get branded icon
     * @since S60 v5.2
     * @param in aBrandingId Brandind Id
     * @param out aBrandedBitmap Bitmap for branded icon
     * @param out aBrandedBitmapMask Mask to branded icon
     */
    //void GetBrandedIconL( const TDesC8& aBrandingId,
    //    CFbsBitmap*& aBrandedBitmap, CFbsBitmap*& aBrandedBitmapMask );

    /**
     * see CVoiceMailbox::GetVmbxImage
     * 
     * @since S60 v5.2
     */
    //CGulIcon* GetVmbxImageL( const TVoiceMailboxParams& aParams );

    /**
     * Get brand id
     * @since S60 v5.2
     * @param in aServiceId Used Service
     * @param out aBrandId On completion, contains Brand Id
     */
    void BrandIdL( TInt aServiceId, TDes8& aBrandId );

    /**
     * Get service ids
     * @since S60 v5.2
     * @param in&out aServiceInfo On completion contains service ids
     */
    TInt GetServiceInfo( TVmbxServiceInfo& aServiceInfo );

private: // data

    /**
     * Handles Service proviver Settings, Own.
     */
    CVmSPSHandler* iVmSpsHandler;
    
    /**
     * Handles Branding Server, Own.
     */
    CVmBSHandler* iVmBsHandler;
    };

#endif  // C_VMBXVOIPENGINE_H
