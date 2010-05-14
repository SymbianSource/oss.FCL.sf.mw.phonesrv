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
*/

#include <mpbutil.h>
#include <e32base.h>
#include <e32des8.h>

CPhoneBookBuffer::CPhoneBookBuffer():iMonitor(NULL,0,0)
{
}

void CPhoneBookBuffer::Set(TDes8* /*aData*/){}

TInt CPhoneBookBuffer::AddNewEntryTag()
{ return 0; }

TInt CPhoneBookBuffer::AddNewNumberTag()
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, TUint8 /*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, TUint16 /*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, TUint32 /*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, const TDesC8 &/*aData*/)
{ return 0; }

TInt CPhoneBookBuffer::PutTagAndValue(TUint8 /*aTagValue*/, const TDesC16 &/*aData*/)
{ return 0; }

TInt CPhoneBookBuffer::RemovePartialEntry()
{ return 0; }

void CPhoneBookBuffer::StartRead(){}

TInt CPhoneBookBuffer::GetTagAndType(TUint8 &/*aTagValue*/, TPhBkTagType &/*aDataType*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TUint8 &/*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TUint16 &/*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TUint32 &/*aInteger*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TPtrC8 &/*aData*/)
{ return 0; }

TInt CPhoneBookBuffer::GetValue(TPtrC16 &/*aData*/)
{ return 0; }

void CPhoneBookBuffer::SkipValue(TPhBkTagType /*aDataType*/){}

TInt CPhoneBookBuffer::BufferLength()
{ return 0; }

TInt CPhoneBookBuffer::RemainingReadLength()
{ return 0; }
