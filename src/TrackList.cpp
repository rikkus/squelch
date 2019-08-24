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

#include "Track.h"
#include "TrackList.h"

TrackList::TrackList()
	: QList<Track>()
{
	// Empty.
}

TrackList::~TrackList()
{
	// Empty.
}

	Track *
TrackList::find(Track * t)
{
	return find(t->filename());
}

	Track *
TrackList::find(const QString & filename)
{
	for (TrackListIterator it(*this); it.current(); ++it)
		if (it.current()->filename() == filename)
			return it.current();

	return 0;
}

	bool
TrackList::contains(Track * t)
{
	return (0 != find(t));
}

	bool
TrackList::contains(const QString & filename)
{
	return (0 != find(filename));
}

	int
TrackList::compareItems(QCollection::Item item1, QCollection::Item item2)
{
	return		static_cast<Track *>(item1)->key()
		.compare(static_cast<Track *>(item2)->key());
}

// vim:ts=2:sw=2:tw=78:noet
