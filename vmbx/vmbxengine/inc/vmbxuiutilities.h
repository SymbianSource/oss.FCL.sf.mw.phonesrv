/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
     */
    static CVmbxUiUtilities* NewL();

    /**
     * Destructor.
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
     * @param aType in TVmbxType type
     * @param aMode Tin VmbxQueryMode mode
     * @param aNumber the voice mailbox number
     * @param aResult out the result of dialog
     */
    void ShowQueryDialogL( const TVmbxType& aType,
        const TVmbxQueryMode & aMode, TDes& aNumber, TInt& aResult );

    /**
     * Show informationd note
     *
     * @param aType in TVmbxNoteType type
     */
    void ShowInformationdNoteL(const TVmbxNoteType aType);
 

    /**
     * Show define number in selection dialog
     * Leaves if user cancel selected.
     *
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    void ShowDefineSelectionDialog( TVmbxType& aType, TInt& aResult );

    /**
     * Show call number in selection dialog
     * Leaves if user cancel selected.
     *
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
     */
    void DismissDialogL();


    /**
     * Get Mailbox type default image.
     * If get unseccessful, Leaves with Symbian OS error code
     *
     * @param aParams Mailbox type for which image is needed.
     * @return aImage New image. Ownership is transferred.
     */
    //CGulIcon* GetVmbxImageL( const TVoiceMailboxParams& aParams );

    /**
     * Show not allowed editing dialog
     *
     */
    void ShowNotAllowedEditingDialogL();

    /**
     * Show invalid or warning dialog
     *
     */
    void ShowInvalidWarningDialogL();


private: // data

    /**
     * C++ default constructor.
     */
    CVmbxUiUtilities();

    /**
     * C++  Two-phased constructor.
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
