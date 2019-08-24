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
#include <qlistview.h>

#include "Static.h"
#include "PlayList.h"
#include "TitleItem.h"
#include "Track.h"
#include "SquelchItem.h"

TitleItem::TitleItem(
		SquelchItem * parent,
		Track * t
)
	: SquelchItem(TitleItemT, parent, t->title()),
		track_(t)
{
	setPixmap(0, Static::instance()->pixmap("track"));
	setText(1, track_->lengthAsString());
	setText(2, QString::number(QABS(track_->score())));

	if (track_->title().isEmpty())
		setText(0, "[not set]");
}

TitleItem::~TitleItem()
{
}

	QString
TitleItem::key(int, bool) const
{
	return track_->key();
}

	void
TitleItem::paintCell
(
 QPainter * p,
 const QColorGroup & g,
 int c,
 int w,
 int a
)
{
	if (c != 0)
		return SquelchItem::paintCell(p, g, c, w, a);

	else
	{
		QColorGroup g2(g);
		QColor col(track_->score() > 0 ? g.text() : g.mid());
		g2.setColor(QColorGroup::Text, col);
		SquelchItem::paintCell(p, g2, c, w, a);
	}
}

	void
TitleItem::increaseScore()
{
	PlayList::instance()->increaseScore(track_);
	setText(2, QString::number(QABS(track_->score())));
}

	void
TitleItem::decreaseScore()
{
	PlayList::instance()->decreaseScore(track_);
	setText(2, QString::number(QABS(track_->score())));
}

	void
TitleItem::update()
{
	setText(1, track_->lengthAsString());
	setText(2, QString::number(QABS(track_->score())));
}

// vim:ts=2:sw=2:tw=78:noet
