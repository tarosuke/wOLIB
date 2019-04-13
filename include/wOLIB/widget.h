/** Widget
 * PanelとWindowの元締、親子関係を制御
 */
#pragma once

#include "message.h"


namespace wO{

	/** WidgetNode
	 * C++で多重継承した時、同名の仮想メソッドの親を指定する方法がないので
	 * 多重継承させる前に一度クラスを作ってそこで実装しておくためだけのクラス
	 */
	template<class T> class WidgetNode : public TB::List<T>::Node{
		WidgetNode(const WidgetNode&);
		void operator=(const WidgetNode&);
	protected:
		WidgetNode(){};

	private:
		//親Widgetがdeleteするとき自動deleteする設定
		void NotifyListDeleted() final { delete this; };
	};


	/** Widget
	 * Widget関連の定義
	 */
	class Widget{
		Widget(const Widget&);
		void operator=(const Widget&);
	public:

		/** 特殊ID
		 */
		static const unsigned invalidParentID = ~0U;


		/** 属性定義
		 */
		//Canvasで予約
		static const unsigned reservedForCanvas = 0x00ff000f;

		//挙動、機能に関する属性
		static const unsigned hasPosition = 0x00000010; //場所あり
		static const unsigned hasBorder = 0x00000020; //境界あり
		static const unsigned hasContent = 0x00000040; //中身あり
		static const unsigned noFocusMe = 0x00000080; //フォーカス時に自身を指定しない
		static const unsigned autoDepth = 0x00000100; //ContainerWidget

		//描画に関する属性
		static const unsigned dockItem = 0x00000200; //Dockに追加
		static const unsigned druggingContext = 0x00000400; //ドラッグ中のもの
		static const unsigned backboard =  0x00000800; //背後に板

		//移動に関する属性 NOTE:0の場合は全座標軸が有効
		static const unsigned moveAxisMask = 0x00007000;
		static const unsigned moveXAxis = 0x00001000; //X
		static const unsigned moveYAxis = 0x00002000; //Y
		static const unsigned moveZAxis = 0x00004000; //Z

		//(ドラッグ＆ドロップの)ドロッププラグ
		static const unsigned dropFlag = 0x80000000; //drop if nonzero

		//カーソル設定専用
		static const unsigned useSight = 0x00000001;
		static const unsigned repeatWatch = 0x00000002;

		//モディファイアキー
		static const unsigned shiftL = 0x01;
		static const unsigned shiftR = 0x02;
		static const unsigned ctrlL = 0x04;
		static const unsigned ctrlR = 0x08;
		static const unsigned altL = 0x10;
		static const unsigned altR = 0x20;
		static const unsigned caps = 0x40;
		static const unsigned shift = (shiftL | shiftR);
		static const unsigned ctrl = (ctrlL | ctrlR);
		static const unsigned alt = (altL | altR);

		/** メッセージ定義
		 */
		struct CommandPack{
			Message::Head head;
			unsigned parent;
			unsigned attributes;
			int left;
			int top;
			int depth; //NOTE:奥行き
			unsigned width;
			unsigned height;
			unsigned color;
		}__attribute__((packed));
		struct CursorPack{
			Message::Head head;
			unsigned state; //see cursor.h
			unsigned preWatchTime; //in ms,
			unsigned watchTime; //in ms,
			unsigned attributes;
		}__attribute__((packed));
		struct EventPack{
			Message::Head head;
			int x;
			int y;
			unsigned pressed;
			unsigned released;
			unsigned buttonState; //MSBはドロップフラグ
		}__attribute__((packed));
		struct KeyPack{
			Message::Head head;
			unsigned modifiers;
			unsigned keyCode;
			unsigned charCode;
		}__attribute__((packed));

	protected:
		Widget(){};
		virtual ~Widget(){};

	private:
	};

}
