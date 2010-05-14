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
#include <QtTest/QtTest>
#include "vmbxqtuihandler.h"

VmbxQtUiHandler::VmbxQtUiHandler(QObject* parent): QObject(parent)
{
    qDebug("mock VmbxQtUiHandler");
}

VmbxQtUiHandler::~VmbxQtUiHandler()
{
    qDebug("mock ~VmbxQtUiHandler");
}


void VmbxQtUiHandler::showVmbxQueryDialog(const TVmbxType& /*aType*/,
                          QString& /*aNumber*/, int& aResult)
{
    qDebug("mock showVmbxQueryDialog");
    aResult = KErrNone;
}


void VmbxQtUiHandler::showDefineSelectionDialog(
    TVmbxType& /*aType*/, int& aResult)
{
    qDebug("mock showDefineSelectionDialog");
    // Make sure no leave in CVoiceMailboxImpl::QueryVmbxTypeL(
    aResult = KErrNotFound;
}

void VmbxQtUiHandler::showInformationNote(const TVmbxNoteType /*aType*/)
{
    qDebug("mock showInformationNote");    
}

void VmbxQtUiHandler::showCallSelectionDialog(
            const QList<CVoiceMailboxEntry *> /*entryList*/,
            TVoiceMailboxParams &/*params*/, int &/*result*/ )
{
    qDebug("mock showCallSelectionDialogL");  
}

//End of file
