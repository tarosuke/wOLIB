/*
 * Copyright (C) 2023 tarosuke<webmaster@tarosuke.net>
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
#include <wOLIB/message.h>
#include <wOLIB/object.h>



namespace wO {

	TB::Table<Object> Object::table;

	Object::Object(Comm& comm) : TB::TableBase::Node(table), comm(comm) {
		comm.Register(*this);
	}
	Object::Object(Comm& comm, unsigned id)
		: TB::TableBase::Node(table, id), comm(comm) {
		comm.Register(*this);
	}

	void Object::OnMessage(Message& m) {
		Message::Packet& p(m);
		Object* const o(dynamic_cast<Object*>(table[p.head.id]));
		if (o) {
			o->OnMessage(p);
		}
	}
}
