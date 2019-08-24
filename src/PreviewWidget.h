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

#ifndef PREVIEW_WIDGET_H
#define PREVIEW_WIDGET_H

#include <qlist.h>
#include <qlistview.h>
#include <qstringlist.h>

class Track;
class TrackList;
class PreviewItem;
class QTimer;

class PreviewWidget : public QListView
{
	Q_OBJECT

	public:

		static PreviewWidget * instance()
		{
			return instance_;
		}

		PreviewWidget(QWidget *parent);
		virtual ~PreviewWidget();

	protected slots:

		void slotScoreChanged(Track *);
		void slotTrackAdded(Track *);
		void slotTrackChanged(Track *);
		void slotTrackListAdded(TrackList &);
		void slotTrackListDisappeared(TrackList &);
		void slotUpdate();

		void slotTrackChange(const QString &);
		void slotCurrentChanged(QListViewItem *);

	signals:

		void skipToTrack(const QString &);
		void closed();

	private:

		void _update(uint delay);
		PreviewItem * _find(const QString &);
		PreviewItem * _find(Track *);
		void _makeVisible(PreviewItem *);
		void _makeInvisible(PreviewItem *);

		void _addItem(Track *);
		static PreviewWidget * instance_;
		QList<PreviewItem> heldItemList_;

		QTimer * updateTimer_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
