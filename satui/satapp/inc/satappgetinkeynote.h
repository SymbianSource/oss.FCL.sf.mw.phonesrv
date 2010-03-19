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
*
*/

#ifndef SATAPPGETINKEYNOTE_H
#define SATAPPGETINKEYNOTE_H

#include <exception>
#include <QObject>
#include <hbmessagebox.h>


class SatAppGetInkeyNote : public HbMessageBox
{
    Q_OBJECT

public:

    SatAppGetInkeyNote(const QString &text,
                       MessageBoxType type=MessageTypeInformation, 
                       QGraphicsItem *parent=0);

    virtual ~SatAppGetInkeyNote();

signals:

    void digitalKeyPressed(const int key);

public:
    void  keyPressEvent(QKeyEvent *event);

    };
#endif //SATAPPGETINKEYNOTE_H

