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

#include <qpixmap.h>
#include <qtimer.h>

#include "Static.h"
#include "Track.h"
#include "PreviewItem.h"

PreviewItem::PreviewItem(QListView * lv, Track * t)
	: QListViewItem (lv),
		active_				(t->score() > 0),
		track_				(t),
		timer_				(0)
{
	setPixmap(0, Static::instance()->pixmap("none"));
	setText(0, track_->artist());
	setText(1, track_->title());
	setText(2, track_->lengthAsString());
}

	QString
PreviewItem::key(int, bool) const
{
	return track_->key();
}

	void
PreviewItem::setActive(bool active)
{
	active_ = active;
	setPixmap(0, Static::instance()->pixmap(active_ ? "play1" : "none"));
}

	void
PreviewItem::update()
{
	setText(0, track_->artist());
	setText(1, track_->title());
	setText(2, track_->lengthAsString());
}

// vim:ts=2:sw=2:tw=78:noet
