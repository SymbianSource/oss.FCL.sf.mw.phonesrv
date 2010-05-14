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
* Description:
*
*
*/

// qt
#include <QString>
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>

#include "vmbxqtuihandler.h"
#include "voicemailboxdefsinternal.h"
#include "vmbxuihandler.h"
#include "vmbxlogger.h"

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// CVmbxUiHandler::NewL
// (Constructor).
// ----------------------------------------------------------------------------
//
CVmbxUiHandler* CVmbxUiHandler::NewL()
{
    VMBLOGSTRING( "CVmbxUiHandler::CVmbxUiHandler" )
    CVmbxUiHandler* handler = new( ELeave ) CVmbxUiHandler();
    CleanupStack::PushL( handler );
    handler->ConstructL();
    CleanupStack::Pop( handler );
    VMBLOGSTRING( "CVmbxUiHandler::CVmbxUiHandler Exit" )
    return handler;
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ConstructL
// (Constructor).
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ConstructL()
{
    VMBLOGSTRING( "CVmbxUiHandler::ConstructL" )
    iHandler = new VmbxQtUiHandler;
    VMBLOGSTRING( "CVmbxUiHandler::ConstructL Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::CVmbxUiHandler
// (Constructor).
// ----------------------------------------------------------------------------
//
CVmbxUiHandler::CVmbxUiHandler()
{
    VMBLOGSTRING( "CVmbxUiHandler::CVmbxUiHandler" )
    VMBLOGSTRING( "CVmbxUiHandler::CVmbxUiHandler Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::~CVmbxUiHandler
// (Destructor).
// ----------------------------------------------------------------------------
//
CVmbxUiHandler::~CVmbxUiHandler()
{
    VMBLOGSTRING( "CVmbxUiHandler::~CVmbxUiHandler" )
    delete iHandler;
    VMBLOGSTRING( "CVmbxUiHandler::~CVmbxUiHandler Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowVmbxQueryDialog
// (Show query dialog).
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowVmbxQueryDialog(const TVmbxType& aType,
                          TDes& aNumber, TInt& aResult)
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowVmbxQueryDialog" )
    QString vmbxNumber;
    if ( aNumber.Length() > 0 )
        {
        vmbxNumber=QString::fromUtf16(aNumber.Ptr(), aNumber.Length());
        }
    QT_TRYCATCH_LEAVING(
        iHandler->showVmbxQueryDialog( aType, vmbxNumber, aResult ));
    aNumber = vmbxNumber.utf16();
    VMBLOGSTRING2( "CVmbxUiHandler::ShowVmbxQueryDialog:\
        aNumber = %S", &aNumber );
    VMBLOGSTRING( "CVmbxUiHandler::ShowVmbxQueryDialog show over" )
    VMBLOGSTRING( "CVmbxUiHandler::ShowVmbxQueryDialog Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowVmbxQueryDialog
// (Show query dialog).
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowDefineSelectionDialog( TVmbxType& aType,
       TInt& aResult )
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowDefineSelectionDialog" )
    QT_TRYCATCH_LEAVING(iHandler->showDefineSelectionDialog( aType, aResult ));
    VMBLOGSTRING2( "CVmbxUiHandler::ShowDefineSelectionDialog:\
        aType = %d", aType );
    VMBLOGSTRING2( "CVmbxUiHandler::ShowDefineSelectionDialog:\
        aResult = %d", aResult );
    VMBLOGSTRING( "CVmbxUiHandler::ShowDefineSelectionDialog Exit" )

}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowCallSelectionDialogL
// (Show query dialog).
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowCallSelectionDialogL(
            const RPointerArray<CVoiceMailboxEntry>& aArray,
            TVoiceMailboxParams& aParams, TInt& aResult )
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowCallSelectionDialogL" )
    TInt count = aArray.Count();
    VMBLOGSTRING2("CVmbxUiHandler::showCallSelectionDialogL count = %d", count)
    if (count < 1) 
        {
        VMBLOGSTRING("CVmbxUiHandler::showCallSelectionDialogL leave<=")
        User::Leave(KErrArgument);
        }
    QList<CVoiceMailboxEntry* > entryList;
    for ( int i = 0; i < aArray.Count(); i++ ) 
        {
        entryList.append(aArray[i]);
        }
    QT_TRYCATCH_LEAVING(
        iHandler->showCallSelectionDialog( entryList, aParams, aResult ));
    VMBLOGSTRING2( "CVmbxUiHandler::ShowCallSelectionDialogL:\
        aResult = %d", aResult );
    VMBLOGSTRING( "CVmbxUiHandler::ShowCallSelectionDialogL" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowInformationdNote
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowInformationdNoteL(const TVmbxNoteType aType)
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowInformationdNoteL" )
    QT_TRYCATCH_LEAVING(iHandler->showInformationNote(aType));
    VMBLOGSTRING( "CVmbxUiHandler::ShowInformationdNoteL Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowInvalidWarningNote
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowInvalidWarningNoteL()
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowInvalidWarningNoteL" )
    QT_TRYCATCH_LEAVING(iHandler->showInformationNote( EInvalidNumber ));
    VMBLOGSTRING( "CVmbxUiHandler::ShowInvalidWarningNoteL Exit" )
}

//End of file
