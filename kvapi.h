
/*
 * KVAPI
 */

struct kvs_handle;
typedef struct kvs_handle * kvs_handle_t;
typedef int kvs_error_t;
struct kvs_future;
typedef struct kvs_future *kvs_future_t;
typedef u_int8_t *kvs_data_t;
typedef u_int32_t kvs_len_t;
void		kvs_free(kvs_data_t foo);

struct kvs_caps {
	int	KVS_CAN_GET		: 1;
	int	KVS_CAN_PUT		: 1;
	int	KVS_CAN_REKEY		: 1;
	int	KVS_CAN_APPEND		: 1;
	int	KVS_CAN_PREPEND		: 1;
	int	KVS_CAN_ASYNCIO		: 1;
	int	KVS_CAN_COMPARE_AND_SWAP: 1;
	int	KVS_CAN_VERIFY		: 1;
	int	KVS_CAN_BATCH		: 1;
	int	KVS_CAN_BATCHX		: 1;
	int	KVS_CAN_OPENX		: 1;
	int	KVS_CAN_2PC		: 1;
};


kvs_error_t	kvstore_open(char *uri, kvs_handle_t *newhandle);
kvs_error_t	kvs_close(kvs_handle_t handle);
struct kvs_caps	kvs_capability(kvs_handle_t kvstore);

int		kvfuture_done(kvs_future_t future);
kvs_error_t	kvfuture_error(kvs_future_t future);
kvs_error_t	kvfuture_join(kvs_future_t future);
void		kvfuture_free(kvs_future_t future);
void		kvs_free_futures(kvs_handle_t *handle);

/*
 * Meta-data manipulation
 */
// find key in meta data
char		*kvs_meta_get(kvs_data_t meta, kvs_len_t mlen, char *metakey);
// create or update meta data
kvs_data_t	kvs_meta_put(kvs_data_t meta, kvs_len_t mlen, kvs_len_t *newlen,
			char *newkey, char *newval);


/*
 * Batched operations
 * Even if the batch is synchronous
 * Only async operations go in a batch
 */
kvs_error_t	kvstore_batch_begin(kvs_handle_t storehandle, kvs_handle_t *batchhandle);
kvs_error_t 	kvs_sync_batch_submit(kvs_handle_t batchhandle);
kvs_future_t 	kvs_batch_submit(kvs_handle_t batchhandle);
kvs_error_t 	kvs_sync_batch_abort(kvs_handle_t batchhandle);
kvs_future_t 	kvs_batch_abort(kvs_handle_t batchhandle);

/*
 * Batched, closed transactions
 * Only async operations go in a batched/closed transaction
 */
kvs_error_t	kvstore_batchx_begin(kvs_handle_t storehandle, kvs_handle_t *batchhandle);
kvs_error_t 	kvs_sync_batchx_commit(kvs_handle_t batchhandle);
kvs_future_t 	kvs_batchx_commit(kvs_handle_t batchhandle);
kvs_error_t 	kvs_sync_batchx_abort(kvs_handle_t batchhandle);
kvs_future_t 	kvs_batchx_abort(kvs_handle_t batchhandle);


/*
 * Open transactions
 * Can handle sync or async operations
 */
kvs_error_t	kvstore_openx_begin(kvs_handle_t storehandle, kvs_handle_t *newhandle);
kvs_error_t 	kvs_sync_openx_commit(kvs_handle_t batchhandle);
kvs_future_t 	kvs_openx_commit(kvs_handle_t batchhandle);
kvs_error_t 	kvs_sync_openx_abort(kvs_handle_t batchhandle);
kvs_future_t 	kvs_openx_abort(kvs_handle_t batchhandle);

/*
 * 2 Phase Commit transactions
 */
kvs_error_t	kvstore_2pcx_begin(kvs_handle_t storehandle, kvs_handle_t *newhandle);
kvs_error_t 	kvs_sync_2pcx_abort(kvs_handle_t batchhandle);		// only before prep_commit
kvs_future_t 	kvs_2pcx_abort(kvs_handle_t batchhandle);		// only before prep_commit
kvs_error_t 	kvs_sync_2pcx_prep_commit(kvs_handle_t batchhandle);
kvs_future_t 	kvs_2pcx_prep_commit(kvs_handle_t batchhandle);
kvs_error_t 	kvs_sync_2pcx_do_commit(kvs_handle_t batchhandle);
kvs_future_t 	kvs_2pcx_do_commit(kvs_handle_t batchhandle);
kvs_error_t 	kvs_sync_2pcx_rollback(kvs_handle_t batchhandle);
kvs_future_t 	kvs_2pcx_rollback(kvs_handle_t batchhandle);

/*
 * Take or open a snapshot
 */
kvs_error_t	kvstore_snapshot(kvs_handle_t storehandle, kvs_handle_t *newhandle, time_t stamp);

/*
 * Get
 */
kvs_error_t	kvs_sync_get(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta_in, kvs_len_t mlen,
			kvs_data_t *meta_out, kvs_len_t *meta_len,
			kvs_data_t *value, kvs_len_t *vlen);
kvs_future_t	kvs_get(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta_in, kvs_len_t mlen,
			kvs_data_t *meta_out, kvs_len_t *meta_len,
			kvs_data_t *value, kvs_len_t *vlen);

/*
 * Get Next, Get Next Bulk
 */
kvs_error_t	kvs_sync_get_next(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t *keyout, kvs_len_t *keyout_len,
			kvs_data_t *metaout, kvs_len_t *metaout_len,
			kvs_data_t *value, kvs_len_t *value_len);
kvs_future_t	kvs_get_next(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t *keyout, kvs_len_t *keyout_len,
			kvs_data_t *metaout, kvs_len_t *metaout_len,
			kvs_data_t *value, kvs_len_t *value_len);
/* must pre-allocate output list arrays */
kvs_error_t	kvs_sync_get_next_bulk(kvs_handle_t handle,
			int maxkeys, int *actualkeys,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t *keylist, kvs_len_t *keylens,
			kvs_data_t *metalist, kvs_len_t *metalens,
			kvs_data_t *vallist, kvs_len_t *vallens);
kvs_future_t	kvs_get_next_bulk(kvs_handle_t handle,
			int maxkeys, int *actualkeys,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t *keylist, kvs_len_t *keylens,
			kvs_data_t *metalist, kvs_len_t *metalens,
			kvs_data_t *vallist, kvs_len_t *vallens);

/*
 * Get Range
 * meta == NULL means start with minkey
 * meta != NULL means starts after minkey
 * minkey == NULL means first
 * maxkey == NULL means last
 * must pre-allocate output list arrays
 */
kvs_error_t	kvs_sync_get_range(kvs_handle_t handle,
			int maxkeys, int *actualkeys,
			kvs_data_t minkey, kvs_len_t minkey_len,
			kvs_data_t minkey_meta, kvs_len_t minkeymeta_len,
			kvs_data_t maxkey, kvs_len_t maxkey_len,
			kvs_data_t *keylist, kvs_len_t *keylens,
			kvs_data_t *metalist, kvs_len_t *metalens,
			kvs_data_t *vallist, kvs_len_t *vallens);
kvs_future_t	kvs_get_range(kvs_handle_t handle,
			int maxkeys, int *actualkeys,
			kvs_data_t minkey, kvs_len_t minkey_len,
			kvs_data_t minkey_meta, kvs_len_t minkeymeta_len,
			kvs_data_t maxkey, kvs_len_t maxkey_len,
			kvs_data_t *keylist, kvs_len_t *keylens,
			kvs_data_t *metalist, kvs_len_t *metalens,
			kvs_data_t *vallist, kvs_len_t *vallens);


/*
 * Put, Create, Replace
 */
kvs_error_t	kvs_sync_put(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
		       	kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t value, kvs_len_t vlen);
kvs_future_t	kvs_put(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
		       	kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t value, kvs_len_t vlen);
kvs_error_t	kvs_sync_create(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
		       	kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t value, kvs_len_t vlen);
kvs_future_t	kvs_create(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
		       	kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t value, kvs_len_t vlen);
kvs_error_t	kvs_sync_replace(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
		       	kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t value, kvs_len_t vlen);
kvs_future_t	kvs_replace(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
		       	kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t value, kvs_len_t vlen);

/*
 * Put Bulk
 * probably not needed if batching works
 */
kvs_error_t	kvs_sync_put_bulk(kvs_handle_t handle, int keycount,
			kvs_data_t *keylist, kvs_len_t *keylens,
			kvs_data_t *metalist, kvs_len_t *metalens,
			kvs_data_t *vallist, kvs_len_t *vallens);
kvs_future_t	kvs_put_bulk(kvs_handle_t handle, int keycount,
			kvs_data_t *keylist, kvs_len_t *keylens,
			kvs_data_t *metalist, kvs_len_t *metalens,
			kvs_data_t *vallist, kvs_len_t *vallens);

/*
 * Delete
 */
kvs_error_t	kvs_sync_delete(kvs_handle_t handle, kvs_data_t key, kvs_len_t klen);
kvs_future_t	kvs_delete(kvs_handle_t handle, kvs_data_t key, kvs_len_t klen);

/* 
 * Compare And Swap
 */
kvs_error_t	kvs_sync_compare_and_swap(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta_old, kvs_len_t meta_old_len,
			kvs_data_t meta_new, kvs_len_t meta_new_len,
			kvs_data_t value, kvs_len_t vlen);
kvs_future_t	kvs_compare_and_swap(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta_old, kvs_len_t meta_old_len,
			kvs_data_t meta_new, kvs_len_t meta_new_len,
			kvs_data_t value, kvs_len_t vlen);

/* 
 * Verify - Checks that <key,meta> is lastest for <key>
 */
kvs_error_t	kvs_sync_verify(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta, kvs_len_t mlen);
kvs_future_t	kvs_verify(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta, kvs_len_t mlen);

/* 
 * Rekey
 * If meta, verify and rekey
 */
kvs_error_t	kvs_sync_rekey(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t newkey, kvs_len_t newkeylen);
kvs_future_t	kvs_rekey(kvs_handle_t handle,
			kvs_data_t key, kvs_len_t klen,
			kvs_data_t meta, kvs_len_t mlen,
			kvs_data_t newkey, kvs_len_t newkeylen);
