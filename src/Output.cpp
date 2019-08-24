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

#include <qtimer.h>
#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>

#include "Output.h"
#include "TimeChangeEvent.h"
#include "TrackLengthChangeEvent.h"
#include "TrackChangeEvent.h"
#include "BitRateChangeEvent.h"
#include "StatusChangeEvent.h"
#include "DeviceOpenInProgressEvent.h"
#include "DeviceOpenCompleteEvent.h"
#include "BufferItem.h"
#include "BufferItemPool.h"

static inline char max(char a, char b) { return a > b ? a : b; }

Output::Output(QObject * parent)
	: QThread			(),
		parent_			(parent),
		reopen_			(false),
		pause_			(false),
		flush_			(false),
		status_			(Init),
		time_				(-1),
		trackID_		(QString::null),
		trackLength_(-1),
		device_			(0)
{
	ao_initialize();

	int i = 0;

	ao_info * driverInfo = ao_driver_info(i++);

	while (0 != driverInfo)
	{
		driverList_ << QString::fromUtf8(driverInfo->short_name);
		driverInfo = ao_driver_info(i++);
	}
}

Output::~Output()
{
	closeDevice();
	ao_shutdown();
}

	void
Output::playEmptyBlock()
{
	static const int emptyBufLength(4096);

	static char * emptyBuf = new char[emptyBufLength];

	memset(emptyBuf, 0, emptyBufLength);

	ao_play(device_, emptyBuf, emptyBufLength);
}

	void
Output::run()
{
	int emptyCount = 0;
	int pauseCount = 0;

	int sampleRate = 44100, channels = 2;

	if (!openDevice(sampleRate, channels))
	{
		reopen_ = true;
	}

	while (true)
	{
		if (reopen_)
		{
			reopen_ = false;

			closeDevice();

			if (!openDevice(sampleRate, channels))
			{
				while (!reopen_)
					msleep(10);

				continue;
			}
		}

		if (pause_)
		{
			if (status_ != Paused)
			{
				postEvent(parent_, new StatusChangeEvent(Paused));
				status_ = Paused;
				pauseCount = 0;
			}

			playEmptyBlock();
			continue;
		}

		if (flush_)
		{
			flush_ = false;
			BufferItemPool::instance()->flush();

			continue;
		}

		if (BufferItemPool::instance()->isEmpty())
		{
			if (status_ != NoData)
			{
				postEvent(parent_, new StatusChangeEvent(NoData));
				status_ = NoData;
				emptyCount = 0;
			}

			playEmptyBlock();
			continue;
		}

		if (status_ != Playing)
		{
			postEvent(parent_, new StatusChangeEvent(Playing));
			status_ = Playing;
		}

		// Get some data from the pool.

		BufferItem * item = BufferItemPool::instance()->peek();

		if (item->channels() != channels || item->sampleRate() != sampleRate)
		{
			channels		= item->channels();
			sampleRate	= item->sampleRate();
			reopen_ = true;
			continue;
		}

		item = BufferItemPool::instance()->take();

		ao_play(device_, item->buffer(), item->length());

		int			newTime		= item->time();
		QString	newID			= item->trackID();
		int			newLength	= item->trackLength();

		if (newTime != time_)
			postEvent(parent_, new TimeChangeEvent(time_ = newTime));

		if (newID != trackID_)
			postEvent(parent_, new TrackChangeEvent(trackID_ = newID));

		if (newLength != trackLength_)
			postEvent(parent_, new TrackLengthChangeEvent(trackLength_ = newLength));
	}
}

	long
Output::time() const
{
	return time_;
}

	void
Output::reopen()
{
	reopen_ = true;
}

	void
Output::pause(bool b)
{
	pause_ = b;
}

	void
Output::flush()
{
	flush_ = true;
}

	void
Output::closeDevice()
{
	if (0 != device_)
		ao_close(device_);
}

	bool
Output::openDevice(int sampleRate, int channels)
{
	QString driverName;
	QString driverFlags;

	{
		QFile f(QDir::homeDirPath() + "/.squelch/outputdriver");

		if (!f.open(IO_ReadOnly))
			driverName = "oss";

		else
		{
			QTextStream t(&f);
			t >> driverName;
		}
	}

	{
		QFile f(QDir::homeDirPath() + "/.squelch/outputdriverflags");

		if (!f.open(IO_ReadOnly))
			driverFlags = "";

		else
		{
			QTextStream t(&f);
			t >> driverFlags;
			driverFlags = driverFlags.stripWhiteSpace();
		}
	}

	int driverID = ao_driver_id(driverName.ascii());

	if (driverID < 0)
	{
		qWarning("Can't get driver id for %s", driverName.ascii());
		return false;
	}

	ao_option * options = 0;

	if (!driverFlags.isEmpty())
	{
		QStringList flags(QStringList::split(',', driverFlags));

		for (QStringList::ConstIterator it(flags.begin()); it != flags.end(); ++it)
		{
			QString flag((*it).stripWhiteSpace());

			QStringList l(QStringList::split('=', flag));

			if (l.count() != 2)
			{
				qWarning("Option format incorrect: %s", flag.ascii());
				continue;
			}

			QString key(l[0].stripWhiteSpace());
			QString value(l[1].stripWhiteSpace());

			int x = ao_append_option(&options, key.ascii(), value.ascii());
			(void)x;
		}
	}

	postEvent(parent_, new DeviceOpenInProgressEvent);

	ao_info * driverInfo = ao_driver_info(driverID);

	if (0 == driverInfo)
	{
		qWarning("Can't get driver info for driver id %d", driverID);
		return false;
	}

	ao_sample_format sampleFormat;

	sampleFormat.bits 		= 16;
	sampleFormat.rate 		= sampleRate;
	sampleFormat.channels	= channels;

	const char * filename = "/tmp/ao_output"; // Grr.

	if (AO_TYPE_FILE == driverInfo->type)
	{
		device_ = ao_open_file(driverID, filename, 0, &sampleFormat, options);
	}
	else
	{
		device_ = ao_open_live(driverID, &sampleFormat, options);
	}
	
	ao_free_options(options);

	postEvent(parent_, new DeviceOpenCompleteEvent(0 != device_));

	return (0 != device_);
}

	QStringList
Output::driverList() const
{
	return driverList_;
}

	uint
Output::latency() const
{
//	return ao_get_latency(device_);
}

// vim:ts=2:sw=2:tw=78:noet
