
# include <endian.h>

#include <wOLIB/canvas.h>
#include <wOLIB/comm.h>


namespace wO{

	Canvas::Pack& Canvas::GetPack(Message& m){
		return *(Pack*)&m.GetContent();
	};

	Canvas::UpdateMessage::UpdateMessage(
		int x,
		int y,
		unsigned width,
		unsigned height) : Message((Message::Pack*)packBody){
		Canvas::Pack& p(*(Canvas::Pack*)packBody);
		p.head.type = Message::canvasUpdated;
		p.head.len = sizeof(Canvas::Pack) - sizeof(Head);
		p.left = x;
		p.top = y;
		p.width = width;
		p.height = height;
	}

}

