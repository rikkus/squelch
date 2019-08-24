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

#ifndef DIR_WATCHER_H
#define DIR_WATCHER_H

#include <qthread.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qobject.h>

#include "Track.h"
#include "TrackList.h"

class DirWatcher : public QObject
{
	Q_OBJECT

	public:

		DirWatcher(QMutex *);
		virtual ~DirWatcher();

		void start();
		void restart();

	protected:

		bool event(QEvent *);

	signals:

		void dirChanged(const QString & path);
		void trackDisappeared(TrackList);

	private:

		class Thread : public QThread
		{
			public:

				Thread(QMutex *, QObject * parent);
				virtual ~Thread();
				void rereadAudioDir();

			private:

				typedef QMap<QString, QDateTime> MTimeMap;

				void _updateDirList();
				void _recursiveRead(const QString & dir);
				void _readFiles(const QString & dir);
				void _checkDisappeared();

				void run();

				QString dir_;
				QMap<QString, QDateTime> mTimeMap_;
				QMutex * mutex_;
				QObject * parent_;
				bool reread_;
		};

		Thread * thread_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
