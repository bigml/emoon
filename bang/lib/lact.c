#include "mheads.h"
#include "lheads.h"

NEOERR* lact_init()
{
    return STATUS_OK;
}

NEOERR* lact_on(SDL_Event e, bool *running)
{
    mentity_camera_control_orbit((CameraEntity*)mentity_node_get("camera"), e);

    return STATUS_OK;
}

void lact_finish()
{
    ;
}
