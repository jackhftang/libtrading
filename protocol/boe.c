#include "fix/boe.h"

#include "fix/buffer.h"

#include <string.h>

int boe_decode_header(struct buffer *buf, struct boe_header *header)
{
	memcpy(header, buffer_start(buf), sizeof *header);

	buffer_advance(buf, sizeof *header);

	return 0;
}

struct boe_login_request *boe_decode_login_request(struct boe_header *header, struct buffer *buf)
{
	struct boe_login_request *login;
	size_t size;

	size = header->MessageLength - sizeof(header);

	login = malloc(size);
	if (!login)
		return NULL;

	memcpy(login, buffer_start(buf), size);

	buffer_advance(buf, size);

	return login;
}

struct boe_login_response *boe_decode_login_response(struct boe_header *header, struct buffer *buf)
{
	struct boe_login_response *login;
	size_t size;

	size = header->MessageLength - sizeof(header);

	login = malloc(size);
	if (!login)
		return NULL;

	memcpy(login, buffer_start(buf), size);

	buffer_advance(buf, size);

	return login;
}

struct boe_logout *boe_decode_logout(struct boe_header *header, struct buffer *buf)
{
	struct boe_logout *logout;
	size_t size;

	size = header->MessageLength - sizeof(header);

	logout = malloc(size);
	if (!logout)
		return NULL;

	memcpy(logout, buffer_start(buf), size);

	buffer_advance(buf, size);

	return logout;
}
