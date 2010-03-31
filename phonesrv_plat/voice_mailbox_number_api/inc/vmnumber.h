/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for fetching/saving the vmbx number and querying
*                for it.
*                Also notify on a number change is supported.
*
*/


#ifndef VMNUMBER_H
#define VMNUMBER_H

// INCLUDES
/*****************************************************
*  Series 60 Customer / ETel
*  Series 60 ETel API
*****************************************************/
#include <etelmm.h>
#include <etelsat.h>
#include <mmtsy_names.h>

#include <PhCltTypes.h>         // from PhoneClient

#include <MSSSettingsObserver.h>
#include <MSatRefreshObserver.h>
#include <RSatSession.h>
#include <RSatRefresh.h>
#include <centralrepository.h>
#include <CenRepNotifyHandler.h>
#include <aknappui.h>
#include <AknQueryDialog.h>
#include <CPhCltEmergencyCall.h>     // CPhCltEmergencyCall
#include <MPhCltEmergencyCallObserver.h>

// FORWARD DECLARATIONS

class CVmSPSHandler;

// CONSTANTS


_LIT( KEmptyVoiceMailNumber, "" );

enum TVmbxQueryType
    {
    EVmbxNotDefinedQuery,   // use this
    EVmbxChangeNumberQuery  // this is only for vmbx application
    };

enum TVmbxNumberEntry
    {
    EAlsActiveLineEntry = 0,    // currently active ALS line
    EAlsLine1Entry = 1,         // ALS primary line
    EAlsLine2Entry = 2          // ALS auxiliary line
    };

enum TVmbxAlsLine
    {
    EAlsLine1 = 1,
    EAlsLine2 = 2
    };

enum TVmbxPhonebookOperation
    {
    EVmbxPhonebookGetInfo,
    EVmbxPhonebookRead,
    EVmbxPhonebookWrite,
    EVmbxPhonebookDelete
    };


enum TVmbxMemoryLocation
    {
    EVmbxSimMemory = 0,
    EVmbxPhoneMemory
    };

_LIT( KVmbxPanicCategory, "RVmbxNumber");

enum TVmbxPanic
    {
    EVmbxNotConnected       = 0,
    EVmbxDescriptorTooSmall = 1,
    EVmbxNumberTooLong      = 2,
    EVmbxNoNotifyHandler    = 3,
    EVmbxNoCoeEnv           = 4,
    EVmbxUnhandledOperation = 5
    };

enum TVmbxSimNumAcceptance
    {
    EVmbxSimNumNotAccepted = 0,
    EVmbxSimNumAccepted
    };

enum TVmbxUserEditIndicator
    {
    EVmbxNotUserEdit = 0,
    EVmbxUserEdit,
    EvmbxPhoneMemForcedEdit
    };

const TInt KVmbxMaxNumberLength = KPhCltTelephoneNumberLength;
const TInt KVmbxMaxAddressLength = 100;

// Phonebook entry name length
const TInt KVmbxPhoneBookEntryNameLength = 5;

// Branding id
const TInt KVmSettingsUiBrandingIdLength = 512;

// Publish & Subscribe property category definition

enum TVmbxNumType
    {
    EVmbxMDN,
    EVmbxMIN
    };

enum TVmbxType
    {
    EVmbx,
    EVmbxIP,
    EVmbxVideo,
    EVmbxNone
    };

enum TVmbxTitle
    {
    EVmbxCall,
    EVmbxDefine
    };

// Phonebook types (VMBX = 6f17)
enum TVmSimPhoneBookTypes
    {
    EVMBXPhoneBook = 0,
    EMBDNPhoneBook
    };


// FORWARD DECLARATIONS
class CVmTextQueryDialog;
class CCoeEnv;
class CVPbkContactManager;
class CPhCntSingleItemFetch;
class CVMSimQueryDilaog;
class CVmListQueryDialog;
class CVmBSHandler;
// CLASS DECLARATION

/**
*  TVmbxEntry
*  Contains vmbx sim entry data.
*
*  @lib vmbx.lib
*  @since 2.0
**/
class TVmbxEntry
    {
    public: // Constructors and destructor

        /**
        * C++ default constructor.
        */
        TVmbxEntry() : iIndex( 0 )
            {
            iTelNumber.FillZ();
            iTelNumber.Zero();
            };

    public: // Data

        // Data index
        TInt iIndex;

        // Telefony number
        TBuf16<KVmbxMaxNumberLength> iTelNumber;

        // Phonebook entry name
        TBuf16<KVmbxPhoneBookEntryNameLength> iText;
    };

/**
*  MVmbxNotifyHandler
*
*  @lib vmbx.lib
*  @since
**/
class MVmbxNotifyHandler
    {
    public:
        /**
        * Does the action user wants when a vmbx number has been changed.
        * If the parameter aNotifyOnActiveLineOnly was ETrue in notify request,
        * only a change of the vmbx number of the active ALS line causes
        * calling this.
        *
        * @param aLine ALS line of which number was changed ( 1 or 2 )
        * @param aValue the new vmbx number of that line.
        * (this reference is only valid in scope of this method)
        */
        virtual void HandleNotifyL( TInt aLine, const TDesC& aValue ) = 0;
    };


/**
 *  Observer interface for ServiceNotifier.
 *
 *  @since S60 3.2
 **/
class MServiceNotifyHandler
    {
    public:

        /**
        * Called when a Service profile has been changed.
        */
        virtual void HandleServiceNotifyL() = 0;
    };

/**
*  MVmbxAlsObserver
*
*  @lib vmbx.lib
*  @since
**/
class MVmbxAlsObserver
    {
    public:
        /**
        * Does the action user wants when the active ALS line has been changed.
        *
        * @param aLine new ALS line number ( 1 or 2 )
        **/
        virtual void HandleAlsChangeL( TVmbxAlsLine aLine ) = 0;

    };


/**
 *  TVmbxServiceInfo, contains Service information.
 *
 *  @since S60 3.2
 **/
class TVmbxServiceInfo
    {
    public: // Constructors and destructor

        TVmbxServiceInfo()
            {
            iServiceId = 0;
            iSnapId    = 0;
            }

    public: // Data

        // Service's Id
        TUint       iServiceId;

        // Service's SNAP Id
        TUint32     iSnapId;

        // Service's name.
        TBuf<64>    iName;

        // Service's address.
        TBuf<KVmbxMaxAddressLength> iAddress;
    };


/**
 *  TVmbxServiceSelection, contains selected service information.
 *
 *  @since S60 3.2
 **/
class TVmbxServiceSelection
    {
    public: // Constructors and destructor

        TVmbxServiceSelection( TVmbxType aServiceType = EVmbxNone,
                               TUint aServiceId = 0 )
            {
            iServiceType = aServiceType;
            iServiceId   = aServiceId;
            }

    public: // Data

        // Service type
        TVmbxType   iServiceType;

        // Service Id
        TUint      iServiceId;

    };


/**
*  RVmbxNumber
*
*  @lib vmbx.lib
*  @since
**/
class RVmbxNumber : public MCenRepNotifyHandlerCallback,
                    public MSSSettingsObserver,
                    public MSatRefreshObserver,
                    public MPhCltEmergencyCallObserver
    {
    public:
        /**
        * status for QueryNumberL.
        * EVmQueryNormal Query number running normal case
        * EVmQueryProcess Query number running FOREVER case
        */
        enum TVmVmQueryStatusType
            {
            EVmQueryNormal = 0,
            EVmQueryProcess
            };

    public:

        /**
        * C++ default constructor.
        */
        IMPORT_C RVmbxNumber();

        /**
        * Destructor.
        */
        IMPORT_C ~RVmbxNumber();

    public: // New functions

        /**
        * Opens the resources needed. This or the other overloaded Open() has to
        * be called successfully before doing anything else.
        *
        * ----------------------------------------------------------------------
        * A new ETel connection is opened when using this method!
        * ----------------------------------------------------------------------
        *
        * @return Symbian OS error code (KErrNone is successful)
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        IMPORT_C TInt Open();

        /**
        * Opens the resources needed. This or the other overloaded Open() has to
        * be called successfully before doing anything else.
        *
        * Use this overload if you already have ETel connection.
        *
        * ----------------------------------------------------------------------
        * An existing ETel connection is used when using this method!
        * ----------------------------------------------------------------------
        *
        * @param aPhone Phone client
        * @return Symbian OS error code (KErrNone is successful)
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        IMPORT_C TInt Open( RMobilePhone& aPhone );

        /**
        * Fetches the specified vmbx number. If there is no vmbx number
        * saved for the specified ALS line, parameter aNumber remains unchanged
        * and the return value is KErrNotFound.
        *
        * @param aNumber vmbx number is saved here (must be room for at least
        *                KVmbxMaxNumberLength characters)
        *
        * @param aEntry determines which vmbx number entry is used:
        *               EAlsLine1Entry = vmbx number entry of primary line
        *               EAlsLine2Entry = vmbx number entry of secondary line
        *               EAlsActiveLineEntry = entry of currently active line
        *
        * @return - KErrNone if the number was found.
        *           aNumber will contain the vmbx number.
        *
        *         - KErrNotFound if the number is not defined
        *
        *         - Other error code if there was a problem in getting
        *           the number. Clients should show error note if this happens.
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        IMPORT_C TInt GetVmbxNumber( TDes& aNumber,
                            TVmbxNumberEntry aEntry = EAlsActiveLineEntry );

        /**
        * Check wether the voice mailbox number can be changed. If the number
        * cannot be edited, the QueryNumberL leaves with and SaveVmbxNumber
        * returns KErrAccessDenied.
        * @since 2.7
        * @return ETrue if editing is allowed.
        *         EFalse if editing is forbidden.
        */
        IMPORT_C TBool AllowedToChangeTheNumber();

        /**
        * Puts a query on the screen asking for a voice mailbox number.
        *
        * IMPORTANT:
        *
        * Other subsystems than voice mailbox application should call this
        * ONLY IF GetVmbxNumber() returned KErrNotFound. Also, only the
        * parameter EVmbxNotDefinedQuery should be used outside the voice
        * mailbox application.
        *
        * @param aQueryType type of the query, alternatives:
        *                   - EVmbxNotDefinedQuery
        *                     (use this when no vmbx number is defined)
        *
        *                   - EVmbxChangeNumberQuery
        * @param aNumber    Initial value for the query, AND the result value
        *                   of an accepted query. (must be room for at least
        *                   KVmbxMaxNumberLength characters. Not allowed to
        *                   contain longer initial value than
        *                   KVmbxMaxNumberLength)
        * @param aShowQuery When this is set and the sim has support for vmbx
        *                    number the user is shown a list query by which the
        *                    storage of sim number can be decided
        *
        * @return ETrue if the query was accepted and the vmbx number saved.
        *               This is the case also when user saves empty number!
        *         EFalse if the user canceled the query. No vmbx number changed.
        **/
        IMPORT_C TBool QueryNumberL( TVmbxQueryType aQueryType, TDes& aNumber,
                                                    TBool aShowQuery = EFalse );

        /**
        * Saves the vmbx number. (in phone file system and SIM)
        *
        * @param aNumber vmbx number ( max length is KVmbxMaxNumberLength )
        * @param aEntry determines in which entry the number is saved:
        *               EAlsLine1Entry = vmbx number entry of ALS primary line
        *               EAlsLine2Entry = vmbx number entry of ALS secondary line
        *               EAlsActiveLineEntry = entry of currently active ALS line
        *
        * @return Symbian OS error code (KErrNone is successful)
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        IMPORT_C TInt SaveVmbxNumber( const TDesC& aNumber,
                                      TVmbxNumberEntry aEntry );

        /**
        * Saves vmbx number to Sim
        * @since
        * @param aNumber Vmbx number
        * @param aEntry Als line entry
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt SaveVmbxNumberToSim( const TDesC& aNumber,
                                                    TVmbxNumberEntry aEntry );

        /**
        * Saves vmbx number to phone memory
        * @since
        * @param aNumber Vmbx number
        * @param aEntry Als line entry
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt SaveVmbxNumberToPhone( const TDesC& aNumber,
                                                    TVmbxNumberEntry aEntry );

        /**
        * Issues a notify request on a vmbx number change.
        * User's HandleNotifyL() will be called whenever the vmbx number has
        * been changed by any subsystem. Only one notify request can be done
        * per one RVmbxNumber object simultaneously. When issuing a notify
        * request while one is active already, only parameters aHandler and
        * aNotifyOnActiveLineOnly are updated in existing request.
        *
        * @param aHandler user's notify handler
        * @param aNotifyOnActiveLineOnly if this is ETrue, notify events will
        *        be generated only if the vmbx number of the active ALS line is
        *        changed, not if the number of the other line is changed. When
        *        there is no ALS support, this has no effect ie. notification
        *        will be generated always.
        *
        * Leaves if the notify request could not be done.
        **/
        IMPORT_C void NotifyVmbxNumberChangeL( MVmbxNotifyHandler* aHandler,
                                                TBool aNotifyOnActiveLineOnly );

        /**
        * Cancels a notify request on a vmbx number change.
        **/
        IMPORT_C void NotifyVmbxNumberChangeCancel();

        /**
        * Gets the currently active ALS line number ( 1 or 2 ).
        * Information is fetched from supplementary settings client.
        * @param aLineNumber line number is returned here. If ALS is not
        *                    supported, value 1 is returned.
        * @return Symbian OS error code (KErrNone is successful)
        **/
        IMPORT_C TInt GetAlsLine( TInt& aLineNumber ) const;

        /**
        * Issues a notify request on active ALS line change. A callback to
        * user's code is made when the ALS line is changed. The notify request
        * remains active as long as it is canceled with
        * NotifyAlsLineChangeCancel(), ie. it is not canceled when the ALS line
        * is changed. Implementation uses SharedData's notify system.
        * Leaves if the notify request could not be done.
        * @param aObserver user's observer
        **/
        IMPORT_C void NotifyAlsLineChangeL( MVmbxAlsObserver* aObserver );

        /**
        * Cancels a notify request on ALS line change.
        **/
        IMPORT_C void NotifyAlsLineChangeCancel();

        /**
        * Closes the resources allocated in Open().
        *
        * Cancels all issued notify requests.
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        IMPORT_C void Close();


        /**
        * Puts a query on the screen asking for a voice mailbox address.
        * @since 3.0
        *
        * IMPORTANT:
        *
        * Other subsystems than voice mailbox application should call this
        * ONLY IF GetVmbxNumber() returned KErrNotFound. Also, only the
        * parameter EVmbxNotDefinedQuery should be used outside the voice
        * mailbox application.
        *
        * @param aQueryType type of the query, alternatives:
        *                   - EVmbxNotDefinedQuery
        *                     (use this when no vmbx address is defined)
        *
        *                   - EVmbxChangeNumberQuery
        * @param aNumber    Initial value for the query, AND the result value
        *                   of an accepted query. (must be room for at least
        *                   KVmbxMaxAddressLength characters. Not allowed to
        *                   contain longer initial value than
        *                   KVmbxMaxAddressLength)
        * @return ETrue if the query was accepted and the vmbx address saved.
        *               This is the case also when user saves empty address!
        *         EFalse if the user canceled the query. No vmbx address
        *         changed.
        **/
        IMPORT_C TBool QueryAddressL( TVmbxQueryType aQueryType,
                                      TDes& aNumber );

        /**
        * Puts a popup on the screen asking for which type of Vmbx is used.
        * @since 3.0
        * @param aTitle Vmbx title type ( call / define ).
        * @return which type of Vmbx is selected.
        */
        IMPORT_C TInt SelectTypeL( TBool aTitle );

        /**
        * Save address.
        * @since 3.0
        * @param aAddress includes saved vmbx address
        * @return Symbian OS error code (KErrNone is successful)
        **/
        IMPORT_C TInt SaveVmbxAddressL( TDes& aAddress );

        /**
        * Check if VoIP profiles exists.
        * @since 3.0
        * @return ETrue if there is at least one VoIP profile, otherwise EFalse
        */
        IMPORT_C TBool IsVoIPProfilesFound();

        /**
        * Check if VoIP voice mailbox services exists
        * @since 5.0
        * @return ETrue if there is at least one service, otherwise EFalse
        */
        IMPORT_C TBool IsIpVoiceMailboxServices();

        /**
        * Fetches the specified video mailbox number. If there is no video
        * mailbox number saved for the specified ALS line, parameter aNumber
        * remains unchanged and the return value is KErrNotFound.
        * @since 3.1
        * @param aNumber video mailbox number is saved here (must be room for
        *                at least KVmbxMaxNumberLength characters)
        * @param aEntry determines which video mbx number entry is used:
        *               EAlsLine1Entry = video mbx number entry of primary line
        *               EAlsLine2Entry = video mbx number entry of secondary line
        *               EAlsActiveLineEntry = entry of currently active line
        * @return - KErrNone if the number was found.
        *           aNumber will contain the video mbx number.
        *
        *         - KErrNotFound if the number is not defined
        *
        *         - Other error code if there was a problem in getting
        *           the number. Clients should show error note if this happens.
        **/
        IMPORT_C TInt GetVideoMbxNumber( TDes& aNumber,
                                TVmbxNumberEntry aEntry = EAlsActiveLineEntry );

        /**
        * Puts a query on the screen asking for a video mailbox number.
        * @since 3.1
        *
        * IMPORTANT:
        *
        * Other subsystems than voice mailbox application should call this
        * ONLY IF GetVideoMbxNumber() returned KErrNotFound. Also, only the
        * parameter EVmbxNotDefinedQuery should be used outside the voice
        * mailbox application.
        *
        * @param aQueryType type of the query, alternatives:
        *                   - EVmbxNotDefinedQuery
        *                     (use this when no vmbx number is defined)
        *                   - EVmbxChangeNumberQuery
        * @param aNumber    Initial value for the query, AND the result value
        *                   of an accepted query. (must be room for at least
        *                   KVmbxMaxNumberLength characters. Not allowed to
        *                   contain longer initial value than
        *                   KVmbxMaxNumberLength)
        * @return ETrue if the query was accepted and the vmbx number saved.
        *               This is the case also when user saves empty number!
        *         EFalse if the user canceled the query. No vmbx number changed.
        **/
        IMPORT_C TBool QueryVideoMbxNumberL( TVmbxQueryType aQueryType,
                                                                TDes& aNumber );

        /**
        * Saves the vmbx number. (in phone file system)
        * @since 3.1
        * @param aNumber video mbx number ( max length is KVmbxMaxNumberLength )
        * @param aEntry determines in which entry the number is saved:
        *               EAlsLine1Entry = video mbx number entry of ALS primary
        *                                line
        *               EAlsLine2Entry = video mbx number entry of ALS secondary
        *               line
        *               EAlsActiveLineEntry = entry of currently active ALS line
        * @return Standard Symbian OS errorcode.
        *         KErrNone if number was successfully saved.
        **/
        IMPORT_C TInt SaveVideoMbxNumber( const TDesC& aNumber,
                                                     TVmbxNumberEntry aEntry );

        /**
        * Returns if the memory location for vmbx is sim memory
        * @since 3.0
        * @return ETrue if the vmbx number should be stored in the SIM
        *          EFalse otherwise.
        */
        IMPORT_C TBool IsSimMemory();

        /**
        * Fetches vmbx address from SPS.
        * @since 3.0
        * @param aAddress vmbx address is saved here (must be room for at least
        *                KVmbxMaxAddressLength characters).
        * @return - KErrNone if the address was found.
        *           aNumber will contain the vmbx address.
        *
        *         - KErrNotFound if the address is not defined
        *
        *         - Other error code if there was a problem in getting
        *           the address. Clients should show error note if this happens.
        */
        IMPORT_C TInt GetVmbxAddressL( TDes& aAddress );

        /**
         * Fetches vmbx address from SPS.
         * @since 3.2
         * @param aAddress vmbx address is saved here (must be room for at least
         *                KVmbxMaxAddressLength characters).
         * @param aServiceId Used Service Id
         * @return General symbian error code.
         */
        IMPORT_C TInt GetVmbxAddress( TDes& aAddress, TUint aServiceId );

        /**
         * Puts a popup on the screen asking for which type of Vmbx is used.
         * @since 3.2
         * @param aTitle Vmbx title type ( call / define ).
         * @param aServiceId On completion, contains selected service's
         * Service Id. If CS or Video Vmbx selected, value is 1, otherwise
         * service's value is dynamic.
         * @return which type of Vmbx is selected.
         */
        IMPORT_C TInt SelectTypeL( TBool aTitle, TUint& aServiceId );

        /**
         * Service Ids getter
         * @since 3.2
         * @param aProfileIds On completion, contains all Service Ids.
         * @return General symbian error code.
         */
        IMPORT_C TInt GetServiceIds( RArray<TUint>& aProfileIds );

        /**
         * Service info getter
         * @since 3.2
         * @param aServiceInfo On completion, contains Service information
         * @return General symbian error code.
         */
        IMPORT_C TInt GetServiceInfo( TVmbxServiceInfo& aServiceInfo );

        /**
         * Issues a notify request on a Service profile change.
         * @param aHandler User's notify handler
         */
        IMPORT_C void NotifyServiceChange( MServiceNotifyHandler* aHandler );

        /**
         * Cancels a notify request on a Service profile change.
         */
        IMPORT_C void NotifyServiceChangeCancel();

        /**
         * Get Service's Brand Id
         * @since S60 3.2
         * @param aServiceId Used Service Id
         * @param aBrandId On completion, contains Brand Id
         */
        IMPORT_C void BrandIdL( TInt aServiceId, TDes8& aBrandId );

        /**
         * Branded icon getter
         * @since S60 3.2
         * @param aBrandingId Used branding Id
         * @param aBrandedBitmap Bitmap for branded icon
         * @param aBrandedBitmapMask Mask to branded icon
         */
        IMPORT_C void GetBrandedIconL( const TDesC8& aBrandingId,
                                       CFbsBitmap*& aBrandedBitmap,
                                       CFbsBitmap*& aBrandedBitmapMask );

        /**
         * Construct default icons(CS, Video and VoIP)
         * @since S60 3.2
         * @param aIcons On completion, contains created icons
         */
        IMPORT_C void ConstructDefaultIconsL( CArrayPtr<CGulIcon>* aIcons );

    private:

        /**
         * Add brand icon if it exist.
         * @since S60 3.2
         * @param aIcons On completion, contains new brand icon
         * @param aServiceId Used Service Id
         * @param aIconId On completion, contains branded icon's icon list id
         */
        void AddBrandIconL( CArrayPtr<CGulIcon>* aIcons, TUint aServiceId, TInt& aIconId );

    public:  // Functions from base classes

        /**
        * From MCenRepNotifyHandlerCallback, to notify the client about
        * changes for string value keys
        **/
        void HandleNotifyString( TUint32 aKey, const TDesC16& aValue );

        /**
        * From MCenRepNotifyHandlerCallback, to notify the client about
        * changes in keys when the whole repository is listened for.
        **/
        void HandleNotifyGeneric( TUint32 aKey );

        /**
        * From MSSSettingsObserver, to notify changes in SS settings in phone
        **/
        void PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue );

        /**
        * From MVmbxAlsObserver:
        **/
        void HandleAlsChangeL( TVmbxAlsLine aLine );
        /**
        * Refresh query. Client should determine whether it allow the
        * refresh to happen.
        * @since 2.6
        * @param aType Refresh type.
        * @param aFiles Elementary files which are to be changed.
        * May be zero length.
        * @return ETrue to allow refresh, EFalse to decline refresh.
        */
         TBool AllowRefresh( TSatRefreshType aType, const TSatRefreshFiles& aFiles ) ;

         /**
        * Notification of refresh.
        * @since 2.6
        * @param aType Type of refresh which has happened.
        * @param aFiles List of elementary files which has been changed.
        * May be zero length.
        */
        void Refresh( TSatRefreshType aType, const TSatRefreshFiles& aFiles );

    private:  // New functions

        /**
        * Routine to query SIM number
        * @since
        * @param aNumber Vmbx number
        * @param aEntry Als line entry
        * @return ETrue if user inserted number
        **/
        TBool QuerySimNumberL( TDes& aNumber,
                               TVmbxNumberEntry aEntry = EAlsActiveLineEntry );


        /**
        * Loads the resources needed for queries and confirmation notes.
        * @since
        **/
        void LoadResourceL();

        /**
        * Shows a confirmation note after a saved vmbx number.
        * @param aResourceId Resource id
        **/
        void ShowConfirmationNoteL( TInt aResourceId ) const;

        /**
        * Verifies that the given string is a valid phone number.
        * Rules:
        *
        * - '+' is only allowed as the first character
        * - The phone number part must have 3-21 digits
        * @since
        * @param aNumber Vmbx number
        * @return ETrue if valid number
        **/
        TBool IsValidPhoneNumber( const TDesC& aNumber ) const;

        /**
        * Checks if a number is an emergency number.
        * @since
        * @param aNumber Vmbx number
        * @return ETrue if emergency number
        **/
        TBool IsEmergencyNumberL( const TDesC& aNumber );

        /**
        * Fetches information about the SIM phonebook for vmbx numbers.
        * Updates internal flags KVmFlagSimSupport and
        * KVmFlagPhonebookBeingCached.
        *
        * @return Phonebook's GetInfo() return code.
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        TInt PhoneBookInfo();

        /**
        * Does a vmbx phonebook operation
        *
        * If the phonebook is being cached, defined amount of retries are
        * made with 0.2 second delay in between.
        * @param aOperation Operation identifier
        * @param aEntry Vmbx entry (NULL)
        * @return Phonebook error code. Specifically, KErrInUse, if
        * the phonebook is still being cached after retries.
        **/
        TInt DoPhonebookOperation( TVmbxPhonebookOperation aOperation,
                                                    TVmbxEntry* aEntry = NULL );

        // Prohibit copy constructor
        RVmbxNumber( const RVmbxNumber& );

        // Prohibit assigment operator
        RVmbxNumber& operator = ( const RVmbxNumber& );

        /**
        * Reads an entry from the ICC-based phonebook.
        * @since
        * @param aEntry Vmbx sim entry data
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        void PhonebookReadL( TVmbxEntry& aEntry );

        /**
        * Writes an entry into ICC-based phonebook.
        * @since
        * @param aEntry Vmbx sim entry data
        * @return Completion status of a request
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        TInt PhonebookWrite( TVmbxEntry& aEntry );

        /**
        * Removes an entry from ICC-based phonebook.
        * @since
        * @param aEntry Vmbx sim entry data
        * @return Completion status of a request
        **/
        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        TInt PhonebookDelete( TVmbxEntry& aEntry );

        /**
        * Converts a phone number to western format
        * @param aDes Parameter to change
        **/
        void ConvertToWesternDigits( TDes& aDes ) const;

        /**
        * Create connection to ETel server
        * @since
        * @return KErrNone if success
        **/
        TInt ConnectETelServer();

        /**
        * Open phonebook's Single Fetch Dialog
        * @since
        * @param aPhoneNumber Phone number
        */
        void FetchNumberFromPhonebook2L( TDes& aPhoneNumber );

        /**
         * Handling of emergency dial result.
         * @since 3.2
         * @see MPhCltEmergencyCallObserver::HandleEmergencyDialL().
         */
         void HandleEmergencyDialL( const TInt aStatus );

        /**
        * Retrieves voice mailbox number from SIM/R-UIM
        * @since
        * @param aNumber Number
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt GetVmNumFromSIM( TDes& aNumber, TInt aLineNumber );

        /**
        * Retrieves voice mailbox number from VMN summary info
        * @since
        * @param aNumber Number
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt GetVmNumFromVMN( TDes& aNumber );

        /**
        * Retrieves voice mailbox number from NAM
        * @since
        * @param aNumber Number
        * @param aNumType Number type (EVmbxMDN)
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt GetVmNumFromNamStore( TDes& aNumber, TInt aNumType = EVmbxMDN );

        /**
        * Retrieves voice mailbox number from Shared Data
        * @since
        * @param aNumber Number
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt GetVmNumFromSD( TDes& aNumber, TInt aLineNumber );

        /**
        * Closes ETel resources
        * @since
        */
        void CloseETel();

        /**
        * Appends string from resource to descriptor array
        * @since
        * @param aResourceId Resource id
        * @param aArray String array
        */
        void AppendStrToArrayL( TInt aResourceId, CDesCArray& aArray );

        /**
        * Set query dialog header text from resource string
        * @since
        * @param aResourceId Resource id
        * @param aDlg Query dialog
        */
        void SetHeaderFromResourceL( TInt aResourceId, CAknQueryDialog& aDlg );

        /**
        * Subscribe SAT notifications
        * @since
        */
        void SubscribeSATNotificationsL();

        /**
        * Gets MBI-file settings from Sim. MBI-file identifies the usage
        * of the mailbox records settings
        * @since
        * @return ETrue if successed
        */
        TInt MailboxNumbersIdentifiers();

        /**
        * Reading and writing to sim are tested
        * @since
        */
        void TestAndSetSimAccessFlags();

        /**
        * The actual size for number must be determine from max length
        * returned from GetInfo-method and saved number length on Sim.
        * @param aSimNumberLength The length of the number read from sim.
        * @since
        */
        void CalculateActualMaxLengthOnSim( TInt aSimNumberLength );

        /**
        * Returns video support state
        * @since S60 3.2
        * return ETrue if supported
        */
        TBool VideoSupported();

        /**
        * Checks if writing to sim is prohibited
        * @since S60 3.2
        * return ETrue if writing is not allowed
        */
        TBool IsReadOnlySIM();

        /**
        * Checks is VMBX number defined in phone memory
        * @since S60 3.2
        * @param aLineNumber Line 1 or 2
        * @return ETrue if VMBX number defined in Phone memory
        */
        TBool VmbxNumDefInPhoneMemory( TInt aLineNumber);

        /**
        * Checks is the given number stored in phone memory
        * @since S60 3.2
        * @param aNumber The number checked
        * @param aLineNumber Line 1 or 2
        * @return ETrue if number stored in Phone memory
        */
        TBool NumStoredInPhone( const TDesC& aNumber, TInt aLineNumber );

        /**
        * Saves VMBX number to the backup store
        * @since S60 3.2
        * @param aNumber The number to be saved
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt BackUpSimNumber( const TDesC& aNumber, TInt aLineNumber );

        /**
        * Fetches VMBX number from backup store
        * @since S60 3.2
        * @param aNumber number
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt FetchSimNumberBackUp( TDes& aNumber, TInt aLineNumber );

        /**
        * Clears VMBX number from the phone memory
        * @since S60 3.2
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt ClearVMBXNumberFromPhone( TInt aLineNumber );

        /**
        * Indicates if user has edited VMBX number
        * @since S60 3.2
        * @param aLineNumber Line 1 or 2
        * @return ETrue if VMBX is edited by user
        */
        TBool UserEditNumber( TInt aLineNumber );

        /**
        * Sets the flag indicating that the user has edited
        * VMBX number
        * @since S60 3.2
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt SetUserEditNumber( TInt aLineNumber );

        /**
        * Resets the flag that indicates that the user has edited
        * VMBX number
        * @since S60 3.2
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt ResetUserEditNumber( TInt aLineNumber );

        /**
        * Fetches VMBX number stored in SIM if number is
        * available
        * @since S60 3.2
        * @param aNumber number
        * @param aLineNumber Line 1 or 2
        * @return ETrue if number is fetched
        */
        TBool UseSimNumIfAvailable( TDes& aNumber, TInt aLineNumber );

        /**
        * Handles the updating of a number stores in phone memory
        * @since S60 3.2
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */
        TInt HandleNumberStores( TInt aLineNumber );

        /**
        * Returns if the SIM memory is used on ALS 1
        * @since 3.2
        * @return ETrue if the SIM memory is used to store ALS1 number
        *         EFalse otherwise.
        */
        TBool UsesSimMemory();

        /**
        * check count of VMBX number
        * available
        * @since S60 3.2.2
        * @param out aDefinedNumber a count of defined numbers
        * @param out aDefinedFlag a flag of which voice mail services are defined
        * @param out aServiceId Id of selected service is set
        *
        * @return Type of Vmbx, if there is only one mailbox configured, otherwise EVmbxNone
        */
        TInt CheckDefinedNumberCountL( TInt& aDefinedNumber,
                    TInt& aDefinedFlag, TUint& aServiceId );

        /**
        * Returns if VMBX number stored in phone memory via external clients
        * @since S60 3.2.3
        * @param aLineNumber Line 1 or 2
        * @return ETrue if number is stored in phone memory via 
        *                                               external clients
        *         EFalse number is stored in phone memory via internal clients
        */
        TBool IsPhoneMemForcedEdit( const TInt aLineNumber );

        /**
        * Sets the flag indicating that external clients has edited
        * VMBX number
        * @since S60 3.2.3
        * @param aLineNumber Line 1 or 2
        * @return Symbian OS error code (KErrNone is successful)
        */   
        TInt SetPhoneMemForcedEdit( TInt aLineNumber );

        /**
         * Displays a note with a text loaded from resource file.
         * @since S60 v5.2
         * @param aResourceId Resource id for loading text for note
         */
        void ShowInformationNoteL( const TInt aResourceId ) const;

        /**
         * Check whether the note should be shown
         * this check is done because phone client calls GetVmbxNumber 
         * and then SelectTypeL, which causes note qtn_... to be shown twice
         * @since S60 v5.2
         * @return ETrue if the note should be shown
         */
        TBool IsNoNumberNoteAllowed();

    private:  // data

        // Observer for user action for saved vmbx number
        MVmbxNotifyHandler* iNotifyCallBack;

        // Observer for user action for als line
        MVmbxAlsObserver* iUserAlsObserver;


        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        // Root telephony server session
        RTelServer iTelServer;

        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        //Mobile phone client
        RMobilePhone iPhone;

        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        //Phonebook client
        RMobilePhoneBookStore iPhoneBook;

        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        //  Multimode ETel phone book info structure that will
        //  be used to retrieve data from phone book store.
        RMobilePhoneBookStore::TMobilePhoneBookInfoV1 iPhoneBookInfo;

        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        //  A package for the iPhoneBookInfo.
        RMobilePhoneBookStore::TMobilePhoneBookInfoV1Pckg iPhoneBookInfoPckg;

        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        //  A package for the iPhoneVoicemailInfo.
        RMobilePhone::TMobilePhoneVoicemailIdsV3 iPhoneVoicemailInfo;

        /*****************************************************
        *  Series 60 Customer / ETel
        *  Series 60 ETel API
        *****************************************************/
        //  A package for the iPhoneVoicemailInfoPckg.
        RMobilePhone::TMobilePhoneVoicemailIdsV3Pckg iPhoneVoicemailInfoPckg;

        /*****************************************************
        *  Series 60 Customer / RSAT
        *  Series 60 RSAT API
        *****************************************************/
        RSatSession  iRSatSession; //// Root RSATsession opening RSatClient

        /*****************************************************
        *  Series 60 Customer / RSAT
        *  Series 60 RSAT API
        *****************************************************/
        RSatRefresh  iRSatClient; //RSatClient for notification of file changed.

        /*****************************************************
        *  Series 60 Customer / RSAT
        *  Series 60 RSAT API/ ETel
        *  Series 60 ETel API
        *****************************************************/
        //  A package for the TRefreshFileList.
        // list of files, waiting for notification.
        RSat::TRefreshFileList iObservedFileList;

        //Central repository observer
        CCenRepNotifyHandler* iNotifyHandler;

        //Central repository session
        CRepository* iSession;

        //SS setting
        RSSSettings iSsSettings;

        // Offset in resourse file
        TInt iResourceFileOffset;

        // Vmbx state flags
        TInt iFlags;

        // text query dialog
        CVmTextQueryDialog* iQuery;  // owned

        // indicates if the RVmbxNumber was closed during a dialog
        TBool* iClosedPtr;

        // Control enviroment
        CCoeEnv* iCoeEnv;  // not owned

        //PRI settings
        // nNumber options
        TInt iVmNumOpts;

        //Ui options
        TInt iVmUiOpts;

        // Synchronization state
        TBool iSynchronize;

        // Vmbx type
        TInt iType;

        // Memory location (sim/phone)
        TInt iMemoryLocation;

        // Sim query dialog
        CVMSimQueryDilaog* iVMSimQueryDialog;

        // Text query dialog
        CVmTextQueryDialog* iVideoQuery;  // owned

        // voice mailbox type selection dialog
        CAknListQueryDialog* iTypeSelectionQuery; //owned


        TBool iAlphaStringFound;
        // Alpha string which is read from the SIM card
        HBufC* iAlphaStringFromSIM;
        // True if number in sim
        TBool iNumberFound;
        //Phonebook type
        TUint8 iPhoneBookType;
        //Number fetched from Sim when checking write protection
        TBuf<KVmbxMaxNumberLength> iNumberFromSim;

        //ETrue when first USim phonebook type is tested
        TBool iUSimFirstRoundTest;


    private:

        /**
         * True if Sim access rights are tested
         */ 
        TBool iStartUpTest;

        /**
         * For single item fetch
         */ 
        CPhCntSingleItemFetch* iCntFetch;

        /**
         * To test if mbdn number doesn't exist
         */ 
        TBool iNoNumberFound;

        /**
         * To test if number can be found from vmbx-phonebook
         */ 
        TBool iMbdnPhonebookOk;

        /**
         * querying state of QueryNumberL
         */ 
        TVmVmQueryStatusType iQueryingState;

        /**
         * Type and Id of mailboxes to list in iTypeSelectionQuery, Own
         */
        RArray<TVmbxServiceSelection> iVmbxServiceSelection;

        /**
         * Handles Service proviver Settings, Own.
         */
        CVmSPSHandler* iVmSpsHandler;

        /**
         * Handles Branding Server, Own.
         */
        CVmBSHandler* iVmBsHandler;
        
        /**
         * Feature manager is initialized
         */
        TBool iFeatMgrInitialized;
        
        /**
         * Sim card is whether found in ME
         */
        TBool iSimCardFound;

    };

#endif  // VMNUMBER_H

// End of File
