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

class CommandTextEdit;

/** @addtogroup ui_vc_widgets
 * @{
 */

#define KXMLQLCVCPROGRAMMER "Programmer"

class VCProgrammer : public VCWidget, public DMXSource
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
public:
    /** @reimp */
    void writeDMX(MasterTimer* timer, QList<Universe*> universes);

public slots:
    void slotUniverseWritten(quint32 idx, const QByteArray& universeData);

private:
    bool m_inputValueChanged;

    /** Map used to lookup a GenericFader instance for a Universe ID */
    QMap<quint32, QSharedPointer<GenericFader> > m_fadersMap;

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
public:
    /** @reimp */
    bool loadXML(QXmlStreamReader &root);

    /** @reimp */
    bool saveXML(QXmlStreamWriter *doc);
private:
	QVBoxLayout* m_mainVbox=nullptr;
	CommandTextEdit* m_commandTextEdit=nullptr;
};

/** @} */

#endif
