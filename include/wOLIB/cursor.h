#pragma once

#include <toolbox/gl/texture.h>

#include <wOLIB/widget.h>

#include "message.h"


namespace wO{

	class Cursor{
		Cursor(const Cursor&);
		void operator=(const Cursor&);
	public:
		enum State{
			inProgress,
			normal,
			sightNormal,
			watching,
			ibeam,
			notAllowed,
			move,
			moveHorizontal,
			moveVertical,
			resize,
			resizeHorizontal,
			resizeVertical,
		};
		struct Pack{
			wO::Message::Head head;
			unsigned width;
			unsigned height;
			unsigned size;
			unsigned bpp;
			unsigned attributes;
		}__attribute__((packed));
		struct ChooserPack{
			wO::Message::Head head;
			unsigned device; //0:mouse/1:sight
		}__attribute__((packed));
	protected:
		Cursor(){};
		class Message : public ::wO::Message{
			Message();
			Message(const Message&);
			void operator=(const Message&);
		public:
			Message(
				unsigned size,
				unsigned width,
				unsigned height,
				unsigned depth,
				unsigned attributes);
		private:
			Cursor::Pack pack;
		};
	private:
	};

	class CursorSet : public Cursor{
		CursorSet(const CursorSet&);
		void operator=(const CursorSet&);
	public:
		CursorSet(){};
	};

}
