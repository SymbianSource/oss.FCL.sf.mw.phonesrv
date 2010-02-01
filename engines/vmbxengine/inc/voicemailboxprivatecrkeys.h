/*
* Copyright (c) 2002 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Private keys for voice mailbox.
*
*/


#ifndef VOICEMAILBOXPRIVATECRKEYS_H
#define VOICEMAILBOXPRIVATECRKEYS_H
#include <e32std.h>

/** VoiceMailbox Settings API */
/** Provides access to the VoiceMailbox numbers and settings */

const TUid KCRUidVoiceMailbox = {0x101F874F};

/** Voice Mailbox number for primary line 
*
* Integer type
*
* Default value: 0
*/
const TUint32 KVmbxNumberLineAuxiliary    = 0x00000000;

/** Voice Mailbox number for auxiliary line 
*
* Integer type
*
* Default value: 0
*/
const TUint32 KVmbxNumberLinePrimary    = 0x00000001;

/** Voice Message Callback Number, a field in VMN for a direct callback Number
*
* Text type
*
* Default value: ""
**/
const TUint32 KVmCallbackNumber            = 0x00000002;

/** 
* Voicemail Summary Info Set flag, a field in VMN 
*
* Integer type
*
* Possible values:
* 0 (not set)
* 1 (set)
*
* Default value: 0
**/
const TUint32  KVmSummaryInfoIsSet        = 0x00000005;

/** Local Variation key. Voice Mailbox Number default for CDMA operators. 
* Default number can be Blank, MIN, MDN, and etc 
*
* Integer type
*
* Possible values:
* 0 (EBlank)
* 1 (EDedicatedVMN)
* 2 (EMIN)
* 3 (EMDN)
* 4 (EOnePlusMIN)
* 5 (EOnePlusMDN)
* 6 (EMDNCustom)
* 7 (EOnePlusMDNCustom)
*
* Default value: 0
**/
const TUint32  KVmNumOpts                = 0x00000009;

/**
 * Values for KVmNumOpts
 */
enum TVmNumOpts 
    {
    // For GSM SIM- and CDMA RUIM-based voice mbox number.
    EVmNumOptsBlank = 0,
    EVmNumOptsDedicatedVMN,
    EVmNumOptsMIN,
    EVmNumOptsMDN,
    EVmNumOptsOnePlusMIN,
    EVmNumOptsOnePlusMDN,
    EVmNumOptsMDNCustom,
    EVmNumOptsOnePlusMDNCustom,
    EVmNumOptsSharedData
    };

/** Voice Message Option to configure UI related features.
*
* Integer type
*
* This is a bit flag pattern, whose value is specified by setting individual bits.
* For example if you want to set 3rd and 2nd bits: 0100b and 0010b ===> 0110b ==> 
* 6 in decimal representation ==> use value 6.
* Possible values.
* Bit 1 = Set this bit to prevent user from defining or 
* modifying the voice mailbox number, clear to allow.
* Rest of the bits are reserved for future use and currently not used.
*
* Default value: 2
**/
const TUint32 KVmUIOpts                    = 0x00000010;

/**
 * Values for KVmUIOpts
 */
enum TVmUiOpts
    {
    // Prevent all to change voice mail number
    EUiNmbrChangeNotAllowed = 0x01
    };

/** Flag which decides where the vmbx number is to be stored. 
* If this is Off (equals 0) the voice mailbox number is stored in SIM memory, 
* else it is stored in Phone memory.
*
* Integer type
*
* Default value: 0
*/
const TUint32 KVmUsesSimMemory          = 0x00000011;

/** Video Mailbox number for primary line 
*
* Text type
*
* Default value: ""
*/
const TUint32 KVideoMbxNumberLinePrimary    = 0x00000012;

/** Video Mailbox number for auxiliary line 
*
* Text type
*
* Default value: ""
*/
const TUint32 KVideoMbxNumberLineAuxiliary    = 0x00000013;

/** Flag which decides if writing to sim allowed.
* If this is On (equals 1) writing to SIM is not allowed.
* Writing is allowed with value 0 
* Value
* Integer type
*
* Default value: 1
*/
const TUint32 KReadOnlySim = 0x00000016;

/** Voice Mailbox number backup for primary line 
*
* Integer type
*
* Default value: 0
*/
const TUint32 KVmbxTmpNumberLinePrimary    = 0x000000017;
/** Voice Mailbox number for auxiliary line 
*
* Integer type
*
* Default value: 0
*/
const TUint32 KVmbxTmpNumberLineAuxiliary    = 0x000000018;

/** Flag which indicates that user has edited primary number.
* If this is off (equals 0), number from SIM is used,
* when available.
* Integer type
*
* Default value: 0
*/
const TUint32 KUserEditedNumber = 0x00000019;

/** Flag which indicates that user has edited auxiliary number.
* If this is off (equals 0), number from SIM is used,
* when available.
* Integer type
*
* Default value: 0
*/
const TUint32 KUserEditedNumberAuxiliary = 0x00000020;

#endif      // VOICEMAILBOXPRIVATECRKEYS_H