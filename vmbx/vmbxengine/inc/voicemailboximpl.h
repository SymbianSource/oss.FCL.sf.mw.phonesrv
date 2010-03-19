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
* Description:  Implementation class for voice mailbox
*  Interface   : Private, CVoiceMailboxImpl
*
*/

#ifndef C_VOICEMAILBOXIMPL_H
#define C_VOICEMAILBOXIMPL_H


// INCLUDES
#include <e32base.h>
#include <cvoicemailbox.h>
#include "mvmbxchangeobserver.h"
#include "mvoicemailboxobserver.h"
#include "mvmbxresourceprovider.h"
#include "mvmbxuiutilities.h"
#include "mvmbxcenrephandler.h"

// FORWARD DECLARATIONS
class CVmbxUiUtilities;
class CVmbxEngineFactory;
class CVmbxObserver;
class CVmbxCenRepHandler;

// CLASS DECLARATION

/**
*  Implemets mailbox entry base class functionality
*
*  @lib vmbxengine.lib
*  @since S60 v5.2
*/
NONSHARABLE_CLASS( CVoiceMailboxImpl ) : public CBase,
                         public MVmbxChangeObserver,
                         public MVmbxResourceProvider
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor
     */
    static CVoiceMailboxImpl* NewL();

    /**
     * Two-phased constructor
     */
    static CVoiceMailboxImpl* NewLC();

    /**
     * Destructor.
     */
    virtual ~CVoiceMailboxImpl();

public: // New functions

    /**
     * see CVoiceMailbox::GetStoredEntry
     * 
     * @since S60 v5.2
     */
    TInt GetStoredEntry( const TVoiceMailboxParams& aParams,
                                    CVoiceMailboxEntry*& aEntry ) const;

    /**
     * see CVoiceMailbox::SaveEntry
     * 
     * @since S60 v5.2
     */
    TInt SaveEntry( const CVoiceMailboxEntry& aEntry );

    /**
     * see CVoiceMailbox::QueryNewEntry
     * 
     * @since S60 v5.2
     */
    TInt QueryNewEntry( const TVoiceMailboxParams& aParams,
                                        CVoiceMailboxEntry*& aEntry );

    /**
     * see CVoiceMailbox::QueryChangeEntry
     * 
     * @since S60 v5.2
     */
    TInt QueryChangeEntry( const TVoiceMailboxParams& aParams,
                                    CVoiceMailboxEntry*& aEntry);

    /**
     * see CVoiceMailbox::QueryVmbxType
     * 
     * @since S60 v5.2
     */
    TInt QueryVmbxType( TVoiceMailboxParams& aParams );

    /**
     * see CVoiceMailbox::NotifyVmbxNumberChangeL
     * 
     * @since S60 v5.2
     */
    void NotifyVmbxNumberChangeL( MVoiceMailboxObserver& aObserver,
                                     const TBool aNotifyOnActiveLineOnly );

    /**
     * see CVoiceMailbox::NotifyVmbxNumberChangeCancel
     * 
     * @since S60 v5.2
     */
    void NotifyVmbxNumberChangeCancel();

    /**
     * see CVoiceMailbox::CheckConfiguration
     * 
     * @since S60 v5.2
     */
    TBool CheckConfiguration( const TVoiceMailboxParams& aParams,
                                 const TInt aFlags );

    /**
     * see CVoiceMailbox::GetServiceIds
     * 
     * @since S60 v5.2
     */
    TInt GetServiceIds( RIdArray& aProfileIds );

    /**
     * see CVoiceMailbox::GetVmbxImage
     * 
     * @since S60 v5.2
     */
    //CGulIcon* GetVmbxImage( const TVoiceMailboxParams& aParams );

    /**
     * see CVoiceMailbox::GetVmbxImages
     * 
     * @since S60 v5.2
     */
    //CArrayPtr<CGulIcon>* GetVmbxImages(
    //                        RArray<TVoiceMailboxParams>& aParams );

    /**
     * see CVoiceMailbox::SaveProvisionedEntry
     * 
     * @since S60 v5.2
     */
    TInt SaveProvisionedEntry( const CVoiceMailboxEntry& aEntry );


public:  // From MVmbxChangeObserver

    /**
     * see MVmbxChangeObserver::SatRefreshL
     * 
     * @since S60 v5.2
     */
    void SatRefreshL();

    /**
     * see MVmbxChangeObserver::CenRepChangedL
     * 
     * @since S60 v5.2
     */
    void CenRepChangedL( TVmbxCenRepKey aId );

    /**
     * see MVmbxChangeObserver::SimStoreChangedL
     * 
     * @since S60 v5.2
     */
    void SimStoreChangedL();

public:  // From MVmbxChangeObserver

    /**
     * Get Handler of VmbxUiUtilities
     *
     * @since S60 v5.2
     * @return Refereance of MVmbxUiUtilities
     */
    MVmbxUiUtilities& VmbxUiUtilities();

    /**
     * Get Handler of VmbxCenrepHandler
     *
     * @since S60 v5.2
     * @return Refereance of MVmbxCenrepHandler
     */
    MVmbxCenrepHandler& VmbxCenRepHandler();


private: //New method

    /**
     * C++ default constructor.
     * 
     */
    CVoiceMailboxImpl();

    /**
     * ConstructL
     *
     * @since S60 v5.2
     */
    void ConstructL();

    /**
     * Retrieves voice mailbox entry for the specified service.
     * Leave - KErrNotFound if no entry available 
     *       - KErrArgument when illegal argument
     *       - Other system wide errors.
     *
     * @since S60 v5.2
     * @param in Params Service id and type of the mailbox for which
     *          a number/address should be defined.
     * @param out aEntry If successful, a new result container is
     *  created and ownership passed to the caller.
     */
    void GetStoredEntryL( const TVoiceMailboxParams& aParams,
                                    CVoiceMailboxEntry*& aEntry ) const;

    /**
     * Displays a query asking the user to define a mailbox number/address.
     * Returns the input which the user entered, but does not save it.
     * For that caller has to use SaveEntry().
     * This method may display related UI notes or queries.
     *
     * If editing for the specified mailbox service is not allowed, 
     * this method displays an error note and returns an error.
     * If client is interested, CheckConfiguration()
     * can be used to check permissions.
     *
     *  Leave  - KErrNotFound if the number is not defined
     *                        (user entered no number or selected Cancel)
     *         - KErrArgument When illegal argument.
     *         - KErrNotSupported if not capable or allowed to execute this
     *              for the specified mailbox.
     *         - Other system wide errors.
     *
     * @since S60 v5.2
     * @param in aParams Service id and type of the mailbox for which
     *          a number/address should be defined.
     * @param out aEntry If successful, a new result container is
     *  created and ownership passed to the caller.
     */
    void QueryNewEntryL( const TVoiceMailboxParams& aParams,
                                        CVoiceMailboxEntry*& aEntry );

    /**
     * Saves the vmbx entry to its storage.
     * Leave - KErrNotSupported if not capable or allowed to execute this
     *              for the specified mailbox service.
     *       - KErrArgument When illegal argument was passed.
     *       - Other system wide errors.
     *
     * Some mailbox types may not support saving all arguments.
     * In that case unsupported arguments are ignored.
     * This method may display related UI notes or queries.
     *
     * @since S60 v5.2
     * @param in aEntry The vmbx entry to save.
     */
    void SaveEntryL( const CVoiceMailboxEntry& aEntry );

    /**
     * Displays a query asking the user to change a mailbox number/address.
     * Returns the input from user, but does not save it.
     * For that caller has to use SaveEntry().
     *
     * If editing for the specified mailbox service is not allowed, 
     * this method displays an error note and returns an error.
     * If client is interested, CheckConfiguration()
     * can be used to check permissions.
     *
     *  created and ownership passed to the caller.
     *           aNumber will contain the vmbx number.
     *  Leave  - KErrCancel If the user selected Cancel.
     *         - KErrArgument When illegal argument.
     *         - KErrNotSupported if not capable or allowed to execute this
     *              for the specified mailbox.
     *         - Other system wide errors.
     *
     * @since S60 v5.2
     * @param in aParams Service id and type of the mailbox.
     * @param out aEntry If successful, a new result container is
     */
    void QueryChangeEntryL( const TVoiceMailboxParams& aParams,
                                    CVoiceMailboxEntry*& aEntry);

    /**
     * Displays a query on the screen asking the user to select a
     * Voice Mailbox service.
     *
     *  Leave  - KErrCancel if user selected Cancel
     *         - KErrNotFound if there is no defined voice number and
     *             user have selected the type to define or the default
     *             type given.
     *         - Other system wide errors.
     *
     * @since S60 v5.2
     * @param out aParams If successful,
     *          contains selected mailbox info.
     */
    void QueryVmbxTypeL( TVoiceMailboxParams& aParams );

    /**
     * Get defined entries via an array
     * Leave with symbian OS error code except KErrNotFound and KErrSupported.
     *
     * @since S60 v5.2
     * @param out aArray array contains the defined entries
     */
    void GetDefinedEntriesL( RPointerArray<CVoiceMailboxEntry>& aArray );

    /**
     * Get defined voice entry via an array
     *  Leave with symbian OS error code except KErrSupported.
     *
     * @since S60 v5.2
     * @param out aArray array contains the defined entries
     */
    void GetDefinedVoiceEntryL( 
                            RPointerArray<CVoiceMailboxEntry>& aArray );

    /**
     * Get defined video entry via an array
     * Leave with symbian OS error code except KErrNotFound and KErrSupported.
     *
     * @since S60 v5.2
     * @param out ay array contains the defined entries
     */
    void GetDefinedVideoEntryL( 
                            RPointerArray<CVoiceMailboxEntry>& aArray );

    /**
     * Get defined voip entries via an array
     * Leave with symbian OS error code except KErrNotFound and KErrSupported.
     *
     * @since S60 v5.2
     * @param out aArray array contains the defined entries
     */
    void GetDefinedVoipEntriesL( 
                            RPointerArray<CVoiceMailboxEntry>& aArray );

    /**
     * Checks the Voice Mailbox how many VoIP service Ids
     * Leave with symbian OS error code
     *
     * @since S60 v5.2
     * @param out aProfileIds
     * @return KErrNone if get service Ids successfully.
     */
    void GetServiceIdsL( RIdArray& aProfileIds );

    /**
     * Retrieves an image associated with the specified mailbox,
     * if an image is available.
     * Image can be displayed e.g. in a selection list.
     * Leave with symbian OS error code.
     *
     * @since S60 v5.2
     * @param in aParams Mailbox service id and type for which image is needed.
     * @return aImage New image. Ownership is transferred.
     */
    //CGulIcon* GetVmbxImageL( const TVoiceMailboxParams& aParams );

    /**
     * Retrieves images for the specified mailboxes.
     * Image can be displayed e.g. in a selection list.
     * Leave with symbian OS error code.
     *
     * If an image for a service is not available,
     * then a NULL pointer is appended to the returned array, to an index
     * corresponding to the index of the service id in the input
     * array aMailboxServices.
     * NULL array pointer is returned if not a signle icon was retrieved
     * successfully.
     *
     * @since S60 v5.2
     * @param in aParams Mailboxes (id and type) for which an image is needed.
     * @return aImage Icons for the specified mailboxes.
     *          Index of an image in the returned array corresponds to the
     *          index of the service id in the input array.
     *          Array and icon ownerships are transferred to caller.
     *          A NULL array pointer is returned if not a single icon was
     *          retrieved successfully.
     */
    //CArrayPtr<CGulIcon>* GetVmbxImagesL(
    //                        RArray<TVoiceMailboxParams>& aParams );

    /**
     * Notify entry changed
     * Leave with symbian OS error code.
     *
     * @since S60 v5.2
     * @param in aType Type of TVmbxType
     */
    void NotifyChangedL( TVmbxType aType);

    /**
     * Saves provisioned entry to corresponding storage.
     *
     * Leave   - KErrNotSupported if not capable or allowed to execute this
     *              for the specified mailbox service.
     *         - KErrArgument When illegal argument was passed.
     *         - Other system wide errors.
     *
     * Some mailbox types may not support saving all arguments.
     * In that case unsupported arguments are ignored.
     *
     * @since S60 v5.2
     * @param in aEntry The provisioned vmbx entry to save.
     */
    void SaveProvisionedEntryL( const CVoiceMailboxEntry& aEntry );

protected: // data

    /**
     * Pointer to CVmbxUiUtilities
     * Own.
     */
    CVmbxUiUtilities* iUiUtilities;

    /**
     * Pointer to CVmbxEngineFactory
     * Own.
     */
    CVmbxEngineFactory* iVmbxFactory;

    /**
     * Observer for user action for saved vmbx number
     * Own.
     */
    MVoiceMailboxObserver* iNotifyCallBack;

    /**
     * Observer for user action for saved vmbx number
     * Own.
     */
    CVmbxObserver* iVmbxObserver;

    /**
     * CVmbxCenRepHandler
     * Own.
     */
    CVmbxCenRepHandler* iCenRepHandler;
    };

#endif  // C_VOICEMAILBOXIMPL_H
