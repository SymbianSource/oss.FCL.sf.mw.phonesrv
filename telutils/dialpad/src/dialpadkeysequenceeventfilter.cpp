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
* Description: Implements key sequence recognition filter for Dialpad.
*
*/
#include <qdebug>
#include <qkeyevent>
#include <hblineedit.h>
#include <hbstringutil.h>
#ifdef Q_OS_SYMBIAN
#include <xqservicerequest.h>
#include <xqserviceutil.h>
#endif //Q_OS_SYMBIAN
#include "dialpadkeysequenceeventfilter.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

/*!
  DialpadKeySequenceEventFilter::DialpadKeySequenceEventFilter.
 */
DialpadKeySequenceEventFilter::DialpadKeySequenceEventFilter(
    Dialpad* dialpad, QObject* parent) 
    :
    QObject(parent), mDialpad(dialpad)
{
    PHONE_TRACE;
}


/*!
  DialpadKeySequenceEventFilter::~DialpadKeySequenceEventFilter.
 */
DialpadKeySequenceEventFilter::~DialpadKeySequenceEventFilter()
{
    PHONE_TRACE;
}


/*!
  DialpadKeySequenceEventFilter::eventFilter.
 */
bool DialpadKeySequenceEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    PHONE_TRACE;
    Q_UNUSED(watched)
    
    const bool eventFiltered = false;
#ifdef Q_OS_SYMBIAN
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    const int keyCode = keyEvent->key();
    const int eventType = event->type();
    
    // Code is executed after '#' is pressed as specified in Dialer UI 
    // specification.
    QString keySequenceCandidate = HbStringUtil::convertDigitsTo(
        mDialpad->editor().text(), WesternDigit);
    if (QEvent::KeyRelease == eventType && 
        Qt::Key_NumberSign == keyCode &&
        preValidateKeySequence(keySequenceCandidate)) {
        XQServiceRequest request(
            "com.nokia.services.telephony",
            "executeKeySequence(QString)",
            true);
        
        // Workaround for getting focus back to dialer after service request.
        XQRequestInfo requestInfo;
        requestInfo.setBackground(true);
        request.setInfo(requestInfo);
        
        QVariant keySequenceProcessed;
        request << keySequenceCandidate;
        bool requestOk = request.send(keySequenceProcessed);
        if (requestOk && keySequenceProcessed.toBool()) {
            mDialpad->editor().setText(QString(""));
        }
    }
#else
    Q_UNUSED(event)
#endif // Q_OS_SYMBIAN
    
    return eventFiltered;
}


/*!
  DialpadKeySequenceEventFilter::preValidateKeySequence.
  Checks that key sequence starts with '*#'and ends with '#'.
 */
bool DialpadKeySequenceEventFilter::preValidateKeySequence(
    const QString &sequence)
{
    const int KMinimumLength = 4;
    bool isValid = false;
    
    int lengthOfSequence = sequence.length();
    if (KMinimumLength <= lengthOfSequence) {
        isValid = 
            (sequence.at(0) == '*') && 
            (sequence.at(1) == '#') && 
            (sequence.at(lengthOfSequence - 1) == '#');
    }
    
    return isValid;
}
