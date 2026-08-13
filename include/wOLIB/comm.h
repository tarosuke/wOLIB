/***** Comm : 通信の端点とObjectの保持
 * wODM側ではsshを起動して繋げる
 * wOApp側ではtb::Appと多重継承
 *
 * Copyright (C) 2017,2025, 2026 tarosuke<webmaster@tarosuke.net>
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

#include "message.h"
#include <deque>
#include <tb/list.h>



namespace wO {

	struct Comm {
		Comm(int r = 0, int w = 1) : readHandle(r), writeHandle(w) {};
		virtual ~Comm();

		bool Send(const void*, unsigned nElement);
		void Hear(void*, unsigned nElement);

	private:
		const int readHandle;
		const int writeHandle;

		Comm(const Comm&) = delete;
		void operator=(const Comm&) = delete;
	};


	struct App : Comm {
		/***** サーバ / クライアント間で通信するObject
		 * メッセージを送信するときはSend、受け取ったメッセージはmessagesに追加される
		 */
		struct Object {
			const unsigned id;

			Object(unsigned id, App&);
			virtual ~Object();

			void Send(const Message& m) { m.Send(app); };
			void OnMessage(Message& m) { messages.Add(m); };

		private:
			App& app;
			tb::List<Message> messages;
		};

		App() = default;

		Object* operator[](unsigned);
		void Register(unsigned id, Object& o);
		virtual void Unregister(unsigned id);

		/***** Hear
		 * ブロッキングでメッセージ受信と配信
		 * NOTE:戻らないのでスレッドで並列化する必要がある
		 */
		void Hear();

	protected:
		struct Node {
			const unsigned id;
			Node* next;
			Object* object;
			explicit Node(unsigned id) : id(id) {};
			~Node() {
				if (object) {
					delete object;
				}
			}

			Node() = delete;
			Node(const Node&) = delete;
			void operator=(const Node&) = delete;
		};

		std::deque<Node> nodes;
	};

}
