#ifndef RWLOCK_H
#define RWLOCK_H

#include "atomic.h"

typedef struct rwlock_s {
	atomic_u32_t cnt;
	/* 0 lock not taken
	 * -1 write lock taken
	 * >0 read lock(s) taken */
} rwlock_t;

#define RW_LOCK_UNLOCKED { .cnt = ATOMIC_INIT(0) }

/**
 * Initialize a reader/writer lock.
 *
 * @param rwlock Pointer to a reader/writer lock
 */
void rwlock_init(rwlock_t *rwlock);

/**
 * Acquire read permission on a reader/writer lock.
 *
 * @param rwlock Pointer to a reader/writer lock
 */
void rwlock_read_lock(rwlock_t *rwlock);

/**
 * Try to acquire read permission to a reader/writer lock.
 *
 * @param rwlock Pointer to a reader/writer lock
 *
 * @retval  0 Lock was not available for read access
 * @retval !0 Read access to lock acquired
 */
int rwlock_read_trylock(rwlock_t *rwlock);

/**
 * Release read permission on a reader/writer lock.
 *
 * @param rwlock Pointer to a reader/writer lock
 */
void rwlock_read_unlock(rwlock_t *rwlock);

/**
 * Acquire write permission on a reader/writer lock.
 *
 * @param rwlock Pointer to a reader/writer lock
 */
void rwlock_write_lock(rwlock_t *rwlock);

/**
 * Try to acquire write permission to a reader/writer lock.
 *
 * @param rwlock Pointer to a reader/writer lock
 *
 * @retval  0 Lock was not available for write access
 * @retval !0 Write access to lock acquired
 */
int rwlock_write_trylock(rwlock_t *rwlock);

/**
 * Release write permission on a reader/writer lock.
 *
 * @param rwlock Pointer to a reader/writer lock
 */
void rwlock_write_unlock(rwlock_t *rwlock);

#endif /* RWLOCK_H */
