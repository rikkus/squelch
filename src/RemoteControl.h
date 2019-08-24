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

#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include <qwidget.h>
#include <qfont.h>

#include "Track.h"
#include "Defines.h"

class PlayListEditWidget;
class ScrollingLabel;
class Seeker;
class TimeDisplay;
class QLabel;
class QToolButton;
class OptionsDialog;

class RemoteControl : public QWidget
{
	Q_OBJECT

	public:

		RemoteControl(QWidget * parent = 0);
		virtual ~RemoteControl();

	protected:

		void info(const QString &);

	protected slots:

		void slotPlayListEditorToggle();
		void slotConfigDialogToggle();

		void slotVolumeChange(int);
		void slotStatusChange(OutputStatus);
		void slotTrackChange(const QString &);

		void slotDeviceOpenInProgress();
		void slotDeviceOpenComplete(bool);

	private:

		ScrollingLabel				* trackNameLabel_;
		TimeDisplay						* timeDisplay_;
		Seeker								* seeker_;

		QToolButton						* play_;
		QToolButton						* next_;
		QToolButton						* prev_;
		QToolButton						* stop_;
		QToolButton						* rstt_;

		QToolButton						* conf_;
		QToolButton						* list_;

		PlayListEditWidget		* playListEditor_;
		OptionsDialog					* configDialog_;

		OutputStatus						outputStatus_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet
