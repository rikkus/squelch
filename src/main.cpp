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

#include <qapplication.h>
#include <qdir.h>

#include "RemoteControl.h"

#if QT_VERSION < 300
#include "RikStyle.h"
#endif

#include "BufferItemPool.h"
#include "IOController.h"
#include "InfoReader.h"
#include "PlayList.h"
#include "PreviewWidget.h"
#include "Static.h"
#include "TrackAddNotifyDialog.h"

	int
main(int argc, char * argv[])
{
#if QT_VERSION < 300
	bool customStyle = false;

	for (int i = 1; i < argc; i++)
		if (0 == strncmp(argv[i], "-style", strlen(argv[i])))
		{
			customStyle = true;
			break;
		}
#endif

	if (!QDir(QDir::homeDirPath() + "/.squelch").exists())
		if (!QDir().mkdir(QDir::homeDirPath() + "/.squelch"))
		{
			qDebug("Can't make ~/.squelch - check home dir perms !");
			return 1;
		}

	QApplication * app = new QApplication(argc, argv);

#if QT_VERSION < 300
	if (!customStyle)
		app->setStyle(new RikStyle);
#endif

	new TrackAddNotifyDialog;

	app->setMainWidget(new RemoteControl);

	return app->exec();
}

// vim:ts=2:sw=2:tw=78:noet
