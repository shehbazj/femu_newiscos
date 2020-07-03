#ifndef __FEMU_MEM_BACKEND__
#define __FEMU_MEM_BACKEND__

#include <stdint.h>


/* Coperd: FEMU memory backend structure */
struct femu_mbe {
    void *mem_backend;
    int64_t size; /* in bytes */
    int femu_mode;
	int computation_mode;
	int nscdelay_mode;
};

void femu_init_mem_backend(struct femu_mbe *mbe, int64_t nbytes);
void femu_destroy_mem_backend(struct femu_mbe *mbe);
int femu_rw_mem_backend_bb(struct femu_mbe *mbe, QEMUSGList *qsg,
        uint64_t data_offset, bool is_write, int computational_fd_send, int computational_fd_recv);
int femu_rw_mem_backend_oc(struct femu_mbe *mbe, QEMUSGList *qsg,
        uint64_t *data_offset, bool is_write);

#endif
