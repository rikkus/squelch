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

#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include "OptionsDialogBase.h"

class OptionsDialog : public OptionsDialogBase
{
	Q_OBJECT

	public:

		OptionsDialog(QWidget * parent = 0, const char * name = 0);
		virtual ~OptionsDialog();

	protected slots:

		void slotBrowse();
		void accept();
		void reject();

	protected:

		void closeEvent(QCloseEvent *);

		void load();
		void save();

	signals:

		void closed();

	private:

		QString savedOutputDriver_;
		QString savedOutputFlags_;
		QString savedAudioDir_;
};

#endif
// vim:ts=2:sw=2:tw=78:noet