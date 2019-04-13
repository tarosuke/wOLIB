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
 *
 * スレッドを使うときはTB::Threadと多重継承してTB::Thread::ThreadBodyでRunを呼ぶ
 */
#pragma once

class Resource;

namespace wO{
	class Message;

	/** 指定したハンドルで入出力する端末
	 */
	class Comm{
		Comm(const Comm&);
		void operator=(const Comm&);
	public:

		struct Handles{
			int read;
			int write;
		};

		Comm(const Handles& h) :
			readHandle(h.read), writeHandle(h.write){};
		Comm(int r = 0, int w = 1) : readHandle(r), writeHandle(w){};
		virtual ~Comm();

		bool Send(const void*, unsigned);
		bool Send(const Message&);

	protected:

		virtual bool OnMessage(Message&)=0;
		void Run();
		Message* ReceiveMessage();

	private:

		const int readHandle;
		const int writeHandle;

		bool Receive(void*, unsigned);
	};

}
