# libhyle-bud

[![C99](https://img.shields.io/badge/C-C99-555?logo=c)](#)
[![BSD-2-Clause](https://img.shields.io/badge/License-BSD--2--Clause-blue)](#)
[![Hyle × Bud bridge](https://img.shields.io/badge/bridge-hyle%E2%80%94bud-8B5CF6)](#)

> Schema-driven UI bridge rendering Bud HTML components directly from Hyle canonical data schemas (`hyle_schema_desc_t`). Canonical header is `<hyle-bud/hyle-bud.h>`.

`libhyle-bud` is the designated junction between `libhyle` (data schemas) and `libbud` (declarative UI). It powers every filter, form, omni-picker, and sortable table across native SSR and browser WASM.

---

## 1. Quickstart

Render a complete HTML form directly from a C schema descriptor:

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

int main(void) {
    // Generate full POST form from schema descriptor
    bud_node *form = hyle_bud_form(
        album_fields, NULL, "/album/save", "/", "Save",
        NULL, NULL, NULL
    );

    char *html = bud_render_html(form);
    printf("%s\n", html);

    bud_free_string(html);
    bud_free(form);
    return 0;
}
```

Compile and link:
```sh
cc form_demo.c $(pkg-config --cflags --libs hyle-bud) -o form_demo
```

---

## 2. Component Architecture & Progressive Enhancement

`libhyle-bud` renders rich UI controls that degrade gracefully to standard HTML5:

```text
┌────────────────────────────────────────────────────────────────────────┐
│ Filter Bar (hyle_bud_filter_group)                                     │
│ [ Search text... ]  [ Category: All ▼ ]  [ Status: Active ▼ ]  [Apply] │
└────────────────────────────────────────────────────────────────────────┘
┌────────────────────────────────────────────────────────────────────────┐
│ Listing Table (hyle_bud_table)                                         │
│ Title ▲                     Year      Status        Actions            │
│ ────────────────────────────────────────────────────────────────────── │
│ Ain't Talkin' 'Bout Love    1978      Published     [Edit] [Delete]    │
│ Jump                        1984      Published     [Edit] [Delete]    │
└────────────────────────────────────────────────────────────────────────┘
```

- **No-JS Full Compatibility**: Filters submit plain GET forms, pagination uses URL query strings, and mutations submit standard POST bodies.
- **WASM Interactive Layer**: When browser WASM executes, event listeners attach directly to existing server-rendered HTML elements, streaming fine-grained DOM patch operations without full page reloads.

---

## 3. Core API Reference

| Component | Key Functions | Description |
| :--- | :--- | :--- |
| **Forms & Inputs** | `hyle_bud_form`, `hyle_bud_text_input`, `hyle_bud_multiselect_field` | Generate complete POST forms and typed inputs from schema descriptors |
| **Filters & Groups** | `hyle_bud_filter`, `hyle_bud_filter_group`, `hyle_bud_filter_scoped` | Render schema-driven search inputs, dropdowns, and multi-filter bars |
| **Listing Tables** | `hyle_bud_table`, `hyle_bud_table_header`, `hyle_bud_table_actions`, `hyle_bud_pagination` | Sortable tables with uniform row activation, link/submit actions, and paging |
| **Omni-Pickers** | `hyle_bud_picker_field`, `hyle_bud_action_picker`, `hyle_bud_picker_slots` | Faceted search dropdowns with infinite-scroll option chunking |
| **State Hydration** | `hyle_bud_state_apply`, `hyle_bud_state_overlay_from_desc` | Unpack `bud-state` JSON into C memory with zero-copy stride layouts |

👉 *For detailed picker conventions, see [docs/PICKERS.md](../../docs/PICKERS.md).*

---

## 4. Installation & Build

### Prebuilt Packages
Prebuilt packages are distributed on tty.pt for Linux (APT / Alpine / Arch / Fedora-RHEL), macOS (Homebrew), Windows (winget / MSYS2), and OpenBSD. Follow the [installation instructions](https://github.com/tty-pt/ci/blob/main/docs/install.md) and use **libhyle-bud** as the package name.

### Build from Source
```sh
make                  # builds lib/libhyle-bud.so
make test             # run verification tests
sudo make install     # install headers and library to system
```

## License

BSD 2-Clause License. Copyright (c) 2026, tty-pt. See `LICENSE`.
