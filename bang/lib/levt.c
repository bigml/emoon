#include "mheads.h"
#include "lheads.h"

NEOERR* levt_on(SDL_Event e, bool *running)
{
    mentity_camera_control_orbit((CameraEntity*)mentity_node_get("camera"), e);

    return STATUS_OK;
}
