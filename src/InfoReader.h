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

#ifndef INFO_READER_H
#define INFO_READER_H

#include <qthread.h>
#include <qstring.h>

#include "MTQueue.h"

class QObject;
class InfoEvent;

class InfoReader : public QThread
{
	public:

		static InfoReader * instance()
		{
			if (0 == instance_)
				instance_ = new InfoReader;

			return instance_;
		}
 
		~InfoReader();

		void registerListener(QObject *);
		void unregisterListener(QObject *);

		void read(const QString &);

	protected:

		virtual void run();

		class Command {

			public:

				Command(const QString & path)
					: path_(path)
				{
				}

				QString path() const { return path_; }

			private:

				QString path_;
		};

	private:

		void _notify(InfoEvent *);

		InfoReader();

		static InfoReader * instance_;

		QObject * parent_;
		MTQueue<Command> commandQueue_;

		QMutex listenerMutex_;
		QList<QObject> listenerList_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
