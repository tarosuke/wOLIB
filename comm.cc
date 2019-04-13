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

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>

#include <wOLIB/comm.h>
#include <wOLIB/message.h>
#include <wOLIB/widget.h>
#include <wOLIB/debug.h>


namespace wO{

	Comm::~Comm(){
		if(0 <= readHandle){ close(readHandle); }
		if(0 <= writeHandle){ close(writeHandle); }
	}

	bool Comm::Send(const void* buff, unsigned bytes){
		return !bytes || (write(writeHandle, buff, bytes) == (int)bytes);
	}

	bool Comm::Send(const Message& m){
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		Message::Pack& p(m);
		p.head.timestamp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
		return Send(&p, p.head.len + sizeof(Message::Head));
	}

	bool Comm::Receive(void* buff, unsigned bytes){
		return !bytes || (read(readHandle, buff, bytes) == (int)bytes);
	}

	/** メッセージ受信
	 */
	Message* Comm::ReceiveMessage(){
		//領域確保
		Message* m(new ReceivedMessage);
		if(!m){
			DPUTS("メッセージメモリ確保失敗。");
			return 0;
		}
		Message::Pack& p(*m);

		//ヘッダ待ち
		if(!Receive(&p, sizeof(Message::Head))){
			DPUTS("メッセージ(ヘッダ)受信失敗。");
			delete m;
			return 0;
		};

		//ヘッダのエンディアン検出、変換
		bool reverseEndian(false);
		switch(p.head.type & 0x8080){
		case 0x8000: //順エンディアン
			break;
		case 0x0080: //逆エンディアン
			reverseEndian = true;
			p.head.len = __bswap_16(p.head.len);
			p.head.type = __bswap_16(p.head.type);
			p.head.id = __bswap_32(p.head.id);
			break;
		default: //不正値
			DPRINTF("不正なメッセージ(エンディアン不明;%08x)を受信。", p.head.type);
			delete m;
			return 0;
		}

		//コンテント長チェック
		if(Message::maxLen < sizeof(Message::Head) + p.head.len){
			DPRINTF("不正なメッセージ(長過ぎる:%u)を受信。", p.head.len);
			delete m;
			return 0;
		}

		//コンテント読み込み
		if(!Receive(p.body, p.head.len)){
			DPUTS("メッセージ(本体)受信失敗。");
			delete m;
			return 0;
		}

		//必要ならエンディアン変換
		if(reverseEndian){
			unsigned reverseLen(p.head.len);
			switch(p.head.type){
			case Message::canvasUpdated:
				reverseLen = sizeof(Widget::CommandPack) - sizeof(Message::Head);
				break;
			case Message::spawn:
				reverseLen = 0;
				break;
			}
			for(unsigned* b((unsigned*)p.body);
				b < (unsigned*)&p.body[reverseLen]; ++b){
				*b = __bswap_32(*b);
			}
		}

		return m;
	}

	/** メッセージを受信しては子クラスのOnMessageへ送る
	 * 問題があれば戻る
	 */
	void Comm::Run(){
		while(Message* const m = ReceiveMessage()){
			OnMessage(*m);
			delete m;
		}
	}

}
