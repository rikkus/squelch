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

#include "BufferItem.h"
#include "BufferItemPool.h"

BufferItemPool * BufferItemPool::instance_ = 0L;

BufferItemPool::BufferItemPool(uint max)
	: head_(0),
		tail_(0),
		max_(max),
		last_(max - 1)
{
	pool_.setAutoDelete(true);

	for (uint i = 0; i < max_; i++)
		pool_.append(new BufferItem);
}

	BufferItem *
BufferItemPool::give()
{
	mutex_.lock();

	BufferItem * i(pool_.at(head_));
	_advance(head_);

	mutex_.unlock();

	return i;
}

	BufferItem *
BufferItemPool::take()
{
	mutex_.lock();

	BufferItem * i(pool_.at(tail_));
	_advance(tail_);

	mutex_.unlock();

	return i;
}

	BufferItem *
BufferItemPool::peek()
{
	mutex_.lock();

	BufferItem * i(pool_.at(tail_));

	mutex_.unlock();

	return i;
}

 bool
BufferItemPool::isEmpty() const
{
	mutex_.lock();

	bool empty = head_ == tail_;

	mutex_.unlock();

	return empty;
}

	bool
BufferItemPool::isFull() const
{
	mutex_.lock();

	bool full =
		(
		((tail_ == 0) && (head_ == last_)) ||
		(tail_ == head_ + 1)
		);

	mutex_.unlock();

	return full;
}

	uint
BufferItemPool::count() const
{
	mutex_.lock();

	int c = (head_ < tail_) ? (last_ - head_ + tail_) : (head_ - tail_);

	mutex_.unlock();

	return c;
}

	void
BufferItemPool::flush()
{
	mutex_.lock();

	head_ = tail_ = 0;

	mutex_.unlock();
}

	void
BufferItemPool::_advance(uint & i)
{
	if (i == last_)
		i = 0;
	else
		++i;
}

// vim:ts=2:sw=2:tw=78:noet
