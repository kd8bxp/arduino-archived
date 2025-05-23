/*
 * LoRaMaDoR (LoRa-based mesh network for hams) project
 * Copyright (c) 2019 PU5EPX
 */

/* Class that encapsulates a buffer or string.
 *
 * This class is used instead of Arduino String for a couple reasons:
 * 1) Works on other operating systems, so it is easier on unit testing
 *    run outside Arduuino. (Arduino String has an API different from
 *    C++ STL String.)
 * 2) String is considered a memory hog in AVR Arduino. This reason is
 *    probably obsolete since ESP32 is the reference platform now.
 * 3) Arduino strings miss a couple features to be used as raw buffers
 *    (e.g. create buffer with predefined length) and it seems to
 *    assume a zero-terminated string in some methods
 *    (case in focus: strcpy() in String::move method.)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <cstring>

#include <string.h>
#include <stdarg.h>
#include "Buffer.h"
#include "Arduino.h"

class BufferImpl {
	friend class Buffer;

	static void init(Buffer* b, const char *s, size_t len)
	{
		b->len = len;
		b->buf = new char[len + 1];
		if (s) {
			memcpy(b->buf, s, len);
		} else {
			memset(b->buf, 0, len);
		}
		b->buf[len] = 0;
	}

	static Buffer sprintf(const char *mask, ...)
	{
		va_list args;
		va_start(args, mask);
	
		va_list tmpargs;
		va_copy(tmpargs, args);
	
		int size = vsnprintf(NULL, 0, mask, tmpargs);
		va_end(tmpargs);
	
		Buffer ret;
		if (size < 0) {
			ret = "fail";
		} else {
			ret = Buffer(size);
			vsprintf(ret.buf, mask, args);
		}
	
		va_end(args);
		return ret;
	}
};

Buffer::Buffer()
{
	BufferImpl::init(this, 0, 0);
}

Buffer::Buffer(int len)
{
	BufferImpl::init(this, 0, len);
}

Buffer::Buffer(const Buffer& model)
{
	BufferImpl::init(this, model.buf, model.len);
}

Buffer::Buffer(Buffer&& moved)
{
	this->len = moved.len;
	this->buf = moved.buf;
	moved.len = 0;
	moved.buf = 0;
}

Buffer& Buffer::operator=(Buffer&& moved)
{
	if (this != &moved) {
		delete [] this->buf;
		this->len = moved.len;
		this->buf = moved.buf;
		moved.len = 0;
		moved.buf = 0;
	}
	return *this;
}

Buffer& Buffer::operator=(const Buffer& model)
{
	if (this != &model) {
		delete [] buf;
		BufferImpl::init(this, model.buf, model.len);
	}
	return *this;
}

Buffer& Buffer::operator+=(const Buffer &b)
{
	return append(b.buf, b.len);
}

Buffer& Buffer::operator+=(const char *b)
{
	return append(b, strlen(b));
}

Buffer& Buffer::append(const char *s, size_t add_length)
{
	if (!s) {
		return *this;
	}

	char *oldbuf = this->buf;
	this->buf = new char[this->len + add_length + 1];
	memcpy(this->buf, oldbuf, this->len);
	delete [] oldbuf;

	memcpy(this->buf + this->len, s, add_length);
	this->buf[this->len + add_length] = 0;
	this->len += add_length;

	return *this;
}

Buffer& Buffer::operator+=(const char c)
{
	char *oldbuf = this->buf;
	this->buf = new char[this->len + 2];
	memcpy(this->buf, oldbuf, this->len);
	delete [] oldbuf;

	this->buf[this->len] = c;
	this->buf[this->len + 1] = 0;
	this->len += 1;

	return *this;
}

Buffer Buffer::operator+(const Buffer& b) const
{
	return Buffer(*this) += b;
}

Buffer Buffer::operator+(const char * b) const
{
	return Buffer(*this) += b;
}

Buffer Buffer::operator+(const char b) const
{
	return Buffer(*this) += b;
}

Buffer::~Buffer()
{
	delete [] this->buf;
	this->buf = 0;
	this->len = 0;
}

Buffer::Buffer(const char *s, int len)
{
	BufferImpl::init(this, s, len);
}

Buffer::Buffer(const char *s)
{
	BufferImpl::init(this, s, strlen(s));
}

const char* Buffer::c_str() const
{
	return this->buf;
}

char* Buffer::hot(size_t n)
{
	return this->buf + n;
}

size_t Buffer::length() const
{
	return this->len;
}

int Buffer::charAt(int i) const
{
	if (i < 0) {
		i = this->len + i;
	}
	if (i >= (signed) this->len || i < 0) {
		return -1;
	}
	return this->buf[i];
}


int Buffer::indexOf(char c) const
{
	for (size_t i = 0; i < this->len; ++i) {
		if (this->buf[i] == c) {
			return i;
		}
	}
	return -1;
}

Buffer& Buffer::cut(int i)
{
	size_t ai = abs(i);
	if (ai > this->len) {
		ai = this->len;
	}
	size_t hi = (i >= 0) ? i : 0;
	if (hi > this->len) {
		hi = this->len;
	}
	
	char *oldbuf = this->buf;
	this->buf = new char[this->len - ai + 1];
	memcpy(this->buf, oldbuf + hi, this->len - ai);
	delete [] oldbuf;

	this->len -= ai;
	this->buf[this->len] = 0;

	return *this;
}

Buffer& Buffer::lstrip() {
	while (charAt(0) == ' ') {
		cut(1);
	}
	return *this;
}

Buffer& Buffer::rstrip() {
	while (charAt(-1) == ' ') {
		cut(-1);
	}
	return *this;
}

Buffer& Buffer::strip() {
	lstrip();
	rstrip();
	return *this;
}

bool Buffer::empty() const {
	return this->len == 0;
}

Buffer& Buffer::uppercase()
{
	for (size_t i = 0; i < len; ++i) {
		if (buf[i] >= 'a' && buf[i] <= 'z') {
			buf[i] += 'A' - 'a';
		}
	}
	return *this;
}

bool Buffer::operator==(const char *cmp) const
{
	return strcmp(buf, cmp) == 0;
  //return std::string::compare(buf, cmp) == 0;
}

bool Buffer::operator!=(const char *cmp) const
{
	return ! (*this == cmp);
}

bool Buffer::operator==(const Buffer& cmp) const
{
	return compareTo(cmp) == 0;
}

bool Buffer::operator!=(const Buffer &cmp) const
{
	return ! (*this == cmp);
}

int Buffer::compareTo(const Buffer &cmp) const
{
	size_t minlen = len < cmp.len ? len : cmp.len;
	int ret = memcmp(buf, cmp.buf, minlen);
	if (ret == 0) {
		if (len < cmp.len) {
			ret = -1;
		} else if (len > cmp.len) {
			ret = +1;
		}
	}
	return ret;
}

int Buffer::compareTo(const char *cmp) const
{
	return strcmp(buf, cmp);
 //return std::string::compare(buf,cmp);
}

bool Buffer::startsWith(const char *cmp) const
{
	size_t cmplen = strlen(cmp);
	if (len < cmplen) {
		return false;
	}
	return strncmp(buf, cmp, cmplen) == 0;
}

bool Buffer::startsWith(const Buffer &cmp) const
{
	if (len < cmp.len) {
		return false;
	}
	for (size_t i = 0; i < cmp.len; ++i) {
		if (buf[i] != cmp.buf[i]) {
			return false;
		}
	}
	return true;
}

Buffer Buffer::millis_to_hms(int32_t t)
{
	if (t < 0) return "???";
	t /= 1000;
	int32_t s = t % 60;
	t -= s;
	t /= 60;
	int32_t m = t % 60;
	t -= m;
	t /= 60;
	int32_t h = t % 24;
	t -= h;
	t /= 24;
	int32_t d = t;

	if (d > 0) return BufferImpl::sprintf("%d:%02d:%02d:%02d", d, h, m, s);
	if (h > 0) return BufferImpl::sprintf("%d:%02d:%02d", h, m, s);
	return BufferImpl::sprintf("%d:%02d", m, s);
}

Buffer Buffer::itoa(int32_t i)
{
	return BufferImpl::sprintf("%d", i);
}


Buffer Buffer::substr(size_t start) const
{
	return substr(start, this->len - start);
}

Buffer Buffer::substr(size_t start, size_t count) const
{
	if (start >= this->len) {
		start = 0;
		count = 0;
	}

	if ((start + count) > this->len) {
		count = this->len - start;
	}

	Buffer copy(count);
	memcpy(copy.buf, this->buf + start, count);
	return copy;
}
