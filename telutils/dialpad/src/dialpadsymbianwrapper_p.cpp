/*!
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
* Description: Provides information about ongoing calls.
*
*/

#include "dialpadsymbianwrapper.h"
#include "dialpadsymbianwrapper_p.h"
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>

DialpadSymbianWrapperPrivate::DialpadSymbianWrapperPrivate(DialpadSymbianWrapper *parent) : 
    q_ptr(parent)
{
    TRAPD(error, mVmbx = CVoiceMailbox::NewL());
    qt_symbian_throwIfError(error);
}

DialpadSymbianWrapperPrivate::~DialpadSymbianWrapperPrivate()
{
    delete mVmbx;
    mVmbx = NULL;
}

int DialpadSymbianWrapperPrivate::getMailboxNumber(QString &vmbxNumber)
{
    int errValue(KErrNone);
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    errValue = mVmbx->QueryVmbxType( vmbxParams );
    
    if ((KErrNone == errValue) && 
        (KErrNone == mVmbx->GetStoredEntry(vmbxParams, vmbxEntry))) { 
        // Number retrieved succesfully:
        vmbxNumber = getVmbxNumber(*vmbxEntry);
     }
    // Entry ownership was transferred.
    delete vmbxEntry;
    vmbxEntry = NULL;
    return errValue;
}


int DialpadSymbianWrapperPrivate::defineMailboxNumber(QString &vmbxNumber)
{
    CVoiceMailboxEntry* vmbxEntry = NULL;
    TVoiceMailboxParams vmbxParams;
    int errValue = mVmbx->QueryVmbxType( vmbxParams );
    
    if ((KErrNotFound == errValue)) {
        errValue = mVmbx->QueryNewEntry(vmbxParams, vmbxEntry);
        if (KErrNone == errValue) {
            mVmbx->SaveEntry(*vmbxEntry);
            // Do appropriate tasks, e.g. save number.
            vmbxNumber = getVmbxNumber(*vmbxEntry);
        }
    }
    return errValue;
}

QString DialpadSymbianWrapperPrivate::getVmbxNumber(CVoiceMailboxEntry &vmbxEntry)
{
    TPtrC ptrNumber(KNullDesC);
    QString dialpadText = NULL;
    if(KErrNone == vmbxEntry.GetVmbxNumber(ptrNumber)) {
        dialpadText = QString::fromUtf16(ptrNumber.Ptr(), ptrNumber.Length());
    }
    return dialpadText;
}

