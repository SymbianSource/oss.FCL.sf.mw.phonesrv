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
* Description: 
*
*/

// System includes
#include <hbevent.h>

// User includes
#include "testdialpadlineedit.h"


// External function prototypes

// Local constants

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========
/*!
     Constructor.
 */
TestDialpadLineEdit::TestDialpadLineEdit(QGraphicsItem *parent) : 
        DialpadLineEdit(parent)
{
}

/*!
     Destructor.
 */
TestDialpadLineEdit::~TestDialpadLineEdit()
{
}

void TestDialpadLineEdit::testFocusOutEvent(QFocusEvent * event) 
{
    DialpadLineEdit::focusOutEvent(event);
}
void TestDialpadLineEdit::testFocusInEvent(QFocusEvent * event) 
{
    DialpadLineEdit::focusInEvent(event);
}
