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
#pragma once

#include <toolbox/container/list.h>
#include <toolbox/container/table.h>
#include <wOLIB/comm.h>
#include <wOLIB/message.h>



namespace wO {

	struct Object : private TB::TableBase::Node, TB::List<Object>::Node {
		Object() = delete;
		Object(Comm&); // Commの先にペアを作成
		Object(Comm&, unsigned id); // ペア作成のためにID指定

		void Send(Message&); // ペアオブジェクトに届く
		static void OnMessage(Message&); // IDのObjectへOnMessage

	protected:
		virtual void OnMessage(Message::Packet&){};
		virtual ~Object(){};

	private:
		static TB::Table<Object> table;
		Comm& comm;
		bool byed;
	};
}
