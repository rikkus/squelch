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

#include <qdir.h>
#include <qfileinfo.h>
#include <qtextstream.h>

#include "InfoReader.h"
#include "DirWatcher.h"
#include "DirChangedEvent.h"
#include "TrackDisappearedEvent.h"
#include "PlayList.h"
#include "Track.h"

DirWatcher::DirWatcher(QMutex * m)
	:	QObject()
{
	thread_ = new Thread(m, this);
}

DirWatcher::~DirWatcher()
{
	delete thread_;
}

	void
DirWatcher::start()
{
	if (thread_->running())
	{
		qWarning("DirWatcher: already running");
	}
	else
	{
		thread_->start();
	}
}

	void
DirWatcher::restart()
{
	thread_->rereadAudioDir();
}

	bool
DirWatcher::event(QEvent * e)
{
	switch (e->type())
	{
		case DirChangedEventT:
			emit(dirChanged(static_cast<DirChangedEvent *>(e)->dir()));
			return true;
			break;

		case TrackDisappearedEventT:
			emit(trackDisappeared(static_cast<TrackDisappearedEvent *>(e)->list()));
			return true;
			break;

		default:
			return false;
			break;
	}
}

DirWatcher::Thread::Thread(QMutex * m, QObject * parent)
	: mutex_	(m),
		parent_ (parent),
		reread_ (false)
{
}

DirWatcher::Thread::~Thread()
{
}

	void
DirWatcher::Thread::run()
{
	{
		QFile f(QDir::homeDirPath() + "/.squelch/audiodir");

		while (!f.open(IO_ReadOnly))
		{
			while (!reread_)
				sleep(1);
		}

		QTextStream t(&f);

		dir_ = t.readLine();
	}

	while (true)
	{
		mutex_->lock();

		if (reread_)
		{
			qDebug("DirWatcher::Thread::run(): rereading audio dir");
			reread_ = false;

			mTimeMap_.clear();

			QFile f(QDir::homeDirPath() + "/.squelch/audiodir");

			if (!f.open(IO_ReadOnly))
			{
				//qWarning("No audio dir specified");
				return;
			}

			QTextStream t(&f);

			dir_ = t.readLine();
		}

		_updateDirList();
		_checkDisappeared();

		mutex_->unlock();

		sleep(5);
	}
}

	void
DirWatcher::Thread::rereadAudioDir()
{
	reread_ = true;
}

	void
DirWatcher::Thread::_updateDirList()
{
	if (mTimeMap_.isEmpty())
		_recursiveRead(dir_);

	else
	{
		bool modified(false);

		for (MTimeMap::Iterator it(mTimeMap_.begin()); it != mTimeMap_.end(); ++it)
		{
			QFileInfo fi(it.key());

			if (!fi.exists() || (it.data() != fi.lastModified()))
			{
				modified = true;
				break;
			}
		}

		if (modified)
		{
			mTimeMap_.clear();
			_recursiveRead(dir_);
		}
	}
}

	void
DirWatcher::Thread::_recursiveRead(const QString & dir)
{
	QDir d(dir, QString::null, QDir::Unsorted, QDir::Dirs | QDir::Readable);

	if (!d.exists())
		return;

	const QFileInfoList * l(d.entryInfoList());

	for (QFileInfoListIterator it(*l); it.current(); ++it)
		if (it.current()->isDir() && it.current()->fileName()[0] != '.')
		{
			QString path(it.current()->absFilePath());

			mTimeMap_.insert(path, it.current()->lastModified());

			_recursiveRead(path);
		}

	mTimeMap_.insert(dir, QFileInfo(dir).lastModified());
	_readFiles(dir);
}

	void
DirWatcher::Thread::_readFiles(const QString & dir)
{
	QStringList l(QDir(dir).entryList(QDir::Files | QDir::Readable));

	for (QStringList::ConstIterator it(l.begin()); it != l.end(); ++it)
	{
		QString filename(dir + '/' + *it);

		Track * t(PlayList::instance()->find(filename));

		if (0 == t)
			InfoReader::instance()->read(filename);

		else
			if (QFileInfo(filename).lastModified() != t->mTime())
				InfoReader::instance()->read(filename);
	}
}

	void
DirWatcher::Thread::_checkDisappeared()
{
	TrackList disappeared;

	TrackList l(PlayList::instance()->all());

	for (TrackListIterator it(l); it.current(); ++it)
		if (!QFile(it.current()->filename()).exists())
			disappeared.append(it.current());

	if (!disappeared.isEmpty())
		postEvent(parent_, new TrackDisappearedEvent(disappeared));
}

#include "DirWatcher.moc"

// vim:ts=2:sw=2:tw=78:noet
