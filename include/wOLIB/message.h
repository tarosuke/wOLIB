/****************************************************************** Message
 * Copyright (C) 2017, 2023 tarosuke<webmaster@tarosuke.net>
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
#include <toolbox/type.h>

#include <wOLIB/feature.h>



namespace wO {

	struct Message : public TB::List<Message>::Node {
		Message() = delete;
		Message(const Message&) = delete;
		void operator=(const Message&) = delete;

		/* メッセージID
		 * エンディアン検出のため最上位ビットが1
		 */
		static constexpr u16 typeSystem = 0x8900;
		static constexpr u16 typeWidget = 0x8100;
		static constexpr u16 typeDraw = 0x8200;
		enum Type : u16 {
			/** システムメッセージ
			 */
			helo = typeSystem,
			commID,
			bye, // 切断予告
			disconnected, // 切断された場合にwOSH / wODMが通知
			spawn, // 新たに何かを開くとき
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

		struct Head {
			u32 len : 16;
			u32 type : 16;
			u32 id;
			u32 timustamp;
			u32 endianConvertElements;
		};
		struct Packet {
			Head head;
			u32 body[0];
		};

		void Send(int fd);

	protected:
		Message(
			Packet& pack,
			Type type,
			unsigned id,
			unsigned elements,
			unsigned endianConvertElements)
			: pack(pack) {
			pack.head.type = type;
			pack.head.len = elements;
			pack.head.id = id;
			pack.head.timustamp = GetTimestamp();
			pack.head.endianConvertElements = endianConvertElements;
		};
		Message(Packet& pack) : pack(pack){};
		virtual ~Message(){};

	private:
		Packet& pack;
		void NotifyListDeleted() {
			delete this;
		}; // つながってるList自体がなくなった時は消滅
		static unsigned GetTimestamp();
	};



	/** 受信用メッセージ
	 */
	struct ReceivedMessage : public Message {
		ReceivedMessage() : Message(pack.pack){};
		ReceivedMessage(int fd); // fdからをread

	protected:
		union Pack {
			Packet pack;
			u32 raw[maxElements];
		} pack;

		static void Reverse(u32*, unsigned elements);
		void ReadBody(int);
	};

	/** bodyなしメッセージ
	 */
	struct HeadMessage : public Message {
		HeadMessage(Type type, unsigned id) : Message(pack, type, id, 0, 0){};

	private:
		Packet pack;
	};

	/***** 汎用bodyありメッセージ
	 */
	template <typename T> struct SomeMessage : public Message {
		SomeMessage(
			Packet& pack,
			Type type,
			unsigned id,
			unsigned endianConvertElements)
			: Message(pack, type, id, Elements(), endianConvertElements){};

	private:
		constexpr u16 Elements() {
			return (sizeof(T) + sizeof(u32) - 1) / sizeof(u32);
		};
	};

	/** helo送信用メッセージ
	 */
	struct HeloMessage : public SomeMessage<Features> {
		HeloMessage(const Features& helo, unsigned id)
			: SomeMessage(pack.pack, Message::helo, id, 1){};

	private:
		struct {
			Packet pack;
			Features features;
		} pack;
	};
	/** bye送信用メッセージ
	 */
	struct ByeMessage : public HeadMessage {
		ByeMessage(unsigned id) : HeadMessage(Message::bye, id){};
	};
	/***** spawnメッセージ
	 * pathが指す対象にIntentを適用し、起動したプロセスをidに結びつける
	 */
	struct SpawnMessage : public ReceivedMessage {
		enum Intent : u8 {
			open,
		};
		SpawnMessage(u32 id, Intent, const char* path);

	private:
		struct Body {
			u32 id;
			Intent intent;
			char* target;
		} __attribute__((packed))* const body;
	};
}
