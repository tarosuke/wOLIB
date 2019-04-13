/** common Scenery
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
 *
 * スカイスフィアやスカイボックスなどの抽象
 */
 #pragma once

 #include "../message.h"

namespace wO{
	namespace Scenery{
		//背景画像の解釈法
		static const unsigned mask = 0x00f00000;
		static const unsigned cubeMap = 0x01000000;
		static const unsigned sphereMap = 0x02000000;
		static const unsigned pylamitEncode = 0x03000000;
		static const unsigned domeMaster = 0x04000000;

		struct Pack{
			wO::Message::Head head;
			unsigned width;
			unsigned height;
			unsigned bpp;
			unsigned attributes;
		}__attribute__((packed));
	}
}
