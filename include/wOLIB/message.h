/****************************************************************** Message
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
 */

#pragma once

#include <toolbox/container/list.h>



namespace wO{

	class Message : public TB::List<Message>::Node{
		Message();
		Message(const Message&);
	public:
		static const unsigned typeMask = 0xff00;
		static const unsigned typesToApp = 0x8100;
		static const unsigned typesToCanvas = 0x8200;
		static const unsigned typePlanet = 0x8300;
		static const unsigned typePrefs = 0x8400;
		enum Types{
			/** システムメッセージ
			 */
			helo = 0x8000,
			commID,
			bye,
			disconnected,
			disposed, //Resourceのデストラクタが送る自動消滅
			enterProgress, //処理開始(カーソルをprogressにする)
			exitProgress, //処理終了
			spawn, //プロセス起動

			/**メッセージ
			 */
			widgetCreated,		//親なしWidgetの場合
			parentChanged, //親が変更された時
			ChildWidgetCreated, //親に対して送られる
			moveTo, //補間移動
			jumpTo, //即時移動
			move, //相対補完移動
			jump, //相対即時移動
			focus, //フォーカスを取得
			pick, //対象とそれを含むWidgetを一番手前へ
			show, //可視化
			hide, //不可視化
			setCursor, //Widgetのカーソルを設定

			/** カーソルメッセージ
			 * cursorChooseはカーソル位置として使うデバイスを選択する
			 * タブレットは有限なので使わない(代わりにControlにマップして使う)
			 */
			cursorsetCreated,
			cursorChoose,

			/** コマンド
			 */
			logout, //wODMとしては終了
			stickCenter, //中央リセット用)
			lockCenter,
			closeRequest, //非推奨
			//状態変化通知
			cursorActivated,

			/** DM -> App
			 * 画面からAppへのメッセージ
			 * イベントと設定一覧
			 */
			//イベント
			onMouseEnter = typesToApp,
			onMouseLeave,
			onMouseMove,
			onMouseButton, //何もないところだとCursorSetに送られる
			onScroll,
			onSightEnter,
			onSightMove,
			onSightLeave,
			onWatched,
			onGrabStart,
			onGrabMove,
			onGrabEnd,
			onKeyDown, //onKeyUpなしで連続して送られた場合、キーリピート
			onKeyRepeat, //repeat用予約
			onKeyUp,
			onUnfocused,
			onResized,

			/** 設定用メッセージ(双方向)
			 * 内容は設定パスと型、内容
			 * グループを使う時は設定パスを使う(設定自体は独立だが設定ツールが解釈する)
			 * 画面側は設定パスを「wODM/」で始めること
			 * prefItemメッセージに最後フラグが立っていたら転送終了
			 * 画面からの場合は設定項目と現在の設定
			 * サーバからの場合は設定
			 * 値は文字列で
			 */
			prefStart = typePrefs, //設定アプリ起動リクエスト->wOSH
			prefRequest, //設定情報リクエスト
			prefItem, //設定情報

			/** canvas
			 * Canvasのためのメッセージ
			 */
			canvasUpdated = typesToCanvas,

			/** planet関連メッセージ
			 * さらにplanetメッセージタイプで区別する
			 */
			planetCreated = typePlanet,
			createScenery, //背景
			sceneryActivated,
			roomCreated,
			materialCreated,
			avatarCreated, //Roomに対して送られる
			newPanel,
			floorMaterial,
			ceilMaterial,
			floorVertex,
			ceilVertex,
			drawRoom,
			//Avatar
			newAvatar,
			attachPlayerGhost,

			/** development
			 * You can use 0xff00 - 0xff7f for development
			 * Issue PULL-REQUEST before you release it.
			 * We'll assign message number and merge it.
			 * 0xff00 - 0xff7fの範囲は開発用に使えます。
			 * 番号を与えて組み込むのでプルリクエストしてください。
			 */
			development = 0xff00,
		};
		static const unsigned maxLen = 32768;
		struct Head{
			unsigned short len; //lenはmaxLen以下なのでMSBは0
			unsigned short type; //typeのMSBは1
			unsigned id;
			unsigned timestamp;
		}__attribute__((packed));;
		struct Pack{
			Head head;
			unsigned char body[];
		}__attribute__((packed));

		/** 内容の取得
		 */
		Pack& GetContent() const { return body; };
		template<typename T> operator T&() const { return *(T*)&body; };

		/** 内容のダンプ
		 */
		void Dump() const;

		/** メッセージ生成
		 */
		Message(Pack* const);

	protected:
		Pack& body;

	private:
		void NotifyListDeleted(){ delete this; }; //つながってるList自体がなくなった時は消滅
	};

	/** 受信用メッセージ
	 */
	class ReceivedMessage : public Message{
		ReceivedMessage(const ReceivedMessage&);
		void operator=(const ReceivedMessage&);
	public:
		ReceivedMessage() : Message((Pack*)buff){};
	private:
		unsigned buff[(maxLen + sizeof(unsigned) - 1) / sizeof(unsigned)];
	};

	/** len=0のメッセージ
	 */
	class HeadMessage : public Message{
		HeadMessage();
		HeadMessage(const HeadMessage&);
		void operator=(const HeadMessage&);
	public:
		HeadMessage(Types type) : Message(&pack){
			pack.head.len = 0;
			pack.head.type = type;
		};
	private:
		Pack pack;
	};

 	/** helo送信用メッセージ
 	 */
	class HeloMessage : public Message{
		HeloMessage(const HeloMessage&);
		void operator=(const HeloMessage&);
	public:
		HeloMessage() : Message((Pack*)&heloHead){};
	private:
		static Head heloHead;
	};
 	/** bye送信用メッセージ
 	 */
	class ByeMessage : public Message{
		ByeMessage(const ByeMessage&);
		void operator=(const ByeMessage&);
	public:
		ByeMessage() : Message((Pack*)&byeHead){};
	private:
		static Head byeHead;
	};
	/** spawnメッセージ
	 */
	class SpawnMessage : public ReceivedMessage{
		SpawnMessage();
		SpawnMessage(const SpawnMessage&);
		void operator=(const SpawnMessage&);
	public:
		SpawnMessage(const char*);
	};

}
