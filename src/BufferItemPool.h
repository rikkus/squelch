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

#ifndef BUFFER_ITEM_POOL_H
#define BUFFER_ITEM_POOL_H

#include <qlist.h>
#include <qthread.h>
#include <qqueue.h>

class BufferItem;

class BufferItemPool
{
	public:

		static BufferItemPool * instance(uint max = 128)
		{
			if (0 == instance_)
				instance_ = new BufferItemPool(max);

			return instance_;
		}

		uint count() const;

		BufferItem * give();
		BufferItem * take();
		BufferItem * peek();

		void flush();

		bool isEmpty() const;
		bool isFull() const;

	private:

		void _advance(uint &);

		BufferItemPool(uint max);
		~BufferItemPool();

		static BufferItemPool * instance_;

		uint head_;
		uint tail_;
		uint max_;
		uint last_;

		QList<BufferItem> pool_;

		mutable QMutex mutex_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
