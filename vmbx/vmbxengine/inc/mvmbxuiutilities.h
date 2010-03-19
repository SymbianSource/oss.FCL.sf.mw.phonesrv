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
* Description:  Public APIs of VmbxUiUtilities to internal class
*  Interface   : Internal, MVmbxUiutilities
*
*/


#ifndef M_VMBXUIUTILITIES_H
#define M_VMBXUIUTILITIES_H

// INCLUDES
#include <cvoicemailbox.h>

// CLASS DECLARATION

/**
 *  MVmbxUiUtilities
 *
 *  @since S60 v5.2
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxUiUtilities )
    {

public:
    /**
     * Show mailbox query
     *
     * @since S60 v5.2
     * @param in aType TVmbxType type
     * @param in aMode TVmbxQueryMode mode
     * @param out aNumber the voice mailbox number
     */
    virtual void ShowQueryL( const TVmbxType& aType,
                    const TVmbxQueryMode& aMode,
                    TDes& aNumber ) = 0;

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
    virtual void ShowQueryDialogL( const TVmbxType& aType,
        const TVmbxQueryMode & aMode, TDes& aNumber, TInt& aResult ) = 0;

    /**
     * Show confirmation dialog when save number to phone
     *
     * @since S60 v5.2
     */
    virtual void ShowSaveToPhoneNote() = 0;

    /**
     * Show confirmation dialog when save number to SIM
     *
     * @since S60 v5.2
     */
    virtual void ShowSaveToSimNote() = 0;

    /**
     * Show confirmation dialog when save video number
     *
     * @since S60 v5.2
     */
    virtual void ShowVideoSavedNote() = 0;

    /**
     * Show define number in selection dialog
     * Leaves if user cancel selected.
     *
     * @since S60 v5.2
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    virtual void ShowDefineSelectionDialogL( TVmbxType& aType, TInt& aResult ) = 0;

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
    virtual void ShowCallSelectionDialogL( 
                const RPointerArray<CVoiceMailboxEntry>& aArray, 
               // CArrayPtr<CGulIcon>* aIcons,
                    TVoiceMailboxParams& aParams, TInt& aResult ) = 0;

    /**
     * Tries to close all open dialogs
     *
     * @since S60 v5.2
     */
    virtual void DismissDialogL() = 0;

    /**
     * To get mailbox entry value
     * Leaves with Symbian OS error code
     *
     * @since S60 v5.2
     * @param aType TVmbxType type
     */
    virtual void ShowSaveEmptyNoteL( const TVmbxType& aType ) = 0;

    /**
     * Get Mailbox type default image.
     * If get unseccessful, Leaves with Symbian OS error code
     * 
     * @since S60 v5.2
     * @param aParams Mailbox type for which image is needed.
     * @return aImage New image. Ownership is transferred.
     */
    //virtual CGulIcon* GetVmbxImageL( const TVoiceMailboxParams& aParams ) = 0;

    /**
     * Show error dialog
     *
     * @since S60 v5.2
     */
    virtual void ShowErrorDialogL() = 0;

    /**
     * Show Invalid Warning dialog.
     *
     * @since S60 v5.2
     */
    virtual void ShowInvalidWarningDialogL() = 0;
    };

#endif // M_VMBXUIUTILITIES_H
