/* Copyright (C) 2017,2023,2025 tarosuke<webmaster@tarosuke.net>
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

#include <tb/list.h>
#include <tb/types.h>

#include <wOLIB/feature.h>
#include <wOLIB/object.h>



namespace wO {

	struct Message : public tb::List<Message>::Node {
		Message() = delete;
		Message(const Message&) = delete;
		void operator=(const Message&) = delete;

		/* メッセージID
		 * エンディアン検出のため最上位ビットが1
		 */
		static constexpr tb::u16 typeSystem = 0x8900;
		static constexpr tb::u16 typeWidget = 0x8100;
		static constexpr tb::u16 typeDraw = 0x8200;
		enum Type : tb::u16 {
			/** システムメッセージ
			 */
			helo = typeSystem,
			commID,
			newObject,	  // Qbject生成
			deleted,	  // Object消滅を通知
			bye,		  // 切断予告
			disconnected, // 切断された場合にwOSH / wODMが通知
			spawn,		  // 新たに何かを開くとき
			// 他、システムアラートなどを予定

			/** Widget関連
			 */
			onMouseEnter = typeWidget,
			onMouseLeave,
			onMouseMove,
			onMouseButton,
			onScroll,
			onSightEnter,
			onSightMove,
			onSightLeave,
			onKeyDown,
			onKeyRepeat, // repeat用予約
			onKeyUp,
			onFocused,
			onUnfocused,
			onResized,

			/** 描画関連
			 */
			updateTile = typeDraw, // タイルのアップデート
			setCursorType, // Widget内でのカーソルの種類を設定
						   // 他、個別の描画指令
		};
		static constexpr unsigned maxElements = 32768 / sizeof(unsigned);

		struct Pack {
			tb::u32 elements : 16;
			tb::u32 type : 16;
			tb::u32 id;
			tb::u32 timestamp;
			tb::u32 endianConvertElements;
			tb::u32 body[0];
		} & pack;
		static constexpr unsigned headElements = (sizeof(Pack) + 3) / 4;

		void Send(int fd); // NOTE:Sendするとメッセージは壊れる

		Message(Pack& p) : pack(p) {};

		tb::u32 Elements() const { return pack.elements; };
		tb::u32 Type() const { return pack.type; };
		tb::u32 ID() const { return pack.id; };

	protected:
		void Receive(int);

	private:
		void NotifyListDeleted() {
			delete this;
		}; // つながってるList自体がなくなった時は消滅
		void ReadBody(int);
		void Reverse(tb::u32* body, unsigned elements);
	};

	struct BufferdMessage : public Message {
	protected:
		BufferdMessage() : Message(pack.pack) {};

	private:
		union {
			Pack pack;
			tb::u32 raw[maxElements];
		} pack;
	};

	struct ReceivedMessage : public BufferdMessage {

		ReceivedMessage(int fd) { Receive(fd); };
	};

	struct HeadMessage : public Message {
		HeadMessage(tb::u32 type, tb::u32 id, tb::u32 nEce = 0)
			: Message(pack),
			  pack{.elements = 0,
				  .type = type,
				  .id = id,
				  .endianConvertElements = nEce} {}

	private:
		Pack pack;
	};


	struct NewObjectMessage : public Message {
		enum Type : tb::u32 {
			Pane,
		};
		struct Pack {
			Message::Pack pack;
			tb::u32 objectType;
		} pack;

		NewObjectMessage(Object::I id, Type objectType)
			: Message(pack.pack),
			  pack{.pack{.elements = 1,
					   .type = Message::newObject,
					   .id = id,
					   .endianConvertElements = 0},
				  .objectType = objectType} {};

	private:
	};
}
