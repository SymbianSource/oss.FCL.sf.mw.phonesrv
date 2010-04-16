/*
* Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the Ut_vmbxCpPlugin class
*
*/


#ifndef UT_VMBXCPPLUGIN_H
#define UT_VMBXCPPLUGIN_H

// System includes
#include <QtTest/QtTest>
#include <QString>
#include <QStringList>
#include <QSignalSpy>
#include <QMetaType>
#include <QObject>


// Class forwards
class VmbxCpPlugin;
class VmbxCpGroup;

/*!
    Mt_vmbxEngine module test class.

    Tests Ut_vmbxCpPlugin interface.
    See cpp file for more information.

    @lib Ut_vmbxCpPlugin.lib
*/

// Class declaration
class Ut_vmbxCpPlugin : public QObject
{
    Q_OBJECT

public:
    Ut_vmbxCpPlugin();

    virtual ~Ut_vmbxCpPlugin();

private slots: 

    /*!
        Called before the first testfunction is executed.
    */
    void initTestCase();

    /*!
        Called after the last testfunction has been executed.
    */
    void cleanupTestCase();

    /*!
        Test method in VmbxCpPlugin
    */
    void testCreateSettingFormItemData();

private:  //data

    /*!
        Own.
    */
    VmbxCpPlugin *mCpPlugin;

};

#endif // _UT_VMBXCPPLUGIN_H
