/*
  Copyright (C) 2001 Rik Hemsley (rikkus) <rik@kde.org>
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to
  deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <unistd.h>

#include <qlcdnumber.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qfontmetrics.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qdial.h>
#include <qfontdialog.h>
#include <qpopupmenu.h>
#include <qdragobject.h>
#include <qmessagebox.h>
#include <qaccel.h>
#include <qtooltip.h>

#include "Defines.h"
#include "Static.h"
#include "RemoteControl.h"
#include "PlayListEditWidget.h"
#include "ScrollingLabel.h"
#include "Seeker.h"
#include "TimeDisplay.h"
#include "PlayList.h"
#include "IOController.h"
#include "BufferItemPool.h"
#include "OptionsDialog.h"

#include "netwm.h"
#undef Bool

RemoteControl::RemoteControl(QWidget * parent)
	: QWidget
		(
			parent,
			"Squelch remote control",
			WDestructiveClose | WGroupLeader
		),
		outputStatus_(Init),
		playListEditor_(0),
		configDialog_(0)
{
	NETWinInfo nwi
		(
		 qt_xdisplay(),
		 winId(),
		 qt_xrootwin(),
		 NET::WMName | NET::WMWindowType | NET::WMIcon | NET::WMPid
		);

	nwi.setWindowType(NET::Normal);
	nwi.setName("squelch");
	nwi.setPid(getpid());

	QImage largeIcon = Static::instance()
		->pixmap("squelch_large").convertToImage().convertDepth(32);

	QImage smallIcon = Static::instance()
		->pixmap("squelch").convertToImage().convertDepth(32);

	NETIcon lni;
	NETIcon sni;

	lni.size.width	= largeIcon.size().width();
	lni.size.height	= largeIcon.size().height();
	lni.data				= static_cast<unsigned char *>(largeIcon.bits());

	sni.size.width	= smallIcon.size().width();
	sni.size.height	= smallIcon.size().height();
	sni.data				= static_cast<unsigned char *>(smallIcon.bits());

	nwi.setIcon(lni, true);
	nwi.setIcon(sni, false);

	// ------------------------------------------------------------------------

	trackNameLabel_	= new ScrollingLabel("Loading...", this);
	seeker_					= new Seeker(this);
	timeDisplay_		= new TimeDisplay(this);

	play_ = new QToolButton(this);
	rstt_ = new QToolButton(this);
	next_ = new QToolButton(this);
	prev_ = new QToolButton(this);
	stop_ = new QToolButton(this);

	next_->setAutoRepeat(true);
	prev_->setAutoRepeat(true);

	play_->setPixmap(Static::instance()->pixmap("play1"));
	rstt_->setPixmap(Static::instance()->pixmap("restart"));
	next_->setPixmap(Static::instance()->pixmap("next"));
	prev_->setPixmap(Static::instance()->pixmap("previous"));
	stop_->setPixmap(Static::instance()->pixmap("stop"));

	conf_ = new QToolButton(this);
	conf_->setPixmap(Static::instance()->pixmap("config"));
	conf_->setToggleButton(true);

	list_ = new QToolButton(this);
	list_->setToggleButton(true);
	list_->setPixmap(Static::instance()->pixmap("playlist"));

	playListEditor_ = new PlayListEditWidget(this);
	configDialog_ = new OptionsDialog(this);

	QToolTip::add(play_, "key: space");
	QToolTip::add(rstt_, "key: return");
	QToolTip::add(next_, "key: l");
	QToolTip::add(prev_, "key: h");
	QToolTip::add(stop_, "key: escape");

	// ------------------------------------------------------------------------

	QVBoxLayout * layout = new QVBoxLayout(this);

	layout->addWidget(trackNameLabel_);

	QHBoxLayout * timeLayout = new QHBoxLayout(layout);

	timeLayout->addSpacing(2);
	timeLayout->addWidget(seeker_);
	timeLayout->addSpacing(2);
	timeLayout->addWidget(timeDisplay_);
	timeLayout->addSpacing(2);

	layout->addSpacing(2);

	QHBoxLayout * buttonLayout = new QHBoxLayout(layout);
	buttonLayout->addWidget(list_);
	buttonLayout->addWidget(conf_);
	buttonLayout->addWidget(stop_);
	buttonLayout->addWidget(prev_);
	buttonLayout->addWidget(next_);
	buttonLayout->addWidget(rstt_);
	buttonLayout->addWidget(play_);

	buttonLayout->setStretchFactor(list_, 0);
	buttonLayout->setStretchFactor(conf_, 0);
	buttonLayout->setStretchFactor(stop_, 0);
	buttonLayout->setStretchFactor(prev_, 0);
	buttonLayout->setStretchFactor(next_, 0);
	buttonLayout->setStretchFactor(rstt_, 0);
	buttonLayout->setStretchFactor(play_, 1);

	seeker_->setFocusProxy(list_);

	QWidget::setTabOrder(list_, conf_);
	QWidget::setTabOrder(conf_, stop_);
	QWidget::setTabOrder(stop_, prev_);
	QWidget::setTabOrder(prev_, next_);
	QWidget::setTabOrder(next_, rstt_);
	QWidget::setTabOrder(rstt_, play_);
	QWidget::setTabOrder(play_, list_);

	// ------------------------------------------------------------------------

	QAccel * a = new QAccel(this, "Accelerators");

	int accelPlay	= a->insertItem(Key_Space);
	int accelNext	=	a->insertItem(Key_H);
	int accelPrev	= a->insertItem(Key_L);
	int accelStop = a->insertItem(Key_Escape);
	int accelRstt = a->insertItem(Key_Return);
	int accelQuit = a->insertItem(Key_Q);

	a->connectItem(accelPlay, IOController::instance(), SLOT(slotPlay()));
	a->connectItem(accelNext, IOController::instance(), SLOT(slotNext()));
	a->connectItem(accelPrev, IOController::instance(), SLOT(slotPrevious()));
	a->connectItem(accelStop, IOController::instance(), SLOT(slotStop()));
	a->connectItem(accelRstt, IOController::instance(), SLOT(slotRestart()));

	// ------------------------------------------------------------------------

	connect
		(
		list_,											SIGNAL(clicked()),
		this,												SLOT(slotPlayListEditorToggle())
		);

	connect
		(
		conf_,											SIGNAL(clicked()),
		this,												SLOT(slotConfigDialogToggle())
		);

	connect
		(
		play_,											SIGNAL(clicked()),
		IOController::instance(),		SLOT(slotPlay())
		);

	connect
		(
		next_,											SIGNAL(clicked()),
		IOController::instance(),		SLOT(slotNext())
		);

	connect
		(
		prev_,											SIGNAL(clicked()),
		IOController::instance(),		SLOT(slotPrevious())
		);

	connect
		(
		rstt_,											SIGNAL(clicked()),
		IOController::instance(),		SLOT(slotRestart())
		);

	connect
		(
		stop_,											SIGNAL(clicked()),
		IOController::instance(),		SLOT(slotStop())
		);

	connect
		(
		IOController::instance(),		SIGNAL(trackChange(const QString &)),
		this,												SLOT(slotTrackChange(const QString &))
		);

	connect
		(
		IOController::instance(),		SIGNAL(deviceOpenInProgress()),
		this,												SLOT(slotDeviceOpenInProgress())
		);

	connect
		(
		IOController::instance(),		SIGNAL(deviceOpenComplete(bool)),
		this,												SLOT(slotDeviceOpenComplete(bool))
		);

	connect
		(
		IOController::instance(),		SIGNAL(statusChange(OutputStatus)),
		this,												SLOT(slotStatusChange(OutputStatus))
		);

	connect
		(
		 playListEditor_,						SIGNAL(closed()),
		 list_,											SLOT(toggle())
		);

	connect
		(
		 configDialog_,							SIGNAL(closed()),
		 conf_,											SLOT(toggle())
		);

	// ------------------------------------------------------------------------

	play_->setFocus();

	setCaption("squelch");

	setIcon(Static::instance()->pixmap("squelch"));

	trackNameLabel_->setText("s.q.u.e.l.c.h");

	resize(minimumSizeHint());

	show();
}

RemoteControl::~RemoteControl()
{
}

	void
RemoteControl::slotStatusChange(OutputStatus s)
{
	if (s == outputStatus_)
		return;

	outputStatus_ = s;

	switch (outputStatus_)
	{
		case Playing:

			play_->setPixmap(Static::instance()->pixmap("pause"));
			stop_->setEnabled(true);
			break;

		case NoData:

			play_->setPixmap(Static::instance()->pixmap("play1"));
			stop_->setEnabled(false);
			break;

		case Paused:

			play_->setPixmap(Static::instance()->pixmap("play1"));
			stop_->setEnabled(true);
			break;

		default:
			break;
	}
}

	void
RemoteControl::slotPlayListEditorToggle()
{
	if (playListEditor_->isHidden())
		playListEditor_->show();
	else
		playListEditor_->hide();
}

	void
RemoteControl::slotConfigDialogToggle()
{
	if (configDialog_->isHidden())
		configDialog_->show();
	else
		configDialog_->hide();
}

	void
RemoteControl::slotTrackChange(const QString & id)
{
	if (!id)
	{
		trackNameLabel_->setText("s.q.u.e.l.c.h");
		setCaption("squelch");
	}
	else
	{
		Track * t = PlayList::instance()->track(id);

		if (0 != t)
		{
			trackNameLabel_->setText(t->title() + " - " + t->artist());
			setCaption(t->title() + " - " + t->artist() + " - squelch");
		}
	}
}

	void
RemoteControl::slotVolumeChange(int vol)
{
	QString s(tr("Volume: %1"));
	trackNameLabel_->setText(s.arg(vol), 1500);
}

	void
RemoteControl::slotDeviceOpenInProgress()
{
	trackNameLabel_->setText
		(
		 "Trying to open output device...",
		 30000
		);
}

	void
RemoteControl::slotDeviceOpenComplete(bool b)
{
	if (!b)
	{
		trackNameLabel_->setText
			(
			 "Cannot open output device. Please try another.",
			 30000
			);
	}
	else
	{
		trackNameLabel_->setText
			(
			 "Output opened",
			 3000
			);
	}
}

#include "RemoteControl.moc"

// vim:ts=2:sw=2:tw=78:noet
