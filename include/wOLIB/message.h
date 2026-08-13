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



namespace wO {

	struct Comm;

	/***** メッセージ
	 * メッセージはAppとDisplayの間の通信で、ステートレスになるよう設計される
	 */
	struct Message : public tb::List<Message>::Node {
		Message() = delete;
		Message(const Message&) = delete;
		void operator=(const Message&) = delete;

		/* メッセージID
		 * エンディアン検出のため最上位ビットが1
		 */
		static constexpr tb::u16 typeSystem = 0x8000;
		static constexpr tb::u16 typeObject = 0x8010;
		static constexpr tb::u16 typeDocument = 0x8020;
		static constexpr tb::u16 typeWidget = 0x8030;
		static constexpr tb::u16 typeDraw = 0x8100;
		static constexpr tb::u16 typeExtention = 0xffff;
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

			/** Qbject一般
			 * App <-> Display
			 */
			destroy = typeObject, // そのオブジェクトを破棄する


			/** 書類管理
			 * App -> Display
			 */
			markDirty = typeDocument, // 書類を「変更あり」にマーク
			clearDirty,				  // 「変更あり」をクリア

			/** Widget関連
			 * Display -> App
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
			 * App -> Display
			 */
			updateTile = typeDraw, // タイルのアップデート
			setCursorType,		   // Widget内でのカーソルの種類を設定
			// 他、個別の描画指令

			/**** 拡張メッセージ
			 * 拡張メッセージはデータの先頭がUUIDになっていて、そこで種別が判定
			 * される。これは「本家」とは無関係にメッセージを拡張できるようにす
			 * るためにある。
			 */
			uuid = typeExtention,
		};
		static constexpr unsigned maxElements = 32768 / sizeof(unsigned);

		tb::u32 Elements() const { return pack.nElement; };
		tb::u32 Type() const { return pack.type; };
		tb::u32 ID() const { return pack.id; };

		void Send(Comm&) const;

	protected:
		struct Pack {
			tb::u32 nElement : 16; // ヘッダ抜きの4bytes単位
			tb::u32 type : 16;
			tb::u32 id;
			tb::u32 endianConvertElements;
			tb::u32 body[0];
		} & pack;
		static constexpr unsigned packNElement =
			(sizeof(Pack) / sizeof(tb::u32));

		Message(Pack& p) : pack(p) {};

	private:
		void NotifyListDeleted() {
			delete this;
		}; // つながってるList自体がなくなった時は消滅
	};

	struct BufferdMessage : public Message {
	protected:
		BufferdMessage() : Message(buffer.pack) {};

	private:
		union {
			Pack pack;
			tb::u32 raw[maxElements];
		} buffer;
	};

	struct ReceivedMessage : public BufferdMessage {
		ReceivedMessage(Comm&); // Commからヘッダ、ボディの順に受信
	private:
		void Hear(Comm&);
		void Reverse(tb::u32* body, unsigned elements);
	};

	struct HeadMessage : public Message {
		HeadMessage(tb::u32 type, tb::u32 id) :
			Message(pack),
			pack{.nElement = 0,
				.type = type,
				.id = id,
				.endianConvertElements = 0} {}

	private:
		Pack pack;
	};
}
