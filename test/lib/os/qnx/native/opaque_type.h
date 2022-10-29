#ifndef RJCP_OS_QNX_NATIVE_OPAQUE_TYPE_H
#define RJCP_OS_QNX_NATIVE_OPAQUE_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct opaque_type opaque_type_t;

opaque_type_t* opaque_create();
void           opaque_destroy(opaque_type_t *value);

#ifdef __cplusplus
}
#endif

#endif
