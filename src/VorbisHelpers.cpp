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

#include <qfile.h>
#include <qfileinfo.h>

#include "VorbisHelpers.h"

	size_t
fread_func(void * ptr, size_t size, size_t n, void * stream)
{
	QIODevice * dev = reinterpret_cast<QIODevice *>(stream);

	return size_t(dev->readBlock(reinterpret_cast<char *>(ptr), uint(size * n)));
}

	int
fseek_func(void * stream, int64_t offset, int whence)
{
	int retval = -1;

	QIODevice * dev = reinterpret_cast<QIODevice *>(stream);

	if (dev->isSequentialAccess())
		return -1;

	switch (whence)
	{
		case SEEK_SET:	retval = dev->at(int(offset));							break;
		case SEEK_CUR:	retval = dev->at(dev->at() + int(offset));	break;
		case SEEK_END:	retval = dev->at(dev->size());							break;
		default:				retval = -1;																break;
	}

	return retval;
}

	int
fclose_func(void * stream)
{
	QIODevice * dev = reinterpret_cast<QIODevice *>(stream);
	dev->close();
	return 1;
}

	long
ftell_func(void * stream)
{
	QIODevice * dev = reinterpret_cast<QIODevice *>(stream);
	return long(dev->at());
}

// vim:ts=2:sw=2:tw=78:noet
