/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Constant values for the Phone Setting UI Notes
*
*/

#ifndef CPSUIINTERNALCONSTANTS_H
#define CPSUIINTERNALCONSTANTS_H

// INCLUDES

//CONSTANTS
const TInt KPSUINoBscGroup = 255;      //unknown basic service group
const TInt KPSUIDetailsText = 64;      //max. length of text in detailed note
const TText KLRMarker = 0x200E;        //LRM marker

// Phone Setting Notes UI's Note Controller Singleton UID
static const TUid KUidNoteControllerSingleton = { 0x0C };

_LIT( KPSUIListDivider, "\n" );        //list row divisor

// Type of Call Forwarding request.
//   EPsuiNoCF - not a call forwarding request
//   EPsuiCFNry - call forwarding with number and timeout information
//   EPsuiCFOther - call forwarding with number information
enum TCFType
    {
    EPsuiNoCF = 1,
    EPsuiCFNry, 
    EPsuiCFOther
    };

// Note types.
enum TPsuiNoteTypes
    {
    EPsuiConfirmationNote,
    EPsuiWaitingNote,
    EPsuiErrorNote,
    EPsuiInformationNote
    };

#endif //CPSUIINTERNALCONSTANTS_H
