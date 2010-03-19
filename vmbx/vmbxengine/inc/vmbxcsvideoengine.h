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
* Description:  Video mailbox engine implementation
*  Interface   : Private, CVmbxCsVideoEngine
*
*/

#ifndef C_VMBXCSVIDEOENGINE_H
#define C_VMBXCSVIDEOENGINE_H

// INCLUDES
#include "mvmbxresourceprovider.h"

// FORWARD DECLARATIONS
class CVmbxEngineBase;


// CLASS DECLARATION

/**
*  Implements functionality specifig to video mailbox implementation
*
*  @lib vmbxengine.lib
*  @since S60 v5.2
*/
NONSHARABLE_CLASS( CVmbxCsVideoEngine ) : public CVmbxEngineBase
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     * @param in aProvider Reference to Ui Utilities
     * @return New instance of the object. Ownership transferred.
     */
    static CVmbxCsVideoEngine* NewL( MVmbxResourceProvider& aProvider );

    /**
     * Destructor.
     */
    virtual ~CVmbxCsVideoEngine();

// from base class CVmbxEntryBase

    /**
     * @see CVmbxEntryBase::GetL
     *
     * @since S60 v5.2
     * @param in aEntry a pointer reference of CVoiceMailboxEntry
     */
    void GetL( CVoiceMailboxEntry*& aEntry );

    /**
     * @see CVmbxEntryBase::SaveL
     *
     * @since S60 v5.2
     * @param aEntry Reference to CVoiceMailboxEntry
     */
    void SaveL( const CVoiceMailboxEntry& aEntry );

    /**
     * To save mailbox Provisioned entry value
     *
     * @since S60 v5.2
     * @param in aEntry Reference to CVoiceMailboxEntry
     */
    void SaveProvisionedEntryL( const CVoiceMailboxEntry& aEntry );

private:

    /**
     * C++ default constructor.
     *
     * @since S60 v5.2
     * @param in aProvider Reference to Ui Utilities
     */
    CVmbxCsVideoEngine( MVmbxResourceProvider& aProvider );

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @since S60 v5.2
     */
    void ConstructL();

    };

#endif  // C_VMBXCSVIDEOENGINE_H
