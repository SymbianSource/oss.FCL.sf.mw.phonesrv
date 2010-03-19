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
* Description:  public VmbxCenrepHandler APIs for internal class
*  Interface   : Internal, MVmbxCenrepHandler
*
*/


#ifndef M_VMBXCENREPHANDLER_H
#define M_VMBXCENREPHANDLER_H

// INCLUDES

// FORWARD DECLARATION
class CVoiceMailboxEntry;

// CLASS DECLARATION

/**
 *  MVmbxCenrepHandler
 *
 *  @since S60 v5.2
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxCenrepHandler )
    {
    
public:

    /**
     * Get mailbox number from CenRep
     *
     * @since S60 v5.2
     * @param in&out aEntry the voice mailbox information data type
     * @return Symbian OS error code (KErrNone if successful)
     */
    virtual TInt GetVmbxNumber( CVoiceMailboxEntry& aEntry ) = 0;

    /**
     * Saves the mailbox number
     *
     * @since S60 v5.2
     * @param in aEntry vmbx number data to save
     * @return Symbian OS error code (KErrNone if successful)
     */
    virtual TInt Save( const CVoiceMailboxEntry& aEntry ) = 0;

    /**
     * Fetches active store type
     *
     * @since S60 v5.2
     * @return aStoreType Active store in use
     */
    virtual TVmbxMemoryLocation StoreType() = 0;

    /**
     * Returns state of the video support
     *
     * @since S60 v5.2
     * @return ETrue if supported
     */
    virtual TBool VideoSupported() = 0;

    /**
     * Returns ETrue if CenRep setting Sim read only
     *
     * @since S60 v5.2
     * @return ETrue if Sim read only
     */
    virtual TBool IsSimReadOnly() = 0;

     /**
     * Returns ETrue if number allowed to user change
     *
     * @since S60 v5.2
     * @return ETrue if from Sim
     */
    virtual TBool IsAllowedUserEdit() = 0;

    };

#endif // M_VMBXCENREPHANDLER_H
