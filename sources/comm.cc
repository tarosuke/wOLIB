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
#include <assert.h>
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

	/**** メッセージ送受信
	 * こんな薄いメソッドにしたのはCommがreadHandle/writeHandleを知っているため
	 */
	bool Comm::Send(const void* data, unsigned nElement) {
		const unsigned s(nElement * sizeof(u32));
		return write(writeHandle, data, s) == s;
	}
	void Comm::Hear(void* b, unsigned nElement) {
		const unsigned s(nElement * sizeof(u32));
		if (read(readHandle, b, s) != s) {
			throw -1;
		}
	}


	/***** AppとObject
	 */
	App::Object::Object(unsigned id, App& app) : id(id), app(app) {
		app.Register(id, *this);
	};
	App::Object::~Object() {
		HeadMessage(Message::destroy, id).Send(app);
		app.Unregister(id);
	}

	App::Object* App::operator[](unsigned id) {
		assert(id < nodes.size());
		return nodes[id].object;
	}
	void App::Register(unsigned id, Object& o) {
		assert(id <= nodes.size());
		if (nodes.size() == id) {
			nodes.emplace_back(id);
		}
		nodes[id].object = &o;
	}
	void App::Unregister(unsigned id) { nodes[id].object = 0; }

	void App::Hear() {
		while (Message* m = new ReceivedMessage(*this)) {
			nodes[m->ID()].object->OnMessage(*m);
		}
	}



}
