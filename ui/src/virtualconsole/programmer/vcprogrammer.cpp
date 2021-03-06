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
#include <memory>
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
#include "genericdmxsource.h"
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

	m_dmxSource=std::make_unique<GenericDMXSource>(m_doc);
	m_dmxSource->setOutputEnabled(true);

    ui.setupUi(this);

    setFrameStyle(KVCFrameStyleSunken);
    setType(VCWidget::XYPadWidget);
    setCaption("Programmer");
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

	ui.btnOverview->setChecked(true);
	on_btnOverview_clicked();

	ui.edtCommandInput->setCommandGuiInterface(this);

    slotModeChanged(m_doc->mode());
    setLiveEdit(m_liveEdit);

    connect(m_doc->inputOutputMap(), SIGNAL(universeWritten(quint32,QByteArray)),
            this, SLOT(slotUniverseWritten(quint32,QByteArray)));
	connect(ui.edtCommandInput, &CommandTextEdit::helpMessage, this, &VCProgrammer::commandEditHelpChanged);
}

VCProgrammer::~VCProgrammer()
{
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

void VCProgrammer::commandEditHelpChanged(const std::string& helpMessage)
{
	ui.edtCommandHelp->setText(QString::fromStdString(helpMessage));
}
void VCProgrammer::on_btnOverview_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.pageOverview);
}
void VCProgrammer::on_btnBrightness_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.pageBrightness);
}
void VCProgrammer::on_btnColour_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.pageColour);
}
void VCProgrammer::on_btnEffect_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.pageEffect);
}
void VCProgrammer::commandSetSelectedFixtures(VcProgrammerSelectedObjects&& objects)
{
	m_selectedObjects=std::move(objects);
	refreshOverview();
}
void VCProgrammer::setFixtureToValue(int fixtureUserID, QLCChannel::Preset channelType, uint8_t dmxValue)
{
	m_fixtureState[fixtureUserID].m_channelValues[channelType]=dmxValue;
	Fixture* f=m_doc->fixtureByUserID(fixtureUserID);
	if (!f)
		return;
	switch (channelType)
	{
		case QLCChannel::IntensityDimmer:
		{
			quint32 channel=f->channel(QLCChannel::Intensity);
			if (channel==QLCChannel::invalid())
				break;
			m_dmxSource->set(f->id(), channel, dmxValue);			
			break;
		}
		case QLCChannel::IntensityRed:
		{
			quint32 channel=f->channel(QLCChannel::Intensity, QLCChannel::Red);
			if (channel==QLCChannel::invalid())
				break;
			m_dmxSource->set(f->id(), channel, dmxValue);			
			break;
		}
		case QLCChannel::IntensityGreen:
		{
			quint32 channel=f->channel(QLCChannel::Intensity, QLCChannel::Green);
			if (channel==QLCChannel::invalid())
				break;
			m_dmxSource->set(f->id(), channel, dmxValue);			
			break;
		}
		case QLCChannel::IntensityBlue:
		{
			quint32 channel=f->channel(QLCChannel::Intensity, QLCChannel::Blue);
			if (channel==QLCChannel::invalid())
				break;
			m_dmxSource->set(f->id(), channel, dmxValue);			
			break;
		}
		case QLCChannel::IntensityWhite:
		{
			quint32 channel=f->channel(QLCChannel::Intensity, QLCChannel::White);
			if (channel==QLCChannel::invalid())
				break;
			m_dmxSource->set(f->id(), channel, dmxValue);			
			break;
		}
		case QLCChannel::IntensityAmber:
		{
			quint32 channel=f->channel(QLCChannel::Intensity, QLCChannel::Amber);
			if (channel==QLCChannel::invalid())
				break;
			m_dmxSource->set(f->id(), channel, dmxValue);			
			break;
		}
		case QLCChannel::IntensityUV:
		{
			quint32 channel=f->channel(QLCChannel::Intensity, QLCChannel::UV);
			if (channel==QLCChannel::invalid())
				break;
			m_dmxSource->set(f->id(), channel, dmxValue);			
			break;
		}
		default:
			break;
	}
}
void VCProgrammer::commandSetChannel(QLCChannel::Preset channelType, uint8_t dmxValue, uint8_t dmxValueFine)
{
	std::ignore=dmxValueFine;
	auto objects=m_selectedObjects.getCurrentObjects();
	objects.sort();
	objects.unique();
	for(int obj : objects)
	{
		setFixtureToValue(obj, channelType, dmxValue);
	}

	refreshOverview();
}
void VCProgrammer::commandClearAll()
{
	m_fixtureState.clear();
	m_dmxSource->unsetAll();
	refreshOverview();
}
void VCProgrammer::commandClearSelected()
{
	auto objects=m_selectedObjects.getCurrentObjects();
	objects.sort();
	objects.unique();
	for(int obj : objects)
	{
		m_fixtureState.erase(obj);
	}
	refreshOverview();
}
void VCProgrammer::commandRotateSelection()
{
	m_selectedObjects.stepRight();
	refreshOverview();
}
QString getChannelTypeShortName(QLCChannel::Preset channelType)
{
	switch (channelType)
	{
		case QLCChannel::IntensityDimmer:
			return QString("I");
		case QLCChannel::IntensityRed:
			return QString("R");
		case QLCChannel::IntensityGreen:
			return QString("G");
		case QLCChannel::IntensityBlue:
			return QString("B");
		case QLCChannel::IntensityWhite:
			return QString("W");
		case QLCChannel::IntensityAmber:
			return QString("A");
		case QLCChannel::IntensityUV:
			return QString("UV");
		default:
			return QString("?");
        /*Custom = 0,
        IntensityMasterDimmer,
        IntensityMasterDimmerFine,
        IntensityDimmerFine,
        IntensityRedFine,
        IntensityGreenFine,
        IntensityBlueFine,
        IntensityCyan,
        IntensityCyanFine,
        IntensityMagenta,
        IntensityMagentaFine,
        IntensityYellow,
        IntensityYellowFine,
        IntensityAmberFine,
        IntensityWhiteFine,
        IntensityUVFine,
        IntensityIndigo,
        IntensityIndigoFine,
        IntensityLime,
        IntensityLimeFine,
        IntensityHue,
        IntensityHueFine,
        IntensitySaturation,
        IntensitySaturationFine,
        IntensityLightness,
        IntensityLightnessFine,
        IntensityValue,
        IntensityValueFine,
        PositionPan,
        PositionPanFine,
        PositionTilt,
        PositionTiltFine,
        PositionXAxis,
        PositionYAxis,
        SpeedPanSlowFast,
        SpeedPanFastSlow,
        SpeedTiltSlowFast,
        SpeedTiltFastSlow,
        SpeedPanTiltSlowFast,
        SpeedPanTiltFastSlow,
        ColorMacro,
        ColorWheel,
        ColorWheelFine,
        ColorRGBMixer,
        ColorCTOMixer,
        ColorCTCMixer,
        ColorCTBMixer,
        GoboWheel,
        GoboWheelFine,
        GoboIndex,
        GoboIndexFine,
        ShutterStrobeSlowFast,
        ShutterStrobeFastSlow,
        ShutterIrisMinToMax,
        ShutterIrisMaxToMin,
        ShutterIrisFine,
        BeamFocusNearFar,
        BeamFocusFarNear,
        BeamFocusFine,
        BeamZoomSmallBig,
        BeamZoomBigSmall,
        BeamZoomFine,
        PrismRotationSlowFast,
        PrismRotationFastSlow,
        NoFunction,
        LastPreset // dummy for cycles*/

	}
}
void VCProgrammer::refreshOverview()
{
	QString infoString;
	auto objects=m_selectedObjects.getCurrentObjects();
	objects.sort();
	objects.unique();
	auto selectIt=objects.begin();
	auto stateIt=m_fixtureState.begin();
	while (selectIt!=objects.end() || stateIt!=m_fixtureState.end())
	{
		bool activeSelect=false;
		bool activeState=false;
		int fixtureUserId=0;

		if (selectIt==objects.end())
		{
			activeState=true;
			fixtureUserId=stateIt->first;
		}
		else if (stateIt==m_fixtureState.end())
		{
			activeSelect=true;
			fixtureUserId=*selectIt;
		}
		else if (*selectIt<stateIt->first)
		{
			activeSelect=true;
			fixtureUserId=*selectIt;
		}
		else if (*selectIt>stateIt->first)
		{
			activeState=true;
			fixtureUserId=stateIt->first;
		}
		else if (*selectIt==stateIt->first)
		{
			activeSelect=true;
			activeState=true;
			fixtureUserId=*selectIt;
		}
		else
		{
			//Error - should not occure because all other cases are not possible
			continue;
		}
		if (m_doc->fixtureByUserID(fixtureUserId))
		{

			if (activeSelect)
				infoString.append(QString("<b>"));
			infoString.append(QString("%1").arg(fixtureUserId));
			if (activeState)
			{
				infoString.append(QString(": "));
				bool firstEntry=true;
				for(const auto& channelValue : stateIt->second.m_channelValues)
				{
					if (!firstEntry)
					{
						infoString.append(QString(", "));
					}
					else
					{
						firstEntry=false;
					}
					infoString.append(QString("%1:%2").arg(getChannelTypeShortName(channelValue.first)).arg(channelValue.second*100/255));
				}
			}
			if (activeSelect)
				infoString.append(QString("</b>"));
			infoString.append(QString("<br>"));
		}

		if (activeSelect)
			++selectIt;
		if (activeState)
			++stateIt;
	}
	ui.edtOverview->setText(infoString);
}
