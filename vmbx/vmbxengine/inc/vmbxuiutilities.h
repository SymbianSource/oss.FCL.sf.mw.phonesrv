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
* Description:  Utility methods for universal usage
*  Interface   : Private, CVmbxUiUtilities
*
*/

#ifndef C_VMBXUIUTILITIES_H
#define C_VMBXUIUTILITIES_H

// INCLUDES
#include <e32base.h>
#include <cvoicemailbox.h>

#include "mvmbxuiutilities.h"
#include "vmbxuihandler.h"

// FORWARD DECLARATIONS


// CLASS DECLARATION
/**
*  Implementation of the auxiliary methods for the VMBX engine classes
*
*  @lib vmbxengine.lib
*  @since S60 v5.2
*/
NONSHARABLE_CLASS( CVmbxUiUtilities ): public CBase,
                                       public MVmbxUiUtilities
    {

public:

    /**
     * Two-phased constructor
     * @since S60 v5.2
     */
    static CVmbxUiUtilities* NewL();

    /**
     * Destructor.
     * @since S60 v5.2
     */
    ~CVmbxUiUtilities();

    /**
     * Show mailbox query
     *
     * @since S60 v5.2
     * @param in aType TVmbxType type
     * @param in aMode TVmbxQueryMode mode
     * @param out aNumber the voice mailbox number
     */
    void ShowQueryL( const TVmbxType& aType,
                    const TVmbxQueryMode& aMode,
                    TDes& aNumber );

    /**
     * Show mailbox query dialog
     * Leaves if user cancel selected.
     *
     * @since S60 v5.2
     * @param aType in TVmbxType type
     * @param aMode Tin VmbxQueryMode mode
     * @param aNumber the voice mailbox number
     * @param aResult out the result of dialog
     */
    void ShowQueryDialogL( const TVmbxType& aType,
        const TVmbxQueryMode & aMode, TDes& aNumber, TInt& aResult );

    /**
     * Show confirmation dialog when save number to phone
     *
     * @since S60 v5.2
     */
    void ShowSaveToPhoneNote();

    /**
     * Show confirmation dialog when save number to SIM
     *
     * @since S60 v5.2
     */
    void ShowSaveToSimNote();

    /**
     * Show confirmation dialog when save video number
     *
     * @since S60 v5.2
     */
    void ShowVideoSavedNote();

    /**
     * Show define number in selection dialog
     * Leaves if user cancel selected.
     *
     * @since S60 v5.2
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    void ShowDefineSelectionDialogL( TVmbxType& aType, TInt& aResult );

    /**
     * Show call number in selection dialog
     * Leaves if user cancel selected.
     *
     * @since S60 v5.2
     * @param in aArray array of the defined voice mailbox entry
     * @param in aIcons icons of the defined voice mailbox
     * @param out aParams the type TVoiceMailboxParams which should include
     *          the service id and the type of seclected TVmbxType
     * @param out aResult the result user seclected
     */
    void ShowCallSelectionDialogL(
                const RPointerArray<CVoiceMailboxEntry>& aArray,
                //CArrayPtr<CGulIcon>* aIcons,
                TVoiceMailboxParams& aParams, TInt& aResult );

    /**
     * Tries to close all open dialogs
     *
     * @since S60 v5.2
     */
    void DismissDialogL();

    /**
     * To get mailbox entry value
     * Leaves if Symbian OS error code
     *
     * @since S60 v5.2
     * @param aType TVmbxType type
     */
    void ShowSaveEmptyNoteL( const TVmbxType& aType );

    /**
     * Get Mailbox type default image.
     * If get unseccessful, Leaves with Symbian OS error code
     *
     * @since S60 v5.2
     * @param aParams Mailbox type for which image is needed.
     * @return aImage New image. Ownership is transferred.
     */
    //CGulIcon* GetVmbxImageL( const TVoiceMailboxParams& aParams );

    /**
     * Show error dialog
     *
     * @since S60 v5.2
     */
    void ShowErrorDialogL();

    /**
     * Show invalid or warning dialog
     *
     * @since S60 v5.2
     */
    void ShowInvalidWarningDialogL();


private: // data

    /**
     * C++ default constructor.
     */
    CVmbxUiUtilities();

    /**
     * C++  Two-phased constructor.
     * @since S60 v5.2
     */
    void ConstructL();


    /**
    * Open phonebook's Single Fetch Dialog
    * @since S60 v5.2
    * @param aPhoneNumber Phone number
    */
    void FetchNumberFromPhonebook2L( TDes& aPhoneNumber );

private:

    /**
     * Ui handler
     * Own.
     */
    CVmbxUiHandler* iUiHandler;
    };

#endif  // C_VMBXUIUTILITIES_H
