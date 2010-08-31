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
* Description: Dialpad keypad
*
*/

#include <QLocale>
#include <QSignalMapper>

#include <hbinstance.h>
#include <hbinputkeymapfactory.h>
#include <hbinputkeymap.h>
#include <hbinpututils.h>
#include <hbinputsettingproxy.h>
#include <hbinputlanguage.h>
#include <hbapplication.h>
#include <hbcolorscheme.h>
#include <hblineedit.h>
#include <hbfontspec.h>
#include <hbevent.h>

#include "dialpadnumericbutton.h"
#include "dialpadkeypad.h"
#include "dialpadbutton.h"
#include "dialpadinputfield.h"

static const int DialpadRowCount = 4;
static const int DialpadColumnCount = 3;
static const QLatin1String handsetIcon("qtg_mono_call");
static const QLatin1String vmbxIcon("qtg_mono_voice_mailbox");
// layout values in units
static const qreal DialpadPrimaryTextSize = 5.5;
static const qreal DialpadSecondaryTextSize = 4.0;
static const qreal DialpadIconSize = 4.0;
static const qreal DialpadPrimaryTextLeftMargin  = 1.5;
static const qreal DialpadPrimarySecondaryMargin  = 0.75;

static const int DialpadKeyCodeTable[DialpadRowCount*DialpadColumnCount] =
{
    Qt::Key_1,        Qt::Key_2,      Qt::Key_3,
    Qt::Key_4,        Qt::Key_5,      Qt::Key_6,
    Qt::Key_7,        Qt::Key_8,      Qt::Key_9,
    Qt::Key_Asterisk, Qt::Key_0,      Qt::Key_NumberSign
    // Qt::Key_Yes and Qt::Key_BackSpace are handled separately
};

DialpadKeypad::DialpadKeypad(
    const HbMainWindow& mainWindow,
    DialpadInputField& inputField,
    QGraphicsItem* parent) :
    HbInputButtonGroup(parent),
    mMainWindow(mainWindow),
    mInputField(inputField),
    mMaxPrimaryLineWidth(0)
{
    setObjectName("keypad");

    // create clicked signal mapper
    mKeyClickedSignalMapper = new QSignalMapper(this);
    connect(mKeyClickedSignalMapper,SIGNAL(mapped(int)),
            SLOT(handleKeyClicked(int)));

    // connect backspace signals
    connect(&mInputField.backspaceButton(),SIGNAL(clicked()),
            mKeyClickedSignalMapper,SLOT(map()));
    mKeyClickedSignalMapper->setMapping(&mInputField.backspaceButton(),
                                        Qt::Key_Backspace);

    // create keypad
    setGridSize(QSize(DialpadColumnCount, DialpadRowCount));
    setButtonBorderSize(0);    

    QList<HbInputButton*> buttons;

    for (int i = 0; i < DialpadRowCount * DialpadColumnCount; ++i) {
        DialpadNumericButton *item = new DialpadNumericButton(
            DialpadKeyCodeTable[i],
            QPoint(i % DialpadColumnCount, i / DialpadColumnCount));
        buttons.append(item);

        item->setType(HbInputButton::ButtonTypeNormal);
    }

    setButtons(buttons);

    // connect keypad signals
    QObject::connect(this, SIGNAL(buttonPressed(const QKeyEvent&)),
                     this, SLOT(sendKeyPressEvent(const QKeyEvent&)));
    QObject::connect(this, SIGNAL(buttonReleased(const QKeyEvent&)),
                     this, SLOT(sendKeyReleaseEvent(const QKeyEvent&)));
    QObject::connect(this, SIGNAL(buttonLongPressed(const QKeyEvent&)),
                     this, SLOT(sendLongPressEvent(const QKeyEvent&)));
    QObject::connect(this, SIGNAL(pressedButtonChanged(const QKeyEvent&,
                                                       const QKeyEvent&)),
                     this, SLOT(handleKeyChangeEvent(const QKeyEvent&,
                                                     const QKeyEvent&)));

    // create call button (parent layouts this)
    mCallButton = new DialpadButton(parent);
    mCallButton->setButtonType(DialpadButton::CallButton);
    mCallButton->setIcon(HbIcon(handsetIcon));
    QString buttonName;
    buttonName.setNum(Qt::Key_Yes);
    mCallButton->setObjectName(buttonName);
    connect(mCallButton,SIGNAL(clicked()),
            mKeyClickedSignalMapper,SLOT(map()));
    connect(mCallButton,SIGNAL(longPress(QPointF)),
            mKeyClickedSignalMapper,SLOT(map()));
    mKeyClickedSignalMapper->setMapping(mCallButton,
                                        Qt::Key_Yes);

    // set button texts
    setButtonTexts();
    // set button icons
    button(0)->setIcon(HbIcon(vmbxIcon));

    // update button texts when input language is changed
    connect(HbInputSettingProxy::instance(),
            SIGNAL(globalInputLanguageChanged(HbInputLanguage)),
            this,SLOT(setButtonTexts()));

    updateColorArray();

    mUnit = HbDeviceProfile::profile(this).unitValue();
}

DialpadKeypad::~DialpadKeypad()
{
}

void DialpadKeypad::setButtonTexts()
{
    HbInputLanguage inputLanguage =
        HbInputSettingProxy::instance()->globalInputLanguage();
    const HbKeymap *keymap =
        HbKeymapFactory::instance()->keymap(inputLanguage.language());

    mGeneratedChar.clear();

    if (keymap) {
        int buttonCount = (DialpadRowCount*DialpadColumnCount);
        for (int i = 0; i < buttonCount; i++) {
            int keyCode = DialpadKeyCodeTable[i];

            if (keyCode == Qt::Key_Asterisk) {
                // asterisk is not localized
                QChar asterisk('*');
                button(i)->setText(asterisk);
                button(i)->setSecondaryText(QLatin1String("+"));
                mGeneratedChar.insert(Qt::Key_Asterisk, asterisk);
                continue;
            }

            if (keyCode == Qt::Key_NumberSign) {
                // number sign is not localized
                QChar numberSign('#');
                button(i)->setText(numberSign);
                mGeneratedChar.insert(Qt::Key_NumberSign, numberSign);
                continue;
            }

            int index = i;
            if (keyCode==Qt::Key_0) {
                index = i-1;
            }

            const HbMappedKey *key =
                keymap->keyForIndex(HbKeyboardVirtual12Key, index);

            if (key) {
                QChar numberChar =
                    HbInputUtils::findFirstNumberCharacterBoundToKey(
                        key,
                        inputLanguage.language(),
                        HbInputUtils::inputDigitType(inputLanguage.language()));

                // button text
                button(i)->setText(numberChar);
                mGeneratedChar.insert(keyCode,numberChar);

                // additional text (letters)
                int numberOfCharacters;
                if (keyCode==Qt::Key_7 || keyCode == Qt::Key_9) {
                    numberOfCharacters = 4;
                } else if (keyCode==Qt::Key_0||keyCode==Qt::Key_1) {
                    numberOfCharacters = 0;
                } else {
                    numberOfCharacters = 3;
                }

                QString characters = key->characters(HbModifierNone);

                if (numberOfCharacters!=0 && keyCode!=Qt::Key_1) {
                    button(i)->setSecondaryText(characters.left(numberOfCharacters));
                }
            }
        }
    }
}

void DialpadKeypad::handleKeyClicked(int key)
{
    // concerns only yes and backspace keys
    postKeyEvent(QEvent::KeyPress, key);
    postKeyEvent(QEvent::KeyRelease, key);
}

void DialpadKeypad::postKeyEvent(QEvent::Type type, int key)
{
    // send simulated key to application
    QKeyEvent *keyEvent = new QKeyEvent(type, key, Qt::NoModifier);
    HbApplication::postEvent(const_cast<HbMainWindow*>(&mMainWindow),keyEvent);
}

void DialpadKeypad::sendKeyEventToEditor(QEvent::Type type, int key)
{
    // send key event to editor
    QKeyEvent keyEvent(type, key, Qt::NoModifier, mGeneratedChar.value(key));
    HbApplication::sendEvent(&mInputField.editor(), &keyEvent);
}

void DialpadKeypad::sendKeyPressEvent(const QKeyEvent& event)
{
    updateButtonLabels();
    mPressedNumericKey = event.key();
    postKeyEvent(QEvent::KeyPress, event.key());
}

void DialpadKeypad::sendKeyReleaseEvent(const QKeyEvent& event)
{
    updateButtonLabels();

    if (mPressedNumericKey) {
        // short press, update editor here
        sendKeyEventToEditor(QEvent::KeyPress, event.key());
    }

    postKeyEvent(QEvent::KeyRelease, event.key());    
}

void DialpadKeypad::sendLongPressEvent(const QKeyEvent& event)
{
    sendKeyEventToEditor(QEvent::KeyPress, event.key());
    mPressedNumericKey = 0;
}

void DialpadKeypad::handleKeyChangeEvent(
    const QKeyEvent& releaseEvent,
    const QKeyEvent& pressEvent)
{
    Q_UNUSED(pressEvent)

    postKeyEvent(QEvent::KeyRelease, releaseEvent.key());
    cancelButtonPress();
}

void DialpadKeypad::setCallButtonEnabled(bool enabled)
{
    mCallButton->setEnabled(enabled);
}

void DialpadKeypad::resetButtons()
{
    cancelButtonPress();
    mCallButton->setDown(false);
}

DialpadButton& DialpadKeypad::callButton() const
{
    return *mCallButton;
}

DialpadNumericButton* DialpadKeypad::button(int i) const
{
    return static_cast<DialpadNumericButton*>(HbInputButtonGroup::button(i));
}

void DialpadKeypad::updateButtonLabels()
{
    // update numeric buttons according to button state (pressed/released)
    updateIconColor();
    updateTextLayouts(rect().size());
}

void DialpadKeypad::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Paints empty buttons
    HbInputButtonGroup::paint(painter,option,widget);

    qreal cellWidth = boundingRect().width() / gridSize().width();
    qreal cellHeight = boundingRect().height() / gridSize().height();

    // Draw icons
    for (int i = 0; i < DialpadRowCount * DialpadColumnCount; i++) {
        DialpadNumericButton *item = button(i);

        if (!item->icon().isNull()) {
            // icon is centered to button
            qreal x = (item->position().x() * cellWidth) + (cellWidth / 2) -
                      ((DialpadIconSize * mUnit) / 2);
            qreal y = (item->position().y() * cellHeight) +  (cellHeight / 2) -
                      ((DialpadIconSize * mUnit) / 2);

            qreal width = DialpadIconSize * mUnit;
            qreal height = DialpadIconSize * mUnit;

            Qt::Alignment alignment =
                static_cast<Qt::Alignment>(Qt::AlignVCenter | Qt::AlignHCenter);
            item->icon().paint(painter,
                               QRectF(x,y,width,height),
                               Qt::KeepAspectRatio,
                               alignment);
        }                      
    }

    // Draw texts
    QPen origPen = painter->pen();
    for (int i = 0; i < mTextLayouts.count(); ++i) {
        if (i==SecondaryText) {
            // dimmed in normal state
            painter->setPen(mColors.at(Pressed+1));
        } else {
            // otherwise normal or pressed color
            painter->setPen(mColors.at(i/TextTypeCount));
        }
        mTextLayouts.at(i)->draw(painter, QPointF(0, 0));
    }
    painter->setPen(origPen);
}

void DialpadKeypad::updateColorArray()
{
    mColors.clear();

    QColor normalColor = HbColorScheme::color("qtc_input_button_normal");
    mColors.insert(Normal, normalColor);

    QColor pressedColor = HbColorScheme::color("qtc_input_button_pressed");
    mColors.insert(Pressed, pressedColor);

    // this is used for alphabets shown dimmed, use alpha until exact color
    // is specified
    QColor disabledColor = HbColorScheme::color("qtc_input_button_normal");
    disabledColor.setAlpha(128);
    mColors.insert(Pressed+1, disabledColor);
}

void DialpadKeypad::updateIconColor()
{
    for (int i = 0; i < (DialpadRowCount * DialpadColumnCount); i++) {
        DialpadNumericButton *item = button(i);

        if (item->state()==HbInputButton::ButtonStatePressed) {
            item->icon().setColor(mColors.at(Pressed));
        } else {
            item->icon().setColor(mColors.at(Normal));
        }
    }
}

void DialpadKeypad::cancelButtonPress()
{
    HbInputButtonGroup::cancelButtonPress();
    updateButtonLabels();
}

void DialpadKeypad::setGeometry(const QRectF &rect)
{
    HbInputButtonGroup::setGeometry(rect);
    updateTextLayouts(rect.size());
}

void DialpadKeypad::changeEvent(QEvent *event)
{
    HbInputButtonGroup::changeEvent(event);

    if (event->type() == HbEvent::ThemeChanged) {
        updateColorArray();
        updateIconColor();
    }
}

void DialpadKeypad::updateTextLayouts(const QSizeF &size)
{
    if (!size.width() && !size.height()) {
        return;
    }

    // get normal and pressed state texts
    QList<QString> textContent;
    resolveTextContent(textContent);

    // layout the texts
    createTextLayouts(size, textContent);
}

void DialpadKeypad::resolveTextContent(QList<QString> &content)
{
    QString normalState;
    QString normalStateSecondary;
    QString pressedState;
    QString pressedStateSecondary;

    for (int i = 0; i < (DialpadRowCount*DialpadColumnCount); i++) {
        DialpadNumericButton *item = button(i);
        if (item->state()==HbInputButton::ButtonStatePressed) {
            if (item->text().length()) {
                pressedState.append(item->text());
                pressedState.append(QChar(QChar::LineSeparator));
            }

            if (item->secondaryText().length()) {
                pressedStateSecondary.append(item->secondaryText());
                pressedStateSecondary.append(QChar(QChar::LineSeparator));
            }
        } else { // ButtonStateNormal
            if (item->text().length()) {
                normalState.append(item->text());
                normalState.append(QChar(QChar::LineSeparator));
            }

            if (item->secondaryText().length()) {
                normalStateSecondary.append(item->secondaryText());
                normalStateSecondary.append(QChar(QChar::LineSeparator));
            }
        }
    }

    content.insert(PrimaryText, normalState);
    content.insert(SecondaryText, normalStateSecondary);
    content.insert(TextTypeCount + Pressed, pressedState);
    content.insert(StateCount + SecondaryText, pressedStateSecondary);
}

void DialpadKeypad::createTextLayouts(
    const QSizeF &size, const QList<QString> &content)
{
    // clear old layouts
    qDeleteAll(mTextLayouts);
    mTextLayouts.clear();

    if (content.count()==2) {
        // line width is measured only when all buttons are in normal state
        mMaxPrimaryLineWidth = 0;
    }

    QFont primaryfFont = HbFontSpec(HbFontSpec::Primary).font();
    primaryfFont.setPixelSize(DialpadPrimaryTextSize * mUnit);

    QFont secondaryFont = HbFontSpec(HbFontSpec::Secondary).font();
    secondaryFont.setPixelSize(DialpadSecondaryTextSize * mUnit);

    for (int i=0; i < (StateCount*TextTypeCount); i++ ) {
        QString text = content.at(i);

        if (!text.isNull()) {
            QTextLayout* textLayout;
            int type;

            if (i%TextTypeCount) {
                textLayout = new QTextLayout(text,secondaryFont);
                type = SecondaryText;
            } else {
                textLayout = new QTextLayout(text,primaryfFont);
                type = PrimaryText;
            }

            mTextLayouts.append(textLayout);

            textLayout->beginLayout();

            int state = (i>=TextTypeCount) ? Pressed : Normal;

            layoutTextLines(size,*textLayout,state,type);

            textLayout->endLayout();

            textLayout->setCacheEnabled(true);
        }
    }
}

void DialpadKeypad::layoutTextLines(
    const QSizeF &size,
    QTextLayout &textLayout,
    int state,
    int type)
{
    QFontMetricsF fontMetrics(textLayout.font());
    qreal textHeight = fontMetrics.height();

    qreal cellWidth = size.width() / gridSize().width();
    qreal cellHeight = size.height() / gridSize().height();
    qreal maxLineWidth = 0;

    for (int j = 0; j < (DialpadRowCount*DialpadColumnCount); j++) {
        DialpadNumericButton *item = button(j);

        if ((type==PrimaryText && item->text().isNull()) ||
            (type==SecondaryText && item->secondaryText().isNull())) {
            continue; // no text for this button -> next button
        }

        if ( ( state==Normal &&
               item->state()==HbInputButton::ButtonStateReleased ) ||
             ( state==Pressed &&
               item->state()==HbInputButton::ButtonStatePressed ) ) {

            QTextLine line = textLayout.createLine();

            qreal textPositionX = 0;
            qreal textPositionY = 0;

            if (line.isValid()) {
                line.setNumColumns(item->text().length());
                // layout text line
                if (type==SecondaryText) {
                    if (j==9) {
                        // + is centered to button
                        qreal lineWidth = fontMetrics.width(item->text());
                        textPositionX = (item->position().x() * cellWidth) +
                                        (cellWidth / 2) -
                                        (lineWidth / 2);
                        textPositionY = (item->position().y() +
                                        (0.5 * item->size().height())) *
                                        cellHeight - (0.5 * textHeight);

                    } else {
                        textPositionX = (item->position().x() * cellWidth) +
                                        (DialpadPrimaryTextLeftMargin * mUnit) +
                                        mMaxPrimaryLineWidth +
                                        (DialpadPrimarySecondaryMargin * mUnit)
                                        + buttonBorderSize();
                        textPositionY = (item->position().y() +
                                        (0.5 * item->size().height())) *
                                        cellHeight - (0.5 * textHeight);
                    }                    
                } else {
                    textPositionX = (item->position().x() * cellWidth) +
                                    (DialpadPrimaryTextLeftMargin * mUnit)
                                    + buttonBorderSize();
                    textPositionY = (item->position().y() +
                                    (0.5 * item->size().height())) *
                                    cellHeight - (0.5 * textHeight);

                    // store line width, for drawing secondary text
                    qreal lineWidth = fontMetrics.width(item->text());
                    if (mMaxPrimaryLineWidth == 0 && (j>0 && j<10) &&
                        lineWidth > maxLineWidth) {
                        maxLineWidth = lineWidth;
                    }
                }
            }

            line.setPosition(QPointF(textPositionX, textPositionY));
        }
    }

    mMaxPrimaryLineWidth = maxLineWidth;
}
