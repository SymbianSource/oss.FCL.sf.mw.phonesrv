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
* Description:  Base class for different mailbox entries
*  Interface   : Private, CVmbxEngineBase
*
*/


#ifndef C_VMBXENGINEBASE_H
#define C_VMBXENGINEBASE_H

// INCLUDES
#include <e32base.h>
#include <cvoicemailbox.h>

#include "mvmbxresourceprovider.h"

//FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  Implemets mailbox entry base class functionality
*
*  @lib vmbxengine.lib
*  @since S60 v5.2
*/
NONSHARABLE_CLASS( CVmbxEngineBase ) : public CBase
    {

public:  // Constructors and destructor

    /**
     * C++ default constructor.
     * @param aProvider Reference to CVmbxUiUtilities
     */
    CVmbxEngineBase( MVmbxResourceProvider& aProvider );

    /**
     * Destructor.
     */
    virtual ~CVmbxEngineBase();

public: // New functions
    /**
     * To get mailbox entry value
     * Leave if no number got or Symbian OS error code
     *
     * @since S60 v5.2
     * @param aEntry Entry data
     */
    virtual void GetL( CVoiceMailboxEntry*& aEntry ) = 0;

    /**
     * To save mailbox entry value
     * Leave with Symbian OS error code
     *
     * @since S60 v5.2
     * @param aEntry Entry data
     */
    virtual void SaveL( const CVoiceMailboxEntry& aEntry ) = 0;

    /**
     * To save mailbox Provisioned entry value
     * Leave with Symbian OS error code
     *
     * @since S60 v5.2
     * @param aEntry Provisioned Entry data
     */
    virtual void SaveProvisionedEntryL( const CVoiceMailboxEntry& aEntry ) = 0;

    /**
     * Checks the Voice Mailbox how many VoIP service Ids
     * Leave with Symbian OS error code
     *
     * @since S60 v5.2
     * @param aProfileIds id of voip profile
     */
    virtual void GetServiceIdsL( RIdArray& aProfileIds );

    /**
     * see CVoiceMailbox::GetVmbxImage
     * 
     * @since S60 v5.2
     */
    //virtual CGulIcon* GetVmbxImageL( const TVoiceMailboxParams& aParams );

    /**
     * Sets the Voice Mailbox current service id.
     *
     * @since S60 v5.2
     * @param aVmbxServiceId Voice Mailbox service id.
     */
    virtual void SetCurrentServiceId( const TServiceId& aVmbxServiceId );

    /**
     * Checks the Voice Mailbox configuration to find out if a features
     * are disabled or enabled.
     *
     * @since S60 v5.2
     * @param aParams Specifies which mailbox capabilities,
     *          the caller wants to check.
     * @param aFlags Use TVmbxFeatureCapability values for this parameter.
     *          Specifies what features client wants to check.

     * @return True if feature(s) enabled.
     */
    virtual TBool CheckConfiguration( const TVoiceMailboxParams& 
            aParams, const TInt aFlags );
    
    /**
     * Displays number query dialog
     * Leave if user don't select or Symbian OS error code
     *
     * @since S60 v5.2
     * @param aEntry CVoiceMailboxEntry
     */
    virtual void QueryDefineNumberL( CVoiceMailboxEntry& aEntry );

    /**
     * Displays number query dialog
     * Leave if user don't select or Symbian OS error code
     *
     * @since S60 v5.2
     * @param aEntry Entry data
     */
    virtual void QueryChangeNumberL( CVoiceMailboxEntry& aEntry );


protected: // data

    /**
     * Reference to CVmbxUiUtilities
     */
    MVmbxResourceProvider& iProvider;

    /**
     * Mailbox service id.
     */
    TServiceId iVmbxServiceId;
    };

#endif  // C_VMBXENGINEBASE_H
