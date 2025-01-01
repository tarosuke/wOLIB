/**
 * Copyright (C) 2017,2025 tarosuke<webmaster@tarosuke.net>
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
#include <unistd.h>

#include <wOLIB/comm.h>
#include <wOLIB/debug.h>
#include <wOLIB/message.h>
#include <wOLIB/object.h>



namespace wO {

	Comm::~Comm() {
		if (0 <= readHandle) {
			close(readHandle);
		}
		if (0 <= writeHandle) {
			close(writeHandle);
		}
	}

	/**** メッセージ送信
	 */
	void Comm::Send(Message& m) {
		try {
			m.Send(writeHandle);
		} catch (...) { delete this; }
	}

	/** メッセージを受信してはObject::OnMessageへ流す
	 */
	void Comm::Run() {
		try {
			for (;;) {
				Message m(readHandle);
				OnMessage(m);
			}
		} catch (...) { delete this; }
	}

}
