/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Implementation of CCbsUiLanguageSettingPage.
*
*
*/


// INCLUDES
#include    <cbsuiApp.rsg>
#include    <AknQueryDialog.h>
#include    "CCbsUiLanguageSettingPage.h"
#include    "CCbsUiListQueryDialog.h"
#include    "CbsUiPanic.h"
#include    "CbsUiConstants.h"
#include    "MCbs.h"



// CONSTANTS
const TInt KFirstLaunch = 1;
const TInt KAverageReceivedLanguages = 4;



// ================= MEMBER FUNCTIONS =======================


// C++ default constructor can NOT contain any code, that
// might leave.
//
CCbsUiLanguageSettingPage::CCbsUiLanguageSettingPage( 
    TInt aResourceID, 
    TInt& aCurrentSelectionIndex,
    CDesCArrayFlat* aItemArray,
    MCbs& aServer ) 
    : 
    CAknRadioButtonSettingPage(aResourceID, aCurrentSelectionIndex, aItemArray),
    iServer(aServer),
    iSelectionIndex(aCurrentSelectionIndex),
    iArray(*aItemArray)    
    {
    }

// ---------------------------------------------------------
// CCbsUiLanguageSettingPage::OkToExitL
// 
// (other iItems were commented in a header).
// ---------------------------------------------------------
//
TBool CCbsUiLanguageSettingPage::OkToExitL( TBool aAccept )
    {   
    __ASSERT_DEBUG( &iServer, CbsUiPanic(EServerNullError) );

    // if ok key has been pushed, then this level options
    // settings will be made
    if ( aAccept )
        {        
        TCbsDbLanguages lang;
        iServer.GetLanguages(lang);
        
        switch ( iSelectionIndex )
            {
            case ECbsUiAll:
                lang.iLanguages[ECbsAll] = ETrue;
                lang.iLanguages[ECbsOther] = EFalse;
                break;
            case ECbsUiSelected: 
                // don't set languages after this, allready set in language dlg
                LaunchLanguageDialogL();
                break;
            case ECbsUiOther:
                lang.iLanguages[ECbsAll] = EFalse;
                lang.iLanguages[ECbsOther] = ETrue;
                break;
            default:
                __ASSERT_DEBUG( 0, CbsUiPanic(EErrInvalidValue) );
                break;
            }    

        if ( iSelectionIndex == ECbsUiAll || iSelectionIndex == ECbsUiOther )
            {
            User::LeaveIfError( iServer.SetLanguages(lang) );
            }
        }


    return ETrue;
    }


// ---------------------------------------------------------
// CCbsUiLanguageSettingPage::OfferKeyEventL
// 
// (other iItems were commented in a header).
// ---------------------------------------------------------
//
TKeyResponse CCbsUiLanguageSettingPage::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent, 
    TEventCode aType )
    {
    __ASSERT_DEBUG( ListBoxControl(), CbsUiPanic(EListBoxNullError) );
    
    switch ( aKeyEvent.iCode )
        {
        case EKeyOK:
            LaunchLanguageDialogL();
            break;       
        default:
            break;
        }

    return CAknSettingPage::OfferKeyEventL( aKeyEvent, aType );
    }

// ---------------------------------------------------------
// CCbsUiLanguageSettingPage::GetLanguagesL
// 
// (other iItems were commented in a header).
// ---------------------------------------------------------
//
void CCbsUiLanguageSettingPage::GetLanguagesL(
    CArrayFix<TInt>& aIndexArray, TCbsDbLanguages& aLanguages )
    {
    iServer.GetLanguages(aLanguages);
    
    for ( TInt j(0); j < ECbsOther; j++ )
        {
        if ( aLanguages.iLanguages[j] )
            {
            aIndexArray.AppendL(j);
            }
        }
    }

// ---------------------------------------------------------
// CCbsUiLanguageSettingPage::LaunchLanguageDialogL
// 
// (other iItems were commented in a header).
// ---------------------------------------------------------
//
void CCbsUiLanguageSettingPage::LaunchLanguageDialogL()
    {
    iCurrentLaunchingId++;
    
    if ( iCurrentLaunchingId == KFirstLaunch )
        {        
        if ( ListBoxControl()->CurrentItemIndex() == ECbsUiSelected )
            {
            CArrayFixFlat<TInt>* indexArray = 
                new (ELeave) CArrayFixFlat<TInt>(KAverageReceivedLanguages);
            CleanupStack::PushL(indexArray);
            
            TCbsDbLanguages languages;        
            GetLanguagesL(*indexArray, languages);
            
            CCbsUiListQueryDialog* dlg = new (ELeave)
                CCbsUiListQueryDialog(*indexArray);
            TInt ret( dlg->ExecuteLD(R_LANGUAGE_MULTISELECTION_LIST_QUERY) );
            
            if( ret )
                {
                SetLanguagesL(*indexArray, languages);
                }
            else
                {
                iCurrentLaunchingId--;
                User::Leave( KErrNone );
                }
            
            CleanupStack::PopAndDestroy(); // indexArray;
            }
        }
    }

// ---------------------------------------------------------
// CCbsUiLanguageSettingPage::SetLanguagesL
// 
// (other iItems were commented in a header).
// ---------------------------------------------------------
//
void CCbsUiLanguageSettingPage::SetLanguagesL(
    CArrayFix<TInt>& aIndexArray, TCbsDbLanguages& aLanguages )
    {     
    // just clearing the language selection before setting them
    for ( TInt i(0); i < ECbsOther; i++ )
        {
        aLanguages.iLanguages[i] = EFalse;
        }

    // set the values which are chosen
    if ( &aIndexArray )
        {        
        TInt count( aIndexArray.Count() );

        for (TInt i(0); i < count; i++)
            {
            TInt selectedIndex( aIndexArray.At(i) );
            __ASSERT_DEBUG( selectedIndex < ECbsOther, 
                CbsUiPanic(EErrIndexOutOfBounds) );
            aLanguages.iLanguages[selectedIndex] = ETrue;
            }
       
        TBool languageSelected( count > 0 );
        if ( languageSelected )
            {
            aLanguages.iLanguages[ECbsAll] = EFalse;            
            aLanguages.iLanguages[ECbsOther] = EFalse;
            }
        else 
            {
            // The user can clear the languages and then previously
            // defined value will be selected.
            // If other has not been previously selected, then
            // all has to be selected
            if ( !aLanguages.iLanguages[ECbsOther] )
                {                
                aLanguages.iLanguages[ECbsAll] = ETrue;
                }
            }       

        User::LeaveIfError( iServer.SetLanguages( aLanguages ) );
        }      
    }


//  End of File  
