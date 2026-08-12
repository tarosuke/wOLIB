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

#include <wOLIB/comm.h>
#include <wOLIB/debug.h>
#include <wOLIB/message.h>



namespace wO {

	void Message::Send(Comm& c) { c.Send((void*)&pack, pack.nElement); };



	void ReceivedMessage::Hear(Comm& c) {
		// ヘッダ読み
		c.Hear(&pack, packNElement);

		// エンディアンチェック
		if (Message::pack.type & 0x8000) {
			Reverse((u32*)&pack, sizeof(Pack) / sizeof(u32));
			c.Hear(pack.body, pack.nElement);
			Reverse((tb::u32*)&pack, pack.endianConvertElements);
		} else {
			c.Hear(pack.body, pack.nElement);
		}
	}

	void ReceivedMessage::Reverse(u32* body, unsigned elements) {
		for (; elements--; ++body) {
			*body = (*body >> 24) | ((*body >> 8) & 0xff00) |
					((*body << 8) & 0xff0000) | (*body << 24);
		}
	}


}
