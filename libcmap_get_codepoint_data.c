/* See LICENSE file for copyright and license details. */
#include "common.h"


static int
construct_codepoint_label(const char *metalabel, uint32_t codepoint, void *buffer, size_t *bufsizep)
{
	/* See Unicode Core Spec 4.8.2 Code Point Labels */

	size_t req = sizeof("-NNNN") + strlen(metalabel) - 2u * (size_t)(*metalabel == '<');
	char *p = buffer;

	if (codepoint >> 16)  req += 1u;
	if (codepoint >> 20)  req += 1u;

	if (*bufsizep < req) {
		*bufsizep = req;
		errno = ERANGE;
		return -1;
	}

	p = stpcpy(buffer, &metalabel[*metalabel == '<']);
	p -= *metalabel == '<';
	*p++ = '-';
	if (codepoint >> 20)  *p++ = "0123456789ABCDEF"[(codepoint >> 20) & 15u];
	if (codepoint >> 16)  *p++ = "0123456789ABCDEF"[(codepoint >> 16) & 15u];
	*p++ = "0123456789ABCDEF"[(codepoint >> 12) & 15u];
	*p++ = "0123456789ABCDEF"[(codepoint >> 8) & 15u];
	*p++ = "0123456789ABCDEF"[(codepoint >> 4) & 15u];
	*p++ = "0123456789ABCDEF"[(codepoint >> 0) & 15u];
	*p = '\0';

	return 0;
}


int
libcmap_get_codepoint_data(uint32_t codepoint, struct libcmap_codepoint_data *data_out, void *buffer, size_t *bufsizep)
{
	const struct libcmap_codepoint_data *data;
	size_t i, namelen;
	const char *metalabel;

	memset(data_out, 0, sizeof(*data_out));
	data_out->name = NULL;
	data_out->unicode_1_name = NULL;
	data_out->numeric_value = NULL;
	data_out->decomposition_mapping = NULL;

	if (codepoint > LIBCMAP_ULTIMATE_CODEPOINT)
		return 0;

	if ((0xFDD0u <= codepoint && codepoint <= 0xFDEFu) || (codepoint & 0xFFFEu) == 0xFFFEu) {
		if (data_out) {
			if (construct_codepoint_label("noncharacter", codepoint, buffer, bufsizep))
				return -1;
			data_out->name = buffer;
			data_out->general_category = LIBCMAP_GENERAL_CATEGORY_OTHER_NOT_ASSIGNED;
			data_out->noncharacter = 1u;
		}
		return 1;
	}

	for (i = 0u; i < libcmap_no_data_range_list_size; i++) {
		if (codepoint <= libcmap_no_data_range_list[i].last) {
			if (libcmap_no_data_range_list[i].first > codepoint)
				break;
			if (!data_out)
				return 1;
			data = libcmap_find_codepoint_data(libcmap_no_data_range_list[i].first);
			switch ((int)data->general_category) {
			case LIBCMAP_GENERAL_CATEGORY_OTHER_CONTROL:      metalabel = "control";      goto have_metalabel;
			case LIBCMAP_GENERAL_CATEGORY_OTHER_SURROGATE:    metalabel = "surrogate";    goto have_metalabel;
			case LIBCMAP_GENERAL_CATEGORY_OTHER_PRIVATE_USE:  metalabel = "private-use";  goto have_metalabel;
			default:
				namelen = strlen(data->name);
				namelen -= sizeof(", First") - 1u;
				if (namelen + 1u > *bufsizep) {
					*bufsizep = namelen + 1u;
					errno = ERANGE;
					return -1;
				}
				memcpy(buffer, data->name, namelen - 1u);
				((char *)buffer)[namelen - 1u] = '>';
				((char *)buffer)[namelen] = '\0';
				break;

			have_metalabel:
				if (construct_codepoint_label(metalabel, codepoint, buffer, bufsizep))
					return -1;
				break;
			}
			*data_out = *data;
			data_out->name = buffer;
			return 1;
		}
	}

	data = libcmap_find_codepoint_data(codepoint);
	if (!data) {
		if (data_out) {
			if (construct_codepoint_label("reserved", codepoint, buffer, bufsizep))
				return -1;
			data_out->name = buffer;
			data_out->general_category = LIBCMAP_GENERAL_CATEGORY_OTHER_NOT_ASSIGNED;
			data_out->noncharacter = 0u;
		}
		return 1;
	}

	if (data_out) {
		if (data->name[0u] == '<') {
			if (construct_codepoint_label(data->name, codepoint, buffer, bufsizep))
				return -1;
			*data_out = *data;
			data_out->name = buffer;
		} else {
			*data_out = *data;
		}
	}

	return 1;
}
