/**
 * Copyright (C) 2017,2023,2025 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <tb/time.h>
#include <wOLIB/comm.h>
#include <wOLIB/debug.h>
#include <wOLIB/message.h>



namespace wO {

	void Message::Send(int fd) {
		pack.timestamp = tb::msec(tb::Timestamp().Uptime());
		Reverse((tb::u32*)&pack, headElements);
		if (write(fd, &pack, sizeof(Pack)) != sizeof(Pack)) {
			throw -1;
		}
		if (pack.elements) {
			Reverse(pack.body, pack.endianConvertElements);
			const unsigned len(sizeof(Pack) + pack.elements * sizeof(u32));
			if (write(fd, pack.body, len) != len) {
				throw -1;
			}
		}
	}


	void Message::Receive(int fd) {
		// ヘッダ読み
		if (read(fd, &pack, sizeof(Pack)) != (int)sizeof(Pack)) {
			throw -1;
		}

		// エンディアンチェック
		if (0 <= (i16)pack.type) {
			Reverse((u32*)&pack, sizeof(Pack) / sizeof(u32));
			ReadBody(fd);
			Reverse((tb::u32*)&pack, pack.endianConvertElements);
		} else {
			ReadBody(fd);
		}
	}

	void Message::Reverse(u32* body, unsigned elements) {
		for (; elements--; ++body) {
			*body = (*body >> 24) | ((*body >> 8) & 0xff00) |
					((*body << 8) & 0xff0000) | (*body << 24);
		}
	}

	void Message::ReadBody(int fd) {
		if (pack.elements) {
			const unsigned len(pack.elements * sizeof(u32));
			/// body読み
			if (read(fd, pack.body, len) != len) {
				throw -1;
			}
		}
	}

}
