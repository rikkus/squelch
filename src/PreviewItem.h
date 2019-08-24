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

#ifndef PREVIEW_ITEM
#define PREVIEW_ITEM

#include <qlistview.h>
#include <qstring.h>

class Track;
class QTimer;

class PreviewItem : public QListViewItem
{
	public:

		PreviewItem(QListView *, Track *);

		QString key(int, bool)	const;

		bool		active() 				const 	{ return active_; }
		int			score()					const 	{ return track_->score(); }
		Track *	track() 								{ return track_; }

		void		setActive(bool);

		void update();

	private:

		bool			active_;
		Track		* track_;
		QTimer	* timer_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
