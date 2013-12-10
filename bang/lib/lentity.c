#include "mheads.h"
#include "lheads.h"

NEOERR* lentity_init()
{
    NEOERR *err;

    err = mentity_driver_regist(&car_entity_driver);
    if (err != STATUS_OK) return nerr_pass(err);

    return STATUS_OK;
}
