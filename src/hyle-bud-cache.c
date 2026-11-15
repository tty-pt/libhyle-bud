#include <hyle-bud/hyle-bud.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef __wasm__
#include <hyle-source/hyle_source.h>

int hyle_bud_cache_put(
        const char *dataset_id, const char *query_str, const char *json,
        size_t len)
{
	return hyle_source_cache_put(dataset_id, query_str, json, len);
}

const char *hyle_bud_cache_get(
        const char *dataset_id, const char *query_str, size_t *len_out)
{
	return hyle_source_cache_get(dataset_id, query_str, len_out);
}

void hyle_bud_cache_invalidate(const char *dataset_id)
{
	hyle_source_cache_invalidate(dataset_id);
}

void hyle_bud_cache_clear(void)
{
	hyle_source_cache_clear();
}

size_t hyle_bud_cache_count(const char *dataset_id)
{
	return hyle_source_cache_count(dataset_id);
}

#else

#define HYLE_BUD_CACHE_CAPACITY 64

typedef struct {
	char dataset[64];
	char *query;
	char *json;
	size_t len;
	uint32_t seq;
} hyle_bud_cache_entry_t;

static hyle_bud_cache_entry_t g_bud_cache[HYLE_BUD_CACHE_CAPACITY];
static size_t g_bud_cache_count = 0;
static uint32_t g_bud_cache_seq = 0;

static void bud_entry_free(hyle_bud_cache_entry_t *e)
{
	if (!e)
		return;
	e->dataset[0] = '\0';
	if (e->query) {
		free(e->query);
		e->query = NULL;
	}
	if (e->json) {
		free(e->json);
		e->json = NULL;
	}
	e->len = 0;
	e->seq = 0;
}

int hyle_bud_cache_put(
        const char *dataset_id, const char *query_str, const char *json,
        size_t len)
{
	size_t i;
	size_t target_idx = (size_t)-1;
	uint32_t oldest_seq = UINT32_MAX;
	size_t oldest_idx = 0;

	if (!dataset_id || !dataset_id[0] || !json)
		return -1;
	if (!query_str)
		query_str = "";
	if (len == 0 && json[0])
		len = strlen(json);

	for (i = 0; i < HYLE_BUD_CACHE_CAPACITY; i++) {
		if (g_bud_cache[i].dataset[0] &&
		    strcmp(g_bud_cache[i].dataset, dataset_id) == 0 &&
		    g_bud_cache[i].query &&
		    strcmp(g_bud_cache[i].query, query_str) == 0)
		{
			target_idx = i;
			break;
		}
	}

	if (target_idx == (size_t)-1) {
		for (i = 0; i < HYLE_BUD_CACHE_CAPACITY; i++) {
			if (!g_bud_cache[i].dataset[0]) {
				target_idx = i;
				g_bud_cache_count++;
				break;
			}
			if (g_bud_cache[i].seq < oldest_seq) {
				oldest_seq = g_bud_cache[i].seq;
				oldest_idx = i;
			}
		}
		if (target_idx == (size_t)-1) {
			target_idx = oldest_idx;
			bud_entry_free(&g_bud_cache[target_idx]);
		}
	}

	hyle_bud_cache_entry_t *e = &g_bud_cache[target_idx];
	char *new_query = strdup(query_str);
	char *new_json = malloc(len + 1);
	if (!new_query || !new_json) {
		free(new_query);
		free(new_json);
		return -1;
	}
	memcpy(new_json, json, len);
	new_json[len] = '\0';

	if (e->query)
		free(e->query);
	if (e->json)
		free(e->json);

	snprintf(e->dataset, sizeof(e->dataset), "%s", dataset_id);
	e->query = new_query;
	e->json = new_json;
	e->len = len;
	e->seq = ++g_bud_cache_seq;
	return 0;
}

const char *hyle_bud_cache_get(
        const char *dataset_id, const char *query_str, size_t *len_out)
{
	size_t i;

	if (!dataset_id || !dataset_id[0])
		return NULL;
	if (!query_str)
		query_str = "";

	for (i = 0; i < HYLE_BUD_CACHE_CAPACITY; i++) {
		if (g_bud_cache[i].dataset[0] &&
		    strcmp(g_bud_cache[i].dataset, dataset_id) == 0 &&
		    g_bud_cache[i].query &&
		    strcmp(g_bud_cache[i].query, query_str) == 0)
		{
			g_bud_cache[i].seq = ++g_bud_cache_seq;
			if (len_out)
				*len_out = g_bud_cache[i].len;
			return g_bud_cache[i].json;
		}
	}
	return NULL;
}

void hyle_bud_cache_invalidate(const char *dataset_id)
{
	size_t i;

	for (i = 0; i < HYLE_BUD_CACHE_CAPACITY; i++) {
		if (g_bud_cache[i].dataset[0]) {
			if (!dataset_id || !dataset_id[0] ||
			    strcmp(g_bud_cache[i].dataset, dataset_id) == 0)
			{
				bud_entry_free(&g_bud_cache[i]);
				if (g_bud_cache_count > 0)
					g_bud_cache_count--;
			}
		}
	}
}

void hyle_bud_cache_clear(void)
{
	size_t i;

	for (i = 0; i < HYLE_BUD_CACHE_CAPACITY; i++)
		bud_entry_free(&g_bud_cache[i]);
	g_bud_cache_count = 0;
	g_bud_cache_seq = 0;
}

size_t hyle_bud_cache_count(const char *dataset_id)
{
	size_t i, cnt = 0;

	for (i = 0; i < HYLE_BUD_CACHE_CAPACITY; i++) {
		if (g_bud_cache[i].dataset[0]) {
			if (!dataset_id || !dataset_id[0] ||
			    strcmp(g_bud_cache[i].dataset, dataset_id) == 0)
				cnt++;
		}
	}
	return cnt;
}

#endif
