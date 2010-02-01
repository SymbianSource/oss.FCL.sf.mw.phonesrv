/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A list query where voice mail type can be selected.
 *
*/


#ifndef VMLIST_H
#define VMLIST_H

// INCLUDES
#include <aknlistquerydialog.h>


// CLASS DECLARATION

/**
*  ?one_line_short_description.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
class CVmListQueryDialog : public CAknListQueryDialog
{
 public:
	static CVmListQueryDialog* NewL( TInt* aIndex );
	virtual ~CVmListQueryDialog();

 public:  
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

    
 protected:  
    TBool OkToExitL(TInt aButtonId);
 private:
	CVmListQueryDialog( TInt* aIndex );  

 private:  // Data
     TBool iOkToExit;

};


#endif  // VMLIST_H
            
// End of File
