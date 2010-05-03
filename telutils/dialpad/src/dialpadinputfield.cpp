/*!
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
* Description: Input field
*
*/

#include <QtGui>

#include <hblineedit.h>
#include <hbinputeditorinterface.h>
#include <hbinputstandardfilters.h>
#include <hbdeviceprofile.h>

#include "dialpadinputfield.h"
#include "dialpadbuttonstyle.h"
#include "dialpadbutton.h"

static const QString HbBackspaceIcon("qtg_mono_backspace2");
static const int DialpadAutoRepeatInterval = 150; // ms
static const int DialpadAutoRepeatDelay = 1000; // ms
static const qreal DialpadComponentMargin = 0.75; // units
static const qreal DialpadBackspaceHeight = 9.4; // units
static const qreal DialpadInputFieldHeight = 6.3; // units
static const int DialpadMaxEditStringLenght = 100;

DialpadInputField::DialpadInputField(QGraphicsItem* parent)
    : HbWidget(parent)
{
    // create editor
    mNumberEditor = new HbLineEdit(this);
    HbEditorInterface editorInterface(mNumberEditor);
    editorInterface.setFilter(HbPhoneNumberFilter::instance());
    editorInterface.setUpAsPhoneNumberEditor();
    editorInterface.setConstraints(HbEditorConstraintIgnoreFocus);
    mNumberEditor->setMaxLength(DialpadMaxEditStringLenght);

    // create backspace button
    mBackspace = new DialpadButton(this);
    mFunctionButtonStyle = new DialpadButtonStyle();
    mFunctionButtonStyle->setButtonStyle(
        DialpadButtonStyle::FunctionButtonStyle);
    mBackspace->setStyle(mFunctionButtonStyle);
    mBackspace->setButtonType(DialpadButton::FunctionButton); // for css
    mBackspace->setFocusPolicy(Qt::NoFocus);
    mBackspace->setFlag(QGraphicsItem::ItemIsFocusable,false);
    mBackspace->setIcon(HbIcon(HbBackspaceIcon));
    mBackspace->setEnabled(false);
    QString buttonName;
    buttonName.setNum(Qt::Key_Backspace);
    mBackspace->setObjectName(buttonName);
    mBackspace->setAutoRepeat(true);
    mBackspace->setAutoRepeatInterval(DialpadAutoRepeatInterval);
    mBackspace->setAutoRepeatDelay(DialpadAutoRepeatDelay);

    connect(mNumberEditor,SIGNAL(contentsChanged()),
            SLOT(onEditorContentChanged()));

    // set input field layout
    qreal unit = HbDeviceProfile::current().unitValue();
    mHeight = (int) DialpadInputFieldHeight * unit;
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
    layout->addItem(mNumberEditor);
    layout->setAlignment(mNumberEditor,Qt::AlignVCenter);
    layout->addItem(mBackspace);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(DialpadComponentMargin* unit);
    // layout parameters
    mBackspace->setPreferredWidth(DialpadBackspaceHeight * unit);
    mBackspace->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    setLayout(layout);

    HbFontSpec editFont(HbFontSpec::Primary);
    // 85% of input field height
    editFont.setTextHeight(mHeight*0.85);
    mNumberEditor->setFontSpec(editFont);
}

DialpadInputField::~DialpadInputField()
{
    delete mFunctionButtonStyle;
}

HbLineEdit& DialpadInputField::editor() const
{
    return *mNumberEditor;
}

DialpadButton& DialpadInputField::backspaceButton() const
{
    return *mBackspace;
}

void DialpadInputField::onEditorContentChanged()
{
    mBackspace->setEnabled(mNumberEditor->text().length());
}

QSizeF DialpadInputField::sizeHint(
    Qt::SizeHint which,
    const QSizeF & constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    // input field height is fixed
    QSizeF sh;
    sh.setHeight(mHeight);
    return sh;
}
