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
* Description:  This class handles the communication between server and user
*                   interface
*
*/


#ifndef CSATUIOBSERVER_H
#define CSATUIOBSERVER_H

//  INCLUDES
#include <msatuiobserver.h>
#include <rsatuisession.h>
#include <rsatsession.h>
#include <QString>
#include "satappcommonconstant.h"
// CONSTANTS
//256 max length-header information fields
const TInt KAdditionalInfoMaxSize = 244;
const TInt KMaxPlayToneLength = 50;

enum TIconCommand
    {
    EIconSetUpMenuContext = 1,
    EIconSetUpMenuItems,
    EIconDisplayText,
    EIconGetInput,
    EIconGetInkey,
    EIconGetYesNo,
    EIconPlayTone,
    EIconNotification,
    EIconConfirmCommand
    };

// FORWARD DECLARATIONS
class MSatUiAdapter;
class MSatUiActionImplementer;
class CSatUiIconHandler;
class CFbsBitmap;

// CLASS DECLARATION

/**
*  This class implements the methods specified in the MSatUiObserver.
*/

class CSatUiObserver : public CBase,
                       public MSatUiObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSatUiObserver* NewL();

        /**
        * Destructor.
        */
        virtual ~CSatUiObserver();

    public: // New functions

        /**
        * Sets the UI.
        * @param aImplementer Pointer to be set.
        */
        void SetImplementer( MSatUiActionImplementer* aImplementer );

        /**
        * Returns the adapter provided by SatCli.
        */
        MSatUiAdapter* Adapter();


        /**
        * Return current profile state.
        * @return current profile state.
        */
        TInt ProfileState();

        /**
        * ConnectRSatSession.
        * 
        */
        void ConnectRSatSessionL();

        /**
        * DisconnectRSatSession
        * 
        */
        void DisconnectRSatSession();

    public: // Functions from base classes

         /**
        * From MSatUiObserver Notification of the SAT Display Text command.
        * @param aText The text string to be displayed.
        * @param aSimApplicationName  Sim Application name
        * @param aIconId The id of icon.
        * @param aRequestedIconDisplayed Informs if icon is not used.
        * @param aSustainedText Indicates is this text sustained ie. no timers
        *        used if ETrue.
        * @param aDuration Duration for showing the text
        * @param aWaitUserToClear Indication if user is needed to clear
        *        message
        * @return The response to this command.
        */
        TSatUiResponse DisplayTextL( const TDesC& aText,
            const TDesC& aSimApplicationName,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed,
            const TBool aSustainedText,
            const TTimeIntervalSeconds aDuration,
            const TBool aWaitUserToClear );

        /**
        * From MSatUiObserver Notification of the SAT Get Inkey command.
        * @param aText The query text.
        * @param aCharacterSet The character range allowed.
        * @param aInput The input character.
        * @param aHelpIsAvailable A flag indicating  if SAT help is available
        * @param aIconId The id of icon.
        * @param aRequestedIconDisplayed Informs if icon is not used.
        * @param aDuration Duration for showing the dialog.
        * @param aImmediateDigitResponse Indication if Immediate digit response
        *        is needed.
        * @return The response of to this command.
        */
        TSatUiResponse GetInkeyL( const TDesC& aText,
            const TSatCharacterSet aCharacterSet, TChar& aInput,
            const TBool aHelpIsAvailable,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed,
            TUint& aDuration,
            const TBool aImmediateDigitResponse );

        /**
        * From MSatUiObserver Notification of the SAT Get Input command.
        * @param aText The query text.
        * @param aCharacterSet The character range allowed.
        * @param aInput The input string.
        * @param aMinLength The minimum length of the input allowed.
        * @param aMaxLength The maximum length of the input allowed.
        * @param aHideInput A flag indicating if the input should be hidden.
        * @param aHelpIsAvailable A flag indicating if SAT help is available.
        * @param aIconId The id of icon.
        * @param aRequestedIconDisplayed Informs if icon is not used.
        * @return The response to this command.
        */
        TSatUiResponse GetInputL( const TDesC& aText,
            const TSatCharacterSet aCharacterSet, TDes& aInput,
            const TInt aMinLength, const TInt aMaxLength,
            const TBool aHideInput,
            const TBool aHelpIsAvailable,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed );

        /**
        * From MSatUiObserver Notification of the SAT Set Up Menu command.
        * @param aText The title text.
        * @param aMenuItems The array of menu item captions.
        * @param aMenuItemNextActions The array of menu item next action
        * indicator codes.
        * @param aHelpIsAvailable A flag indicating if SAT help is available.
        * @param aIconId The id of title icon.
        * @param aMenuIcons List of icon identifiers for menu items.
        * @param aIconListQualifier Indicates how to use icons in the icon list.
        * @param aSelectionPreference Specifies is the soft keys preferred.
        * @return The response to this command.
        */
        TSatUiResponse SetUpMenuL( const TDesC& aText,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TBool aHelpIsAvailable,
            const TSatIconId& aIconId,
            const CArrayFixFlat<TInt>* aMenuIcons,
            const enum TSatIconQualifier aIconListQualifier,
            const enum TSatSelectionPreference aSelectionPreference );

        /**
        * From MSatUiObserver Notification of the SAT Select Item command.
        * @param aText The title text.
        * @param aMenuItems The array of menu item captions.
        * @param aMenuItemNextActions The array of menu item next action
        * indicator codes.
        * @param aDefaultItem The item selected by default.
        * @param aSelection The item index selected by the user.
        * @param aHelpIsAvailable A flag indicating if help is available.
        * @param aIconId The id of title icon.
        * @param aMenuIcons List of icon identifiers for menu items.
        * @param aIconListQualifier Indicates how to use icons in the icon list.
        * @param aRequestedIconDisplayed Informs if icon is not used.
        * @param aSelectionPreference Specifies is the soft keys preferred.
        * @return The response to this command.
        */
        TSatUiResponse SelectItemL( const TDesC& aText,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TInt aDefaultItem, TUint8& aSelection,
            const TBool aHelpIsAvailable,
            const TSatIconId& aIconId,
            const CArrayFixFlat<TInt>* aMenuIcons,
            const enum TSatIconQualifier aIconListQualifier,
            TBool& aRequestedIconDisplayed,
            const enum TSatSelectionPreference aSelectionPreference );

        /**
        * From MSatUiObserver Notification of the SAT Play Tone command.
        * @param aText The text to be displayed.
        * @param aTone The tone to be played.
        * @param aDuration The duration of the tone to be played.
        * @param aIconId The id of icon.
        * @param aRequestedIconDisplayed Informs if icon is not used.
        * @return The response to this command.
        */
        TSatUiResponse PlayTone( const TDesC& aText,
            const TSatTone aTone,
            const TTimeIntervalMicroSeconds aDuration,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed );

        /**
        * From MSatUiObserver General confirmation request
        * @param aCommandId ID of the quering command
        * @param aAlphaIdStatus Alpha Identifier status
        * @param aText The text to be displayed.
        * @param aAdditionalText Additional text to be used in queries.
        * @param aActionAccepted Indicates whether the command was accepted.
        * @param aIconId The id of icon.
        * @param aRequestedIconDisplayed Informs if icon is not used.
        * @param aTerminatedByUser Informs if end key is used.
        */
        void ConfirmCommand(
            const TSatSQueryCommand aCommandId,
            const TSatAlphaIdStatus aAlphaIdStatus,
            const TDesC& aText,
            const TDesC& aAdditionalText,
            TBool& aActionAccepted,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed,
            TBool& aTerminatedByUser );

        /**
        * From MSatUiObserver General notification
        * @param aCommandId ID of the notifying command
        * @param aAlphaIdStatus Alpha Identifier status
        * @param aText Alpha Identifier
        * @param aIconId The id of icon.
        * @param aRequestedIconDisplayed Informs if icon is not used.
        * @param aControlResult Control result of the MoSm and CallControl
        * @return The response of the UI to this command.
        */
        TSatUiResponse Notification(
            const TSatSNotifyCommand aCommandId,
            const TSatAlphaIdStatus aAlphaIdStatus,
            const TDesC& aText,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed,
            const TSatControlResult aControlResult );

        /**
        * From MSatUiObserverGeneral event notification. Used for example to
        * tell UI that command has completed its execution.
        * @param aEventId, identifies the event
        * @param aEventStatus, status of the event, used as additional info for
        *        the event
        * @param aError, possible error code that may affect on event handling.
        *        This is also used as additional info
        */
        void EventNotification(
            const TSatSEvent aEventId,
            const TSatSEventStatus aEventStatus,
            const TInt aError );

    private:

        /**
        * C++ default constructor.
        */
        CSatUiObserver();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Fetch icon information.
        * @param aIconId Id for icons
        * @param aIconCommand Command type for icon.
        * @return Pointer to selected FbsBitmap icon
        */
        CFbsBitmap* FetchIcon( const TUint8 aIconId,
            const enum TIconCommand aIconCommand );

        /**
        * Gets default text's resource id
        * @param aCommandId Identifies the command
        * @param aControl Used only for MoSm and Call Control
        * @return Resource id for the default text
        */
        void DefaultAlphaId(
            const TSatSNotifyCommand aCommandId,
            const TSatControlResult aControlResult,
            QString &resource) const;

        /**
        * Get size for the icon in popup window.
        * @param aSize Reference to the size of icon place.
        * @param aIconCommand Dialog information
        * @return TBool Returns EFalse if popup window is not supported
        *               in current layout.
        */
        TBool GetPopUpWindowIconSize( TSize& aSize,
             const TIconCommand  aIconCommand );

        /**
        * Creates scalable bitmap.
        * @param aBitMapToConvert Reference icon to be scaled.
        * @param aIconCommand Dialog information
        */
        void GetScalableBitmapL( CFbsBitmap*& aBitMapToConvert,
            const TIconCommand  aIconCommand );

        /**
        * Handles scalable bitmap.
        * @param aBitMapToConvert Reference icon to be scaled.
        * @param aIconCommand Dialog information
        * @param aRequestedIconDisplayed Informs if icon is not shown.
        */
        void GetScalableBitmap(
            CFbsBitmap*& aBitMapToConvert,
            const TIconCommand  aIconCommand,
            TBool& aRequestedIconDisplayed );

        /**
        * Handles scalable bitmap.
        * @param aBitMapToConvert Reference icon to be scaled.
        * @param aIconCommand Dialog information
        */
        void GetScalableBitmap(
            CFbsBitmap*& aBitMapToConvert,
            const TIconCommand  aIconCommand );

    public: // Data

        //wait scheduler
        CActiveSchedulerWait iWait;

    private: // Data

        // play tone sequence
        TBuf8<KMaxPlayToneLength> iSequence;

        //Ref: Interface to the UI-part
        MSatUiActionImplementer* iActionImplementer;

        // own: used with refresh started and finished
        TBool iRefresh;

        // own: session to sat client
        RSatUiSession iSat;

        //Ref: Adapter provided by SAT Client
        MSatUiAdapter* iAdapter;

        //Own: Session
        RSatSession iSatSession;

        //Own: Current Iconididentifier
        TInt iIconIdIdentifier;

        // own: used to signal that icons are supported
        TBool iIconSupport;

        //Own: used to indicate if ESatSClearScreenEvent occurs
        TBool iClearScreenEvent;

    };

#endif    // CSATUIOBSERVER_H

//  End of File
