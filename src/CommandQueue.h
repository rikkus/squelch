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

template<class T> class MTQueue
{
	public:

		MTQueue()
		{
		}

		bool isEmpty()
		{
			mutex_.lock();
			bool empty = queue_.isEmpty();
			mutex_.unlock();
			return empty;
		}

		void flush()
		{
			mutex_.lock();
			while (!queue_.isEmpty())
				delete (queue_.dequeue());
			mutex_.unlock();
		}

		void enqueue(T * t)
		{
			mutex_.lock();
			queue_.enqueue(t)
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
		QMutex mutex_;
}

// vim:ts=2:sw=2:tw=78:noet
