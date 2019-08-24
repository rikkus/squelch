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

#ifndef MTQUEUE_H
#define MTQUEUE_H

#include <qthread.h>
#include <qqueue.h>

template<class T> class MTQueue
{
	public:

		MTQueue(uint max = -1)
			: max_(max)
		{
		}

		~MTQueue()
		{
			flush();
		}

		uint count() const
		{
			mutex_.lock();
			int count = queue_.count();
			mutex_.unlock();
			return count;
		}

		bool isFull() const
		{
			if (-1 == max_)
				return false;

			mutex_.lock();
			int count = queue_.count();
			mutex_.unlock();
			return count >= max_;
		}

		bool isEmpty() const
		{
			mutex_.lock();
			bool empty = queue_.isEmpty();
			mutex_.unlock();
			return empty;
		}

		void flush()
		{
			mutex_.lock();
			queue_.clear();
			mutex_.unlock();
		}

		void enqueue(T * t)
		{
			mutex_.lock();
			queue_.enqueue(t);
			mutex_.unlock();
		}

		T * dequeue()
		{
			mutex_.lock();
			T * i = queue_.dequeue();
			mutex_.unlock();
			return i;
		}

	private:

		QQueue<T> queue_;
		mutable QMutex mutex_;
		int max_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
