#ifndef LOVE_EVENT_EVENTDEFS_H
#define LOVE_EVENT_EVENTDEFS_H

namespace love
{
namespace event
{
	typedef enum {
		LOVE_NOEVENT = 0,
		LOVE_KEYDOWN,
		LOVE_KEYUP,
		LOVE_MOUSEMOTION,
		LOVE_MOUSEBUTTONDOWN,
		LOVE_MOUSEBUTTONUP,

		LOVE_NUMEVENTS = 32

	} LEvent;

} // event
} // love

#endif // LOVE_EVENT_EVENTDEFS_H