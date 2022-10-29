#ifndef RJCP_OS_QNX_NATIVE_FILEHANDLE_TYPE_H
#define RJCP_OS_QNX_NATIVE_FILEHANDLE_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

int  file_create(int success);
void file_close(int filehandle);

#ifdef __cplusplus
}
#endif

#endif
