/*
  Q Light Controller Plus
  vcxypad.h

  Copyright (c) Stefan Krumm
                Heikki Junnila
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

#ifndef VCPROGRAMMER_H
#define VCPROGRAMMER_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QMutex>
#include <QList>

#include "vcxypadfixture.h"
#include "vcxypadpreset.h"
#include "dmxsource.h"
#include "vcwidget.h"
#include "commandline/command/icommandgui.h"
#include "ui_vcprogrammer.h"

#include <map>

class QXmlStreamReader;
class QXmlStreamWriter;
class ctkRangeSlider;
class QPaintEvent;
class QMouseEvent;
class MasterTimer;
class VCProgrammerArea;
class QHBoxLayout;
class QVBoxLayout;
class FlowLayout;
class QByteArray;
class QSlider;
class EFX;
class Doc;
class GenericDMXSource;

class CommandTextEdit;

/** @addtogroup ui_vc_widgets
 * @{
 */

#define KXMLQLCVCPROGRAMMER "Programmer"

class VCProgrammer : public VCWidget, public Command::ICommandGui
{
    Q_OBJECT
    Q_DISABLE_COPY(VCProgrammer)

    /*************************************************************************
     * Initialization
     *************************************************************************/
public:
    VCProgrammer(QWidget* parent, Doc* doc);
    virtual ~VCProgrammer();

    /** @reimp */
    void enableWidgetUI(bool enable);

    /*************************************************************************
     * Clipboard
     *************************************************************************/
public:
    /** @reimp */
    VCWidget* createCopy(VCWidget* parent);

    /** @reimp */
    bool copyFrom(const VCWidget* widget);

    /*************************************************************************
     * Caption
     *************************************************************************/
public:
    /** @reimp */
    void setCaption(const QString& text);

    /*************************************************************************
     * Properties
     *************************************************************************/
public:
    /** @reimp */
    void editProperties();

    /*************************************************************************
     * Current position
     *************************************************************************/
public slots:
    void slotUniverseWritten(quint32 idx, const QByteArray& universeData);

    /*********************************************************************
     * External input
     *********************************************************************/
public:
    void updateFeedback();

protected slots:
    /** Called when an external input device produces input data */
    void slotInputValueChanged(quint32 universe, quint32 channel, uchar value);
    void slotKeyPressed(const QKeySequence& keySequence);

    /*************************************************************************
     * QLC+ mode
     *************************************************************************/
protected slots:
    /** @reimp */
    void slotModeChanged(Doc::Mode mode);

    /*************************************************************************
     * Load & Save
     *************************************************************************/
private slots:
	void commandEditHelpChanged(const std::string& helpMessage);
public slots:
	void on_btnOverview_clicked();
	void on_btnBrightness_clicked();
	void on_btnColour_clicked();
	void on_btnEffect_clicked();
public:
    /** @reimp */
    bool loadXML(QXmlStreamReader &root);

    /** @reimp */
    bool saveXML(QXmlStreamWriter *doc);
private:
	QVBoxLayout* m_mainVbox=nullptr;
	CommandTextEdit* m_commandTextEdit=nullptr;
	Ui_VCProgrammerForm ui;
	void refreshOverview();
	/** Engine */
	virtual void commandSetSelectedFixtures(VcProgrammerSelectedObjects&& objects) override;
	virtual void commandSetChannel(QLCChannel::Preset channelType, uint8_t dmxValue, uint8_t dmxValueFine=0) override;
	virtual void commandClearAll() override;
	virtual void commandClearSelected() override;
	virtual void commandRotateSelection() override;
	void setFixtureToValue(int fixtureUserID, QLCChannel::Preset channelType, uint8_t dmxValue);
	VcProgrammerSelectedObjects m_selectedObjects;
	struct FixtureData
	{
		std::map<QLCChannel::Preset, uint8_t> m_channelValues;
	};
	std::map<int, FixtureData> m_fixtureState;
	std::unique_ptr<GenericDMXSource> m_dmxSource;
};

/** @} */

#endif