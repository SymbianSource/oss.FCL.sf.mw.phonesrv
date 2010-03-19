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
*
*
*/

// qt
#include <QString>

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
                          const TVmbxQueryMode& aMode,
                          TDes& aNumber, TInt& aResult)
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowVmbxQueryDialog" )
    QString vmbxNumber;
    if ( aNumber.Length() > 0 )
        {
        vmbxNumber=QString::fromUtf16(aNumber.Ptr(), aNumber.Length());
        }
    iHandler->showVmbxQueryDialog( aType, aMode, vmbxNumber, aResult );
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
    iHandler->showDefineSelectionDialog( aType, aResult );
    VMBLOGSTRING2( "CVmbxUiHandler::ShowDefineSelectionDialog:\
        aType = %d", aType );
    VMBLOGSTRING2( "CVmbxUiHandler::ShowDefineSelectionDialog:\
        aResult = %d", aResult );
    VMBLOGSTRING( "CVmbxUiHandler::ShowDefineSelectionDialog Exit" )

}
// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowSaveToPhoneNote
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowSaveToPhoneNote()
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowSaveToPhoneNote" )
    iHandler->showInformationNote( ESavedToPhoneMemory );
    VMBLOGSTRING( "CVmbxUiHandler::ShowSaveToPhoneNote Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowSaveToSimNote
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowSaveToSimNote()
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowSaveToSimNote" )
    iHandler->showInformationNote( ESavedToSimMemory );
    VMBLOGSTRING( "CVmbxUiHandler::ShowSaveToSimNote Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowVideoSavedNote
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowVideoSavedNote()
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowVideoSavedNote" )
    iHandler->showInformationNote( EVideoNumberSaved );
    VMBLOGSTRING( "CVmbxUiHandler::ShowVideoSavedNote Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowInvalidNumberNote
// ----------------------------------------------------------------------------
//

void CVmbxUiHandler::ShowInvalidNumberNote()
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowInvalidNumberNote" )
    iHandler->showInformationNote( EInvalidNumber );
    VMBLOGSTRING( "CVmbxUiHandler::ShowInvalidNumberNote Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowInvalidWarningNote
// ----------------------------------------------------------------------------
//

void CVmbxUiHandler::ShowInvalidWarningNote()
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowInvalidWarningNote" )
    iHandler->showInformationNote( EInvalidNumber );
    VMBLOGSTRING( "CVmbxUiHandler::ShowInvalidWarningNote Exit" )
}

// ----------------------------------------------------------------------------
// CVmbxUiHandler::ShowSaveEmptyNote
// ----------------------------------------------------------------------------
//
void CVmbxUiHandler::ShowSaveEmptyNote( const TVmbxType& aType )
{
    VMBLOGSTRING( "CVmbxUiHandler::ShowSaveEmptyNote" )
    iHandler->showSaveEmptyNote( aType );
    VMBLOGSTRING( "CVmbxUiHandler::ShowSaveEmptyNote Exit" )
}

//End of file
