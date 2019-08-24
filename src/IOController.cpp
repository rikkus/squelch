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

#include <qfileinfo.h>
#include <qdir.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qstring.h>
#include <qapplication.h>
#include <qtimer.h>

#include "Defines.h"
#include "Track.h"
#include "IOController.h"
#include "PlayList.h"
#include "TimeChangeEvent.h"
#include "TrackLengthChangeEvent.h"
#include "TrackChangeEvent.h"
#include "BitRateChangeEvent.h"
#include "StatusChangeEvent.h"
#include "InfoEvent.h"
#include "DeviceOpenInProgressEvent.h"
#include "DeviceOpenCompleteEvent.h"
#include "BufferItem.h"
#include "Input.h"
#include "Output.h"

#include "VorbisInput.h"

IOController * IOController::instance_ = 0L;

IOController::IOController()
	: QObject				(0, "Squelch IOController"),
		outputStatus_ (Init)
{
	connect
		(
		 PlayList::instance(),
		 SIGNAL(changed()),
		 this,
		 SLOT(slotPlayListChanged())
		);

	input_	= new VorbisInput(this);
	output_ = new Output(this);

	input_	->start();
	output_ ->start();
}

IOController::~IOController()
{
	delete input_;
	delete output_;
}

	void
IOController::slotPlay()
{
	switch (outputStatus_)
	{
		case Playing:
			output_->pause(true);
			break;

		case Paused:
			output_->pause(false);
			break;

		case NoData:
		default:
			input_->play();
			output_->pause(false);
			break;
	}
}

	void
IOController::slotStop()
{
	input_->stop();
//	output_->flush();
}

void IOController::slotSkipToTrack(const QString & id)
{
//	output_->flush();
	input_->skipToTrack(id);
}

	void
IOController::slotNext()
{
//	output_->flush();
	input_->next();
}

	void
IOController::slotPrevious()
{
//	output_->flush();
	input_->previous();
}

	void
IOController::slotRestart()
{
//	output_->flush();
	input_->seek(0L);
}

	void
IOController::slotSeek(int secs)
{
//	output_->flush();
	input_->seek(long(secs));
}

	void
IOController::outputDriverChanged()
{
	output_->reopen();
}

	bool
IOController::event(QEvent * e)
{
	bool retval = true;

	switch (e->type())
	{
		case TimeChangeEventT:
			emit(timeChange(static_cast<TimeChangeEvent *>(e)->time()));
			retval = true;
			break;

		case TrackLengthChangeEventT:
			emit(trackLengthChange(
				static_cast<TrackLengthChangeEvent *>(e)->trackLength())
			);
			retval = true;
			break;

		case TrackChangeEventT:
			emit(trackChange(static_cast<TrackChangeEvent *>(e)->trackID()));
			retval = true;
			break;

		case BitRateChangeEventT:
			emit(bitRateChange(static_cast<BitRateChangeEvent *>(e)->bitRate()));
			retval = true;
			break;

		case StatusChangeEventT:
			outputStatus_ = static_cast<StatusChangeEvent *>(e)->status();
			emit(statusChange(outputStatus_));
			retval = true;
			break;

		case InfoEventT:
			emit(newTrack(static_cast<InfoEvent *>(e)->track()));
			retval = true;
			break;

		case DeviceOpenInProgressEventT:
			emit(deviceOpenInProgress());
			retval = true;
			break;

		case DeviceOpenCompleteEventT:
			emit(deviceOpenComplete(static_cast<DeviceOpenCompleteEvent *>(e)->ok()));
			retval = true;
			break;


		default:
			retval = QObject::event(e);
			break;
	}

	return retval;
}

	QStringList
IOController::outputDriverList() const
{
	return output_->driverList();
}

	uint
IOController::outputLatency() const
{
	return output_->latency();
}

	void
IOController::slotPlayListChanged()
{
	if (0 == PlayList::instance()->filtered().count())
		input_->stop();
}

#include "IOController.moc"

// vim:ts=2:sw=2:tw=78:noet
