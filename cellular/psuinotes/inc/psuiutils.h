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

#ifndef PSUIUTILS_H
#define PSUIUTILS_H

#include <qvariant.h>

#ifdef BUILD_PSUINOTES
#define PSUINOTES_EXPORT Q_DECL_EXPORT
#else
#define PSUINOTES_EXPORT Q_DECL_IMPORT
#endif

class XQSettingsManager;

/*!
    \class PsUiUtils
    \brief The static functionality.
 */
class PSUINOTES_EXPORT PsUiUtils : public QObject
{
    Q_OBJECT

public:
    /*!
      Error code text conversion
      */
    static bool errorCodeTextMapping(const int errorcode, QString &errorText);
   
};

/*!
    \class SettingsWrapper
    \brief Wraps central repository and P&S dependency.
 */
class PSUINOTES_EXPORT PsUiSettingsWrapper : public QObject
{
    Q_OBJECT
public:
    PsUiSettingsWrapper(QObject *parent = NULL);
    ~PsUiSettingsWrapper();
public:
    
    /*!
     Call waiting distiquish not provisioned support
     */
    bool isFeatureCallWaitingDistiquishNotProvisionedEnabled();
    
    /*!
     Checks if phone is in offline mode or not.
     Return true if phone is in offline mode.
     Return false if phone is not in offline mode.
     */
    bool isPhoneOffline() const;
    
    /*!
     Number grouping support
     */
    bool numberGroupingSupported() const;

private:
    /*!
      Read cenrep value. 
      */
    QVariant readCenrepValue( const long int uid, const unsigned long int key) const;

private: // Data
    // Own
    XQSettingsManager* m_Settings;
};


#endif // PSUIUTILS_H
