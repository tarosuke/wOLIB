/**
 * Copyright (C) 2017, 2023 tarosuke<webmaster@tarosuke.net>
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

#include <wOLIB/comm.h>
#include <wOLIB/debug.h>
#include <wOLIB/message.h>



namespace wO {

	void Message::Send(int fd) const {
		const unsigned len(sizeof(Head) + pack.head.len * sizeof(u32));
		if (write(fd, &pack, len) != len) {
			throw -1;
		}
	}


	ReceivedMessage::ReceivedMessage(int fd) : Message(pack.pack) {
		// ヘッダ読み
		if (read(fd, &pack.pack.head, sizeof(Head)) < (int)sizeof(Head)) {
			throw -1;
		}

		// エンディアンチェック
		if (0 <= (i16)pack.pack.head.type) {
			Reverse((u32*)&pack.pack.head, sizeof(Head) / sizeof(u32));
			ReadBody(fd);
			Reverse(
				pack.pack.body,
				pack.pack.head.endianConvertElements * sizeof(u32));
		} else {
			ReadBody(fd);
		}
	}

	void ReceivedMessage::Reverse(u32* body, unsigned elements) {
		for (; elements--; ++body) {
			*body = (*body >> 24) | ((*body >> 8) & 0xff00) |
					((*body << 8) & 0xff0000) | (*body << 24);
		}
	}

	void ReceivedMessage::ReadBody(int fd) {
		if (pack.pack.head.len) {
			const unsigned len(pack.pack.head.len * sizeof(u32));
			/// body読み
			if (read(fd, pack.pack.body, len) < len) {
				throw -1;
			}
		}
	}


	SpawnMessage::SpawnMessage(u32 id, Intent intent, const char* path)
		: body((Body*)pack.pack.body) {
		const unsigned pathLen(strlen(path));

		pack.pack.head.len = sizeof(body) + pathLen;
		if (maxElements * sizeof(u32) < pack.pack.head.len) {
			// パス長すぎ
			throw -1;
		}
		strcpy(body->target, path);
		pack.pack.head.id = id;
		pack.pack.head.len = 2 + (pathLen + sizeof(u32) - 1) / sizeof(u32);
		pack.pack.head.endianConvertElements = 2;
		pack.pack.head.type = spawn;
	}
}
