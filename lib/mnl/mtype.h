#ifndef __MTYPE_H__
#define __MTYPE_H__

#include "mheads.h"

__BEGIN_DECLS

typedef char  byte;
typedef float vec;
typedef vec   vec2[2];
typedef vec   vec3[2];

typedef enum {
    CNODE_TYPE_STRING = 100,
    CNODE_TYPE_BOOL,
    CNODE_TYPE_INT,
    CNODE_TYPE_INT64,
    CNODE_TYPE_FLOAT,
    CNODE_TYPE_DATETIME, /**< 8byte timestamp; milliseconds since Unix epoch */
    CNODE_TYPE_TIMESTAMP, /**< 4bytes increment + 4bytes timestamp */
    CNODE_TYPE_OBJECT,
    CNODE_TYPE_ARRAY,
    CNODE_TYPE_JS,
    CNODE_TYPE_SYMBOL,
    CNODE_TYPE_OID, /**< 12byte ObjectID (uint) */
    
    CNODE_TYPE_POINT = 120,
    CNODE_TYPE_BOX,
    CNODE_TYPE_PATH,
    CNODE_TYPE_TIME,
    CNODE_TYPE_BITOP
} CnodeType;

__END_DECLS
#endif  /* __MTYPE_H__ */
