# @tty-pt/hyle-bud

[![npm version](https://img.shields.io/npm/v/@tty-pt/hyle-bud.svg)](https://www.npmjs.com/package/@tty-pt/hyle-bud)
[![License: BSD-2-Clause](https://img.shields.io/badge/License-BSD--2--Clause-blue)](https://github.com/tty-pt/libhyle-bud/blob/main/LICENSE)
[![TypeScript Ready](https://img.shields.io/badge/TypeScript-Ready-3178C6?logo=typescript)](#)
[![WebAssembly](https://img.shields.io/badge/WebAssembly-Bridge-654FF0?logo=webassembly)](#)
[![Zero Dependencies](https://img.shields.io/badge/dependencies-0-success)](#)

> Universal Node.js and Browser bindings for **`libhyle-bud`**, the schema-to-Bud UI bridge rendering sortable tables, filter bars, omni-pickers, and SSR/WASM state overlays directly from Hyle canonical data schemas (`hyle_schema_desc_t`). Canonical C header: `<hyle-bud/hyle-bud.h>`.

📖 **Full C & UI Bridge Documentation**: See the main [**libhyle-bud GitHub README**](https://github.com/tty-pt/libhyle-bud#readme) for C form/table/picker builders (`hyle_bud_form`, `hyle_bud_table`, `hyle_bud_picker_field`) and progressive enhancement contracts.

---

## 1. Overview & Domain Concepts

`libhyle-bud` (`@tty-pt/hyle-bud`) is the sole bridge between [`@tty-pt/hyle`](https://www.npmjs.com/package/@tty-pt/hyle) (canonical data schemas) and [`@tty-pt/bud`](https://www.npmjs.com/package/@tty-pt/bud) (declarative VDOM & HTML builder):

1. **Schema-Driven Table & Form Binding (`bindSchemaTable`)**:
   - Maps declarative Hyle field descriptors (`hyle_schema_desc_t` / `FieldDescriptor[]`) and record rows into structured table headers and normalized cell rows without duplicating field labels or formatting logic.
2. **Progressive Enhancement (No-JS SSR $\rightarrow$ Browser WASM)**:
   - Emits plain accessible HTML `<form>`, `<table>`, and `<select>` markup during server-side rendering, then hydrates interactive sorting, faceted filtering, and omni-dropdown pickers in the browser.
3. **Fragment Cache & Web HAL Capability Bridge (`dist/hyle-fragments.js`, `dist/hyle-cap.js`)**:
   - Provides client-side HTML fragment caching, optimistic table/filter patching, and registers `hyle.*` host capabilities on [`@tty-pt/stele`](https://www.npmjs.com/package/@tty-pt/stele).

---

## 2. Installation

```bash
npm install @tty-pt/hyle-bud
```

**Native OS Packages (`tty.pt`)**: Prebuilt native C packages (`libhyle-bud`) for Linux (`apt`, `apk`, `pacman`, `dnf`), macOS (`brew`), Windows (`winget`, MSYS2), and OpenBSD (`pkg_add`) are available via the [**tty.pt package repositories**](https://github.com/tty-pt/ci/blob/main/docs/install.md).

---

## 3. Quickstart

```typescript
import { bindSchemaTable } from "@tty-pt/hyle-bud";

const songSchema = [
  { key: "title", label: "Song Title" },
  { key: "author", label: "Composer" },
  { key: "year", label: "Year" }
];

const table = bindSchemaTable(songSchema, [
  { title: "Ave Maria", author: "Franz Schubert", year: 1825 },
  { title: "Panis Angelicus", author: "César Franck", year: 1872 }
]);

console.log("Headers:", table.headers);
console.log("Rows:", table.rows);
```

---

## 4. JavaScript / TypeScript API Reference

### `bindSchemaTable(schemaFields: FieldDescriptor[], rows: Record<string, any>[]): TableBinding`
Binds canonical Hyle schema field definitions (`key`/`name` and `label`) and raw record objects into normalized tabular presentation data:
- Returns `{ headers: Array<{ key: string; label: string }>, rows: Array<Record<string, string>> }`.

### Browser Fragment & Capability Modules
- `@tty-pt/hyle-bud/fragments` (`dist/hyle-fragments.js`): Client-side fragment cache and slot state synchronization for Hyle tables, filters, and pickers.
- `@tty-pt/hyle-bud/cap` (`dist/hyle-cap.js`): Registers Web HAL `hyle.*` capabilities on `@tty-pt/stele`'s `CapabilityRegistry`.
- `version(): string`: Returns the library semantic version string.

---

## 5. C Development Headers & Native Integration

```makefile
CFLAGS += -I./node_modules/@tty-pt/hyle-bud/include
LDLIBS += -lhyle-bud
```

---

## License

BSD-2-Clause — Copyright (c) 2026, Paulo Quirino <quirinpa@gmail.com>. See [LICENSE](https://github.com/tty-pt/libhyle-bud/blob/main/LICENSE).
