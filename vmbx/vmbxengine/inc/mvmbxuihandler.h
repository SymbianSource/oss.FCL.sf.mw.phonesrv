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
* Description:  
*  Interface   : Internal, MVmbxUiHandler
*
*/


#ifndef M_MVMBXUIHANDLER_H
#define M_MVMBXUIHANDLER_H

// INCLUDES

// FORWARD DECLARATION


// CLASS DECLARATION

/**
 *  MVmbxUiHandler
 *
 *  @since S60 v5.2
 *  @lib vmbxengine.lib
 *
 */
NONSHARABLE_CLASS ( MVmbxUiHandler )
    {

public:

    /**
     * Show mailbox query dialog
     *
     * @since S60 v5.2
     * @param aType in TVmbxType type
     * @param aMode Tin VmbxQueryMode mode
     * @param aNumber the voice mailbox number
     * @param aResult out the result of dialog
     */
    virtual void ShowVmbxQueryDialog( const TVmbxType& aType,
        const TVmbxQueryMode & aMode, TDes& aNumber, TInt& aResult )= 0;


    /**
     * Show define number in selection dialog
     *
     * @since S60 v5.2
     * @param in aType vmbx type
     * @param out aResult the result of dialog
     */
    virtual void ShowDefineSelectionDialog( 
        TVmbxType& aType, TInt& aResult ) = 0;

    /**
     * Show call number in selection dialog
     *
     * @since S60 v5.2
     * @param in aArray array of the defined voice mailbox entry
     * @param in aIcons icons of the defined voice mailbox
     * @param out aParams the type TVoiceMailboxParams which should include
     *          the service id and the type of seclected TVmbxType
     * @param out aResult the result user seclected
     */
    /*virtual void ShowCallSelectionDialogL(
                const RPointerArray<CVoiceMailboxEntry>& aArray,
                //CArrayPtr<CGulIcon>* aIcons,
                TVoiceMailboxParams& aParams, TInt& aResult ) = 0;*/

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
     * show an error dialog for invalid number
     *
     * @since S60 v5.2
     */
    virtual void ShowInvalidNumberNote() = 0;

    /**
     * show an error dialog for invalid number
     *
     * @since S60 v5.2
     */
    virtual void ShowInvalidWarningNote() = 0;

    /**
     * Show save empty information note
     *
     * @since S60 v5.2
     * @param aType TVmbxType type
     */
     virtual void ShowSaveEmptyNote( const TVmbxType& aType ) = 0;
    };

#endif // M_MVMBXUIHANDLER_H
