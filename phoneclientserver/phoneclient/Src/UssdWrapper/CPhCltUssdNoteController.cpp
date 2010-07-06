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
* Description:  Ussd note controller
*
*/


// INCLUDE FILES
#include <hbdevicemessageboxsymbian.h>
#include <hbdeviceprogressdialogsymbian.h>
#include <hbtextresolversymbian.h>

#include "phcltclientserver.h" 
#include "cphcltussdnotecontroller.h" 
#include "tflogger.h"

_LIT(KUssdLocFilename, "ussd_");
_LIT(KCommonLocFilename, "common_");
_LIT(KPath, "z:\\resource\\qt\\translations");
_LIT(KUssdRequesting, "txt_common_info_requesting"); // Requesting
_LIT(KUssdDone, "txt_ussd_dpopinfo_done"); // Done
_LIT(KUssdNotDone, "txt_ussd_dpopinfo_not_done"); // NotDone
_LIT(KUssdNotAllowed, "txt_ussd_dpopinfo_not_allowed"); //NotAllowed
_LIT(KUssdUnconfirmed, "txt_ussd_dpopinfo_unconfirmed"); // Unconfirmed
_LIT(KUssdNoService, "txt_ussd_dpopinfo_no_service"); // NoService
_LIT(KUssdOffline, "txt_ussd_dpopinfo_offline_not_possible"); // Offline
_LIT(KUssdHide, "txt_common_button_hide"); // Hide

const int KPhCltUssdProgressBarMaxLength = 10;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCltUssdNoteController* CPhCltUssdNoteController::NewL(
        MPhCltUssdNoteControllerCallBack& aCallBack )
    {
    TFLOGSTRING("CPhCltUssdNoteController: NewL call")
    CPhCltUssdNoteController* self = new( ELeave ) 
        CPhCltUssdNoteController( aCallBack );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    TFLOGSTRING("CPhCltUssdNoteController: NewL exit")
    return self;
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::ConstructL()
    {
    TFLOGSTRING( "CPhCltUssdNoteController: ConstructL call_exit" ) 
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::CPhCltUssdNoteController
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CPhCltUssdNoteController::CPhCltUssdNoteController(
    MPhCltUssdNoteControllerCallBack& aCallBack )
    : iGlobalWaitNote( NULL ),
      iCallBack( aCallBack ),
      iIsResolverSuccess( EFalse ),
      iGlobalResource( NULL )
    {
    TFLOGSTRING("CPhCltUssdNoteController: CPhCltUssdNoteController call")
    TFLOGSTRING("CPhCltUssdNoteController: CPhCltUssdNoteController exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController:~CPhCltUssdNoteController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltUssdNoteController::~CPhCltUssdNoteController()
    {
    TFLOGSTRING("CPhCltUssdNoteController: ~CPhCltUssdNoteController call")
    DestroyGlobalWaitNote();
    TFLOGSTRING("CPhCltUssdNoteController: ~CPhCltUssdNoteController exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowGlobalInformationNoteL
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::ShowGlobalInformationNoteL( 
        const TPhCltUssdInformationType aInfoType )
    {
    TFLOGSTRING2("CPhCltUssdNoteController: ShowGlobalInformationNoteL\
            aInfoType = %d call", aInfoType)
    iIsResolverSuccess = HbTextResolverSymbian::Init( KUssdLocFilename, KPath );
    TFLOGSTRING2("CPhCltUssdNoteController: ShowGlobalInformationNoteL\
        ussd iIsResolverSuccess = %d", iIsResolverSuccess ) 
    HBufC* temp(NULL);
    switch ( aInfoType )
        {
        case EPhCltUssdUnconfirme:
            {
            temp = LoadDefaultStringL(KUssdUnconfirmed);
            break;
            }
        case EPhCltUssdNotallowed:
            {
            temp = LoadDefaultStringL(KUssdNotAllowed);
            break;
            }
        case EPhCltUssdNoservice:
            {
            temp = LoadDefaultStringL(KUssdNoService);
            break;
            }
        case EPhCltUssdOffline:
            {
            temp = LoadDefaultStringL(KUssdOffline);
            break;
            }
        case EPhCltUssdDone:
            {
            temp = LoadDefaultStringL(KUssdDone);
            break;
            }
        case EPhCltUssdNotDone:
            {
            temp = LoadDefaultStringL(KUssdNotDone);
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            }
            break;
        }
    CHbDeviceMessageBoxSymbian* dlg = CHbDeviceMessageBoxSymbian::NewL(
            CHbDeviceMessageBoxSymbian::EInformation);
    CleanupStack::PushL( dlg );
    dlg->SetTextL( temp->Des() );
    dlg->SetButton( CHbDeviceMessageBoxSymbian::EAcceptButton,
             ETrue );
    dlg->ExecL();
    CleanupStack::PopAndDestroy( dlg );
    delete temp;
    temp = NULL;
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalInformationNoteL exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ShowGlobalWaitNoteL
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::ShowGlobalWaitNoteL( )
    {
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL call")
    DestroyGlobalWaitNote();
    iIsResolverSuccess = HbTextResolverSymbian::Init( KCommonLocFilename, KPath );
    TFLOGSTRING2("CPhCltUssdNoteController: ConstructL\
        init common iIsResolverSuccess = %d", iIsResolverSuccess ) 
    //CHbDeviceProgressDialogSymbian
    iGlobalWaitNote = CHbDeviceProgressDialogSymbian::NewL(
            CHbDeviceProgressDialogSymbian::EProgressDialog );
    CleanupStack::PushL( iGlobalWaitNote );
    iGlobalResource = LoadDefaultStringL( KUssdRequesting );
    iGlobalWaitNote->SetTextL( iGlobalResource->Des() );
    iGlobalWaitNote->SetObserver( this );
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL before setactive")
    iGlobalWaitNote->SetRange(0,KPhCltUssdProgressBarMaxLength);
    iGlobalWaitNote->SetProgressValue( KPhCltUssdProgressBarMaxLength );
    iGlobalWaitNote->SetAutoClose( EFalse );
    iGlobalWaitNote->SetButtonTextL( LoadDefaultStringL( KUssdHide )->Des() );
    iGlobalWaitNote->ShowL();
    CleanupStack::Pop( iGlobalWaitNote );
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL after setactive")
    TFLOGSTRING("CPhCltUssdNoteController: ShowGlobalWaitNoteL exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::DestroyGlobalWaitNote
// -----------------------------------------------------------------------------
void CPhCltUssdNoteController::DestroyGlobalWaitNote()
    {
    TFLOGSTRING("CPhCltUssdNoteController: DestroyGlobalWaitNote call")
    if ( iGlobalWaitNote )
        {
        iGlobalWaitNote->Close();
        delete iGlobalWaitNote;
        iGlobalWaitNote = NULL;
        delete iGlobalResource;
        iGlobalResource = NULL;
        }
    TFLOGSTRING("CPhCltUssdNoteController: DestroyGlobalWaitNote exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ProgressDialogCancelled
//
// 
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::ProgressDialogCancelled(
        const CHbDeviceProgressDialogSymbian* /*aProgressDialog*/)
    {
    TFLOGSTRING("CPhCltUssdNoteController: ProgressDialogCancelled call")
    iCallBack.GlobalWaitNoteHidden();
    delete iGlobalResource;
    iGlobalResource = NULL;
    TFLOGSTRING("CPhCltUssdNoteController: ProgressDialogCancelled exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::ProgressDialogClosed
//
// 
// -----------------------------------------------------------------------------
//
void CPhCltUssdNoteController::ProgressDialogClosed(
        const CHbDeviceProgressDialogSymbian* /*aProgressDialog*/)
    {
    TFLOGSTRING("CPhCltUssdNoteController: ProgressDialogClosed call")
    TFLOGSTRING("CPhCltUssdNoteController: ProgressDialogClosed exit")
    }

// -----------------------------------------------------------------------------
// CPhCltUssdNoteController::LoadDefaultString
//
// 
// -----------------------------------------------------------------------------
//
HBufC* CPhCltUssdNoteController::LoadDefaultStringL( const TDesC& aText )
    {
    TFLOGSTRING("CPhCltUssdNoteController: LoadDefaultString call")
    HBufC* newText = NULL;
    if ( iIsResolverSuccess )
        {
        newText = HbTextResolverSymbian::LoadL( aText );
        }
    else
        {
        newText = aText.AllocL();
        TFLOGSTRING("CPhCltUssdNoteController: LoadDefaultString load failed.");
        }
    TFLOGSTRING("CPhCltUssdNoteController: LoadDefaultString exit")
    return newText;
    }

// End of file

