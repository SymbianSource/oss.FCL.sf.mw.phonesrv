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
* Description:  Voice mailbox engine implementation
*  Interface   : Private, CVmbxCsVoiceEngine
*
*/

#ifndef C_VMBXCSVOICEENGINE_H
#define C_VMBXCSVOICEENGINE_H

// INCLUDES
#include "mvmbxresourceprovider.h"

// FORWARD DECLARATIONS
class CVmbxEngineBase;
class CVmbxSimHandler;


// CLASS DECLARATION
/**
*  Implements functionality specifig to voice mailbox entry
*
*  @lib vmbxengine.lib
*  @since S60 v5.2
*/
NONSHARABLE_CLASS( CVmbxCsVoiceEngine ) : public CVmbxEngineBase
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     * @since S60 v5.2
     * @param aProvider Reference to MVmbxResourceProvider.
     * @return New instance of the object. Ownership transferred.
     */
    static CVmbxCsVoiceEngine* NewL( MVmbxResourceProvider& aProvider );

    /**
     * Destructor.
     */
    virtual ~CVmbxCsVoiceEngine();

// from base class CVmbxEntryBase

    /**
     * @see CVmbxEntryBase::GetL
     *
     * @since S60 v5.2
     * @param aEntry Reference to CVoiceMailboxEntry
     */
    void GetL( CVoiceMailboxEntry*& aEntry );

    /**
     * @see CVmbxEntryBase::SaveL
     *
     * @since S60 v5.2
     * @param aEntry 
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
     * @see CVoiceMailbox::CheckConfiguration
     *
     * @since S60 v5.2
     * @param aParams Specifies which mailbox capabilities,
     *          the caller wants to check.
     * @param aFlags Use TVmbxFeatureCapabilityFlag values for this parameter.
     *          Specifies what features client wants to check.

     * @return True if feature(s) enabled.
     */
    TBool CheckConfiguration( const TVoiceMailboxParams& aParams,
                                 const TInt aFlags );

    /**
     * Check Entry writable
     *
     * @since S60 v5.2
     * @param aParams params of Entry data
     * @return ETrue if entry is writable
     */
    TBool IsWritable( const TVoiceMailboxParams& aParams );
 
private:

    /**
     * C++ default constructor.
     *
     * @since S60 v5.2
     * @param aProvider Reference to MVmbxResourceProvider
     */
    CVmbxCsVoiceEngine( MVmbxResourceProvider& aProvider );

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @since S60 v5.2
     */
    void ConstructL();

    /**
     * Queries where to save and then saves
     *
     * @since S60 v5.2
     * @param in aEntry Entry data
     */
    void SaveEntryToPhoneL( const CVoiceMailboxEntry& aEntry );

    /**
     * For saving to place that user selects
     *
     * @since S60 v5.2
     * @param in aEntry Entry data
     */
    void SaveEntryToSimL( const CVoiceMailboxEntry& aEntry );

    /**
     * Returns Sim writable or not
     *
     * @since S60 v5.2
     * @return ETrue if writable
     */
    TBool IsSimWritable();

private: // data

    /**
     * CVmbxSimHandler
     * Own.
     */
    CVmbxSimHandler* iSimHandler;
    };

#endif  // C_VMBXCSVOICEENGINE_H
