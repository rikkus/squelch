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

#include <qbitmap.h>
#include <qpixmap.h>

#include "Static.h"
#include "pixmaps.h"

Static * Static::instance_ = 0L;

Static::Static()
{
	pix_["close"]					= QPixmap((const char **)(px_close));
	pix_["playlist"]			= QPixmap((const char **)(px_playlist));
	pix_["album"]					= QPixmap((const char **)(px_album));
	pix_["artist"]				= QPixmap((const char **)(px_artist));
	pix_["track"]					= QPixmap((const char **)(px_track));
	pix_["next"]					= QPixmap((const char **)(px_next));
	pix_["stop"]					= QPixmap((const char **)(px_stop));
	pix_["previous"]			= QPixmap((const char **)(px_previous));
	pix_["play1"]					= QPixmap((const char **)(px_play1));
	pix_["play2"]					= QPixmap((const char **)(px_play2));
	pix_["play3"]					= QPixmap((const char **)(px_play3));
	pix_["pause"]					= QPixmap((const char **)(px_pause));
	pix_["x"]							= QPixmap((const char **)(px_x));
	pix_["plus"]					= QPixmap((const char **)(px_plus));
	pix_["minus"]					= QPixmap((const char **)(px_minus));
	pix_["config"]				= QPixmap((const char **)(px_config));
	pix_["none"]					= QPixmap((const char **)(px_none));
	pix_["squelch"]				= QPixmap((const char **)(px_squelch));
	pix_["squelch_large"]	= QPixmap((const char **)(px_squelch_large));
	pix_["restart"]				= QPixmap((const char **)(px_restart));
}

// vim:ts=2:sw=2:tw=78:noet
