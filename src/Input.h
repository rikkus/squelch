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

#ifndef INPUT_H
#define INPUT_H

#include <qobject.h>
#include <qthread.h>
#include <qstring.h>
#include <qfile.h>

#include "Track.h"
#include "MTQueue.h"

class BufferItem;

class Input : public QThread
{
	public:

		Input(QObject * parent);
		virtual ~Input();

		class Command {

			public:

				enum Type { Play, Stop, Next, Previous, Seek, Skip };

				Command(Type t, long d = 0L)
					: type_(t),
						data_(d)
				{
				}

				Command(Type t, const QString & d)
					: type_(t),
						stringData_(d)
				{
				}


				Type 		type() 				const { return type_;				}
				long 		data() 				const { return data_;				}
				QString	stringData() 	const { return stringData_;	}

			private:

				Type type_;
				long data_;
				QString stringData_;
		};

		void play();
		void skipToTrack(const QString &);
		void seek(long);
		void stop();
		void next();
		void previous();

	protected:

		void run();

		QObject *						parent()				{ return parent_;				}
		MTQueue<Command> &	commandQueue() 	{ return commandQueue_;	}

		void _enqueueCommand(Command *);

		QString	currentTrack() 											const	{	return currentTrack_;	}
		void		setCurrentTrack(const QString & s)				{	currentTrack_ = s;		}

		void		_changeTrack(const QString &);

		virtual void _changeTrack(const QString &, const QString & location) = 0;
		virtual void _seek(long sec) = 0;
		virtual bool _read(BufferItem *) = 0;

	private:

		// Order dependency
		QObject	* parent_;
		QString		currentTrack_;
		long			trackLength_;
		// End order dependency

		MTQueue<Command> commandQueue_;

		QFile file_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
