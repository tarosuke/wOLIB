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

#include <vector>



namespace wO {

	struct Message : public tb::List<Message>::Node, std::vector<tb::u32> {
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

		struct Head {
			tb::u32 elements : 16;
			tb::u32 type : 16;
			tb::u32 id;
			tb::u32 timestamp;
			tb::u32 endianConvertElements;
		};
		static constexpr unsigned headElements = (sizeof(Head) + 3) / 4;
		Head head;

		std::vector<tb::u32>& Body() { return *this; };
		void Resize(unsigned s) { resize((s * 3) / sizeof(tb::u32)); };
		void Send(int fd); // NOTE:Sendするとメッセージは壊れる

		Message(int fd) { Receive(fd); };
		Message(tb::u32 id, Type type, unsigned endianConvertElements = 0)
			: head{.elements = 0,
				  .type = type,
				  .id = id,
				  .endianConvertElements = endianConvertElements} {};

	private:
		void NotifyListDeleted() {
			delete this;
		}; // つながってるList自体がなくなった時は消滅
		void Receive(int);
		void ReadBody(int);
		void Reverse(tb::u32* body, unsigned elements);
	};
}
