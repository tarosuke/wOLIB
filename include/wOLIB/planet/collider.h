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

#include <toolbox/container/list.h>
#include <toolbox/geometry/vector.h>



namespace wO{
	namespace Planet{

		/** 衝突計算用の形状
		 * NOTE:これはinterfaceなので具体的な形状はこれを導出して作る
		 */
		class Collider : public TB::List<Collider>{
			Collider();
			Collider(const Collider&);
			void operator=(const Collider&);
		public:

			/** 衝突処理
			 * 衝突を検出し、衝突していたら位置、速度を更新する
			 */
			virtual void Collide(Collider&)=0;

		protected:
			Collider(const TB::Vector<float, 3>& p) : position(p){};
			//TODO:この辺に物理の設定を書く

		private:
			TB::Vector<float, 3> position;
		};


		/** 仮コライダ
		 */
		class FakeCollider : public Collider{
		public:
			FakeCollider() : Collider(TB::Vector<float, 3>()){};
		private:
			void Collide(Collider&) final{};
		};

	}
}
