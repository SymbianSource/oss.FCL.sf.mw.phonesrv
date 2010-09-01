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
* Description:  Inline implementation of CCbsUiappDocument class.
*
*
*/



// ---------------------------------------------------------
// CCbsUiappDocument::OpenFileL
// 
// Stop doc file being created
//
// ---------------------------------------------------------
//
CFileStore* CCbsUiappDocument::OpenFileL( TBool /*aDoOpen*/, 
                             const TDesC& /*aFilename*/,
                             RFs& /*aFs*/)
    { 
    return NULL; 
    }


// ---------------------------------------------------------
// CCbsUiappDocument::OpenFileL
// 
// Stop doc file being created
//
// ---------------------------------------------------------
//
void CCbsUiappDocument::OpenFileL( CFileStore*& /*aFileStore*/, RFile& /*aFile*/ )
    {  
    }

// End of file
