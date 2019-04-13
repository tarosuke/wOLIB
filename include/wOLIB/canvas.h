/** canvas関連の共通定義
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

#include "message.h"


namespace wO{

	class Canvas{
	public:
		//属性定義 NOTE:wO::Widgetの属性と重ならないようにすること
		static const unsigned attrMask = 0x0000000f;
		static const unsigned transparent = 1;
		static const unsigned cairoImage = 2;

		//ミップマップ使用フラグ
		static const unsigned mitmapMask = 0x00ff0000;
		static const unsigned mitmap0 = 0x00010000; //オリジナルサイズ
		static const unsigned mitmap1 = 0x00020000; // 1/2
		static const unsigned mitmap2 = 0x00040000; // 1/4
		static const unsigned mitmap3 = 0x00080000; // 1/8

	protected:
		struct Pack{
			Message::Head head;
			int left;
			int top;
			unsigned width;
			unsigned height;
			unsigned char buff[];
		}__attribute__((packed));
		static Pack& GetPack(Message&);


		class UpdateMessage : public wO::Message{
			UpdateMessage();
			UpdateMessage(const UpdateMessage&);
			void operator=(const UpdateMessage&);
		public:
			explicit UpdateMessage(
				int x,
				int y,
				unsigned width,
				unsigned height);
			Pack& GetPack(){ return *(Pack*)packBody; };
		private:
			unsigned char packBody[maxLen];
		};

	};

}
