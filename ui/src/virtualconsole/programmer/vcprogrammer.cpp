/*
  Q Light Controller Plus
  vcxypad.cpp

  Copyright (c) Heikki Junnila
                Stefan Krumm
                Massimo Callegari

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
#include "vcprogrammer.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <QGridLayout>
#include <QByteArray>
#include <QSettings>
#include <QPainter>
#include <QPixmap>
#include <QCursor>
#include <QSlider>
#include <qmath.h>
#include <QDebug>
#include <QPoint>
#include <QMenu>
#include <QList>

#include "qlcmacros.h"
#include "qlcfile.h"

#include "vcpropertieseditor.h"
#include "vcxypadproperties.h"
#include "qlcinputchannel.h"
#include "virtualconsole.h"
#include "ctkrangeslider.h"
#include "mastertimer.h"
#include "vcxypadarea.h"
#include "inputpatch.h"
#include "flowlayout.h"
#include "fixture.h"
#include "apputil.h"
#include "scene.h"
#include "efx.h"
#include "doc.h"
#include "commandline/commandtextedit.h"

const qreal MAX_VALUE = 256.0;
const qreal MAX_DMX_VALUE = MAX_VALUE - 1.0/256;

static const QString presetBtnSS = "QPushButton { background-color: %1; height: 32px; border: 2px solid #6A6A6A; border-radius: 5px; }"
                                   "QPushButton:pressed { border: 2px solid #0000FF; }"
                                   "QPushButton:checked { border: 2px solid #0000FF; }"
                                   "QPushButton:unchecked { border: 2px solid #6A6A6A; }"
                                   "QPushButton:disabled { border: 2px solid #BBBBBB; color: #8f8f8f }";

/*****************************************************************************
 * VCProgrammer Initialization
 *****************************************************************************/

VCProgrammer::VCProgrammer(QWidget* parent, Doc* doc) : VCWidget(parent, doc)
{
    /* Set the class name "VCProgrammer" as the object name as well */
    setObjectName(VCProgrammer::staticMetaObject.className());

    ui.setupUi(this);

    setFrameStyle(KVCFrameStyleSunken);
    setType(VCWidget::XYPadWidget);
    setCaption("XY Pad");
    setMinimumSize(20, 20);

    QSettings settings;
    QVariant var = settings.value(SETTINGS_XYPAD_SIZE);
    if (var.isValid() == true)
	{
        resize(var.toSize());
	}
    else
	{
        resize(QSize(230, 230));
	}

	
	ui.edtCommandInput->setCommandGuiInterface(this);

    slotModeChanged(m_doc->mode());
    setLiveEdit(m_liveEdit);

    m_doc->masterTimer()->registerDMXSource(this);
    connect(m_doc->inputOutputMap(), SIGNAL(universeWritten(quint32,QByteArray)),
            this, SLOT(slotUniverseWritten(quint32,QByteArray)));
	connect(ui.edtCommandInput, SIGNAL(&CommandTextEdit::helpMessage), this, SLOT(&VCProgrammer::commandEditHelpChanged));
}

VCProgrammer::~VCProgrammer()
{
    m_doc->masterTimer()->unregisterDMXSource(this);
    foreach (QSharedPointer<GenericFader> fader, m_fadersMap.values())
    {
        if (!fader.isNull())
            fader->requestDelete();
    }
    m_fadersMap.clear();
}

void VCProgrammer::enableWidgetUI(bool enable)
{
	Q_UNUSED(enable);
}

/*****************************************************************************
 * Clipboard
 *****************************************************************************/

VCWidget* VCProgrammer::createCopy(VCWidget* parent)
{
    Q_ASSERT(parent != NULL);

    VCProgrammer* xypad = new VCProgrammer(parent, m_doc);
    if (xypad->copyFrom(this) == false)
    {
        delete xypad;
        xypad = NULL;
    }

    return xypad;
}

bool VCProgrammer::copyFrom(const VCWidget* widget)
{
    const VCProgrammer* xypad = qobject_cast <const VCProgrammer*> (widget);
    if (xypad == NULL)
        return false;
    resize(xypad->size());

    /* Copy common stuff */
    return VCWidget::copyFrom(widget);
}

/*****************************************************************************
 * Caption
 *****************************************************************************/

void VCProgrammer::setCaption(const QString& text)
{
    VCWidget::setCaption(text);
}

/*****************************************************************************
 * Properties
 *****************************************************************************/

void VCProgrammer::editProperties()
{
}

/*****************************************************************************
 * Current XY position
 *****************************************************************************/

void VCProgrammer::writeDMX(MasterTimer* timer, QList<Universe *> universes)
{
	Q_UNUSED(timer);
	Q_UNUSED(universes);
}

void VCProgrammer::slotUniverseWritten(quint32 idx, const QByteArray &universeData)
{

	Q_UNUSED(idx);
	Q_UNUSED(universeData);

}


/*********************************************************************
 * External input
 *********************************************************************/

void VCProgrammer::updateFeedback()
{
}

void VCProgrammer::slotInputValueChanged(quint32 universe, quint32 channel,
                                     uchar value)
{
	Q_UNUSED(universe);
	Q_UNUSED(channel);
	Q_UNUSED(value);
}

void VCProgrammer::slotKeyPressed(const QKeySequence &keySequence)
{
    if (acceptsInput() == false)
        return;
	Q_UNUSED(keySequence);

}

/*****************************************************************************
 * QLC mode
 *****************************************************************************/

void VCProgrammer::slotModeChanged(Doc::Mode mode)
{
    if (mode == Doc::Operate && isDisabled() == false)
    {
        enableWidgetUI(true);
    }
    else
    {
        enableWidgetUI(false);
    }

    VCWidget::slotModeChanged(mode);
}

/*****************************************************************************
 * Load & Save
 *****************************************************************************/

bool VCProgrammer::loadXML(QXmlStreamReader &root)
{
    bool visible = false;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    if (root.name() != KXMLQLCVCPROGRAMMER)
    {
        qWarning() << Q_FUNC_INFO << "XY Pad node not found";
        return false;
    }

    /* Widget commons */
    loadXMLCommon(root);

    QXmlStreamAttributes attrs = root.attributes();

    /* Children */
    while (root.readNextStartElement())
    {
        if (root.name() == KXMLQLCWindowState)
        {
            loadXMLWindowState(root, &x, &y, &w, &h, &visible);
        }
        else
        {
            qWarning() << Q_FUNC_INFO << "Unknown XY Pad tag:" << root.name().toString();
            root.skipCurrentElement();
        }
    }

    setGeometry(x, y, w, h);
    show(); // Qt doesn't update the widget's geometry without this.

    return true;
}

bool VCProgrammer::saveXML(QXmlStreamWriter *doc)
{
    Q_ASSERT(doc != NULL);

    /* VC XY Pad entry */
    doc->writeStartElement(KXMLQLCVCPROGRAMMER);

    saveXMLCommon(doc);

    /* Window state */
    saveXMLWindowState(doc);

    /* Appearance */
    saveXMLAppearance(doc);

    /* End the >XYPad> tag */
    doc->writeEndElement();

    return true;
}
