/**
 * Copyright (C) 2017 tarosuke<webmaster@tarosuke.net>
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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include <wOLIB/comm.h>
#include <wOLIB/debug.h>
#include <wOLIB/message.h>


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
	void Comm::Send(const Message& m) {
		try {
			m.Send(writeHandle);
		} catch (...) { Close(); }
	}

	/** メッセージを受信しては子クラスのOnMessageへ送る
	 */
	void Comm::Run() {
		try {
			while (Message* const m = new ReceivedMessage(readHandle)) {
				OnMessage(*m);
				delete m;
			}
		} catch (...) { Close(); }
	}

}
