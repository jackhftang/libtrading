#include "trading/buffer.h"

#include "trading/read-write.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

struct buffer *buffer_new(unsigned long capacity)
{
	struct buffer *buf;

	buf = malloc(sizeof *buf + capacity);
	if (!buf)
		return NULL;

	buf->data	= (void *) buf + sizeof(*buf);
	buf->capacity	= capacity;
	buf->start	= 0;
	buf->end	= 0;

	return buf;
}

void buffer_delete(struct buffer *buf)
{
	free(buf);
}

uint8_t buffer_sum(struct buffer *buf)
{
	unsigned long sum = 0;
	int i;

	for (i = buf->start; i < buf->end; i++)
		sum += buf->data[i];

	return sum;
}

bool buffer_printf(struct buffer *buf, const char *format, ...)
{
	size_t size;
	va_list ap;
	char *p;
	int len;

	p	= buffer_end(buf);
	size	= buffer_remaining(buf);

	va_start(ap, format);
	len = vsnprintf(p, size, format, ap);
	va_end(ap);

	if (len < 0 || len >= size)
		return false;

	buf->end += len;

	return true;
}

char *buffer_find(struct buffer *buf, char delim)
{
	while (buffer_first_char(buf) != delim) {
		if (!buffer_remaining(buf))
			return NULL;

		buffer_advance(buf, 1);
	}

	return buffer_start(buf);
}

ssize_t buffer_read(struct buffer *buf, int fd)
{
	size_t count;
	ssize_t len;
	void *p;

	p	= buffer_end(buf);
	count	= buffer_remaining(buf);

	len = xread(fd, p, count);
	if (len < 0)
		return len;

	buf->end	+= len;

	return len;
}

ssize_t buffer_write(struct buffer *buf, int fd)
{
	return xwrite(fd, buffer_start(buf), buffer_size(buf));
}

void buffer_compact(struct buffer *buf)
{
	size_t count;
	void *start;

	start	= buffer_start(buf);
	count	= buffer_size(buf);

	memmove(buf->data, start, count);

	buf->start	= 0;
	buf->end	= count;
}
