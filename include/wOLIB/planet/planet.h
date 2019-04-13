/** Planet
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

#include "collider.h"
#include "../message.h"



namespace wO{

	namespace Planet{

		struct Pack{

			struct Border{
				struct{
					float a;
					float b;
					float c;
					float d;
				}u, v;
				unsigned transparent;
				char label[];
			};


			wO::Message::Head head;
			union{
				/** マテリアル生成
				* 座標などはscale倍、uvはuvScale倍の値
				*/
				struct{
					unsigned id;
					unsigned width;
					unsigned height;
					unsigned bpp;
					unsigned attr;
					char label[0];
				}newMaterial;
				/** 部屋生成
				 */
				struct{
					unsigned id;
					float floor;
					float ceil;
					char label[];
				}newRoom;
				/** 部屋の角
				 * 上から見て時計回りの順に送られる
				 */
				struct{
					struct{
						float x;
						float y;
						float z;
					}from, to;
					Border border;
				}newPanel;
				struct{
					struct{
						float x;
						float y;
					}vertex[];
				}horizontalVertex;
				struct{
					Border border;
				}horizontalMaterial;
				/** Avatar生成
				 */
				struct{
					unsigned id;
					struct{
						float x;
						float y;
						float z;
					}position, cameraOffset;
					float azimuth;
				}newAvatar;
			};
		};


		class Object : public TB::List<Object>::Node{
			Object(const Object&);
			void operator=(const Object&);
		public:

			/** Object生成
			 */
			Object(){};
			Object(Collider& b){};

			/** Objectの移動処理とObject同士の衝突処理
			 * NOTE:これはインターフェイスだけの仮実装
			 */
			virtual void Collide(Object&){};
			virtual void Update(float){};

			/** 描画
			 */
			virtual void Draw(){};

		protected:

		};

	};
}
