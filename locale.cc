/** ロケールサポート
 * ロケールの値を保持したりデータファイルをロケールに従って探したりする
 */

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <wOLIB/locale.h>

#include <wOLIB/debug.h>



namespace{
	//パス前半
	static const char* const bases[] = {
		"./resource/",
		"../resource/",
		"~/.local/share/wOCE/",
		"/usr/local/share/wOCE/",
		0
	};

	//探索パス全て
	static const unsigned numOfBases((sizeof(bases) / sizeof(bases[0])) - 1);
	static const unsigned numOfPathes(4 * numOfBases);
	TB::String searchPath[numOfPathes];
}

namespace wO{
	void Locale::PreparePathes(){
		static bool inited(false);

		if(inited){
			return;
		}

		//ロケール文字列を準備
		TB::String locale[4];
		locale[0] = getenv("LANG");
		locale[1] = locale[0].SubStr(0, 5);
		locale[2] = locale[0].SubStr(0, 2);
		locale[3] = ".";


		//探索パス生成
		unsigned pid(0);
		for(unsigned ph(0); ph < numOfBases; ++ph){
			for(unsigned ah(0); ah < 4; ++ah, ++pid){
				TB::String& p(searchPath[pid]);
				p += bases[ph];
				p += locale[ah] + "/";
			}
		}

		//初期化完了
		inited = true;
	}

	int Locale::Open(const char* fn){
		if(!fn){ return -1; }

		//初期化
		PreparePathes();

		//順番に開いてみる
		for(unsigned n(0); n < numOfPathes; ++n){
			TB::String p(searchPath[n] + fn);
			const int f(open(p, O_RDONLY));
			if(0 <= f){
				//開けたので戻る
				return f;
			}
		}

		return -1;
	}

	TB::String Locale::FindPath(const char* fn){
		TB::String path;
		if(!fn){ return path; }

		//初期化
		PreparePathes();

		//順番にチェックしてみる
		for(unsigned n(0); n < numOfPathes; ++n){
			TB::String p(searchPath[n] + fn);
			struct stat st;
			if(0 <= stat(p, &st)){
				if(st.st_mode & S_IFREG){
					return p;
				}
			}
		}

		return path;
	}


}
