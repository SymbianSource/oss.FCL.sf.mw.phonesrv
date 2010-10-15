/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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
#include <smcmockclassincludes.h>
#include <hbapplication.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbApplication::HbApplication
// -----------------------------------------------------------------------------
//
HbApplication::HbApplication( 
          int &argc,
          char *argv[],
          Hb::ApplicationFlags flags ) : QApplication(argc, argv)
    {
    
    }

#if defined(Q_OS_SYMBIAN)
// -----------------------------------------------------------------------------
// HbApplication::HbApplication
// -----------------------------------------------------------------------------
//
HbApplication::HbApplication( 
        QApplication::QS60MainApplicationFactory factory,
        int &argc, char *argv[], Hb::ApplicationFlags flags ) :QApplication(argc, argv)
    {
    
    }
#endif // Q_OS_SYMBIAN

// -----------------------------------------------------------------------------
// HbApplication::~HbApplication
// -----------------------------------------------------------------------------
//
HbApplication::~HbApplication(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbApplication::symbianEventFilter
// -----------------------------------------------------------------------------
//
bool HbApplication::symbianEventFilter( 
        const QSymbianEvent * event )
    {
    //SMC_MOCK_METHOD1( bool, const QSymbianEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbApplication::activate
// -----------------------------------------------------------------------------
//
void HbApplication::activate(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbApplication::activityManager
// -----------------------------------------------------------------------------
//
HbActivityManager * HbApplication::activityManager(  )
    {
    SMC_MOCK_METHOD0( HbActivityManager * )
    }


// -----------------------------------------------------------------------------
// HbApplication::activateReason
// -----------------------------------------------------------------------------
//
Hb::ActivationReason HbApplication::activateReason(  ) const
    {
    SMC_MOCK_METHOD0( Hb::ActivationReason )
    }


// -----------------------------------------------------------------------------
// HbApplication::activateParams
// -----------------------------------------------------------------------------
//
QVariantHash HbApplication::activateParams(  ) const
    {
    SMC_MOCK_METHOD0( QVariantHash )
    }


// -----------------------------------------------------------------------------
// HbApplication::activateId
// -----------------------------------------------------------------------------
//
QString HbApplication::activateId(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbApplication::activateData
// -----------------------------------------------------------------------------
//
QVariant HbApplication::activateData(  )
    {
    SMC_MOCK_METHOD0( QVariant )
    }


// -----------------------------------------------------------------------------
// HbApplication::hideSplash
// -----------------------------------------------------------------------------
//
void HbApplication::hideSplash(  )
    {
    SMC_MOCK_METHOD0( void )
    }


