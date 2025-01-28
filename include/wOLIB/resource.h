/** Copyright (C) 2024,2025 tarosuke<webmaster@tarosuke.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR Master PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#pragma once

#include <assert.h>
#include <tb/key.h>
#include <tb/types.h>
#include <vector>



namespace wO {

	template <class T, typename I, bool Master, class L = tb::NullLock>
	struct Resource {
		static constexpr I invalidID = ~(I)0;
		struct Node {
			Node() = delete;
			Node(const Node&) = delete;
			void operator=(const Node&) = delete;

			Node(Resource& t, I id = invalidID) : table(&t), id(id) {
				if constexpr (Master) {
					id = t.Assign();
				}
				T* const target(dynamic_cast<T*>(this));
				assert(target);
				t.table[id].Attach(*target);
			};
			virtual ~Node() {
				table->table[id].Detach(*table, id);
				table->Release(id);
			};
			virtual void NotifyTableDeleted() {};

			I ID() const { return id; };

		private:
			Resource* table;
			I id;
		};

		Resource() : pool(~(I)0) {};
		T* operator[](I id) { return table[id]; };

	private:
		struct N {
			void Attach(T& t) { target = &t; };
			void Detach(Resource& r, I p) {
				target = 0;
				next = r.pool;
				r.pool = p;
			};

			N() : target(0) {};
			virtual ~N() {
				if (target) {
					((Node*)target)->NotifyTableDeleted();
				}
			};
			operator T*() { return target; };

		private:
			T* target;
			I next;
		};
		std::vector<N> table;
		I pool;


		I Assign() {
			static_assert(Master);
			if (~pool) {
				// スタックからリサイクル
				const I id(pool);
				T& t(table[id]);
				pool = t.next;
				return id;
			} else {
				// スタックが空なので新規割当
				const I id(table.size());
				table.resize(id + 1);
				return id;
			}
		};

		void Release(I i) {
			static_assert(Master);
			pool = i;
		};
	};
}
