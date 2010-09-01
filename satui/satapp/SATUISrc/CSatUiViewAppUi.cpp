/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*    Implements the application UI and the methods derived from 
*    MSatUiActionImplementer i.e. showing messages and asking input. 
*
*
*/


// INCLUDE FILES
#include    <barsread.h>
#include    <badesca.h> // for descriptor arrays
#include    <e32std.h>
#include    <eikenv.h>
#include    <eikapp.h>
#include    <StringLoader.h>
#include    <aknview.h>
#include    <aknnotedialog.h>
#include    <aknstaticnotedialog.h>
#include    <AknGlobalNote.h>
#include    <AknQueryDialog.h>
#include    <aknmessagequerydialog.h>
#include    <AudioPreference.h>
#include    <mdaaudiotoneplayer.h>
#include    <activitymanager.h>
#include    <AknIconArray.h>
#include    <AknPopupform.h>                //DisplayText icon popupform
#include    <eikcapc.h>                     //CEikCaptionedControl
#include    <txtrich.h>                     //CRichText
#include    <eikrted.h>                     //CEikRichTextEditor
#include    <hlplch.h>
#include    <aknsoundsystem.h>
#include    <AknWaitDialog.h>
#include    <akntitle.h>
#include    <AknUtils.h>
#include    <AknIconUtils.h>
#include    <aknconsts.h>
#include    <avkon.hrh>
#include    <SatUi.rsg>
#include    <msatuiadapter.h>
#include    <aknkeylock.h>

#include    "satui.hrh"
#include    "CSatUiApplication.h"
#include    "CSatUiViewAppUi.h"
#include    "CSatUiView.h"
#include    "CSatUiTextQueryDialog.h"
#include    "CSatUiMessageQueryIcon.h" 
#include    "CSatUiWaitDialog.h"
#include    "tflogger.h"

#include    <MProfile.h>
#include    <MProfileEngine.h>
#include    <MProfileTones.h>
#include    <TProfileToneSettings.h>
#include    <MProfileName.h>
#include    <MProfileExtraTones.h>
#include    <featmgr.h>                     // Used for FeatureManager
#include    <BTSapDomainPSKeys.h>           // For BT SAP connection state keys.
#include    <e32property.h>                 // Used for RProperty
#include    <aknnotewrappers.h>

#include    <aknlayout.cdl.h> 
#include    <aknlayoutscalable_avkon.cdl.h>
#include    <ctsydomainpskeys.h>
#include    <mda/common/resource.h>

#include    <ProfileEngineSDKCRKeys.h>
#include    <centralrepository.h>


// CONSTANTS
const TInt KSatActiveProfileOffline = 5; // value from ProfileEngine
const TInt K32TIntMaxValue = 0x7FFFFFFF;
// From UI spec 60 sec delay user response.
const TInt KNoResponseFromUserDelay = 60; 
const TInt KMicroSecondsInSecond = 1000000;
const TInt KClearScreenDelay = 3;
const TInt KMicroSecondsInTenthOfSecond = 100000;
const TInt K3Seconds = 3000000;

// Maximum length for sound file. 
const TInt KMaxSoundFileLength = 256;
// Audio sample is repeated indefinitely.
const TInt KSoundPlayerRepeatForever = KMdaRepeatForever;
// Used when dividing some values. Used only for preventing the use of magic 
// numbers
const TInt KDivideByEight = 8;

const TUint8 KGetInputStringMaxSize = 0xEF;

_LIT( K3Spaces, "   " );                    // Used for empty title.
const TInt KEmptyTitleLength( 3 );          // Length of empty title.                      
_LIT( KSatEmptySpace, " " );                // Used for empty item.
_LIT( KSatEmptyDes, "" );

const TUint8 KKeyZero = 0x30;
const TUint8 KKeyNine = 0x39;
const TUint8 KHwAsterisk = 0x2a;    // In hw scan code value for asterisk

// The max volume value from settings.
// From TProfileToneSettings.h
const TInt KMaxVolumeFromProfile( 10 );

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSatUiViewAppUi::CSatUiViewAppUi():
    iHelp( EFalse ),
    iSetUpMenuFirst( ETrue ),
    iSelectItemFirst( ETrue ),
    iPreviousForeground( EFalse ),
    iViewLoopCounter( 0 ),
    iDuration( TTimeIntervalMicroSeconds( static_cast<TInt64>( 0 ) ) ),
    iReqWSBufferID( KErrNone )
    {
    }

// Symbian OS default constructor can leave.
void CSatUiViewAppUi::ConstructL()
    {
    TFLOGSTRING( "CSatUiViewAppUi::ConstructL() called" )
    BaseConstructL( EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible );
    
    // Requests Window Server client buffer used by the application to
    // be of at least the given size.
    iReqWSBufferID = CAknEnv::Static()->RequestWsBuffer( KAknMaxWsBufferSize );
    TFLOGSTRING2( "iReqWSBufferID is %d", iReqWSBufferID )

    if ( BTSAPActivatedL() )
        {
        HBufC* sapText = 
            iCoeEnv->AllocReadResourceLC( R_QTN_OFFLINE_NOT_POSSIBLE_SAP );
        CAknInformationNote* note = new ( ELeave ) CAknInformationNote( ETrue );
        iForegroundControl = note;
        note->ExecuteLD( *sapText );
        iForegroundControl = NULL;
        CleanupStack::PopAndDestroy( sapText );
        Exit();
        }

    if ( ProfileState() == KSatActiveProfileOffline )
        {
        ShowNotAvailableNoteL();
        Exit();        
        }

    
    if ( StatusPane()->PaneCapabilities( TUid::Uid(
        EEikStatusPaneUidTitle ) ).IsPresent() )
        {
        CAknTitlePane* titlePane = STATIC_CAST( CAknTitlePane*,
            StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
        titlePane->SetTextL( KNullDesC );
        titlePane->DrawNow();
        }
        
    TFLOGSTRING( "CSatUiViewAppUi::ConstructL() exit" )
    }

// Destructor.
CSatUiViewAppUi::~CSatUiViewAppUi()
    {
    TFLOGSTRING( "CSatUiViewAppUi::~CSatUiViewAppUi() called" )
    this->StopShowWaitNote();

    delete iTitle;
    delete iAppName;
    delete iPlayer;
    delete iTimer;    
    
    TFLOGSTRING( "CSatUiViewAppUi::~CSatUiViewAppUi() exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::CreateEmptyMenuL
// Creates empty menu. This is used in SIM originated commands.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::CreateEmptyMenuL()
    {
    TFLOGSTRING( "CSatUiViewAppUi::CreateEmptyMenuL() called" )

    // Create the menu item array with no content.
    CDesCArrayFlat* menuItems = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( menuItems );
    menuItems->Reset();
    menuItems->AppendL( KSatEmptySpace );

    // Create the empty next action indicator array.
    CArrayFixFlat<TSatAction>* menuActions =
        new( ELeave ) CArrayFixFlat<TSatAction>( 1 );
    CleanupStack::PushL( menuActions );
    menuActions->Reset();

    // Create empty title pane.
    HBufC* satTitle = HBufC::NewLC( KEmptyTitleLength );
    satTitle->Des().Append( K3Spaces );

    iSetUpMenuFirst = EFalse;
    iSetUpMenuView = CSatUiView::NewL( ETrue, this, ETrue );
    iSetUpMenuView->SetItemsL( satTitle, 
        *menuItems, 
        menuActions, 
        EFalse );
        
    // Hide list box.
    iSetUpMenuView->MakeListBoxVisible( EFalse );
    
    // Transfer ownership to CAknViewAppUi.
    AddViewL( iSetUpMenuView );

    iSetUpMenuView->SetAdapter( iAdapter );
    iActiveView = KSetUpMenuViewId; 
    
    // satTitle, menuActions, menuItems  
    CleanupStack::PopAndDestroy( satTitle );
    CleanupStack::PopAndDestroy( menuActions );
    CleanupStack::PopAndDestroy( menuItems );
    TFLOGSTRING( "CSatUiViewAppUi::CreateEmptyMenuL() exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::SetAdapter
// Sets the pointer to MSatUiAdapter object.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::SetAdapter(
    MSatUiAdapter* aAdapter )
    {
    iAdapter = aAdapter;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ShowWaitNoteL
// Displays the wait note.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ShowWaitNoteL()
    {
    TFLOGSTRING( "CSatUiViewAppUi::ShowWaitNoteL() called" )
    if ( !iWaitNote )
        {
        iWaitNote = new(ELeave)CAknWaitDialog(
            (REINTERPRET_CAST(CEikDialog**,&iWaitNote)));
        iForegroundControl = iWaitNote;
        iWaitNote->ExecuteLD( R_SATUI_WAIT_NOTE );
        // iForegroundControl won't be clear until the wait dialog
        // closed. It will avoid we lose the chance to set focus for
        // the wait dialog when the switch of foreground/background
        // happened.
        }
    TFLOGSTRING( "CSatUiViewAppUi::ShowWaitNoteL() exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ShowWaitNoteWithoutDelayL
// Displays the wait note without delay
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ShowWaitNoteWithoutDelayL()
    {
    TFLOGSTRING( "CSatUiViewAppUi::ShowWaitNoteWithoutDelayL() called" )
    if ( !iWaitNote )
        {
        iWaitNote = new(ELeave)CAknWaitDialog(
            (REINTERPRET_CAST(CEikDialog**,&iWaitNote)), ETrue );
        iForegroundControl = iWaitNote;
        iWaitNote->ExecuteLD( R_SATUI_WAIT_NOTE );
        // iForegroundControl won't be clear until the wait dialog
        // closed. It will avoid we lose the chance to set focus for
        // the wait dialog when the switch of foreground/background
        // happened.
        }
    TFLOGSTRING( "CSatUiViewAppUi::ShowWaitNoteWithoutDelayL() exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::StopShowWaitNote
// Removes the wait note from the screen.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::StopShowWaitNote()
    {
    TFLOGSTRING( "CSatUiViewAppUi::StopShowWaitNote() called" )

    if ( iNoteDialog )
        {
        iForegroundControl = NULL;

        TFLOGSTRING( "CSatUiViewAppUi::StopShowWaitNote() Closing \
            NoteDialog" )
        delete iNoteDialog;
        iNoteDialog = NULL;
        }

    if ( iWaitNote )
        {
        iForegroundControl = NULL;

        TFLOGSTRING( "CSatUiViewAppUi::StopShowWaitNote() Closing WaitNote" )
        // Process finished. The function was either ExecuteLD or RunLD.
        // This function call stops the execution of these function and 
        // causes the deletion of object
        TInt err(0);
        TRAP( err, iWaitNote->ProcessFinishedL() )
        TFLOGSTRING2(" 2nd: iWaitNote: %x ", iWaitNote )
        TFLOGSTRING2( "TRAP: %d", err )
        // Delete the object just in case. Seems that above method does not 
        // delete the object in some cases. e.g in very fast execution
        delete iWaitNote;
        iWaitNote = NULL;
        }
    if ( iBipWaitNote )
        {
        iForegroundControl = NULL;

        TFLOGSTRING( "CSatUiViewAppUi::StopShowWaitNote() Closing BIP note" )
        delete iBipWaitNote;
        iBipWaitNote = NULL;
        }
        
    if ( iPermanentNote )
        {
        TFLOGSTRING( "CSatUiViewAppUi::StopShowWaitNote() \
               iPermanentNote cleared" )
        // If iPermanentNote exists until wait note is stopped,
        // then dialog has been already removed.
        iPermanentNote->ExitSleepingDialog();
        delete iPermanentNote;
        iPermanentNote = NULL;
        }
        
    TFLOGSTRING( "CSatUiViewAppUi::StopShowWaitNote() exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::BTSAPActivatedL
// Check that if BT SAP connection activated.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CSatUiViewAppUi::BTSAPActivatedL()
    {

    TFLOGSTRING( "CSatUiViewAppUi::BTSAPActivated() called" )

    TBool isActive( EFalse );
    FeatureManager::InitializeLibL();
    if ( FeatureManager::FeatureSupported( KFeatureIdBtSap ) )
        {
        TFLOGSTRING( "BTSAP is supported by ME" )
        TInt status( EBTSapNotConnected );
        TInt err = RProperty::Get( KPSUidBluetoothSapConnectionState, 
                                   KBTSapConnectionState, 
                                   status );

        if ( EBTSapNotConnected != status && !err )
            {
            TFLOGSTRING( "BTSAP is connected" )
            isActive = ETrue;
            }
        else
            {
            TFLOGSTRING3( "BTSAP status %d and error %d", status, err )
            }
        }
    else
        {
        TFLOGSTRING( "BTSAP is not supported by ME" )
        }

    FeatureManager::UnInitializeLib();
    TFLOGSTRING( "CSatUiViewAppUi::BTSAPActivated() exit" )
    return isActive;

    }

// ---------------------------------------------------------
// CSatUiViewAppUi::CoeEnv
// Returns CoeEnv.
// (other items were commented in a header).
// ---------------------------------------------------------
//
CCoeEnv* CSatUiViewAppUi::CoeEnv()
    {
    return iCoeEnv;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::DisplayTextL
// Displays the text in a note. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::DisplayTextL(
    const TDesC& aText,
    const TDesC& aSimApplicationName,
    CFbsBitmap* aIconBitmapDisplayText,
    const TBool aSelfExplanatoryIcon,
    const TBool aSustainedText,
    const TTimeIntervalSeconds aDuration,
    const TBool aWaitUserToClear )
    {
    TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL called" )
    StopShowWaitNote();

    TSatUiResponse response( ESatSuccess );
    iHighPriorityEvent = EFalse;
    
    // Indication immediate response TLV for Display Text during MT call,
    // if it is immediate response, close dialog, otherwise leave it alone.
    iImmediateRsp = aSustainedText;
    TFLOGSTRING2( "CSatUiViewAppUi::DisplayTextL immediate rsp: %d", 
                   iImmediateRsp )    
    
    HBufC* header;

    if ( iHelp )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL iHelp ETrue" )
        header = StringLoader::LoadLC(
            R_QTN_SAT_HELP_HEADER, iCoeEnv );
        iHelp = EFalse;
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL iHelp EFalse" )
        TFLOGSTRING2( "CSatUiViewAppUi::DisplayTextL length of \
            aSimApplicationName: %d", aSimApplicationName.Length() )
        if ( aSimApplicationName.Length() > 0 )
            {
            header = aSimApplicationName.AllocLC();
            }
        else
            {
            //Reads a resource string with memory allocation
            header = StringLoader::LoadLC( R_QTN_SAT_MESSAGE_HEADER, iCoeEnv );
            }
        }

    // Remove NULL characters from title
    RemoveNullCharacters( header );

    if ( aIconBitmapDisplayText )
        {
        //icon available use querydialog
        iDisplayTextIconDialog = CAknPopupForm::NewL();
        }

    CUserActivityManager* activityManager = 
        CUserActivityManager::NewL( EPriorityNormal );

    CleanupStack::PushL( activityManager );
    
    // Duration handling: 
    // If duration is given use that value for timeout.
    TTimeIntervalSeconds duration ( aDuration );

    // If user is expected to clear shown dialog and 
    // duration is NOT given use 60s timeout.
    if ( aWaitUserToClear && !aDuration.Int() )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL wait user to clear" )
        duration = TTimeIntervalSeconds( KNoResponseFromUserDelay );
        }
    // If user is NOT expected to clear shown dialog and 
    // duration is NOT given use 3s timeout.
    else if ( !aDuration.Int() )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL don't wait user \
            to clear" )
        duration = TTimeIntervalSeconds( KClearScreenDelay );
        }        

    // If not sustained or Clear after delay requested start timer
    if ( !aSustainedText || aDuration.Int() || !aWaitUserToClear )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL No sustained text" )
        TFLOGSTRING2( "CSatUiViewAppUi::DisplayTextL duration: %d", 
            duration.Int() )
        
        // If duration is given start timer which doesn't 
        // care about user activity.        
        if ( aDuration.Int() )
            {
            CreateCallBackTimerL( duration.Int() * KMicroSecondsInSecond,
                DelayCallback );
            }
        // Otherwise start timer which takes account user activity.
        else
            {
            User::ResetInactivityTime();
            activityManager->Start( duration, 
                TCallBack( DispatchUserInactive, this ), 0 );            
            }
        }

    TInt userResponse;

    if ( aIconBitmapDisplayText )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL have IconBitmap" )
        // Prepare dialog with icon
        iDisplayTextIconDialog->SetPopupFormType( EAknQuery );
        iDisplayTextIconDialog->PrepareLC( R_SATUI_MESSAGE_TEXT_ICON );
        iDisplayTextIconDialog->SetTitleL( *header );

        // Get richtext control
        CEikCaptionedControl* capControl = 
            iDisplayTextIconDialog->GetControlByControlType
            ( EEikCtRichTextEditor );
        CCoeControl* coeControl = NULL;
        if ( capControl )
            {
            coeControl = capControl->iControl;
            }
        if ( !coeControl )
            {
            User::Leave( KErrUnknown );
            }
        CEikRichTextEditor* rtxtEditor = 
            reinterpret_cast<CEikRichTextEditor*>( coeControl );

        if ( !aSelfExplanatoryIcon )
            {
            TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL not \
                SelfExplanatoryIcon" )
            // Not selfexplanatory icon, add text
            // Get font
            const CFont* logicalFont = AknLayoutUtils::FontFromId( 
                EAknLogicalFontSecondaryFont );
            
            // Extract font information
            TFontSpec fontspec = logicalFont->FontSpecInTwips();
            TCharFormat charFormat( fontspec.iTypeface.iName, 
                            fontspec.iHeight );
            
            // Make the font to be anti-aliased
            TFontStyle style;
            style.SetBitmapType( EAntiAliasedGlyphBitmap );
            charFormat.iFontSpec.iFontStyle = style;
                
            // Set mask (which settings are set)
            TCharFormatMask charFormatMask;
            charFormatMask.SetAttrib( EAttColor );
            charFormatMask.SetAttrib( EAttFontTypeface );
            charFormatMask.SetAttrib( EAttFontHeight );
            
            // Apply settings
            rtxtEditor->RichText()->ApplyCharFormatL(
                charFormat, charFormatMask, 0, 0 );
                    
            // One space between icon and text
            rtxtEditor->RichText()->InsertL( 0, KSatEmptySpace );
            rtxtEditor->RichText()->InsertL( 1, aText );
            }

        // Count icon size in twips 
        CWsScreenDevice* device = CCoeEnv::Static()->ScreenDevice();
        TInt width = device->HorizontalPixelsToTwips( 
            aIconBitmapDisplayText->SizeInPixels().iWidth );
        TInt height = device->VerticalPixelsToTwips( 
            aIconBitmapDisplayText->SizeInPixels().iHeight );
        TSize twipsSize( width, height );

        // Create picture
        CSatUiMessageQueryIcon* picture = new( ELeave ) 
            CSatUiMessageQueryIcon( twipsSize, *aIconBitmapDisplayText );
        TPictureHeader pictureHeader;    
        pictureHeader.iPicture = TSwizzle<CPicture>( picture );
        //Insert picture
        rtxtEditor->RichText()->InsertL( 0, pictureHeader );
        //Needed to show whole picture
        _LIT( KString, "\f\f\f");
        rtxtEditor->RichText()->InsertL( rtxtEditor->Text()->DocumentLength(),
            KString );

        // Set MSK same as LSK without any text causing the icon drawing
        iDisplayTextIconDialog->ButtonGroupContainer().SetCommandL(
            CEikButtonGroupContainer::EMiddleSoftkeyPosition, EAknSoftkeyOk,
            KSatEmptyDes );
        iForegroundControl = iDisplayTextIconDialog;
        userResponse = iDisplayTextIconDialog->RunLD();
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL don't have IconBitmap" )
        // Dialog without icon
        iDisplayTextDialog = new (ELeave) CAknMessageQueryDialog();
        iDisplayTextDialog->PrepareLC( R_SATUI_MESSAGE );
        iDisplayTextDialog->SetMessageTextL( aText );
        iDisplayTextDialog->QueryHeading()->SetTextL( *header );

        // Set MSK same as LSK without any text causing the icon drawing
        iDisplayTextDialog->ButtonGroupContainer().SetCommandL(
            CEikButtonGroupContainer::EMiddleSoftkeyPosition, EAknSoftkeyOk,
            KSatEmptyDes );
        iForegroundControl = iDisplayTextDialog;
        userResponse = iDisplayTextDialog->RunLD();
        }
    iForegroundControl = NULL;

    CleanupStack::PopAndDestroy( activityManager );
    CleanupStack::PopAndDestroy( header ); // header,activityManager
    // After duration call back timer is destroyed
    DestroyCallBackTimer();
    
    // If user is not expected to clear message used response is success
    // in timeout case.
    if ( userResponse 
        || ( iNoteClearedAfterDelay && !aWaitUserToClear ) )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL successful response" )
        response = ESatSuccess;
        }
    else if ( iNoResponseFromUserAfterDelay || 
        ( aWaitUserToClear && iHighPriorityEvent ) )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL no user response" )
        // User did not close the text, closed by the timer or MT call
        response = ESatNoResponseFromUser;
        }
    else
        {
        if ( IsAppShutterRunning() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL satsession \
                terminated by user " )
            response = ESatSessionTerminatedByUser;
            iSessionTerminated = ETrue;
            }
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::DisplayTextL backward \
                requested by user " )
            response = ESatBackwardModeRequestedByUser;
            }
        }

    if ( iDisplayTextDialog )
        {
        iDisplayTextDialog = NULL;
        }
    if ( iDisplayTextIconDialog )
        {
        iDisplayTextIconDialog = NULL;
        }

    iNoteClearedAfterDelay = EFalse;
    iNoResponseFromUserAfterDelay = EFalse;

    ShowWaitNoteL();
        
    TFLOGSTRING2( "CSatUiViewAppUi::DisplayTextL exit, return: %d", response )
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ClearScreen
// Displays yes/no confirmation query.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ClearScreen()
    {
    TFLOGSTRING( "CSatUiViewAppUi::ClearScreen() called" )
    
    iNoteClearedAfterDelay = ETrue;
    CloseDialogs();
    
    TFLOGSTRING( "CSatUiViewAppUi::ClearScreen() exit" )
    }
// ---------------------------------------------------------
// CSatUiViewAppUi::GetYesNoL
// Displays yes/no confirmation query.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::GetYesNoL(
    const TDesC& aText,
    const TSatCharacterSet aCharacterSet,
    TChar& aInkey,
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory,
    TUint& aDuration,
    const TBool aImmediateDigitResponse )
    {
    TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL called" )
    TSatUiResponse response( ESatSuccess );

    StopShowWaitNote();

    CUserActivityManager* activityManager = 
        CUserActivityManager::NewL( EPriorityNormal );
    CleanupStack::PushL( activityManager );

    // Start time for duration
    TTime startTime;
    iImmediateDigitResponse = aImmediateDigitResponse;
    
    TFLOGSTRING2( "CSatUiViewAppUi::GetYesNoL duration in 1/10 sec: %d",
            aDuration )

    // If duration is given use that value for timeout.
    // Start timer which doesn't care about user activity.
    if ( aDuration )
        {
        TInt duration ( 0 );
         
        TTimeIntervalMicroSeconds max32Bit(
            static_cast<TInt64>( K32TIntMaxValue ) );        
        TTimeIntervalMicroSeconds durationInTenthOfSeconds( 
            static_cast<TInt64>( aDuration ) );
        TTimeIntervalMicroSeconds durationInus( 
            durationInTenthOfSeconds.Int64() * KMicroSecondsInTenthOfSecond );
        
        if ( durationInus > max32Bit )
            {
            TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL durationInus bigger \
                than max32Bit" )
            // since timers can only handle 32-bit values,
            // let's break the aDuration to smaller bits.
            // we break it here to 8 bits to fit in 32-bit value
            duration = durationInus.Int64() / KDivideByEight;
            iTimerRepeatOn = ETrue;
            }
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL durationInus less \
                than max32Bit" )
            duration = durationInus.Int64();
            }
            
        CreateCallBackTimerL( duration, DelayCallback );
        startTime.HomeTime();
        }
    // Otherwise start timer which takes account user activity.
    else
        {
        User::ResetInactivityTime();
        activityManager->Start( 
            TTimeIntervalSeconds( KNoResponseFromUserDelay ),
            TCallBack( DispatchUserInactive, this ), 0 );            
        }

    TInt resource( 0 );        

    if ( aIconBitmap )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL iGetYesNoIconDialog" )

        if ( ESatYesNo == aCharacterSet )
            {
            resource = R_SATUI_CONFIRM_YES_NO_QUERY_WITH_ICON;
            }
        else
            {
            resource = R_SATUI_IMMEDIATE_DIGIT_NOTE_WITH_ICON;
            }
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL iGetYesNoDialog" )

        if ( ESatYesNo == aCharacterSet )
            {
            resource = R_SATUI_CONFIRM_YES_NO_QUERY;
            }
        else
            {
            resource = R_SATUI_IMMEDIATE_DIGIT_QUERY;
            }
        }
    
    TInt userResponse = RunQueryDialogL( resource , aText, aIconBitmap,
        aSelfExplanatory, EAknSoftkeyYes );

    iImmediateDigitResponse = EFalse;
    
    // set received immediate digit response
    aInkey = iKey;
    iKey = 0;
    
    if ( aDuration )
        {
        TTime endTime;
        endTime.HomeTime();
        TTimeIntervalMicroSeconds timeDifference( 0 );
        timeDifference = endTime.MicroSecondsFrom( startTime );
        aDuration = timeDifference.Int64() / KMicroSecondsInTenthOfSecond;
        
        // After duration call back timer is destroyed
        DestroyCallBackTimer();
        }
    
    CleanupStack::PopAndDestroy( activityManager );

    // Timeout
    if ( iNoResponseFromUserAfterDelay )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL no response from user" )
        response = ESatNoResponseFromUser;
        }
    // Cancel is pressed if immediate digit response without Yes/No
    else if ( !userResponse &&  ESatYesNo != aCharacterSet && !aInkey )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL backward request by user" )
        response = ESatBackwardModeRequestedByUser;
        }
    // Yes button is used
    else if ( userResponse && !aInkey)
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL button used" )
        aInkey = 1;
        }
    // No button is used
    else if ( !aInkey )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetYesNoL no button used" )
        aInkey = 0;
        }

    iNoResponseFromUserAfterDelay = EFalse;

    ShowWaitNoteL();
    
    TFLOGSTRING2( "CSatUiViewAppUi::GetYesNoL exit, return: %d", response )
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::GetInputL
// Displays data query.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::GetInputL(
    const TDesC& aText,
    const TSatCharacterSet aCharacterSet,
    TDes& aInput,
    const TInt aMinLength,
    const TInt aMaxLength,
    const TBool aHideInput,
    const TBool aGetInkey,
    const CFbsBitmap* aIconBitmapGetInput,
    const TBool aSelfExplanatory,
    TUint& aDuration ) 
    {
    TFLOGSTRING( "CSatUiViewAppUi::GetInputL called" )
    StopShowWaitNote();

    TSatUiResponse response( ESatSuccess );

    TPtrC textPtr( aText );

    TInt minLength( 0 );
    TInt maxLength( 1 );
    
    TFLOGSTRING2( "CSatUiViewAppUi::GetInputL aMinLength %d", aMinLength )
    TFLOGSTRING2( "CSatUiViewAppUi::GetInputL aMaxLength %d", aMaxLength )
    //lint -e{961} else block not needed, minLength initialized.
    if ( aMinLength <= KGetInputStringMaxSize && aMinLength > 0 )
        {
        minLength = aMinLength;
        }
    else if ( aMinLength > KGetInputStringMaxSize )
        {
        minLength = KGetInputStringMaxSize;
        }

    //lint -e{961} else block not needed, maxLength initialized.
    if ( aMaxLength < minLength )
        {
        maxLength = minLength;
        }
    else if ( aMaxLength <= KGetInputStringMaxSize && aMaxLength > 1 )
        {
        maxLength = aMaxLength;
        }
    else if ( aMaxLength > KGetInputStringMaxSize )
        {
        maxLength = KGetInputStringMaxSize;
        }

    HBufC* input = HBufC::NewLC( KGetInputStringMaxSize );
    TPtr inputPtr(input->Des());

    HBufC* textHolder = NULL;
    
    TFLOGSTRING2( "CSatUiViewAppUi::GetInputL aInput.Length %d",\
        aInput.Length() )

    if ( ( aInput.Length() >= minLength )
        && ( aInput.Length() <= maxLength ) )
        {
        inputPtr = aInput;
        }

    TInt userResponse;

    CUserActivityManager* activityManager = 
        CUserActivityManager::NewL( EPriorityNormal );

    CleanupStack::PushL( activityManager );

    // Start time for timer
    TTime startTime;

    // If duration is given use that value for timeout.
    // Start timer which doesn't care about user activity.
    if ( aDuration && aGetInkey )
        {
        TFLOGSTRING2( "CSatUiViewAppUi::GetInputL duration in 1/10 sec: %d",
            aDuration )        
        
        TInt duration ( 0 );
         
        TTimeIntervalMicroSeconds max32Bit(
            static_cast<TInt64>( K32TIntMaxValue ) );        
        TTimeIntervalMicroSeconds durationInTenthOfSeconds( 
            static_cast<TInt64>( aDuration ) );
        TTimeIntervalMicroSeconds durationInus( 
            durationInTenthOfSeconds.Int64() * KMicroSecondsInTenthOfSecond );
        
        if ( durationInus > max32Bit )
            {
            TFLOGSTRING( "CSatUiViewAppUi::GetInputL durationInus bigger \
                than max32Bit" )
            // since timers can only handle 32-bit values,
            // let's break the aDuration to smaller bits.
            // we break it here to 8 bits to fit in 32-bit value
            duration = durationInus.Int64() / KDivideByEight;
            iTimerRepeatOn = ETrue;
            }
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::GetInputL durationInus less \
                than max32Bit" )
            duration = durationInus.Int64();
            }
            
        CreateCallBackTimerL( duration, DelayCallback );
        startTime.HomeTime();
        }
    // Otherwise start timer which takes account user activity.
    else
        {
        User::ResetInactivityTime();
        activityManager->Start( 
            TTimeIntervalSeconds( KNoResponseFromUserDelay ),
            TCallBack( DispatchUserInactive, this ), 0 );            
        }

    CEikImage* image = new(ELeave) CEikImage;
    CleanupStack::PushL( image );
    
    if ( aIconBitmapGetInput )
        {        
        image->SetBitmap( aIconBitmapGetInput );
    
        // Create mask for icon bitmap
        CFbsBitmap* mask = new(ELeave) CFbsBitmap;

        TSize size = aIconBitmapGetInput->SizeInPixels();
        CleanupStack::PushL( mask );
        MakeGetInputBitmapMask( mask, size );

        image->SetMask( mask );
        CleanupStack::Pop( mask ); 
        }

    TInt resource( 0 );
    TInt textResource( 0 );
    // Query dialog
    iGetInputDialog = new (ELeave) CSatUiTextQueryDialog( inputPtr,
                CAknQueryDialog::ENoTone, minLength, maxLength );
    iForegroundControl = iGetInputDialog;

    if ( aCharacterSet == ESatDigitOnly ) 
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetInputL ESatDigitOnly" )
        if ( aText.Length() == 0 )
            {
            if ( aGetInkey )
                {
                textResource = R_TEXT_SIMATK_ENTER_NBR;
                }
            else
                {
                textResource = R_TEXT_SIMATK_ENTER_NBR_STRING;
                }
            }
        
        // Hidden digit as input
        if ( aHideInput )
            {            
            TFLOGSTRING( "CSatUiViewAppUi::GetInputL Hidden digit" )            
            if ( aIconBitmapGetInput )
                {
                resource = R_SATUI_HIDDEN_NUMBER_QUERY_WITH_LABEL_AND_ICON;
                }
            else
                {
                resource = R_SATUI_HIDDEN_NUMBER_QUERY_WITHOUT_LABEL;
                }
            }
        // Normal number query
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::GetInputL normal digit" ) 
            if ( aIconBitmapGetInput )
                {
                resource = R_SATUI_NUMBER_QUERY_WITH_LABEL_AND_ICON;
                }
            else
                {
                resource = R_SATUI_NUMBER_QUERY_WITHOUT_LABEL;
                }
            }
        }

    else // ESatCharSmsDefaultAlphabet or ESatCharUcs2Alphabet
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetInputL ESatCharSmsDefaultAlphabet \
            or ESatCharUcs2Alphabet" )
        if ( aText.Length() == 0 )
            {
            if ( aGetInkey )
                {
                textResource = R_TEXT_SIMATK_ENTER_CHAR;
                }
            else
                {
                textResource = R_TEXT_SIMATK_ENTER_CHAR_STRING;
                }
            }
        
        if ( !aGetInkey )
            {
            iGetInputDialog->SetPredictiveTextInputPermitted( ETrue );
            }

        iGetInputDialog->SetMaxLength( maxLength );

        if ( aIconBitmapGetInput )
            {
            resource = R_SATUI_TEXT_QUERY_WITH_LABEL_AND_ICON;
            }
        else
            {
            resource = R_SATUI_TEXT_QUERY_WITHOUT_LABEL;
            }
        }

    // Read default text if sim doesn't provide text to dialog 
    if ( aText.Length() == 0 )
        {
        // Reads a resource string with memory allocation
        // and pushes the string onto the cleanup stack.
        textHolder = StringLoader::LoadLC( textResource, iCoeEnv);
        textPtr.Set( *textHolder );       
        }

    // Icon is used and command is get input
    if ( aIconBitmapGetInput )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetInputL icon is used" )
        iGetInputDialog->PrepareLC( resource );
        if ( !aSelfExplanatory )
            {
            // Icon is not self-explanatory, show also text
            iGetInputDialog->SetPromptL( textPtr );            
            }
        // Add icon to query
        iGetInputDialog->QueryHeading()->SetHeaderImageL( image );
        // Set MSK same as LSK without any text causing the icon drawing
        iGetInputDialog->ButtonGroupContainer().SetCommandL(
            CEikButtonGroupContainer::EMiddleSoftkeyPosition, EAknSoftkeyOk,
            KSatEmptyDes );
        userResponse = iGetInputDialog->RunLD();
        TFLOGSTRING2( "CSatUiViewAppUi::GetInputL userResponse is: %d",
            userResponse )                
        }
    // No icon in use
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetInputL icon isn't used" )
        iGetInputDialog->PrepareLC( resource );
        iGetInputDialog->SetPromptL( textPtr );  
        // Set MSK same as LSK without any text causing the icon drawing
        iGetInputDialog->ButtonGroupContainer().SetCommandL(
            CEikButtonGroupContainer::EMiddleSoftkeyPosition, EAknSoftkeyOk,
            KSatEmptyDes );
        userResponse = iGetInputDialog->RunLD();
        TFLOGSTRING2( "CSatUiViewAppUi::GetInputL userResponse is: %d",
            userResponse )                
        }
    iForegroundControl = NULL;

    if ( textHolder )
        {
        CleanupStack::PopAndDestroy( textHolder ); 
        }

    if ( aDuration && aGetInkey )
        {
        TTime endTime;
        endTime.HomeTime();
        TTimeIntervalMicroSeconds timeDifference( 0 );
        timeDifference = endTime.MicroSecondsFrom( startTime );
        aDuration = timeDifference.Int64() / KMicroSecondsInTenthOfSecond;
        
        // After duration call back timer is destroyed
        DestroyCallBackTimer();
        }

    CleanupStack::PopAndDestroy( image );
    CleanupStack::PopAndDestroy( activityManager ); // image, activityManager
    iGetInputDialog = NULL;

    if ( iNoResponseFromUserAfterDelay )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetInputL no response from user" )
        response = ESatNoResponseFromUser;
        }
    else if ( userResponse )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetInputL successful response" )
        if ( inputPtr.Length() )
            {
            aInput.Copy( inputPtr );
            }
        response = ESatSuccess;
        }
    else
        {
        if ( IsAppShutterRunning() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::GetInputL satsession \
                terminated by user " )
            response = ESatSessionTerminatedByUser;
            iSessionTerminated = ETrue;
            }
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::GetInputL backward \
                requested by user " )
            response = ESatBackwardModeRequestedByUser;
            }
        }

    iNoResponseFromUserAfterDelay = EFalse;
    
    CleanupStack::PopAndDestroy( input ); // input

    ShowWaitNoteL();

    TFLOGSTRING2( "CSatUiViewAppUi::GetInputL exit, return: %d", response )
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::SetUpMenuL
// Sets the parameters for SetUpMenu view and calls the method
// from the CSatUiView class.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::SetUpMenuL(
    const TDesC& aText,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const CFbsBitmap* aIconBitmap,
    const CAknIconArray* aItemsIconArray,
    const TBool aSelfExplanatoryItems,
    const TBool aHelpIsAvailable )
    {
    TFLOGSTRING( "CSatUiViewAppUi::SetUpMenuL called" )
    StopShowWaitNote();
    TSatUiResponse response( ESatSuccess );

    delete iTitle;
    iTitle = NULL;

    delete iAppName;
    iAppName = NULL;

    TFLOGSTRING2( "CSatUiViewAppUi::SetUpMenuL length of aText %d", \
        aText.Length() )
    if ( aText.Length() > 0 )
        {
        iTitle = aText.AllocL();
        iAppName = aText.AllocL();
        } 
    else
        {
        //Reads a resource string with memory allocation
        iTitle = StringLoader::LoadL( R_QTN_SAT_TITLE, iCoeEnv );
        }

    if ( iSetUpMenuFirst )
        {
        iSetUpMenuFirst = EFalse;
        iSetUpMenuView = CSatUiView::NewL( ETrue, this, EFalse );

        // transfer ownership to CAknViewAppUi:
        AddViewL( iSetUpMenuView );

        iSetUpMenuView->SetAdapter( iAdapter );
        }

    // Make list box visible.
    iSetUpMenuView->MakeListBoxVisible( ETrue );
        
    //set contextpane icon and item icons
    iSetUpMenuView->SetIconL( aIconBitmap );
    iSetUpMenuView->SetItemsIconL( aItemsIconArray, aSelfExplanatoryItems );    
    iSetUpMenuView->SetTitle( iTitle );

    response = iSetUpMenuView->SetUpMenuL( iTitle, aMenuItems,
        aMenuItemNextActions, aHelpIsAvailable );

    iActiveView = KSetUpMenuViewId;
    
    // Reset counter for detecting possible loop in HandleForegroundEventL.
    iViewLoopCounter = 0;

    TFLOGSTRING2( "CSatUiViewAppUi::SetUpMenuL exit, return: %d", response )
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::HandleForegroundEventL
// Called when satui is brought to foreground or put into
// background
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::HandleForegroundEventL( TBool aForeground )
    {
    TFLOGSTRING2( "CSatUiViewAppUi::HandleForegroundEventL called \
        value %d", aForeground )
    
    iForeground = aForeground;
    
    // Check whether it is only need to display a dialog. 
    if ( iForeground && ( iForegroundControl || iCcNote ) ) 
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleForegroundEventL \
            need to display a dialog" )
        if ( iForegroundControl && ForegroundAlive() )
            {
            iForegroundControl->SetFocus( ETrue, EDrawNow );
            }
        }
    // Check is it allowed to activate local view.
    // Removes possibility of view activation loop for example when
    // launching Browser.
    else if ( ViewActivationAllowed( aForeground, iActiveView ) )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleForegroundEventL \
            activate local view allowed" )
        TVwsViewId viewId;
        GetActiveViewId( viewId );
        TFLOGSTRING2( "CSatUiViewAppUi::HandleForegroundEventL called \
            appUid %x", viewId.iAppUid )
        TFLOGSTRING2( "CSatUiViewAppUi::HandleForegroundEventL called \
            viewUid %x", viewId.iViewUid )
            
        if( ( KUidSATUI != viewId.iAppUid ) || 
            ( ( KUidSATUI == viewId.iAppUid ) 
                && ( viewId.iViewUid != iActiveView ) ) )
            {       
            TFLOGSTRING( "CSatUiViewAppUi::HandleForegroundEventL \
                activate local view executing" )
            if ( KSetUpMenuViewId == iActiveView )
                {
                ActivateLocalViewL( KSetUpMenuViewId );
                }
            else
                {
                ActivateLocalViewL( KSelectItemViewId );
                }
            }
            
        }        
    // Forwards event to the base class.
    CAknViewAppUi::HandleForegroundEventL( aForeground );
    TFLOGSTRING( "CSatUiViewAppUi::HandleForegroundEventL exit" )
    }
    
// ---------------------------------------------------------
// CSatUiViewAppUi::ViewActivationAllowed
// Sets the parameters for SelectItem view and calls the method
// from the CSatUiView class.
// (other items were commented in a header).
// ---------------------------------------------------------
//    
TBool CSatUiViewAppUi::ViewActivationAllowed( const TBool aForeground, 
    const TUid  aActiveView )
    {

    TFLOGSTRING( "CSatUiViewAppUi::ViewActivationAllowed called" )
    TBool activationAllowed( EFalse );
    // Value for detecting view activation loop. 
    const TInt certainViewLoopCount( 6 );
    
    // Check for foreground event, background event, 
    // foreground event, background event, ... sequence.
    if ( aForeground ^ iPreviousForeground )
        {
        TFLOGSTRING( "CSatUiViewAppUi::ViewActivationAllowed \
            check foreground event" )
        iViewLoopCounter++;
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::ViewActivationAllowed \
            no foreground event" )
        iViewLoopCounter = 0;
        }
    
    // Store information is application in foreground or
    // in background for next evaluation round.  
    iPreviousForeground = aForeground;

    // Check that there is no loop.
    if ( aForeground && aActiveView.iUid ) 
        {
        if ( iViewLoopCounter < certainViewLoopCount )
            {
            TFLOGSTRING( "CSatUiViewAppUi::ViewActivationAllowed \
                there are loops" )
            activationAllowed = ETrue;    
            }
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::ViewActivationAllowed \
                no loop" )
            // Counter is reset in case of foreground event
            // because of background event does not draw anything.
            iViewLoopCounter = 0;
            }
        
        }

    if ( iDisplayTextDialog || iDisplayTextIconDialog )
        {
        activationAllowed = EFalse;
        }
        
    TFLOGSTRING2( "CSatUiViewAppUi::ViewActivationAllowed exit, return: %d",\
        activationAllowed )
    return activationAllowed;
    
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::SelectItemL
// Sets the parameters for SelectItem view and calls the method
// from the CSatUiView class.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::SelectItemL(
    const TDesC& aText,
    const MDesCArray& aMenuItems,
    const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
    const TInt aDefaultItem,
    TUint8& aSelection,
    const CFbsBitmap* aIconBitmap,
    const CAknIconArray* aItemsIconArray,
    const TBool aSelfExplanatoryItems,
    const TBool aHelpIsAvailable )
    {
    TFLOGSTRING( "CSatUiViewAppUi::SelectItemL called" )
    StopShowWaitNote();

    TSatUiResponse response( ESatSuccess );

    delete iTitle;
    iTitle = NULL;

    TFLOGSTRING2( "CSatUiViewAppUi::SelectItemL length of aText %d",
        aText.Length() )
    if ( aText.Length() > 0 )
        {
        iTitle = aText.AllocL();
        } 
    else if ( iAppName )
        {
        TFLOGSTRING( "CSatUiViewAppUi::SelectItemL there is appname" )
        iTitle = iAppName->AllocL();
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::SelectItemL read appname" )
        //Reads a resource string with memory allocation
        iTitle = StringLoader::LoadL( R_QTN_SAT_TITLE, iCoeEnv );
        } 

    if ( iSelectItemFirst )
        {
        iSelectItemFirst = EFalse;
        iSelectItemView = CSatUiView::NewL( EFalse, this, EFalse );

        // transfer ownership to CAknViewAppUi:
        AddViewL( iSelectItemView );
        }
                
    iSelectItemPending = ETrue;
    iActiveView = KSelectItemViewId;

    //set contextpane icon and item icons
    iSelectItemView->SetIconL( aIconBitmap );
    iSelectItemView->SetItemsIconL( aItemsIconArray, aSelfExplanatoryItems );
    response = iSelectItemView->SelectItemL( iTitle, aMenuItems,
        aMenuItemNextActions, aDefaultItem, aSelection, aHelpIsAvailable );

    iSelectItemPending = EFalse;

    if ( iEndKeyPressed )
        {
        response = ESatSessionTerminatedByUser;
        }

    if ( response != ESatSessionTerminatedByUser )
        {
        ShowWaitNoteL();
        }

    TFLOGSTRING2( "CSatUiViewAppUi::SelectItemL exit, return: %d", response )
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::MatoPrepareComplete
// Indicates success or failure.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::MatoPrepareComplete(TInt aError)
    {
    TFLOGSTRING( "CSatUiViewAppUi::MatoPrepareComplete called" )
    TFLOGSTRING2( "CSatUiViewAppUi::MatoPrepareComplete error %i", aError )
    if ( KErrNone == aError )
        {
        iPlayToneError = ESatSuccess;

        TTimeIntervalMicroSeconds zero( static_cast<TInt64>( 0 ) );

        iPlayer->SetPriority( KAudioPriorityLowLevel,
               STATIC_CAST( TMdaPriorityPreference, KAudioPrefConfirmation ) );

        iPlayer->SetRepeats( KMdaAudioToneRepeatForever, zero );

        iPlayer->Play();
        }
    else
        {
        iPlayToneError = ESatFailure;
        }
    TFLOGSTRING( "CSatUiViewAppUi::MatoPrepareComplete exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::MatoPlayComplete
// Indicates success or failure.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::MatoPlayComplete( TInt aError )
    {
    TFLOGSTRING( "CSatUiViewAppUi::MatoPlayComplete calling" )
    if ( KErrNone == aError && iPlayer )
        {
        TFLOGSTRING( "CSatUiViewAppUi::MatoPlayComplete play" )
        iPlayer->Play();
        }
    else if ( iWait && iWait->IsStarted() )
        {
        TFLOGSTRING( "CSatUiViewAppUi::MatoPlayComplete stop playing" )
        // Stops playing if error.
        iWait->AsyncStop();
        }
    TFLOGSTRING2( "CSatUiViewAppUi::MatoPlayComplete exit, error %i", aError )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::PlayStandardToneL
// Plays Standard Tone. If needed displays the text in a note
// while playing.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::PlayStandardToneL(
    const TDesC& aText,
    const TDesC8& aSequence,
    TTimeIntervalMicroSeconds aDuration,
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory )
    {
    TFLOGSTRING( "CSatUiViewAppUi::PlayStandardToneL called" )
    StopShowWaitNote();
    // Delete timer just in case e.g. if DisplayText was on the screen
    DestroyCallBackTimer();

    iPermanentNote = NULL;
    iTimerRepeatOn = EFalse;
    
    TFLOGSTRING2( "CSatUiViewAppUi::PlayStandardToneL length of aText %i", \
        aText.Length() )
    if (aText.Length() != 0)
        {
        iPermanentNote = new (ELeave) CAknStaticNoteDialog();
        iPermanentNote->PrepareLC( R_SATUI_PERMANENT_NOTE );
        
        if ( ( !aSelfExplanatory ) || ( !aIconBitmap ) )
            {
            // Set text
            iPermanentNote->SetTextL( aText );
            }
        
        if ( aIconBitmap )
            {
            //Set image 
            TFLOGSTRING( "CSatUiViewAppUi::PlayStandardToneL SetImage" )
            // Get scalable bitmap if available.
            CEikImage* image = new ( ELeave ) CEikImage();
            CleanupStack::PushL( image );
            image->SetBitmap( aIconBitmap ); 
            iPermanentNote->SetImageL( image );
            CleanupStack::Pop( image );
            }
        
        iForegroundControl = iPermanentNote;
        iPermanentNote->RunLD();
        iPermanentNote->DrawNow();
        iCoeEnv->WsSession().Flush();
        }

    iPlayer = CMdaAudioToneUtility::NewL(*this);

    TInt volume( iPlayer->MaxVolume() );
    iPlayer->SetVolume( volume );
    TFLOGSTRING2( "CSatUiViewAppUi::PlayStandardToneL SetVolume %d",
        volume )    

    iPlayer->PrepareToPlayDesSequence( aSequence );

    TTimeIntervalMicroSeconds max32Bit(
        static_cast<TInt64>( K32TIntMaxValue ) );
    
    if ( aDuration > max32Bit )
        {
        // since timers can only handle 32-bit values,
        // let's break the aDuration to smaller bits.
        // we break it here to 8 bits to fit in 32-bit value
        aDuration = aDuration.Int64() / KDivideByEight;
        iTimerRepeatOn = ETrue;
        }

    CreateCallBackTimerL( I64INT( aDuration.Int64() ), DelayCallback );

    if ( !iWait->IsStarted() )
        {
        TFLOGSTRING( "CSatUiViewAppUi::PlayStandardToneL starting iWait" )
        iWait->Start();
        TFLOGSTRING( "CSatUiViewAppUi::PlayStandardToneL strarting done" )
        }

    iPlayer->CancelPlay();

    DestroyCallBackTimer();

    delete iPlayer;
    iPlayer = NULL;
    
    if ( iPermanentNote )
        {
        iPermanentNote->ExitSleepingDialog();
        delete iPermanentNote;
        iPermanentNote = NULL;
        }
    iForegroundControl = NULL;
    
    if ( !iEndKeyPressed )
        {
        ShowWaitNoteL();
        }
    
    //get warning and game tones setting from active profile
    GetProfileParamsL();
    
    if( ( EFalse == iWarningAndPlayTones ) && 
                        ( ESatSuccess == iPlayToneError  ) )
        {
        iPlayToneError = ESatSuccessToneNotPlayed;
        }
        
    TFLOGSTRING2( "CSatUiViewAppUi::PlayStandardToneL exit,return: %d", \
        iPlayToneError )
    return iPlayToneError;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ConfirmSendL
// Displays a query to get confirmation for Send SMS,
// Send SS or Send USSD command.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::ConfirmSendL(
    const TDesC& /* aText */,
    TBool& aActionAccepted,
    TInt aType)
    {
    TFLOGSTRING2( "CSatUiViewAppUi::ConfirmSendL called, aType: %d", \
        aType )
    HBufC* textHolder = NULL;

    switch ( aType )
        {
        case ESatUiConfirmSendSms:
            textHolder = StringLoader::LoadLC( R_TEXT_SIMATK_SEND_SMS,
                iCoeEnv );
            break;
        case ESatUiConfirmSendSs:
        case ESatUiConfirmSendUssd: //the same string for SS and USSD
            textHolder = StringLoader::LoadLC( R_TEXT_SIMATK_SEND_SS,
                iCoeEnv );
            break;
        default:
            return ESatFailure;
        }
        
    TInt userResponse = RunQueryDialogL( R_SATUI_CONFIRM_SEND_CANCEL_QUERY,
        *textHolder, NULL, EFalse, ESatUiSoftkeySend );
    
    TFLOGSTRING2( "CSatUiViewAppUi::ConfirmSendL userResponse: %d", \
        userResponse )
    if ( userResponse )
        {
        aActionAccepted = ETrue;
        }
    else
        {
        aActionAccepted = EFalse;
        }

    if ( textHolder )
        {
        CleanupStack::PopAndDestroy( textHolder ); //textHolder
        }

    TFLOGSTRING( "CSatUiViewAppUi::ConfirmSendL return ESatSuccess" )
    return ESatSuccess;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ShowSmsWaitNoteL
// Displays a wait note to indicate SMS sending.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ShowSmsWaitNoteL( const TDesC& aText, 
    const CFbsBitmap* aIconBitmapSendSM,
    const TBool aSelfExplanatoryIcon )
    {

    TFLOGSTRING( "CSatUiViewAppUi::ShowSmsWaitNoteL called" )
    
    iWaitNoteInUse = ETrue;
    
    TFLOGSTRING2( "CSatUiViewAppUi::ShowSmsWaitNoteL iWaitNote: %d", \
        iWaitNote )
    if ( NULL == iWaitNote )
        {
        iWaitNote = new(ELeave) CAknWaitDialog(
            ( REINTERPRET_CAST( CEikDialog**, &iWaitNote ) ), ETrue );
        iWaitNote->PrepareLC( R_SATUI_SMS_WAIT_NOTE );

        if ( ( aText.Length() > 0 ) && ( !aSelfExplanatoryIcon ||
            !aIconBitmapSendSM ) )
            {
            //Set text
            TFLOGSTRING( "CSatUiViewAppUi::ShowSmsWaitNoteL SetText" )
            iWaitNote->SetTextL( aText );
            }
            
        if ( aIconBitmapSendSM )
            {
            //Set image
            TFLOGSTRING( "CSatUiViewAppUi::ShowSmsWaitNoteL SetImage" )
            CEikImage* image = new ( ELeave ) CEikImage();
            CleanupStack::PushL( image );
            image->SetBitmap( aIconBitmapSendSM );               
            iWaitNote->SetImageL( image );
            CleanupStack::Pop( image );
            }
            
        iForegroundControl = iWaitNote;
        iWaitNote->RunLD();
        // iForegroundControl won't be clear until the wait dialog
        // closed. It will avoid we lose the chance to set focus for
        // the wait dialog when the switch of foreground/background
        // happened.
        }
        
    TFLOGSTRING( "CSatUiViewAppUi::ShowSmsWaitNoteL exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ShowSsWaitNoteL
// Displays a wait note to indicate SS sending.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ShowSsWaitNoteL( const TDesC& aText, 
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatoryIcon )
    {
    TFLOGSTRING( "CSatUiViewAppUi::ShowSsWaitNoteL called" )
    
    iWaitNoteInUse = ETrue;
    
    TFLOGSTRING2( "CSatUiViewAppUi::ShowSsWaitNoteL iWaitNote: %d", \
        iWaitNote )
    if ( !iWaitNote )
        {
        iWaitNote = new ( ELeave ) CSatUiWaitDialog( 
            ( REINTERPRET_CAST ( CEikDialog**,&iWaitNote ) ), 
            *iAdapter, ETrue );
        iForegroundControl = iWaitNote;
        
        TFLOGSTRING2( "CSatUiViewAppUi::ShowSsWaitNoteL length of aText: %d",\
            aText.Length() )
        // if there is an alpha id
        if ( aText.Length() > 0 )
            {
            // if there is not an icon or it's not self explanatory
            if ( ( !aSelfExplanatoryIcon ) || ( !aIconBitmap ) )
                {
                iWaitNote->SetTextL( aText );
                }
            
            if ( aIconBitmap )
                {
                //Set image
                TFLOGSTRING( "CSatUiViewAppUi::ShowSsWaitNoteL SetImage" )
                CEikImage* image = new ( ELeave ) CEikImage();
                CleanupStack::PushL( image );
                image->SetBitmap( aIconBitmap );
                iWaitNote->SetImageL( image );
                CleanupStack::Pop( image );
                }
                
            iWaitNote->ExecuteLD( R_SATUI_SS_WAIT_NOTE );
            }
        else
            {
            iWaitNote->ExecuteLD( R_SATUI_REQUESTING_WAIT_NOTE );    
            }
        // iForegroundControl won't be clear until the wait dialog
        // closed. It will avoid we lose the chance to set focus for
        // the wait dialog when the switch of foreground/background
        // happened.
        }
    TFLOGSTRING( "CSatUiViewAppUi::ShowSsWaitNoteL exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ShowDtmfWaitNoteL
// Shows a wait note while DTMF are being sent.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::ShowDtmfWaitNoteL( const TDesC& aText, 
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatoryIcon )
    {
    TFLOGSTRING( "CSatUiViewAppUi::ShowDtmfWaitNoteL called" )
    TSatUiResponse response(ESatFailure);

    iWaitNoteInUse = ETrue;
    
    TFLOGSTRING2( "CSatUiViewAppUi::ShowDtmfWaitNoteL iWaitNote: %d", \
        iWaitNote )
    if ( !iWaitNote )
        {
        iWaitNote = new ( ELeave ) CSatUiWaitDialog(
            (REINTERPRET_CAST( CEikDialog**,&iWaitNote ) ), *iAdapter, ETrue );
        
        if ( ( aText.Length() > 0 ) &&
            ( ( !aSelfExplanatoryIcon ) || ( !aIconBitmap ) ) )
            {
            iWaitNote->SetTextL( aText );
            }
            
        if ( aIconBitmap )
            {
            //Set image
            TFLOGSTRING( "CSatUiViewAppUi::ShowDtmfWaitNoteL SetImage" )
            CEikImage* image = new ( ELeave ) CEikImage();
            CleanupStack::PushL( image );
            image->SetBitmap( aIconBitmap ); 
            iWaitNote->SetImageL( image );
            CleanupStack::Pop( image );
            }
        
        iForegroundControl = iWaitNote;
        iWaitNote->ExecuteLD( R_SATUI_DTMF_WAIT_NOTE );
        // iForegroundControl won't be clear until the wait dialog
        // closed. It will avoid we lose the chance to set focus for
        // the wait dialog when the switch of foreground/background
        // happened.
        response = ESatSuccess;
        }
    TFLOGSTRING2( "CSatUiViewAppUi::ShowDtmfWaitNoteL exit, return: %d",\
        response )
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ConfirmRefreshL
// Displays a query to get confirmation Refresh command.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::ConfirmRefreshL(TBool& aActionAccepted)
    {
    TFLOGSTRING( "CSatUiViewAppUi::ConfirmRefreshL called" )
    StopShowWaitNote();

    TSatUiResponse response( ESatSuccess );

    HBufC* textHolder = NULL;

    textHolder = StringLoader::LoadLC( R_QTN_SAT_QUERY_ALLOW_RESET,
            iCoeEnv );
            
    TInt userResponse = RunQueryDialogL( R_SATUI_CONFIRM_OK_CANCEL_QUERY,
        *textHolder, NULL, EFalse, EAknSoftkeyOk );
    
    TFLOGSTRING2( "CSatUiViewAppUi::ConfirmRefreshL userResponse: %d", \
        userResponse )
    if ( userResponse )
        {
        aActionAccepted = ETrue;
        }
    else
        {
        aActionAccepted = EFalse;
        }

    if ( textHolder )
        {
        CleanupStack::PopAndDestroy( textHolder ); //textHolder
        }

    ShowWaitNoteL();

    TFLOGSTRING2( "CSatUiViewAppUi::ConfirmRefreshL exit, return: %d",\
        response )
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::CallControlL
// Displays info about call control to user
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::CallControlL(
    const TDesC& aText,
    const TSatAlphaIdStatus aAlphaIdStatus )
    {
    TFLOGSTRING( "CSatUiViewAppUi::CallControlL called" )
    
    if ( !iCcNote ) 
        {
        iCcNote = CAknGlobalNote::NewL();
        }
    iCcNote->SetTone( CAknNoteDialog::ENoTone );
    
    if ( ESatAlphaIdNotNull == aAlphaIdStatus )
        {
        TFLOGSTRING( "CSatUiViewAppUi::CallControlL AlphaIdNotNull" )
        
        // The note will be disabled by the phone UI when the call is 
        // in connecting status. But call controll case is an exception, So we
        // use RAknKeyLock to enable soft notifications.
        RAknKeyLock keyLock;
        TInt errorCode = keyLock.Connect();
        TFLOGSTRING2( "CSatUiViewAppUi::CallControlL connect error %d", \
            errorCode )
        if ( KErrNone == errorCode )
            {
            keyLock.EnableSoftNotifications( ETrue );
            }
        keyLock.Close();
        
        iCcNote->SetSoftkeys( R_AVKON_SOFTKEYS_OK_EMPTY );
        iCcNoteId = iCcNote->ShowNoteL( 
                                       EAknGlobalInformationNote, aText );
        CreateCallBackTimerL( K3Seconds, CloseCallControlNoteL );
        if ( iWait && !iWait->IsStarted() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::CallControlL iWait starting" )
            iWait->Start();
            }
        DestroyCallBackTimer();
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::CallControlL AlphaId Null" )
        iCcNote->ShowNoteL( EAknGlobalInformationNote, aText );
        }

    if ( iCcNote )
        {
        delete iCcNote;
        iCcNote = NULL;
        }
    TFLOGSTRING( "CSatUiViewAppUi::CallControlL exit" )
    
    return ESatSuccess;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::MoSmControlL
// Displays info about Mo SM control to user
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::MoSmControlL(
    const TDesC& aText,
    const TSatAlphaIdStatus aAlphaIdStatus )
    {
    TFLOGSTRING( "CSatUiViewAppUi::MoSmControlL called" )
    if ( ESatAlphaIdNotNull == aAlphaIdStatus )
        {
        TFLOGSTRING( "CSatUiViewAppUi::MoSmControlL AlphaIdNotNull" )
        CAknNoteDialog* dialog = new (ELeave) CAknNoteDialog(
            REINTERPRET_CAST( CEikDialog**,&dialog ),
            CAknNoteDialog::ENoTone,
            CAknNoteDialog::ELongTimeout );

        dialog->PrepareLC( R_SATUI_MOSM_CONTROL_NOTE );

        dialog->SetTextL( aText );

        iForegroundControl = dialog;
        dialog->RunLD();
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::MoSmControlL AlphaId Null" )
        CAknNoteDialog* dialog = new (ELeave) CAknNoteDialog(
            REINTERPRET_CAST( CEikDialog**,&dialog ),
            CAknNoteDialog::ENoTone,
            CAknNoteDialog::ELongTimeout );

        dialog->PrepareLC( R_SATUI_INFORMATION_NOTE );

        dialog->SetTextL( aText );

        iForegroundControl = dialog;
        dialog->RunLD();
        }
    iForegroundControl = NULL;
    TFLOGSTRING( "CSatUiViewAppUi::MoSmControlL exit" )
    return ESatSuccess;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ConfirmLaunchBrowserL
// Displays a query to get confirmation for Launch Browser
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ConfirmLaunchBrowserL(
    const TDesC& aText,
    TBool& aActionAccepted,
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory )
    {
    TFLOGSTRING( "CSatUiViewAppUi::ConfirmLaunchBrowserL called" )
    
    TInt resource( 0 );
    
    if ( aIconBitmap )
        {
        TFLOGSTRING( "CSatUiViewAppUi::ConfirmLaunchBrowserL icon" )
        resource = R_SATUI_CONFIRM_OK_CANCEL_QUERY_WITH_ICON;
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::ConfirmLaunchBrowserL no icon" )
        resource = R_SATUI_CONFIRM_OK_CANCEL_QUERY;
        }
        
    TInt userResponse = RunQueryDialogL( resource, aText, aIconBitmap,
        aSelfExplanatory, EAknSoftkeyOk ); 
    
    TFLOGSTRING2( "CSatUiViewAppUi::ConfirmLaunchBrowserL userResponse %d",\
        userResponse )
    if ( userResponse )
        {
        aActionAccepted = ETrue;
        }
    else
        {
        aActionAccepted = EFalse;
        }
        
    TFLOGSTRING( "CSatUiViewAppUi::ConfirmLaunchBrowserL exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::HandleCommandL
// Called by CSatUiView::HandleCommandL.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::HandleCommandL(
    TInt aCommand )
    {
    TFLOGSTRING2( "CSatUiViewAppUi::HandleCommandL called, aCommand: %d",\
        aCommand )
    switch ( aCommand )
        {
        case EEikCmdExit:
            {
            HandleExitCommandL();
            break;
            }
        case EAknCmdHelp:
            {
            //Show Series 60 Help
            HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(), 
                                                AppHelpContextL() );
            break;
            }
        default: 
            break;
        }
    TFLOGSTRING( "CSatUiViewAppUi::HandleCommandL exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::HandleExitCommandL
// Handling of the exit softkey.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::HandleExitCommandL()
    {
    TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL called" )
  
    if ( iBipWaitNote )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL iBipWaitNote" )
        SetCommandPending( EFalse );
        }

    if ( iWait )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL iWait " )
        if ( iWait->IsStarted() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL \
                iWait->AsyncStop()" )
            iWait->AsyncStop();
            }
        }

    CloseDialogs();

    if ( iEndKeyPressed )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL endkeypressed" )
        // Inform SAT Server that end key has been pressed
        iAdapter->SessionTerminated( EEndKeyUsed );        
        }

    if ( !iCommandPending )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL !iCommandPending" )

        if ( iPlayer && EMdaAudioToneUtilityPlaying == iPlayer->State() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL cancel playtone")
            iPlayer->CancelPlay();
            }
            
        if ( iAudioPlayer )
            {
            TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL cancel \
                userselected tone" )
            iAudioPlayer->Stop();
            delete iAudioPlayer;
            iAudioPlayer = NULL;
            TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL\
                iAudioPlayer deleted" )
            }
        TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL exiting AppUI ")
        Exit();
        }
    if ( iWaitNoteInUse )
        {
        iCommandPending = EFalse;
        }
    if ( iSelectItemPending && !iEndKeyPressed )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL SelectItem pending" )
        iSessionTerminated = ETrue;
        iSelectItemView->HandleCommandL( ESatUiCmdExit );
        }

    TFLOGSTRING( "CSatUiViewAppUi::HandleExitCommandL exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::HandleWsEventL
// Handling of the window server events.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::HandleWsEventL( 
        const TWsEvent& aEvent,
        CCoeControl* aDestination )
    {
    switch ( aEvent.Type() )
        {
        // This is End key closing event. If this window server event is 
        // received, by default application will be closed unless it is 
        // the system application.
        case KAknUidValueEndKeyCloseEvent:
            {            
            // Performs the same behaviour as when Exit softkey is pressed.
            TFLOGSTRING( "CSatUiViewAppUi::HandleWsEventL End key pressed" )
            iEndKeyPressed = ETrue;
            HandleExitCommandL();
            break;
            }
        case EEventPointer:
            {
            TFLOGSTRING( "CSatUiViewAppUi::HandleWsEventL EEventPointer" )
            const TPointerEvent& pointer = *( aEvent.Pointer() );
            const TPointerEvent::TType pointerType = pointer.iType;
            
            if ( ( TPointerEvent::EButton1Down == pointerType ) ||
                 ( TPointerEvent::EButton2Down == pointerType ) ||
                 ( TPointerEvent::EButton3Down == pointerType ) )
                {
                TFLOGSTRING( "CSatUiViewAppUi::HandleWsEventL EEventPointer\
                    handle the event" )
                // Check if we need to end play tone    
                StopPlayToneL();
                }
            break;
            }
        case EEventKeyDown:
            {
            // Handles other user key presses
            TFLOGSTRING( "CSatUiViewAppUi::HandleWsEventL EEventKeyDown" )
            // Check if we need to end play tone
            StopPlayToneL();
            if ( iImmediateDigitResponse )
                {
                HandleImmediateDigitResponse( aEvent.Key()->iScanCode );
                }
            break;
            }

        default:
            break;
        }
    // Forwards event to the base class.
    CAknViewAppUi::HandleWsEventL( aEvent, aDestination );
    PriorityVerifyAndClose( aEvent );
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::HelpStatus
// Help support status
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::HelpStatus( TBool aStatus )
    {
    TFLOGSTRING( "CSatUiViewAppUi::HelpStatus calling" )
    iHelp = aStatus;
    TFLOGSTRING( "CSatUiViewAppUi::HelpStatus exiting" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::DelayCallback
// Delay Callback function. Completes after predefined 
// time has passed
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSatUiViewAppUi::DelayCallback( TAny* aPtr )
    {
    TFLOGSTRING( "CSatUiViewAppUi::DelayCallback calling" )
    if ( ((CSatUiViewAppUi* )aPtr)->iTimerRepeatOn )
        {
        if ( ((CSatUiViewAppUi* )aPtr)->iTimerRepeatCount++ == KDivideByEight )
            {
            TFLOGSTRING( "CSatUiViewAppUi::DelayCallback  -- 1" )
            if ( ((CSatUiViewAppUi* )aPtr)->iWait->IsStarted() )
                {
                TFLOGSTRING( "CSatUiViewAppUi::DelayCallback  -- 2" )
                ((CSatUiViewAppUi* )aPtr)->iWait->AsyncStop();
                }
            if ( ( ( CSatUiViewAppUi* ) aPtr )->iGetInputDialog ||
                ( ( CSatUiViewAppUi* ) aPtr )->iQueryDialog )
                {
                TFLOGSTRING( "CSatUiViewAppUi::DelayCallback  -- 3" )
                ( ( CSatUiViewAppUi* ) aPtr )->
                    iNoResponseFromUserAfterDelay = ETrue;
                ( ( CSatUiViewAppUi* ) aPtr )->CloseDialogs();
                }
            }
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::DelayCallback  -- 4" )
            return ( ETrue );
            }
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::DelayCallback  -- 5" )
        if ( ((CSatUiViewAppUi* )aPtr)->iWait->IsStarted() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::DelayCallback  -- 6" )
            ((CSatUiViewAppUi* )aPtr)->iWait->AsyncStop();
            }
        }

    // Close DisplayText dialog if clearing after delay is requested
    if ( ((CSatUiViewAppUi* )aPtr)->iDisplayTextDialog ||
        ((CSatUiViewAppUi* )aPtr)->iDisplayTextIconDialog )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DelayCallback  -- 7" )
        ( ( CSatUiViewAppUi* ) aPtr )->iNoResponseFromUserAfterDelay = ETrue;
        ( ( CSatUiViewAppUi* ) aPtr )->ClearScreen();
        }    
    else if ( ( ( CSatUiViewAppUi* ) aPtr )->iGetInputDialog ||
        ( ( CSatUiViewAppUi* ) aPtr )->iQueryDialog )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DelayCallback  -- 8" )
        ( ( CSatUiViewAppUi* ) aPtr )->iNoResponseFromUserAfterDelay = ETrue;
        ( ( CSatUiViewAppUi* ) aPtr )->CloseDialogs();
        }

    TFLOGSTRING( "CSatUiViewAppUi::DelayCallback exiting" )
    return ( EFalse );
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::CreateCallBackTimerLC
// Creates a callback timer
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::CreateCallBackTimerL( const TInt aDelay, 
    TInt (*aFunction)(TAny* aPtr) )
    {
    TFLOGSTRING( "CSatUiViewAppUi::CreateCallBackTimerLC called" )
    iTimerRepeatCount = 0;
    iTimer = CPeriodic::NewL( CActive::EPriorityStandard );

    if ( iTimerRepeatOn )
        {
        TFLOGSTRING( "CSatUiViewAppUi::CreateCallBackTimerLC repeating" )
        iTimer->Start( aDelay, aDelay,
            TCallBack( aFunction, this  ) );
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::CreateCallBackTimerLC \
            no repeating" )
        iTimer->Start( aDelay, KMicroSecondsInSecond,
            TCallBack( aFunction, this  ) );
        }
    
    TFLOGSTRING( "CSatUiViewAppUi::CreateCallBackTimerLC exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::DestroyCallBackTimer
// Destroys the callback timer
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::DestroyCallBackTimer( )
    {
    TFLOGSTRING( "CSatUiViewAppUi::DestroyCallBackTimer() called" )
    delete iTimer;
    iTimer = NULL;
    iTimerRepeatOn = EFalse;
    TFLOGSTRING( "CSatUiViewAppUi::DestroyCallBackTimer() exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ProcessCommandParametersL
// Takes care of external launching.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CSatUiViewAppUi::ProcessCommandParametersL(
    TApaCommand aCommand,
    TFileName& aDocumentName,
    const TDesC8& aTail )
    {
    TFLOGSTRING2( "CSatUiViewAppUi::ProcessCommandParametersL called,\
        aCommand: %d", aCommand )    
    // TApaCommand is 0 for SIM originated opening and 2 for user originated
    // opening. Empty view is only needed for SIM originated opening. 
    if ( EApaCommandOpen == aCommand )
        {
        // Creates empty view for commands without view.
        CreateEmptyMenuL();
        }
    
    TFLOGSTRING( "CSatUiViewAppUi::ProcessCommandParametersL exit" )
    return CAknViewAppUi::ProcessCommandParametersL( aCommand, aDocumentName, 
                                                     aTail );
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::DispatchUserInactive
// Called when user has been inactive for a pre-defined time period
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSatUiViewAppUi::DispatchUserInactive( TAny* aPtr )
    {
    TFLOGSTRING( "CSatUiViewAppUi::DispatchUserInactive calling" )
    ((CSatUiViewAppUi* )aPtr)->iNoResponseFromUserAfterDelay = ETrue;

    //lint -e{961} else block not needed.
    if ( ( ( CSatUiViewAppUi* ) aPtr )->iDisplayTextDialog ||
        ( ( CSatUiViewAppUi* ) aPtr )->iDisplayTextIconDialog )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DispatchUserInactive clear screen" )
        ( ( CSatUiViewAppUi* ) aPtr )->ClearScreen();
        }
    else if ( ( ( CSatUiViewAppUi* ) aPtr )->iQueryDialog ||
        ( (CSatUiViewAppUi* ) aPtr )->iGetInputDialog )
        {
        TFLOGSTRING( "CSatUiViewAppUi::DispatchUserInactive close dialog" )
        ( ( CSatUiViewAppUi* ) aPtr )->CloseDialogs();
        }

    TFLOGSTRING( "CSatUiViewAppUi::DispatchUserInactive exiting" )
    return (EFalse);
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::MakeGetInputBitmapMask
// Make a mask for get input icon
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::MakeGetInputBitmapMask( CFbsBitmap* aMask,
        TSize aSize ) const
    {
    TFLOGSTRING( "CSatUiViewAppUi::MakeGetInputBitmapMask calling" )
    TInt width = aSize.iWidth;
    TInt height = aSize.iHeight;
    aMask->Create( TSize( width, height ), EColor16M );

    TBitmapUtil bitmap1Util( aMask );
    // Begin manipulation with bitmap1Util, setting initial pixel to 0,0 
    bitmap1Util.Begin( TPoint( 0,0 ) ); 

    // Make mask which covers whole icon
    for ( TInt y = 0; y < height; y++ )
        {
        for ( TInt x = 0; x < width; x++ )
            {
            bitmap1Util.SetPos( TPoint( x, y ) );
            bitmap1Util.SetPixel( 0x0000ff );
            }
        }
    bitmap1Util.End();
    TFLOGSTRING( "CSatUiViewAppUi::MakeGetInputBitmapMask exiting" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::DispatchTimer
// Dispatch iWait to action implementer.
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CSatUiViewAppUi::DispatchTimer( CActiveSchedulerWait& aWait )
    {
    TFLOGSTRING( "CSatUiViewAppUi::DispatchTimer calling" )
    iWait = &aWait;
    TFLOGSTRING( "CSatUiViewAppUi::DispatchTimer exiting" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::GetEndKey
// Get a flag which is indicating if end key is pressed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CSatUiViewAppUi::GetEndKey()
    {
    TFLOGSTRING( "CSatUiViewAppUi::GetEndKey calling-exiting" )
    return iEndKeyPressed;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::SetEndKey
// Set a flag which is indicating if end key is pressed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::SetEndKey( TBool aValue )
    {
    TFLOGSTRING2( "CSatUiViewAppUi::SetEndKey calling, aValue:%d", aValue )
    iEndKeyPressed = aValue;
    TFLOGSTRING( "CSatUiViewAppUi::SetEndKey exiting" )    
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::SetCommandPending
// Set a flag which is indicating proactive command is pending.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::SetCommandPending( TBool aValue )
    {
    TFLOGSTRING2( "CSatUiViewAppUi::SetCommandPending calling, aValue:%d", \
        aValue )
    iCommandPending = aValue;
    if ( aValue )
        {
        // Cancels a request to save RAM when higher than normal buffer size is
        // no longer required.
        CAknEnv::Static()->CancelWsBufferRequest( iReqWSBufferID );
        }
    TFLOGSTRING( "CSatUiViewAppUi::SetCommandPending exiting" )  
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::CloseDialogs
// Close all existing dialogs.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::CloseDialogs()
    {
    TFLOGSTRING( "CSatUiViewAppUi::CloseDialogs() called" )
    if ( iWaitNote || iBipWaitNote || iNoteDialog ) 
        {
        TFLOGSTRING( "CSatUiViewAppUi::CloseDialogs() StopShowWaitNote" )
        StopShowWaitNote();
        }
    if ( iDisplayTextDialog )
        {
        TFLOGSTRING( "CSatUiViewAppUi::CloseDialogs() iDisplayTextDialog" )
        delete iDisplayTextDialog;
        iDisplayTextDialog = NULL;
        iImmediateRsp = EFalse;
        }
    if ( iDisplayTextIconDialog )
        {
        TFLOGSTRING( "CSatUiViewAppUi::CloseDialogs() iDisplayTextIconDialog" )
        delete iDisplayTextIconDialog;
        iDisplayTextIconDialog = NULL;
        iImmediateRsp = EFalse;
        }
    if ( iGetInputDialog )
        {
        TFLOGSTRING( "CSatUiViewAppUi::CloseDialogs() iGetInputDialog" )
        TRAP_IGNORE( iGetInputDialog->DismissQueryL() )
        iGetInputDialog = NULL;
        }
    if ( iQueryDialog )
        {
        TFLOGSTRING( "CSatUiViewAppUi::CloseDialogs() iQueryDialog" )
        delete iQueryDialog;
        iQueryDialog = NULL;
        }
    if ( iPermanentNote )
        {
        TFLOGSTRING( "CSatUiViewAppUi::CloseDialogs() iPermanentNote" )
        // CAknNoteDialog has already performed deletion when user action.
        iPermanentNote = NULL;
        }
    TFLOGSTRING( "CSatUiViewAppUi::CloseDialogs() exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::MapcInitComplete
// Plays the tone.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::MapcInitComplete( TInt aError, 
    const TTimeIntervalMicroSeconds& /*aDuration*/ )
    {
    TFLOGSTRING( "CSatUiViewAppUi::MapcInitComplete called" )
    // Audio player is initialised, so if there is no error, we can start 
    // playing the tone.
    if( KErrNone == aError )
        {
        const TInt volume( Max( 0, Min( iVolume, 
                           iAudioPlayer->MaxVolume() ) ) );
        TFLOGSTRING2( "CSatUiViewAppUi::\
            MapcInitComplete SetVolume %i", volume )
        // Set volume according Settings. 
        iAudioPlayer->SetVolume( volume );
        iAudioPlayer->Play();
        TFLOGSTRING( "CSatUiViewAppUi::MapcInitComplete Play" )
        }
    else
        {
        // Otherwise, we delete the initialised audio player.
        if ( iAudioPlayer )
            {
            TFLOGSTRING( "CSatUiViewAppUi::MapcInitComplete iAudioPlayer \
                          true" )
            delete iAudioPlayer;
            iAudioPlayer = NULL;
            }

        // Remove also the note for play tone
        if ( iPermanentNote )
            {
            TFLOGSTRING( "CSatUiViewAppUi::MapcInitComplete iPermanentNote \
                          true" )
            iPermanentNote->ExitSleepingDialog();
            delete iPermanentNote;
            iPermanentNote = NULL;
            }

        // If there was a duration for play tone, stop waiting
        if ( iWait->IsStarted() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::MapcInitComplete stop iWait" )
            iWait->AsyncStop();
            }

        TFLOGSTRING2( "CSatUiViewAppUi::MapcInitComplete Error %i", aError )
        }

    TFLOGSTRING( "CSatUiViewAppUi::MapcInitComplete exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::MapcPlayComplete
// Deletes audio player after playing is completed.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::MapcPlayComplete( TInt aError )
    {
    TFLOGSTRING( "CSatUiViewAppUi::MapcPlayComplete called" )
    
    // When playing is completed, deleting the audio player.
    if ( iAudioPlayer )
        {
        TFLOGSTRING( "CSatUiViewAppUi::MapcPlayComplete delete iAudioPlayer" )
        delete iAudioPlayer;
        iAudioPlayer = NULL;
        }
    // Remove note after play has completed.
    if ( iPermanentNote )
        {
        TFLOGSTRING( "CSatUiViewAppUi::MapcPlayComplete ExitSleepingDialog" )
        iPermanentNote->ExitSleepingDialog();
        delete iPermanentNote;
        iPermanentNote = NULL;
        }
    // Show wait note only for samples that have duration, otherwise there is
    // chance that wait note is not stopped.
    if ( TTimeIntervalMicroSeconds( static_cast<TInt64>( 0 ) ) != iDuration )
        {
        // It does not affect for tone playing if wait note
        // is not able to show.
        TFLOGSTRING( "CSatUiViewAppUi::MapcPlayComplete ShowWaitNoteL" )
        TRAP_IGNORE( ShowWaitNoteL() )
        }

    TFLOGSTRING2( "CSatUiViewAppUi::MapcPlayComplete exit, error:%i", aError )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::PlayUserSelectedToneL
// Plays user selected tone from active Profile. 
// If needed displays the text in a note while playing.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::PlayUserSelectedToneL(
    const TDesC& aText,
    TTimeIntervalMicroSeconds aDuration,
    TSatTone aTone,
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory )
    {
    TFLOGSTRING( "CSatUiViewAppUi::PlayUserSelectedToneL called" )
    StopShowWaitNote();
    // Delete timer just in case e.g. if DisplayText was on the screen
    DestroyCallBackTimer();

    iPermanentNote = NULL;
    iDuration = aDuration;
    
    TFLOGSTRING2( "CSatUiViewAppUi::PlayUserSelectedToneL length of aText \
        %d", aText.Length() )
    if ( 0 != aText.Length() )
        {
        iPermanentNote = new (ELeave) CAknStaticNoteDialog();
        iPermanentNote->PrepareLC( R_SATUI_PERMANENT_NOTE );
        
        if ( ( !aSelfExplanatory ) || ( !aIconBitmap ) )
            {
            // Set text
            iPermanentNote->SetTextL( aText );
            }
        
        if ( aIconBitmap )
            {
            //Set image 
            TFLOGSTRING( "CSatUiViewAppUi::PlayUserSelectedToneL SetImage" )
            CEikImage* image = new ( ELeave ) CEikImage();
            CleanupStack::PushL( image );
            image->SetBitmap( aIconBitmap ); 
            iPermanentNote->SetImageL( image );
            CleanupStack::Pop( image );
            }
        
        iPermanentNote->SetTimeout( CAknNoteDialog::ENoTimeout );
        iForegroundControl = iPermanentNote;
        iPermanentNote->RunLD();
        }

    // If several messages/calls arrive during a small amount of time and if the
    // message received or incoming call tone is already being played we do not 
    // restart it.
    if ( iAudioPlayer )
        {
        TFLOGSTRING( "CSatUiViewAppUi::\
            PlayUserSelectedToneL Error - already played" )
        return ESatFailure;
        }

    // This defines name for the current message alert or incoming call tone. 
    TBuf<KMaxSoundFileLength> soundFileName;
    GetProfileParamsL( aTone, &soundFileName );
    // This defines the behaviour to be adopted by an
    // audio client if a higher priority client takes over the device.
    TInt mdaPriority( KAudioPrefIncomingCall );
    // This is used to resolve conflicts when more than one client tries to 
    // access the same sound output device simultaneously.
    TInt audioPriority( KAudioPriorityPhoneCall );
    
    TFLOGSTRING2( "CSatUiViewAppUi::PlayUserSelectedToneL\
        Volume is %i ", iVolume )

    // Creates the audio player.
    iAudioPlayer = CMdaAudioPlayerUtility::NewFilePlayerL( 
        soundFileName,                                                
        *this,                                              
        audioPriority,                                                  
        ( TMdaPriorityPreference ) mdaPriority );

    if ( TTimeIntervalMicroSeconds( static_cast<TInt64>( 0 ) ) != aDuration )
        {
        TFLOGSTRING( "CSatUiViewAppUi::PlayUserSelectedToneL duration not 0" )
        TTimeIntervalMicroSeconds max32Bit( 
            static_cast<TInt64>( K32TIntMaxValue ) );
        if ( aDuration > max32Bit )
            {
            // Timers can only handle 32-bit values 
            // and 64-bit value is received.
            // Let's break the aDuration to smaller bits.
            // Divided here by 8 to fit in 32-bit maxvalue.
            // Loops 8 times in DelayCallback to play tone
            // right duration. 
            aDuration = aDuration.Int64() / KDivideByEight;
            iTimerRepeatOn = ETrue;
            }
        iAudioPlayer->SetRepeats( KSoundPlayerRepeatForever, 
                                  TTimeIntervalMicroSeconds( 
                                  static_cast<TInt64>( 0 ) ) );
        CreateCallBackTimerL( I64INT( aDuration.Int64() ), DelayCallback );
        
        if ( !iWait->IsStarted() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::PlayUserSelectedToneL\
                starting iWait" )
            iWait->Start();
            TFLOGSTRING( "CSatUiViewAppUi::PlayUserSelectedToneL\
                strarting done" )
            }
        
        // Sample is played in forever loop for the duration.
        // After duration call back timers are destroyed       
        DestroyCallBackTimer();
        // If player exists, stop playing and delete player. MapcPlayComplete
        // is not called because CallBackTimer stops the playing.
        if ( iAudioPlayer )
            {
            iAudioPlayer->Stop();
            delete iAudioPlayer;
            iAudioPlayer = NULL;
            TFLOGSTRING( "CSatUiViewAppUi::PlayUserSelectedToneL\
                iAudioPlayer deleted" )
            }
        // Remove note after play has completed.
        if ( iPermanentNote )
            {
            iPermanentNote->ExitSleepingDialog();
            delete iPermanentNote;
            iPermanentNote = NULL;
            }
        iForegroundControl = NULL;
        
        if ( !iEndKeyPressed )
            {
            ShowWaitNoteL();
            }
            
        }
    else
        {
        // If duration is zero then tone is played only once.
        // Playing duration is same as duration of the sample.
        iAudioPlayer->SetRepeats(
            0, TTimeIntervalMicroSeconds( static_cast<TInt64>( 0 ) ) );
        TFLOGSTRING( "CSatUiViewAppUi::PlayUserSelectedToneL SetRepeats 0" )
        }

    TFLOGSTRING( "CSatUiViewAppUi::PlayUserSelectedToneL exit" )
    if( EFalse == iWarningAndPlayTones )
        {
        return ESatSuccessToneNotPlayed;
        }
        
    return ESatSuccess;
    }


// ---------------------------------------------------------
// CSatUiViewAppUi::ConfirmOpenChannelL
// Displays a query to get confirmation for Open Channel command.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSatUiResponse CSatUiViewAppUi::ConfirmOpenChannelL(
    const TDesC& aText,
    TBool& aActionAccepted,
    const CFbsBitmap* aIconBitmapOpenChannel,
    const TBool aSelfExplanatory )
    {
    TFLOGSTRING( "CSatUiViewAppUi::ConfirmOpenChannelL called" )
    
    TSatUiResponse response( ESatSuccess );
    TInt resource( 0 );
    TPtrC textPtr( aText );
    HBufC* textHolder = NULL;
    
    if ( aIconBitmapOpenChannel )
        {
        TFLOGSTRING( "CSatUiViewAppUi::ConfirmOpenChannelL have icon" )
        resource = R_SATUI_CONFIRM_OK_CANCEL_QUERY_WITH_ICON;
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::ConfirmOpenChannelL no icon" )
        resource = R_SATUI_CONFIRM_OK_CANCEL_QUERY;
        
        TFLOGSTRING2( "CSatUiViewAppUi::ConfirmOpenChannelL length of \
            aText %d", aText.Length() )
        if ( 0 == aText.Length() )
            {
            textHolder = StringLoader::LoadLC( R_QTN_SAT_CONF_OPEN_CHANNEL_BIP,
                iCoeEnv );    
            textPtr.Set( *textHolder ); 
            }
        }
    
    TInt userResponse = RunQueryDialogL( resource, textPtr,
        aIconBitmapOpenChannel, aSelfExplanatory, EAknSoftkeyOk );
        
    if ( textHolder )
        {
        CleanupStack::PopAndDestroy( textHolder ); 
        }

    if ( userResponse )
        {
        TFLOGSTRING( "CSatUiViewAppUi::ConfirmOpenChannelL action accepted" )
        aActionAccepted = ETrue;
        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::ConfirmOpenChannelL action not \
            accepted" )
        aActionAccepted = EFalse;
        }

    

    ShowWaitNoteL();

    TFLOGSTRING2( "CSatUiViewAppUi::ConfirmOpenChannelL exit,return: %d",\
        response )
    return response;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ShowBIPNoteL
// Displays the BIP note.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ShowBIPNoteL( TInt aCommand, const TDesC& aText,
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory )
    {
    TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL() called" )
    TFLOGSTRING2( "CSatUiViewAppUi::ShowBIPNoteL() aCommand: %d", aCommand )
    
    iWaitNoteInUse = ETrue;
    TBool showNote( EFalse );
    TInt resourceId( 0 );
    switch ( aCommand )
        {
        case ESendDataIdentier: // fall through
        case EReceiveDataIdentifier:
            {
            resourceId = R_SATUI_BIP_WAIT_NOTE;
            showNote = ETrue;
            break;
            }
        case ECloseChannelIdentifier:
            {
            HBufC* textHolder = StringLoader::LoadLC(
                        R_QTN_SAT_CONF_CLOSE_CHANNEL_BIP, iCoeEnv );
                        
            if ( !iNoteDialog )
                {
                TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL no NoteDialog" )
                iNoteDialog = new (ELeave) CAknNoteDialog(
                    REINTERPRET_CAST( CEikDialog**, &iNoteDialog ),
                    CAknNoteDialog::ENoTone,
                    CAknNoteDialog::EShortTimeout );
                              
                if ( aIconBitmap )
                    {
                    iNoteDialog->PrepareLC( R_SATUI_CLOSE_CHANNEL_ICON_NOTE );
                    //Set image
                    TFLOGSTRING(
                        "CSatUiViewAppUi::ShowBIPNoteL SetImage" )
                    CEikImage* image = new ( ELeave ) CEikImage();
                    CleanupStack::PushL( image );
                    image->SetBitmap( aIconBitmap );
                    iNoteDialog->SetImageL( image );
                    CleanupStack::Pop( image );
                    }
                else
                    {
                    TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL no Icon" )
                    iNoteDialog->PrepareLC( R_SATUI_CLOSE_CHANNEL_NOTE );
                    }
                
                // inform the user using received alpha id
                if ( ( aText.Length() > 0 ) &&
                    ( !aSelfExplanatory || !aIconBitmap ) )
                    {
                    TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL using \
                        received alpha id" )
                    iNoteDialog->SetTextL( aText );
                    }
                // inform the user using localized text
                else if ( aText.Length() == 0 )
                    {
                    TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL using \
                        localized text" )
                    iNoteDialog->SetTextL( *textHolder );
                    }
                    
                iForegroundControl = iNoteDialog;
                iNoteDialog->RunLD();    
                // iForegroundControl won't be clear until the note dialog
                // closed. It will avoid we lose the chance to set focus for
                // the wait dialog when the switch of foreground/background
                // happened.
                }
                
            CleanupStack::PopAndDestroy( textHolder ); // textHolder
            
            showNote = EFalse;
            break;
            }
        default:
            {
            showNote = EFalse;
            break;
            }
        }
    if ( showNote )
        {
        
        // First, stop normal wait note.
        if ( iWaitNote )
            {
            TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL() Closing WaitNote" )
            TRAP_IGNORE( iWaitNote->ProcessFinishedL() )
            iWaitNote = NULL;
            }
        
        if ( !iBipWaitNote )
            {
            TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL() no  BipWaitNote" )
            iBipWaitNote = new ( ELeave ) CSatUiWaitDialog(
                (REINTERPRET_CAST(CEikDialog**,&iBipWaitNote)),
                *iAdapter, ETrue );

            if ( ( aText.Length() > 0 ) && ( !aSelfExplanatory ||
                !aIconBitmap ) )
                {
                iBipWaitNote->SetTextL( aText );
                }
                
            if ( aIconBitmap )
                {
                //Set image
                TFLOGSTRING(
                    "CSatUiViewAppUi::ShowBIPNoteL SetImage" )    
                CEikImage* image = new ( ELeave ) CEikImage();
                CleanupStack::PushL( image );
                image->SetBitmap( aIconBitmap );      
                iBipWaitNote->SetImageL( image );
                CleanupStack::Pop( image );
                }
            
            iForegroundControl = iBipWaitNote;
            iBipWaitNote->ExecuteLD( resourceId );
            // iForegroundControl won't be clear until the BipWait dialog
            // closed. It will avoid we lose the chance to set focus for
            // the wait dialog when the switch of foreground/background
            // happened.
            }
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL() have BipWaitNote" )
            // BIP note already exists, update text if not with self
            // explanatory icon
            if ( ( aText.Length() > 0 ) &&
                ( !aSelfExplanatory || !aIconBitmap ) )
                {
                iBipWaitNote->SetTextL( aText );
                }
            else if ( aSelfExplanatory && aIconBitmap )
                {
                const TDesC emptyText( KNullDesC );
                iBipWaitNote->SetTextL( emptyText );
                }
            // BIP note already exists, update image
            if ( aIconBitmap )
                {
                //Set image
                TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL SetImage" )   
                CEikImage* image = new ( ELeave ) CEikImage();
                CleanupStack::PushL( image ); 
                image->SetBitmap( aIconBitmap );            
                iBipWaitNote->SetImageL( image );
                CleanupStack::Pop( image );
                }
            }
        }
    TFLOGSTRING( "CSatUiViewAppUi::ShowBIPNoteL() exit" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::RemoveNullCharacters
// Null characters are removed from text
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::RemoveNullCharacters( HBufC* aText )
    {
    TFLOGSTRING( "CSatUiViewAppUi::RemoveNullCharacters calling" )
    TPtr titleptr = aText->Des();
    TInt nullLocation( 0 );
    while ( ( nullLocation = titleptr.Locate( NULL ) ) >= 0 )
        {
        titleptr.Delete( nullLocation, 1 );
        }
    TFLOGSTRING( "CSatUiViewAppUi::RemoveNullCharacters exiting" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ConfirmSetUpCallL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ConfirmSetUpCallL( 
    const TDesC& aText,
    const TDesC& aSimAppName,
    TBool& aActionAccepted,
    const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory )
    {
    TFLOGSTRING( "CSatUiViewAppUi::ConfirmSetUpCallL calling" )
    
    aActionAccepted = EFalse;
    TPtrC text( aText );
    HBufC* textHolder = NULL;
    HBufC* titleHolder = NULL;    
    TInt resource( 0 );
    
    if ( aIconBitmap )
        {
        TFLOGSTRING( "CSatUiViewAppUi::ConfirmSetUpCallL have icon" )
        resource = R_SATUI_CONFIRM_CALL_CANCEL_QUERY_WITH_ICON;

        }
    else
        {
        TFLOGSTRING( "CSatUiViewAppUi::ConfirmSetUpCallL no icon" )
        resource = R_SATUI_CONFIRM_CALL_CANCEL_QUERY;
        
        TFLOGSTRING2( "CSatUiViewAppUi::ConfirmSetUpCallL length of aText, \
            %d", aText.Length() )
        if ( aText.Length() == 0 )
            {
            
            if ( aSimAppName.Length() > 0 )
                {
                titleHolder = aSimAppName.AllocLC();
                }
            else
                {
                titleHolder = StringLoader::LoadLC( R_QTN_SAT_QUERY_TITLE, 
                    iCoeEnv );
                }

            textHolder = StringLoader::LoadLC( R_TEXT_CALL_SETUP_BY_SIMATK, 
                *titleHolder, iCoeEnv );

            RemoveNullCharacters( textHolder );

            text.Set(*textHolder);
            }
        }

    TInt userResponse = RunQueryDialogL( resource, text, aIconBitmap,
        aSelfExplanatory, ESatUiSoftkeyCall );

    if ( textHolder )
        {
        CleanupStack::PopAndDestroy( textHolder ); //textHolder
        }

    if ( titleHolder )
        {
        CleanupStack::PopAndDestroy( titleHolder ); //titleHolder
        }
    
    if ( userResponse )
        {
        aActionAccepted = ETrue;
        }

    TFLOGSTRING( "CSatUiViewAppUi::ConfirmSetUpCallL exiting" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ShowNotAvailableNoteL
// Show note indicating SAT is not available
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ShowNotAvailableNoteL()
    {
    TFLOGSTRING( "CSatUiViewAppUi::ShowNotAvailableNoteL() called" )
    CAknNoteDialog* noteDialog = new (ELeave) CAknNoteDialog(
        REINTERPRET_CAST(CEikDialog**,&noteDialog),
        CAknNoteDialog::ENoTone,
        CAknNoteDialog::ELongTimeout );

    HBufC* textHolder = StringLoader::LoadLC( R_QTN_SAT_SERVICES_NOT_AVAILABLE,
            iCoeEnv );

    noteDialog->PrepareLC( R_SATUI_INFORMATION_NOTE );
    noteDialog->SetTextL( *textHolder );
    iForegroundControl = noteDialog;
    noteDialog->RunLD();
    iForegroundControl = NULL;
    
    CleanupStack::PopAndDestroy( textHolder );
    
    TFLOGSTRING( "CSatUiViewAppUi::ShowNotAvailableNoteL() exit" )    
    }
    
// ---------------------------------------------------------
// CSatUiViewAppUi::RunQueryIconDialogL
// Make and run note dialog with an icon
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSatUiViewAppUi::RunQueryDialogL(
    TInt aResourceId, const TDesC& aText, const CFbsBitmap* aIconBitmap,
    const TBool aSelfExplanatory, TInt aMskCommand )
    {
    TFLOGSTRING( "CSatUiViewAppUi::RunQueryDialogL called" )
    CEikImage* image = new(ELeave) CEikImage;
    CleanupStack::PushL( image );     
    iQueryDialog = CAknQueryDialog::NewL();
    iQueryDialog->PrepareLC( aResourceId );
    TInt userResponse( 0 );

    if ( aIconBitmap )
        {
        TFLOGSTRING( "CSatUiViewAppUi::RunQueryDialogL set icon" ) 
        image->SetBitmap( aIconBitmap );
    
        // Create mask for icon bitmap
        CFbsBitmap* mask = new(ELeave) CFbsBitmap;
        TSize size = aIconBitmap->SizeInPixels();
        MakeGetInputBitmapMask( mask, size );
        image->SetMask( mask );
        // Add icon to query
        iQueryDialog->QueryHeading()->SetHeaderImageL( image );
        }

    if ( !aSelfExplanatory || !aIconBitmap )
        {
        TFLOGSTRING( "CSatUiViewAppUi::RunQueryDialogL set text" )
        TPtrC textPtr( aText );
        // Icon is not self-explanatory, show also text
        iQueryDialog->SetPromptL( textPtr );            
        }
                   
    // Set MSK same as LSK without any text causing the icon drawing
    iQueryDialog->ButtonGroupContainer().SetCommandL(
        CEikButtonGroupContainer::EMiddleSoftkeyPosition, aMskCommand,
        KSatEmptyDes );
    iForegroundControl = iQueryDialog;
    userResponse = iQueryDialog->RunLD();
    iForegroundControl = NULL;
    CleanupStack::PopAndDestroy( image ); // image

    iQueryDialog = NULL;
    
    TFLOGSTRING2( "CSatUiViewAppUi::RunQueryDialogL exit,return: %d", \
        userResponse )
    return userResponse;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::CloseSatUI
// Start SatUi closing process
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::CloseSatUI()
    {
    TFLOGSTRING( "CSatUiViewAppUi::CloseSatUI called" )
    
    TRAP_IGNORE( HandleExitCommandL() );
    
    if ( !IsAppShutterRunning() )
        {
        TFLOGSTRING( "CSatUiViewAppUi::CloseSatUI RunAppShutter" )
        RunAppShutter();
        }    
    
    TFLOGSTRING( "CSatUiViewAppUi::CloseSatUI exit" )
    }
    
// ---------------------------------------------------------
// CSatUiViewAppUi::GetProfileParamsL
// Get and store settings from current profile 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::GetProfileParamsL( TSatTone aTone /*= ESatToneNotSet*/,
                                         TDes* aToneName /*= NULL*/ )
    {
    TFLOGSTRING( "CSatUiViewAppUi::GetProfileParamsL calling" )
    
    MProfile* profile = NULL;
    MProfileEngine* engine = NULL;
    
    engine = CreateProfileEngineL();
    CleanupReleasePushL( *engine );
        
    profile = engine->ActiveProfileLC();
               
    const MProfileTones& tones = profile->ProfileTones();
    
    // Takes a current warning and gametones setting.
    const TProfileToneSettings& ts = tones.ToneSettings();
    iWarningAndPlayTones = ts.iWarningAndGameTones;
    iVolume = ts.iRingingVolume;
    TFLOGSTRING2( "CSatUiViewAppUi::GetProfileParamsL \
        iVolume before mapping %d", iVolume )
    
    // Max volume from profile is KMaxVolumeFromProfile, Max volume from 
    // CMdaAudioToneUtility is different, maybe 10,000. So, 
    // rate = maxVolumeFromPlayer / KMaxVolumeFromProfile
    // User may never hear the TONE, because volume is too small.
    // iVolume times the rate make it can be heard.
    
    CMdaAudioToneUtility* toneUtl = CMdaAudioToneUtility::NewL( *this );
    TInt maxVolumeFromPlayer( toneUtl->MaxVolume() );
    iVolume *= maxVolumeFromPlayer / KMaxVolumeFromProfile;
    delete toneUtl;
    toneUtl = NULL;
    TFLOGSTRING2( "CSatUiViewAppUi::GetProfileParamsL \
        iVolume after mapping %d", iVolume )
    
    if ( ( ESatUserSelectedToneIncomingSms == aTone ) && ( aToneName ) )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetProfileParamsL message tone" )
        aToneName->Copy( tones.MessageAlertTone() );
        }
    else if( aToneName )
        {
        TFLOGSTRING( "CSatUiViewAppUi::GetProfileParamsL ring tone" )
        aToneName->Copy( tones.RingingTone1() );
        }
        
    CleanupStack::PopAndDestroy( 2 ); // engine, profile
    
    TFLOGSTRING( "CSatUiViewAppUi::GetProfileParamsL exits" )    
    }
    
// ---------------------------------------------------------
// CSatUiViewAppUi::HandleImmediateDigitResponse
// Check received key event.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::HandleImmediateDigitResponse( TInt aScanCode )
    {
    TFLOGSTRING2( "CSatUiViewAppUi::HandleImmediateDigitResponse() code: %d",
        aScanCode )
    TBool closeDialog( ETrue );

    // If number button is pressed
    if ( ( KKeyZero <= aScanCode ) && ( KKeyNine >= aScanCode ) )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleImmediateDigitResponse press \
            digit 0-9" )
        iKey = aScanCode;
        }
    else if ( EStdKeyNkpAsterisk == aScanCode || KHwAsterisk == aScanCode )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleImmediateDigitResponse press \
            *" )
        iKey = 0x2A;
        }
    else if ( EStdKeyHash == aScanCode )
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleImmediateDigitResponse press \
            #" )
        iKey = 0x23;
        }
    else 
        {
        TFLOGSTRING( "CSatUiViewAppUi::HandleImmediateDigitResponse press \
            others" )
        closeDialog = EFalse;
        iKey = 0x00;
        }
        
    if( closeDialog )
        {
        CloseDialogs();
        }
    
    TFLOGSTRING( "CSatUiViewAppUi::HandleImmediateDigitResponse exits" )
    }
      
// ---------------------------------------------------------
// CSatUiViewAppUi::PriorityVerifyAndClose
// Handle closing display text in high priority events
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::PriorityVerifyAndClose( const TWsEvent& aEvent )
    {
    TFLOGSTRING( "CSatUiViewAppUi::PriorityVerifyAndClose calling" )
    
    if ( ( iDisplayTextDialog || iDisplayTextIconDialog ) &&
        ( EEventFocusLost == aEvent.Type() ) )
        {
        // Open the WS server session.
        RWsSession wsSession;
        if ( KErrNone == wsSession.Connect() )
            {
            TInt wgFocus( wsSession.GetFocusWindowGroup() );
            TInt wgPriority(
                wsSession.GetWindowGroupOrdinalPriority( wgFocus ) );
            TFLOGSTRING2(
                "CSatUiViewAppUi::PriorityVerifyAndClose priority=%d",
                wgPriority )
            
            // For some reason window group priority for call dialogs was 0
            // so those cases must be checked in addition
            TInt callActiveState = EPSCTsyCallStateNone;
                
            // Get current call status from P&S
            const TInt errCode( RProperty::Get(
                KPSUidCtsyCallInformation,
                KCTsyCallState,
                callActiveState ) );
            
            // Receiving MT or creating MO call
            const TBool callIncoming(
                ( EPSCTsyCallStateAlerting == callActiveState ) ||
                ( EPSCTsyCallStateRinging == callActiveState ) ||
                ( EPSCTsyCallStateDialling == callActiveState ) ); 
            TFLOGSTRING2(
                "CSatUiViewAppUi::PriorityVerifyAndClose callActiveState=%d",
                callActiveState )
            TFLOGSTRING2(
                "CSatUiViewAppUi::PriorityVerifyAndClose iImmediateRsp=%d",
                iImmediateRsp )  
                
            // If the new focus window group has high priority
            // close dialog
            if ( ( wgPriority > 0 && iForeground ) || 
                 ( iImmediateRsp && callIncoming ) )
                {
                iHighPriorityEvent = ETrue;
                CloseDialogs();
                }
                
            wsSession.Close();
            }
        }
        
    TFLOGSTRING( "CSatUiViewAppUi::PriorityVerifyAndClose exiting" )
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::CloseCallControlNote
// Call back method, close the note in call control. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSatUiViewAppUi::CloseCallControlNoteL( TAny* aPtr )
    {
    TFLOGSTRING( "CSatUiViewAppUi::CloseCallControlNote calling" )

    if ( !aPtr ) 
        {
        return KErrArgument;
        }

    CSatUiViewAppUi* pAppUi = 
        reinterpret_cast<CSatUiViewAppUi*>( aPtr );
    
    TRAPD( err, pAppUi->iCcNote->CancelNoteL( pAppUi->iCcNoteId ) );
    if( KErrNone != err )
        {
        return err;
        }

    if ( pAppUi->iWait->IsStarted() ) 
        {
        pAppUi->iWait->AsyncStop();
        }
    delete pAppUi->iCcNote;
    pAppUi->iCcNote = NULL;
        
    TFLOGSTRING( "CSatUiViewAppUi::CloseCallControlNote exiting" )
    
    return 0;
    }

// ---------------------------------------------------------
// CSatUiViewAppUi::ShowSsErrorNoteL
// Show a Ss Error note. 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSatUiViewAppUi::ShowSsErrorNoteL()
    {
    TFLOGSTRING( "CSatUiViewAppUi::ShowSsErrorNoteL calling" )
    CAknNoteDialog* errorNote = new ( ELeave ) CAknNoteDialog( 
                                REINTERPRET_CAST( CEikDialog**,&errorNote ),
                                CAknNoteDialog::ENoTone, 
                                CAknNoteDialog::ELongTimeout );
    CleanupStack::PushL( errorNote );
        
    HBufC* textHolder = StringLoader::LoadLC(R_TEXT_NOT_DONE, iCoeEnv );
    errorNote->SetTextL( *textHolder );
    CleanupStack::PopAndDestroy( textHolder ); 
    CleanupStack::Pop( errorNote );
    errorNote->ExecuteLD( R_SATUI_INFORMATION_NOTE );
        
    TFLOGSTRING( "CSatUiViewAppUi::ShowSsErrorNoteL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatUiViewAppUi::ProfileState
// Return current profile state
// -----------------------------------------------------------------------------
//
TInt CSatUiViewAppUi::ProfileState()
    {
    TFLOGSTRING( "CSatUiViewAppUi::ProfileState called" )

    TInt profileId( 0 );
    CRepository* cr ( NULL );

    TRAPD( err, cr = CRepository::NewL( KCRUidProfileEngine ) );
    if ( ( KErrNone == err ) && cr )
        {
        TFLOGSTRING( "CSatUiViewAppUi::ProfileState no err" )
        // Get the ID of the currently active profile:
        cr->Get( KProEngActiveProfile, profileId );
        delete cr;
        }

    else
        {
        TFLOGSTRING2( "CR failed: %i", err )
        }

    TFLOGSTRING2( "CSatUiViewAppUi::ProfileState exit value: %d",
        profileId )

    return profileId;
    }
// -----------------------------------------------------------------------------
// CSatUiViewAppUi::StopPlayToneL
// Stops playing play tone. Used when user terminates play tone 
// -----------------------------------------------------------------------------
//
void CSatUiViewAppUi::StopPlayToneL()
    {
    TFLOGSTRING( "CSatUiViewAppUi::StopPlayToneL called" )
    if ( iPlayer && iWait->IsStarted() )
        {
        TFLOGSTRING( "CSatUiViewAppUi::StopPlayToneL\
            iPlayer exists" )
        iPlayToneError = ESatSessionTerminatedByUser;
        iWait->AsyncStop();
        }
    if ( iAudioPlayer )
        {
        TFLOGSTRING( "StopPlayToneL iAudioPlayer exists" )
        if ( iWait->IsStarted() )
            {
            TFLOGSTRING( "CSatUiViewAppUi::StopPlayToneL\
                iWait started" )
            // Remove note when play has been interrupted.
            if ( iPermanentNote )
                {
                iPermanentNote->ExitSleepingDialog();
                delete iPermanentNote;
                iPermanentNote = NULL;
                }
            iPlayToneError = ESatSessionTerminatedByUser;
            iWait->AsyncStop();
            }
        else
            {
            TFLOGSTRING( "CSatUiViewAppUi::StopPlayToneL\
                iWait not started" )
            iAudioPlayer->Stop();
            // When playing is stopped, deleting the audio player.
            // MapcPlayComplete is not called because user stops
            // the playing.
            if ( iAudioPlayer )
                {
                delete iAudioPlayer;
                iAudioPlayer = NULL;
                TFLOGSTRING( "CSatUiViewAppUi::StopPlayToneL\
                    iAudioPlayer deleted" )

                // Remove note after play has completed.
                if ( iPermanentNote )
                    {
                    TFLOGSTRING( "CSatUiViewAppUi::StopPlayToneL\
                                 iPermanentNote true" )
                    iPermanentNote->ExitSleepingDialog();
                    delete iPermanentNote;
                    iPermanentNote = NULL;
                    }

                // Show wait note only for samples that have 
                // duration, otherwise there is chance that wait 
                // note is not stopped.
                if ( TTimeIntervalMicroSeconds(
                    static_cast<TInt64>( 0 ) ) != iDuration )
                    {
                    ShowWaitNoteL();
                    }
                }
            }
        }
    TFLOGSTRING( "CSatUiViewAppUi::StopPlayToneL exiting" )
    }
    
// -----------------------------------------------------------------------------
// CSatUiViewAppUi::ForegroundAlive
// Check if foreground control is alive when HandleForegroundEventL is called.
// -----------------------------------------------------------------------------
//
TBool CSatUiViewAppUi::ForegroundAlive() const
    {
    TFLOGSTRING( "CSatUiViewAppUi::ForegroundAlive called" )
    
    // Only if value of iForegroundControl equals to one of the following 8
    // pointers, the alive value will be set to true. This will check if 
    // the foreground control is alive.
    TBool alive = 
        iForegroundControl == static_cast<CCoeControl*>( iWaitNote ) 
        || iForegroundControl == static_cast<CCoeControl*>( 
           iDisplayTextIconDialog )
        || iForegroundControl == static_cast<CCoeControl*>( 
           iDisplayTextDialog )
        || iForegroundControl == static_cast<CCoeControl*>( iGetInputDialog )
        || iForegroundControl == static_cast<CCoeControl*>( iPermanentNote )
        || iForegroundControl == static_cast<CCoeControl*>( iNoteDialog )
        || iForegroundControl == static_cast<CCoeControl*>( iBipWaitNote )
        || iForegroundControl == static_cast<CCoeControl*>( iQueryDialog );
    
    TFLOGSTRING2( "CSatUiViewAppUi::ForegroundAlive exiting alive=%d", alive )
    
    return alive;
    }

// End of File
