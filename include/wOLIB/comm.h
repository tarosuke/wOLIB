/** 通信ポート
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
#pragma once

#include <wOLIB/message.h>



namespace wO {

	struct Object;

	/** 指定したハンドルでMessageをやり取りするポート
	 */
	struct Comm {
		Comm(const Comm&) = delete;
		void operator=(const Comm&) = delete;

	public:
		struct Node {
			Node() = delete;
			Node(const Node&) = delete;
			void operator=(const Node&) = delete;

			Node(Comm& c) : comm(c) { c.Register(*this); };
			virtual ~Node() {};

		protected:
			void Send(Message&);

		private:
			Comm& comm;
		};


		Comm(int r = 0, int w = 1) : readHandle(r), writeHandle(w) {};
		virtual ~Comm();

		void Register(Object&);
		void Send(Message&);

	protected:
		virtual void Register(Node&) = 0;
		virtual void OnMessage(const Message&) = 0;

	private:
		const int readHandle;
		const int writeHandle;
		tb::List<Object> objects;
		void Run();

		bool Receive(void*, unsigned);
	};
}
