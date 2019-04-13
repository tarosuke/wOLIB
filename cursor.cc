
#include <wOLIB/comm.h>

#include <wOLIB/cursor.h>


namespace wO{

	Cursor::Message::Message(
		unsigned size,
		unsigned width,
		unsigned height,
		unsigned bpp,
		unsigned attributes) :
		::wO::Message((::wO::Message::Pack*)&pack){
		pack.head.type = cursorsetCreated;
		pack.head.len = sizeof(Cursor::Pack) - sizeof(Head);
		pack.size = size;
		pack.width = width;
		pack.height = height;
		pack.bpp = bpp;
		pack.attributes = attributes;
	}

}
