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
* Description: Active object wrapper to handle popup note.
*
*/

// System includes
#include <avkon.hrh>
#include <aknglobalconfirmationquery.h> 
#include <bautils.h>
#include <pathinfo.h>

// Includes
#include "networknotifiernotewrapper.h"

// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CNetworkNotifierNoteWrapper* CNetworkNotifierNoteWrapper::NewL( MNoteWrapperObserver& aObserver,
                                                                TInt aResID )
    {
    CNetworkNotifierNoteWrapper* self = new( ELeave ) CNetworkNotifierNoteWrapper( aObserver,
                                                                                   aResID );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );    
    return self;
    }

// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CNetworkNotifierNoteWrapper::ConstructL()
    {
    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::ConstructL() -START") );
    
    User::LeaveIfError( iFs.Connect() );
    
    HBufC* note = ReadResourceLC();
    
    if( note )
        {
        DisplayPopupNoteL( note );
        CleanupStack::PopAndDestroy( note );
        }
    
    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::ConstructL() -END") );
    }


// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::CNetworkNotifierNoteWrapper
// 
// -----------------------------------------------------------------------------
//
CNetworkNotifierNoteWrapper::CNetworkNotifierNoteWrapper( MNoteWrapperObserver& aObserver, 
                                                          TInt aResID ) : 
                                                          CActive( EPriorityStandard ), 
                                                          iObserver( aObserver ) 
    {
    iErrNotes.Insert( aResID, 0 );
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::~CNetworkNotifierNoteWrapper
// 
// -----------------------------------------------------------------------------
//
CNetworkNotifierNoteWrapper::~CNetworkNotifierNoteWrapper()
    {
    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::~CNetworkNotifierNoteWrapper() -START") );
    
    iFs.Close();
    iErrNotes.Close(); // cleanup and close
    
    Cancel();
    delete iGlobalQuery;
    
    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::~CNetworkNotifierNoteWrapper() -END") );
    }


// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::AppendNote
//
// Monitor inserts the latest error notification err# from ETEL to this stack
// -----------------------------------------------------------------------------
//
void CNetworkNotifierNoteWrapper::AppendNote( const TInt aResID )
    {
    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::AppendNote() -START") );
    iErrNotes.Insert( aResID, 0 );
    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::AppendNote() -END") );
    }


// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::DisplayPopupNoteL
// 
// -----------------------------------------------------------------------------
//
void CNetworkNotifierNoteWrapper::DisplayPopupNoteL( HBufC* aNote )
    {
    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::DisplayPopupNoteL() -START") );
    
    if( !IsActive() )
        {   
        if( !iGlobalQuery )
			{
            iGlobalQuery = CAknGlobalConfirmationQuery::NewL();
        	}
		if( aNote )
			{
        	iGlobalQuery->ShowConfirmationQueryL( iStatus, 
                                                  *aNote, 
                                                  R_AVKON_SOFTKEYS_OK_EMPTY, 
                                                  R_QGN_NOTE_WARNING_ANIM );
			}
		
        SetActive();
        }

    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::DisplayPopupNoteL() -END") );
    }

// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::ReadResourceLC
//
// Just to read localized error strings from resource 
//
// -----------------------------------------------------------------------------
//
HBufC* CNetworkNotifierNoteWrapper::ReadResourceLC()
    {
    RDebug::Print(
            _L("NW: CNetworkNotifierNoteWrapper::ReadResourceLC() -START") );

    TInt resId;
    RResourceFile resFile;
    TResourceReader reader;

    TFileName fullFileName;
    TDriveUnit driveNum = PathInfo::RomRootPath();

    fullFileName.Insert( 0, driveNum.Name() );
    fullFileName.Append( KResFile );

    TFileName file( fullFileName );
    HBufC16* textBuffer = NULL;
    
    // We are interested in the most recent err note at a time
    resId = iErrNotes[0];

    // Err# can be removed from the stack now
    iErrNotes.Remove( 0 );
        
    BaflUtils::NearestLanguageFile( iFs, file );   
    resFile.OpenL( iFs, file );
    // According to SDK docs, resFile will be closed if this function leaves
    resFile.ConfirmSignatureL();
    
    HBufC8* readBuffer = resFile.AllocReadLC( resId );
    reader.SetBuffer( readBuffer );
    
    TPtrC textdata = reader.ReadTPtrC();
    textBuffer = HBufC16::NewL( textdata.Length() );
    *textBuffer = textdata;
    
    CleanupStack::PopAndDestroy( readBuffer );
    CleanupStack::PushL( textBuffer );
    
    resFile.Close();
        
    RDebug::Print(
            _L("NW: CNetworkNotifierNoteWrapper::ReadResourceLC() -END"));
    return textBuffer;
    }


// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::RunL
// Pure virtuals from CActive implemented in this derived class.
// iStatus error handling is not needed here, since the service provider for
// this AO (in DisplayPopupNoteL) will leave in case of an error, and will 
// therefore be handled in RunError.
// -----------------------------------------------------------------------------
//
void CNetworkNotifierNoteWrapper::RunL()
    {
    RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::RunL -START") );
        
    if( iErrNotes.Count() ) // There are more errors to display: 
        { 
        HBufC* note = ReadResourceLC();
        
        if ( note )
            {
            DisplayPopupNoteL( note );
            CleanupStack::PopAndDestroy( note );
            }
        }
    else
        { 
        // Error note stack is empty, since this task has completed and let monitor 
        // to destruct this instance
        RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::RunL : No more notes to display") );
        iObserver.PopupNoteClosed();
        }
    }


// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::DoCancel
// 
// -----------------------------------------------------------------------------
//
void CNetworkNotifierNoteWrapper::DoCancel()
    {
    if( iGlobalQuery )
        {
        iGlobalQuery->CancelConfirmationQuery();
        }
    }

// -----------------------------------------------------------------------------
// CNetworkNotifierNoteWrapper::RunError()
// 
// -----------------------------------------------------------------------------
//
TInt CNetworkNotifierNoteWrapper::RunError( TInt aError )
    {
    // Resource reader failed in ReadResourceLC
    // Just reissue the request
    if ( aError )
        {
        RDebug::Print( _L("NW: CNetworkNotifierNoteWrapper::RunError: Showing of the note failed!") );
        }
    
    return KErrNone;
    }

// end of file
