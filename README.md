# libhyle-bud

[![C99](https://img.shields.io/badge/C-C99-555?logo=c)](#)
[![BSD-2-Clause](https://img.shields.io/badge/License-BSD--2--Clause-blue)](#)
[![Hyle × Bud bridge](https://img.shields.io/badge/bridge-hyle%E2%80%94bud-8B5CF6)](#)

A schema-driven UI bridge that renders Bud HTML components directly from Hyle
canonical data schemas (`hyle_schema_desc_t`). It is the **only** library in the
stack permitted to link both `hyle` and `bud`, and it is the layer every filter,
form, picker, and table on the platform is built through — natively for SSR and,
via the WASM bridge, in the browser.

---

## Contents

- [Features](#features)
- [Build & install](#build--install)
- [Quickstart](#quickstart)
- [API overview](#api-overview)
- [Consumers](#consumers)
- [Documentation](#documentation)
- [Testing](#testing)
- [License](#license)

## Features

**Schema-driven filters** — `hyle_bud_filter` inspects a field in the schema
descriptor and draws the exact control: text search input, boolean switch,
single-reference dropdown, multi-reference facet picker, or omnisearch
omni-dropdown. `hyle_bud_filter_scoped` adds an integer scope for repeated rows,
tables, or cards (`field_0`, `pick_q_field_0`, …); `hyle_bud_filter_group`
builds a whole filter bar from a schema and a list of field names, and
`hyle_bud_filter_from_schema` renders one field with caller-supplied static
options.

**Declarative forms** — `hyle_bud_form` builds a complete HTML POST form from a
schema descriptor: labels, typed inputs, HTML validation attributes, an optional
hidden CSRF token, and submit/cancel buttons. Pass a C record for prefilled
values or `NULL` for an empty form.

**Omni-dropdown pickers** — `hyle_bud_picker_field` and `hyle_bud_action_picker`
wrap the universal picker (trigger + panel + No-JS sibling GET form). Fragment
routes reuse `hyle_bud_picker_slots` (panel reset + summary) and
`hyle_bud_picker_rows` (infinite-scroll option chunks); `hyle_bud_pick_find_active_scope`
and the `hyle_bud_picker_view_collect_*` helpers discover active pickers from the
current query string and schema. All behavior degrades to plain HTML forms with
JavaScript off.

**Tables & row actions** — `hyle_bud_table`, `_header`, `_body`, and
`hyle_bud_table_actions` render sortable listing tables with uniform row
activation; `hyle_bud_pagination` renders paging controls. Row actions use a
stretched overlay (`LINK` opens `/module/{id}`, `SUBMIT` posts a hidden field to
a `form_id` via the HTML5 `form=` attribute — no-JS friendly inside another
form).

**WASM state hydration** — `hyle_bud_state_apply` / `hyle_bud_state_apply_len`
unpack server-rendered `bud-state` JSON into C state structs using zero-copy
stride layouts, and `hyle_bud_state_overlay_from_desc` / `hyle_bud_state_overlay_array`
serialize C structs back to JSON for hydration. `hyle_bud_picker_state_from_json`
/ `_to_json` carry picker state across the wire.

**i18n** — `hyle_bud_set_translator` installs a `const char *(*)(const char *)`
callback; `hyle_bud_tr` routes every UI label through it (identity translation
by default).

**Picker DTOs** — the picker presentation types (`hyle_option_t`,
`hyle_picker_desc_t`, `hyle_picker_entry_t`, `hyle_picker_view_t`,
`hyle_picker_buffer_t`) are *declared* in `<hyle-source/picker.h>`
(libhyle-source); `hyle-bud.h` includes it and re-exports them as
`hyle_bud_*` typedefs for compatibility.

## Build & install

```sh
cd external/libhyle-bud
make          # lib/libhyle-bud.so

sudo make install   # lib, headers, and hyle-bud.pc → $(PREFIX), default /usr/local
```

Link it from your own C code:

```sh
cc my_app.c $(pkg-config --cflags --libs hyle-bud)
```

`hyle-bud.pc` carries the full dependency chain
(`-lhyle-bud -lhyle -lbud -lcorm -ljson-c -lhyle-source`).

**Dependencies:** `external/libhyle` (schemas), `external/libbud` (HTML AST and
bridge), `external/libhyle-source` (picker DTOs + option resolution),
`external/libcorm`, and `json-c`.

**WASM:** when a WASM module needs the bridge, include `hyle-bud-wasm.mk`
(single ownership, L03) — it adds `filter.c`, `table.c`, `picker.c`, `form.c`,
and `libhyle/src/url.c` to the module's sources together with the three include
paths.

> **Note:** libhyle-bud is framework-bound by definition — it is the deliberate
> junction of Hyle (data) and Bud (UI). Everything UI-neutral lives downstream
> in `libhyle` / `libhyle-source`; nothing UI belongs upstream.

## Quickstart

```c
#include <hyle-bud/hyle-bud.h>
#include <bud/bud.h>
#include <stdio.h>

typedef struct {
	char title[256];
	int year;
} album_t;

static const hyle_schema_desc_t album_fields[] = {
	FIELD_TEXT(title, album_t),
	FIELD_INT(year, album_t),
	FIELD_END
};

int main(void)
{
	/* A full POST form from the schema; record/picker/CSRF may be NULL. */
	bud_node *form = hyle_bud_form(
	        album_fields, NULL, "/album/save", "/", "Save",
	        NULL, NULL, NULL);

	char *html = bud_render_html(form);
	printf("%s\n", html);
	bud_free_string(html);
	return 0;
}
```

## API overview

Full signatures live in `include/hyle-bud/hyle-bud.h`.

**State & hydration** — `hyle_bud_state_apply` / `hyle_bud_state_apply_len`,
`hyle_bud_state_overlay_from_desc`, `hyle_bud_state_overlay_array`,
`hyle_bud_picker_state_from_json`, `hyle_bud_picker_state_to_json`.

**Forms & widgets** — `hyle_bud_form`, `hyle_bud_text_input`,
`hyle_bud_filter_field`, `hyle_bud_multiselect_field`,
`hyle_bud_reference_select_dropdown`, `hyle_bud_ms_reset`.

**Filters** — `hyle_bud_filter`, `hyle_bud_filter_from_schema`,
`hyle_bud_filter_scoped`, `hyle_bud_filter_group`.

**Pickers** — `hyle_bud_picker_field`, `hyle_bud_action_picker`
(`hyle_bud_action_picker_spec_t`), `hyle_bud_picker_slots`,
`hyle_bud_picker_rows`, `hyle_bud_pick_find_active_scope`,
`hyle_bud_picker_view_collect_schema`, `_collect_scoped`,
`_collect_auto_fields_schema`.

**Tables** — `hyle_bud_table`, `hyle_bud_table_header`, `hyle_bud_table_body`,
`hyle_bud_table_actions` (`hyle_bud_row_action_t`, `hyle_row_action_kind_t`),
`hyle_bud_pagination`.

**i18n** — `hyle_bud_set_translator`, `hyle_bud_tr`.

## Consumers

| Target | Where |
|--------|-------|
| Site entity UIs (filters, pickers, tables, routes) | site modules, native SSR + WASM |
| Schema Picker front-end rule | `../../docs/PICKERS.md` |
| Picker option resolution backend | `../libhyle-source/` |
| Picker presentation DTOs | `../libhyle-source/include/hyle-source/picker.h` |

## Documentation

- `../../docs/ARCHITECTURE.md` — module graph and the single-boundary rule
- `../../docs/PICKERS.md` — the universal picker / `hyle_bud_filter` contract
- `../../docs/FILTERS.md` — query/filter semantics powering pickers and tables
- `../../docs/C-ISOMORPHIC-BUD.md` — one renderer for SSR + WASM

## Testing

libhyle-bud ships no standalone test binary; it is exercised end-to-end by the
site suite: `make test` at the repository root covers the SSR and E2E paths that
drive filters, forms, and pickers, and `make boundary-check` enforces the
single-link rule (only libhyle-bud may link both `hyle` and `bud`). The WASM
lane is rebuilt and exercised as part of the site's WASM builds.

## License

BSD 2-Clause License. Copyright (c) 2026, tty-pt. See `../../LICENSE`.