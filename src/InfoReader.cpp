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

#include <string.h> // For memset.

#include <vorbis/vorbisfile.h>

#include <qobject.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qdatetime.h>

#include "Track.h"
#include "Defines.h"
#include "InfoReader.h"
#include "InfoEvent.h"
#include "VorbisHelpers.h"

InfoReader * InfoReader::instance_ = 0L;

InfoReader::InfoReader()
	: QThread()
{
	start();
}

InfoReader::~InfoReader()
{
}

	void
InfoReader::registerListener(QObject * o)
{
	listenerMutex_.lock();

	listenerList_.append(o);

	listenerMutex_.unlock();
}

	void
InfoReader::unregisterListener(QObject * o)
{
	listenerMutex_.lock();

	listenerList_.append(o);

	listenerMutex_.unlock();
}

	void
InfoReader::_notify(InfoEvent * e)
{
	listenerMutex_.lock();

	for (QListIterator<QObject> it(listenerList_); it.current(); ++it)
		postEvent(it.current(), e);

	listenerMutex_.unlock();
}

	void
InfoReader::run()
{
	while (true)
	{
		if (commandQueue_.isEmpty())
		{
			msleep(10);
			continue;
		}

		Command * c = commandQueue_.dequeue();

		QString artist, album, track;

		QFile f(c->path());

		if (!f.open(IO_ReadOnly))
		{
			delete c;
			continue;
		}

		OggVorbis_File vf;

		memset(&vf, 0, sizeof(vf));

		ov_callbacks callbacks;

		callbacks.read_func		= fread_func;
		callbacks.seek_func		= fseek_func;
		callbacks.close_func	= fclose_func;
		callbacks.tell_func		= ftell_func;

		if (ov_open_callbacks(&f, &vf, NULL, 0, callbacks) < 0)
		{
			delete c;
			continue;
		}

		char **ptr = ov_comment(&vf, -1)->user_comments;

		while (0 != *ptr)
		{
			if (0 == qstrnicmp(*ptr, "ARTIST=", 7))
				artist = QString::fromUtf8(*ptr + 7);

			if (0 == qstrnicmp(*ptr, "ALBUM=", 6))
				album	= QString::fromUtf8(*ptr + 6);

			if (0 == qstrnicmp(*ptr, "TITLE=", 6))
				track	= QString::fromUtf8(*ptr + 6);

			++ptr;
		}

		long l = long(ov_time_total(&vf, -1));

		QDateTime mTime(QFileInfo(f).lastModified());

		Track * t = new Track(artist, album, track, c->path(), mTime, l);

		_notify(new InfoEvent(t));

		ov_clear(&vf);

		delete c;
	}
}

	void
InfoReader::read(const QString & filename)
{
	commandQueue_.enqueue(new Command(filename));
}

// vim:ts=2:sw=2:tw=78:noet
