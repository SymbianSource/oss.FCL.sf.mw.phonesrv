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
* Description:  Header file for the CCbsUisettingsDialog class.
*
*
*/


#ifndef __CCBSUISETTINGSDIALOG_H
#define __CCBSUISETTINGSDIALOG_H

//  INCLUDES
#include <eikdialg.h>
#include <eiklbx.h>
#include <aknappui.h>
#include <RCbs.h>
#include <CbsCommon.h>



// CLASS DECLARATION
/**
*	It is the container class viewing messages.
*/

class CCbsUiSettingsDialog : public CEikDialog, MEikListBoxObserver
    {
public:
    ~CCbsUiSettingsDialog();
    CCbsUiSettingsDialog( RCbs* aServer );

private:
	TBool OkToExitL(TInt aButtonId);
	void PreLayoutDynInitL();
	void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );
	void HandleListViewActionL( CEikListBox* aListBox );

private:
	CEikListBox* iInbox;
	RCbs* iServer;

    };

#endif // __CCBSUISETTINGSDIALOG_H

// End of File
