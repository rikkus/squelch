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

#include <qfile.h>
#include <qfileinfo.h>

#include "PlayList.h"
#include "Input.h"
#include "BufferItem.h"
#include "BufferItemPool.h"

Input::Input(QObject * parent)
	: QThread				(),
		parent_				(parent),
		currentTrack_	(QString::null),
		trackLength_	(0)
{
	// Empty.
}

Input::~Input()
{
	// Empty.
}

	void
Input::run()
{
	while (true)
	{
		if (!commandQueue_.isEmpty())
		{
			BufferItemPool::instance()->flush();

			Command * c = commandQueue_.dequeue();

			switch (c->type())
			{
				case Command::Play:
					_changeTrack(PlayList::instance()->firstTrack());
					break;

				case Command::Next:
					_changeTrack(PlayList::instance()->nextTrack(currentTrack()));
					break;

				case Command::Previous:
					_changeTrack(PlayList::instance()->previousTrack(currentTrack()));
					break;

				case Command::Stop:
					setCurrentTrack(QString::null);
					break;

				case Command::Seek:
					_seek(c->data());
					break;

				case Command::Skip:
					_changeTrack(c->stringData());
					break;

				default:
					break;
			}

			delete c;
			c = 0;
		}

		// If the pool is full or no current track, start again.

		if (BufferItemPool::instance()->isFull() || !currentTrack())
		{
			msleep(10);
			continue;
		}

		// We should try to read some data now.

		BufferItem * i = BufferItemPool::instance()->give();

		bool ok = _read(i);

		// If we ran out of data, we change to the next track.

		if (!ok)
		{
			_changeTrack(PlayList::instance()->nextTrack(currentTrack()));
		}
	}
}

	void
Input::_changeTrack(const QString & id)
{
	Track * t = PlayList::instance()->track(id);

	if (0 == t)
		return;

	_changeTrack(id, t->filename());
}

	void
Input::skipToTrack(const QString & id)
{
	_enqueueCommand(new Command(Command::Skip, id));
}

	void
Input::play()
{
	_enqueueCommand(new Command(Command::Play));
}

	void
Input::next()
{
	_enqueueCommand(new Command(Command::Next));
}

	void
Input::previous()
{
	_enqueueCommand(new Command(Command::Previous));
}

	void
Input::seek(long sec)
{
	_enqueueCommand(new Command(Command::Seek, sec));
}

	void
Input::stop()
{
	_enqueueCommand(new Command(Command::Stop));
}

	void
Input::_enqueueCommand(Command * c)
{
	commandQueue_.enqueue(c);
}

// vim:ts=2:sw=2:tw=78:noet
