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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <endian.h>
#include <string.h>
#include <syslog.h>

#include <wOLIB/message.h>
#include <wOLIB/comm.h>
#include <wOLIB/debug.h>


namespace wO{

	Message::Message(Pack* const p) :
		body(*p){};

	void Message::Dump() const{
		DPRINTF("type:%04x len:%d id:%08x time:%u.", body.head.type, body.head.len, body.head.id, body.head.timestamp);
		fprintf(stderr, "type:%04x len:%d id:%08x time:%u.", body.head.type, body.head.len, body.head.id, body.head.timestamp);
		for(unsigned n(0); n < body.head.len;){
			fprintf(stderr, "\n%04x: ", n);
			do{
				fprintf(stderr, "%02x ", body.body[n++]);
			}while(n < body.head.len && !!(n & 15));
		}
		fputs("\n", stderr);
	}


	Message::Head HeloMessage::heloHead = {
		0, Message::helo, 0
	};
	Message::Head ByeMessage::byeHead = {
		0, Message::bye, 0
	};

	SpawnMessage::SpawnMessage(const char* path){
		Pack& pack(*this);
		pack.head.type = spawn;
		pack.head.len = strlen(path) + 1;
		if(sizeof(pack.head) + pack.head.len < maxLen){
			strcpy((char*)pack.body, path);
		}else{
			syslog(LOG_ERR, "too long path:%s", path);
			pack.body[0] = 0; //空文字列なら無視される
		}
	}
}
