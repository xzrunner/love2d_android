#ifndef LOVE_MOUSE_MOUSEDEFS_H
#define LOVE_MOUSE_MOUSEDEFS_H

namespace love
{
namespace mouse
{
	typedef enum {
		LB_UNKNOWN = 0,
		LB_LEFT,
		LB_MIDDLE,
		LB_RIGHT,
		LB_WHEELUP,
		LB_WHEELDOWN,
		LB_X1,
		LB_X2,

		LB_LAST

	} LButton;

} // mouse
} // love

#endif // LOVE_MOUSE_MOUSEDEFS_H