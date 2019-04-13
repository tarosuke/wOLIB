/** ロケールサポート
 * ロケールの値を保持したりデータファイルをロケールに従って探したりする
 */
#pragma once

#include <toolbox/string.h>



namespace wO{

	class Locale{
		Locale();
		Locale(const Locale&);
		void operator=(const Locale&);
	public:
		/** Open
		 * 与えられたパスの後半にマッチするファイルを探して読み出しモードで開く
		 * 開けなかったら負数を返す(openと同じ)
		 */
		static int Open(const char*);

		/** FindPath
		 * パスを探す(なければ空文字列を返す)
		 */
		static TB::String FindPath(const char*);

	private:
		static void PreparePathes();
	};

}
